#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "attr.h"
#شامل "object.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "delta.h"
#شامل "pack.h"
#شامل "pack-revindex.h"
#شامل "csum-file.h"
#شامل "tree-walk.h"
#شامل "diff.h"
#شامل "revision.h"
#شامل "list-objects.h"
#شامل "list-objects-filter-options.h"
#شامل "pack-objects.h"
#شامل "progress.h"
#شامل "refs.h"
#شامل "thread-utils.h"
#شامل "pack-bitmap.h"
#شامل "delta-islands.h"
#شامل "reachable.h"
#شامل "oid-array.h"
#شامل "strvec.h"
#شامل "strmap.h"
#شامل "list.h"
#شامل "packfile.h"
#شامل "object-file.h"
#شامل "object-file-convert.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "replace-object.h"
#شامل "dir.h"
#شامل "midx.h"
#شامل "trace2.h"
#شامل "shallow.h"
#شامل "promisor-remote.h"
#شامل "pack-mtimes.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "blob.h"
#شامل "tree.h"
#شامل "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
ساکن ساخت packing_ڈیٹا مقصد_pack;

ساکن اندرونی ساخت object_اندراج *oe_delta(
		ثابت ساخت packing_ڈیٹا *pack,
		ثابت ساخت object_اندراج *e)
{
	اگر (!e->delta_idx)
		واپس تہی;
	اگر (e->ext_بنیاد)
		واپس &pack->ext_bases[e->delta_idx - 1];
	ورنہ
		واپس &pack->objects[e->delta_idx - 1];
}

ساکن اندرونی سائز_t oe_delta_سائز(ساخت packing_ڈیٹا *pack,
				   ثابت ساخت object_اندراج *e)
{
	اگر (e->delta_سائز_valid)
		واپس e->delta_سائز;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	واپس pack->delta_سائز_2[e - pack->objects];
}

سائز_t oe_get_سائز_slow(ساخت packing_ڈیٹا *pack,
			ثابت ساخت object_اندراج *e);

ساکن اندرونی سائز_t oe_سائز(ساخت packing_ڈیٹا *pack,
			     ثابت ساخت object_اندراج *e)
{
	اگر (e->سائز_valid)
		واپس e->سائز_2;

	واپس oe_get_سائز_slow(pack, e);
}

ساکن اندرونی خالی oe_سیٹ_delta(ساخت packing_ڈیٹا *pack,
				ساخت object_اندراج *e,
				ساخت object_اندراج *delta)
{
	اگر (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	ورنہ
		e->delta_idx = 0;
}

ساکن اندرونی ساخت object_اندراج *oe_delta_sibling(
		ثابت ساخت packing_ڈیٹا *pack,
		ثابت ساخت object_اندراج *e)
{
	اگر (e->delta_sibling_idx)
		واپس &pack->objects[e->delta_sibling_idx - 1];
	واپس تہی;
}

ساکن اندرونی ساخت object_اندراج *oe_delta_بچہ(
		ثابت ساخت packing_ڈیٹا *pack,
		ثابت ساخت object_اندراج *e)
{
	اگر (e->delta_بچہ_idx)
		واپس &pack->objects[e->delta_بچہ_idx - 1];
	واپس تہی;
}

ساکن اندرونی خالی oe_سیٹ_delta_بچہ(ساخت packing_ڈیٹا *pack,
				      ساخت object_اندراج *e,
				      ساخت object_اندراج *delta)
{
	اگر (delta)
		e->delta_بچہ_idx = (delta - pack->objects) + 1;
	ورنہ
		e->delta_بچہ_idx = 0;
}

ساکن اندرونی خالی oe_سیٹ_delta_sibling(ساخت packing_ڈیٹا *pack,
					ساخت object_اندراج *e,
					ساخت object_اندراج *delta)
{
	اگر (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	ورنہ
		e->delta_sibling_idx = 0;
}

ساکن اندرونی خالی oe_سیٹ_سائز(ساخت packing_ڈیٹا *pack,
			       ساخت object_اندراج *e,
			       سائز_t سائز)
{
	اگر (سائز < pack->oe_سائز_حد_بندی) {
		e->سائز_2 = سائز;
		e->سائز_valid = 1;
	} ورنہ {
		e->سائز_valid = 0;
		اگر (oe_get_سائز_slow(pack, e) != سائز)
			BUG("'size' is supposed to be the object size!");
	}
}

ساکن اندرونی خالی oe_سیٹ_delta_سائز(ساخت packing_ڈیٹا *pack,
				     ساخت object_اندراج *e,
				     سائز_t سائز)
{
	اگر (سائز < pack->oe_delta_سائز_حد_بندی) {
		e->delta_سائز = سائز;
		e->delta_سائز_valid = 1;
	} ورنہ {
		packing_ڈیٹا_تالا(pack);
		اگر (!pack->delta_سائز_2)
			ALLOC_ARRAY(pack->delta_سائز_2, pack->nr_alloc);
		packing_ڈیٹا_unlock(pack);

		pack->delta_سائز_2[e - pack->objects] = سائز;
		e->delta_سائز_valid = 0;
	}
}

#تعریف_کرو اندر_PACK(obj) oe_اندر_pack(&مقصد_pack, obj)
#تعریف_کرو سائز_3(obj) oe_سائز(&مقصد_pack, obj)
#تعریف_کرو سیٹ_سائز(obj,سائز) oe_سیٹ_سائز(&مقصد_pack, obj, سائز)
#تعریف_کرو DELTA_سائز(obj) oe_delta_سائز(&مقصد_pack, obj)
#تعریف_کرو DELTA(obj) oe_delta(&مقصد_pack, obj)
#تعریف_کرو DELTA_بچہ(obj) oe_delta_بچہ(&مقصد_pack, obj)
#تعریف_کرو DELTA_SIBLING(obj) oe_delta_sibling(&مقصد_pack, obj)
#تعریف_کرو سیٹ_DELTA(obj, val) oe_سیٹ_delta(&مقصد_pack, obj, val)
#تعریف_کرو سیٹ_DELTA_EXT(obj, oid) oe_سیٹ_delta_ext(&مقصد_pack, obj, oid)
#تعریف_کرو سیٹ_DELTA_سائز(obj, val) oe_سیٹ_delta_سائز(&مقصد_pack, obj, val)
#تعریف_کرو سیٹ_DELTA_بچہ(obj, val) oe_سیٹ_delta_بچہ(&مقصد_pack, obj, val)
#تعریف_کرو سیٹ_DELTA_SIBLING(obj, val) oe_سیٹ_delta_sibling(&مقصد_pack, obj, val)

ساکن ثابت حرف *ثابت pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	تہی
};

ساکن ساخت pack_idx_اندراج **written_list;
ساکن uint32_t nr_نتیجہ, nr_written, nr_نظر_آیا;
ساکن ساخت bitmap_اشاریہ *bitmap_git;
ساکن uint32_t write_layer;

ساکن صحیح non_خالی_خانے;
ساکن صحیح reuse_delta = 1, reuse_object = 1;
ساکن صحیح keep_unreachable, unpack_unreachable, include_لیبل;
ساکن timestamp_t unpack_unreachable_expiration;
ساکن صحیح pack_loose_unreachable;
ساکن صحیح cruft;
ساکن صحیح shallow = 0;
ساکن timestamp_t cruft_expiration;
ساکن صحیح local;
ساکن صحیح have_non_local_packs;
ساکن صحیح incremental;
ساکن صحیح ignore_packed_keep_on_disk;
ساکن صحیح ignore_packed_keep_اندر_core;
ساکن صحیح ignore_packed_keep_اندر_core_open;
ساکن صحیح ignore_packed_keep_اندر_core_has_cruft;
ساکن صحیح allow_ofs_delta;
ساکن ساخت pack_idx_option pack_idx_opts;
ساکن ثابت حرف *بنیاد_name;
ساکن صحیح پیش_رفت = 1;
ساکن صحیح کھڑکی = 10;
ساکن غیرعلامتی طویل pack_سائز_حد_بندی;
ساکن صحیح گہرائی = 50;
ساکن صحیح delta_تلاش_دھاگے;
ساکن صحیح pack_مقصد_stdout;
ساکن صحیح sparse;
ساکن صحیح thin;
ساکن صحیح راستہ_walk = -1;
ساکن صحیح عدد_preferred_بنیاد;
ساکن ساخت پیش_رفت *پیش_رفت_state;

ساکن ساخت bitmapped_pack *reuse_packfiles;
ساکن سائز_t reuse_packfiles_nr;
ساکن سائز_t reuse_packfiles_مستعمل_nr;
ساکن uint32_t reuse_packfile_objects;
ساکن ساخت bitmap *reuse_packfile_bitmap;

ساکن صحیح use_bitmap_اشاریہ_default = 1;
ساکن صحیح use_bitmap_اشاریہ = -1;
ساکن شمار {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ساکن شمار {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_اشاریہ;
ساکن uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ساکن صحیح exclude_promisor_objects;
ساکن صحیح exclude_promisor_objects_بہترین_effort;

ساکن صحیح use_delta_islands;

ساکن سائز_t delta_cache_سائز = 0;
ساکن سائز_t max_delta_cache_سائز = DEFAULT_DELTA_CACHE_سائز;
ساکن غیرعلامتی طویل cache_max_small_delta_سائز = 1000;

ساکن غیرعلامتی طویل کھڑکی_حافظہ_حد_بندی = 0;

ساکن ساخت string_list uri_protocols = STRING_LIST_INIT_NODUP;

شمار missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساکن شمار missing_action arg_missing_action;
ساکن show_object_fn fn_show_object;

ساخت configured_exclusion {
	ساخت oidmap_اندراج e;
	حرف *pack_hash_hex;
	حرف *uri;
};
ساکن ساخت oidmap configured_exclusions;

ساکن ساخت oidset excluded_by_config;
ساکن صحیح name_hash_نسخہ = -1;

شمار stdin_packs_منوال {
	STDIN_PACKS_منوال_NONE,
	STDIN_PACKS_منوال_STANDARD,
	STDIN_PACKS_منوال_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ساکن خالی validate_name_hash_نسخہ(خالی)
{
	اگر (name_hash_نسخہ < 1 || name_hash_نسخہ > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_نسخہ);
	اگر (write_bitmap_اشاریہ && name_hash_نسخہ != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_نسخہ = 1;
	}
}

ساکن اندرونی uint32_t pack_name_hash_fn(ثابت حرف *name)
{
	ساکن صحیح نظر_آیا_نسخہ = -1;

	اگر (نظر_آیا_نسخہ < 0)
		نظر_آیا_نسخہ = name_hash_نسخہ;
	ورنہ اگر (نظر_آیا_نسخہ != name_hash_نسخہ)
		BUG("name hash version changed from %d to %d mid-process",
		    نظر_آیا_نسخہ, name_hash_نسخہ);

	انتخاب (name_hash_نسخہ) {
	حالت 1:
		واپس pack_name_hash(name);

	حالت 2:
		واپس pack_name_hash_v2((ثابت غیرعلامتی حرف *)name);

	طےشدہ:
		BUG("invalid name-hash version: %d", name_hash_نسخہ);
	}
}

/*
 * stats
 */
ساکن uint32_t written, written_delta;
ساکن uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
ساکن ساخت commit **اشاریہ_دار_commits;
ساکن غیرعلامتی صحیح اشاریہ_دار_commits_nr;
ساکن غیرعلامتی صحیح اشاریہ_دار_commits_alloc;

ساکن خالی اشاریہ_commit_for_bitmap(ساخت commit *commit)
{
	اگر (اشاریہ_دار_commits_nr >= اشاریہ_دار_commits_alloc) {
		اشاریہ_دار_commits_alloc = (اشاریہ_دار_commits_alloc + 32) * 2;
		REALLOC_ARRAY(اشاریہ_دار_commits, اشاریہ_دار_commits_alloc);
	}

	اشاریہ_دار_commits[اشاریہ_دار_commits_nr++] = commit;
}

ساکن خالی *get_delta(ساخت object_اندراج *اندراج_2)
{
	غیرعلامتی طویل سائز, بنیاد_سائز;
	سائز_t delta_سائز_2;
	خالی *buf, *بنیاد_buf, *delta_buf;
	شمار object_type type;
	سائز_t سائز_st = 0, بنیاد_سائز_st = 0;

	buf = odb_read_object(the_repository->objects, &اندراج_2->idx.oid,
			      &type, &سائز_st);
	سائز = cast_سائز_t_مقصد_ulong(سائز_st);
	اگر (!buf)
		die(_("unable to read %s"), oid_مقصد_hex(&اندراج_2->idx.oid));
	بنیاد_buf = odb_read_object(the_repository->objects,
				   &DELTA(اندراج_2)->idx.oid, &type,
				   &بنیاد_سائز_st);
	بنیاد_سائز = cast_سائز_t_مقصد_ulong(بنیاد_سائز_st);
	اگر (!بنیاد_buf)
		die("unable to read %s",
		    oid_مقصد_hex(&DELTA(اندراج_2)->idx.oid));
	delta_buf = diff_delta(بنیاد_buf, بنیاد_سائز,
			       buf, سائز, &delta_سائز_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	اگر (!delta_buf || delta_سائز_2 != DELTA_سائز(اندراج_2))
		BUG("delta size changed");
	آزاد(buf);
	آزاد(بنیاد_buf);
	واپس delta_buf;
}

ساکن غیرعلامتی طویل do_compress(خالی **pptr, غیرعلامتی طویل سائز)
{
	git_zstream دھارا;
	خالی *اندر, *برآمد;
	غیرعلامتی طویل maxsize;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	git_deflate_init(&دھارا, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&دھارا, سائز);

	اندر = *pptr;
	برآمد = xmalloc(maxsize);
	*pptr = برآمد;

	دھارا.اگلا_اندر = اندر;
	دھارا.avail_اندر = سائز;
	دھارا.اگلا_برآمد = برآمد;
	دھارا.avail_برآمد = maxsize;
	جب (git_deflate(&دھارا, Z_اختتام) == Z_OK)
		; /* nothing */
	git_deflate_انتہا(&دھارا);

	آزاد(اندر);
	واپس دھارا.کل_برآمد;
}

ساکن غیرعلامتی طویل write_large_blob_ڈیٹا(ساخت odb_دھارا *st, ساخت hashfile *f,
					   ثابت ساخت object_id *oid)
{
	git_zstream دھارا;
	غیرعلامتی حرف ibuf[1024 * 16];
	غیرعلامتی حرف obuf[1024 * 16];
	غیرعلامتی طویل olen = 0;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	git_deflate_init(&دھارا, cfg->pack_compression_level);

	برائے (;;) {
		ssize_t readlen;
		صحیح zret = Z_OK;
		readlen = odb_دھارا_read(st, ibuf, حجم(ibuf));
		اگر (readlen == -1)
			die(_("unable to read %s"), oid_مقصد_hex(oid));

		دھارا.اگلا_اندر = ibuf;
		دھارا.avail_اندر = readlen;
		جب ((دھارا.avail_اندر || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			دھارا.اگلا_برآمد = obuf;
			دھارا.avail_برآمد = حجم(obuf);
			zret = git_deflate(&دھارا, readlen ? 0 : Z_اختتام);
			hashwrite(f, obuf, دھارا.اگلا_برآمد - obuf);
			olen += دھارا.اگلا_برآمد - obuf;
		}
		اگر (دھارا.avail_اندر)
			die(_("deflate error (%d)"), zret);
		اگر (readlen == 0) {
			اگر (zret != Z_دھارا_انتہا)
				die(_("deflate error (%d)"), zret);
			توڑو;
		}
	}
	git_deflate_انتہا(&دھارا);
	واپس olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ساکن صحیح check_pack_inflate(ساخت packed_git *p,
		ساخت pack_کھڑکی **w_curs,
		off_t انحراف,
		off_t len,
		سائز_t expect)
{
	git_zstream دھارا;
	غیرعلامتی حرف fakebuf[4096], *اندر;
	صحیح st;

	memset(&دھارا, 0, حجم(دھارا));
	git_inflate_init(&دھارا);
	کرو {
		اندر = use_pack(p, w_curs, انحراف, &دھارا.avail_اندر);
		دھارا.اگلا_اندر = اندر;
		دھارا.اگلا_برآمد = fakebuf;
		دھارا.avail_برآمد = حجم(fakebuf);
		st = git_inflate(&دھارا, Z_اختتام);
		انحراف += دھارا.اگلا_اندر - اندر;
	} جب (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_انتہا(&دھارا);
	واپس (st == Z_دھارا_انتہا &&
		دھارا.کل_برآمد == expect &&
		دھارا.کل_اندر == len) ? 0 : -1;
}

ساکن خالی copy_pack_ڈیٹا(ساخت hashfile *f,
		ساخت packed_git *p,
		ساخت pack_کھڑکی **w_curs,
		off_t انحراف,
		off_t len)
{
	غیرعلامتی حرف *اندر;
	سائز_t avail;

	جب (len) {
		اندر = use_pack(p, w_curs, انحراف, &avail);
		اگر (avail > len)
			avail = (غیرعلامتی طویل)len;
		hashwrite(f, اندر, avail);
		انحراف += avail;
		len -= avail;
	}
}

ساکن اندرونی صحیح oe_سائز_greater_than(ساخت packing_ڈیٹا *pack,
				       ثابت ساخت object_اندراج *lhs,
				       سائز_t rhs)
{
	اگر (lhs->سائز_valid)
		واپس lhs->سائز_2 > rhs;
	اگر (rhs < pack->oe_سائز_حد_بندی) /* rhs < 2^x <= lhs ? */
		واپس 1;
	واپس oe_get_سائز_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ساکن غیرعلامتی طویل write_no_reuse_object(ساخت hashfile *f, ساخت object_اندراج *اندراج_2,
					   غیرعلامتی طویل حد_بندی, صحیح usable_delta)
{
	غیرعلامتی طویل سائز, datalen;
	غیرعلامتی حرف سرخی[MAX_PACK_OBJECT_سرخی],
		      dheader[MAX_PACK_OBJECT_سرخی];
	غیرعلامتی hdrlen;
	شمار object_type type;
	خالی *buf;
	ساخت odb_دھارا *st = تہی;
	ثابت غیرعلامتی hashsz = the_hash_algo->rawsz;

	اگر (!usable_delta) {
		اگر (oe_type(اندراج_2) == OBJ_BLOB &&
		    oe_سائز_greater_than(&مقصد_pack, اندراج_2,
					 repo_settings_get_big_file_حد(the_repository)) &&
		    (st = odb_دھارا_from_object(the_repository->objects, &اندراج_2->idx.oid,
						 تہی)) != تہی) {
			buf = تہی;
			type = st->type;
			سائز = st->سائز;
		} ورنہ {
			سائز_t سائز_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &اندراج_2->idx.oid, &type,
					      &سائز_st);
			سائز = cast_سائز_t_مقصد_ulong(سائز_st);
			اگر (!buf)
				die(_("unable to read %s"),
				    oid_مقصد_hex(&اندراج_2->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(اندراج_2->delta_ڈیٹا);
		اندراج_2->z_delta_سائز = 0;
	} ورنہ اگر (اندراج_2->delta_ڈیٹا) {
		سائز = DELTA_سائز(اندراج_2);
		buf = اندراج_2->delta_ڈیٹا;
		اندراج_2->delta_ڈیٹا = تہی;
		type = (allow_ofs_delta && DELTA(اندراج_2)->idx.انحراف) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} ورنہ {
		buf = get_delta(اندراج_2);
		سائز = DELTA_سائز(اندراج_2);
		type = (allow_ofs_delta && DELTA(اندراج_2)->idx.انحراف) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	اگر (st)	/* large blob case, just assume we don't compress well */
		datalen = سائز;
	ورنہ اگر (اندراج_2->z_delta_سائز)
		datalen = اندراج_2->z_delta_سائز;
	ورنہ
		datalen = do_compress(&buf, سائز);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_اندر_pack_object_سرخی(سرخی, حجم(سرخی),
					      type, سائز);

	اگر (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = اندراج_2->idx.انحراف - DELTA(اندراج_2)->idx.انحراف;
		غیرعلامتی pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		جب (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		اگر (حد_بندی && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد_بندی) {
			اگر (st)
				odb_دھارا_close(st);
			آزاد(buf);
			واپس 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
	} ورنہ اگر (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		اگر (حد_بندی && hdrlen + hashsz + datalen + hashsz >= حد_بندی) {
			اگر (st)
				odb_دھارا_close(st);
			آزاد(buf);
			واپس 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, DELTA(اندراج_2)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} ورنہ {
		اگر (حد_بندی && hdrlen + datalen + hashsz >= حد_بندی) {
			اگر (st)
				odb_دھارا_close(st);
			آزاد(buf);
			واپس 0;
		}
		hashwrite(f, سرخی, hdrlen);
	}
	اگر (st) {
		datalen = write_large_blob_ڈیٹا(st, f, &اندراج_2->idx.oid);
		odb_دھارا_close(st);
	} ورنہ {
		hashwrite(f, buf, datalen);
		آزاد(buf);
	}

	واپس hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساکن off_t write_reuse_object(ساخت hashfile *f, ساخت object_اندراج *اندراج_2,
				غیرعلامتی طویل حد_بندی, صحیح usable_delta)
{
	ساخت packed_git *p = اندر_PACK(اندراج_2);
	ساخت pack_کھڑکی *w_curs = تہی;
	uint32_t pos;
	off_t انحراف, cur;
	شمار object_type type = oe_type(اندراج_2);
	شمار object_type اندر_pack_type;
	off_t datalen;
	غیرعلامتی حرف سرخی[MAX_PACK_OBJECT_سرخی],
		      dheader[MAX_PACK_OBJECT_سرخی];
	غیرعلامتی hdrlen;
	ثابت غیرعلامتی hashsz = the_hash_algo->rawsz;
	سائز_t اندراج_سائز;

	cur = اندراج_2->اندر_pack_انحراف;
	اندر_pack_type = unpack_object_سرخی(p, &w_curs, &cur, &اندراج_سائز);
	اگر (اندر_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_مقصد_hex(&اندراج_2->idx.oid));

	اگر (DELTA(اندراج_2))
		type = (allow_ofs_delta && DELTA(اندراج_2)->idx.انحراف) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_اندر_pack_object_سرخی(سرخی, حجم(سرخی),
					      type, اندراج_سائز);

	انحراف = اندراج_2->اندر_pack_انحراف;
	اگر (انحراف_مقصد_pack_pos(p, انحراف, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_مقصد_hex(&اندراج_2->idx.oid), (uintmax_t)انحراف,
		    p->pack_name);
	datalen = pack_pos_مقصد_انحراف(p, pos + 1) - انحراف;
	اگر (!pack_مقصد_stdout && p->اشاریہ_نسخہ > 1 &&
	    check_pack_چکری_جانچ(p, &w_curs, انحراف, datalen,
			   pack_pos_مقصد_اشاریہ(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_مقصد_hex(&اندراج_2->idx.oid));
		unuse_pack(&w_curs);
		واپس write_no_reuse_object(f, اندراج_2, حد_بندی, usable_delta);
	}

	انحراف += اندراج_2->اندر_pack_سرخی_سائز;
	datalen -= اندراج_2->اندر_pack_سرخی_سائز;

	اگر (!pack_مقصد_stdout && p->اشاریہ_نسخہ == 1 &&
	    check_pack_inflate(p, &w_curs, انحراف, datalen, اندراج_سائز)) {
		error(_("corrupt packed object for %s"),
		      oid_مقصد_hex(&اندراج_2->idx.oid));
		unuse_pack(&w_curs);
		واپس write_no_reuse_object(f, اندراج_2, حد_بندی, usable_delta);
	}

	اگر (type == OBJ_OFS_DELTA) {
		off_t ofs = اندراج_2->idx.انحراف - DELTA(اندراج_2)->idx.انحراف;
		غیرعلامتی pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		جب (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		اگر (حد_بندی && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد_بندی) {
			unuse_pack(&w_curs);
			واپس 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
		reused_delta++;
	} ورنہ اگر (type == OBJ_REF_DELTA) {
		اگر (حد_بندی && hdrlen + hashsz + datalen + hashsz >= حد_بندی) {
			unuse_pack(&w_curs);
			واپس 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, DELTA(اندراج_2)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} ورنہ {
		اگر (حد_بندی && hdrlen + datalen + hashsz >= حد_بندی) {
			unuse_pack(&w_curs);
			واپس 0;
		}
		hashwrite(f, سرخی, hdrlen);
	}
	copy_pack_ڈیٹا(f, p, &w_curs, انحراف, datalen);
	unuse_pack(&w_curs);
	reused++;
	واپس hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساکن off_t write_object(ساخت hashfile *f,
			  ساخت object_اندراج *اندراج_2,
			  off_t write_انحراف)
{
	غیرعلامتی طویل حد_بندی;
	off_t len;
	صحیح usable_delta, مقصد_reuse;

	اگر (!pack_مقصد_stdout)
		چکری_جانچ_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	اگر (!pack_سائز_حد_بندی || !nr_written)
		حد_بندی = 0;
	ورنہ اگر (pack_سائز_حد_بندی <= write_انحراف)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		حد_بندی = 1;
	ورنہ
		حد_بندی = pack_سائز_حد_بندی - write_انحراف;

	اگر (!DELTA(اندراج_2))
		usable_delta = 0;	/* no delta */
	ورنہ اگر (!pack_سائز_حد_بندی)
	       usable_delta = 1;	/* unlimited packfile */
	ورنہ اگر (DELTA(اندراج_2)->idx.انحراف == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	ورنہ اگر (DELTA(اندراج_2)->idx.انحراف)
		usable_delta = 1;	/* base already exists in this pack */
	ورنہ
		usable_delta = 0;	/* base could end up in another pack */

	اگر (!reuse_object)
		مقصد_reuse = 0;	/* explicit */
	ورنہ اگر (!اندر_PACK(اندراج_2))
		مقصد_reuse = 0;	/* can't reuse what we don't have */
	ورنہ اگر (oe_type(اندراج_2) == OBJ_REF_DELTA ||
		 oe_type(اندراج_2) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		مقصد_reuse = usable_delta;
				/* ... but pack split may override that */
	ورنہ اگر (oe_type(اندراج_2) != اندراج_2->اندر_pack_type)
		مقصد_reuse = 0;	/* pack has delta which is unusable */
	ورنہ اگر (DELTA(اندراج_2))
		مقصد_reuse = 0;	/* we want to pack afresh */
	ورنہ
		مقصد_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	اگر (!مقصد_reuse)
		len = write_no_reuse_object(f, اندراج_2, حد_بندی, usable_delta);
	ورنہ
		len = write_reuse_object(f, اندراج_2, حد_بندی, usable_delta);
	اگر (!len)
		واپس 0;

	اگر (usable_delta)
		written_delta++;
	written++;
	اگر (!pack_مقصد_stdout)
		اندراج_2->idx.چکری_جانچ_32 = چکری_جانچ_32_انتہا(f);
	واپس len;
}

شمار write_one_حالت {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*معمول*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ساکن شمار write_one_حالت write_one(ساخت hashfile *f,
				       ساخت object_اندراج *e,
				       off_t *انحراف)
{
	off_t سائز;
	صحیح recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.انحراف == 1);
	اگر (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_مقصد_hex(&e->idx.oid));
		واپس WRITE_ONE_RECURSIVE;
	} ورنہ اگر (e->idx.انحراف || e->preferred_بنیاد) {
		/* offset is non zero if object is written already. */
		واپس WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	اگر (DELTA(e)) {
		e->idx.انحراف = 1; /* now recurse */
		انتخاب (write_one(f, DELTA(e), انحراف)) {
		حالت WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			سیٹ_DELTA(e, تہی);
			توڑو;
		طےشدہ:
			توڑو;
		حالت WRITE_ONE_BREAK:
			e->idx.انحراف = recursing;
			واپس WRITE_ONE_BREAK;
		}
	}

	e->idx.انحراف = *انحراف;
	سائز = write_object(f, e, *انحراف);
	اگر (!سائز) {
		e->idx.انحراف = recursing;
		واپس WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	اگر (signed_جوڑو_overflows(*انحراف, سائز))
		die(_("pack too large for current definition of off_t"));
	*انحراف += سائز;
	واپس WRITE_ONE_WRITTEN;
}

ساکن صحیح mark_tagged(ثابت ساخت reference *ref, خالی *cb_ڈیٹا غیر_مستعمل)
{
	ساخت object_id peeled;
	ساخت object_اندراج *اندراج_2 = packlist_ڈھونڈو(&مقصد_pack, ref->oid);

	اگر (اندراج_2)
		اندراج_2->tagged = 1;
	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		اندراج_2 = packlist_ڈھونڈو(&مقصد_pack, &peeled);
		اگر (اندراج_2)
			اندراج_2->tagged = 1;
	}
	واپس 0;
}

ساکن اندرونی غیرعلامتی حرف oe_layer(ساخت packing_ڈیٹا *pack,
				     ساخت object_اندراج *e)
{
	اگر (!pack->layer)
		واپس 0;
	واپس pack->layer[e - pack->objects];
}

ساکن اندرونی خالی جوڑو_مقصد_write_ترتیب(ساخت object_اندراج **wo,
			       غیرعلامتی صحیح *endp,
			       ساخت object_اندراج *e)
{
	اگر (e->filled || oe_layer(&مقصد_pack, e) != write_layer)
		واپس;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ساکن خالی جوڑو_descendants_مقصد_write_ترتیب(ساخت object_اندراج **wo,
					   غیرعلامتی صحیح *endp,
					   ساخت object_اندراج *e)
{
	صحیح جوڑو_مقصد_ترتیب = 1;
	جب (e) {
		اگر (جوڑو_مقصد_ترتیب) {
			ساخت object_اندراج *s;
			/* add this node... */
			جوڑو_مقصد_write_ترتیب(wo, endp, e);
			/* all its siblings... */
			برائے (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				جوڑو_مقصد_write_ترتیب(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		اگر (DELTA_بچہ(e)) {
			جوڑو_مقصد_ترتیب = 1;
			e = DELTA_بچہ(e);
		} ورنہ {
			جوڑو_مقصد_ترتیب = 0;
			/* our sibling might have some children, it is next */
			اگر (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				جاری;
			}
			/* go back to our parent node */
			e = DELTA(e);
			جب (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			اگر (!e) {
				/* done- we hit our original root node */
				واپس;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

ساکن خالی جوڑو_family_مقصد_write_ترتیب(ساخت object_اندراج **wo,
				      غیرعلامتی صحیح *endp,
				      ساخت object_اندراج *e)
{
	ساخت object_اندراج *جڑ;

	برائے (جڑ = e; DELTA(جڑ); جڑ = DELTA(جڑ))
		; /* nothing */
	جوڑو_descendants_مقصد_write_ترتیب(wo, endp, جڑ);
}

ساکن خالی compute_layer_ترتیب(ساخت object_اندراج **wo, غیرعلامتی صحیح *wo_انتہا)
{
	غیرعلامتی صحیح i, آخری_untagged;
	ساخت object_اندراج *objects = مقصد_pack.objects;

	برائے (i = 0; i < مقصد_pack.nr_objects; i++) {
		اگر (objects[i].tagged)
			توڑو;
		جوڑو_مقصد_write_ترتیب(wo, wo_انتہا, &objects[i]);
	}
	آخری_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	برائے (; i < مقصد_pack.nr_objects; i++) {
		اگر (objects[i].tagged)
			جوڑو_مقصد_write_ترتیب(wo, wo_انتہا, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	برائے (i = آخری_untagged; i < مقصد_pack.nr_objects; i++) {
		اگر (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_لیبل)
			جاری;
		جوڑو_مقصد_write_ترتیب(wo, wo_انتہا, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	برائے (i = آخری_untagged; i < مقصد_pack.nr_objects; i++) {
		اگر (oe_type(&objects[i]) != OBJ_درخت)
			جاری;
		جوڑو_مقصد_write_ترتیب(wo, wo_انتہا, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	برائے (i = آخری_untagged; i < مقصد_pack.nr_objects; i++) {
		اگر (!objects[i].filled && oe_layer(&مقصد_pack, &objects[i]) == write_layer)
			جوڑو_family_مقصد_write_ترتیب(wo, wo_انتہا, &objects[i]);
	}
}

ساکن ساخت object_اندراج **compute_write_ترتیب(خالی)
{
	uint32_t max_layers = 1;
	غیرعلامتی صحیح i, wo_انتہا;

	ساخت object_اندراج **wo;
	ساخت object_اندراج *objects = مقصد_pack.objects;

	برائے (i = 0; i < مقصد_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		سیٹ_DELTA_بچہ(&objects[i], تہی);
		سیٹ_DELTA_SIBLING(&objects[i], تہی);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	برائے (i = مقصد_pack.nr_objects; i > 0;) {
		ساخت object_اندراج *e = &objects[--i];
		اگر (!DELTA(e))
			جاری;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_بچہ_idx;
		سیٹ_DELTA_بچہ(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_لیبل_ref(get_main_ref_store(the_repository), mark_tagged,
			      تہی);

	اگر (use_delta_islands) {
		max_layers = compute_pack_layers(&مقصد_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, مقصد_pack.nr_objects);
	wo_انتہا = 0;

	برائے (; write_layer < max_layers; ++write_layer)
		compute_layer_ترتیب(wo, &wo_انتہا);

	اگر (wo_انتہا != مقصد_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_انتہا, مقصد_pack.nr_objects);

	واپس wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

ساکن ساخت reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
ساکن صحیح reused_chunks_nr;
ساکن صحیح reused_chunks_alloc;

ساکن خالی ریکارڈ_reused_object(off_t where, off_t انحراف)
{
	اگر (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == انحراف)
		واپس;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = انحراف;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
ساکن off_t ڈھونڈو_reused_انحراف(off_t where)
{
	صحیح lo = 0, hi = reused_chunks_nr;
	جب (lo < hi) {
		صحیح mi = lo + ((hi - lo) / 2);
		اگر (where == reused_chunks[mi].original)
			واپس reused_chunks[mi].difference;
		اگر (where < reused_chunks[mi].original)
			hi = mi;
		ورنہ
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	أكد(lo);
	واپس reused_chunks[lo-1].difference;
}

ساکن خالی write_reused_pack_one(ساخت packed_git *reuse_packfile,
				  سائز_t pos, ساخت hashfile *برآمد,
				  off_t pack_آغاز,
				  ساخت pack_کھڑکی **w_curs)
{
	off_t انحراف, اگلا, cur;
	شمار object_type type;
	سائز_t سائز;

	انحراف = pack_pos_مقصد_انحراف(reuse_packfile, pos);
	اگلا = pack_pos_مقصد_انحراف(reuse_packfile, pos + 1);

	ریکارڈ_reused_object(انحراف,
			     انحراف - (hashfile_کل(برآمد) - pack_آغاز));

	cur = انحراف;
	type = unpack_object_سرخی(reuse_packfile, w_curs, &cur, &سائز);
	أكد(type >= 0);

	اگر (type == OBJ_OFS_DELTA) {
		off_t بنیاد_انحراف;
		off_t fixup;

		غیرعلامتی حرف سرخی[MAX_PACK_OBJECT_سرخی];
		غیرعلامتی len;

		بنیاد_انحراف = get_delta_بنیاد(reuse_packfile, w_curs, &cur, type, انحراف);
		أكد(بنیاد_انحراف != 0);

		/* Convert to REF_DELTA if we must... */
		اگر (!allow_ofs_delta) {
			uint32_t بنیاد_pos;
			ساخت object_id بنیاد_oid;

			اگر (انحراف_مقصد_pack_pos(reuse_packfile, بنیاد_انحراف, &بنیاد_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)بنیاد_انحراف,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&بنیاد_oid, reuse_packfile,
					     pack_pos_مقصد_اشاریہ(reuse_packfile, بنیاد_pos));

			len = encode_اندر_pack_object_سرخی(سرخی, حجم(سرخی),
							   OBJ_REF_DELTA, سائز);
			hashwrite(برآمد, سرخی, len);
			hashwrite(برآمد, بنیاد_oid.hash, the_hash_algo->rawsz);
			copy_pack_ڈیٹا(برآمد, reuse_packfile, w_curs, cur, اگلا - cur);
			واپس;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = ڈھونڈو_reused_انحراف(انحراف) -
			ڈھونڈو_reused_انحراف(بنیاد_انحراف);
		اگر (fixup) {
			غیرعلامتی حرف ofs_سرخی[MAX_PACK_OBJECT_سرخی];
			غیرعلامتی i, ofs_len;
			off_t ofs = انحراف - بنیاد_انحراف - fixup;

			len = encode_اندر_pack_object_سرخی(سرخی, حجم(سرخی),
							   OBJ_OFS_DELTA, سائز);

			i = حجم(ofs_سرخی) - 1;
			ofs_سرخی[i] = ofs & 127;
			جب (ofs >>= 7)
				ofs_سرخی[--i] = 128 | (--ofs & 127);

			ofs_len = حجم(ofs_سرخی) - i;

			hashwrite(برآمد, سرخی, len);
			hashwrite(برآمد, ofs_سرخی + حجم(ofs_سرخی) - ofs_len, ofs_len);
			copy_pack_ڈیٹا(برآمد, reuse_packfile, w_curs, cur, اگلا - cur);
			واپس;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_ڈیٹا(برآمد, reuse_packfile, w_curs, انحراف, اگلا - انحراف);
}

ساکن سائز_t write_reused_pack_verbatim(ساخت bitmapped_pack *reuse_packfile,
					 ساخت hashfile *برآمد,
					 ساخت pack_کھڑکی **w_curs)
{
	سائز_t pos = 0;
	سائز_t انتہا;

	اگر (reuse_packfile->bitmap_pos) {
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
		واپس reuse_packfile->bitmap_pos / بٹ_اندر_EWORD;
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
	انتہا = reuse_packfile->bitmap_nr / بٹ_اندر_EWORD;
	اگر (reuse_packfile_bitmap->لفظ_alloc < انتہا)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	جب (pos < انتہا && reuse_packfile_bitmap->الفاظ[pos] == (eword_t)~0)
		pos++;

	اگر (pos) {
		off_t مقصد_write;

		written = (pos * بٹ_اندر_EWORD);
		مقصد_write = pack_pos_مقصد_انحراف(reuse_packfile->p, written)
			- حجم(ساخت pack_سرخی);

		/* We're recording one chunk, not one object. */
		ریکارڈ_reused_object(حجم(ساخت pack_سرخی), 0);
		hashflush(برآمد);
		copy_pack_ڈیٹا(برآمد, reuse_packfile->p, w_curs,
			حجم(ساخت pack_سرخی), مقصد_write);

		display_پیش_رفت(پیش_رفت_state, written);
	}
	واپس pos;
}

ساکن خالی write_reused_pack(ساخت bitmapped_pack *reuse_packfile,
			      ساخت hashfile *f)
{
	سائز_t i = reuse_packfile->bitmap_pos / بٹ_اندر_EWORD;
	uint32_t انحراف;
	off_t pack_آغاز = hashfile_کل(f) - حجم(ساخت pack_سرخی);
	ساخت pack_کھڑکی *w_curs = تہی;

	اگر (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	برائے (; i < reuse_packfile_bitmap->لفظ_alloc; ++i) {
		eword_t لفظ = reuse_packfile_bitmap->الفاظ[i];
		سائز_t pos = (i * بٹ_اندر_EWORD);

		برائے (انحراف = 0; انحراف < بٹ_اندر_EWORD; ++انحراف) {
			uint32_t pack_pos;
			اگر ((لفظ >> انحراف) == 0)
				توڑو;

			انحراف += ewah_ثنائی_ہندسہ_ctz64(لفظ >> انحراف);
			اگر (pos + انحراف < reuse_packfile->bitmap_pos)
				جاری;
			اگر (pos + انحراف >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				جاؤ مکمل;

			اگر (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				ساخت multi_pack_اشاریہ *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				اگر (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_مقصد_midx(m, pos + انحراف);
				pack_ofs = nth_midxed_انحراف(m, midx_pos);

				اگر (انحراف_مقصد_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} ورنہ {
				/*
				 * Can use bit positions directly, even for MIDX
				 * bitmaps. See comment in try_partial_reuse()
				 * for why.
				 */
				pack_pos = pos + انحراف;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_آغاز, &w_curs);
			display_پیش_رفت(پیش_رفت_state, ++written);
		}
	}

مکمل:
	unuse_pack(&w_curs);
}

ساکن خالی write_excluded_by_configs(خالی)
{
	ساخت oidset_iter iter;
	ثابت ساخت object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	جب ((oid = oidset_iter_اگلا(&iter))) {
		ساخت configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		اگر (!ex)
			BUG("configured exclusion wasn't configured");
		write_اندر_بھرا(1, ex->pack_hash_hex, لڑی_لمبائی(ex->pack_hash_hex));
		write_اندر_بھرا(1, " ", 1);
		write_اندر_بھرا(1, ex->uri, لڑی_لمبائی(ex->uri));
		write_اندر_بھرا(1, "\n", 1);
	}
}

ساکن ثابت حرف no_بانٹو_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ساکن خالی write_pack_file(خالی)
{
	uint32_t i = 0, j;
	ساخت hashfile *f;
	off_t انحراف;
	uint32_t nr_بقایا = nr_نتیجہ;
	وقت_t آخری_mtime = 0;
	ساخت object_اندراج **write_ترتیب;
	off_t بائٹیں_written = 0;

	اگر (پیش_رفت > pack_مقصد_stdout)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Writing objects"), nr_نتیجہ);
	ALLOC_ARRAY(written_list, مقصد_pack.nr_objects);
	write_ترتیب = compute_write_ترتیب();

	کرو {
		غیرعلامتی حرف hash[GIT_MAX_RAWSZ];
		حرف *pack_tmp_name = تہی;

		اگر (pack_مقصد_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			ساخت hashfd_options opts = {
				.پیش_رفت = پیش_رفت_state,
				.بفر_len = LARGE_PACKET_ڈیٹا_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} ورنہ {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		انحراف = write_pack_سرخی(f, nr_بقایا);

		اگر (reuse_packfiles_nr) {
			أكد(pack_مقصد_stdout);
			برائے (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				اگر (reused_chunks_nr)
					reuse_packfiles_مستعمل_nr++;
			}
			انحراف = hashfile_کل(f);
		}

		nr_written = 0;
		برائے (; i < مقصد_pack.nr_objects; i++) {
			ساخت object_اندراج *e = write_ترتیب[i];
			اگر (write_one(f, e, &انحراف) == WRITE_ONE_BREAK)
				توڑو;
			display_پیش_رفت(پیش_رفت_state, written);
		}

		بائٹیں_written += hashfile_کل(f) +
			the_repository->hash_algo->rawsz;
		اگر (pack_مقصد_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_اندر_دھارا | CSUM_CLOSE);
		} ورنہ اگر (nr_written == nr_بقایا) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_اندر_دھارا | CSUM_FSYNC | CSUM_CLOSE);
		} ورنہ {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			صحیح fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_سرخی_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, انحراف);
			close(fd);
			اگر (write_bitmap_اشاریہ) {
				اگر (write_bitmap_اشاریہ != WRITE_BITMAP_QUIET)
					warning(_(no_بانٹو_warning));
				write_bitmap_اشاریہ = 0;
			}
		}

		اگر (!pack_مقصد_stdout) {
			ساخت stat st;
			ساخت strbuf tmpname = STRBUF_INIT;
			ساخت bitmap_writer bitmap_writer;
			حرف *idx_tmp_name = تہی;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			اگر (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} ورنہ اگر (!آخری_mtime) {
				آخری_mtime = st.st_mtime;
			} ورنہ {
				ساخت utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --آخری_mtime;
				اگر (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", بنیاد_name,
				    hash_مقصد_hex(hash));

			اگر (write_bitmap_اشاریہ) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &مقصد_pack,
						   تہی);
				bitmap_writer_سیٹ_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_اشاریہ(&bitmap_writer,
							       written_list);
			}

			اگر (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &مقصد_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			اگر (write_bitmap_اشاریہ) {
				سائز_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_پیش_رفت(&پیش_رفت_state);

				bitmap_writer_show_پیش_رفت(&bitmap_writer,
							    پیش_رفت);
				bitmap_writer_select_commits(&bitmap_writer,
							     اشاریہ_دار_commits,
							     اشاریہ_دار_commits_nr);
				اگر (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_اختتام(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_اشاریہ = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			آزاد(idx_tmp_name);
			strbuf_release(&tmpname);
			آزاد(pack_tmp_name);
			puts(hash_مقصد_hex(hash));
		}

		/* mark written objects as written to previous pack */
		برائے (j = 0; j < nr_written; j++) {
			written_list[j]->انحراف = (off_t)-1;
		}
		nr_بقایا -= nr_written;
	} جب (nr_بقایا && i < مقصد_pack.nr_objects);

	آزاد(written_list);
	آزاد(write_ترتیب);
	stop_پیش_رفت(&پیش_رفت_state);
	اگر (written != nr_نتیجہ)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_نتیجہ);
	trace2_ڈیٹا_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_نتیجہ);
	trace2_ڈیٹا_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", بائٹیں_written);
}

ساکن صحیح no_try_delta(ثابت حرف *راستہ)
{
	ساکن ساخت attr_check *check;

	اگر (!check)
		check = attr_check_initl("delta", تہی);
	git_check_attr(the_repository->اشاریہ, راستہ, check);
	اگر (ATTR_FALSE(check->اشیاء[0].قدر))
		واپس 1;
	واپس 0;
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
ساکن صحیح have_duplicate_اندراج(ثابت ساخت object_id *oid,
				صحیح exclude)
{
	ساخت object_اندراج *اندراج_2;

	اگر (reuse_packfile_bitmap &&
	    bitmap_walk_شامل_ہے(bitmap_git, reuse_packfile_bitmap, oid))
		واپس 1;

	اندراج_2 = packlist_ڈھونڈو(&مقصد_pack, oid);
	اگر (!اندراج_2)
		واپس 0;

	اگر (exclude) {
		اگر (!اندراج_2->preferred_بنیاد)
			nr_نتیجہ--;
		اندراج_2->preferred_بنیاد = 1;
	}

	واپس 1;
}

ساکن صحیح want_cruft_object_mtime(ساخت repository *r,
				   ثابت ساخت object_id *oid,
				   غیرعلامتی flags, uint32_t mtime)
{
	ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(r->objects->ماخذ);

	برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		ساخت packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		برائے (; *cache; cache++) {
			ساخت packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = ڈھونڈو_pack_اندراج_one(oid, p);
			اگر (!ofs)
				جاری;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			اگر (!p->is_cruft)
				واپس 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			اگر (!بھرو_pack_mtimes(p)) {
				uint32_t pos;
				اگر (انحراف_مقصد_pack_pos(p, ofs, &pos) < 0)
					جاری;
				candidate_mtime = nth_packed_mtime(p, pos);
			} ورنہ {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			اگر (mtime <= candidate_mtime)
				واپس 0;
		}
	}

	واپس -1;
}

ساکن صحیح want_ملا_object(ثابت ساخت object_id *oid, صحیح exclude,
			     ساخت packed_git *p, uint32_t mtime)
{
	اگر (exclude)
		واپس 1;
	اگر (incremental)
		واپس 0;

	اگر (!is_pack_valid(p))
		واپس -1;

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
	اگر (local && !p->pack_local)
		واپس 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	اگر (ignore_packed_keep_on_disk || ignore_packed_keep_اندر_core ||
	    ignore_packed_keep_اندر_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		غیرعلامتی flags = 0;
		اگر (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		اگر (ignore_packed_keep_اندر_core)
			flags |= KEPT_PACK_اندر_CORE;
		اگر (ignore_packed_keep_اندر_core_open)
			flags |= KEPT_PACK_اندر_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		اگر (!ignore_packed_keep_اندر_core_has_cruft) {
			اگر (ignore_packed_keep_on_disk && p->pack_keep)
				واپس 0;
			اگر (ignore_packed_keep_اندر_core && p->pack_keep_اندر_core)
				واپس 0;
			اگر (ignore_packed_keep_اندر_core_open && p->pack_keep_اندر_core_open)
				واپس 0;
			اگر (has_object_kept_pack(p->repo, oid, flags))
				واپس 0;
		} ورنہ {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			اگر (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				واپس 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	اگر (!local || !have_non_local_packs)
		واپس 1;

	/* we don't know yet; keep looking for more packs */
	واپس -1;
}

ساکن صحیح want_object_اندر_pack_one(ساخت packed_git *p,
				   ثابت ساخت object_id *oid,
				   صحیح exclude,
				   ساخت packed_git **ملا_pack,
				   off_t *ملا_انحراف,
				   uint32_t ملا_mtime)
{
	off_t انحراف;

	اگر (p == *ملا_pack)
		انحراف = *ملا_انحراف;
	ورنہ
		انحراف = ڈھونڈو_pack_اندراج_one(oid, p);

	اگر (انحراف) {
		اگر (!*ملا_pack) {
			اگر (!is_pack_valid(p))
				واپس -1;
			*ملا_انحراف = انحراف;
			*ملا_pack = p;
		}
		واپس want_ملا_object(oid, exclude, p, ملا_mtime);
	}
	واپس -1;
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
ساکن صحیح want_object_اندر_pack_mtime(ثابت ساخت object_id *oid,
				     صحیح exclude,
				     ساخت packed_git **ملا_pack,
				     off_t *ملا_انحراف,
				     uint32_t ملا_mtime)
{
	ساخت odb_ماخذ_files *files =
		odb_ماخذ_files_downcast(the_repository->objects->ماخذ);
	صحیح want;
	ساخت packfile_list_اندراج *e;

	اگر (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		برائے (ساخت odb_files_dir *dir = files->dirs->اگلا; dir; dir = dir->اگلا)
			اگر (!odb_ماخذ_read_object_info(&dir->loose->بنیاد, oid, تہی, 0, تہی))
				واپس 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	اگر (*ملا_pack) {
		want = want_ملا_object(oid, exclude, *ملا_pack,
					 ملا_mtime);
		اگر (want != -1)
			واپس want;

		*ملا_pack = تہی;
		*ملا_انحراف = 0;
	}

	برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		ساخت multi_pack_اشاریہ *m = get_multi_pack_اشاریہ(dir->packed);
		ساخت pack_اندراج e;

		اگر (m && midx_fill_اندراج(m, oid, &e, تہی) == MIDX_FILL_کامیابی) {
			want = want_object_اندر_pack_one(e.p, oid, exclude, ملا_pack, ملا_انحراف, ملا_mtime);
			اگر (want != -1)
				واپس want;
		}
	}

	برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		برائے (e = dir->packed->packs.سر; e; e = e->اگلا) {
			ساخت packed_git *p = e->pack;
			want = want_object_اندر_pack_one(p, oid, exclude, ملا_pack, ملا_انحراف, ملا_mtime);
			اگر (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			اگر (want != -1)
				واپس want;
		}
	}

	اگر (uri_protocols.nr) {
		ساخت configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		صحیح i;
		ثابت حرف *p;

		اگر (ex) {
			برائے (i = 0; i < uri_protocols.nr; i++) {
				اگر (skip_سابقہ(ex->uri,
						uri_protocols.اشیاء[i].string,
						&p) &&
				    *p == ':') {
					oidset_داخل_کرو(&excluded_by_config, oid);
					واپس 0;
				}
			}
		}
	}

	واپس 1;
}

ساکن اندرونی صحیح want_object_اندر_pack(ثابت ساخت object_id *oid,
				      صحیح exclude,
				      ساخت packed_git **ملا_pack,
				      off_t *ملا_انحراف)
{
	واپس want_object_اندر_pack_mtime(oid, exclude, ملا_pack, ملا_انحراف,
					 0);
}

ساکن ساخت object_اندراج *create_object_اندراج(ثابت ساخت object_id *oid,
						شمار object_type type,
						uint32_t hash,
						صحیح exclude,
						صحیح no_try_delta,
						ساخت packed_git *ملا_pack,
						off_t ملا_انحراف)
{
	ساخت object_اندراج *اندراج_2;

	اندراج_2 = packlist_alloc(&مقصد_pack, oid);
	اندراج_2->hash = hash;
	oe_سیٹ_type(اندراج_2, type);
	اگر (exclude)
		اندراج_2->preferred_بنیاد = 1;
	ورنہ
		nr_نتیجہ++;
	اگر (ملا_pack) {
		oe_سیٹ_اندر_pack(&مقصد_pack, اندراج_2, ملا_pack);
		اندراج_2->اندر_pack_انحراف = ملا_انحراف;
	}

	اندراج_2->no_try_delta = no_try_delta;

	واپس اندراج_2;
}

ساکن ثابت حرف no_احاطہ_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ساکن خالی جوڑو_object_اندراج(ثابت ساخت object_id *oid, شمار object_type type,
			     ثابت حرف *name, صحیح exclude)
{
	ساخت packed_git *ملا_pack = تہی;
	off_t ملا_انحراف = 0;

	display_پیش_رفت(پیش_رفت_state, ++nr_نظر_آیا);

	اگر (have_duplicate_اندراج(oid, exclude))
		واپس;

	اگر (!want_object_اندر_pack(oid, exclude, &ملا_pack, &ملا_انحراف)) {
		/* The pack is missing an object, so it will not have closure */
		اگر (write_bitmap_اشاریہ) {
			اگر (write_bitmap_اشاریہ != WRITE_BITMAP_QUIET)
				warning(_(no_احاطہ_warning));
			write_bitmap_اشاریہ = 0;
		}
		واپس;
	}

	create_object_اندراج(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    ملا_pack, ملا_انحراف);
}

ساکن صحیح جوڑو_object_اندراج_from_bitmap(ثابت ساخت object_id *oid,
					شمار object_type type,
					صحیح flags غیر_مستعمل, uint32_t name_hash,
					ساخت packed_git *pack, off_t انحراف,
					خالی *payload غیر_مستعمل)
{
	display_پیش_رفت(پیش_رفت_state, ++nr_نظر_آیا);

	اگر (have_duplicate_اندراج(oid, 0))
		واپس 0;

	اگر (!want_object_اندر_pack(oid, 0, &pack, &انحراف))
		واپس 0;

	create_object_اندراج(oid, type, name_hash, 0, 0, pack, انحراف);
	واپس 0;
}

ساخت pbase_درخت_cache {
	ساخت object_id oid;
	صحیح ref;
	صحیح temporary;
	خالی *درخت_ڈیٹا;
	غیرعلامتی طویل درخت_سائز;
};

ساکن ساخت pbase_درخت_cache *(pbase_درخت_cache[256]);
ساکن صحیح pbase_درخت_cache_ix(ثابت ساخت object_id *oid)
{
	واپس oid->hash[0] % ARRAY_سائز(pbase_درخت_cache);
}
ساکن صحیح pbase_درخت_cache_ix_incr(صحیح ix)
{
	واپس (ix+1) % ARRAY_سائز(pbase_درخت_cache);
}

ساکن ساخت pbase_درخت {
	ساخت pbase_درخت *اگلا;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	ساخت pbase_درخت_cache pcache;
} *pbase_درخت;

ساکن ساخت pbase_درخت_cache *pbase_درخت_get(ثابت ساخت object_id *oid)
{
	ساخت pbase_درخت_cache *ent, *nent;
	خالی *ڈیٹا;
	غیرعلامتی طویل سائز;
	سائز_t سائز_st = 0;
	شمار object_type type;
	صحیح neigh;
	صحیح my_ix = pbase_درخت_cache_ix(oid);
	صحیح دستیاب_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	برائے (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_درخت_cache[my_ix];
		اگر (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			واپس ent;
		}
		ورنہ اگر (((دستیاب_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= دستیاب_ix) &&
			  (!ent && pbase_درخت_cache[دستیاب_ix])))
			دستیاب_ix = my_ix;
		اگر (!ent)
			توڑو;
		my_ix = pbase_درخت_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	ڈیٹا = odb_read_object(the_repository->objects, oid, &type, &سائز_st);
	سائز = cast_سائز_t_مقصد_ulong(سائز_st);
	اگر (!ڈیٹا)
		واپس تہی;
	اگر (type != OBJ_درخت) {
		آزاد(ڈیٹا);
		واپس تہی;
	}

	/* We need to either cache or return a throwaway copy */

	اگر (دستیاب_ix < 0)
		ent = تہی;
	ورنہ {
		ent = pbase_درخت_cache[دستیاب_ix];
		my_ix = دستیاب_ix;
	}

	اگر (!ent) {
		nent = xmalloc(حجم(*nent));
		nent->temporary = (دستیاب_ix < 0);
	}
	ورنہ {
		/* evict and reuse */
		آزاد(ent->درخت_ڈیٹا);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->درخت_ڈیٹا = ڈیٹا;
	nent->درخت_سائز = سائز;
	nent->ref = 1;
	اگر (!nent->temporary)
		pbase_درخت_cache[my_ix] = nent;
	واپس nent;
}

ساکن خالی pbase_درخت_put(ساخت pbase_درخت_cache *cache)
{
	اگر (!cache->temporary) {
		cache->ref--;
		واپس;
	}
	آزاد(cache->درخت_ڈیٹا);
	آزاد(cache);
}

ساکن سائز_t name_cmp_len(ثابت حرف *name)
{
	واپس strcspn(name, "\n/");
}

ساکن خالی جوڑو_pbase_object(ساخت درخت_desc *درخت,
			     ثابت حرف *name,
			     سائز_t cmplen,
			     ثابت حرف *fullname)
{
	ساخت name_اندراج اندراج_2;
	صحیح cmp;

	جب (درخت_اندراج(درخت,&اندراج_2)) {
		اگر (S_ISGITLINK(اندراج_2.منوال))
			جاری;
		cmp = درخت_اندراج_len(&اندراج_2) != cmplen ? 1 :
		      memcmp(name, اندراج_2.راستہ, cmplen);
		اگر (cmp > 0)
			جاری;
		اگر (cmp < 0)
			واپس;
		اگر (name[cmplen] != '/') {
			جوڑو_object_اندراج(&اندراج_2.oid,
					 object_type(اندراج_2.منوال),
					 fullname, 1);
			واپس;
		}
		اگر (S_ISDIR(اندراج_2.منوال)) {
			ساخت درخت_desc sub;
			ساخت pbase_درخت_cache *درخت;
			ثابت حرف *نیچے_کو = name+cmplen+1;
			سائز_t downlen = name_cmp_len(نیچے_کو);

			درخت = pbase_درخت_get(&اندراج_2.oid);
			اگر (!درخت)
				واپس;
			init_درخت_desc(&sub, &درخت->oid,
				       درخت->درخت_ڈیٹا, درخت->درخت_سائز);

			جوڑو_pbase_object(&sub, نیچے_کو, downlen, fullname);
			pbase_درخت_put(درخت);
		}
	}
}

ساکن غیرعلامتی *مکمل_pbase_paths;
ساکن صحیح مکمل_pbase_paths_عدد;
ساکن صحیح مکمل_pbase_paths_alloc;
ساکن صحیح مکمل_pbase_راستہ_pos(غیرعلامتی hash)
{
	صحیح lo = 0;
	صحیح hi = مکمل_pbase_paths_عدد;
	جب (lo < hi) {
		صحیح mi = lo + (hi - lo) / 2;
		اگر (مکمل_pbase_paths[mi] == hash)
			واپس mi;
		اگر (مکمل_pbase_paths[mi] < hash)
			hi = mi;
		ورنہ
			lo = mi + 1;
	}
	واپس -lo-1;
}

ساکن صحیح check_pbase_راستہ(غیرعلامتی hash)
{
	صحیح pos = مکمل_pbase_راستہ_pos(hash);
	اگر (0 <= pos)
		واپس 1;
	pos = -pos - 1;
	ALLOC_GROW(مکمل_pbase_paths,
		   مکمل_pbase_paths_عدد + 1,
		   مکمل_pbase_paths_alloc);
	مکمل_pbase_paths_عدد++;
	اگر (pos < مکمل_pbase_paths_عدد)
		MOVE_ARRAY(مکمل_pbase_paths + pos + 1, مکمل_pbase_paths + pos,
			   مکمل_pbase_paths_عدد - pos - 1);
	مکمل_pbase_paths[pos] = hash;
	واپس 0;
}

ساکن خالی جوڑو_preferred_بنیاد_object(ثابت حرف *name)
{
	ساخت pbase_درخت *it;
	سائز_t cmplen;
	غیرعلامتی hash = pack_name_hash_fn(name);

	اگر (!عدد_preferred_بنیاد || check_pbase_راستہ(hash))
		واپس;

	cmplen = name_cmp_len(name);
	برائے (it = pbase_درخت; it; it = it->اگلا) {
		اگر (cmplen == 0) {
			جوڑو_object_اندراج(&it->pcache.oid, OBJ_درخت, تہی, 1);
		}
		ورنہ {
			ساخت درخت_desc درخت;
			init_درخت_desc(&درخت, &it->pcache.oid,
				       it->pcache.درخت_ڈیٹا, it->pcache.درخت_سائز);
			جوڑو_pbase_object(&درخت, name, cmplen, name);
		}
	}
}

ساکن خالی جوڑو_preferred_بنیاد(ساخت object_id *oid)
{
	ساخت pbase_درخت *it;
	خالی *ڈیٹا;
	غیرعلامتی طویل سائز;
	سائز_t سائز_st = 0;
	ساخت object_id درخت_oid;

	اگر (کھڑکی <= عدد_preferred_بنیاد++)
		واپس;

	ڈیٹا = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_درخت, &سائز_st, &درخت_oid);
	سائز = cast_سائز_t_مقصد_ulong(سائز_st);
	اگر (!ڈیٹا)
		واپس;

	برائے (it = pbase_درخت; it; it = it->اگلا) {
		اگر (oideq(&it->pcache.oid, &درخت_oid)) {
			آزاد(ڈیٹا);
			واپس;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->اگلا = pbase_درخت;
	pbase_درخت = it;

	oidcpy(&it->pcache.oid, &درخت_oid);
	it->pcache.درخت_ڈیٹا = ڈیٹا;
	it->pcache.درخت_سائز = سائز;
}

ساکن خالی cleanup_preferred_بنیاد(خالی)
{
	ساخت pbase_درخت *it;
	غیرعلامتی i;

	it = pbase_درخت;
	pbase_درخت = تہی;
	جب (it) {
		ساخت pbase_درخت *tmp = it;
		it = tmp->اگلا;
		آزاد(tmp->pcache.درخت_ڈیٹا);
		آزاد(tmp);
	}

	برائے (i = 0; i < ARRAY_سائز(pbase_درخت_cache); i++) {
		اگر (!pbase_درخت_cache[i])
			جاری;
		آزاد(pbase_درخت_cache[i]->درخت_ڈیٹا);
		FREE_AND_NULL(pbase_درخت_cache[i]);
	}

	FREE_AND_NULL(مکمل_pbase_paths);
	مکمل_pbase_paths_عدد = مکمل_pbase_paths_alloc = 0;
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
ساکن صحیح can_reuse_delta(ثابت ساخت object_id *بنیاد_oid,
			   ساخت object_اندراج *delta,
			   ساخت object_اندراج **بنیاد_برآمد)
{
	ساخت object_اندراج *بنیاد;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	بنیاد = packlist_ڈھونڈو(&مقصد_pack, بنیاد_oid);
	اگر (بنیاد) {
		اگر (!اندر_same_island(&delta->idx.oid, &بنیاد->idx.oid))
			واپس 0;
		*بنیاد_برآمد = بنیاد;
		واپس 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	اگر (thin && bitmap_has_oid_اندر_uninteresting(bitmap_git, بنیاد_oid)) {
		اگر (use_delta_islands) {
			اگر (!اندر_same_island(&delta->idx.oid, بنیاد_oid))
				واپس 0;
		}
		*بنیاد_برآمد = تہی;
		واپس 1;
	}

	واپس 0;
}

ساکن خالی prefetch_مقصد_pack(uint32_t object_اشاریہ_آغاز) {
	ساخت oid_array مقصد_fetch = OID_ARRAY_INIT;
	uint32_t i;

	برائے (i = object_اشاریہ_آغاز; i < مقصد_pack.nr_objects; i++) {
		ساخت object_اندراج *اندراج_2 = مقصد_pack.objects + i;

		اگر (!odb_read_object_info_extended(the_repository->objects,
						   &اندراج_2->idx.oid,
						   تہی,
						   OBJECT_INFO_FOR_PREFETCH))
			جاری;
		oid_array_append(&مقصد_fetch, &اندراج_2->idx.oid);
	}
	promisor_remote_get_براہ_راست(the_repository,
				   مقصد_fetch.oid, مقصد_fetch.nr);
	oid_array_clear(&مقصد_fetch);
}

ساکن خالی check_object(ساخت object_اندراج *اندراج_2, uint32_t object_اشاریہ)
{
	سائز_t canonical_سائز;
	شمار object_type type;
	ساخت object_info oi = {.typep = &type, .sizep = &canonical_سائز};

	اگر (اندر_PACK(اندراج_2)) {
		ساخت packed_git *p = اندر_PACK(اندراج_2);
		ساخت pack_کھڑکی *w_curs = تہی;
		صحیح have_بنیاد = 0;
		ساخت object_id بنیاد_ref;
		ساخت object_اندراج *بنیاد_اندراج;
		سائز_t مستعمل_2, مستعمل_0, avail;
		off_t ofs;
		غیرعلامتی حرف *buf, c;
		شمار object_type type;
		سائز_t اندر_pack_سائز;

		buf = use_pack(p, &w_curs, اندراج_2->اندر_pack_انحراف, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		مستعمل_2 = unpack_object_سرخی_بفر(buf, avail,
						   &type,
						   &اندر_pack_سائز);
		اگر (مستعمل_2 == 0)
			جاؤ give_اوپر;

		اگر (type < 0)
			BUG("invalid type %d", type);
		اندراج_2->اندر_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		انتخاب (اندراج_2->اندر_pack_type) {
		طےشدہ:
			/* Not a delta hence we've already got all we need. */
			oe_سیٹ_type(اندراج_2, اندراج_2->اندر_pack_type);
			سیٹ_سائز(اندراج_2, اندر_pack_سائز);
			اندراج_2->اندر_pack_سرخی_سائز = مستعمل_2;
			اگر (oe_type(اندراج_2) < OBJ_COMMIT || oe_type(اندراج_2) > OBJ_BLOB)
				جاؤ give_اوپر;
			unuse_pack(&w_curs);
			واپس;
		حالت OBJ_REF_DELTA:
			اگر (reuse_delta && !اندراج_2->preferred_بنیاد) {
				oidread(&بنیاد_ref,
					use_pack(p, &w_curs,
						 اندراج_2->اندر_pack_انحراف + مستعمل_2,
						 تہی),
					the_repository->hash_algo);
				have_بنیاد = 1;
			}
			اندراج_2->اندر_pack_سرخی_سائز = مستعمل_2 + the_hash_algo->rawsz;
			توڑو;
		حالت OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       اندراج_2->اندر_pack_انحراف + مستعمل_2, تہی);
			مستعمل_0 = 0;
			c = buf[مستعمل_0++];
			ofs = c & 127;
			جب (c & 128) {
				ofs += 1;
				اگر (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_مقصد_hex(&اندراج_2->idx.oid));
					جاؤ give_اوپر;
				}
				c = buf[مستعمل_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = اندراج_2->اندر_pack_انحراف - ofs;
			اگر (ofs <= 0 || ofs >= اندراج_2->اندر_pack_انحراف) {
				error(_("delta base offset out of bound for %s"),
				      oid_مقصد_hex(&اندراج_2->idx.oid));
				جاؤ give_اوپر;
			}
			اگر (reuse_delta && !اندراج_2->preferred_بنیاد) {
				uint32_t pos;
				اگر (انحراف_مقصد_pack_pos(p, ofs, &pos) < 0)
					جاؤ give_اوپر;
				اگر (!nth_packed_object_id(&بنیاد_ref, p,
							  pack_pos_مقصد_اشاریہ(p, pos)))
					have_بنیاد = 1;
			}
			اندراج_2->اندر_pack_سرخی_سائز = مستعمل_2 + مستعمل_0;
			توڑو;
		}

		اگر (have_بنیاد &&
		    can_reuse_delta(&بنیاد_ref, اندراج_2, &بنیاد_اندراج)) {
			oe_سیٹ_type(اندراج_2, اندراج_2->اندر_pack_type);
			سیٹ_سائز(اندراج_2, اندر_pack_سائز); /* delta size */
			سیٹ_DELTA_سائز(اندراج_2, اندر_pack_سائز);

			اگر (بنیاد_اندراج) {
				سیٹ_DELTA(اندراج_2, بنیاد_اندراج);
				اندراج_2->delta_sibling_idx = بنیاد_اندراج->delta_بچہ_idx;
				سیٹ_DELTA_بچہ(بنیاد_اندراج, اندراج_2);
			} ورنہ {
				سیٹ_DELTA_EXT(اندراج_2, &بنیاد_ref);
			}

			unuse_pack(&w_curs);
			واپس;
		}

		اگر (oe_type(اندراج_2)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = اندراج_2->اندر_pack_انحراف + اندراج_2->اندر_pack_سرخی_سائز;
			canonical_سائز = get_سائز_from_delta(p, &w_curs,
							     delta_pos);
			اگر (canonical_سائز == 0)
				جاؤ give_اوپر;
			سیٹ_سائز(اندراج_2, canonical_سائز);
			unuse_pack(&w_curs);
			واپس;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_اوپر:
		unuse_pack(&w_curs);
	}

	اگر (odb_read_object_info_extended(the_repository->objects, &اندراج_2->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		اگر (repo_has_promisor_remote(the_repository)) {
			prefetch_مقصد_pack(object_اشاریہ);
			اگر (odb_read_object_info_extended(the_repository->objects, &اندراج_2->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} ورنہ {
			type = -1;
		}
	}
	oe_سیٹ_type(اندراج_2, type);
	اگر (اندراج_2->type_valid) {
		سیٹ_سائز(اندراج_2, canonical_سائز);
	} ورنہ {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ساکن صحیح pack_انحراف_ترتیب_دو(ثابت خالی *_a, ثابت خالی *_b)
{
	ثابت ساخت object_اندراج *a = *(ساخت object_اندراج **)_a;
	ثابت ساخت object_اندراج *b = *(ساخت object_اندراج **)_b;
	ثابت ساخت packed_git *a_اندر_pack = اندر_PACK(a);
	ثابت ساخت packed_git *b_اندر_pack = اندر_PACK(b);

	/* avoid filesystem trashing with loose objects */
	اگر (!a_اندر_pack && !b_اندر_pack)
		واپس oidcmp(&a->idx.oid, &b->idx.oid);

	اگر (a_اندر_pack < b_اندر_pack)
		واپس -1;
	اگر (a_اندر_pack > b_اندر_pack)
		واپس 1;
	واپس a->اندر_pack_انحراف < b->اندر_pack_انحراف ? -1 :
			(a->اندر_pack_انحراف > b->اندر_pack_انحراف);
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
ساکن خالی drop_reused_delta(ساخت object_اندراج *اندراج_2)
{
	غیرعلامتی *idx = &مقصد_pack.objects[اندراج_2->delta_idx - 1].delta_بچہ_idx;
	ساخت object_info oi = OBJECT_INFO_INIT;
	شمار object_type type;
	سائز_t سائز;

	جب (*idx) {
		ساخت object_اندراج *oe = &مقصد_pack.objects[*idx - 1];

		اگر (oe == اندراج_2)
			*idx = oe->delta_sibling_idx;
		ورنہ
			idx = &oe->delta_sibling_idx;
	}
	سیٹ_DELTA(اندراج_2, تہی);
	اندراج_2->گہرائی = 0;

	oi.sizep = &سائز;
	oi.typep = &type;
	اگر (packed_object_info(تہی, اندر_PACK(اندراج_2), اندراج_2->اندر_pack_انحراف, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_سیٹ_type(اندراج_2,
			    odb_read_object_info(the_repository->objects,
						 &اندراج_2->idx.oid, &سائز));
	} ورنہ {
		oe_سیٹ_type(اندراج_2, type);
	}
	سیٹ_سائز(اندراج_2, سائز);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
ساکن خالی break_delta_chains(ساخت object_اندراج *اندراج_2)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t کل_گہرائی;
	ساخت object_اندراج *cur, *اگلا;

	برائے (cur = اندراج_2, کل_گہرائی = 0;
	     cur;
	     cur = DELTA(cur), کل_گہرائی++) {
		اگر (cur->dfs_state == DFS_مکمل) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			کل_گہرائی += cur->گہرائی;
			توڑو;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		اگر (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		اگر (!DELTA(cur)) {
			cur->dfs_state = DFS_مکمل;
			توڑو;
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
		cur->dfs_state = DFS_فعال;
		اگر (DELTA(cur)->dfs_state == DFS_فعال) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_مکمل;
			توڑو;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	برائے (cur = اندراج_2; cur; cur = اگلا) {
		اگلا = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		اگر (cur->dfs_state == DFS_مکمل)
			توڑو;
		ورنہ اگر (cur->dfs_state != DFS_فعال)
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
		cur->گہرائی = (کل_گہرائی--) % (گہرائی + 1);
		اگر (!cur->گہرائی)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_مکمل;
	}
}

ساکن خالی get_object_details(خالی)
{
	uint32_t i;
	ساخت object_اندراج **sorted_by_انحراف;

	اگر (پیش_رفت)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Counting objects"),
						مقصد_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_انحراف, مقصد_pack.nr_objects);
	برائے (i = 0; i < مقصد_pack.nr_objects; i++)
		sorted_by_انحراف[i] = مقصد_pack.objects + i;
	QSORT(sorted_by_انحراف, مقصد_pack.nr_objects, pack_انحراف_ترتیب_دو);

	برائے (i = 0; i < مقصد_pack.nr_objects; i++) {
		ساخت object_اندراج *اندراج_2 = sorted_by_انحراف[i];
		check_object(اندراج_2, i);
		اگر (اندراج_2->type_valid &&
		    oe_سائز_greater_than(&مقصد_pack, اندراج_2,
					 repo_settings_get_big_file_حد(the_repository)))
			اندراج_2->no_try_delta = 1;
		display_پیش_رفت(پیش_رفت_state, i + 1);
	}
	stop_پیش_رفت(&پیش_رفت_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	برائے (i = 0; i < مقصد_pack.nr_objects; i++)
		break_delta_chains(&مقصد_pack.objects[i]);

	آزاد(sorted_by_انحراف);
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
ساکن صحیح type_سائز_ترتیب_دو(ثابت خالی *_a, ثابت خالی *_b)
{
	ثابت ساخت object_اندراج *a = *(ساخت object_اندراج **)_a;
	ثابت ساخت object_اندراج *b = *(ساخت object_اندراج **)_b;
	ثابت شمار object_type a_type = oe_type(a);
	ثابت شمار object_type b_type = oe_type(b);
	ثابت غیرعلامتی طویل a_سائز = سائز_3(a);
	ثابت غیرعلامتی طویل b_سائز = سائز_3(b);

	اگر (a_type > b_type)
		واپس -1;
	اگر (a_type < b_type)
		واپس 1;
	اگر (a->hash > b->hash)
		واپس -1;
	اگر (a->hash < b->hash)
		واپس 1;
	اگر (a->preferred_بنیاد > b->preferred_بنیاد)
		واپس -1;
	اگر (a->preferred_بنیاد < b->preferred_بنیاد)
		واپس 1;
	اگر (use_delta_islands) {
		ثابت صحیح island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		اگر (island_cmp)
			واپس island_cmp;
	}
	اگر (a_سائز > b_سائز)
		واپس -1;
	اگر (a_سائز < b_سائز)
		واپس 1;
	واپس a < b ? -1 : (a > b);  /* newest first */
}

ساخت unpacked {
	ساخت object_اندراج *اندراج_2;
	خالی *ڈیٹا;
	ساخت delta_اشاریہ *اشاریہ;
	غیرعلامتی گہرائی;
};

ساکن صحیح delta_cacheable(سائز_t src_سائز, سائز_t trg_سائز,
			   سائز_t delta_سائز_2)
{
	اگر (max_delta_cache_سائز && delta_cache_سائز + delta_سائز_2 > max_delta_cache_سائز)
		واپس 0;

	اگر (delta_سائز_2 < cache_max_small_delta_سائز)
		واپس 1;

	/* cache delta, if objects are large enough compared to delta size */
	اگر ((src_سائز >> 20) + (trg_سائز >> 21) > (delta_سائز_2 >> 10))
		واپس 1;

	واپس 0;
}

/* Protect delta_cache_size */
ساکن pthread_mutex_t cache_mutex;
#تعریف_کرو cache_تالا()		pthread_mutex_تالا(&cache_mutex)
#تعریف_کرو cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ساکن pthread_mutex_t پیش_رفت_mutex;
#تعریف_کرو پیش_رفت_تالا()		pthread_mutex_تالا(&پیش_رفت_mutex)
#تعریف_کرو پیش_رفت_unlock()	pthread_mutex_unlock(&پیش_رفت_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ساکن اندرونی صحیح oe_سائز_less_than(ساخت packing_ڈیٹا *pack,
				    ثابت ساخت object_اندراج *lhs,
				    سائز_t rhs)
{
	اگر (lhs->سائز_valid)
		واپس lhs->سائز_2 < rhs;
	اگر (rhs < pack->oe_سائز_حد_بندی) /* rhs < 2^x <= lhs ? */
		واپس 0;
	واپس oe_get_سائز_slow(pack, lhs) < rhs;
}

ساکن اندرونی خالی oe_سیٹ_درخت_گہرائی(ساخت packing_ڈیٹا *pack,
				     ساخت object_اندراج *e,
				     غیرعلامتی صحیح درخت_گہرائی)
{
	اگر (!pack->درخت_گہرائی)
		CALLOC_ARRAY(pack->درخت_گہرائی, pack->nr_alloc);
	pack->درخت_گہرائی[e - pack->objects] = درخت_گہرائی;
}

ساکن خالی ریکارڈ_درخت_گہرائی(ثابت ساخت object_id *oid, ثابت حرف *name)
{
	ثابت حرف *p;
	غیرعلامتی گہرائی;
	ساخت object_اندراج *ent;

	/* the empty string is a root tree, which is depth 0 */
	گہرائی = *name ? 1 : 0;
	برائے (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		گہرائی++;

	ent = packlist_ڈھونڈو(&مقصد_pack, oid);
	اگر (ent && گہرائی > oe_درخت_گہرائی(&مقصد_pack, ent))
		oe_سیٹ_درخت_گہرائی(&مقصد_pack, ent, گہرائی);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
سائز_t oe_get_سائز_slow(ساخت packing_ڈیٹا *pack,
			ثابت ساخت object_اندراج *e)
{
	ساخت packed_git *p;
	ساخت pack_کھڑکی *w_curs;
	غیرعلامتی حرف *buf;
	شمار object_type type;
	سائز_t مستعمل_2, avail, سائز;

	اگر (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		سائز_t sz;
		packing_ڈیٹا_تالا(&مقصد_pack);
		اگر (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_مقصد_hex(&e->idx.oid));
		packing_ڈیٹا_unlock(&مقصد_pack);
		واپس sz;
	}

	p = oe_اندر_pack(pack, e);
	اگر (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_ڈیٹا_تالا(&مقصد_pack);
	w_curs = تہی;
	buf = use_pack(p, &w_curs, e->اندر_pack_انحراف, &avail);
	مستعمل_2 = unpack_object_سرخی_بفر(buf, avail, &type, &سائز);
	اگر (مستعمل_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_مقصد_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_ڈیٹا_unlock(&مقصد_pack);
	واپس سائز;
}

ساکن صحیح try_delta(ساخت unpacked *trg, ساخت unpacked *src,
		     غیرعلامتی max_گہرائی, سائز_t *mem_usage)
{
	ساخت object_اندراج *trg_اندراج = trg->اندراج_2;
	ساخت object_اندراج *src_اندراج = src->اندراج_2;
	غیرعلامتی طویل trg_سائز, src_سائز, sizediff, max_سائز, sz;
	سائز_t delta_سائز_2;
	غیرعلامتی ref_گہرائی;
	شمار object_type type;
	خالی *delta_buf;

	/* Don't bother doing diffs between different types */
	اگر (oe_type(trg_اندراج) != oe_type(src_اندراج))
		واپس -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	اگر (reuse_delta && اندر_PACK(trg_اندراج) &&
	    اندر_PACK(trg_اندراج) == اندر_PACK(src_اندراج) &&
	    !src_اندراج->preferred_بنیاد &&
	    trg_اندراج->اندر_pack_type != OBJ_REF_DELTA &&
	    trg_اندراج->اندر_pack_type != OBJ_OFS_DELTA)
		واپس 0;

	/* Let's not bust the allowed depth. */
	اگر (src->گہرائی >= max_گہرائی)
		واپس 0;

	/* Now some size filtering heuristics. */
	trg_سائز = سائز_3(trg_اندراج);
	اگر (!DELTA(trg_اندراج)) {
		max_سائز = trg_سائز/2 - the_hash_algo->rawsz;
		ref_گہرائی = 1;
	} ورنہ {
		max_سائز = DELTA_سائز(trg_اندراج);
		ref_گہرائی = trg->گہرائی;
	}
	max_سائز = (uint64_t)max_سائز * (max_گہرائی - src->گہرائی) /
						(max_گہرائی - ref_گہرائی + 1);
	اگر (max_سائز == 0)
		واپس 0;
	src_سائز = سائز_3(src_اندراج);
	sizediff = src_سائز < trg_سائز ? trg_سائز - src_سائز : 0;
	اگر (sizediff >= max_سائز)
		واپس 0;
	اگر (trg_سائز < src_سائز / 32)
		واپس 0;

	اگر (!اندر_same_island(&trg->اندراج_2->idx.oid, &src->اندراج_2->idx.oid))
		واپس 0;

	/* Load data if not already done */
	اگر (!trg->ڈیٹا) {
		سائز_t sz_st = 0;
		packing_ڈیٹا_تالا(&مقصد_pack);
		trg->ڈیٹا = odb_read_object(the_repository->objects,
					    &trg_اندراج->idx.oid, &type,
					    &sz_st);
		sz = cast_سائز_t_مقصد_ulong(sz_st);
		packing_ڈیٹا_unlock(&مقصد_pack);
		اگر (!trg->ڈیٹا)
			die(_("object %s cannot be read"),
			    oid_مقصد_hex(&trg_اندراج->idx.oid));
		اگر (sz != trg_سائز)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_مقصد_hex(&trg_اندراج->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_سائز);
		*mem_usage += sz;
	}
	اگر (!src->ڈیٹا) {
		سائز_t sz_st = 0;
		packing_ڈیٹا_تالا(&مقصد_pack);
		src->ڈیٹا = odb_read_object(the_repository->objects,
					    &src_اندراج->idx.oid, &type,
					    &sz_st);
		sz = cast_سائز_t_مقصد_ulong(sz_st);
		packing_ڈیٹا_unlock(&مقصد_pack);
		اگر (!src->ڈیٹا) {
			اگر (src_اندراج->preferred_بنیاد) {
				ساکن صحیح warned = 0;
				اگر (!warned++)
					warning(_("object %s cannot be read"),
						oid_مقصد_hex(&src_اندراج->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				واپس 0;
			}
			die(_("object %s cannot be read"),
			    oid_مقصد_hex(&src_اندراج->idx.oid));
		}
		اگر (sz != src_سائز)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_مقصد_hex(&src_اندراج->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_سائز);
		*mem_usage += sz;
	}
	اگر (!src->اشاریہ) {
		src->اشاریہ = create_delta_اشاریہ(src->ڈیٹا, src_سائز);
		اگر (!src->اشاریہ) {
			ساکن صحیح warned = 0;
			اگر (!warned++)
				warning(_("suboptimal pack - out of memory"));
			واپس 0;
		}
		*mem_usage += sizeof_delta_اشاریہ(src->اشاریہ);
	}

	delta_buf = create_delta(src->اشاریہ, trg->ڈیٹا, trg_سائز, &delta_سائز_2, max_سائز);
	اگر (!delta_buf)
		واپس 0;

	اگر (DELTA(trg_اندراج)) {
		/* Prefer only shallower same-sized deltas. */
		اگر (delta_سائز_2 == DELTA_سائز(trg_اندراج) &&
		    src->گہرائی + 1 >= trg->گہرائی) {
			آزاد(delta_buf);
			واپس 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	آزاد(trg_اندراج->delta_ڈیٹا);
	cache_تالا();
	اگر (trg_اندراج->delta_ڈیٹا) {
		delta_cache_سائز -= DELTA_سائز(trg_اندراج);
		trg_اندراج->delta_ڈیٹا = تہی;
	}
	اگر (delta_cacheable(src_سائز, trg_سائز, delta_سائز_2)) {
		delta_cache_سائز += delta_سائز_2;
		cache_unlock();
		trg_اندراج->delta_ڈیٹا = xrealloc(delta_buf, delta_سائز_2);
	} ورنہ {
		cache_unlock();
		آزاد(delta_buf);
	}

	سیٹ_DELTA(trg_اندراج, src_اندراج);
	سیٹ_DELTA_سائز(trg_اندراج, delta_سائز_2);
	trg->گہرائی = src->گہرائی + 1;

	واپس 1;
}

ساکن غیرعلامتی صحیح check_delta_حد_بندی(ساخت object_اندراج *me, غیرعلامتی صحیح n)
{
	ساخت object_اندراج *بچہ = DELTA_بچہ(me);
	غیرعلامتی صحیح m = n;
	جب (بچہ) {
		ثابت غیرعلامتی صحیح c = check_delta_حد_بندی(بچہ, n + 1);
		اگر (m < c)
			m = c;
		بچہ = DELTA_SIBLING(بچہ);
	}
	واپس m;
}

ساکن سائز_t free_unpacked(ساخت unpacked *n)
{
	سائز_t freed_mem = sizeof_delta_اشاریہ(n->اشاریہ);
	free_delta_اشاریہ(n->اشاریہ);
	n->اشاریہ = تہی;
	اگر (n->ڈیٹا) {
		freed_mem += سائز_3(n->اندراج_2);
		FREE_AND_NULL(n->ڈیٹا);
	}
	n->اندراج_2 = تہی;
	n->گہرائی = 0;
	واپس freed_mem;
}

ساکن خالی ڈھونڈو_deltas(ساخت object_اندراج **list, غیرعلامتی *list_سائز,
			صحیح کھڑکی, صحیح گہرائی, غیرعلامتی *processed)
{
	uint32_t i, idx = 0, گنتی = 0;
	ساخت unpacked *array;
	سائز_t mem_usage = 0;

	CALLOC_ARRAY(array, کھڑکی);

	برائے (;;) {
		ساخت object_اندراج *اندراج_2;
		ساخت unpacked *n = array + idx;
		صحیح j, max_گہرائی, بہترین_بنیاد = -1;

		پیش_رفت_تالا();
		اگر (!*list_سائز) {
			پیش_رفت_unlock();
			توڑو;
		}
		اندراج_2 = *list++;
		(*list_سائز)--;
		اگر (!اندراج_2->preferred_بنیاد) {
			(*processed)++;
			display_پیش_رفت(پیش_رفت_state, *processed);
		}
		پیش_رفت_unlock();

		mem_usage -= free_unpacked(n);
		n->اندراج_2 = اندراج_2;

		جب (کھڑکی_حافظہ_حد_بندی &&
		       mem_usage > کھڑکی_حافظہ_حد_بندی &&
		       گنتی > 1) {
			ثابت uint32_t دم = (idx + کھڑکی - گنتی) % کھڑکی;
			mem_usage -= free_unpacked(array + دم);
			گنتی--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		اگر (اندراج_2->preferred_بنیاد)
			جاؤ اگلا;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_گہرائی = گہرائی;
		اگر (DELTA_بچہ(اندراج_2)) {
			max_گہرائی -= check_delta_حد_بندی(اندراج_2, 0);
			اگر (max_گہرائی <= 0)
				جاؤ اگلا;
		}

		j = کھڑکی;
		جب (--j > 0) {
			صحیح ret;
			uint32_t other_idx = idx + j;
			ساخت unpacked *m;
			اگر (other_idx >= کھڑکی)
				other_idx -= کھڑکی;
			m = array + other_idx;
			اگر (!m->اندراج_2)
				توڑو;
			ret = try_delta(n, m, max_گہرائی, &mem_usage);
			اگر (ret < 0)
				توڑو;
			ورنہ اگر (ret > 0)
				بہترین_بنیاد = other_idx;
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
		اگر (اندراج_2->delta_ڈیٹا && !pack_مقصد_stdout) {
			غیرعلامتی طویل سائز;

			سائز = do_compress(&اندراج_2->delta_ڈیٹا, DELTA_سائز(اندراج_2));
			اگر (سائز < (1U << OE_Z_DELTA_بٹ)) {
				اندراج_2->z_delta_سائز = سائز;
				cache_تالا();
				delta_cache_سائز -= DELTA_سائز(اندراج_2);
				delta_cache_سائز += اندراج_2->z_delta_سائز;
				cache_unlock();
			} ورنہ {
				FREE_AND_NULL(اندراج_2->delta_ڈیٹا);
				اندراج_2->z_delta_سائز = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		اگر (DELTA(اندراج_2) && max_گہرائی <= n->گہرائی)
			جاری;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		اگر (DELTA(اندراج_2)) {
			ساخت unpacked تبادلہ_2 = array[بہترین_بنیاد];
			صحیح dist = (کھڑکی + idx - بہترین_بنیاد) % کھڑکی;
			صحیح dst = بہترین_بنیاد;
			جب (dist--) {
				صحیح src = (dst + 1) % کھڑکی;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = تبادلہ_2;
		}

		اگلا:
		idx++;
		اگر (گنتی + 1 < کھڑکی)
			گنتی++;
		اگر (idx >= کھڑکی)
			idx = 0;
	}

	برائے (i = 0; i < کھڑکی; ++i) {
		free_delta_اشاریہ(array[i].اشاریہ);
		آزاد(array[i].ڈیٹا);
	}
	آزاد(array);
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

ساخت thread_params {
	pthread_t thread;
	ساخت object_اندراج **list;
	ساخت packing_region *regions;
	غیرعلامتی list_سائز;
	غیرعلامتی بقایا;
	صحیح کھڑکی;
	صحیح گہرائی;
	صحیح working;
	صحیح ڈیٹا_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	غیرعلامتی *processed;
};

ساکن pthread_cond_t پیش_رفت_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساکن خالی init_threaded_تلاش(خالی)
{
	pthread_mutex_init(&cache_mutex, تہی);
	pthread_mutex_init(&پیش_رفت_mutex, تہی);
	pthread_cond_init(&پیش_رفت_cond, تہی);
}

ساکن خالی cleanup_threaded_تلاش(خالی)
{
	pthread_cond_destroy(&پیش_رفت_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&پیش_رفت_mutex);
}

ساکن خالی *threaded_ڈھونڈو_deltas(خالی *arg)
{
	ساخت thread_params *me = arg;

	پیش_رفت_تالا();
	جب (me->بقایا) {
		پیش_رفت_unlock();

		ڈھونڈو_deltas(me->list, &me->بقایا,
			    me->کھڑکی, me->گہرائی, me->processed);

		پیش_رفت_تالا();
		me->working = 0;
		pthread_cond_signal(&پیش_رفت_cond);
		پیش_رفت_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_تالا(&me->mutex);
		جب (!me->ڈیٹا_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->ڈیٹا_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		پیش_رفت_تالا();
	}
	پیش_رفت_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	واپس تہی;
}

ساکن خالی ll_ڈھونڈو_deltas(ساخت object_اندراج **list, غیرعلامتی list_سائز,
			   صحیح کھڑکی, صحیح گہرائی, غیرعلامتی *processed)
{
	ساخت thread_params *p;
	صحیح i, ret, فعال_دھاگے = 0;

	init_threaded_تلاش();

	اگر (delta_تلاش_دھاگے <= 1) {
		ڈھونڈو_deltas(list, &list_سائز, کھڑکی, گہرائی, processed);
		cleanup_threaded_تلاش();
		واپس;
	}
	اگر (پیش_رفت > pack_مقصد_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_تلاش_دھاگے);
	CALLOC_ARRAY(p, delta_تلاش_دھاگے);

	/* Partition the work amongst work threads. */
	برائے (i = 0; i < delta_تلاش_دھاگے; i++) {
		غیرعلامتی sub_سائز = list_سائز / (delta_تلاش_دھاگے - i);

		/* don't use too small segments or no deltas will be found */
		اگر (sub_سائز < 2*کھڑکی && i+1 < delta_تلاش_دھاگے)
			sub_سائز = 0;

		p[i].کھڑکی = کھڑکی;
		p[i].گہرائی = گہرائی;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].ڈیٹا_ready = 0;

		/* try to split chunks on "path" boundaries */
		جب (sub_سائز && sub_سائز < list_سائز &&
		       list[sub_سائز]->hash &&
		       list[sub_سائز]->hash == list[sub_سائز-1]->hash)
			sub_سائز++;

		p[i].list = list;
		p[i].list_سائز = sub_سائز;
		p[i].بقایا = sub_سائز;

		list += sub_سائز;
		list_سائز -= sub_سائز;
	}

	/* Start work threads. */
	برائے (i = 0; i < delta_تلاش_دھاگے; i++) {
		اگر (!p[i].list_سائز)
			جاری;
		pthread_mutex_init(&p[i].mutex, تہی);
		pthread_cond_init(&p[i].cond, تہی);
		ret = pthread_create(&p[i].thread, تہی,
				     threaded_ڈھونڈو_deltas, &p[i]);
		اگر (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		فعال_دھاگے++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	جب (فعال_دھاگے) {
		ساخت thread_params *ہدف = تہی;
		ساخت thread_params *victim = تہی;
		غیرعلامتی sub_سائز = 0;

		پیش_رفت_تالا();
		برائے (;;) {
			برائے (i = 0; !ہدف && i < delta_تلاش_دھاگے; i++)
				اگر (!p[i].working)
					ہدف = &p[i];
			اگر (ہدف)
				توڑو;
			pthread_cond_wait(&پیش_رفت_cond, &پیش_رفت_mutex);
		}

		برائے (i = 0; i < delta_تلاش_دھاگے; i++)
			اگر (p[i].بقایا > 2*کھڑکی &&
			    (!victim || victim->بقایا < p[i].بقایا))
				victim = &p[i];
		اگر (victim) {
			sub_سائز = victim->بقایا / 2;
			list = victim->list + victim->list_سائز - sub_سائز;
			جب (sub_سائز && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_سائز--;
			}
			اگر (!sub_سائز) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_سائز = victim->بقایا / 2;
				list -= sub_سائز;
			}
			ہدف->list = list;
			victim->list_سائز -= sub_سائز;
			victim->بقایا -= sub_سائز;
		}
		ہدف->list_سائز = sub_سائز;
		ہدف->بقایا = sub_سائز;
		ہدف->working = 1;
		پیش_رفت_unlock();

		pthread_mutex_تالا(&ہدف->mutex);
		ہدف->ڈیٹا_ready = 1;
		pthread_cond_signal(&ہدف->cond);
		pthread_mutex_unlock(&ہدف->mutex);

		اگر (!sub_سائز) {
			pthread_join(ہدف->thread, تہی);
			pthread_cond_destroy(&ہدف->cond);
			pthread_mutex_destroy(&ہدف->mutex);
			فعال_دھاگے--;
		}
	}
	cleanup_threaded_تلاش();
	آزاد(p);
}

ساکن صحیح obj_is_packed(ثابت ساخت object_id *oid)
{
	واپس packlist_ڈھونڈو(&مقصد_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_شامل_ہے(bitmap_git, reuse_packfile_bitmap, oid));
}

ساکن خالی جوڑو_لیبل_chain(ثابت ساخت object_id *oid)
{
	ساخت لیبل *لیبل;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	اگر (obj_is_packed(oid))
		واپس;

	لیبل = lookup_لیبل(the_repository, oid);
	جب (1) {
		اگر (!لیبل || parse_لیبل(the_repository, لیبل) || !لیبل->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_مقصد_hex(oid));

		جوڑو_object_اندراج(&لیبل->object.oid, OBJ_لیبل, تہی, 0);

		اگر (لیبل->tagged->type != OBJ_لیبل)
			واپس;

		لیبل = (ساخت لیبل *)لیبل->tagged;
	}
}

ساکن صحیح جوڑو_ref_لیبل(ثابت ساخت reference *ref, خالی *cb_ڈیٹا غیر_مستعمل)
{
	ساخت object_id peeled;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		جوڑو_لیبل_chain(ref->oid);
	واپس 0;
}

ساکن صحیح should_attempt_deltas(ساخت object_اندراج *اندراج_2)
{
	اگر (DELTA(اندراج_2))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		واپس 0;

	اگر (!اندراج_2->type_valid ||
	    oe_سائز_less_than(&مقصد_pack, اندراج_2, 50))
		واپس 0;

	اگر (اندراج_2->no_try_delta)
		واپس 0;

	اگر (!اندراج_2->preferred_بنیاد) {
		اگر (oe_type(اندراج_2) < 0)
			die(_("unable to get type of object %s"),
				oid_مقصد_hex(&اندراج_2->idx.oid));
	} ورنہ اگر (oe_type(اندراج_2) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		واپس 0;
	}

	واپس 1;
}

ساکن خالی ڈھونڈو_deltas_for_region(ساخت object_اندراج *list,
				   ساخت packing_region *region,
				   غیرعلامتی صحیح *processed)
{
	ساخت object_اندراج **delta_list;
	غیرعلامتی صحیح delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	برائے (سائز_t i = 0; i < region->nr; i++) {
		ساخت object_اندراج *اندراج_2 = list + region->آغاز + i;
		اگر (should_attempt_deltas(اندراج_2))
			delta_list[delta_list_nr++] = اندراج_2;
	}

	QSORT(delta_list, delta_list_nr, type_سائز_ترتیب_دو);
	ڈھونڈو_deltas(delta_list, &delta_list_nr, کھڑکی, گہرائی, processed);
	آزاد(delta_list);
}

ساکن خالی ڈھونڈو_deltas_by_region(ساخت object_اندراج *list,
				  ساخت packing_region *regions,
				  سائز_t آغاز, سائز_t nr)
{
	غیرعلامتی صحیح processed = 0;
	سائز_t پیش_رفت_nr;

	اگر (!nr)
		واپس;

	پیش_رفت_nr = regions[nr - 1].آغاز + regions[nr - 1].nr;

	اگر (پیش_رفت)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Compressing objects by path"),
						پیش_رفت_nr);

	جب (nr--)
		ڈھونڈو_deltas_for_region(list,
				       &regions[آغاز++],
				       &processed);

	display_پیش_رفت(پیش_رفت_state, پیش_رفت_nr);
	stop_پیش_رفت(&پیش_رفت_state);
}

ساکن خالی *threaded_ڈھونڈو_deltas_by_راستہ(خالی *arg)
{
	ساخت thread_params *me = arg;

	پیش_رفت_تالا();
	جب (me->بقایا) {
		جب (me->بقایا) {
			پیش_رفت_unlock();
			ڈھونڈو_deltas_for_region(مقصد_pack.objects,
					       me->regions,
					       me->processed);
			پیش_رفت_تالا();
			me->بقایا--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&پیش_رفت_cond);
		پیش_رفت_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_تالا(&me->mutex);
		جب (!me->ڈیٹا_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->ڈیٹا_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		پیش_رفت_تالا();
	}
	پیش_رفت_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	واپس تہی;
}

ساکن خالی ll_ڈھونڈو_deltas_by_region(ساخت object_اندراج *list,
				     ساخت packing_region *regions,
				     uint32_t آغاز, uint32_t nr)
{
	ساخت thread_params *p;
	صحیح i, ret, فعال_دھاگے = 0;
	غیرعلامتی صحیح processed = 0;
	uint32_t پیش_رفت_nr;
	init_threaded_تلاش();

	اگر (!nr)
		واپس;

	پیش_رفت_nr =  regions[nr - 1].آغاز + regions[nr - 1].nr;
	اگر (delta_تلاش_دھاگے <= 1) {
		ڈھونڈو_deltas_by_region(list, regions, آغاز, nr);
		cleanup_threaded_تلاش();
		واپس;
	}

	اگر (پیش_رفت > pack_مقصد_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_تلاش_دھاگے),
			   delta_تلاش_دھاگے);
	CALLOC_ARRAY(p, delta_تلاش_دھاگے);

	اگر (پیش_رفت)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Compressing objects by path"),
						پیش_رفت_nr);
	/* Partition the work amongst work threads. */
	برائے (i = 0; i < delta_تلاش_دھاگے; i++) {
		غیرعلامتی sub_سائز = nr / (delta_تلاش_دھاگے - i);

		p[i].کھڑکی = کھڑکی;
		p[i].گہرائی = گہرائی;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].ڈیٹا_ready = 0;

		p[i].regions = regions;
		p[i].list_سائز = sub_سائز;
		p[i].بقایا = sub_سائز;

		regions += sub_سائز;
		nr -= sub_سائز;
	}

	/* Start work threads. */
	برائے (i = 0; i < delta_تلاش_دھاگے; i++) {
		اگر (!p[i].list_سائز)
			جاری;
		pthread_mutex_init(&p[i].mutex, تہی);
		pthread_cond_init(&p[i].cond, تہی);
		ret = pthread_create(&p[i].thread, تہی,
				     threaded_ڈھونڈو_deltas_by_راستہ, &p[i]);
		اگر (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		فعال_دھاگے++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	جب (فعال_دھاگے) {
		ساخت thread_params *ہدف = تہی;
		ساخت thread_params *victim = تہی;
		غیرعلامتی sub_سائز = 0;

		پیش_رفت_تالا();
		برائے (;;) {
			برائے (i = 0; !ہدف && i < delta_تلاش_دھاگے; i++)
				اگر (!p[i].working)
					ہدف = &p[i];
			اگر (ہدف)
				توڑو;
			pthread_cond_wait(&پیش_رفت_cond, &پیش_رفت_mutex);
		}

		برائے (i = 0; i < delta_تلاش_دھاگے; i++)
			اگر (p[i].بقایا > 2*کھڑکی &&
			    (!victim || victim->بقایا < p[i].بقایا))
				victim = &p[i];
		اگر (victim) {
			sub_سائز = victim->بقایا / 2;
			ہدف->regions = victim->regions + victim->بقایا - sub_سائز;
			victim->list_سائز -= sub_سائز;
			victim->بقایا -= sub_سائز;
		}
		ہدف->list_سائز = sub_سائز;
		ہدف->بقایا = sub_سائز;
		ہدف->working = 1;
		پیش_رفت_unlock();

		pthread_mutex_تالا(&ہدف->mutex);
		ہدف->ڈیٹا_ready = 1;
		pthread_cond_signal(&ہدف->cond);
		pthread_mutex_unlock(&ہدف->mutex);

		اگر (!sub_سائز) {
			pthread_join(ہدف->thread, تہی);
			pthread_cond_destroy(&ہدف->cond);
			pthread_mutex_destroy(&ہدف->mutex);
			فعال_دھاگے--;
		}
	}
	cleanup_threaded_تلاش();
	آزاد(p);

	display_پیش_رفت(پیش_رفت_state, پیش_رفت_nr);
	stop_پیش_رفت(&پیش_رفت_state);
}

ساکن خالی prepare_pack(صحیح کھڑکی, صحیح گہرائی)
{
	ساخت object_اندراج **delta_list;
	uint32_t i, nr_deltas;
	غیرعلامتی n;

	اگر (use_delta_islands)
		resolve_درخت_islands(the_repository, پیش_رفت, &مقصد_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	اگر (!pack_مقصد_stdout)
		do_check_packed_object_چکری_جانچ = 1;

	اگر (!مقصد_pack.nr_objects || !کھڑکی || !گہرائی)
		واپس;

	اگر (راستہ_walk)
		ll_ڈھونڈو_deltas_by_region(مقصد_pack.objects, مقصد_pack.regions,
					 0, مقصد_pack.nr_regions);

	ALLOC_ARRAY(delta_list, مقصد_pack.nr_objects);
	nr_deltas = n = 0;

	برائے (i = 0; i < مقصد_pack.nr_objects; i++) {
		ساخت object_اندراج *اندراج_2 = مقصد_pack.objects + i;

		اگر (!should_attempt_deltas(اندراج_2))
			جاری;

		اگر (!اندراج_2->preferred_بنیاد)
			nr_deltas++;

		delta_list[n++] = اندراج_2;
	}

	اگر (nr_deltas && n > 1) {
		غیرعلامتی nr_مکمل = 0;

		اگر (پیش_رفت)
			پیش_رفت_state = آغاز_پیش_رفت(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_سائز_ترتیب_دو);
		ll_ڈھونڈو_deltas(delta_list, n, کھڑکی+1, گہرائی, &nr_مکمل);
		stop_پیش_رفت(&پیش_رفت_state);
		اگر (nr_مکمل != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	آزاد(delta_list);
}

ساکن صحیح git_pack_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(k, "pack.window")) {
		کھڑکی = git_config_int(k, v, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.windowmemory")) {
		کھڑکی_حافظہ_حد_بندی = git_config_ulong(k, v, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.depth")) {
		گہرائی = git_config_int(k, v, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_سائز = git_config_سائز_t(k, v, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_سائز = git_config_int(k, v, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.writebitmaphashcache")) {
		اگر (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		ورنہ
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	اگر (!strcmp(k, "pack.writebitmaplookuptable")) {
		اگر (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_جدول;
		ورنہ
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_جدول;
	}

	اگر (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_اشاریہ_default = git_config_bool(k, v);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.allowpackreuse")) {
		صحیح res = git_parse_maybe_bool_متن(v);
		اگر (res < 0) {
			اگر (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			ورنہ اگر (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			ورنہ
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} ورنہ اگر (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} ورنہ {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		واپس 0;
	}
	اگر (!strcmp(k, "pack.threads")) {
		delta_تلاش_دھاگے = git_config_int(k, v, ctx->kvi);
		اگر (delta_تلاش_دھاگے < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_تلاش_دھاگے);
		اگر (!HAVE_دھاگے && delta_تلاش_دھاگے != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_تلاش_دھاگے = 0;
		}
		واپس 0;
	}
	اگر (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.نسخہ = git_config_int(k, v, ctx->kvi);
		اگر (pack_idx_opts.نسخہ > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.نسخہ);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.writereverseindex")) {
		اگر (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		ورنہ
			pack_idx_opts.flags &= ~WRITE_REV;
		واپس 0;
	}
	اگر (!strcmp(k, "uploadpack.blobpackfileuri")) {
		ساخت configured_exclusion *ex;
		ثابت حرف *oid_انتہا, *pack_انتہا;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		ساخت object_id pack_hash;

		اگر (!v)
			واپس config_error_nonbool(k);

		ex = xmalloc(حجم(*ex));
		اگر (parse_oid_hex(v, &ex->e.oid, &oid_انتہا) ||
		    *oid_انتہا != ' ' ||
		    parse_oid_hex(oid_انتہا + 1, &pack_hash, &pack_انتہا) ||
		    *pack_انتہا != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		اگر (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_انتہا - oid_انتہا);
		حافظہ_نقل(ex->pack_hash_hex, oid_انتہا + 1, pack_انتہا - oid_انتہا - 1);
		ex->uri = xstrdup(pack_انتہا + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	واپس git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ساکن صحیح stdin_packs_ملا_nr;
ساکن صحیح stdin_packs_hints_nr;

ساکن صحیح جوڑو_object_اندراج_from_pack(ثابت ساخت object_id *oid,
				      ساخت packed_git *p,
				      uint32_t pos,
				      خالی *ڈیٹا_2)
{
	off_t ofs;
	ساخت object_info oi = OBJECT_INFO_INIT;
	شمار object_type type = OBJ_NONE;

	display_پیش_رفت(پیش_رفت_state, ++nr_نظر_آیا);

	اگر (have_duplicate_اندراج(oid, 0))
		واپس 0;

	stdin_packs_ملا_nr++;

	ofs = nth_packed_object_انحراف(p, pos);

	oi.typep = &type;
	اگر (packed_object_info(تہی, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_مقصد_hex(oid), p->pack_name);
	} ورنہ اگر (type == OBJ_COMMIT) {
		ساخت rev_info *revs = ڈیٹا_2;
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
		جوڑو_pending_oid(revs, تہی, oid, 0);
	}

	اگر (!want_object_اندر_pack(oid, 0, &p, &ofs))
		واپس 0;

	create_object_اندراج(oid, type, 0, 0, 0, p, ofs);

	واپس 0;
}

ساکن خالی show_object_pack_hint(ساخت object *object, ثابت حرف *name,
				  خالی *ڈیٹا)
{
	شمار stdin_packs_منوال منوال = *(شمار stdin_packs_منوال *)ڈیٹا;
	اگر (منوال == STDIN_PACKS_منوال_FOLLOW) {
		اگر (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			واپس;
		جوڑو_object_اندراج(&object->oid, object->type, name, 0);
	} ورنہ {
		ساخت object_اندراج *oe = packlist_ڈھونڈو(&مقصد_pack, &object->oid);
		اگر (!oe)
			واپس;

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

ساکن خالی show_commit_pack_hint(ساخت commit *commit, خالی *ڈیٹا)
{
	شمار stdin_packs_منوال منوال = *(شمار stdin_packs_منوال *)ڈیٹا;

	اگر (منوال == STDIN_PACKS_منوال_FOLLOW) {
		show_object_pack_hint((ساخت object *)commit, "", ڈیٹا);
		واپس;
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
شمار stdin_pack_info_قسم {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

ساخت stdin_pack_info {
	ساخت packed_git *p;
	شمار stdin_pack_info_قسم قسم;
};

ساکن صحیح pack_mtime_cmp(ثابت خالی *_a, ثابت خالی *_b)
{
	ساخت stdin_pack_info *a = ((ثابت ساخت string_list_شے*)_a)->util;
	ساخت stdin_pack_info *b = ((ثابت ساخت string_list_شے*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	اگر (a->p->mtime < b->p->mtime)
		واپس 1;
	ورنہ اگر (b->p->mtime < a->p->mtime)
		واپس -1;
	ورنہ
		واپس 0;
}

ساکن صحیح stdin_packs_include_check_obj(ساخت object *obj, خالی *ڈیٹا غیر_مستعمل)
{
	واپس !has_object_kept_pack(مقصد_pack.repo, &obj->oid,
				     KEPT_PACK_اندر_CORE);
}

ساکن صحیح stdin_packs_include_check(ساخت commit *commit, خالی *ڈیٹا)
{
	واپس stdin_packs_include_check_obj((ساخت object *)commit, ڈیٹا);
}

ساکن خالی stdin_packs_جوڑو_pack_entries(ساخت strmap *packs,
					 ساخت rev_info *revs)
{
	ساخت string_list کلیدیں = STRING_LIST_INIT_NODUP;
	ساخت string_list_شے *شے;
	ساخت hashmap_iter iter;
	ساخت strmap_اندراج *اندراج_2;

	strmap_for_each_اندراج(packs, &iter, اندراج_2) {
		ساخت stdin_pack_info *info = اندراج_2->قدر;
		اگر (!info->p)
			die(_("could not find pack '%s'"), اندراج_2->کلید);

		string_list_append(&کلیدیں, اندراج_2->کلید)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(کلیدیں.اشیاء, کلیدیں.nr, pack_mtime_cmp);

	for_each_string_list_شے(شے, &کلیدیں) {
		ساخت stdin_pack_info *info = شے->util;

		اگر (info->قسم & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		اگر ((info->قسم & STDIN_PACK_INCLUDE) ||
		    (info->قسم & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_اندر_pack(info->p,
						جوڑو_object_اندراج_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ترتیب);
	}

	string_list_clear(&کلیدیں, 0);
}

ساکن خالی stdin_packs_read_داخلہ(ساخت rev_info *revs,
				   شمار stdin_packs_منوال منوال)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت strmap packs = STRMAP_INIT;
	ساخت packed_git *p;

	جب (strbuf_getline(&buf, stdin) != EOF) {
		ساخت stdin_pack_info *info;
		شمار stdin_pack_info_قسم قسم = STDIN_PACK_INCLUDE;
		ثابت حرف *کلید = buf.buf;

		اگر (!*کلید)
			جاری;
		ورنہ اگر (*کلید == '^')
			قسم = STDIN_PACK_EXCLUDE_CLOSED;
		ورنہ اگر (*کلید == '!' && منوال == STDIN_PACKS_منوال_FOLLOW)
			قسم = STDIN_PACK_EXCLUDE_OPEN;

		اگر (قسم != STDIN_PACK_INCLUDE)
			کلید++;

		info = strmap_get(&packs, کلید);
		اگر (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, کلید, info);
		}

		info->قسم |= قسم;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		ساخت stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		اگر (!info)
			جاری;

		اگر (info->قسم & STDIN_PACK_INCLUDE) {
			اگر (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			اگر (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		اگر (info->قسم & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_اندر_core = 1;
		}

		اگر (info->قسم & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_اندر_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_جوڑو_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

ساکن خالی جوڑو_unreachable_loose_objects(ساخت rev_info *revs);

ساکن خالی read_stdin_packs(ساخت repository *repo,
			     شمار stdin_packs_منوال منوال, صحیح rev_list_unpacked)
{
	صحیح پچھلا_fetch_if_missing = repo->fetch_if_missing;
	ساخت rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, تہی);
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
	revs.keep_pack_cache_flags |= KEPT_PACK_اندر_CORE;
	revs.blob_objects = 1;
	revs.درخت_objects = 1;
	revs.لیبل_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_اندر_core = 1;
	اگر (منوال == STDIN_PACKS_منوال_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_اندر_core_open = 1;
	}
	stdin_packs_read_داخلہ(&revs, منوال);
	اگر (rev_list_unpacked)
		جوڑو_unreachable_loose_objects(&revs);

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &منوال);

	release_revisions(&revs);

	trace2_ڈیٹا_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_ملا_nr);
	trace2_ڈیٹا_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = پچھلا_fetch_if_missing;
}

ساکن خالی جوڑو_cruft_object_اندراج(ثابت ساخت object_id *oid, شمار object_type type,
				   ساخت packed_git *pack, off_t انحراف,
				   ثابت حرف *name, uint32_t mtime)
{
	ساخت object_اندراج *اندراج_2;

	display_پیش_رفت(پیش_رفت_state, ++nr_نظر_آیا);

	اندراج_2 = packlist_ڈھونڈو(&مقصد_pack, oid);
	اگر (اندراج_2) {
		اگر (name) {
			اندراج_2->hash = pack_name_hash_fn(name);
			اندراج_2->no_try_delta = no_try_delta(name);
		}
	} ورنہ {
		اگر (!want_object_اندر_pack_mtime(oid, 0, &pack, &انحراف, mtime))
			واپس;
		اگر (!pack && type == OBJ_BLOB) {
			ساخت odb_ماخذ_files *files =
				odb_ماخذ_files_downcast(the_repository->objects->ماخذ);
			صحیح ملا_2 = 0;

			برائے (ساخت odb_files_dir *dir = files->dirs; !ملا_2 && dir; dir = dir->اگلا)
				اگر (!odb_ماخذ_read_object_info(&dir->loose->بنیاد, oid, تہی, 0, تہی))
					ملا_2 = 1;

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
			اگر (!ملا_2)
				واپس;
		}

		اندراج_2 = create_object_اندراج(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, انحراف);
	}

	اگر (mtime > oe_cruft_mtime(&مقصد_pack, اندراج_2))
		oe_سیٹ_cruft_mtime(&مقصد_pack, اندراج_2, mtime);
	واپس;
}

ساکن خالی show_cruft_object(ساخت object *obj, ثابت حرف *name, خالی *ڈیٹا غیر_مستعمل)
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
	جوڑو_cruft_object_اندراج(&obj->oid, obj->type, تہی, 0, name, cruft_expiration);
}

ساکن خالی show_cruft_commit(ساخت commit *commit, خالی *ڈیٹا)
{
	show_cruft_object((ساخت object*)commit, تہی, ڈیٹا);
}

ساکن صحیح cruft_include_check_obj(ساخت object *obj, خالی *ڈیٹا غیر_مستعمل)
{
	واپس !has_object_kept_pack(مقصد_pack.repo, &obj->oid, KEPT_PACK_اندر_CORE);
}

ساکن صحیح cruft_include_check(ساخت commit *commit, خالی *ڈیٹا)
{
	واپس cruft_include_check_obj((ساخت object*)commit, ڈیٹا);
}

ساکن خالی سیٹ_cruft_mtime(ثابت ساخت object *object,
			    ساخت packed_git *pack,
			    off_t انحراف, وقت_t mtime)
{
	جوڑو_cruft_object_اندراج(&object->oid, object->type, pack, انحراف, تہی,
			       mtime);
}

ساکن خالی mark_pack_kept_اندر_core(ساخت string_list *packs, غیرعلامتی keep)
{
	ساخت string_list_شے *شے = تہی;
	for_each_string_list_شے(شے, packs) {
		ساخت packed_git *p = شے->util;
		اگر (!p)
			die(_("could not find pack '%s'"), شے->string);
		اگر (p->is_cruft && keep)
			ignore_packed_keep_اندر_core_has_cruft = 1;
		p->pack_keep_اندر_core = keep;
	}
}

ساکن خالی جوڑو_objects_اندر_unpacked_packs(خالی);

ساکن خالی enumerate_cruft_objects(خالی)
{
	اگر (پیش_رفت)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Enumerating cruft objects"), 0);

	جوڑو_objects_اندر_unpacked_packs();
	جوڑو_unreachable_loose_objects(تہی);

	stop_پیش_رفت(&پیش_رفت_state);
}

ساکن خالی enumerate_and_traverse_cruft_objects(ساخت string_list *fresh_packs)
{
	ساخت packed_git *p;
	ساخت rev_info revs;
	صحیح ret;

	repo_init_revisions(the_repository, &revs, تہی);

	revs.لیبل_objects = 1;
	revs.درخت_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	اگر (پیش_رفت)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = جوڑو_unseen_recent_objects_مقصد_traversal(&revs, cruft_expiration,
						     سیٹ_cruft_mtime, 1);
	stop_پیش_رفت(&پیش_رفت_state);

	اگر (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_اندر_core = 0;
	mark_pack_kept_اندر_core(fresh_packs, 1);

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	اگر (پیش_رفت)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Traversing cruft objects"), 0);
	nr_نظر_آیا = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, تہی);

	stop_پیش_رفت(&پیش_رفت_state);
	release_revisions(&revs);
}

ساکن خالی read_cruft_objects(خالی)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت string_list discard_packs = STRING_LIST_INIT_DUP;
	ساخت string_list fresh_packs = STRING_LIST_INIT_DUP;
	ساخت packed_git *p;

	ignore_packed_keep_اندر_core = 1;

	جب (strbuf_getline(&buf, stdin) != EOF) {
		اگر (!buf.len)
			جاری;

		اگر (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		ورنہ
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_ترتیب_دو(&discard_packs);
	string_list_ترتیب_دو(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *pack_name = pack_basename(p);
		ساخت string_list_شے *شے;

		شے = string_list_lookup(&fresh_packs, pack_name);
		اگر (!شے)
			شے = string_list_lookup(&discard_packs, pack_name);

		اگر (شے) {
			شے->util = p;
		} ورنہ {
			/*
			 * This pack wasn't mentioned in either the "fresh" or
			 * "discard" list, so the caller didn't know about it.
			 *
			 * Mark it as kept so that its objects are ignored by
			 * add_unseen_recent_objects_to_traversal(). We'll
			 * unmark it before starting the traversal so it doesn't
			 * halt the traversal early.
			 */
			p->pack_keep_اندر_core = 1;
		}
	}

	mark_pack_kept_اندر_core(&fresh_packs, 1);
	mark_pack_kept_اندر_core(&discard_packs, 0);

	اگر (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	ورنہ
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

ساکن خالی read_object_list_from_stdin(خالی)
{
	حرف سطر[GIT_MAX_HEXSZ + 1 + راستہ_MAX + 2];
	ساخت object_id oid;
	ثابت حرف *p;

	برائے (;;) {
		اگر (!fgets(سطر, حجم(سطر), stdin)) {
			اگر (feof(stdin))
				توڑو;
			اگر (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			اگر (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			جاری;
		}
		اگر (سطر[0] == '-') {
			اگر (get_oid_hex(سطر+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    سطر);
			جوڑو_preferred_بنیاد(&oid);
			جاری;
		}
		اگر (parse_oid_hex(سطر, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), سطر);

		جوڑو_preferred_بنیاد_object(p + 1);
		جوڑو_object_اندراج(&oid, OBJ_NONE, p + 1, 0);
	}
}

ساکن خالی show_commit(ساخت commit *commit, خالی *ڈیٹا غیر_مستعمل)
{
	جوڑو_object_اندراج(&commit->object.oid, OBJ_COMMIT, تہی, 0);

	اگر (write_bitmap_اشاریہ)
		اشاریہ_commit_for_bitmap(commit);

	اگر (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ساکن خالی show_object(ساخت object *obj, ثابت حرف *name,
			خالی *ڈیٹا غیر_مستعمل)
{
	جوڑو_preferred_بنیاد_object(name);
	جوڑو_object_اندراج(&obj->oid, obj->type, name, 0);

	اگر (use_delta_islands)
		ریکارڈ_درخت_گہرائی(&obj->oid, name);
}

ساکن خالی show_object__ma_allow_any(ساخت object *obj, ثابت حرف *name, خالی *ڈیٹا)
{
	أكد(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	اگر (!odb_has_object(the_repository->objects, &obj->oid, 0))
		واپس;

	show_object(obj, name, ڈیٹا);
}

ساکن خالی show_object__ma_allow_promisor(ساخت object *obj, ثابت حرف *name, خالی *ڈیٹا)
{
	أكد(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	اگر (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(مقصد_pack.repo, &obj->oid))
		واپس;

	show_object(obj, name, ڈیٹا);
}

ساکن صحیح option_parse_missing_action(ثابت ساخت option *opt,
				       ثابت حرف *arg, صحیح unset)
{
	ساخت repository *repo = opt->قدر;

	أكد(arg);
	أكد(!unset);

	اگر (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		واپس 0;
	}

	اگر (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		واپس 0;
	}

	اگر (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		واپس 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	واپس 0;
}

ساکن خالی show_کنارہ(ساخت commit *commit)
{
	جوڑو_preferred_بنیاد(&commit->object.oid);
}

ساکن صحیح جوڑو_object_اندر_unpacked_pack(ثابت ساخت object_id *oid,
				       ساخت object_info *oi,
				       خالی *ڈیٹا غیر_مستعمل)
{
	اگر (cruft) {
		جوڑو_cruft_object_اندراج(oid, OBJ_NONE, oi->ماخذ_infop->u.packed.pack,
				       oi->ماخذ_infop->u.packed.انحراف, تہی,
				       *oi->mtimep);
	} ورنہ {
		جوڑو_object_اندراج(oid, OBJ_NONE, "", 0);
	}
	واپس 0;
}

ساکن خالی جوڑو_objects_اندر_unpacked_packs(خالی)
{
	ساخت odb_ماخذ_files *files =
		odb_ماخذ_files_downcast(مقصد_pack.repo->objects->ماخذ);
	وقت_t mtime;
	ساخت odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ترتیب |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_اندر_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	ساخت odb_ماخذ_info ماخذ_info;
	ساخت object_info oi = {
		.mtimep = &mtime,
		.ماخذ_infop = &ماخذ_info,
	};

	برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		اگر (!dir->local)
			جاری;

		اگر (odb_ماخذ_for_each_object(&dir->packed->بنیاد, &oi,
					       جوڑو_object_اندر_unpacked_pack, تہی, &opts))
			die(_("cannot open pack index"));
	}
}

ساکن صحیح جوڑو_loose_object(ثابت ساخت object_id *oid, ثابت حرف *راستہ,
			    خالی *ڈیٹا)
{
	ساخت rev_info *revs = ڈیٹا;
	شمار object_type type = odb_read_object_info(the_repository->objects, oid, تہی);

	اگر (type < 0) {
		warning(_("loose object at %s could not be examined"), راستہ);
		واپس 0;
	}

	اگر (cruft) {
		ساخت stat st;
		اگر (stat(راستہ, &st) < 0) {
			اگر (errno == ENOENT)
				واپس 0;
			واپس error_errno("unable to stat %s", oid_مقصد_hex(oid));
		}

		جوڑو_cruft_object_اندراج(oid, type, تہی, 0, تہی,
				       st.st_mtime);
	} ورنہ {
		جوڑو_object_اندراج(oid, type, "", 0);
	}

	اگر (revs && type == OBJ_COMMIT)
		جوڑو_pending_oid(revs, تہی, oid, 0);

	واپس 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ساکن خالی جوڑو_unreachable_loose_objects(ساخت rev_info *revs)
{
	for_each_loose_file_اندر_ماخذ(the_repository->objects->ماخذ,
				      جوڑو_loose_object, تہی, تہی, revs);
}

ساکن صحیح has_sha1_pack_kept_or_nonlocal(ثابت ساخت object_id *oid)
{
	ساکن ساخت packed_git *آخری_ملا = تہی;
	ساخت packed_git *p;

	اگر (آخری_ملا && ڈھونڈو_pack_اندراج_one(oid, آخری_ملا))
		واپس 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		اگر (p == آخری_ملا)
			جاری;

		اگر ((!p->pack_local || p->pack_keep || p->pack_keep_اندر_core) &&
		    ڈھونڈو_pack_اندراج_one(oid, p)) {
			آخری_ملا = p;
			واپس 1;
		}
	}

	واپس 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
ساکن ساخت oid_array recent_objects;

ساکن صحیح loosened_object_can_be_discarded(ثابت ساخت object_id *oid,
					    timestamp_t mtime)
{
	اگر (!unpack_unreachable_expiration)
		واپس 0;
	اگر (mtime > unpack_unreachable_expiration)
		واپس 0;
	اگر (oid_array_lookup(&recent_objects, oid) >= 0)
		واپس 0;
	واپس 1;
}

ساکن صحیح force_object_loose(ساخت odb_ماخذ *ماخذ,
			      ثابت ساخت object_id *oid,
			      ثابت وقت_t *mtime)
{
	ساخت odb_ماخذ_files *files = odb_ماخذ_files_downcast(ماخذ);
	ثابت ساخت git_hash_algo *compat = ماخذ->odb->repo->compat_hash_algo;
	ساخت object_info oi = OBJECT_INFO_INIT;
	ساخت object_id compat_oid, *compat_oid_p = تہی;
	شمار object_type type;
	خالی *buf = تہی;
	سائز_t len;
	صحیح ret;

	برائے (ساخت odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا)
		اگر (!odb_ماخذ_read_object_info(&dir->loose->بنیاد, oid, تہی, 0, تہی))
			واپس 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	اگر (odb_read_object_info_extended(ماخذ->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_مقصد_hex(oid));
		جاؤ برآمد;
	}

	اگر (compat) {
		اگر (repo_oid_مقصد_algop(ماخذ->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_مقصد_hex(oid), compat->name);
			جاؤ برآمد;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_ماخذ_write_object(&files->dirs->loose->بنیاد, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

برآمد:
	آزاد(buf);
	واپس ret;
}

ساکن خالی loosen_غیر_مستعمل_packed_objects(خالی)
{
	ساخت packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	ساخت object_id oid;

	repo_for_each_pack(the_repository, p) {
		اگر (!p->pack_local || p->pack_keep || p->pack_keep_اندر_core)
			جاری;

		اگر (open_pack_اشاریہ(p))
			die(_("cannot open pack index"));

		برائے (i = 0; i < p->عدد_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			اگر (!packlist_ڈھونڈو(&مقصد_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				اگر (force_object_loose(the_repository->objects->ماخذ,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_ڈیٹا_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
ساکن صحیح pack_options_allow_reuse(خالی)
{
	واپس allow_pack_reuse != NO_PACK_REUSE &&
	       pack_مقصد_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_اندر_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

ساکن صحیح get_object_list_from_bitmap(ساخت rev_info *revs)
{
	اگر (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		واپس -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_نسخہ = 1;

	اگر (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	اگر (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		اگر (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_نتیجہ += reuse_packfile_objects;
		nr_نظر_آیا += reuse_packfile_objects;
		display_پیش_رفت(پیش_رفت_state, nr_نظر_آیا);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &جوڑو_object_اندراج_from_bitmap);
	واپس 0;
}

ساکن خالی ریکارڈ_recent_object(ساخت object *obj,
				 ثابت حرف *name غیر_مستعمل,
				 خالی *ڈیٹا غیر_مستعمل)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ساکن خالی ریکارڈ_recent_commit(ساخت commit *commit, خالی *ڈیٹا غیر_مستعمل)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ساکن صحیح mark_bitmap_preferred_tip(ثابت ساخت reference *ref, خالی *ڈیٹا غیر_مستعمل)
{
	ثابت ساخت object_id *maybe_peeled = ref->oid;
	ساخت object_id peeled;
	ساخت object *object;

	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	اگر (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	واپس 0;
}

ساکن اندرونی صحیح is_oid_uninteresting(ساخت repository *repo,
				       ساخت object_id *oid)
{
	ساخت object *o = lookup_object(repo, oid);
	واپس !o || (o->flags & UNINTERESTING);
}

ساکن صحیح جوڑو_objects_by_راستہ(ثابت حرف *راستہ,
			       ساخت oid_array *oids,
			       شمار object_type type,
			       خالی *ڈیٹا)
{
	سائز_t oe_آغاز = مقصد_pack.nr_objects;
	سائز_t oe_انتہا;
	غیرعلامتی صحیح *processed = ڈیٹا;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	برائے (سائز_t i = 0; i < oids->nr; i++) {
		صحیح exclude;
		ساخت object_info oi = OBJECT_INFO_INIT;
		ساخت object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		اگر ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			جاری;

		exclude = is_oid_uninteresting(the_repository, oid);

		اگر (exclude && !thin)
			جاری;

		جوڑو_object_اندراج(oid, type, راستہ, exclude);

		اگر (type == OBJ_COMMIT) {
			ساخت commit *commit;

			اگر (!write_bitmap_اشاریہ && !use_delta_islands)
				جاری;

			commit = lookup_commit(the_repository, oid);
			اگر (!commit)
				die(_("could not find commit %s"), oid_مقصد_hex(oid));
			اگر (write_bitmap_اشاریہ)
				اشاریہ_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			اگر (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} ورنہ اگر (type == OBJ_درخت && use_delta_islands) {
			ریکارڈ_درخت_گہرائی(oid, راستہ);
		}
	}

	oe_انتہا = مقصد_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	اگر (oe_انتہا == oe_آغاز || !کھڑکی)
		واپس 0;

	ALLOC_GROW(مقصد_pack.regions,
		   مقصد_pack.nr_regions + 1,
		   مقصد_pack.nr_regions_alloc);

	مقصد_pack.regions[مقصد_pack.nr_regions].آغاز = oe_آغاز;
	مقصد_pack.regions[مقصد_pack.nr_regions].nr = oe_انتہا - oe_آغاز;
	مقصد_pack.nr_regions++;

	*processed += oids->nr;
	display_پیش_رفت(پیش_رفت_state, *processed);

	واپس 0;
}

ساکن صحیح get_object_list_راستہ_walk(ساخت rev_info *revs)
{
	ساخت راستہ_walk_info info = راستہ_WALK_INFO_INIT;
	غیرعلامتی صحیح processed = 0;
	صحیح نتیجہ;

	info.revs = revs;
	info.راستہ_fn = جوڑو_objects_by_راستہ;
	info.راستہ_fn_ڈیٹا = &processed;

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
	info.کنارہ_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	نتیجہ = walk_objects_by_راستہ(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	راستہ_walk_info_clear(&info);

	واپس نتیجہ;
}

ساکن خالی get_object_list(ساخت rev_info *revs, ساخت strvec *argv)
{
	ساخت تیاری_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	حرف سطر[1000];
	صحیح flags = 0;
	صحیح save_warning;

	save_commit_بفر = 0;
	تیاری_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	جب (fgets(سطر, حجم(سطر), stdin) != تہی) {
		صحیح len = لڑی_لمبائی(سطر);
		اگر (len && سطر[len - 1] == '\n')
			سطر[--len] = 0;
		اگر (!len)
			توڑو;
		اگر (*سطر == '-') {
			اگر (!strcmp(سطر, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_اشاریہ = 0;
				جاری;
			}
			اگر (starts_with(سطر, "--shallow ")) {
				ساخت object_id oid;
				اگر (get_oid_hex(سطر + 10, &oid))
					die("not an object name '%s'", سطر + 10);
				رجسٹر_shallow(the_repository, &oid);
				use_bitmap_اشاریہ = 0;
				جاری;
			}
			die(_("not a rev '%s'"), سطر);
		}
		اگر (handle_revision_arg(سطر, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), سطر);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	اگر (use_bitmap_اشاریہ && !get_object_list_from_bitmap(revs))
		واپس;

	اگر (use_delta_islands)
		بھرو_delta_islands(the_repository, پیش_رفت);

	اگر (write_bitmap_اشاریہ)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      تہی);

	اگر (!fn_show_object)
		fn_show_object = show_object;

	اگر (راستہ_walk) {
		اگر (get_object_list_راستہ_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			راستہ_walk = 0;
		}
	}

	اگر (!راستہ_walk) {
		اگر (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_کنارے_uninteresting(revs, show_کنارہ, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				تہی);
	}

	اگر (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		اگر (جوڑو_unseen_recent_objects_مقصد_traversal(revs,
				unpack_unreachable_expiration, تہی, 0))
			die(_("unable to add recent objects"));
		اگر (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, ریکارڈ_recent_commit,
				     ریکارڈ_recent_object, تہی);
	}

	اگر (keep_unreachable)
		جوڑو_objects_اندر_unpacked_packs();
	اگر (pack_loose_unreachable)
		جوڑو_unreachable_loose_objects(تہی);
	اگر (unpack_unreachable)
		loosen_غیر_مستعمل_packed_objects();

	oid_array_clear(&recent_objects);
}

ساکن خالی جوڑو_extra_kept_packs(ثابت ساخت string_list *نام)
{
	ساخت packed_git *p;

	اگر (!نام->nr)
		واپس;

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *name = basename(p->pack_name);
		صحیح i;

		اگر (!p->pack_local)
			جاری;

		برائے (i = 0; i < نام->nr; i++)
			اگر (!fspathcmp(name, نام->اشیاء[i].string))
				توڑو;

		اگر (i < نام->nr) {
			p->pack_keep_اندر_core = 1;
			ignore_packed_keep_اندر_core = 1;
			جاری;
		}
	}
}

ساکن صحیح option_parse_quiet(ثابت ساخت option *opt, ثابت حرف *arg,
			      صحیح unset)
{
	صحیح *val = opt->قدر;

	BUG_ON_OPT_ARG(arg);

	اگر (!unset)
		*val = 0;
	ورنہ اگر (!*val)
		*val = 1;
	واپس 0;
}

ساکن صحیح option_parse_اشاریہ_نسخہ(ثابت ساخت option *opt,
				      ثابت حرف *arg, صحیح unset)
{
	ساخت pack_idx_option *popts = opt->قدر;
	حرف *c;
	ثابت حرف *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->نسخہ = strtoul(val, &c, 10);
	اگر (popts->نسخہ > 2)
		die(_("unsupported index version %s"), val);
	اگر (*c == ',' && c[1])
		popts->off32_حد_بندی = strtoul(c+1, &c, 0);
	اگر (*c || popts->off32_حد_بندی & 0x80000000)
		die(_("bad index version '%s'"), val);
	واپس 0;
}

ساکن صحیح option_parse_unpack_unreachable(ثابت ساخت option *opt غیر_مستعمل,
					   ثابت حرف *arg, صحیح unset)
{
	اگر (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	ورنہ {
		unpack_unreachable = 1;
		اگر (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	واپس 0;
}

ساکن صحیح option_parse_cruft_expiration(ثابت ساخت option *opt غیر_مستعمل,
					 ثابت حرف *arg, صحیح unset)
{
	اگر (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} ورنہ {
		cruft = 1;
		اگر (arg)
			cruft_expiration = approxidate(arg);
	}
	واپس 0;
}

ساکن صحیح is_not_اندر_promisor_pack_obj(ساخت object *obj, خالی *ڈیٹا غیر_مستعمل)
{
	ساخت odb_ماخذ_info ماخذ_info;
	ساخت object_info info = {
		.ماخذ_infop = &ماخذ_info,
	};

	اگر (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	واپس ماخذ_info.ماخذ->type != ODB_ماخذ_PACKED || !ماخذ_info.u.packed.pack->pack_promisor;
}

ساکن صحیح is_not_اندر_promisor_pack(ساخت commit *commit, خالی *ڈیٹا) {
	واپس is_not_اندر_promisor_pack_obj((ساخت object *) commit, ڈیٹا);
}

ساکن صحیح parse_stdin_packs_منوال(ثابت ساخت option *opt, ثابت حرف *arg,
				  صحیح unset)
{
	شمار stdin_packs_منوال *منوال = opt->قدر;

	اگر (unset)
		*منوال = STDIN_PACKS_منوال_NONE;
	ورنہ اگر (!arg || !*arg)
		*منوال = STDIN_PACKS_منوال_STANDARD;
	ورنہ اگر (!strcmp(arg, "follow"))
		*منوال = STDIN_PACKS_منوال_FOLLOW;
	ورنہ
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	واپس 0;
}

صحیح cmd_pack_objects(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo)
{
	صحیح use_اندرونی_rev_list = 0;
	صحیح all_پیش_رفت_implied = 0;
	ساخت strvec rp = STRVEC_INIT;
	صحیح rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	صحیح rev_list_اشاریہ = 0;
	شمار stdin_packs_منوال stdin_packs = STDIN_PACKS_منوال_NONE;
	ساخت string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	ساخت list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	ساخت option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &پیش_رفت, تہی,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_سیٹ_INT(0, "progress", &پیش_رفت,
			    N_("show progress meter"), 1),
		OPT_سیٹ_INT(0, "all-progress", &پیش_رفت,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_پیش_رفت_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_اشاریہ_نسخہ),
		OPT_UNSIGNED(0, "max-pack-size", &pack_سائز_حد_بندی,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &کھڑکی,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &کھڑکی_حافظہ_حد_بندی,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &گہرائی,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_تلاش_دھاگے,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_خالی_خانے,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_اندرونی_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_سیٹ_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_سیٹ_INT_F(0, "indexed-objects", &rev_list_اشاریہ,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_منوال),
		OPT_BOOL(0, "stdout", &pack_مقصد_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_لیبل,
			 N_("include tag objects that refer to objects to be packed")),
		OPT_BOOL(0, "keep-unreachable", &keep_unreachable,
			 N_("keep unreachable objects")),
		OPT_BOOL(0, "pack-loose-unreachable", &pack_loose_unreachable,
			 N_("pack loose unreachable objects")),
		OPT_CALLBACK_F(0, "unpack-unreachable", تہی, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", تہی, N_("time"),
		  N_("expire cruft objects older than <time>"),
		  PARSE_OPT_OPTARG, option_parse_cruft_expiration),
		OPT_BOOL(0, "sparse", &sparse,
			 N_("use the sparse reachability algorithm")),
		OPT_BOOL(0, "thin", &thin,
			 N_("create thin packs")),
		OPT_BOOL(0, "path-walk", &راستہ_walk,
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_اشاریہ,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_سیٹ_INT(0, "write-bitmap-index", &write_bitmap_اشاریہ,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_سیٹ_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_اشاریہ,
			      N_("write a bitmap index if possible"),
			      WRITE_BITMAP_QUIET, PARSE_OPT_HIDDEN),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_CALLBACK_F(0, "missing", repo, N_("action"),
		  N_("handling for missing objects"), PARSE_OPT_NONEG,
		  option_parse_missing_action),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("do not pack objects in promisor packfiles")),
		OPT_BOOL(0, "exclude-promisor-objects-best-effort",
			 &exclude_promisor_objects_بہترین_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_نسخہ,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_انتہا(),
	};

	اگر (DFS_عدد_حالتیں > (1 << OE_DFS_STATE_بٹ))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		اگر (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		اگر (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, تہی);
	اگر (git_env_bool(GIT_TEST_NO_WRITE_REV_اشاریہ, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	پیش_رفت = isatty(2);
	argc = parse_options(argc, argv, سابقہ, pack_objects_options,
			     pack_usage, 0);

	اگر (argc) {
		بنیاد_name = argv[0];
		argc--;
	}
	اگر (pack_مقصد_stdout != !بنیاد_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	اگر (راستہ_walk < 0) {
		اگر (use_bitmap_اشاریہ > 0 ||
		    !use_اندرونی_rev_list)
			راستہ_walk = 0;
		ورنہ اگر (the_repository->gitdir &&
			 the_repository->settings.pack_use_راستہ_walk)
			راستہ_walk = 1;
		ورنہ
			راستہ_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	اگر (گہرائی < 0)
		گہرائی = 0;
	اگر (گہرائی >= (1 << OE_گہرائی_بٹ)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			گہرائی, (1 << OE_گہرائی_بٹ) - 1);
		گہرائی = (1 << OE_گہرائی_بٹ) - 1;
	}
	اگر (cache_max_small_delta_سائز >= (1U << OE_Z_DELTA_بٹ)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_بٹ) - 1);
		cache_max_small_delta_سائز = (1U << OE_Z_DELTA_بٹ) - 1;
	}
	اگر (کھڑکی < 0)
		کھڑکی = 0;

	strvec_ڈالو(&rp, "pack-objects");

	اگر (راستہ_walk) {
		ثابت حرف *option = تہی;
		اگر (!راستہ_walk_filter_compatible(&filter_options))
			option = "--filter";

		اگر (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			راستہ_walk = 0;
		}
	}
	اگر (راستہ_walk) {
		strvec_ڈالو(&rp, "--objects");
	} ورنہ اگر (thin) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} ورنہ
		strvec_ڈالو(&rp, "--objects");

	اگر (rev_list_all) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--all");
	}
	اگر (rev_list_reflog) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--reflog");
	}
	اگر (rev_list_اشاریہ) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--indexed-objects");
	}
	اگر (rev_list_unpacked && !stdin_packs) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_بہترین_effort,
				  "--exclude-promisor-objects-best-effort");
	اگر (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		اگر (!stdin_packs) {
			use_اندرونی_rev_list = 1;
			strvec_ڈالو(&rp, "--exclude-promisor-objects");
		}
	} ورنہ اگر (exclude_promisor_objects_بہترین_effort) {
		use_اندرونی_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	اگر (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_اندرونی_rev_list = 1;

	اگر (!reuse_object)
		reuse_delta = 0;
	اگر (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	ورنہ اگر (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_بہترین_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	اگر (!delta_تلاش_دھاگے)	/* --threads=0 means autodetect */
		delta_تلاش_دھاگے = online_cpus();

	اگر (!HAVE_دھاگے && delta_تلاش_دھاگے != 1)
		warning(_("no threads support, ignoring --threads"));
	اگر (!pack_مقصد_stdout && !pack_سائز_حد_بندی)
		pack_سائز_حد_بندی = pack_سائز_حد_بندی_cfg;
	اگر (pack_مقصد_stdout && pack_سائز_حد_بندی)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	اگر (pack_سائز_حد_بندی && pack_سائز_حد_بندی < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_سائز_حد_بندی = 1024*1024;
	}

	اگر (!pack_مقصد_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	اگر (!rev_list_all || !rev_list_reflog || !rev_list_اشاریہ)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	اگر (stdin_packs && use_اندرونی_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	اگر (cruft) {
		اگر (use_اندرونی_rev_list)
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
	اگر (!pack_مقصد_stdout)
		use_bitmap_اشاریہ_default = 0;

	اگر (use_bitmap_اشاریہ < 0)
		use_bitmap_اشاریہ = use_bitmap_اشاریہ_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	اگر (!use_اندرونی_rev_list || (!pack_مقصد_stdout && write_bitmap_اشاریہ) || is_repository_shallow(the_repository))
		use_bitmap_اشاریہ = 0;

	اگر (pack_مقصد_stdout || !rev_list_all)
		write_bitmap_اشاریہ = 0;

	اگر (name_hash_نسخہ < 0)
		name_hash_نسخہ = (صحیح)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_نسخہ();

	اگر (use_delta_islands)
		strvec_ڈالو(&rp, "--topo-order");

	اگر (پیش_رفت && all_پیش_رفت_implied)
		پیش_رفت = 2;

	جوڑو_extra_kept_packs(&keep_pack_list);
	اگر (ignore_packed_keep_on_disk) {
		ساخت packed_git *p;

		repo_for_each_pack(the_repository, p)
			اگر (p->pack_local && p->pack_keep)
				توڑو;
		اگر (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	اگر (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		ساخت packed_git *p;

		repo_for_each_pack(the_repository, p) {
			اگر (!p->pack_local) {
				have_non_local_packs = 1;
				توڑو;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_ڈیٹا(the_repository, &مقصد_pack);

	اگر (پیش_رفت && !cruft)
		پیش_رفت_state = آغاز_پیش_رفت(the_repository,
						_("Enumerating objects"), 0);
	اگر (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} ورنہ اگر (cruft) {
		read_cruft_objects();
	} ورنہ اگر (!use_اندرونی_rev_list) {
		read_object_list_from_stdin();
	} ورنہ {
		ساخت rev_info revs;

		repo_init_revisions(the_repository, &revs, تہی);
		list_objects_filter_copy(&revs.filter, &filter_options);
		اگر (exclude_promisor_objects_بہترین_effort) {
			revs.include_check = is_not_اندر_promisor_pack;
			revs.include_check_obj = is_not_اندر_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_بنیاد();
	اگر (include_لیبل && nr_نتیجہ)
		refs_for_each_لیبل_ref(get_main_ref_store(the_repository),
				      جوڑو_ref_لیبل, تہی);
	stop_پیش_رفت(&پیش_رفت_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	اگر (non_خالی_خانے && !nr_نتیجہ)
		جاؤ cleanup;
	اگر (nr_نتیجہ) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(کھڑکی, گہرائی);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	اگر (پیش_رفت)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_مستعمل_nr);

	trace2_ڈیٹا_intmax("pack-objects", the_repository, "written", written);
	trace2_ڈیٹا_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_ڈیٹا_intmax("pack-objects", the_repository, "reused", reused);
	trace2_ڈیٹا_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_ڈیٹا_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_ڈیٹا_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_مستعمل_nr);

cleanup:
	clear_packing_ڈیٹا(&مقصد_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	واپس 0;
}
