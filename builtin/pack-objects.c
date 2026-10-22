#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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
ساكن بنية packing_انکڑے منزل_pack;

ساكن مضمن بنية object_اندراج *oe_delta(
		ثابت بنية packing_انکڑے *pack,
		ثابت بنية object_اندراج *e)
{
	إذا (!e->delta_idx)
		أرجع عدم;
	إذا (e->ext_ادھار)
		أرجع &pack->ext_bases[e->delta_idx - 1];
	وإلا
		أرجع &pack->objects[e->delta_idx - 1];
}

ساكن مضمن ناپ_t oe_delta_ناپ(بنية packing_انکڑے *pack,
				   ثابت بنية object_اندراج *e)
{
	إذا (e->delta_ناپ_valid)
		أرجع e->delta_ناپ;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	أرجع pack->delta_ناپ_2[e - pack->objects];
}

ناپ_t oe_get_ناپ_slow(بنية packing_انکڑے *pack,
			ثابت بنية object_اندراج *e);

ساكن مضمن ناپ_t oe_ناپ(بنية packing_انکڑے *pack,
			     ثابت بنية object_اندراج *e)
{
	إذا (e->ناپ_valid)
		أرجع e->ناپ_2;

	أرجع oe_get_ناپ_slow(pack, e);
}

ساكن مضمن فراغ oe_سیٹ_delta(بنية packing_انکڑے *pack,
				بنية object_اندراج *e,
				بنية object_اندراج *delta)
{
	إذا (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_idx = 0;
}

ساكن مضمن بنية object_اندراج *oe_delta_sibling(
		ثابت بنية packing_انکڑے *pack,
		ثابت بنية object_اندراج *e)
{
	إذا (e->delta_sibling_idx)
		أرجع &pack->objects[e->delta_sibling_idx - 1];
	أرجع عدم;
}

ساكن مضمن بنية object_اندراج *oe_delta_بچہ(
		ثابت بنية packing_انکڑے *pack,
		ثابت بنية object_اندراج *e)
{
	إذا (e->delta_بچہ_idx)
		أرجع &pack->objects[e->delta_بچہ_idx - 1];
	أرجع عدم;
}

ساكن مضمن فراغ oe_سیٹ_delta_بچہ(بنية packing_انکڑے *pack,
				      بنية object_اندراج *e,
				      بنية object_اندراج *delta)
{
	إذا (delta)
		e->delta_بچہ_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_بچہ_idx = 0;
}

ساكن مضمن فراغ oe_سیٹ_delta_sibling(بنية packing_انکڑے *pack,
					بنية object_اندراج *e,
					بنية object_اندراج *delta)
{
	إذا (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_sibling_idx = 0;
}

ساكن مضمن فراغ oe_سیٹ_ناپ(بنية packing_انکڑے *pack,
			       بنية object_اندراج *e,
			       ناپ_t ناپ)
{
	إذا (ناپ < pack->oe_ناپ_حد_بندی) {
		e->ناپ_2 = ناپ;
		e->ناپ_valid = 1;
	} وإلا {
		e->ناپ_valid = 0;
		إذا (oe_get_ناپ_slow(pack, e) != ناپ)
			BUG("'size' is supposed to be the object size!");
	}
}

ساكن مضمن فراغ oe_سیٹ_delta_ناپ(بنية packing_انکڑے *pack,
				     بنية object_اندراج *e,
				     ناپ_t ناپ)
{
	إذا (ناپ < pack->oe_delta_ناپ_حد_بندی) {
		e->delta_ناپ = ناپ;
		e->delta_ناپ_valid = 1;
	} وإلا {
		packing_انکڑے_جندرا(pack);
		إذا (!pack->delta_ناپ_2)
			ALLOC_ARRAY(pack->delta_ناپ_2, pack->nr_alloc);
		packing_انکڑے_unlock(pack);

		pack->delta_ناپ_2[e - pack->objects] = ناپ;
		e->delta_ناپ_valid = 0;
	}
}

#تعریف_کرو وچ_PACK(obj) oe_وچ_pack(&منزل_pack, obj)
#تعریف_کرو ناپ_3(obj) oe_ناپ(&منزل_pack, obj)
#تعریف_کرو سیٹ_ناپ(obj,ناپ) oe_سیٹ_ناپ(&منزل_pack, obj, ناپ)
#تعریف_کرو DELTA_ناپ(obj) oe_delta_ناپ(&منزل_pack, obj)
#تعریف_کرو DELTA(obj) oe_delta(&منزل_pack, obj)
#تعریف_کرو DELTA_بچہ(obj) oe_delta_بچہ(&منزل_pack, obj)
#تعریف_کرو DELTA_SIBLING(obj) oe_delta_sibling(&منزل_pack, obj)
#تعریف_کرو سیٹ_DELTA(obj, val) oe_سیٹ_delta(&منزل_pack, obj, val)
#تعریف_کرو سیٹ_DELTA_EXT(obj, oid) oe_سیٹ_delta_ext(&منزل_pack, obj, oid)
#تعریف_کرو سیٹ_DELTA_ناپ(obj, val) oe_سیٹ_delta_ناپ(&منزل_pack, obj, val)
#تعریف_کرو سیٹ_DELTA_بچہ(obj, val) oe_سیٹ_delta_بچہ(&منزل_pack, obj, val)
#تعریف_کرو سیٹ_DELTA_SIBLING(obj, val) oe_سیٹ_delta_sibling(&منزل_pack, obj, val)

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

ساكن بنية pack_idx_اندراج **written_list;
ساكن uint32_t nr_نتیجہ, nr_written, nr_تکیا;
ساكن بنية bitmap_اشاریہ *bitmap_git;
ساكن uint32_t write_layer;

ساكن صحيح non_خالی_تھاواں;
ساكن صحيح reuse_delta = 1, reuse_object = 1;
ساكن صحيح keep_unreachable, unpack_unreachable, include_ٹیگ;
ساكن timestamp_t unpack_unreachable_expiration;
ساكن صحيح pack_loose_unreachable;
ساكن صحيح cruft;
ساكن صحيح shallow = 0;
ساكن timestamp_t cruft_expiration;
ساكن صحيح local;
ساكن صحيح have_non_local_packs;
ساكن صحيح incremental;
ساكن صحيح ignore_packed_keep_on_disk;
ساكن صحيح ignore_packed_keep_وچ_core;
ساكن صحيح ignore_packed_keep_وچ_core_open;
ساكن صحيح ignore_packed_keep_وچ_core_has_cruft;
ساكن صحيح allow_ofs_delta;
ساكن بنية pack_idx_option pack_idx_opts;
ساكن ثابت حرف *ادھار_name;
ساكن صحيح ترقی = 1;
ساكن صحيح کھڑکی = 10;
ساكن بنانشانی لما pack_ناپ_حد_بندی;
ساكن صحيح ڈونگھائی = 50;
ساكن صحيح delta_تلاش_دھاگے;
ساكن صحيح pack_منزل_stdout;
ساكن صحيح sparse;
ساكن صحيح thin;
ساكن صحيح رستہ_walk = -1;
ساكن صحيح عدد_preferred_ادھار;
ساكن بنية ترقی *ترقی_state;

ساكن بنية bitmapped_pack *reuse_packfiles;
ساكن ناپ_t reuse_packfiles_nr;
ساكن ناپ_t reuse_packfiles_ورتیا_nr;
ساكن uint32_t reuse_packfile_objects;
ساكن بنية bitmap *reuse_packfile_bitmap;

ساكن صحيح use_bitmap_اشاریہ_default = 1;
ساكن صحيح use_bitmap_اشاریہ = -1;
ساكن تعداد {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ساكن تعداد {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_اشاریہ;
ساكن uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ساكن صحيح exclude_promisor_objects;
ساكن صحيح exclude_promisor_objects_بہترین_effort;

ساكن صحيح use_delta_islands;

ساكن ناپ_t delta_cache_ناپ = 0;
ساكن ناپ_t max_delta_cache_ناپ = DEFAULT_DELTA_CACHE_ناپ;
ساكن بنانشانی لما cache_max_small_delta_ناپ = 1000;

ساكن بنانشانی لما کھڑکی_حافظہ_حد_بندی = 0;

ساكن بنية string_list uri_protocols = STRING_LIST_INIT_NODUP;

تعداد missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساكن تعداد missing_action arg_missing_action;
ساكن show_object_fn fn_show_object;

بنية configured_exclusion {
	بنية oidmap_اندراج e;
	حرف *pack_hash_hex;
	حرف *uri;
};
ساكن بنية oidmap configured_exclusions;

ساكن بنية oidset excluded_by_config;
ساكن صحيح name_hash_نسخہ = -1;

تعداد stdin_packs_منوال {
	STDIN_PACKS_منوال_NONE,
	STDIN_PACKS_منوال_STANDARD,
	STDIN_PACKS_منوال_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ساكن فراغ validate_name_hash_نسخہ(فراغ)
{
	إذا (name_hash_نسخہ < 1 || name_hash_نسخہ > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_نسخہ);
	إذا (write_bitmap_اشاریہ && name_hash_نسخہ != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_نسخہ = 1;
	}
}

ساكن مضمن uint32_t pack_name_hash_fn(ثابت حرف *name)
{
	ساكن صحيح تکیا_نسخہ = -1;

	إذا (تکیا_نسخہ < 0)
		تکیا_نسخہ = name_hash_نسخہ;
	وإلا إذا (تکیا_نسخہ != name_hash_نسخہ)
		BUG("name hash version changed from %d to %d mid-process",
		    تکیا_نسخہ, name_hash_نسخہ);

	بدل (name_hash_نسخہ) {
	حالة 1:
		أرجع pack_name_hash(name);

	حالة 2:
		أرجع pack_name_hash_v2((ثابت بنانشانی حرف *)name);

	افتراضي:
		BUG("invalid name-hash version: %d", name_hash_نسخہ);
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
ساكن بنية commit **اشاریہ_دار_commits;
ساكن بنانشانی صحيح اشاریہ_دار_commits_nr;
ساكن بنانشانی صحيح اشاریہ_دار_commits_alloc;

ساكن فراغ اشاریہ_commit_for_bitmap(بنية commit *commit)
{
	إذا (اشاریہ_دار_commits_nr >= اشاریہ_دار_commits_alloc) {
		اشاریہ_دار_commits_alloc = (اشاریہ_دار_commits_alloc + 32) * 2;
		REALLOC_ARRAY(اشاریہ_دار_commits, اشاریہ_دار_commits_alloc);
	}

	اشاریہ_دار_commits[اشاریہ_دار_commits_nr++] = commit;
}

ساكن فراغ *get_delta(بنية object_اندراج *اندراج)
{
	بنانشانی لما ناپ, ادھار_ناپ;
	ناپ_t delta_ناپ_2;
	فراغ *buf, *ادھار_buf, *delta_buf;
	تعداد object_type type;
	ناپ_t ناپ_st = 0, ادھار_ناپ_st = 0;

	buf = odb_read_object(the_repository->objects, &اندراج->idx.oid,
			      &type, &ناپ_st);
	ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
	إذا (!buf)
		die(_("unable to read %s"), oid_منزل_hex(&اندراج->idx.oid));
	ادھار_buf = odb_read_object(the_repository->objects,
				   &DELTA(اندراج)->idx.oid, &type,
				   &ادھار_ناپ_st);
	ادھار_ناپ = cast_ناپ_t_منزل_ulong(ادھار_ناپ_st);
	إذا (!ادھار_buf)
		die("unable to read %s",
		    oid_منزل_hex(&DELTA(اندراج)->idx.oid));
	delta_buf = diff_delta(ادھار_buf, ادھار_ناپ,
			       buf, ناپ, &delta_ناپ_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	إذا (!delta_buf || delta_ناپ_2 != DELTA_ناپ(اندراج))
		BUG("delta size changed");
	حرر(buf);
	حرر(ادھار_buf);
	أرجع delta_buf;
}

ساكن بنانشانی لما do_compress(فراغ **pptr, بنانشانی لما ناپ)
{
	git_zstream دھارا;
	فراغ *وچ, *باہرلا;
	بنانشانی لما maxsize;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	git_deflate_init(&دھارا, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&دھارا, ناپ);

	وچ = *pptr;
	باہرلا = xmalloc(maxsize);
	*pptr = باہرلا;

	دھارا.اگلا_وچ = وچ;
	دھارا.avail_وچ = ناپ;
	دھارا.اگلا_باہرلا = باہرلا;
	دھارا.avail_باہرلا = maxsize;
	طالما (git_deflate(&دھارا, Z_انت) == Z_OK)
		; /* nothing */
	git_deflate_سرا(&دھارا);

	حرر(وچ);
	أرجع دھارا.کل_باہرلا;
}

ساكن بنانشانی لما write_large_blob_انکڑے(بنية odb_دھارا *st, بنية hashfile *f,
					   ثابت بنية object_id *oid)
{
	git_zstream دھارا;
	بنانشانی حرف ibuf[1024 * 16];
	بنانشانی حرف obuf[1024 * 16];
	بنانشانی لما olen = 0;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	git_deflate_init(&دھارا, cfg->pack_compression_level);

	لكل (;;) {
		ssize_t readlen;
		صحيح zret = Z_OK;
		readlen = odb_دھارا_read(st, ibuf, حجم(ibuf));
		إذا (readlen == -1)
			die(_("unable to read %s"), oid_منزل_hex(oid));

		دھارا.اگلا_وچ = ibuf;
		دھارا.avail_وچ = readlen;
		طالما ((دھارا.avail_وچ || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			دھارا.اگلا_باہرلا = obuf;
			دھارا.avail_باہرلا = حجم(obuf);
			zret = git_deflate(&دھارا, readlen ? 0 : Z_انت);
			hashwrite(f, obuf, دھارا.اگلا_باہرلا - obuf);
			olen += دھارا.اگلا_باہرلا - obuf;
		}
		إذا (دھارا.avail_وچ)
			die(_("deflate error (%d)"), zret);
		إذا (readlen == 0) {
			إذا (zret != Z_دھارا_سرا)
				die(_("deflate error (%d)"), zret);
			اكسر;
		}
	}
	git_deflate_سرا(&دھارا);
	أرجع olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ساكن صحيح check_pack_inflate(بنية packed_git *p,
		بنية pack_کھڑکی **w_curs,
		off_t وتھ,
		off_t len,
		ناپ_t expect)
{
	git_zstream دھارا;
	بنانشانی حرف fakebuf[4096], *وچ;
	صحيح st;

	memset(&دھارا, 0, حجم(دھارا));
	git_inflate_init(&دھارا);
	افعل {
		وچ = use_pack(p, w_curs, وتھ, &دھارا.avail_وچ);
		دھارا.اگلا_وچ = وچ;
		دھارا.اگلا_باہرلا = fakebuf;
		دھارا.avail_باہرلا = حجم(fakebuf);
		st = git_inflate(&دھارا, Z_انت);
		وتھ += دھارا.اگلا_وچ - وچ;
	} طالما (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_سرا(&دھارا);
	أرجع (st == Z_دھارا_سرا &&
		دھارا.کل_باہرلا == expect &&
		دھارا.کل_وچ == len) ? 0 : -1;
}

ساكن فراغ copy_pack_انکڑے(بنية hashfile *f,
		بنية packed_git *p,
		بنية pack_کھڑکی **w_curs,
		off_t وتھ,
		off_t len)
{
	بنانشانی حرف *وچ;
	ناپ_t avail;

	طالما (len) {
		وچ = use_pack(p, w_curs, وتھ, &avail);
		إذا (avail > len)
			avail = (بنانشانی لما)len;
		hashwrite(f, وچ, avail);
		وتھ += avail;
		len -= avail;
	}
}

ساكن مضمن صحيح oe_ناپ_greater_than(بنية packing_انکڑے *pack,
				       ثابت بنية object_اندراج *lhs,
				       ناپ_t rhs)
{
	إذا (lhs->ناپ_valid)
		أرجع lhs->ناپ_2 > rhs;
	إذا (rhs < pack->oe_ناپ_حد_بندی) /* rhs < 2^x <= lhs ? */
		أرجع 1;
	أرجع oe_get_ناپ_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ساكن بنانشانی لما write_no_reuse_object(بنية hashfile *f, بنية object_اندراج *اندراج,
					   بنانشانی لما حد_بندی, صحيح usable_delta)
{
	بنانشانی لما ناپ, datalen;
	بنانشانی حرف سرخی[MAX_PACK_OBJECT_سرخی],
		      dheader[MAX_PACK_OBJECT_سرخی];
	بنانشانی hdrlen;
	تعداد object_type type;
	فراغ *buf;
	بنية odb_دھارا *st = عدم;
	ثابت بنانشانی hashsz = the_hash_algo->rawsz;

	إذا (!usable_delta) {
		إذا (oe_type(اندراج) == OBJ_BLOB &&
		    oe_ناپ_greater_than(&منزل_pack, اندراج,
					 repo_settings_get_big_file_حد(the_repository)) &&
		    (st = odb_دھارا_from_object(the_repository->objects, &اندراج->idx.oid,
						 عدم)) != عدم) {
			buf = عدم;
			type = st->type;
			ناپ = st->ناپ;
		} وإلا {
			ناپ_t ناپ_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &اندراج->idx.oid, &type,
					      &ناپ_st);
			ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
			إذا (!buf)
				die(_("unable to read %s"),
				    oid_منزل_hex(&اندراج->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(اندراج->delta_انکڑے);
		اندراج->z_delta_ناپ = 0;
	} وإلا إذا (اندراج->delta_انکڑے) {
		ناپ = DELTA_ناپ(اندراج);
		buf = اندراج->delta_انکڑے;
		اندراج->delta_انکڑے = عدم;
		type = (allow_ofs_delta && DELTA(اندراج)->idx.وتھ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} وإلا {
		buf = get_delta(اندراج);
		ناپ = DELTA_ناپ(اندراج);
		type = (allow_ofs_delta && DELTA(اندراج)->idx.وتھ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	إذا (st)	/* large blob case, just assume we don't compress well */
		datalen = ناپ;
	وإلا إذا (اندراج->z_delta_ناپ)
		datalen = اندراج->z_delta_ناپ;
	وإلا
		datalen = do_compress(&buf, ناپ);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_وچ_pack_object_سرخی(سرخی, حجم(سرخی),
					      type, ناپ);

	إذا (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = اندراج->idx.وتھ - DELTA(اندراج)->idx.وتھ;
		بنانشانی pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد_بندی && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد_بندی) {
			إذا (st)
				odb_دھارا_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		إذا (حد_بندی && hdrlen + hashsz + datalen + hashsz >= حد_بندی) {
			إذا (st)
				odb_دھارا_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, DELTA(اندراج)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} وإلا {
		إذا (حد_بندی && hdrlen + datalen + hashsz >= حد_بندی) {
			إذا (st)
				odb_دھارا_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرخی, hdrlen);
	}
	إذا (st) {
		datalen = write_large_blob_انکڑے(st, f, &اندراج->idx.oid);
		odb_دھارا_close(st);
	} وإلا {
		hashwrite(f, buf, datalen);
		حرر(buf);
	}

	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_reuse_object(بنية hashfile *f, بنية object_اندراج *اندراج,
				بنانشانی لما حد_بندی, صحيح usable_delta)
{
	بنية packed_git *p = وچ_PACK(اندراج);
	بنية pack_کھڑکی *w_curs = عدم;
	uint32_t pos;
	off_t وتھ, cur;
	تعداد object_type type = oe_type(اندراج);
	تعداد object_type وچ_pack_type;
	off_t datalen;
	بنانشانی حرف سرخی[MAX_PACK_OBJECT_سرخی],
		      dheader[MAX_PACK_OBJECT_سرخی];
	بنانشانی hdrlen;
	ثابت بنانشانی hashsz = the_hash_algo->rawsz;
	ناپ_t اندراج_ناپ;

	cur = اندراج->وچ_pack_وتھ;
	وچ_pack_type = unpack_object_سرخی(p, &w_curs, &cur, &اندراج_ناپ);
	إذا (وچ_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_منزل_hex(&اندراج->idx.oid));

	إذا (DELTA(اندراج))
		type = (allow_ofs_delta && DELTA(اندراج)->idx.وتھ) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_وچ_pack_object_سرخی(سرخی, حجم(سرخی),
					      type, اندراج_ناپ);

	وتھ = اندراج->وچ_pack_وتھ;
	إذا (وتھ_منزل_pack_pos(p, وتھ, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_منزل_hex(&اندراج->idx.oid), (uintmax_t)وتھ,
		    p->pack_name);
	datalen = pack_pos_منزل_وتھ(p, pos + 1) - وتھ;
	إذا (!pack_منزل_stdout && p->اشاریہ_نسخہ > 1 &&
	    check_pack_چکری_جانچ(p, &w_curs, وتھ, datalen,
			   pack_pos_منزل_اشاریہ(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_منزل_hex(&اندراج->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, اندراج, حد_بندی, usable_delta);
	}

	وتھ += اندراج->وچ_pack_سرخی_ناپ;
	datalen -= اندراج->وچ_pack_سرخی_ناپ;

	إذا (!pack_منزل_stdout && p->اشاریہ_نسخہ == 1 &&
	    check_pack_inflate(p, &w_curs, وتھ, datalen, اندراج_ناپ)) {
		error(_("corrupt packed object for %s"),
		      oid_منزل_hex(&اندراج->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, اندراج, حد_بندی, usable_delta);
	}

	إذا (type == OBJ_OFS_DELTA) {
		off_t ofs = اندراج->idx.وتھ - DELTA(اندراج)->idx.وتھ;
		بنانشانی pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد_بندی && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد_بندی) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
		reused_delta++;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		إذا (حد_بندی && hdrlen + hashsz + datalen + hashsz >= حد_بندی) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرخی, hdrlen);
		hashwrite(f, DELTA(اندراج)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} وإلا {
		إذا (حد_بندی && hdrlen + datalen + hashsz >= حد_بندی) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرخی, hdrlen);
	}
	copy_pack_انکڑے(f, p, &w_curs, وتھ, datalen);
	unuse_pack(&w_curs);
	reused++;
	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_object(بنية hashfile *f,
			  بنية object_اندراج *اندراج,
			  off_t write_وتھ)
{
	بنانشانی لما حد_بندی;
	off_t len;
	صحيح usable_delta, منزل_reuse;

	إذا (!pack_منزل_stdout)
		چکری_جانچ_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	إذا (!pack_ناپ_حد_بندی || !nr_written)
		حد_بندی = 0;
	وإلا إذا (pack_ناپ_حد_بندی <= write_وتھ)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		حد_بندی = 1;
	وإلا
		حد_بندی = pack_ناپ_حد_بندی - write_وتھ;

	إذا (!DELTA(اندراج))
		usable_delta = 0;	/* no delta */
	وإلا إذا (!pack_ناپ_حد_بندی)
	       usable_delta = 1;	/* unlimited packfile */
	وإلا إذا (DELTA(اندراج)->idx.وتھ == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	وإلا إذا (DELTA(اندراج)->idx.وتھ)
		usable_delta = 1;	/* base already exists in this pack */
	وإلا
		usable_delta = 0;	/* base could end up in another pack */

	إذا (!reuse_object)
		منزل_reuse = 0;	/* explicit */
	وإلا إذا (!وچ_PACK(اندراج))
		منزل_reuse = 0;	/* can't reuse what we don't have */
	وإلا إذا (oe_type(اندراج) == OBJ_REF_DELTA ||
		 oe_type(اندراج) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		منزل_reuse = usable_delta;
				/* ... but pack split may override that */
	وإلا إذا (oe_type(اندراج) != اندراج->وچ_pack_type)
		منزل_reuse = 0;	/* pack has delta which is unusable */
	وإلا إذا (DELTA(اندراج))
		منزل_reuse = 0;	/* we want to pack afresh */
	وإلا
		منزل_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	إذا (!منزل_reuse)
		len = write_no_reuse_object(f, اندراج, حد_بندی, usable_delta);
	وإلا
		len = write_reuse_object(f, اندراج, حد_بندی, usable_delta);
	إذا (!len)
		أرجع 0;

	إذا (usable_delta)
		written_delta++;
	written++;
	إذا (!pack_منزل_stdout)
		اندراج->idx.چکری_جانچ_32 = چکری_جانچ_32_سرا(f);
	أرجع len;
}

تعداد write_one_حالت {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*عام*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ساكن تعداد write_one_حالت write_one(بنية hashfile *f,
				       بنية object_اندراج *e,
				       off_t *وتھ)
{
	off_t ناپ;
	صحيح recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.وتھ == 1);
	إذا (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_منزل_hex(&e->idx.oid));
		أرجع WRITE_ONE_RECURSIVE;
	} وإلا إذا (e->idx.وتھ || e->preferred_ادھار) {
		/* offset is non zero if object is written already. */
		أرجع WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	إذا (DELTA(e)) {
		e->idx.وتھ = 1; /* now recurse */
		بدل (write_one(f, DELTA(e), وتھ)) {
		حالة WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			سیٹ_DELTA(e, عدم);
			اكسر;
		افتراضي:
			اكسر;
		حالة WRITE_ONE_BREAK:
			e->idx.وتھ = recursing;
			أرجع WRITE_ONE_BREAK;
		}
	}

	e->idx.وتھ = *وتھ;
	ناپ = write_object(f, e, *وتھ);
	إذا (!ناپ) {
		e->idx.وتھ = recursing;
		أرجع WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جوڑو_overflows(*وتھ, ناپ))
		die(_("pack too large for current definition of off_t"));
	*وتھ += ناپ;
	أرجع WRITE_ONE_WRITTEN;
}

ساكن صحيح mark_tagged(ثابت بنية reference *ref, فراغ *cb_انکڑے بن_ورتیا)
{
	بنية object_id peeled;
	بنية object_اندراج *اندراج = packlist_لبھو(&منزل_pack, ref->oid);

	إذا (اندراج)
		اندراج->tagged = 1;
	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		اندراج = packlist_لبھو(&منزل_pack, &peeled);
		إذا (اندراج)
			اندراج->tagged = 1;
	}
	أرجع 0;
}

ساكن مضمن بنانشانی حرف oe_layer(بنية packing_انکڑے *pack,
				     بنية object_اندراج *e)
{
	إذا (!pack->layer)
		أرجع 0;
	أرجع pack->layer[e - pack->objects];
}

ساكن مضمن فراغ جوڑو_منزل_write_ترتیب(بنية object_اندراج **wo,
			       بنانشانی صحيح *endp,
			       بنية object_اندراج *e)
{
	إذا (e->filled || oe_layer(&منزل_pack, e) != write_layer)
		أرجع;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ساكن فراغ جوڑو_descendants_منزل_write_ترتیب(بنية object_اندراج **wo,
					   بنانشانی صحيح *endp,
					   بنية object_اندراج *e)
{
	صحيح جوڑو_منزل_ترتیب = 1;
	طالما (e) {
		إذا (جوڑو_منزل_ترتیب) {
			بنية object_اندراج *s;
			/* add this node... */
			جوڑو_منزل_write_ترتیب(wo, endp, e);
			/* all its siblings... */
			لكل (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				جوڑو_منزل_write_ترتیب(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		إذا (DELTA_بچہ(e)) {
			جوڑو_منزل_ترتیب = 1;
			e = DELTA_بچہ(e);
		} وإلا {
			جوڑو_منزل_ترتیب = 0;
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

ساكن فراغ جوڑو_family_منزل_write_ترتیب(بنية object_اندراج **wo,
				      بنانشانی صحيح *endp,
				      بنية object_اندراج *e)
{
	بنية object_اندراج *جڑ;

	لكل (جڑ = e; DELTA(جڑ); جڑ = DELTA(جڑ))
		; /* nothing */
	جوڑو_descendants_منزل_write_ترتیب(wo, endp, جڑ);
}

ساكن فراغ compute_layer_ترتیب(بنية object_اندراج **wo, بنانشانی صحيح *wo_سرا)
{
	بنانشانی صحيح i, آخری_untagged;
	بنية object_اندراج *objects = منزل_pack.objects;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			اكسر;
		جوڑو_منزل_write_ترتیب(wo, wo_سرا, &objects[i]);
	}
	آخری_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	لكل (; i < منزل_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			جوڑو_منزل_write_ترتیب(wo, wo_سرا, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	لكل (i = آخری_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_ٹیگ)
			استمر;
		جوڑو_منزل_write_ترتیب(wo, wo_سرا, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	لكل (i = آخری_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_درخت)
			استمر;
		جوڑو_منزل_write_ترتیب(wo, wo_سرا, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	لكل (i = آخری_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (!objects[i].filled && oe_layer(&منزل_pack, &objects[i]) == write_layer)
			جوڑو_family_منزل_write_ترتیب(wo, wo_سرا, &objects[i]);
	}
}

ساكن بنية object_اندراج **compute_write_ترتیب(فراغ)
{
	uint32_t max_layers = 1;
	بنانشانی صحيح i, wo_سرا;

	بنية object_اندراج **wo;
	بنية object_اندراج *objects = منزل_pack.objects;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		سیٹ_DELTA_بچہ(&objects[i], عدم);
		سیٹ_DELTA_SIBLING(&objects[i], عدم);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	لكل (i = منزل_pack.nr_objects; i > 0;) {
		بنية object_اندراج *e = &objects[--i];
		إذا (!DELTA(e))
			استمر;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_بچہ_idx;
		سیٹ_DELTA_بچہ(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_ٹیگ_ref(get_main_ref_store(the_repository), mark_tagged,
			      عدم);

	إذا (use_delta_islands) {
		max_layers = compute_pack_layers(&منزل_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, منزل_pack.nr_objects);
	wo_سرا = 0;

	لكل (; write_layer < max_layers; ++write_layer)
		compute_layer_ترتیب(wo, &wo_سرا);

	إذا (wo_سرا != منزل_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_سرا, منزل_pack.nr_objects);

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

ساكن فراغ ریکارڈ_reused_object(off_t where, off_t وتھ)
{
	إذا (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == وتھ)
		أرجع;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = وتھ;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
ساكن off_t لبھو_reused_وتھ(off_t where)
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
				  ناپ_t pos, بنية hashfile *باہرلا,
				  off_t pack_شروع,
				  بنية pack_کھڑکی **w_curs)
{
	off_t وتھ, اگلا, cur;
	تعداد object_type type;
	ناپ_t ناپ;

	وتھ = pack_pos_منزل_وتھ(reuse_packfile, pos);
	اگلا = pack_pos_منزل_وتھ(reuse_packfile, pos + 1);

	ریکارڈ_reused_object(وتھ,
			     وتھ - (hashfile_کل(باہرلا) - pack_شروع));

	cur = وتھ;
	type = unpack_object_سرخی(reuse_packfile, w_curs, &cur, &ناپ);
	أكد(type >= 0);

	إذا (type == OBJ_OFS_DELTA) {
		off_t ادھار_وتھ;
		off_t fixup;

		بنانشانی حرف سرخی[MAX_PACK_OBJECT_سرخی];
		بنانشانی len;

		ادھار_وتھ = get_delta_ادھار(reuse_packfile, w_curs, &cur, type, وتھ);
		أكد(ادھار_وتھ != 0);

		/* Convert to REF_DELTA if we must... */
		إذا (!allow_ofs_delta) {
			uint32_t ادھار_pos;
			بنية object_id ادھار_oid;

			إذا (وتھ_منزل_pack_pos(reuse_packfile, ادھار_وتھ, &ادھار_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)ادھار_وتھ,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&ادھار_oid, reuse_packfile,
					     pack_pos_منزل_اشاریہ(reuse_packfile, ادھار_pos));

			len = encode_وچ_pack_object_سرخی(سرخی, حجم(سرخی),
							   OBJ_REF_DELTA, ناپ);
			hashwrite(باہرلا, سرخی, len);
			hashwrite(باہرلا, ادھار_oid.hash, the_hash_algo->rawsz);
			copy_pack_انکڑے(باہرلا, reuse_packfile, w_curs, cur, اگلا - cur);
			أرجع;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = لبھو_reused_وتھ(وتھ) -
			لبھو_reused_وتھ(ادھار_وتھ);
		إذا (fixup) {
			بنانشانی حرف ofs_سرخی[MAX_PACK_OBJECT_سرخی];
			بنانشانی i, ofs_len;
			off_t ofs = وتھ - ادھار_وتھ - fixup;

			len = encode_وچ_pack_object_سرخی(سرخی, حجم(سرخی),
							   OBJ_OFS_DELTA, ناپ);

			i = حجم(ofs_سرخی) - 1;
			ofs_سرخی[i] = ofs & 127;
			طالما (ofs >>= 7)
				ofs_سرخی[--i] = 128 | (--ofs & 127);

			ofs_len = حجم(ofs_سرخی) - i;

			hashwrite(باہرلا, سرخی, len);
			hashwrite(باہرلا, ofs_سرخی + حجم(ofs_سرخی) - ofs_len, ofs_len);
			copy_pack_انکڑے(باہرلا, reuse_packfile, w_curs, cur, اگلا - cur);
			أرجع;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_انکڑے(باہرلا, reuse_packfile, w_curs, وتھ, اگلا - وتھ);
}

ساكن ناپ_t write_reused_pack_verbatim(بنية bitmapped_pack *reuse_packfile,
					 بنية hashfile *باہرلا,
					 بنية pack_کھڑکی **w_curs)
{
	ناپ_t pos = 0;
	ناپ_t سرا;

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
		أرجع reuse_packfile->bitmap_pos / بٹ_وچ_EWORD;
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
	سرا = reuse_packfile->bitmap_nr / بٹ_وچ_EWORD;
	إذا (reuse_packfile_bitmap->لفظ_alloc < سرا)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	طالما (pos < سرا && reuse_packfile_bitmap->الفاظ[pos] == (eword_t)~0)
		pos++;

	إذا (pos) {
		off_t منزل_write;

		written = (pos * بٹ_وچ_EWORD);
		منزل_write = pack_pos_منزل_وتھ(reuse_packfile->p, written)
			- حجم(بنية pack_سرخی);

		/* We're recording one chunk, not one object. */
		ریکارڈ_reused_object(حجم(بنية pack_سرخی), 0);
		hashflush(باہرلا);
		copy_pack_انکڑے(باہرلا, reuse_packfile->p, w_curs,
			حجم(بنية pack_سرخی), منزل_write);

		display_ترقی(ترقی_state, written);
	}
	أرجع pos;
}

ساكن فراغ write_reused_pack(بنية bitmapped_pack *reuse_packfile,
			      بنية hashfile *f)
{
	ناپ_t i = reuse_packfile->bitmap_pos / بٹ_وچ_EWORD;
	uint32_t وتھ;
	off_t pack_شروع = hashfile_کل(f) - حجم(بنية pack_سرخی);
	بنية pack_کھڑکی *w_curs = عدم;

	إذا (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	لكل (; i < reuse_packfile_bitmap->لفظ_alloc; ++i) {
		eword_t لفظ = reuse_packfile_bitmap->الفاظ[i];
		ناپ_t pos = (i * بٹ_وچ_EWORD);

		لكل (وتھ = 0; وتھ < بٹ_وچ_EWORD; ++وتھ) {
			uint32_t pack_pos;
			إذا ((لفظ >> وتھ) == 0)
				اكسر;

			وتھ += ewah_ثنائی_ہندسہ_ctz64(لفظ >> وتھ);
			إذا (pos + وتھ < reuse_packfile->bitmap_pos)
				استمر;
			إذا (pos + وتھ >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				اذهب پورا;

			إذا (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				بنية multi_pack_اشاریہ *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				إذا (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_منزل_midx(m, pos + وتھ);
				pack_ofs = nth_midxed_وتھ(m, midx_pos);

				إذا (وتھ_منزل_pack_pos(reuse_packfile->p,
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
				pack_pos = pos + وتھ;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_شروع, &w_curs);
			display_ترقی(ترقی_state, ++written);
		}
	}

پورا:
	unuse_pack(&w_curs);
}

ساكن فراغ write_excluded_by_configs(فراغ)
{
	بنية oidset_iter iter;
	ثابت بنية object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	طالما ((oid = oidset_iter_اگلا(&iter))) {
		بنية configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		إذا (!ex)
			BUG("configured exclusion wasn't configured");
		write_وچ_بھریا(1, ex->pack_hash_hex, لڑی_لمبائی(ex->pack_hash_hex));
		write_وچ_بھریا(1, " ", 1);
		write_وچ_بھریا(1, ex->uri, لڑی_لمبائی(ex->uri));
		write_وچ_بھریا(1, "\n", 1);
	}
}

ساكن ثابت حرف no_ونڈو_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ساكن فراغ write_pack_file(فراغ)
{
	uint32_t i = 0, j;
	بنية hashfile *f;
	off_t وتھ;
	uint32_t nr_بچیا = nr_نتیجہ;
	ویلا_t آخری_mtime = 0;
	بنية object_اندراج **write_ترتیب;
	off_t بائٹاں_written = 0;

	إذا (ترقی > pack_منزل_stdout)
		ترقی_state = شروع_ترقی(the_repository,
						_("Writing objects"), nr_نتیجہ);
	ALLOC_ARRAY(written_list, منزل_pack.nr_objects);
	write_ترتیب = compute_write_ترتیب();

	افعل {
		بنانشانی حرف hash[GIT_MAX_RAWSZ];
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
				.ترقی = ترقی_state,
				.بفر_len = LARGE_PACKET_انکڑے_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} وإلا {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		وتھ = write_pack_سرخی(f, nr_بچیا);

		إذا (reuse_packfiles_nr) {
			أكد(pack_منزل_stdout);
			لكل (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				إذا (reused_chunks_nr)
					reuse_packfiles_ورتیا_nr++;
			}
			وتھ = hashfile_کل(f);
		}

		nr_written = 0;
		لكل (; i < منزل_pack.nr_objects; i++) {
			بنية object_اندراج *e = write_ترتیب[i];
			إذا (write_one(f, e, &وتھ) == WRITE_ONE_BREAK)
				اكسر;
			display_ترقی(ترقی_state, written);
		}

		بائٹاں_written += hashfile_کل(f) +
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
					  CSUM_HASH_وچ_دھارا | CSUM_CLOSE);
		} وإلا إذا (nr_written == nr_بچیا) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_وچ_دھارا | CSUM_FSYNC | CSUM_CLOSE);
		} وإلا {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			صحيح fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_سرخی_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, وتھ);
			close(fd);
			إذا (write_bitmap_اشاریہ) {
				إذا (write_bitmap_اشاریہ != WRITE_BITMAP_QUIET)
					warning(_(no_ونڈو_warning));
				write_bitmap_اشاریہ = 0;
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
			} وإلا إذا (!آخری_mtime) {
				آخری_mtime = st.st_mtime;
			} وإلا {
				بنية utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --آخری_mtime;
				إذا (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", ادھار_name,
				    hash_منزل_hex(hash));

			إذا (write_bitmap_اشاریہ) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &منزل_pack,
						   عدم);
				bitmap_writer_سیٹ_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_اشاریہ(&bitmap_writer,
							       written_list);
			}

			إذا (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &منزل_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			إذا (write_bitmap_اشاریہ) {
				ناپ_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_ترقی(&ترقی_state);

				bitmap_writer_show_ترقی(&bitmap_writer,
							    ترقی);
				bitmap_writer_select_commits(&bitmap_writer,
							     اشاریہ_دار_commits,
							     اشاریہ_دار_commits_nr);
				إذا (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_انت(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_اشاریہ = 0;
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
			written_list[j]->وتھ = (off_t)-1;
		}
		nr_بچیا -= nr_written;
	} طالما (nr_بچیا && i < منزل_pack.nr_objects);

	حرر(written_list);
	حرر(write_ترتیب);
	stop_ترقی(&ترقی_state);
	إذا (written != nr_نتیجہ)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_نتیجہ);
	trace2_انکڑے_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_نتیجہ);
	trace2_انکڑے_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", بائٹاں_written);
}

ساكن صحيح no_try_delta(ثابت حرف *رستہ)
{
	ساكن بنية attr_check *check;

	إذا (!check)
		check = attr_check_initl("delta", عدم);
	git_check_attr(the_repository->اشاریہ, رستہ, check);
	إذا (ATTR_FALSE(check->اشیاء[0].قدر))
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
ساكن صحيح have_duplicate_اندراج(ثابت بنية object_id *oid,
				صحيح exclude)
{
	بنية object_اندراج *اندراج;

	إذا (reuse_packfile_bitmap &&
	    bitmap_walk_شامل_اے(bitmap_git, reuse_packfile_bitmap, oid))
		أرجع 1;

	اندراج = packlist_لبھو(&منزل_pack, oid);
	إذا (!اندراج)
		أرجع 0;

	إذا (exclude) {
		إذا (!اندراج->preferred_ادھار)
			nr_نتیجہ--;
		اندراج->preferred_ادھار = 1;
	}

	أرجع 1;
}

ساكن صحيح want_cruft_object_mtime(بنية repository *r,
				   ثابت بنية object_id *oid,
				   بنانشانی flags, uint32_t mtime)
{
	بنية odb_سروت_files *files = odb_سروت_files_downcast(r->objects->سروت);

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		بنية packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		لكل (; *cache; cache++) {
			بنية packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = لبھو_pack_اندراج_one(oid, p);
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
			إذا (!لدو_pack_mtimes(p)) {
				uint32_t pos;
				إذا (وتھ_منزل_pack_pos(p, ofs, &pos) < 0)
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

ساكن صحيح want_لبھیا_object(ثابت بنية object_id *oid, صحيح exclude,
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
	إذا (ignore_packed_keep_on_disk || ignore_packed_keep_وچ_core ||
	    ignore_packed_keep_وچ_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		بنانشانی flags = 0;
		إذا (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		إذا (ignore_packed_keep_وچ_core)
			flags |= KEPT_PACK_وچ_CORE;
		إذا (ignore_packed_keep_وچ_core_open)
			flags |= KEPT_PACK_وچ_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		إذا (!ignore_packed_keep_وچ_core_has_cruft) {
			إذا (ignore_packed_keep_on_disk && p->pack_keep)
				أرجع 0;
			إذا (ignore_packed_keep_وچ_core && p->pack_keep_وچ_core)
				أرجع 0;
			إذا (ignore_packed_keep_وچ_core_open && p->pack_keep_وچ_core_open)
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

ساكن صحيح want_object_وچ_pack_one(بنية packed_git *p,
				   ثابت بنية object_id *oid,
				   صحيح exclude,
				   بنية packed_git **لبھیا_pack,
				   off_t *لبھیا_وتھ,
				   uint32_t لبھیا_mtime)
{
	off_t وتھ;

	إذا (p == *لبھیا_pack)
		وتھ = *لبھیا_وتھ;
	وإلا
		وتھ = لبھو_pack_اندراج_one(oid, p);

	إذا (وتھ) {
		إذا (!*لبھیا_pack) {
			إذا (!is_pack_valid(p))
				أرجع -1;
			*لبھیا_وتھ = وتھ;
			*لبھیا_pack = p;
		}
		أرجع want_لبھیا_object(oid, exclude, p, لبھیا_mtime);
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
ساكن صحيح want_object_وچ_pack_mtime(ثابت بنية object_id *oid,
				     صحيح exclude,
				     بنية packed_git **لبھیا_pack,
				     off_t *لبھیا_وتھ,
				     uint32_t لبھیا_mtime)
{
	بنية odb_سروت_files *files =
		odb_سروت_files_downcast(the_repository->objects->سروت);
	صحيح want;
	بنية packfile_list_اندراج *e;

	إذا (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		لكل (بنية odb_files_dir *dir = files->dirs->اگلا; dir; dir = dir->اگلا)
			إذا (!odb_سروت_read_object_info(&dir->loose->ادھار, oid, عدم, 0, عدم))
				أرجع 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	إذا (*لبھیا_pack) {
		want = want_لبھیا_object(oid, exclude, *لبھیا_pack,
					 لبھیا_mtime);
		إذا (want != -1)
			أرجع want;

		*لبھیا_pack = عدم;
		*لبھیا_وتھ = 0;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		بنية multi_pack_اشاریہ *m = get_multi_pack_اشاریہ(dir->packed);
		بنية pack_اندراج e;

		إذا (m && midx_fill_اندراج(m, oid, &e, عدم) == MIDX_FILL_کامیابی) {
			want = want_object_وچ_pack_one(e.p, oid, exclude, لبھیا_pack, لبھیا_وتھ, لبھیا_mtime);
			إذا (want != -1)
				أرجع want;
		}
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		لكل (e = dir->packed->packs.سر; e; e = e->اگلا) {
			بنية packed_git *p = e->pack;
			want = want_object_وچ_pack_one(p, oid, exclude, لبھیا_pack, لبھیا_وتھ, لبھیا_mtime);
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
				إذا (skip_اگیتر(ex->uri,
						uri_protocols.اشیاء[i].string,
						&p) &&
				    *p == ':') {
					oidset_داخل_کرو(&excluded_by_config, oid);
					أرجع 0;
				}
			}
		}
	}

	أرجع 1;
}

ساكن مضمن صحيح want_object_وچ_pack(ثابت بنية object_id *oid,
				      صحيح exclude,
				      بنية packed_git **لبھیا_pack,
				      off_t *لبھیا_وتھ)
{
	أرجع want_object_وچ_pack_mtime(oid, exclude, لبھیا_pack, لبھیا_وتھ,
					 0);
}

ساكن بنية object_اندراج *create_object_اندراج(ثابت بنية object_id *oid,
						تعداد object_type type,
						uint32_t hash,
						صحيح exclude,
						صحيح no_try_delta,
						بنية packed_git *لبھیا_pack,
						off_t لبھیا_وتھ)
{
	بنية object_اندراج *اندراج;

	اندراج = packlist_alloc(&منزل_pack, oid);
	اندراج->hash = hash;
	oe_سیٹ_type(اندراج, type);
	إذا (exclude)
		اندراج->preferred_ادھار = 1;
	وإلا
		nr_نتیجہ++;
	إذا (لبھیا_pack) {
		oe_سیٹ_وچ_pack(&منزل_pack, اندراج, لبھیا_pack);
		اندراج->وچ_pack_وتھ = لبھیا_وتھ;
	}

	اندراج->no_try_delta = no_try_delta;

	أرجع اندراج;
}

ساكن ثابت حرف no_گھیرا_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ساكن فراغ جوڑو_object_اندراج(ثابت بنية object_id *oid, تعداد object_type type,
			     ثابت حرف *name, صحيح exclude)
{
	بنية packed_git *لبھیا_pack = عدم;
	off_t لبھیا_وتھ = 0;

	display_ترقی(ترقی_state, ++nr_تکیا);

	إذا (have_duplicate_اندراج(oid, exclude))
		أرجع;

	إذا (!want_object_وچ_pack(oid, exclude, &لبھیا_pack, &لبھیا_وتھ)) {
		/* The pack is missing an object, so it will not have closure */
		إذا (write_bitmap_اشاریہ) {
			إذا (write_bitmap_اشاریہ != WRITE_BITMAP_QUIET)
				warning(_(no_گھیرا_warning));
			write_bitmap_اشاریہ = 0;
		}
		أرجع;
	}

	create_object_اندراج(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    لبھیا_pack, لبھیا_وتھ);
}

ساكن صحيح جوڑو_object_اندراج_from_bitmap(ثابت بنية object_id *oid,
					تعداد object_type type,
					صحيح flags بن_ورتیا, uint32_t name_hash,
					بنية packed_git *pack, off_t وتھ,
					فراغ *payload بن_ورتیا)
{
	display_ترقی(ترقی_state, ++nr_تکیا);

	إذا (have_duplicate_اندراج(oid, 0))
		أرجع 0;

	إذا (!want_object_وچ_pack(oid, 0, &pack, &وتھ))
		أرجع 0;

	create_object_اندراج(oid, type, name_hash, 0, 0, pack, وتھ);
	أرجع 0;
}

بنية pbase_درخت_cache {
	بنية object_id oid;
	صحيح ref;
	صحيح temporary;
	فراغ *درخت_انکڑے;
	بنانشانی لما درخت_ناپ;
};

ساكن بنية pbase_درخت_cache *(pbase_درخت_cache[256]);
ساكن صحيح pbase_درخت_cache_ix(ثابت بنية object_id *oid)
{
	أرجع oid->hash[0] % ARRAY_ناپ(pbase_درخت_cache);
}
ساكن صحيح pbase_درخت_cache_ix_incr(صحيح ix)
{
	أرجع (ix+1) % ARRAY_ناپ(pbase_درخت_cache);
}

ساكن بنية pbase_درخت {
	بنية pbase_درخت *اگلا;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	بنية pbase_درخت_cache pcache;
} *pbase_درخت;

ساكن بنية pbase_درخت_cache *pbase_درخت_get(ثابت بنية object_id *oid)
{
	بنية pbase_درخت_cache *ent, *nent;
	فراغ *انکڑے;
	بنانشانی لما ناپ;
	ناپ_t ناپ_st = 0;
	تعداد object_type type;
	صحيح neigh;
	صحيح my_ix = pbase_درخت_cache_ix(oid);
	صحيح دستیاب_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	لكل (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_درخت_cache[my_ix];
		إذا (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			أرجع ent;
		}
		وإلا إذا (((دستیاب_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= دستیاب_ix) &&
			  (!ent && pbase_درخت_cache[دستیاب_ix])))
			دستیاب_ix = my_ix;
		إذا (!ent)
			اكسر;
		my_ix = pbase_درخت_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	انکڑے = odb_read_object(the_repository->objects, oid, &type, &ناپ_st);
	ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
	إذا (!انکڑے)
		أرجع عدم;
	إذا (type != OBJ_درخت) {
		حرر(انکڑے);
		أرجع عدم;
	}

	/* We need to either cache or return a throwaway copy */

	إذا (دستیاب_ix < 0)
		ent = عدم;
	وإلا {
		ent = pbase_درخت_cache[دستیاب_ix];
		my_ix = دستیاب_ix;
	}

	إذا (!ent) {
		nent = xmalloc(حجم(*nent));
		nent->temporary = (دستیاب_ix < 0);
	}
	وإلا {
		/* evict and reuse */
		حرر(ent->درخت_انکڑے);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->درخت_انکڑے = انکڑے;
	nent->درخت_ناپ = ناپ;
	nent->ref = 1;
	إذا (!nent->temporary)
		pbase_درخت_cache[my_ix] = nent;
	أرجع nent;
}

ساكن فراغ pbase_درخت_put(بنية pbase_درخت_cache *cache)
{
	إذا (!cache->temporary) {
		cache->ref--;
		أرجع;
	}
	حرر(cache->درخت_انکڑے);
	حرر(cache);
}

ساكن ناپ_t name_cmp_len(ثابت حرف *name)
{
	أرجع strcspn(name, "\n/");
}

ساكن فراغ جوڑو_pbase_object(بنية درخت_desc *درخت,
			     ثابت حرف *name,
			     ناپ_t cmplen,
			     ثابت حرف *fullname)
{
	بنية name_اندراج اندراج;
	صحيح cmp;

	طالما (درخت_اندراج(درخت,&اندراج)) {
		إذا (S_ISGITLINK(اندراج.منوال))
			استمر;
		cmp = درخت_اندراج_len(&اندراج) != cmplen ? 1 :
		      memcmp(name, اندراج.رستہ, cmplen);
		إذا (cmp > 0)
			استمر;
		إذا (cmp < 0)
			أرجع;
		إذا (name[cmplen] != '/') {
			جوڑو_object_اندراج(&اندراج.oid,
					 object_type(اندراج.منوال),
					 fullname, 1);
			أرجع;
		}
		إذا (S_ISDIR(اندراج.منوال)) {
			بنية درخت_desc sub;
			بنية pbase_درخت_cache *درخت;
			ثابت حرف *تھلے = name+cmplen+1;
			ناپ_t downlen = name_cmp_len(تھلے);

			درخت = pbase_درخت_get(&اندراج.oid);
			إذا (!درخت)
				أرجع;
			init_درخت_desc(&sub, &درخت->oid,
				       درخت->درخت_انکڑے, درخت->درخت_ناپ);

			جوڑو_pbase_object(&sub, تھلے, downlen, fullname);
			pbase_درخت_put(درخت);
		}
	}
}

ساكن بنانشانی *پورا_pbase_paths;
ساكن صحيح پورا_pbase_paths_عدد;
ساكن صحيح پورا_pbase_paths_alloc;
ساكن صحيح پورا_pbase_رستہ_pos(بنانشانی hash)
{
	صحيح lo = 0;
	صحيح hi = پورا_pbase_paths_عدد;
	طالما (lo < hi) {
		صحيح mi = lo + (hi - lo) / 2;
		إذا (پورا_pbase_paths[mi] == hash)
			أرجع mi;
		إذا (پورا_pbase_paths[mi] < hash)
			hi = mi;
		وإلا
			lo = mi + 1;
	}
	أرجع -lo-1;
}

ساكن صحيح check_pbase_رستہ(بنانشانی hash)
{
	صحيح pos = پورا_pbase_رستہ_pos(hash);
	إذا (0 <= pos)
		أرجع 1;
	pos = -pos - 1;
	ALLOC_GROW(پورا_pbase_paths,
		   پورا_pbase_paths_عدد + 1,
		   پورا_pbase_paths_alloc);
	پورا_pbase_paths_عدد++;
	إذا (pos < پورا_pbase_paths_عدد)
		MOVE_ARRAY(پورا_pbase_paths + pos + 1, پورا_pbase_paths + pos,
			   پورا_pbase_paths_عدد - pos - 1);
	پورا_pbase_paths[pos] = hash;
	أرجع 0;
}

ساكن فراغ جوڑو_preferred_ادھار_object(ثابت حرف *name)
{
	بنية pbase_درخت *it;
	ناپ_t cmplen;
	بنانشانی hash = pack_name_hash_fn(name);

	إذا (!عدد_preferred_ادھار || check_pbase_رستہ(hash))
		أرجع;

	cmplen = name_cmp_len(name);
	لكل (it = pbase_درخت; it; it = it->اگلا) {
		إذا (cmplen == 0) {
			جوڑو_object_اندراج(&it->pcache.oid, OBJ_درخت, عدم, 1);
		}
		وإلا {
			بنية درخت_desc درخت;
			init_درخت_desc(&درخت, &it->pcache.oid,
				       it->pcache.درخت_انکڑے, it->pcache.درخت_ناپ);
			جوڑو_pbase_object(&درخت, name, cmplen, name);
		}
	}
}

ساكن فراغ جوڑو_preferred_ادھار(بنية object_id *oid)
{
	بنية pbase_درخت *it;
	فراغ *انکڑے;
	بنانشانی لما ناپ;
	ناپ_t ناپ_st = 0;
	بنية object_id درخت_oid;

	إذا (کھڑکی <= عدد_preferred_ادھار++)
		أرجع;

	انکڑے = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_درخت, &ناپ_st, &درخت_oid);
	ناپ = cast_ناپ_t_منزل_ulong(ناپ_st);
	إذا (!انکڑے)
		أرجع;

	لكل (it = pbase_درخت; it; it = it->اگلا) {
		إذا (oideq(&it->pcache.oid, &درخت_oid)) {
			حرر(انکڑے);
			أرجع;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->اگلا = pbase_درخت;
	pbase_درخت = it;

	oidcpy(&it->pcache.oid, &درخت_oid);
	it->pcache.درخت_انکڑے = انکڑے;
	it->pcache.درخت_ناپ = ناپ;
}

ساكن فراغ cleanup_preferred_ادھار(فراغ)
{
	بنية pbase_درخت *it;
	بنانشانی i;

	it = pbase_درخت;
	pbase_درخت = عدم;
	طالما (it) {
		بنية pbase_درخت *tmp = it;
		it = tmp->اگلا;
		حرر(tmp->pcache.درخت_انکڑے);
		حرر(tmp);
	}

	لكل (i = 0; i < ARRAY_ناپ(pbase_درخت_cache); i++) {
		إذا (!pbase_درخت_cache[i])
			استمر;
		حرر(pbase_درخت_cache[i]->درخت_انکڑے);
		FREE_AND_NULL(pbase_درخت_cache[i]);
	}

	FREE_AND_NULL(پورا_pbase_paths);
	پورا_pbase_paths_عدد = پورا_pbase_paths_alloc = 0;
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
ساكن صحيح can_reuse_delta(ثابت بنية object_id *ادھار_oid,
			   بنية object_اندراج *delta,
			   بنية object_اندراج **ادھار_باہرلا)
{
	بنية object_اندراج *ادھار;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	ادھار = packlist_لبھو(&منزل_pack, ادھار_oid);
	إذا (ادھار) {
		إذا (!وچ_same_island(&delta->idx.oid, &ادھار->idx.oid))
			أرجع 0;
		*ادھار_باہرلا = ادھار;
		أرجع 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	إذا (thin && bitmap_has_oid_وچ_uninteresting(bitmap_git, ادھار_oid)) {
		إذا (use_delta_islands) {
			إذا (!وچ_same_island(&delta->idx.oid, ادھار_oid))
				أرجع 0;
		}
		*ادھار_باہرلا = عدم;
		أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ prefetch_منزل_pack(uint32_t object_اشاریہ_شروع) {
	بنية oid_array منزل_fetch = OID_ARRAY_INIT;
	uint32_t i;

	لكل (i = object_اشاریہ_شروع; i < منزل_pack.nr_objects; i++) {
		بنية object_اندراج *اندراج = منزل_pack.objects + i;

		إذا (!odb_read_object_info_extended(the_repository->objects,
						   &اندراج->idx.oid,
						   عدم,
						   OBJECT_INFO_FOR_PREFETCH))
			استمر;
		oid_array_append(&منزل_fetch, &اندراج->idx.oid);
	}
	promisor_remote_get_سدھا(the_repository,
				   منزل_fetch.oid, منزل_fetch.nr);
	oid_array_clear(&منزل_fetch);
}

ساكن فراغ check_object(بنية object_اندراج *اندراج, uint32_t object_اشاریہ)
{
	ناپ_t canonical_ناپ;
	تعداد object_type type;
	بنية object_info oi = {.typep = &type, .sizep = &canonical_ناپ};

	إذا (وچ_PACK(اندراج)) {
		بنية packed_git *p = وچ_PACK(اندراج);
		بنية pack_کھڑکی *w_curs = عدم;
		صحيح have_ادھار = 0;
		بنية object_id ادھار_ref;
		بنية object_اندراج *ادھار_اندراج;
		ناپ_t ورتیا_2, ورتیا_0, avail;
		off_t ofs;
		بنانشانی حرف *buf, c;
		تعداد object_type type;
		ناپ_t وچ_pack_ناپ;

		buf = use_pack(p, &w_curs, اندراج->وچ_pack_وتھ, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		ورتیا_2 = unpack_object_سرخی_بفر(buf, avail,
						   &type,
						   &وچ_pack_ناپ);
		إذا (ورتیا_2 == 0)
			اذهب give_اتے;

		إذا (type < 0)
			BUG("invalid type %d", type);
		اندراج->وچ_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		بدل (اندراج->وچ_pack_type) {
		افتراضي:
			/* Not a delta hence we've already got all we need. */
			oe_سیٹ_type(اندراج, اندراج->وچ_pack_type);
			سیٹ_ناپ(اندراج, وچ_pack_ناپ);
			اندراج->وچ_pack_سرخی_ناپ = ورتیا_2;
			إذا (oe_type(اندراج) < OBJ_COMMIT || oe_type(اندراج) > OBJ_BLOB)
				اذهب give_اتے;
			unuse_pack(&w_curs);
			أرجع;
		حالة OBJ_REF_DELTA:
			إذا (reuse_delta && !اندراج->preferred_ادھار) {
				oidread(&ادھار_ref,
					use_pack(p, &w_curs,
						 اندراج->وچ_pack_وتھ + ورتیا_2,
						 عدم),
					the_repository->hash_algo);
				have_ادھار = 1;
			}
			اندراج->وچ_pack_سرخی_ناپ = ورتیا_2 + the_hash_algo->rawsz;
			اكسر;
		حالة OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       اندراج->وچ_pack_وتھ + ورتیا_2, عدم);
			ورتیا_0 = 0;
			c = buf[ورتیا_0++];
			ofs = c & 127;
			طالما (c & 128) {
				ofs += 1;
				إذا (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_منزل_hex(&اندراج->idx.oid));
					اذهب give_اتے;
				}
				c = buf[ورتیا_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = اندراج->وچ_pack_وتھ - ofs;
			إذا (ofs <= 0 || ofs >= اندراج->وچ_pack_وتھ) {
				error(_("delta base offset out of bound for %s"),
				      oid_منزل_hex(&اندراج->idx.oid));
				اذهب give_اتے;
			}
			إذا (reuse_delta && !اندراج->preferred_ادھار) {
				uint32_t pos;
				إذا (وتھ_منزل_pack_pos(p, ofs, &pos) < 0)
					اذهب give_اتے;
				إذا (!nth_packed_object_id(&ادھار_ref, p,
							  pack_pos_منزل_اشاریہ(p, pos)))
					have_ادھار = 1;
			}
			اندراج->وچ_pack_سرخی_ناپ = ورتیا_2 + ورتیا_0;
			اكسر;
		}

		إذا (have_ادھار &&
		    can_reuse_delta(&ادھار_ref, اندراج, &ادھار_اندراج)) {
			oe_سیٹ_type(اندراج, اندراج->وچ_pack_type);
			سیٹ_ناپ(اندراج, وچ_pack_ناپ); /* delta size */
			سیٹ_DELTA_ناپ(اندراج, وچ_pack_ناپ);

			إذا (ادھار_اندراج) {
				سیٹ_DELTA(اندراج, ادھار_اندراج);
				اندراج->delta_sibling_idx = ادھار_اندراج->delta_بچہ_idx;
				سیٹ_DELTA_بچہ(ادھار_اندراج, اندراج);
			} وإلا {
				سیٹ_DELTA_EXT(اندراج, &ادھار_ref);
			}

			unuse_pack(&w_curs);
			أرجع;
		}

		إذا (oe_type(اندراج)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = اندراج->وچ_pack_وتھ + اندراج->وچ_pack_سرخی_ناپ;
			canonical_ناپ = get_ناپ_from_delta(p, &w_curs,
							     delta_pos);
			إذا (canonical_ناپ == 0)
				اذهب give_اتے;
			سیٹ_ناپ(اندراج, canonical_ناپ);
			unuse_pack(&w_curs);
			أرجع;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_اتے:
		unuse_pack(&w_curs);
	}

	إذا (odb_read_object_info_extended(the_repository->objects, &اندراج->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		إذا (repo_has_promisor_remote(the_repository)) {
			prefetch_منزل_pack(object_اشاریہ);
			إذا (odb_read_object_info_extended(the_repository->objects, &اندراج->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} وإلا {
			type = -1;
		}
	}
	oe_سیٹ_type(اندراج, type);
	إذا (اندراج->type_valid) {
		سیٹ_ناپ(اندراج, canonical_ناپ);
	} وإلا {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ساكن صحيح pack_وتھ_ترتیب_دیو(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_اندراج *a = *(بنية object_اندراج **)_a;
	ثابت بنية object_اندراج *b = *(بنية object_اندراج **)_b;
	ثابت بنية packed_git *a_وچ_pack = وچ_PACK(a);
	ثابت بنية packed_git *b_وچ_pack = وچ_PACK(b);

	/* avoid filesystem trashing with loose objects */
	إذا (!a_وچ_pack && !b_وچ_pack)
		أرجع oidcmp(&a->idx.oid, &b->idx.oid);

	إذا (a_وچ_pack < b_وچ_pack)
		أرجع -1;
	إذا (a_وچ_pack > b_وچ_pack)
		أرجع 1;
	أرجع a->وچ_pack_وتھ < b->وچ_pack_وتھ ? -1 :
			(a->وچ_pack_وتھ > b->وچ_pack_وتھ);
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
ساكن فراغ drop_reused_delta(بنية object_اندراج *اندراج)
{
	بنانشانی *idx = &منزل_pack.objects[اندراج->delta_idx - 1].delta_بچہ_idx;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type;
	ناپ_t ناپ;

	طالما (*idx) {
		بنية object_اندراج *oe = &منزل_pack.objects[*idx - 1];

		إذا (oe == اندراج)
			*idx = oe->delta_sibling_idx;
		وإلا
			idx = &oe->delta_sibling_idx;
	}
	سیٹ_DELTA(اندراج, عدم);
	اندراج->ڈونگھائی = 0;

	oi.sizep = &ناپ;
	oi.typep = &type;
	إذا (packed_object_info(عدم, وچ_PACK(اندراج), اندراج->وچ_pack_وتھ, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_سیٹ_type(اندراج,
			    odb_read_object_info(the_repository->objects,
						 &اندراج->idx.oid, &ناپ));
	} وإلا {
		oe_سیٹ_type(اندراج, type);
	}
	سیٹ_ناپ(اندراج, ناپ);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
ساكن فراغ break_delta_chains(بنية object_اندراج *اندراج)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t کل_ڈونگھائی;
	بنية object_اندراج *cur, *اگلا;

	لكل (cur = اندراج, کل_ڈونگھائی = 0;
	     cur;
	     cur = DELTA(cur), کل_ڈونگھائی++) {
		إذا (cur->dfs_state == DFS_پورا) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			کل_ڈونگھائی += cur->ڈونگھائی;
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
			cur->dfs_state = DFS_پورا;
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
			cur->dfs_state = DFS_پورا;
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
	لكل (cur = اندراج; cur; cur = اگلا) {
		اگلا = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		إذا (cur->dfs_state == DFS_پورا)
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
		cur->ڈونگھائی = (کل_ڈونگھائی--) % (ڈونگھائی + 1);
		إذا (!cur->ڈونگھائی)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_پورا;
	}
}

ساكن فراغ get_object_details(فراغ)
{
	uint32_t i;
	بنية object_اندراج **sorted_by_وتھ;

	إذا (ترقی)
		ترقی_state = شروع_ترقی(the_repository,
						_("Counting objects"),
						منزل_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_وتھ, منزل_pack.nr_objects);
	لكل (i = 0; i < منزل_pack.nr_objects; i++)
		sorted_by_وتھ[i] = منزل_pack.objects + i;
	QSORT(sorted_by_وتھ, منزل_pack.nr_objects, pack_وتھ_ترتیب_دیو);

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		بنية object_اندراج *اندراج = sorted_by_وتھ[i];
		check_object(اندراج, i);
		إذا (اندراج->type_valid &&
		    oe_ناپ_greater_than(&منزل_pack, اندراج,
					 repo_settings_get_big_file_حد(the_repository)))
			اندراج->no_try_delta = 1;
		display_ترقی(ترقی_state, i + 1);
	}
	stop_ترقی(&ترقی_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	لكل (i = 0; i < منزل_pack.nr_objects; i++)
		break_delta_chains(&منزل_pack.objects[i]);

	حرر(sorted_by_وتھ);
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
ساكن صحيح type_ناپ_ترتیب_دیو(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_اندراج *a = *(بنية object_اندراج **)_a;
	ثابت بنية object_اندراج *b = *(بنية object_اندراج **)_b;
	ثابت تعداد object_type a_type = oe_type(a);
	ثابت تعداد object_type b_type = oe_type(b);
	ثابت بنانشانی لما a_ناپ = ناپ_3(a);
	ثابت بنانشانی لما b_ناپ = ناپ_3(b);

	إذا (a_type > b_type)
		أرجع -1;
	إذا (a_type < b_type)
		أرجع 1;
	إذا (a->hash > b->hash)
		أرجع -1;
	إذا (a->hash < b->hash)
		أرجع 1;
	إذا (a->preferred_ادھار > b->preferred_ادھار)
		أرجع -1;
	إذا (a->preferred_ادھار < b->preferred_ادھار)
		أرجع 1;
	إذا (use_delta_islands) {
		ثابت صحيح island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		إذا (island_cmp)
			أرجع island_cmp;
	}
	إذا (a_ناپ > b_ناپ)
		أرجع -1;
	إذا (a_ناپ < b_ناپ)
		أرجع 1;
	أرجع a < b ? -1 : (a > b);  /* newest first */
}

بنية unpacked {
	بنية object_اندراج *اندراج;
	فراغ *انکڑے;
	بنية delta_اشاریہ *اشاریہ;
	بنانشانی ڈونگھائی;
};

ساكن صحيح delta_cacheable(ناپ_t src_ناپ, ناپ_t trg_ناپ,
			   ناپ_t delta_ناپ_2)
{
	إذا (max_delta_cache_ناپ && delta_cache_ناپ + delta_ناپ_2 > max_delta_cache_ناپ)
		أرجع 0;

	إذا (delta_ناپ_2 < cache_max_small_delta_ناپ)
		أرجع 1;

	/* cache delta, if objects are large enough compared to delta size */
	إذا ((src_ناپ >> 20) + (trg_ناپ >> 21) > (delta_ناپ_2 >> 10))
		أرجع 1;

	أرجع 0;
}

/* Protect delta_cache_size */
ساكن pthread_mutex_t cache_mutex;
#تعریف_کرو cache_جندرا()		pthread_mutex_جندرا(&cache_mutex)
#تعریف_کرو cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ساكن pthread_mutex_t ترقی_mutex;
#تعریف_کرو ترقی_جندرا()		pthread_mutex_جندرا(&ترقی_mutex)
#تعریف_کرو ترقی_unlock()	pthread_mutex_unlock(&ترقی_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ساكن مضمن صحيح oe_ناپ_less_than(بنية packing_انکڑے *pack,
				    ثابت بنية object_اندراج *lhs,
				    ناپ_t rhs)
{
	إذا (lhs->ناپ_valid)
		أرجع lhs->ناپ_2 < rhs;
	إذا (rhs < pack->oe_ناپ_حد_بندی) /* rhs < 2^x <= lhs ? */
		أرجع 0;
	أرجع oe_get_ناپ_slow(pack, lhs) < rhs;
}

ساكن مضمن فراغ oe_سیٹ_درخت_ڈونگھائی(بنية packing_انکڑے *pack,
				     بنية object_اندراج *e,
				     بنانشانی صحيح درخت_ڈونگھائی)
{
	إذا (!pack->درخت_ڈونگھائی)
		CALLOC_ARRAY(pack->درخت_ڈونگھائی, pack->nr_alloc);
	pack->درخت_ڈونگھائی[e - pack->objects] = درخت_ڈونگھائی;
}

ساكن فراغ ریکارڈ_درخت_ڈونگھائی(ثابت بنية object_id *oid, ثابت حرف *name)
{
	ثابت حرف *p;
	بنانشانی ڈونگھائی;
	بنية object_اندراج *ent;

	/* the empty string is a root tree, which is depth 0 */
	ڈونگھائی = *name ? 1 : 0;
	لكل (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		ڈونگھائی++;

	ent = packlist_لبھو(&منزل_pack, oid);
	إذا (ent && ڈونگھائی > oe_درخت_ڈونگھائی(&منزل_pack, ent))
		oe_سیٹ_درخت_ڈونگھائی(&منزل_pack, ent, ڈونگھائی);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
ناپ_t oe_get_ناپ_slow(بنية packing_انکڑے *pack,
			ثابت بنية object_اندراج *e)
{
	بنية packed_git *p;
	بنية pack_کھڑکی *w_curs;
	بنانشانی حرف *buf;
	تعداد object_type type;
	ناپ_t ورتیا_2, avail, ناپ;

	إذا (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		ناپ_t sz;
		packing_انکڑے_جندرا(&منزل_pack);
		إذا (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_منزل_hex(&e->idx.oid));
		packing_انکڑے_unlock(&منزل_pack);
		أرجع sz;
	}

	p = oe_وچ_pack(pack, e);
	إذا (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_انکڑے_جندرا(&منزل_pack);
	w_curs = عدم;
	buf = use_pack(p, &w_curs, e->وچ_pack_وتھ, &avail);
	ورتیا_2 = unpack_object_سرخی_بفر(buf, avail, &type, &ناپ);
	إذا (ورتیا_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_منزل_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_انکڑے_unlock(&منزل_pack);
	أرجع ناپ;
}

ساكن صحيح try_delta(بنية unpacked *trg, بنية unpacked *src,
		     بنانشانی max_ڈونگھائی, ناپ_t *mem_usage)
{
	بنية object_اندراج *trg_اندراج = trg->اندراج;
	بنية object_اندراج *src_اندراج = src->اندراج;
	بنانشانی لما trg_ناپ, src_ناپ, sizediff, max_ناپ, sz;
	ناپ_t delta_ناپ_2;
	بنانشانی ref_ڈونگھائی;
	تعداد object_type type;
	فراغ *delta_buf;

	/* Don't bother doing diffs between different types */
	إذا (oe_type(trg_اندراج) != oe_type(src_اندراج))
		أرجع -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	إذا (reuse_delta && وچ_PACK(trg_اندراج) &&
	    وچ_PACK(trg_اندراج) == وچ_PACK(src_اندراج) &&
	    !src_اندراج->preferred_ادھار &&
	    trg_اندراج->وچ_pack_type != OBJ_REF_DELTA &&
	    trg_اندراج->وچ_pack_type != OBJ_OFS_DELTA)
		أرجع 0;

	/* Let's not bust the allowed depth. */
	إذا (src->ڈونگھائی >= max_ڈونگھائی)
		أرجع 0;

	/* Now some size filtering heuristics. */
	trg_ناپ = ناپ_3(trg_اندراج);
	إذا (!DELTA(trg_اندراج)) {
		max_ناپ = trg_ناپ/2 - the_hash_algo->rawsz;
		ref_ڈونگھائی = 1;
	} وإلا {
		max_ناپ = DELTA_ناپ(trg_اندراج);
		ref_ڈونگھائی = trg->ڈونگھائی;
	}
	max_ناپ = (uint64_t)max_ناپ * (max_ڈونگھائی - src->ڈونگھائی) /
						(max_ڈونگھائی - ref_ڈونگھائی + 1);
	إذا (max_ناپ == 0)
		أرجع 0;
	src_ناپ = ناپ_3(src_اندراج);
	sizediff = src_ناپ < trg_ناپ ? trg_ناپ - src_ناپ : 0;
	إذا (sizediff >= max_ناپ)
		أرجع 0;
	إذا (trg_ناپ < src_ناپ / 32)
		أرجع 0;

	إذا (!وچ_same_island(&trg->اندراج->idx.oid, &src->اندراج->idx.oid))
		أرجع 0;

	/* Load data if not already done */
	إذا (!trg->انکڑے) {
		ناپ_t sz_st = 0;
		packing_انکڑے_جندرا(&منزل_pack);
		trg->انکڑے = odb_read_object(the_repository->objects,
					    &trg_اندراج->idx.oid, &type,
					    &sz_st);
		sz = cast_ناپ_t_منزل_ulong(sz_st);
		packing_انکڑے_unlock(&منزل_pack);
		إذا (!trg->انکڑے)
			die(_("object %s cannot be read"),
			    oid_منزل_hex(&trg_اندراج->idx.oid));
		إذا (sz != trg_ناپ)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_منزل_hex(&trg_اندراج->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_ناپ);
		*mem_usage += sz;
	}
	إذا (!src->انکڑے) {
		ناپ_t sz_st = 0;
		packing_انکڑے_جندرا(&منزل_pack);
		src->انکڑے = odb_read_object(the_repository->objects,
					    &src_اندراج->idx.oid, &type,
					    &sz_st);
		sz = cast_ناپ_t_منزل_ulong(sz_st);
		packing_انکڑے_unlock(&منزل_pack);
		إذا (!src->انکڑے) {
			إذا (src_اندراج->preferred_ادھار) {
				ساكن صحيح warned = 0;
				إذا (!warned++)
					warning(_("object %s cannot be read"),
						oid_منزل_hex(&src_اندراج->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				أرجع 0;
			}
			die(_("object %s cannot be read"),
			    oid_منزل_hex(&src_اندراج->idx.oid));
		}
		إذا (sz != src_ناپ)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_منزل_hex(&src_اندراج->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_ناپ);
		*mem_usage += sz;
	}
	إذا (!src->اشاریہ) {
		src->اشاریہ = create_delta_اشاریہ(src->انکڑے, src_ناپ);
		إذا (!src->اشاریہ) {
			ساكن صحيح warned = 0;
			إذا (!warned++)
				warning(_("suboptimal pack - out of memory"));
			أرجع 0;
		}
		*mem_usage += sizeof_delta_اشاریہ(src->اشاریہ);
	}

	delta_buf = create_delta(src->اشاریہ, trg->انکڑے, trg_ناپ, &delta_ناپ_2, max_ناپ);
	إذا (!delta_buf)
		أرجع 0;

	إذا (DELTA(trg_اندراج)) {
		/* Prefer only shallower same-sized deltas. */
		إذا (delta_ناپ_2 == DELTA_ناپ(trg_اندراج) &&
		    src->ڈونگھائی + 1 >= trg->ڈونگھائی) {
			حرر(delta_buf);
			أرجع 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	حرر(trg_اندراج->delta_انکڑے);
	cache_جندرا();
	إذا (trg_اندراج->delta_انکڑے) {
		delta_cache_ناپ -= DELTA_ناپ(trg_اندراج);
		trg_اندراج->delta_انکڑے = عدم;
	}
	إذا (delta_cacheable(src_ناپ, trg_ناپ, delta_ناپ_2)) {
		delta_cache_ناپ += delta_ناپ_2;
		cache_unlock();
		trg_اندراج->delta_انکڑے = xrealloc(delta_buf, delta_ناپ_2);
	} وإلا {
		cache_unlock();
		حرر(delta_buf);
	}

	سیٹ_DELTA(trg_اندراج, src_اندراج);
	سیٹ_DELTA_ناپ(trg_اندراج, delta_ناپ_2);
	trg->ڈونگھائی = src->ڈونگھائی + 1;

	أرجع 1;
}

ساكن بنانشانی صحيح check_delta_حد_بندی(بنية object_اندراج *me, بنانشانی صحيح n)
{
	بنية object_اندراج *بچہ = DELTA_بچہ(me);
	بنانشانی صحيح m = n;
	طالما (بچہ) {
		ثابت بنانشانی صحيح c = check_delta_حد_بندی(بچہ, n + 1);
		إذا (m < c)
			m = c;
		بچہ = DELTA_SIBLING(بچہ);
	}
	أرجع m;
}

ساكن ناپ_t free_unpacked(بنية unpacked *n)
{
	ناپ_t freed_mem = sizeof_delta_اشاریہ(n->اشاریہ);
	free_delta_اشاریہ(n->اشاریہ);
	n->اشاریہ = عدم;
	إذا (n->انکڑے) {
		freed_mem += ناپ_3(n->اندراج);
		FREE_AND_NULL(n->انکڑے);
	}
	n->اندراج = عدم;
	n->ڈونگھائی = 0;
	أرجع freed_mem;
}

ساكن فراغ لبھو_deltas(بنية object_اندراج **list, بنانشانی *list_ناپ,
			صحيح کھڑکی, صحيح ڈونگھائی, بنانشانی *processed)
{
	uint32_t i, idx = 0, گنتی = 0;
	بنية unpacked *array;
	ناپ_t mem_usage = 0;

	CALLOC_ARRAY(array, کھڑکی);

	لكل (;;) {
		بنية object_اندراج *اندراج;
		بنية unpacked *n = array + idx;
		صحيح j, max_ڈونگھائی, بہترین_ادھار = -1;

		ترقی_جندرا();
		إذا (!*list_ناپ) {
			ترقی_unlock();
			اكسر;
		}
		اندراج = *list++;
		(*list_ناپ)--;
		إذا (!اندراج->preferred_ادھار) {
			(*processed)++;
			display_ترقی(ترقی_state, *processed);
		}
		ترقی_unlock();

		mem_usage -= free_unpacked(n);
		n->اندراج = اندراج;

		طالما (کھڑکی_حافظہ_حد_بندی &&
		       mem_usage > کھڑکی_حافظہ_حد_بندی &&
		       گنتی > 1) {
			ثابت uint32_t پوچھ = (idx + کھڑکی - گنتی) % کھڑکی;
			mem_usage -= free_unpacked(array + پوچھ);
			گنتی--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		إذا (اندراج->preferred_ادھار)
			اذهب اگلا;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_ڈونگھائی = ڈونگھائی;
		إذا (DELTA_بچہ(اندراج)) {
			max_ڈونگھائی -= check_delta_حد_بندی(اندراج, 0);
			إذا (max_ڈونگھائی <= 0)
				اذهب اگلا;
		}

		j = کھڑکی;
		طالما (--j > 0) {
			صحيح ret;
			uint32_t other_idx = idx + j;
			بنية unpacked *m;
			إذا (other_idx >= کھڑکی)
				other_idx -= کھڑکی;
			m = array + other_idx;
			إذا (!m->اندراج)
				اكسر;
			ret = try_delta(n, m, max_ڈونگھائی, &mem_usage);
			إذا (ret < 0)
				اكسر;
			وإلا إذا (ret > 0)
				بہترین_ادھار = other_idx;
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
		إذا (اندراج->delta_انکڑے && !pack_منزل_stdout) {
			بنانشانی لما ناپ;

			ناپ = do_compress(&اندراج->delta_انکڑے, DELTA_ناپ(اندراج));
			إذا (ناپ < (1U << OE_Z_DELTA_بٹ)) {
				اندراج->z_delta_ناپ = ناپ;
				cache_جندرا();
				delta_cache_ناپ -= DELTA_ناپ(اندراج);
				delta_cache_ناپ += اندراج->z_delta_ناپ;
				cache_unlock();
			} وإلا {
				FREE_AND_NULL(اندراج->delta_انکڑے);
				اندراج->z_delta_ناپ = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		إذا (DELTA(اندراج) && max_ڈونگھائی <= n->ڈونگھائی)
			استمر;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		إذا (DELTA(اندراج)) {
			بنية unpacked وٹاندرا_2 = array[بہترین_ادھار];
			صحيح dist = (کھڑکی + idx - بہترین_ادھار) % کھڑکی;
			صحيح dst = بہترین_ادھار;
			طالما (dist--) {
				صحيح src = (dst + 1) % کھڑکی;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = وٹاندرا_2;
		}

		اگلا:
		idx++;
		إذا (گنتی + 1 < کھڑکی)
			گنتی++;
		إذا (idx >= کھڑکی)
			idx = 0;
	}

	لكل (i = 0; i < کھڑکی; ++i) {
		free_delta_اشاریہ(array[i].اشاریہ);
		حرر(array[i].انکڑے);
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
	بنية object_اندراج **list;
	بنية packing_region *regions;
	بنانشانی list_ناپ;
	بنانشانی بچیا;
	صحيح کھڑکی;
	صحيح ڈونگھائی;
	صحيح working;
	صحيح انکڑے_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	بنانشانی *processed;
};

ساكن pthread_cond_t ترقی_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساكن فراغ init_threaded_تلاش(فراغ)
{
	pthread_mutex_init(&cache_mutex, عدم);
	pthread_mutex_init(&ترقی_mutex, عدم);
	pthread_cond_init(&ترقی_cond, عدم);
}

ساكن فراغ cleanup_threaded_تلاش(فراغ)
{
	pthread_cond_destroy(&ترقی_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&ترقی_mutex);
}

ساكن فراغ *threaded_لبھو_deltas(فراغ *arg)
{
	بنية thread_params *me = arg;

	ترقی_جندرا();
	طالما (me->بچیا) {
		ترقی_unlock();

		لبھو_deltas(me->list, &me->بچیا,
			    me->کھڑکی, me->ڈونگھائی, me->processed);

		ترقی_جندرا();
		me->working = 0;
		pthread_cond_signal(&ترقی_cond);
		ترقی_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_جندرا(&me->mutex);
		طالما (!me->انکڑے_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->انکڑے_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ترقی_جندرا();
	}
	ترقی_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_لبھو_deltas(بنية object_اندراج **list, بنانشانی list_ناپ,
			   صحيح کھڑکی, صحيح ڈونگھائی, بنانشانی *processed)
{
	بنية thread_params *p;
	صحيح i, ret, سرگرم_دھاگے = 0;

	init_threaded_تلاش();

	إذا (delta_تلاش_دھاگے <= 1) {
		لبھو_deltas(list, &list_ناپ, کھڑکی, ڈونگھائی, processed);
		cleanup_threaded_تلاش();
		أرجع;
	}
	إذا (ترقی > pack_منزل_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_تلاش_دھاگے);
	CALLOC_ARRAY(p, delta_تلاش_دھاگے);

	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_تلاش_دھاگے; i++) {
		بنانشانی sub_ناپ = list_ناپ / (delta_تلاش_دھاگے - i);

		/* don't use too small segments or no deltas will be found */
		إذا (sub_ناپ < 2*کھڑکی && i+1 < delta_تلاش_دھاگے)
			sub_ناپ = 0;

		p[i].کھڑکی = کھڑکی;
		p[i].ڈونگھائی = ڈونگھائی;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].انکڑے_ready = 0;

		/* try to split chunks on "path" boundaries */
		طالما (sub_ناپ && sub_ناپ < list_ناپ &&
		       list[sub_ناپ]->hash &&
		       list[sub_ناپ]->hash == list[sub_ناپ-1]->hash)
			sub_ناپ++;

		p[i].list = list;
		p[i].list_ناپ = sub_ناپ;
		p[i].بچیا = sub_ناپ;

		list += sub_ناپ;
		list_ناپ -= sub_ناپ;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_تلاش_دھاگے; i++) {
		إذا (!p[i].list_ناپ)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_لبھو_deltas, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		سرگرم_دھاگے++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (سرگرم_دھاگے) {
		بنية thread_params *نشانہ = عدم;
		بنية thread_params *victim = عدم;
		بنانشانی sub_ناپ = 0;

		ترقی_جندرا();
		لكل (;;) {
			لكل (i = 0; !نشانہ && i < delta_تلاش_دھاگے; i++)
				إذا (!p[i].working)
					نشانہ = &p[i];
			إذا (نشانہ)
				اكسر;
			pthread_cond_wait(&ترقی_cond, &ترقی_mutex);
		}

		لكل (i = 0; i < delta_تلاش_دھاگے; i++)
			إذا (p[i].بچیا > 2*کھڑکی &&
			    (!victim || victim->بچیا < p[i].بچیا))
				victim = &p[i];
		إذا (victim) {
			sub_ناپ = victim->بچیا / 2;
			list = victim->list + victim->list_ناپ - sub_ناپ;
			طالما (sub_ناپ && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_ناپ--;
			}
			إذا (!sub_ناپ) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_ناپ = victim->بچیا / 2;
				list -= sub_ناپ;
			}
			نشانہ->list = list;
			victim->list_ناپ -= sub_ناپ;
			victim->بچیا -= sub_ناپ;
		}
		نشانہ->list_ناپ = sub_ناپ;
		نشانہ->بچیا = sub_ناپ;
		نشانہ->working = 1;
		ترقی_unlock();

		pthread_mutex_جندرا(&نشانہ->mutex);
		نشانہ->انکڑے_ready = 1;
		pthread_cond_signal(&نشانہ->cond);
		pthread_mutex_unlock(&نشانہ->mutex);

		إذا (!sub_ناپ) {
			pthread_join(نشانہ->thread, عدم);
			pthread_cond_destroy(&نشانہ->cond);
			pthread_mutex_destroy(&نشانہ->mutex);
			سرگرم_دھاگے--;
		}
	}
	cleanup_threaded_تلاش();
	حرر(p);
}

ساكن صحيح obj_is_packed(ثابت بنية object_id *oid)
{
	أرجع packlist_لبھو(&منزل_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_شامل_اے(bitmap_git, reuse_packfile_bitmap, oid));
}

ساكن فراغ جوڑو_ٹیگ_chain(ثابت بنية object_id *oid)
{
	بنية ٹیگ *ٹیگ;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	إذا (obj_is_packed(oid))
		أرجع;

	ٹیگ = lookup_ٹیگ(the_repository, oid);
	طالما (1) {
		إذا (!ٹیگ || parse_ٹیگ(the_repository, ٹیگ) || !ٹیگ->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_منزل_hex(oid));

		جوڑو_object_اندراج(&ٹیگ->object.oid, OBJ_ٹیگ, عدم, 0);

		إذا (ٹیگ->tagged->type != OBJ_ٹیگ)
			أرجع;

		ٹیگ = (بنية ٹیگ *)ٹیگ->tagged;
	}
}

ساكن صحيح جوڑو_ref_ٹیگ(ثابت بنية reference *ref, فراغ *cb_انکڑے بن_ورتیا)
{
	بنية object_id peeled;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		جوڑو_ٹیگ_chain(ref->oid);
	أرجع 0;
}

ساكن صحيح should_attempt_deltas(بنية object_اندراج *اندراج)
{
	إذا (DELTA(اندراج))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		أرجع 0;

	إذا (!اندراج->type_valid ||
	    oe_ناپ_less_than(&منزل_pack, اندراج, 50))
		أرجع 0;

	إذا (اندراج->no_try_delta)
		أرجع 0;

	إذا (!اندراج->preferred_ادھار) {
		إذا (oe_type(اندراج) < 0)
			die(_("unable to get type of object %s"),
				oid_منزل_hex(&اندراج->idx.oid));
	} وإلا إذا (oe_type(اندراج) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		أرجع 0;
	}

	أرجع 1;
}

ساكن فراغ لبھو_deltas_for_region(بنية object_اندراج *list,
				   بنية packing_region *region,
				   بنانشانی صحيح *processed)
{
	بنية object_اندراج **delta_list;
	بنانشانی صحيح delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	لكل (ناپ_t i = 0; i < region->nr; i++) {
		بنية object_اندراج *اندراج = list + region->شروع + i;
		إذا (should_attempt_deltas(اندراج))
			delta_list[delta_list_nr++] = اندراج;
	}

	QSORT(delta_list, delta_list_nr, type_ناپ_ترتیب_دیو);
	لبھو_deltas(delta_list, &delta_list_nr, کھڑکی, ڈونگھائی, processed);
	حرر(delta_list);
}

ساكن فراغ لبھو_deltas_by_region(بنية object_اندراج *list,
				  بنية packing_region *regions,
				  ناپ_t شروع, ناپ_t nr)
{
	بنانشانی صحيح processed = 0;
	ناپ_t ترقی_nr;

	إذا (!nr)
		أرجع;

	ترقی_nr = regions[nr - 1].شروع + regions[nr - 1].nr;

	إذا (ترقی)
		ترقی_state = شروع_ترقی(the_repository,
						_("Compressing objects by path"),
						ترقی_nr);

	طالما (nr--)
		لبھو_deltas_for_region(list,
				       &regions[شروع++],
				       &processed);

	display_ترقی(ترقی_state, ترقی_nr);
	stop_ترقی(&ترقی_state);
}

ساكن فراغ *threaded_لبھو_deltas_by_رستہ(فراغ *arg)
{
	بنية thread_params *me = arg;

	ترقی_جندرا();
	طالما (me->بچیا) {
		طالما (me->بچیا) {
			ترقی_unlock();
			لبھو_deltas_for_region(منزل_pack.objects,
					       me->regions,
					       me->processed);
			ترقی_جندرا();
			me->بچیا--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&ترقی_cond);
		ترقی_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_جندرا(&me->mutex);
		طالما (!me->انکڑے_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->انکڑے_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		ترقی_جندرا();
	}
	ترقی_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_لبھو_deltas_by_region(بنية object_اندراج *list,
				     بنية packing_region *regions,
				     uint32_t شروع, uint32_t nr)
{
	بنية thread_params *p;
	صحيح i, ret, سرگرم_دھاگے = 0;
	بنانشانی صحيح processed = 0;
	uint32_t ترقی_nr;
	init_threaded_تلاش();

	إذا (!nr)
		أرجع;

	ترقی_nr =  regions[nr - 1].شروع + regions[nr - 1].nr;
	إذا (delta_تلاش_دھاگے <= 1) {
		لبھو_deltas_by_region(list, regions, شروع, nr);
		cleanup_threaded_تلاش();
		أرجع;
	}

	إذا (ترقی > pack_منزل_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_تلاش_دھاگے),
			   delta_تلاش_دھاگے);
	CALLOC_ARRAY(p, delta_تلاش_دھاگے);

	إذا (ترقی)
		ترقی_state = شروع_ترقی(the_repository,
						_("Compressing objects by path"),
						ترقی_nr);
	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_تلاش_دھاگے; i++) {
		بنانشانی sub_ناپ = nr / (delta_تلاش_دھاگے - i);

		p[i].کھڑکی = کھڑکی;
		p[i].ڈونگھائی = ڈونگھائی;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].انکڑے_ready = 0;

		p[i].regions = regions;
		p[i].list_ناپ = sub_ناپ;
		p[i].بچیا = sub_ناپ;

		regions += sub_ناپ;
		nr -= sub_ناپ;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_تلاش_دھاگے; i++) {
		إذا (!p[i].list_ناپ)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_لبھو_deltas_by_رستہ, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		سرگرم_دھاگے++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (سرگرم_دھاگے) {
		بنية thread_params *نشانہ = عدم;
		بنية thread_params *victim = عدم;
		بنانشانی sub_ناپ = 0;

		ترقی_جندرا();
		لكل (;;) {
			لكل (i = 0; !نشانہ && i < delta_تلاش_دھاگے; i++)
				إذا (!p[i].working)
					نشانہ = &p[i];
			إذا (نشانہ)
				اكسر;
			pthread_cond_wait(&ترقی_cond, &ترقی_mutex);
		}

		لكل (i = 0; i < delta_تلاش_دھاگے; i++)
			إذا (p[i].بچیا > 2*کھڑکی &&
			    (!victim || victim->بچیا < p[i].بچیا))
				victim = &p[i];
		إذا (victim) {
			sub_ناپ = victim->بچیا / 2;
			نشانہ->regions = victim->regions + victim->بچیا - sub_ناپ;
			victim->list_ناپ -= sub_ناپ;
			victim->بچیا -= sub_ناپ;
		}
		نشانہ->list_ناپ = sub_ناپ;
		نشانہ->بچیا = sub_ناپ;
		نشانہ->working = 1;
		ترقی_unlock();

		pthread_mutex_جندرا(&نشانہ->mutex);
		نشانہ->انکڑے_ready = 1;
		pthread_cond_signal(&نشانہ->cond);
		pthread_mutex_unlock(&نشانہ->mutex);

		إذا (!sub_ناپ) {
			pthread_join(نشانہ->thread, عدم);
			pthread_cond_destroy(&نشانہ->cond);
			pthread_mutex_destroy(&نشانہ->mutex);
			سرگرم_دھاگے--;
		}
	}
	cleanup_threaded_تلاش();
	حرر(p);

	display_ترقی(ترقی_state, ترقی_nr);
	stop_ترقی(&ترقی_state);
}

ساكن فراغ prepare_pack(صحيح کھڑکی, صحيح ڈونگھائی)
{
	بنية object_اندراج **delta_list;
	uint32_t i, nr_deltas;
	بنانشانی n;

	إذا (use_delta_islands)
		resolve_درخت_islands(the_repository, ترقی, &منزل_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	إذا (!pack_منزل_stdout)
		do_check_packed_object_چکری_جانچ = 1;

	إذا (!منزل_pack.nr_objects || !کھڑکی || !ڈونگھائی)
		أرجع;

	إذا (رستہ_walk)
		ll_لبھو_deltas_by_region(منزل_pack.objects, منزل_pack.regions,
					 0, منزل_pack.nr_regions);

	ALLOC_ARRAY(delta_list, منزل_pack.nr_objects);
	nr_deltas = n = 0;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		بنية object_اندراج *اندراج = منزل_pack.objects + i;

		إذا (!should_attempt_deltas(اندراج))
			استمر;

		إذا (!اندراج->preferred_ادھار)
			nr_deltas++;

		delta_list[n++] = اندراج;
	}

	إذا (nr_deltas && n > 1) {
		بنانشانی nr_پورا = 0;

		إذا (ترقی)
			ترقی_state = شروع_ترقی(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_ناپ_ترتیب_دیو);
		ll_لبھو_deltas(delta_list, n, کھڑکی+1, ڈونگھائی, &nr_پورا);
		stop_ترقی(&ترقی_state);
		إذا (nr_پورا != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	حرر(delta_list);
}

ساكن صحيح git_pack_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(k, "pack.window")) {
		کھڑکی = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.windowmemory")) {
		کھڑکی_حافظہ_حد_بندی = git_config_ulong(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.depth")) {
		ڈونگھائی = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_ناپ = git_config_ناپ_t(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_ناپ = git_config_int(k, v, ctx->kvi);
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
		use_bitmap_اشاریہ_default = git_config_bool(k, v);
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
		delta_تلاش_دھاگے = git_config_int(k, v, ctx->kvi);
		إذا (delta_تلاش_دھاگے < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_تلاش_دھاگے);
		إذا (!HAVE_دھاگے && delta_تلاش_دھاگے != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_تلاش_دھاگے = 0;
		}
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.نسخہ = git_config_int(k, v, ctx->kvi);
		إذا (pack_idx_opts.نسخہ > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.نسخہ);
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
		ثابت حرف *oid_سرا, *pack_سرا;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		بنية object_id pack_hash;

		إذا (!v)
			أرجع config_error_nonbool(k);

		ex = xmalloc(حجم(*ex));
		إذا (parse_oid_hex(v, &ex->e.oid, &oid_سرا) ||
		    *oid_سرا != ' ' ||
		    parse_oid_hex(oid_سرا + 1, &pack_hash, &pack_سرا) ||
		    *pack_سرا != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		إذا (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_سرا - oid_سرا);
		حافظہ_نقل(ex->pack_hash_hex, oid_سرا + 1, pack_سرا - oid_سرا - 1);
		ex->uri = xstrdup(pack_سرا + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	أرجع git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ساكن صحيح stdin_packs_لبھیا_nr;
ساكن صحيح stdin_packs_hints_nr;

ساكن صحيح جوڑو_object_اندراج_from_pack(ثابت بنية object_id *oid,
				      بنية packed_git *p,
				      uint32_t pos,
				      فراغ *انکڑے_2)
{
	off_t ofs;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type = OBJ_NONE;

	display_ترقی(ترقی_state, ++nr_تکیا);

	إذا (have_duplicate_اندراج(oid, 0))
		أرجع 0;

	stdin_packs_لبھیا_nr++;

	ofs = nth_packed_object_وتھ(p, pos);

	oi.typep = &type;
	إذا (packed_object_info(عدم, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_منزل_hex(oid), p->pack_name);
	} وإلا إذا (type == OBJ_COMMIT) {
		بنية rev_info *revs = انکڑے_2;
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
		جوڑو_pending_oid(revs, عدم, oid, 0);
	}

	إذا (!want_object_وچ_pack(oid, 0, &p, &ofs))
		أرجع 0;

	create_object_اندراج(oid, type, 0, 0, 0, p, ofs);

	أرجع 0;
}

ساكن فراغ show_object_pack_hint(بنية object *object, ثابت حرف *name,
				  فراغ *انکڑے)
{
	تعداد stdin_packs_منوال منوال = *(تعداد stdin_packs_منوال *)انکڑے;
	إذا (منوال == STDIN_PACKS_منوال_FOLLOW) {
		إذا (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			أرجع;
		جوڑو_object_اندراج(&object->oid, object->type, name, 0);
	} وإلا {
		بنية object_اندراج *oe = packlist_لبھو(&منزل_pack, &object->oid);
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

ساكن فراغ show_commit_pack_hint(بنية commit *commit, فراغ *انکڑے)
{
	تعداد stdin_packs_منوال منوال = *(تعداد stdin_packs_منوال *)انکڑے;

	إذا (منوال == STDIN_PACKS_منوال_FOLLOW) {
		show_object_pack_hint((بنية object *)commit, "", انکڑے);
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
	بنية stdin_pack_info *a = ((ثابت بنية string_list_شے*)_a)->util;
	بنية stdin_pack_info *b = ((ثابت بنية string_list_شے*)_b)->util;

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

ساكن صحيح stdin_packs_include_check_obj(بنية object *obj, فراغ *انکڑے بن_ورتیا)
{
	أرجع !has_object_kept_pack(منزل_pack.repo, &obj->oid,
				     KEPT_PACK_وچ_CORE);
}

ساكن صحيح stdin_packs_include_check(بنية commit *commit, فراغ *انکڑے)
{
	أرجع stdin_packs_include_check_obj((بنية object *)commit, انکڑے);
}

ساكن فراغ stdin_packs_جوڑو_pack_entries(بنية strmap *packs,
					 بنية rev_info *revs)
{
	بنية string_list کلیدیں = STRING_LIST_INIT_NODUP;
	بنية string_list_شے *شے;
	بنية hashmap_iter iter;
	بنية strmap_اندراج *اندراج;

	strmap_for_each_اندراج(packs, &iter, اندراج) {
		بنية stdin_pack_info *info = اندراج->قدر;
		إذا (!info->p)
			die(_("could not find pack '%s'"), اندراج->کلید);

		string_list_append(&کلیدیں, اندراج->کلید)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(کلیدیں.اشیاء, کلیدیں.nr, pack_mtime_cmp);

	for_each_string_list_شے(شے, &کلیدیں) {
		بنية stdin_pack_info *info = شے->util;

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
			for_each_object_وچ_pack(info->p,
						جوڑو_object_اندراج_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ترتیب);
	}

	string_list_clear(&کلیدیں, 0);
}

ساكن فراغ stdin_packs_read_داخلہ(بنية rev_info *revs,
				   تعداد stdin_packs_منوال منوال)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strmap packs = STRMAP_INIT;
	بنية packed_git *p;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		بنية stdin_pack_info *info;
		تعداد stdin_pack_info_قسم قسم = STDIN_PACK_INCLUDE;
		ثابت حرف *کلید = buf.buf;

		إذا (!*کلید)
			استمر;
		وإلا إذا (*کلید == '^')
			قسم = STDIN_PACK_EXCLUDE_CLOSED;
		وإلا إذا (*کلید == '!' && منوال == STDIN_PACKS_منوال_FOLLOW)
			قسم = STDIN_PACK_EXCLUDE_OPEN;

		إذا (قسم != STDIN_PACK_INCLUDE)
			کلید++;

		info = strmap_get(&packs, کلید);
		إذا (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, کلید, info);
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
			p->pack_keep_وچ_core = 1;
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
			p->pack_keep_وچ_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_جوڑو_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

ساكن فراغ جوڑو_unreachable_loose_objects(بنية rev_info *revs);

ساكن فراغ read_stdin_packs(بنية repository *repo,
			     تعداد stdin_packs_منوال منوال, صحيح rev_list_unpacked)
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
	revs.keep_pack_cache_flags |= KEPT_PACK_وچ_CORE;
	revs.blob_objects = 1;
	revs.درخت_objects = 1;
	revs.ٹیگ_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_وچ_core = 1;
	إذا (منوال == STDIN_PACKS_منوال_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_وچ_core_open = 1;
	}
	stdin_packs_read_داخلہ(&revs, منوال);
	إذا (rev_list_unpacked)
		جوڑو_unreachable_loose_objects(&revs);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &منوال);

	release_revisions(&revs);

	trace2_انکڑے_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_لبھیا_nr);
	trace2_انکڑے_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

ساكن فراغ جوڑو_cruft_object_اندراج(ثابت بنية object_id *oid, تعداد object_type type,
				   بنية packed_git *pack, off_t وتھ,
				   ثابت حرف *name, uint32_t mtime)
{
	بنية object_اندراج *اندراج;

	display_ترقی(ترقی_state, ++nr_تکیا);

	اندراج = packlist_لبھو(&منزل_pack, oid);
	إذا (اندراج) {
		إذا (name) {
			اندراج->hash = pack_name_hash_fn(name);
			اندراج->no_try_delta = no_try_delta(name);
		}
	} وإلا {
		إذا (!want_object_وچ_pack_mtime(oid, 0, &pack, &وتھ, mtime))
			أرجع;
		إذا (!pack && type == OBJ_BLOB) {
			بنية odb_سروت_files *files =
				odb_سروت_files_downcast(the_repository->objects->سروت);
			صحيح لبھیا_2 = 0;

			لكل (بنية odb_files_dir *dir = files->dirs; !لبھیا_2 && dir; dir = dir->اگلا)
				إذا (!odb_سروت_read_object_info(&dir->loose->ادھار, oid, عدم, 0, عدم))
					لبھیا_2 = 1;

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
			إذا (!لبھیا_2)
				أرجع;
		}

		اندراج = create_object_اندراج(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, وتھ);
	}

	إذا (mtime > oe_cruft_mtime(&منزل_pack, اندراج))
		oe_سیٹ_cruft_mtime(&منزل_pack, اندراج, mtime);
	أرجع;
}

ساكن فراغ show_cruft_object(بنية object *obj, ثابت حرف *name, فراغ *انکڑے بن_ورتیا)
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
	جوڑو_cruft_object_اندراج(&obj->oid, obj->type, عدم, 0, name, cruft_expiration);
}

ساكن فراغ show_cruft_commit(بنية commit *commit, فراغ *انکڑے)
{
	show_cruft_object((بنية object*)commit, عدم, انکڑے);
}

ساكن صحيح cruft_include_check_obj(بنية object *obj, فراغ *انکڑے بن_ورتیا)
{
	أرجع !has_object_kept_pack(منزل_pack.repo, &obj->oid, KEPT_PACK_وچ_CORE);
}

ساكن صحيح cruft_include_check(بنية commit *commit, فراغ *انکڑے)
{
	أرجع cruft_include_check_obj((بنية object*)commit, انکڑے);
}

ساكن فراغ سیٹ_cruft_mtime(ثابت بنية object *object,
			    بنية packed_git *pack,
			    off_t وتھ, ویلا_t mtime)
{
	جوڑو_cruft_object_اندراج(&object->oid, object->type, pack, وتھ, عدم,
			       mtime);
}

ساكن فراغ mark_pack_kept_وچ_core(بنية string_list *packs, بنانشانی keep)
{
	بنية string_list_شے *شے = عدم;
	for_each_string_list_شے(شے, packs) {
		بنية packed_git *p = شے->util;
		إذا (!p)
			die(_("could not find pack '%s'"), شے->string);
		إذا (p->is_cruft && keep)
			ignore_packed_keep_وچ_core_has_cruft = 1;
		p->pack_keep_وچ_core = keep;
	}
}

ساكن فراغ جوڑو_objects_وچ_unpacked_packs(فراغ);

ساكن فراغ enumerate_cruft_objects(فراغ)
{
	إذا (ترقی)
		ترقی_state = شروع_ترقی(the_repository,
						_("Enumerating cruft objects"), 0);

	جوڑو_objects_وچ_unpacked_packs();
	جوڑو_unreachable_loose_objects(عدم);

	stop_ترقی(&ترقی_state);
}

ساكن فراغ enumerate_and_traverse_cruft_objects(بنية string_list *fresh_packs)
{
	بنية packed_git *p;
	بنية rev_info revs;
	صحيح ret;

	repo_init_revisions(the_repository, &revs, عدم);

	revs.ٹیگ_objects = 1;
	revs.درخت_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	إذا (ترقی)
		ترقی_state = شروع_ترقی(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = جوڑو_unseen_recent_objects_منزل_traversal(&revs, cruft_expiration,
						     سیٹ_cruft_mtime, 1);
	stop_ترقی(&ترقی_state);

	إذا (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_وچ_core = 0;
	mark_pack_kept_وچ_core(fresh_packs, 1);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	إذا (ترقی)
		ترقی_state = شروع_ترقی(the_repository,
						_("Traversing cruft objects"), 0);
	nr_تکیا = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, عدم);

	stop_ترقی(&ترقی_state);
	release_revisions(&revs);
}

ساكن فراغ read_cruft_objects(فراغ)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list discard_packs = STRING_LIST_INIT_DUP;
	بنية string_list fresh_packs = STRING_LIST_INIT_DUP;
	بنية packed_git *p;

	ignore_packed_keep_وچ_core = 1;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		إذا (!buf.len)
			استمر;

		إذا (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		وإلا
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_ترتیب_دیو(&discard_packs);
	string_list_ترتیب_دیو(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *pack_name = pack_basename(p);
		بنية string_list_شے *شے;

		شے = string_list_lookup(&fresh_packs, pack_name);
		إذا (!شے)
			شے = string_list_lookup(&discard_packs, pack_name);

		إذا (شے) {
			شے->util = p;
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
			p->pack_keep_وچ_core = 1;
		}
	}

	mark_pack_kept_وچ_core(&fresh_packs, 1);
	mark_pack_kept_وچ_core(&discard_packs, 0);

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
	حرف سطر[GIT_MAX_HEXSZ + 1 + رستہ_MAX + 2];
	بنية object_id oid;
	ثابت حرف *p;

	لكل (;;) {
		إذا (!fgets(سطر, حجم(سطر), stdin)) {
			إذا (feof(stdin))
				اكسر;
			إذا (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			إذا (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			استمر;
		}
		إذا (سطر[0] == '-') {
			إذا (get_oid_hex(سطر+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    سطر);
			جوڑو_preferred_ادھار(&oid);
			استمر;
		}
		إذا (parse_oid_hex(سطر, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), سطر);

		جوڑو_preferred_ادھار_object(p + 1);
		جوڑو_object_اندراج(&oid, OBJ_NONE, p + 1, 0);
	}
}

ساكن فراغ show_commit(بنية commit *commit, فراغ *انکڑے بن_ورتیا)
{
	جوڑو_object_اندراج(&commit->object.oid, OBJ_COMMIT, عدم, 0);

	إذا (write_bitmap_اشاریہ)
		اشاریہ_commit_for_bitmap(commit);

	إذا (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ساكن فراغ show_object(بنية object *obj, ثابت حرف *name,
			فراغ *انکڑے بن_ورتیا)
{
	جوڑو_preferred_ادھار_object(name);
	جوڑو_object_اندراج(&obj->oid, obj->type, name, 0);

	إذا (use_delta_islands)
		ریکارڈ_درخت_ڈونگھائی(&obj->oid, name);
}

ساكن فراغ show_object__ma_allow_any(بنية object *obj, ثابت حرف *name, فراغ *انکڑے)
{
	أكد(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	إذا (!odb_has_object(the_repository->objects, &obj->oid, 0))
		أرجع;

	show_object(obj, name, انکڑے);
}

ساكن فراغ show_object__ma_allow_promisor(بنية object *obj, ثابت حرف *name, فراغ *انکڑے)
{
	أكد(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	إذا (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(منزل_pack.repo, &obj->oid))
		أرجع;

	show_object(obj, name, انکڑے);
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

ساكن فراغ show_کنارا(بنية commit *commit)
{
	جوڑو_preferred_ادھار(&commit->object.oid);
}

ساكن صحيح جوڑو_object_وچ_unpacked_pack(ثابت بنية object_id *oid,
				       بنية object_info *oi,
				       فراغ *انکڑے بن_ورتیا)
{
	إذا (cruft) {
		جوڑو_cruft_object_اندراج(oid, OBJ_NONE, oi->سروت_infop->u.packed.pack,
				       oi->سروت_infop->u.packed.وتھ, عدم,
				       *oi->mtimep);
	} وإلا {
		جوڑو_object_اندراج(oid, OBJ_NONE, "", 0);
	}
	أرجع 0;
}

ساكن فراغ جوڑو_objects_وچ_unpacked_packs(فراغ)
{
	بنية odb_سروت_files *files =
		odb_سروت_files_downcast(منزل_pack.repo->objects->سروت);
	ویلا_t mtime;
	بنية odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ترتیب |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_وچ_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	بنية odb_سروت_info سروت_info;
	بنية object_info oi = {
		.mtimep = &mtime,
		.سروت_infop = &سروت_info,
	};

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا) {
		إذا (!dir->local)
			استمر;

		إذا (odb_سروت_for_each_object(&dir->packed->ادھار, &oi,
					       جوڑو_object_وچ_unpacked_pack, عدم, &opts))
			die(_("cannot open pack index"));
	}
}

ساكن صحيح جوڑو_loose_object(ثابت بنية object_id *oid, ثابت حرف *رستہ,
			    فراغ *انکڑے)
{
	بنية rev_info *revs = انکڑے;
	تعداد object_type type = odb_read_object_info(the_repository->objects, oid, عدم);

	إذا (type < 0) {
		warning(_("loose object at %s could not be examined"), رستہ);
		أرجع 0;
	}

	إذا (cruft) {
		بنية stat st;
		إذا (stat(رستہ, &st) < 0) {
			إذا (errno == ENOENT)
				أرجع 0;
			أرجع error_errno("unable to stat %s", oid_منزل_hex(oid));
		}

		جوڑو_cruft_object_اندراج(oid, type, عدم, 0, عدم,
				       st.st_mtime);
	} وإلا {
		جوڑو_object_اندراج(oid, type, "", 0);
	}

	إذا (revs && type == OBJ_COMMIT)
		جوڑو_pending_oid(revs, عدم, oid, 0);

	أرجع 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ساكن فراغ جوڑو_unreachable_loose_objects(بنية rev_info *revs)
{
	for_each_loose_file_وچ_سروت(the_repository->objects->سروت,
				      جوڑو_loose_object, عدم, عدم, revs);
}

ساكن صحيح has_sha1_pack_kept_or_nonlocal(ثابت بنية object_id *oid)
{
	ساكن بنية packed_git *آخری_لبھیا = عدم;
	بنية packed_git *p;

	إذا (آخری_لبھیا && لبھو_pack_اندراج_one(oid, آخری_لبھیا))
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		إذا (p == آخری_لبھیا)
			استمر;

		إذا ((!p->pack_local || p->pack_keep || p->pack_keep_وچ_core) &&
		    لبھو_pack_اندراج_one(oid, p)) {
			آخری_لبھیا = p;
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

ساكن صحيح force_object_loose(بنية odb_سروت *سروت,
			      ثابت بنية object_id *oid,
			      ثابت ویلا_t *mtime)
{
	بنية odb_سروت_files *files = odb_سروت_files_downcast(سروت);
	ثابت بنية git_hash_algo *compat = سروت->odb->repo->compat_hash_algo;
	بنية object_info oi = OBJECT_INFO_INIT;
	بنية object_id compat_oid, *compat_oid_p = عدم;
	تعداد object_type type;
	فراغ *buf = عدم;
	ناپ_t len;
	صحيح ret;

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->اگلا)
		إذا (!odb_سروت_read_object_info(&dir->loose->ادھار, oid, عدم, 0, عدم))
			أرجع 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	إذا (odb_read_object_info_extended(سروت->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_منزل_hex(oid));
		اذهب باہرلا;
	}

	إذا (compat) {
		إذا (repo_oid_منزل_algop(سروت->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_منزل_hex(oid), compat->name);
			اذهب باہرلا;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_سروت_write_object(&files->dirs->loose->ادھار, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

باہرلا:
	حرر(buf);
	أرجع ret;
}

ساكن فراغ loosen_بن_ورتیا_packed_objects(فراغ)
{
	بنية packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	بنية object_id oid;

	repo_for_each_pack(the_repository, p) {
		إذا (!p->pack_local || p->pack_keep || p->pack_keep_وچ_core)
			استمر;

		إذا (open_pack_اشاریہ(p))
			die(_("cannot open pack index"));

		لكل (i = 0; i < p->عدد_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			إذا (!packlist_لبھو(&منزل_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				إذا (force_object_loose(the_repository->objects->سروت,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_انکڑے_intmax("pack-objects", the_repository,
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
	       !ignore_packed_keep_وچ_core &&
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
	name_hash_نسخہ = 1;

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

		nr_نتیجہ += reuse_packfile_objects;
		nr_تکیا += reuse_packfile_objects;
		display_ترقی(ترقی_state, nr_تکیا);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &جوڑو_object_اندراج_from_bitmap);
	أرجع 0;
}

ساكن فراغ ریکارڈ_recent_object(بنية object *obj,
				 ثابت حرف *name بن_ورتیا,
				 فراغ *انکڑے بن_ورتیا)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ساكن فراغ ریکارڈ_recent_commit(بنية commit *commit, فراغ *انکڑے بن_ورتیا)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ساكن صحيح mark_bitmap_preferred_tip(ثابت بنية reference *ref, فراغ *انکڑے بن_ورتیا)
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

ساكن صحيح جوڑو_objects_by_رستہ(ثابت حرف *رستہ,
			       بنية oid_array *oids,
			       تعداد object_type type,
			       فراغ *انکڑے)
{
	ناپ_t oe_شروع = منزل_pack.nr_objects;
	ناپ_t oe_سرا;
	بنانشانی صحيح *processed = انکڑے;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	لكل (ناپ_t i = 0; i < oids->nr; i++) {
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

		جوڑو_object_اندراج(oid, type, رستہ, exclude);

		إذا (type == OBJ_COMMIT) {
			بنية commit *commit;

			إذا (!write_bitmap_اشاریہ && !use_delta_islands)
				استمر;

			commit = lookup_commit(the_repository, oid);
			إذا (!commit)
				die(_("could not find commit %s"), oid_منزل_hex(oid));
			إذا (write_bitmap_اشاریہ)
				اشاریہ_commit_for_bitmap(commit);
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
		} وإلا إذا (type == OBJ_درخت && use_delta_islands) {
			ریکارڈ_درخت_ڈونگھائی(oid, رستہ);
		}
	}

	oe_سرا = منزل_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	إذا (oe_سرا == oe_شروع || !کھڑکی)
		أرجع 0;

	ALLOC_GROW(منزل_pack.regions,
		   منزل_pack.nr_regions + 1,
		   منزل_pack.nr_regions_alloc);

	منزل_pack.regions[منزل_pack.nr_regions].شروع = oe_شروع;
	منزل_pack.regions[منزل_pack.nr_regions].nr = oe_سرا - oe_شروع;
	منزل_pack.nr_regions++;

	*processed += oids->nr;
	display_ترقی(ترقی_state, *processed);

	أرجع 0;
}

ساكن صحيح get_object_list_رستہ_walk(بنية rev_info *revs)
{
	بنية رستہ_walk_info info = رستہ_WALK_INFO_INIT;
	بنانشانی صحيح processed = 0;
	صحيح نتیجہ;

	info.revs = revs;
	info.رستہ_fn = جوڑو_objects_by_رستہ;
	info.رستہ_fn_انکڑے = &processed;

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
	info.کنارا_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	نتیجہ = walk_objects_by_رستہ(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	رستہ_walk_info_clear(&info);

	أرجع نتیجہ;
}

ساكن فراغ get_object_list(بنية rev_info *revs, بنية strvec *argv)
{
	بنية تیاری_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	حرف سطر[1000];
	صحيح flags = 0;
	صحيح save_warning;

	save_commit_بفر = 0;
	تیاری_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	طالما (fgets(سطر, حجم(سطر), stdin) != عدم) {
		صحيح len = لڑی_لمبائی(سطر);
		إذا (len && سطر[len - 1] == '\n')
			سطر[--len] = 0;
		إذا (!len)
			اكسر;
		إذا (*سطر == '-') {
			إذا (!strcmp(سطر, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_اشاریہ = 0;
				استمر;
			}
			إذا (starts_with(سطر, "--shallow ")) {
				بنية object_id oid;
				إذا (get_oid_hex(سطر + 10, &oid))
					die("not an object name '%s'", سطر + 10);
				رجسٹر_shallow(the_repository, &oid);
				use_bitmap_اشاریہ = 0;
				استمر;
			}
			die(_("not a rev '%s'"), سطر);
		}
		إذا (handle_revision_arg(سطر, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), سطر);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	إذا (use_bitmap_اشاریہ && !get_object_list_from_bitmap(revs))
		أرجع;

	إذا (use_delta_islands)
		لدو_delta_islands(the_repository, ترقی);

	إذا (write_bitmap_اشاریہ)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      عدم);

	إذا (!fn_show_object)
		fn_show_object = show_object;

	إذا (رستہ_walk) {
		إذا (get_object_list_رستہ_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			رستہ_walk = 0;
		}
	}

	إذا (!رستہ_walk) {
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_کنارے_uninteresting(revs, show_کنارا, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				عدم);
	}

	إذا (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		إذا (جوڑو_unseen_recent_objects_منزل_traversal(revs,
				unpack_unreachable_expiration, عدم, 0))
			die(_("unable to add recent objects"));
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, ریکارڈ_recent_commit,
				     ریکارڈ_recent_object, عدم);
	}

	إذا (keep_unreachable)
		جوڑو_objects_وچ_unpacked_packs();
	إذا (pack_loose_unreachable)
		جوڑو_unreachable_loose_objects(عدم);
	إذا (unpack_unreachable)
		loosen_بن_ورتیا_packed_objects();

	oid_array_clear(&recent_objects);
}

ساكن فراغ جوڑو_extra_kept_packs(ثابت بنية string_list *ناں)
{
	بنية packed_git *p;

	إذا (!ناں->nr)
		أرجع;

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *name = basename(p->pack_name);
		صحيح i;

		إذا (!p->pack_local)
			استمر;

		لكل (i = 0; i < ناں->nr; i++)
			إذا (!fspathcmp(name, ناں->اشیاء[i].string))
				اكسر;

		إذا (i < ناں->nr) {
			p->pack_keep_وچ_core = 1;
			ignore_packed_keep_وچ_core = 1;
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

ساكن صحيح option_parse_اشاریہ_نسخہ(ثابت بنية option *opt,
				      ثابت حرف *arg, صحيح unset)
{
	بنية pack_idx_option *popts = opt->قدر;
	حرف *c;
	ثابت حرف *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->نسخہ = strtoul(val, &c, 10);
	إذا (popts->نسخہ > 2)
		die(_("unsupported index version %s"), val);
	إذا (*c == ',' && c[1])
		popts->off32_حد_بندی = strtoul(c+1, &c, 0);
	إذا (*c || popts->off32_حد_بندی & 0x80000000)
		die(_("bad index version '%s'"), val);
	أرجع 0;
}

ساكن صحيح option_parse_unpack_unreachable(ثابت بنية option *opt بن_ورتیا,
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

ساكن صحيح option_parse_cruft_expiration(ثابت بنية option *opt بن_ورتیا,
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

ساكن صحيح is_not_وچ_promisor_pack_obj(بنية object *obj, فراغ *انکڑے بن_ورتیا)
{
	بنية odb_سروت_info سروت_info;
	بنية object_info info = {
		.سروت_infop = &سروت_info,
	};

	إذا (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	أرجع سروت_info.سروت->type != ODB_سروت_PACKED || !سروت_info.u.packed.pack->pack_promisor;
}

ساكن صحيح is_not_وچ_promisor_pack(بنية commit *commit, فراغ *انکڑے) {
	أرجع is_not_وچ_promisor_pack_obj((بنية object *) commit, انکڑے);
}

ساكن صحيح parse_stdin_packs_منوال(ثابت بنية option *opt, ثابت حرف *arg,
				  صحيح unset)
{
	تعداد stdin_packs_منوال *منوال = opt->قدر;

	إذا (unset)
		*منوال = STDIN_PACKS_منوال_NONE;
	وإلا إذا (!arg || !*arg)
		*منوال = STDIN_PACKS_منوال_STANDARD;
	وإلا إذا (!strcmp(arg, "follow"))
		*منوال = STDIN_PACKS_منوال_FOLLOW;
	وإلا
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	أرجع 0;
}

صحيح cmd_pack_objects(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اگیتر,
		     بنية repository *repo)
{
	صحيح use_اندرونی_rev_list = 0;
	صحيح all_ترقی_implied = 0;
	بنية strvec rp = STRVEC_INIT;
	صحيح rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	صحيح rev_list_اشاریہ = 0;
	تعداد stdin_packs_منوال stdin_packs = STDIN_PACKS_منوال_NONE;
	بنية string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	بنية option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &ترقی, عدم,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_سیٹ_INT(0, "progress", &ترقی,
			    N_("show progress meter"), 1),
		OPT_سیٹ_INT(0, "all-progress", &ترقی,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_ترقی_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_اشاریہ_نسخہ),
		OPT_UNSIGNED(0, "max-pack-size", &pack_ناپ_حد_بندی,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &کھڑکی,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &کھڑکی_حافظہ_حد_بندی,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &ڈونگھائی,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_تلاش_دھاگے,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_خالی_تھاواں,
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
		OPT_BOOL(0, "stdout", &pack_منزل_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_ٹیگ,
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
		OPT_BOOL(0, "path-walk", &رستہ_walk,
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
		OPT_سرا(),
	};

	إذا (DFS_عدد_حالتاں > (1 << OE_DFS_STATE_بٹ))
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
	إذا (git_env_bool(GIT_TEST_NO_WRITE_REV_اشاریہ, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	ترقی = isatty(2);
	argc = parse_options(argc, argv, اگیتر, pack_objects_options,
			     pack_usage, 0);

	إذا (argc) {
		ادھار_name = argv[0];
		argc--;
	}
	إذا (pack_منزل_stdout != !ادھار_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	إذا (رستہ_walk < 0) {
		إذا (use_bitmap_اشاریہ > 0 ||
		    !use_اندرونی_rev_list)
			رستہ_walk = 0;
		وإلا إذا (the_repository->gitdir &&
			 the_repository->settings.pack_use_رستہ_walk)
			رستہ_walk = 1;
		وإلا
			رستہ_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	إذا (ڈونگھائی < 0)
		ڈونگھائی = 0;
	إذا (ڈونگھائی >= (1 << OE_ڈونگھائی_بٹ)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			ڈونگھائی, (1 << OE_ڈونگھائی_بٹ) - 1);
		ڈونگھائی = (1 << OE_ڈونگھائی_بٹ) - 1;
	}
	إذا (cache_max_small_delta_ناپ >= (1U << OE_Z_DELTA_بٹ)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_بٹ) - 1);
		cache_max_small_delta_ناپ = (1U << OE_Z_DELTA_بٹ) - 1;
	}
	إذا (کھڑکی < 0)
		کھڑکی = 0;

	strvec_ڈالو(&rp, "pack-objects");

	إذا (رستہ_walk) {
		ثابت حرف *option = عدم;
		إذا (!رستہ_walk_filter_compatible(&filter_options))
			option = "--filter";

		إذا (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			رستہ_walk = 0;
		}
	}
	إذا (رستہ_walk) {
		strvec_ڈالو(&rp, "--objects");
	} وإلا إذا (thin) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} وإلا
		strvec_ڈالو(&rp, "--objects");

	إذا (rev_list_all) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--all");
	}
	إذا (rev_list_reflog) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--reflog");
	}
	إذا (rev_list_اشاریہ) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--indexed-objects");
	}
	إذا (rev_list_unpacked && !stdin_packs) {
		use_اندرونی_rev_list = 1;
		strvec_ڈالو(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_بہترین_effort,
				  "--exclude-promisor-objects-best-effort");
	إذا (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		إذا (!stdin_packs) {
			use_اندرونی_rev_list = 1;
			strvec_ڈالو(&rp, "--exclude-promisor-objects");
		}
	} وإلا إذا (exclude_promisor_objects_بہترین_effort) {
		use_اندرونی_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	إذا (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_اندرونی_rev_list = 1;

	إذا (!reuse_object)
		reuse_delta = 0;
	إذا (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	وإلا إذا (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_بہترین_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	إذا (!delta_تلاش_دھاگے)	/* --threads=0 means autodetect */
		delta_تلاش_دھاگے = online_cpus();

	إذا (!HAVE_دھاگے && delta_تلاش_دھاگے != 1)
		warning(_("no threads support, ignoring --threads"));
	إذا (!pack_منزل_stdout && !pack_ناپ_حد_بندی)
		pack_ناپ_حد_بندی = pack_ناپ_حد_بندی_cfg;
	إذا (pack_منزل_stdout && pack_ناپ_حد_بندی)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	إذا (pack_ناپ_حد_بندی && pack_ناپ_حد_بندی < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_ناپ_حد_بندی = 1024*1024;
	}

	إذا (!pack_منزل_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	إذا (!rev_list_all || !rev_list_reflog || !rev_list_اشاریہ)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	إذا (stdin_packs && use_اندرونی_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	إذا (cruft) {
		إذا (use_اندرونی_rev_list)
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
		use_bitmap_اشاریہ_default = 0;

	إذا (use_bitmap_اشاریہ < 0)
		use_bitmap_اشاریہ = use_bitmap_اشاریہ_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	إذا (!use_اندرونی_rev_list || (!pack_منزل_stdout && write_bitmap_اشاریہ) || is_repository_shallow(the_repository))
		use_bitmap_اشاریہ = 0;

	إذا (pack_منزل_stdout || !rev_list_all)
		write_bitmap_اشاریہ = 0;

	إذا (name_hash_نسخہ < 0)
		name_hash_نسخہ = (صحيح)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_نسخہ();

	إذا (use_delta_islands)
		strvec_ڈالو(&rp, "--topo-order");

	إذا (ترقی && all_ترقی_implied)
		ترقی = 2;

	جوڑو_extra_kept_packs(&keep_pack_list);
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
	prepare_packing_انکڑے(the_repository, &منزل_pack);

	إذا (ترقی && !cruft)
		ترقی_state = شروع_ترقی(the_repository,
						_("Enumerating objects"), 0);
	إذا (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} وإلا إذا (cruft) {
		read_cruft_objects();
	} وإلا إذا (!use_اندرونی_rev_list) {
		read_object_list_from_stdin();
	} وإلا {
		بنية rev_info revs;

		repo_init_revisions(the_repository, &revs, عدم);
		list_objects_filter_copy(&revs.filter, &filter_options);
		إذا (exclude_promisor_objects_بہترین_effort) {
			revs.include_check = is_not_وچ_promisor_pack;
			revs.include_check_obj = is_not_وچ_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_ادھار();
	إذا (include_ٹیگ && nr_نتیجہ)
		refs_for_each_ٹیگ_ref(get_main_ref_store(the_repository),
				      جوڑو_ref_ٹیگ, عدم);
	stop_ترقی(&ترقی_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	إذا (non_خالی_تھاواں && !nr_نتیجہ)
		اذهب cleanup;
	إذا (nr_نتیجہ) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(کھڑکی, ڈونگھائی);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	إذا (ترقی)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_ورتیا_nr);

	trace2_انکڑے_intmax("pack-objects", the_repository, "written", written);
	trace2_انکڑے_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_انکڑے_intmax("pack-objects", the_repository, "reused", reused);
	trace2_انکڑے_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_انکڑے_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_انکڑے_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_ورتیا_nr);

cleanup:
	clear_packing_انکڑے(&منزل_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	أرجع 0;
}
