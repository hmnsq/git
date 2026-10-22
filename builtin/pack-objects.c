#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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
ساكن بنية packing_داده مقصد_pack;

ساكن مضمن بنية object_مدخل *oe_delta(
		ثابت بنية packing_داده *pack,
		ثابت بنية object_مدخل *e)
{
	إذا (!e->delta_idx)
		أرجع عدم;
	إذا (e->ext_پایه)
		أرجع &pack->ext_bases[e->delta_idx - 1];
	وإلا
		أرجع &pack->objects[e->delta_idx - 1];
}

ساكن مضمن اندازه_t oe_delta_اندازه(بنية packing_داده *pack,
				   ثابت بنية object_مدخل *e)
{
	إذا (e->delta_اندازه_valid)
		أرجع e->delta_اندازه;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	أرجع pack->delta_اندازه_2[e - pack->objects];
}

اندازه_t oe_get_اندازه_slow(بنية packing_داده *pack,
			ثابت بنية object_مدخل *e);

ساكن مضمن اندازه_t oe_اندازه(بنية packing_داده *pack,
			     ثابت بنية object_مدخل *e)
{
	إذا (e->اندازه_valid)
		أرجع e->اندازه_2;

	أرجع oe_get_اندازه_slow(pack, e);
}

ساكن مضمن فراغ oe_مجموعه_delta(بنية packing_داده *pack,
				بنية object_مدخل *e,
				بنية object_مدخل *delta)
{
	إذا (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_idx = 0;
}

ساكن مضمن بنية object_مدخل *oe_delta_sibling(
		ثابت بنية packing_داده *pack,
		ثابت بنية object_مدخل *e)
{
	إذا (e->delta_sibling_idx)
		أرجع &pack->objects[e->delta_sibling_idx - 1];
	أرجع عدم;
}

ساكن مضمن بنية object_مدخل *oe_delta_فرزند(
		ثابت بنية packing_داده *pack,
		ثابت بنية object_مدخل *e)
{
	إذا (e->delta_فرزند_idx)
		أرجع &pack->objects[e->delta_فرزند_idx - 1];
	أرجع عدم;
}

ساكن مضمن فراغ oe_مجموعه_delta_فرزند(بنية packing_داده *pack,
				      بنية object_مدخل *e,
				      بنية object_مدخل *delta)
{
	إذا (delta)
		e->delta_فرزند_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_فرزند_idx = 0;
}

ساكن مضمن فراغ oe_مجموعه_delta_sibling(بنية packing_داده *pack,
					بنية object_مدخل *e,
					بنية object_مدخل *delta)
{
	إذا (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_sibling_idx = 0;
}

ساكن مضمن فراغ oe_مجموعه_اندازه(بنية packing_داده *pack,
			       بنية object_مدخل *e,
			       اندازه_t اندازه)
{
	إذا (اندازه < pack->oe_اندازه_حد) {
		e->اندازه_2 = اندازه;
		e->اندازه_valid = 1;
	} وإلا {
		e->اندازه_valid = 0;
		إذا (oe_get_اندازه_slow(pack, e) != اندازه)
			BUG("'size' is supposed to be the object size!");
	}
}

ساكن مضمن فراغ oe_مجموعه_delta_اندازه(بنية packing_داده *pack,
				     بنية object_مدخل *e,
				     اندازه_t اندازه)
{
	إذا (اندازه < pack->oe_delta_اندازه_حد) {
		e->delta_اندازه = اندازه;
		e->delta_اندازه_valid = 1;
	} وإلا {
		packing_داده_قفل(pack);
		إذا (!pack->delta_اندازه_2)
			ALLOC_ARRAY(pack->delta_اندازه_2, pack->nr_alloc);
		packing_داده_unlock(pack);

		pack->delta_اندازه_2[e - pack->objects] = اندازه;
		e->delta_اندازه_valid = 0;
	}
}

#تعریف_کن درون_PACK(obj) oe_درون_pack(&مقصد_pack, obj)
#تعریف_کن اندازه_3(obj) oe_اندازه(&مقصد_pack, obj)
#تعریف_کن مجموعه_اندازه(obj,اندازه) oe_مجموعه_اندازه(&مقصد_pack, obj, اندازه)
#تعریف_کن DELTA_اندازه(obj) oe_delta_اندازه(&مقصد_pack, obj)
#تعریف_کن DELTA(obj) oe_delta(&مقصد_pack, obj)
#تعریف_کن DELTA_فرزند(obj) oe_delta_فرزند(&مقصد_pack, obj)
#تعریف_کن DELTA_SIBLING(obj) oe_delta_sibling(&مقصد_pack, obj)
#تعریف_کن مجموعه_DELTA(obj, val) oe_مجموعه_delta(&مقصد_pack, obj, val)
#تعریف_کن مجموعه_DELTA_EXT(obj, oid) oe_مجموعه_delta_ext(&مقصد_pack, obj, oid)
#تعریف_کن مجموعه_DELTA_اندازه(obj, val) oe_مجموعه_delta_اندازه(&مقصد_pack, obj, val)
#تعریف_کن مجموعه_DELTA_فرزند(obj, val) oe_مجموعه_delta_فرزند(&مقصد_pack, obj, val)
#تعریف_کن مجموعه_DELTA_SIBLING(obj, val) oe_مجموعه_delta_sibling(&مقصد_pack, obj, val)

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

ساكن بنية pack_idx_مدخل **written_list;
ساكن uint32_t nr_نتیجه, nr_written, nr_دیده;
ساكن بنية bitmap_شاخص *bitmap_git;
ساكن uint32_t write_layer;

ساكن صحيح non_خالی;
ساكن صحيح reuse_delta = 1, reuse_object = 1;
ساكن صحيح keep_unreachable, unpack_unreachable, include_برچسب;
ساكن timestamp_t unpack_unreachable_expiration;
ساكن صحيح pack_loose_unreachable;
ساكن صحيح cruft;
ساكن صحيح shallow = 0;
ساكن timestamp_t cruft_expiration;
ساكن صحيح local;
ساكن صحيح have_non_local_packs;
ساكن صحيح incremental;
ساكن صحيح ignore_packed_keep_on_disk;
ساكن صحيح ignore_packed_keep_درون_core;
ساكن صحيح ignore_packed_keep_درون_core_open;
ساكن صحيح ignore_packed_keep_درون_core_has_cruft;
ساكن صحيح allow_ofs_delta;
ساكن بنية pack_idx_option pack_idx_opts;
ساكن ثابت حرف *پایه_name;
ساكن صحيح پیشرفت = 1;
ساكن صحيح پنجره = 10;
ساكن بیعلامت دراز pack_اندازه_حد;
ساكن صحيح عمق = 50;
ساكن صحيح delta_جستجو_نخها;
ساكن صحيح pack_مقصد_stdout;
ساكن صحيح sparse;
ساكن صحيح thin;
ساكن صحيح مسیر_walk = -1;
ساكن صحيح رقم_preferred_پایه;
ساكن بنية پیشرفت *پیشرفت_state;

ساكن بنية bitmapped_pack *reuse_packfiles;
ساكن اندازه_t reuse_packfiles_nr;
ساكن اندازه_t reuse_packfiles_استفاده_شده_nr;
ساكن uint32_t reuse_packfile_objects;
ساكن بنية bitmap *reuse_packfile_bitmap;

ساكن صحيح use_bitmap_شاخص_default = 1;
ساكن صحيح use_bitmap_شاخص = -1;
ساكن تعداد {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
ساكن تعداد {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_شاخص;
ساكن uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

ساكن صحيح exclude_promisor_objects;
ساكن صحيح exclude_promisor_objects_بهترین_effort;

ساكن صحيح use_delta_islands;

ساكن اندازه_t delta_cache_اندازه = 0;
ساكن اندازه_t max_delta_cache_اندازه = DEFAULT_DELTA_CACHE_اندازه;
ساكن بیعلامت دراز cache_max_small_delta_اندازه = 1000;

ساكن بیعلامت دراز پنجره_حافظه_حد = 0;

ساكن بنية string_list uri_protocols = STRING_LIST_INIT_NODUP;

تعداد missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساكن تعداد missing_action arg_missing_action;
ساكن show_object_fn fn_show_object;

بنية configured_exclusion {
	بنية oidmap_مدخل e;
	حرف *pack_hash_hex;
	حرف *uri;
};
ساكن بنية oidmap configured_exclusions;

ساكن بنية oidset excluded_by_config;
ساكن صحيح name_hash_نسخه = -1;

تعداد stdin_packs_نما {
	STDIN_PACKS_نما_NONE,
	STDIN_PACKS_نما_STANDARD,
	STDIN_PACKS_نما_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
ساكن فراغ validate_name_hash_نسخه(فراغ)
{
	إذا (name_hash_نسخه < 1 || name_hash_نسخه > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_نسخه);
	إذا (write_bitmap_شاخص && name_hash_نسخه != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_نسخه = 1;
	}
}

ساكن مضمن uint32_t pack_name_hash_fn(ثابت حرف *name)
{
	ساكن صحيح دیده_نسخه = -1;

	إذا (دیده_نسخه < 0)
		دیده_نسخه = name_hash_نسخه;
	وإلا إذا (دیده_نسخه != name_hash_نسخه)
		BUG("name hash version changed from %d to %d mid-process",
		    دیده_نسخه, name_hash_نسخه);

	بدل (name_hash_نسخه) {
	حالة 1:
		أرجع pack_name_hash(name);

	حالة 2:
		أرجع pack_name_hash_v2((ثابت بیعلامت حرف *)name);

	افتراضي:
		BUG("invalid name-hash version: %d", name_hash_نسخه);
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
ساكن بنية commit **نمایهای_commits;
ساكن بیعلامت صحيح نمایهای_commits_nr;
ساكن بیعلامت صحيح نمایهای_commits_alloc;

ساكن فراغ شاخص_commit_for_bitmap(بنية commit *commit)
{
	إذا (نمایهای_commits_nr >= نمایهای_commits_alloc) {
		نمایهای_commits_alloc = (نمایهای_commits_alloc + 32) * 2;
		REALLOC_ARRAY(نمایهای_commits, نمایهای_commits_alloc);
	}

	نمایهای_commits[نمایهای_commits_nr++] = commit;
}

ساكن فراغ *get_delta(بنية object_مدخل *مدخل)
{
	بیعلامت دراز اندازه, پایه_اندازه;
	اندازه_t delta_اندازه_2;
	فراغ *buf, *پایه_buf, *delta_buf;
	تعداد object_type type;
	اندازه_t اندازه_st = 0, پایه_اندازه_st = 0;

	buf = odb_read_object(the_repository->objects, &مدخل->idx.oid,
			      &type, &اندازه_st);
	اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
	إذا (!buf)
		die(_("unable to read %s"), oid_مقصد_hex(&مدخل->idx.oid));
	پایه_buf = odb_read_object(the_repository->objects,
				   &DELTA(مدخل)->idx.oid, &type,
				   &پایه_اندازه_st);
	پایه_اندازه = cast_اندازه_t_مقصد_ulong(پایه_اندازه_st);
	إذا (!پایه_buf)
		die("unable to read %s",
		    oid_مقصد_hex(&DELTA(مدخل)->idx.oid));
	delta_buf = diff_delta(پایه_buf, پایه_اندازه,
			       buf, اندازه, &delta_اندازه_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	إذا (!delta_buf || delta_اندازه_2 != DELTA_اندازه(مدخل))
		BUG("delta size changed");
	حرر(buf);
	حرر(پایه_buf);
	أرجع delta_buf;
}

ساكن بیعلامت دراز do_compress(فراغ **pptr, بیعلامت دراز اندازه)
{
	git_zstream جریان;
	فراغ *درون, *بیرون;
	بیعلامت دراز maxsize;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	git_deflate_init(&جریان, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&جریان, اندازه);

	درون = *pptr;
	بیرون = xmalloc(maxsize);
	*pptr = بیرون;

	جریان.بعدی_درون = درون;
	جریان.avail_درون = اندازه;
	جریان.بعدی_بیرون = بیرون;
	جریان.avail_بیرون = maxsize;
	طالما (git_deflate(&جریان, Z_پایان) == Z_OK)
		; /* nothing */
	git_deflate_انتها(&جریان);

	حرر(درون);
	أرجع جریان.مجموع_بیرون;
}

ساكن بیعلامت دراز write_large_blob_داده(بنية odb_جریان *st, بنية hashfile *f,
					   ثابت بنية object_id *oid)
{
	git_zstream جریان;
	بیعلامت حرف ibuf[1024 * 16];
	بیعلامت حرف obuf[1024 * 16];
	بیعلامت دراز olen = 0;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	git_deflate_init(&جریان, cfg->pack_compression_level);

	لكل (;;) {
		ssize_t readlen;
		صحيح zret = Z_OK;
		readlen = odb_جریان_read(st, ibuf, حجم(ibuf));
		إذا (readlen == -1)
			die(_("unable to read %s"), oid_مقصد_hex(oid));

		جریان.بعدی_درون = ibuf;
		جریان.avail_درون = readlen;
		طالما ((جریان.avail_درون || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			جریان.بعدی_بیرون = obuf;
			جریان.avail_بیرون = حجم(obuf);
			zret = git_deflate(&جریان, readlen ? 0 : Z_پایان);
			hashwrite(f, obuf, جریان.بعدی_بیرون - obuf);
			olen += جریان.بعدی_بیرون - obuf;
		}
		إذا (جریان.avail_درون)
			die(_("deflate error (%d)"), zret);
		إذا (readlen == 0) {
			إذا (zret != Z_جریان_انتها)
				die(_("deflate error (%d)"), zret);
			اكسر;
		}
	}
	git_deflate_انتها(&جریان);
	أرجع olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ساكن صحيح check_pack_inflate(بنية packed_git *p,
		بنية pack_پنجره **w_curs,
		off_t جابه_جایی,
		off_t len,
		اندازه_t expect)
{
	git_zstream جریان;
	بیعلامت حرف fakebuf[4096], *درون;
	صحيح st;

	memset(&جریان, 0, حجم(جریان));
	git_inflate_init(&جریان);
	افعل {
		درون = use_pack(p, w_curs, جابه_جایی, &جریان.avail_درون);
		جریان.بعدی_درون = درون;
		جریان.بعدی_بیرون = fakebuf;
		جریان.avail_بیرون = حجم(fakebuf);
		st = git_inflate(&جریان, Z_پایان);
		جابه_جایی += جریان.بعدی_درون - درون;
	} طالما (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_انتها(&جریان);
	أرجع (st == Z_جریان_انتها &&
		جریان.مجموع_بیرون == expect &&
		جریان.مجموع_درون == len) ? 0 : -1;
}

ساكن فراغ copy_pack_داده(بنية hashfile *f,
		بنية packed_git *p,
		بنية pack_پنجره **w_curs,
		off_t جابه_جایی,
		off_t len)
{
	بیعلامت حرف *درون;
	اندازه_t avail;

	طالما (len) {
		درون = use_pack(p, w_curs, جابه_جایی, &avail);
		إذا (avail > len)
			avail = (بیعلامت دراز)len;
		hashwrite(f, درون, avail);
		جابه_جایی += avail;
		len -= avail;
	}
}

ساكن مضمن صحيح oe_اندازه_greater_than(بنية packing_داده *pack,
				       ثابت بنية object_مدخل *lhs,
				       اندازه_t rhs)
{
	إذا (lhs->اندازه_valid)
		أرجع lhs->اندازه_2 > rhs;
	إذا (rhs < pack->oe_اندازه_حد) /* rhs < 2^x <= lhs ? */
		أرجع 1;
	أرجع oe_get_اندازه_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ساكن بیعلامت دراز write_no_reuse_object(بنية hashfile *f, بنية object_مدخل *مدخل,
					   بیعلامت دراز حد, صحيح usable_delta)
{
	بیعلامت دراز اندازه, datalen;
	بیعلامت حرف سرآیند[MAX_PACK_OBJECT_سرآیند],
		      dheader[MAX_PACK_OBJECT_سرآیند];
	بیعلامت hdrlen;
	تعداد object_type type;
	فراغ *buf;
	بنية odb_جریان *st = عدم;
	ثابت بیعلامت hashsz = the_hash_algo->rawsz;

	إذا (!usable_delta) {
		إذا (oe_type(مدخل) == OBJ_BLOB &&
		    oe_اندازه_greater_than(&مقصد_pack, مدخل,
					 repo_settings_get_big_file_آستانه(the_repository)) &&
		    (st = odb_جریان_from_object(the_repository->objects, &مدخل->idx.oid,
						 عدم)) != عدم) {
			buf = عدم;
			type = st->type;
			اندازه = st->اندازه;
		} وإلا {
			اندازه_t اندازه_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &مدخل->idx.oid, &type,
					      &اندازه_st);
			اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
			إذا (!buf)
				die(_("unable to read %s"),
				    oid_مقصد_hex(&مدخل->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(مدخل->delta_داده);
		مدخل->z_delta_اندازه = 0;
	} وإلا إذا (مدخل->delta_داده) {
		اندازه = DELTA_اندازه(مدخل);
		buf = مدخل->delta_داده;
		مدخل->delta_داده = عدم;
		type = (allow_ofs_delta && DELTA(مدخل)->idx.جابه_جایی) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} وإلا {
		buf = get_delta(مدخل);
		اندازه = DELTA_اندازه(مدخل);
		type = (allow_ofs_delta && DELTA(مدخل)->idx.جابه_جایی) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	إذا (st)	/* large blob case, just assume we don't compress well */
		datalen = اندازه;
	وإلا إذا (مدخل->z_delta_اندازه)
		datalen = مدخل->z_delta_اندازه;
	وإلا
		datalen = do_compress(&buf, اندازه);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_درون_pack_object_سرآیند(سرآیند, حجم(سرآیند),
					      type, اندازه);

	إذا (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = مدخل->idx.جابه_جایی - DELTA(مدخل)->idx.جابه_جایی;
		بیعلامت pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد) {
			إذا (st)
				odb_جریان_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرآیند, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		إذا (حد && hdrlen + hashsz + datalen + hashsz >= حد) {
			إذا (st)
				odb_جریان_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرآیند, hdrlen);
		hashwrite(f, DELTA(مدخل)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} وإلا {
		إذا (حد && hdrlen + datalen + hashsz >= حد) {
			إذا (st)
				odb_جریان_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرآیند, hdrlen);
	}
	إذا (st) {
		datalen = write_large_blob_داده(st, f, &مدخل->idx.oid);
		odb_جریان_close(st);
	} وإلا {
		hashwrite(f, buf, datalen);
		حرر(buf);
	}

	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_reuse_object(بنية hashfile *f, بنية object_مدخل *مدخل,
				بیعلامت دراز حد, صحيح usable_delta)
{
	بنية packed_git *p = درون_PACK(مدخل);
	بنية pack_پنجره *w_curs = عدم;
	uint32_t pos;
	off_t جابه_جایی, cur;
	تعداد object_type type = oe_type(مدخل);
	تعداد object_type درون_pack_type;
	off_t datalen;
	بیعلامت حرف سرآیند[MAX_PACK_OBJECT_سرآیند],
		      dheader[MAX_PACK_OBJECT_سرآیند];
	بیعلامت hdrlen;
	ثابت بیعلامت hashsz = the_hash_algo->rawsz;
	اندازه_t مدخل_اندازه;

	cur = مدخل->درون_pack_جابه_جایی;
	درون_pack_type = unpack_object_سرآیند(p, &w_curs, &cur, &مدخل_اندازه);
	إذا (درون_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_مقصد_hex(&مدخل->idx.oid));

	إذا (DELTA(مدخل))
		type = (allow_ofs_delta && DELTA(مدخل)->idx.جابه_جایی) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_درون_pack_object_سرآیند(سرآیند, حجم(سرآیند),
					      type, مدخل_اندازه);

	جابه_جایی = مدخل->درون_pack_جابه_جایی;
	إذا (جابه_جایی_مقصد_pack_pos(p, جابه_جایی, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_مقصد_hex(&مدخل->idx.oid), (uintmax_t)جابه_جایی,
		    p->pack_name);
	datalen = pack_pos_مقصد_جابه_جایی(p, pos + 1) - جابه_جایی;
	إذا (!pack_مقصد_stdout && p->شاخص_نسخه > 1 &&
	    check_pack_بررسی_چرخهای(p, &w_curs, جابه_جایی, datalen,
			   pack_pos_مقصد_شاخص(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_مقصد_hex(&مدخل->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, مدخل, حد, usable_delta);
	}

	جابه_جایی += مدخل->درون_pack_سرآیند_اندازه;
	datalen -= مدخل->درون_pack_سرآیند_اندازه;

	إذا (!pack_مقصد_stdout && p->شاخص_نسخه == 1 &&
	    check_pack_inflate(p, &w_curs, جابه_جایی, datalen, مدخل_اندازه)) {
		error(_("corrupt packed object for %s"),
		      oid_مقصد_hex(&مدخل->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, مدخل, حد, usable_delta);
	}

	إذا (type == OBJ_OFS_DELTA) {
		off_t ofs = مدخل->idx.جابه_جایی - DELTA(مدخل)->idx.جابه_جایی;
		بیعلامت pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرآیند, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
		reused_delta++;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		إذا (حد && hdrlen + hashsz + datalen + hashsz >= حد) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرآیند, hdrlen);
		hashwrite(f, DELTA(مدخل)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} وإلا {
		إذا (حد && hdrlen + datalen + hashsz >= حد) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرآیند, hdrlen);
	}
	copy_pack_داده(f, p, &w_curs, جابه_جایی, datalen);
	unuse_pack(&w_curs);
	reused++;
	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_object(بنية hashfile *f,
			  بنية object_مدخل *مدخل,
			  off_t write_جابه_جایی)
{
	بیعلامت دراز حد;
	off_t len;
	صحيح usable_delta, مقصد_reuse;

	إذا (!pack_مقصد_stdout)
		بررسی_چرخهای_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	إذا (!pack_اندازه_حد || !nr_written)
		حد = 0;
	وإلا إذا (pack_اندازه_حد <= write_جابه_جایی)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		حد = 1;
	وإلا
		حد = pack_اندازه_حد - write_جابه_جایی;

	إذا (!DELTA(مدخل))
		usable_delta = 0;	/* no delta */
	وإلا إذا (!pack_اندازه_حد)
	       usable_delta = 1;	/* unlimited packfile */
	وإلا إذا (DELTA(مدخل)->idx.جابه_جایی == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	وإلا إذا (DELTA(مدخل)->idx.جابه_جایی)
		usable_delta = 1;	/* base already exists in this pack */
	وإلا
		usable_delta = 0;	/* base could end up in another pack */

	إذا (!reuse_object)
		مقصد_reuse = 0;	/* explicit */
	وإلا إذا (!درون_PACK(مدخل))
		مقصد_reuse = 0;	/* can't reuse what we don't have */
	وإلا إذا (oe_type(مدخل) == OBJ_REF_DELTA ||
		 oe_type(مدخل) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		مقصد_reuse = usable_delta;
				/* ... but pack split may override that */
	وإلا إذا (oe_type(مدخل) != مدخل->درون_pack_type)
		مقصد_reuse = 0;	/* pack has delta which is unusable */
	وإلا إذا (DELTA(مدخل))
		مقصد_reuse = 0;	/* we want to pack afresh */
	وإلا
		مقصد_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	إذا (!مقصد_reuse)
		len = write_no_reuse_object(f, مدخل, حد, usable_delta);
	وإلا
		len = write_reuse_object(f, مدخل, حد, usable_delta);
	إذا (!len)
		أرجع 0;

	إذا (usable_delta)
		written_delta++;
	written++;
	إذا (!pack_مقصد_stdout)
		مدخل->idx.بررسی_چرخهای_32 = بررسی_چرخهای_32_انتها(f);
	أرجع len;
}

تعداد write_one_وضعیت {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*نرمال*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ساكن تعداد write_one_وضعیت write_one(بنية hashfile *f,
				       بنية object_مدخل *e,
				       off_t *جابه_جایی)
{
	off_t اندازه;
	صحيح recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.جابه_جایی == 1);
	إذا (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_مقصد_hex(&e->idx.oid));
		أرجع WRITE_ONE_RECURSIVE;
	} وإلا إذا (e->idx.جابه_جایی || e->preferred_پایه) {
		/* offset is non zero if object is written already. */
		أرجع WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	إذا (DELTA(e)) {
		e->idx.جابه_جایی = 1; /* now recurse */
		بدل (write_one(f, DELTA(e), جابه_جایی)) {
		حالة WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			مجموعه_DELTA(e, عدم);
			اكسر;
		افتراضي:
			اكسر;
		حالة WRITE_ONE_BREAK:
			e->idx.جابه_جایی = recursing;
			أرجع WRITE_ONE_BREAK;
		}
	}

	e->idx.جابه_جایی = *جابه_جایی;
	اندازه = write_object(f, e, *جابه_جایی);
	إذا (!اندازه) {
		e->idx.جابه_جایی = recursing;
		أرجع WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جمع_کن_overflows(*جابه_جایی, اندازه))
		die(_("pack too large for current definition of off_t"));
	*جابه_جایی += اندازه;
	أرجع WRITE_ONE_WRITTEN;
}

ساكن صحيح mark_tagged(ثابت بنية reference *ref, فراغ *cb_داده استفادهنشده)
{
	بنية object_id peeled;
	بنية object_مدخل *مدخل = packlist_پیدا_کن(&مقصد_pack, ref->oid);

	إذا (مدخل)
		مدخل->tagged = 1;
	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		مدخل = packlist_پیدا_کن(&مقصد_pack, &peeled);
		إذا (مدخل)
			مدخل->tagged = 1;
	}
	أرجع 0;
}

ساكن مضمن بیعلامت حرف oe_layer(بنية packing_داده *pack,
				     بنية object_مدخل *e)
{
	إذا (!pack->layer)
		أرجع 0;
	أرجع pack->layer[e - pack->objects];
}

ساكن مضمن فراغ جمع_کن_مقصد_write_ترتیب(بنية object_مدخل **wo,
			       بیعلامت صحيح *endp,
			       بنية object_مدخل *e)
{
	إذا (e->filled || oe_layer(&مقصد_pack, e) != write_layer)
		أرجع;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ساكن فراغ جمع_کن_descendants_مقصد_write_ترتیب(بنية object_مدخل **wo,
					   بیعلامت صحيح *endp,
					   بنية object_مدخل *e)
{
	صحيح جمع_کن_مقصد_ترتیب = 1;
	طالما (e) {
		إذا (جمع_کن_مقصد_ترتیب) {
			بنية object_مدخل *s;
			/* add this node... */
			جمع_کن_مقصد_write_ترتیب(wo, endp, e);
			/* all its siblings... */
			لكل (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				جمع_کن_مقصد_write_ترتیب(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		إذا (DELTA_فرزند(e)) {
			جمع_کن_مقصد_ترتیب = 1;
			e = DELTA_فرزند(e);
		} وإلا {
			جمع_کن_مقصد_ترتیب = 0;
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

ساكن فراغ جمع_کن_family_مقصد_write_ترتیب(بنية object_مدخل **wo,
				      بیعلامت صحيح *endp,
				      بنية object_مدخل *e)
{
	بنية object_مدخل *ریشه;

	لكل (ریشه = e; DELTA(ریشه); ریشه = DELTA(ریشه))
		; /* nothing */
	جمع_کن_descendants_مقصد_write_ترتیب(wo, endp, ریشه);
}

ساكن فراغ compute_layer_ترتیب(بنية object_مدخل **wo, بیعلامت صحيح *wo_انتها)
{
	بیعلامت صحيح i, آخرین_untagged;
	بنية object_مدخل *objects = مقصد_pack.objects;

	لكل (i = 0; i < مقصد_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			اكسر;
		جمع_کن_مقصد_write_ترتیب(wo, wo_انتها, &objects[i]);
	}
	آخرین_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	لكل (; i < مقصد_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			جمع_کن_مقصد_write_ترتیب(wo, wo_انتها, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	لكل (i = آخرین_untagged; i < مقصد_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_برچسب)
			استمر;
		جمع_کن_مقصد_write_ترتیب(wo, wo_انتها, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	لكل (i = آخرین_untagged; i < مقصد_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_درخت)
			استمر;
		جمع_کن_مقصد_write_ترتیب(wo, wo_انتها, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	لكل (i = آخرین_untagged; i < مقصد_pack.nr_objects; i++) {
		إذا (!objects[i].filled && oe_layer(&مقصد_pack, &objects[i]) == write_layer)
			جمع_کن_family_مقصد_write_ترتیب(wo, wo_انتها, &objects[i]);
	}
}

ساكن بنية object_مدخل **compute_write_ترتیب(فراغ)
{
	uint32_t max_layers = 1;
	بیعلامت صحيح i, wo_انتها;

	بنية object_مدخل **wo;
	بنية object_مدخل *objects = مقصد_pack.objects;

	لكل (i = 0; i < مقصد_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		مجموعه_DELTA_فرزند(&objects[i], عدم);
		مجموعه_DELTA_SIBLING(&objects[i], عدم);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	لكل (i = مقصد_pack.nr_objects; i > 0;) {
		بنية object_مدخل *e = &objects[--i];
		إذا (!DELTA(e))
			استمر;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_فرزند_idx;
		مجموعه_DELTA_فرزند(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_برچسب_ref(get_main_ref_store(the_repository), mark_tagged,
			      عدم);

	إذا (use_delta_islands) {
		max_layers = compute_pack_layers(&مقصد_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, مقصد_pack.nr_objects);
	wo_انتها = 0;

	لكل (; write_layer < max_layers; ++write_layer)
		compute_layer_ترتیب(wo, &wo_انتها);

	إذا (wo_انتها != مقصد_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_انتها, مقصد_pack.nr_objects);

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

ساكن فراغ رکورد_reused_object(off_t where, off_t جابه_جایی)
{
	إذا (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == جابه_جایی)
		أرجع;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = جابه_جایی;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
ساكن off_t پیدا_کن_reused_جابه_جایی(off_t where)
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
				  اندازه_t pos, بنية hashfile *بیرون,
				  off_t pack_شروع,
				  بنية pack_پنجره **w_curs)
{
	off_t جابه_جایی, بعدی, cur;
	تعداد object_type type;
	اندازه_t اندازه;

	جابه_جایی = pack_pos_مقصد_جابه_جایی(reuse_packfile, pos);
	بعدی = pack_pos_مقصد_جابه_جایی(reuse_packfile, pos + 1);

	رکورد_reused_object(جابه_جایی,
			     جابه_جایی - (hashfile_مجموع(بیرون) - pack_شروع));

	cur = جابه_جایی;
	type = unpack_object_سرآیند(reuse_packfile, w_curs, &cur, &اندازه);
	أكد(type >= 0);

	إذا (type == OBJ_OFS_DELTA) {
		off_t پایه_جابه_جایی;
		off_t fixup;

		بیعلامت حرف سرآیند[MAX_PACK_OBJECT_سرآیند];
		بیعلامت len;

		پایه_جابه_جایی = get_delta_پایه(reuse_packfile, w_curs, &cur, type, جابه_جایی);
		أكد(پایه_جابه_جایی != 0);

		/* Convert to REF_DELTA if we must... */
		إذا (!allow_ofs_delta) {
			uint32_t پایه_pos;
			بنية object_id پایه_oid;

			إذا (جابه_جایی_مقصد_pack_pos(reuse_packfile, پایه_جابه_جایی, &پایه_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)پایه_جابه_جایی,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&پایه_oid, reuse_packfile,
					     pack_pos_مقصد_شاخص(reuse_packfile, پایه_pos));

			len = encode_درون_pack_object_سرآیند(سرآیند, حجم(سرآیند),
							   OBJ_REF_DELTA, اندازه);
			hashwrite(بیرون, سرآیند, len);
			hashwrite(بیرون, پایه_oid.hash, the_hash_algo->rawsz);
			copy_pack_داده(بیرون, reuse_packfile, w_curs, cur, بعدی - cur);
			أرجع;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = پیدا_کن_reused_جابه_جایی(جابه_جایی) -
			پیدا_کن_reused_جابه_جایی(پایه_جابه_جایی);
		إذا (fixup) {
			بیعلامت حرف ofs_سرآیند[MAX_PACK_OBJECT_سرآیند];
			بیعلامت i, ofs_len;
			off_t ofs = جابه_جایی - پایه_جابه_جایی - fixup;

			len = encode_درون_pack_object_سرآیند(سرآیند, حجم(سرآیند),
							   OBJ_OFS_DELTA, اندازه);

			i = حجم(ofs_سرآیند) - 1;
			ofs_سرآیند[i] = ofs & 127;
			طالما (ofs >>= 7)
				ofs_سرآیند[--i] = 128 | (--ofs & 127);

			ofs_len = حجم(ofs_سرآیند) - i;

			hashwrite(بیرون, سرآیند, len);
			hashwrite(بیرون, ofs_سرآیند + حجم(ofs_سرآیند) - ofs_len, ofs_len);
			copy_pack_داده(بیرون, reuse_packfile, w_curs, cur, بعدی - cur);
			أرجع;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_داده(بیرون, reuse_packfile, w_curs, جابه_جایی, بعدی - جابه_جایی);
}

ساكن اندازه_t write_reused_pack_verbatim(بنية bitmapped_pack *reuse_packfile,
					 بنية hashfile *بیرون,
					 بنية pack_پنجره **w_curs)
{
	اندازه_t pos = 0;
	اندازه_t انتها;

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
		أرجع reuse_packfile->bitmap_pos / بیتها_درون_EWORD;
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
	انتها = reuse_packfile->bitmap_nr / بیتها_درون_EWORD;
	إذا (reuse_packfile_bitmap->کلمه_alloc < انتها)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	طالما (pos < انتها && reuse_packfile_bitmap->واژگان[pos] == (eword_t)~0)
		pos++;

	إذا (pos) {
		off_t مقصد_write;

		written = (pos * بیتها_درون_EWORD);
		مقصد_write = pack_pos_مقصد_جابه_جایی(reuse_packfile->p, written)
			- حجم(بنية pack_سرآیند);

		/* We're recording one chunk, not one object. */
		رکورد_reused_object(حجم(بنية pack_سرآیند), 0);
		hashflush(بیرون);
		copy_pack_داده(بیرون, reuse_packfile->p, w_curs,
			حجم(بنية pack_سرآیند), مقصد_write);

		display_پیشرفت(پیشرفت_state, written);
	}
	أرجع pos;
}

ساكن فراغ write_reused_pack(بنية bitmapped_pack *reuse_packfile,
			      بنية hashfile *f)
{
	اندازه_t i = reuse_packfile->bitmap_pos / بیتها_درون_EWORD;
	uint32_t جابه_جایی;
	off_t pack_شروع = hashfile_مجموع(f) - حجم(بنية pack_سرآیند);
	بنية pack_پنجره *w_curs = عدم;

	إذا (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	لكل (; i < reuse_packfile_bitmap->کلمه_alloc; ++i) {
		eword_t کلمه = reuse_packfile_bitmap->واژگان[i];
		اندازه_t pos = (i * بیتها_درون_EWORD);

		لكل (جابه_جایی = 0; جابه_جایی < بیتها_درون_EWORD; ++جابه_جایی) {
			uint32_t pack_pos;
			إذا ((کلمه >> جابه_جایی) == 0)
				اكسر;

			جابه_جایی += ewah_بیت_ctz64(کلمه >> جابه_جایی);
			إذا (pos + جابه_جایی < reuse_packfile->bitmap_pos)
				استمر;
			إذا (pos + جابه_جایی >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				اذهب انجامشده;

			إذا (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				بنية multi_pack_شاخص *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				إذا (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_مقصد_midx(m, pos + جابه_جایی);
				pack_ofs = nth_midxed_جابه_جایی(m, midx_pos);

				إذا (جابه_جایی_مقصد_pack_pos(reuse_packfile->p,
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
				pack_pos = pos + جابه_جایی;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_شروع, &w_curs);
			display_پیشرفت(پیشرفت_state, ++written);
		}
	}

انجامشده:
	unuse_pack(&w_curs);
}

ساكن فراغ write_excluded_by_configs(فراغ)
{
	بنية oidset_iter iter;
	ثابت بنية object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	طالما ((oid = oidset_iter_بعدی(&iter))) {
		بنية configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		إذا (!ex)
			BUG("configured exclusion wasn't configured");
		write_درون_پر(1, ex->pack_hash_hex, طول_رشته(ex->pack_hash_hex));
		write_درون_پر(1, " ", 1);
		write_درون_پر(1, ex->uri, طول_رشته(ex->uri));
		write_درون_پر(1, "\n", 1);
	}
}

ساكن ثابت حرف no_تقسیم_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ساكن فراغ write_pack_file(فراغ)
{
	uint32_t i = 0, j;
	بنية hashfile *f;
	off_t جابه_جایی;
	uint32_t nr_مانده = nr_نتیجه;
	زمان_t آخرین_mtime = 0;
	بنية object_مدخل **write_ترتیب;
	off_t بایتها_written = 0;

	إذا (پیشرفت > pack_مقصد_stdout)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Writing objects"), nr_نتیجه);
	ALLOC_ARRAY(written_list, مقصد_pack.nr_objects);
	write_ترتیب = compute_write_ترتیب();

	افعل {
		بیعلامت حرف hash[GIT_MAX_RAWSZ];
		حرف *pack_tmp_name = عدم;

		إذا (pack_مقصد_stdout) {
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
				.پیشرفت = پیشرفت_state,
				.میانگیر_len = LARGE_PACKET_داده_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} وإلا {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		جابه_جایی = write_pack_سرآیند(f, nr_مانده);

		إذا (reuse_packfiles_nr) {
			أكد(pack_مقصد_stdout);
			لكل (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				إذا (reused_chunks_nr)
					reuse_packfiles_استفاده_شده_nr++;
			}
			جابه_جایی = hashfile_مجموع(f);
		}

		nr_written = 0;
		لكل (; i < مقصد_pack.nr_objects; i++) {
			بنية object_مدخل *e = write_ترتیب[i];
			إذا (write_one(f, e, &جابه_جایی) == WRITE_ONE_BREAK)
				اكسر;
			display_پیشرفت(پیشرفت_state, written);
		}

		بایتها_written += hashfile_مجموع(f) +
			the_repository->hash_algo->rawsz;
		إذا (pack_مقصد_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_درون_جریان | CSUM_CLOSE);
		} وإلا إذا (nr_written == nr_مانده) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_درون_جریان | CSUM_FSYNC | CSUM_CLOSE);
		} وإلا {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			صحيح fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_سرآیند_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, جابه_جایی);
			close(fd);
			إذا (write_bitmap_شاخص) {
				إذا (write_bitmap_شاخص != WRITE_BITMAP_QUIET)
					warning(_(no_تقسیم_warning));
				write_bitmap_شاخص = 0;
			}
		}

		إذا (!pack_مقصد_stdout) {
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
			} وإلا إذا (!آخرین_mtime) {
				آخرین_mtime = st.st_mtime;
			} وإلا {
				بنية utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --آخرین_mtime;
				إذا (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", پایه_name,
				    hash_مقصد_hex(hash));

			إذا (write_bitmap_شاخص) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &مقصد_pack,
						   عدم);
				bitmap_writer_مجموعه_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_شاخص(&bitmap_writer,
							       written_list);
			}

			إذا (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &مقصد_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			إذا (write_bitmap_شاخص) {
				اندازه_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_پیشرفت(&پیشرفت_state);

				bitmap_writer_show_پیشرفت(&bitmap_writer,
							    پیشرفت);
				bitmap_writer_select_commits(&bitmap_writer,
							     نمایهای_commits,
							     نمایهای_commits_nr);
				إذا (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_پایان(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_شاخص = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			حرر(idx_tmp_name);
			strbuf_release(&tmpname);
			حرر(pack_tmp_name);
			puts(hash_مقصد_hex(hash));
		}

		/* mark written objects as written to previous pack */
		لكل (j = 0; j < nr_written; j++) {
			written_list[j]->جابه_جایی = (off_t)-1;
		}
		nr_مانده -= nr_written;
	} طالما (nr_مانده && i < مقصد_pack.nr_objects);

	حرر(written_list);
	حرر(write_ترتیب);
	stop_پیشرفت(&پیشرفت_state);
	إذا (written != nr_نتیجه)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_نتیجه);
	trace2_داده_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_نتیجه);
	trace2_داده_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", بایتها_written);
}

ساكن صحيح no_try_delta(ثابت حرف *مسیر)
{
	ساكن بنية attr_check *check;

	إذا (!check)
		check = attr_check_initl("delta", عدم);
	git_check_attr(the_repository->شاخص, مسیر, check);
	إذا (ATTR_FALSE(check->اقلام[0].مقدار))
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
ساكن صحيح have_duplicate_مدخل(ثابت بنية object_id *oid,
				صحيح exclude)
{
	بنية object_مدخل *مدخل;

	إذا (reuse_packfile_bitmap &&
	    bitmap_walk_شامل_است(bitmap_git, reuse_packfile_bitmap, oid))
		أرجع 1;

	مدخل = packlist_پیدا_کن(&مقصد_pack, oid);
	إذا (!مدخل)
		أرجع 0;

	إذا (exclude) {
		إذا (!مدخل->preferred_پایه)
			nr_نتیجه--;
		مدخل->preferred_پایه = 1;
	}

	أرجع 1;
}

ساكن صحيح want_cruft_object_mtime(بنية repository *r,
				   ثابت بنية object_id *oid,
				   بیعلامت flags, uint32_t mtime)
{
	بنية odb_منبع_files *files = odb_منبع_files_downcast(r->objects->منبع);

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی) {
		بنية packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		لكل (; *cache; cache++) {
			بنية packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = پیدا_کن_pack_مدخل_one(oid, p);
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
			إذا (!بارگیری_pack_mtimes(p)) {
				uint32_t pos;
				إذا (جابه_جایی_مقصد_pack_pos(p, ofs, &pos) < 0)
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

ساكن صحيح want_یافت_object(ثابت بنية object_id *oid, صحيح exclude,
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
	إذا (ignore_packed_keep_on_disk || ignore_packed_keep_درون_core ||
	    ignore_packed_keep_درون_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		بیعلامت flags = 0;
		إذا (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		إذا (ignore_packed_keep_درون_core)
			flags |= KEPT_PACK_درون_CORE;
		إذا (ignore_packed_keep_درون_core_open)
			flags |= KEPT_PACK_درون_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		إذا (!ignore_packed_keep_درون_core_has_cruft) {
			إذا (ignore_packed_keep_on_disk && p->pack_keep)
				أرجع 0;
			إذا (ignore_packed_keep_درون_core && p->pack_keep_درون_core)
				أرجع 0;
			إذا (ignore_packed_keep_درون_core_open && p->pack_keep_درون_core_open)
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

ساكن صحيح want_object_درون_pack_one(بنية packed_git *p,
				   ثابت بنية object_id *oid,
				   صحيح exclude,
				   بنية packed_git **یافت_pack,
				   off_t *یافت_جابه_جایی,
				   uint32_t یافت_mtime)
{
	off_t جابه_جایی;

	إذا (p == *یافت_pack)
		جابه_جایی = *یافت_جابه_جایی;
	وإلا
		جابه_جایی = پیدا_کن_pack_مدخل_one(oid, p);

	إذا (جابه_جایی) {
		إذا (!*یافت_pack) {
			إذا (!is_pack_valid(p))
				أرجع -1;
			*یافت_جابه_جایی = جابه_جایی;
			*یافت_pack = p;
		}
		أرجع want_یافت_object(oid, exclude, p, یافت_mtime);
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
ساكن صحيح want_object_درون_pack_mtime(ثابت بنية object_id *oid,
				     صحيح exclude,
				     بنية packed_git **یافت_pack,
				     off_t *یافت_جابه_جایی,
				     uint32_t یافت_mtime)
{
	بنية odb_منبع_files *files =
		odb_منبع_files_downcast(the_repository->objects->منبع);
	صحيح want;
	بنية packfile_list_مدخل *e;

	إذا (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		لكل (بنية odb_files_dir *dir = files->dirs->بعدی; dir; dir = dir->بعدی)
			إذا (!odb_منبع_read_object_info(&dir->loose->پایه, oid, عدم, 0, عدم))
				أرجع 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	إذا (*یافت_pack) {
		want = want_یافت_object(oid, exclude, *یافت_pack,
					 یافت_mtime);
		إذا (want != -1)
			أرجع want;

		*یافت_pack = عدم;
		*یافت_جابه_جایی = 0;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی) {
		بنية multi_pack_شاخص *m = get_multi_pack_شاخص(dir->packed);
		بنية pack_مدخل e;

		إذا (m && midx_fill_مدخل(m, oid, &e, عدم) == MIDX_FILL_اصابت) {
			want = want_object_درون_pack_one(e.p, oid, exclude, یافت_pack, یافت_جابه_جایی, یافت_mtime);
			إذا (want != -1)
				أرجع want;
		}
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی) {
		لكل (e = dir->packed->packs.سر; e; e = e->بعدی) {
			بنية packed_git *p = e->pack;
			want = want_object_درون_pack_one(p, oid, exclude, یافت_pack, یافت_جابه_جایی, یافت_mtime);
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
				إذا (skip_پیشوند(ex->uri,
						uri_protocols.اقلام[i].string,
						&p) &&
				    *p == ':') {
					oidset_درج(&excluded_by_config, oid);
					أرجع 0;
				}
			}
		}
	}

	أرجع 1;
}

ساكن مضمن صحيح want_object_درون_pack(ثابت بنية object_id *oid,
				      صحيح exclude,
				      بنية packed_git **یافت_pack,
				      off_t *یافت_جابه_جایی)
{
	أرجع want_object_درون_pack_mtime(oid, exclude, یافت_pack, یافت_جابه_جایی,
					 0);
}

ساكن بنية object_مدخل *create_object_مدخل(ثابت بنية object_id *oid,
						تعداد object_type type,
						uint32_t hash,
						صحيح exclude,
						صحيح no_try_delta,
						بنية packed_git *یافت_pack,
						off_t یافت_جابه_جایی)
{
	بنية object_مدخل *مدخل;

	مدخل = packlist_alloc(&مقصد_pack, oid);
	مدخل->hash = hash;
	oe_مجموعه_type(مدخل, type);
	إذا (exclude)
		مدخل->preferred_پایه = 1;
	وإلا
		nr_نتیجه++;
	إذا (یافت_pack) {
		oe_مجموعه_درون_pack(&مقصد_pack, مدخل, یافت_pack);
		مدخل->درون_pack_جابه_جایی = یافت_جابه_جایی;
	}

	مدخل->no_try_delta = no_try_delta;

	أرجع مدخل;
}

ساكن ثابت حرف no_بستگی_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ساكن فراغ جمع_کن_object_مدخل(ثابت بنية object_id *oid, تعداد object_type type,
			     ثابت حرف *name, صحيح exclude)
{
	بنية packed_git *یافت_pack = عدم;
	off_t یافت_جابه_جایی = 0;

	display_پیشرفت(پیشرفت_state, ++nr_دیده);

	إذا (have_duplicate_مدخل(oid, exclude))
		أرجع;

	إذا (!want_object_درون_pack(oid, exclude, &یافت_pack, &یافت_جابه_جایی)) {
		/* The pack is missing an object, so it will not have closure */
		إذا (write_bitmap_شاخص) {
			إذا (write_bitmap_شاخص != WRITE_BITMAP_QUIET)
				warning(_(no_بستگی_warning));
			write_bitmap_شاخص = 0;
		}
		أرجع;
	}

	create_object_مدخل(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    یافت_pack, یافت_جابه_جایی);
}

ساكن صحيح جمع_کن_object_مدخل_from_bitmap(ثابت بنية object_id *oid,
					تعداد object_type type,
					صحيح flags استفادهنشده, uint32_t name_hash,
					بنية packed_git *pack, off_t جابه_جایی,
					فراغ *payload استفادهنشده)
{
	display_پیشرفت(پیشرفت_state, ++nr_دیده);

	إذا (have_duplicate_مدخل(oid, 0))
		أرجع 0;

	إذا (!want_object_درون_pack(oid, 0, &pack, &جابه_جایی))
		أرجع 0;

	create_object_مدخل(oid, type, name_hash, 0, 0, pack, جابه_جایی);
	أرجع 0;
}

بنية pbase_درخت_cache {
	بنية object_id oid;
	صحيح ref;
	صحيح temporary;
	فراغ *درخت_داده;
	بیعلامت دراز درخت_اندازه;
};

ساكن بنية pbase_درخت_cache *(pbase_درخت_cache[256]);
ساكن صحيح pbase_درخت_cache_ix(ثابت بنية object_id *oid)
{
	أرجع oid->hash[0] % ARRAY_اندازه(pbase_درخت_cache);
}
ساكن صحيح pbase_درخت_cache_ix_incr(صحيح ix)
{
	أرجع (ix+1) % ARRAY_اندازه(pbase_درخت_cache);
}

ساكن بنية pbase_درخت {
	بنية pbase_درخت *بعدی;
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
	فراغ *داده;
	بیعلامت دراز اندازه;
	اندازه_t اندازه_st = 0;
	تعداد object_type type;
	صحيح neigh;
	صحيح my_ix = pbase_درخت_cache_ix(oid);
	صحيح موجود_ix = -1;

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
		وإلا إذا (((موجود_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= موجود_ix) &&
			  (!ent && pbase_درخت_cache[موجود_ix])))
			موجود_ix = my_ix;
		إذا (!ent)
			اكسر;
		my_ix = pbase_درخت_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	داده = odb_read_object(the_repository->objects, oid, &type, &اندازه_st);
	اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
	إذا (!داده)
		أرجع عدم;
	إذا (type != OBJ_درخت) {
		حرر(داده);
		أرجع عدم;
	}

	/* We need to either cache or return a throwaway copy */

	إذا (موجود_ix < 0)
		ent = عدم;
	وإلا {
		ent = pbase_درخت_cache[موجود_ix];
		my_ix = موجود_ix;
	}

	إذا (!ent) {
		nent = xmalloc(حجم(*nent));
		nent->temporary = (موجود_ix < 0);
	}
	وإلا {
		/* evict and reuse */
		حرر(ent->درخت_داده);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->درخت_داده = داده;
	nent->درخت_اندازه = اندازه;
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
	حرر(cache->درخت_داده);
	حرر(cache);
}

ساكن اندازه_t name_cmp_len(ثابت حرف *name)
{
	أرجع strcspn(name, "\n/");
}

ساكن فراغ جمع_کن_pbase_object(بنية درخت_desc *درخت,
			     ثابت حرف *name,
			     اندازه_t cmplen,
			     ثابت حرف *fullname)
{
	بنية name_مدخل مدخل;
	صحيح cmp;

	طالما (درخت_مدخل(درخت,&مدخل)) {
		إذا (S_ISGITLINK(مدخل.نما))
			استمر;
		cmp = درخت_مدخل_len(&مدخل) != cmplen ? 1 :
		      memcmp(name, مدخل.مسیر, cmplen);
		إذا (cmp > 0)
			استمر;
		إذا (cmp < 0)
			أرجع;
		إذا (name[cmplen] != '/') {
			جمع_کن_object_مدخل(&مدخل.oid,
					 object_type(مدخل.نما),
					 fullname, 1);
			أرجع;
		}
		إذا (S_ISDIR(مدخل.نما)) {
			بنية درخت_desc sub;
			بنية pbase_درخت_cache *درخت;
			ثابت حرف *پایین = name+cmplen+1;
			اندازه_t downlen = name_cmp_len(پایین);

			درخت = pbase_درخت_get(&مدخل.oid);
			إذا (!درخت)
				أرجع;
			init_درخت_desc(&sub, &درخت->oid,
				       درخت->درخت_داده, درخت->درخت_اندازه);

			جمع_کن_pbase_object(&sub, پایین, downlen, fullname);
			pbase_درخت_put(درخت);
		}
	}
}

ساكن بیعلامت *انجامشده_pbase_paths;
ساكن صحيح انجامشده_pbase_paths_رقم;
ساكن صحيح انجامشده_pbase_paths_alloc;
ساكن صحيح انجامشده_pbase_مسیر_pos(بیعلامت hash)
{
	صحيح lo = 0;
	صحيح hi = انجامشده_pbase_paths_رقم;
	طالما (lo < hi) {
		صحيح mi = lo + (hi - lo) / 2;
		إذا (انجامشده_pbase_paths[mi] == hash)
			أرجع mi;
		إذا (انجامشده_pbase_paths[mi] < hash)
			hi = mi;
		وإلا
			lo = mi + 1;
	}
	أرجع -lo-1;
}

ساكن صحيح check_pbase_مسیر(بیعلامت hash)
{
	صحيح pos = انجامشده_pbase_مسیر_pos(hash);
	إذا (0 <= pos)
		أرجع 1;
	pos = -pos - 1;
	ALLOC_GROW(انجامشده_pbase_paths,
		   انجامشده_pbase_paths_رقم + 1,
		   انجامشده_pbase_paths_alloc);
	انجامشده_pbase_paths_رقم++;
	إذا (pos < انجامشده_pbase_paths_رقم)
		MOVE_ARRAY(انجامشده_pbase_paths + pos + 1, انجامشده_pbase_paths + pos,
			   انجامشده_pbase_paths_رقم - pos - 1);
	انجامشده_pbase_paths[pos] = hash;
	أرجع 0;
}

ساكن فراغ جمع_کن_preferred_پایه_object(ثابت حرف *name)
{
	بنية pbase_درخت *it;
	اندازه_t cmplen;
	بیعلامت hash = pack_name_hash_fn(name);

	إذا (!رقم_preferred_پایه || check_pbase_مسیر(hash))
		أرجع;

	cmplen = name_cmp_len(name);
	لكل (it = pbase_درخت; it; it = it->بعدی) {
		إذا (cmplen == 0) {
			جمع_کن_object_مدخل(&it->pcache.oid, OBJ_درخت, عدم, 1);
		}
		وإلا {
			بنية درخت_desc درخت;
			init_درخت_desc(&درخت, &it->pcache.oid,
				       it->pcache.درخت_داده, it->pcache.درخت_اندازه);
			جمع_کن_pbase_object(&درخت, name, cmplen, name);
		}
	}
}

ساكن فراغ جمع_کن_preferred_پایه(بنية object_id *oid)
{
	بنية pbase_درخت *it;
	فراغ *داده;
	بیعلامت دراز اندازه;
	اندازه_t اندازه_st = 0;
	بنية object_id درخت_oid;

	إذا (پنجره <= رقم_preferred_پایه++)
		أرجع;

	داده = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_درخت, &اندازه_st, &درخت_oid);
	اندازه = cast_اندازه_t_مقصد_ulong(اندازه_st);
	إذا (!داده)
		أرجع;

	لكل (it = pbase_درخت; it; it = it->بعدی) {
		إذا (oideq(&it->pcache.oid, &درخت_oid)) {
			حرر(داده);
			أرجع;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->بعدی = pbase_درخت;
	pbase_درخت = it;

	oidcpy(&it->pcache.oid, &درخت_oid);
	it->pcache.درخت_داده = داده;
	it->pcache.درخت_اندازه = اندازه;
}

ساكن فراغ cleanup_preferred_پایه(فراغ)
{
	بنية pbase_درخت *it;
	بیعلامت i;

	it = pbase_درخت;
	pbase_درخت = عدم;
	طالما (it) {
		بنية pbase_درخت *tmp = it;
		it = tmp->بعدی;
		حرر(tmp->pcache.درخت_داده);
		حرر(tmp);
	}

	لكل (i = 0; i < ARRAY_اندازه(pbase_درخت_cache); i++) {
		إذا (!pbase_درخت_cache[i])
			استمر;
		حرر(pbase_درخت_cache[i]->درخت_داده);
		FREE_AND_NULL(pbase_درخت_cache[i]);
	}

	FREE_AND_NULL(انجامشده_pbase_paths);
	انجامشده_pbase_paths_رقم = انجامشده_pbase_paths_alloc = 0;
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
ساكن صحيح can_reuse_delta(ثابت بنية object_id *پایه_oid,
			   بنية object_مدخل *delta,
			   بنية object_مدخل **پایه_بیرون)
{
	بنية object_مدخل *پایه;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	پایه = packlist_پیدا_کن(&مقصد_pack, پایه_oid);
	إذا (پایه) {
		إذا (!درون_same_island(&delta->idx.oid, &پایه->idx.oid))
			أرجع 0;
		*پایه_بیرون = پایه;
		أرجع 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	إذا (thin && bitmap_has_oid_درون_uninteresting(bitmap_git, پایه_oid)) {
		إذا (use_delta_islands) {
			إذا (!درون_same_island(&delta->idx.oid, پایه_oid))
				أرجع 0;
		}
		*پایه_بیرون = عدم;
		أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ prefetch_مقصد_pack(uint32_t object_شاخص_شروع) {
	بنية oid_array مقصد_fetch = OID_ARRAY_INIT;
	uint32_t i;

	لكل (i = object_شاخص_شروع; i < مقصد_pack.nr_objects; i++) {
		بنية object_مدخل *مدخل = مقصد_pack.objects + i;

		إذا (!odb_read_object_info_extended(the_repository->objects,
						   &مدخل->idx.oid,
						   عدم,
						   OBJECT_INFO_FOR_PREFETCH))
			استمر;
		oid_array_append(&مقصد_fetch, &مدخل->idx.oid);
	}
	promisor_remote_get_مستقیم(the_repository,
				   مقصد_fetch.oid, مقصد_fetch.nr);
	oid_array_clear(&مقصد_fetch);
}

ساكن فراغ check_object(بنية object_مدخل *مدخل, uint32_t object_شاخص)
{
	اندازه_t canonical_اندازه;
	تعداد object_type type;
	بنية object_info oi = {.typep = &type, .sizep = &canonical_اندازه};

	إذا (درون_PACK(مدخل)) {
		بنية packed_git *p = درون_PACK(مدخل);
		بنية pack_پنجره *w_curs = عدم;
		صحيح have_پایه = 0;
		بنية object_id پایه_ref;
		بنية object_مدخل *پایه_مدخل;
		اندازه_t استفاده_شده_2, استفاده_شده_0, avail;
		off_t ofs;
		بیعلامت حرف *buf, c;
		تعداد object_type type;
		اندازه_t درون_pack_اندازه;

		buf = use_pack(p, &w_curs, مدخل->درون_pack_جابه_جایی, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		استفاده_شده_2 = unpack_object_سرآیند_میانگیر(buf, avail,
						   &type,
						   &درون_pack_اندازه);
		إذا (استفاده_شده_2 == 0)
			اذهب give_به_بالا;

		إذا (type < 0)
			BUG("invalid type %d", type);
		مدخل->درون_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		بدل (مدخل->درون_pack_type) {
		افتراضي:
			/* Not a delta hence we've already got all we need. */
			oe_مجموعه_type(مدخل, مدخل->درون_pack_type);
			مجموعه_اندازه(مدخل, درون_pack_اندازه);
			مدخل->درون_pack_سرآیند_اندازه = استفاده_شده_2;
			إذا (oe_type(مدخل) < OBJ_COMMIT || oe_type(مدخل) > OBJ_BLOB)
				اذهب give_به_بالا;
			unuse_pack(&w_curs);
			أرجع;
		حالة OBJ_REF_DELTA:
			إذا (reuse_delta && !مدخل->preferred_پایه) {
				oidread(&پایه_ref,
					use_pack(p, &w_curs,
						 مدخل->درون_pack_جابه_جایی + استفاده_شده_2,
						 عدم),
					the_repository->hash_algo);
				have_پایه = 1;
			}
			مدخل->درون_pack_سرآیند_اندازه = استفاده_شده_2 + the_hash_algo->rawsz;
			اكسر;
		حالة OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       مدخل->درون_pack_جابه_جایی + استفاده_شده_2, عدم);
			استفاده_شده_0 = 0;
			c = buf[استفاده_شده_0++];
			ofs = c & 127;
			طالما (c & 128) {
				ofs += 1;
				إذا (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_مقصد_hex(&مدخل->idx.oid));
					اذهب give_به_بالا;
				}
				c = buf[استفاده_شده_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = مدخل->درون_pack_جابه_جایی - ofs;
			إذا (ofs <= 0 || ofs >= مدخل->درون_pack_جابه_جایی) {
				error(_("delta base offset out of bound for %s"),
				      oid_مقصد_hex(&مدخل->idx.oid));
				اذهب give_به_بالا;
			}
			إذا (reuse_delta && !مدخل->preferred_پایه) {
				uint32_t pos;
				إذا (جابه_جایی_مقصد_pack_pos(p, ofs, &pos) < 0)
					اذهب give_به_بالا;
				إذا (!nth_packed_object_id(&پایه_ref, p,
							  pack_pos_مقصد_شاخص(p, pos)))
					have_پایه = 1;
			}
			مدخل->درون_pack_سرآیند_اندازه = استفاده_شده_2 + استفاده_شده_0;
			اكسر;
		}

		إذا (have_پایه &&
		    can_reuse_delta(&پایه_ref, مدخل, &پایه_مدخل)) {
			oe_مجموعه_type(مدخل, مدخل->درون_pack_type);
			مجموعه_اندازه(مدخل, درون_pack_اندازه); /* delta size */
			مجموعه_DELTA_اندازه(مدخل, درون_pack_اندازه);

			إذا (پایه_مدخل) {
				مجموعه_DELTA(مدخل, پایه_مدخل);
				مدخل->delta_sibling_idx = پایه_مدخل->delta_فرزند_idx;
				مجموعه_DELTA_فرزند(پایه_مدخل, مدخل);
			} وإلا {
				مجموعه_DELTA_EXT(مدخل, &پایه_ref);
			}

			unuse_pack(&w_curs);
			أرجع;
		}

		إذا (oe_type(مدخل)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = مدخل->درون_pack_جابه_جایی + مدخل->درون_pack_سرآیند_اندازه;
			canonical_اندازه = get_اندازه_from_delta(p, &w_curs,
							     delta_pos);
			إذا (canonical_اندازه == 0)
				اذهب give_به_بالا;
			مجموعه_اندازه(مدخل, canonical_اندازه);
			unuse_pack(&w_curs);
			أرجع;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_به_بالا:
		unuse_pack(&w_curs);
	}

	إذا (odb_read_object_info_extended(the_repository->objects, &مدخل->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		إذا (repo_has_promisor_remote(the_repository)) {
			prefetch_مقصد_pack(object_شاخص);
			إذا (odb_read_object_info_extended(the_repository->objects, &مدخل->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} وإلا {
			type = -1;
		}
	}
	oe_مجموعه_type(مدخل, type);
	إذا (مدخل->type_valid) {
		مجموعه_اندازه(مدخل, canonical_اندازه);
	} وإلا {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ساكن صحيح pack_جابه_جایی_مرتب_سازی(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_مدخل *a = *(بنية object_مدخل **)_a;
	ثابت بنية object_مدخل *b = *(بنية object_مدخل **)_b;
	ثابت بنية packed_git *a_درون_pack = درون_PACK(a);
	ثابت بنية packed_git *b_درون_pack = درون_PACK(b);

	/* avoid filesystem trashing with loose objects */
	إذا (!a_درون_pack && !b_درون_pack)
		أرجع oidcmp(&a->idx.oid, &b->idx.oid);

	إذا (a_درون_pack < b_درون_pack)
		أرجع -1;
	إذا (a_درون_pack > b_درون_pack)
		أرجع 1;
	أرجع a->درون_pack_جابه_جایی < b->درون_pack_جابه_جایی ? -1 :
			(a->درون_pack_جابه_جایی > b->درون_pack_جابه_جایی);
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
ساكن فراغ drop_reused_delta(بنية object_مدخل *مدخل)
{
	بیعلامت *idx = &مقصد_pack.objects[مدخل->delta_idx - 1].delta_فرزند_idx;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type;
	اندازه_t اندازه;

	طالما (*idx) {
		بنية object_مدخل *oe = &مقصد_pack.objects[*idx - 1];

		إذا (oe == مدخل)
			*idx = oe->delta_sibling_idx;
		وإلا
			idx = &oe->delta_sibling_idx;
	}
	مجموعه_DELTA(مدخل, عدم);
	مدخل->عمق = 0;

	oi.sizep = &اندازه;
	oi.typep = &type;
	إذا (packed_object_info(عدم, درون_PACK(مدخل), مدخل->درون_pack_جابه_جایی, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_مجموعه_type(مدخل,
			    odb_read_object_info(the_repository->objects,
						 &مدخل->idx.oid, &اندازه));
	} وإلا {
		oe_مجموعه_type(مدخل, type);
	}
	مجموعه_اندازه(مدخل, اندازه);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
ساكن فراغ break_delta_chains(بنية object_مدخل *مدخل)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t مجموع_عمق;
	بنية object_مدخل *cur, *بعدی;

	لكل (cur = مدخل, مجموع_عمق = 0;
	     cur;
	     cur = DELTA(cur), مجموع_عمق++) {
		إذا (cur->dfs_state == DFS_انجامشده) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			مجموع_عمق += cur->عمق;
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
			cur->dfs_state = DFS_انجامشده;
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
		cur->dfs_state = DFS_فعال;
		إذا (DELTA(cur)->dfs_state == DFS_فعال) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_انجامشده;
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
	لكل (cur = مدخل; cur; cur = بعدی) {
		بعدی = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		إذا (cur->dfs_state == DFS_انجامشده)
			اكسر;
		وإلا إذا (cur->dfs_state != DFS_فعال)
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
		cur->عمق = (مجموع_عمق--) % (عمق + 1);
		إذا (!cur->عمق)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_انجامشده;
	}
}

ساكن فراغ get_object_details(فراغ)
{
	uint32_t i;
	بنية object_مدخل **sorted_by_جابه_جایی;

	إذا (پیشرفت)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Counting objects"),
						مقصد_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_جابه_جایی, مقصد_pack.nr_objects);
	لكل (i = 0; i < مقصد_pack.nr_objects; i++)
		sorted_by_جابه_جایی[i] = مقصد_pack.objects + i;
	QSORT(sorted_by_جابه_جایی, مقصد_pack.nr_objects, pack_جابه_جایی_مرتب_سازی);

	لكل (i = 0; i < مقصد_pack.nr_objects; i++) {
		بنية object_مدخل *مدخل = sorted_by_جابه_جایی[i];
		check_object(مدخل, i);
		إذا (مدخل->type_valid &&
		    oe_اندازه_greater_than(&مقصد_pack, مدخل,
					 repo_settings_get_big_file_آستانه(the_repository)))
			مدخل->no_try_delta = 1;
		display_پیشرفت(پیشرفت_state, i + 1);
	}
	stop_پیشرفت(&پیشرفت_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	لكل (i = 0; i < مقصد_pack.nr_objects; i++)
		break_delta_chains(&مقصد_pack.objects[i]);

	حرر(sorted_by_جابه_جایی);
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
ساكن صحيح type_اندازه_مرتب_سازی(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_مدخل *a = *(بنية object_مدخل **)_a;
	ثابت بنية object_مدخل *b = *(بنية object_مدخل **)_b;
	ثابت تعداد object_type a_type = oe_type(a);
	ثابت تعداد object_type b_type = oe_type(b);
	ثابت بیعلامت دراز a_اندازه = اندازه_3(a);
	ثابت بیعلامت دراز b_اندازه = اندازه_3(b);

	إذا (a_type > b_type)
		أرجع -1;
	إذا (a_type < b_type)
		أرجع 1;
	إذا (a->hash > b->hash)
		أرجع -1;
	إذا (a->hash < b->hash)
		أرجع 1;
	إذا (a->preferred_پایه > b->preferred_پایه)
		أرجع -1;
	إذا (a->preferred_پایه < b->preferred_پایه)
		أرجع 1;
	إذا (use_delta_islands) {
		ثابت صحيح island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		إذا (island_cmp)
			أرجع island_cmp;
	}
	إذا (a_اندازه > b_اندازه)
		أرجع -1;
	إذا (a_اندازه < b_اندازه)
		أرجع 1;
	أرجع a < b ? -1 : (a > b);  /* newest first */
}

بنية unpacked {
	بنية object_مدخل *مدخل;
	فراغ *داده;
	بنية delta_شاخص *شاخص;
	بیعلامت عمق;
};

ساكن صحيح delta_cacheable(اندازه_t src_اندازه, اندازه_t trg_اندازه,
			   اندازه_t delta_اندازه_2)
{
	إذا (max_delta_cache_اندازه && delta_cache_اندازه + delta_اندازه_2 > max_delta_cache_اندازه)
		أرجع 0;

	إذا (delta_اندازه_2 < cache_max_small_delta_اندازه)
		أرجع 1;

	/* cache delta, if objects are large enough compared to delta size */
	إذا ((src_اندازه >> 20) + (trg_اندازه >> 21) > (delta_اندازه_2 >> 10))
		أرجع 1;

	أرجع 0;
}

/* Protect delta_cache_size */
ساكن pthread_mutex_t cache_mutex;
#تعریف_کن cache_قفل()		pthread_mutex_قفل(&cache_mutex)
#تعریف_کن cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ساكن pthread_mutex_t پیشرفت_mutex;
#تعریف_کن پیشرفت_قفل()		pthread_mutex_قفل(&پیشرفت_mutex)
#تعریف_کن پیشرفت_unlock()	pthread_mutex_unlock(&پیشرفت_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ساكن مضمن صحيح oe_اندازه_less_than(بنية packing_داده *pack,
				    ثابت بنية object_مدخل *lhs,
				    اندازه_t rhs)
{
	إذا (lhs->اندازه_valid)
		أرجع lhs->اندازه_2 < rhs;
	إذا (rhs < pack->oe_اندازه_حد) /* rhs < 2^x <= lhs ? */
		أرجع 0;
	أرجع oe_get_اندازه_slow(pack, lhs) < rhs;
}

ساكن مضمن فراغ oe_مجموعه_درخت_عمق(بنية packing_داده *pack,
				     بنية object_مدخل *e,
				     بیعلامت صحيح درخت_عمق)
{
	إذا (!pack->درخت_عمق)
		CALLOC_ARRAY(pack->درخت_عمق, pack->nr_alloc);
	pack->درخت_عمق[e - pack->objects] = درخت_عمق;
}

ساكن فراغ رکورد_درخت_عمق(ثابت بنية object_id *oid, ثابت حرف *name)
{
	ثابت حرف *p;
	بیعلامت عمق;
	بنية object_مدخل *ent;

	/* the empty string is a root tree, which is depth 0 */
	عمق = *name ? 1 : 0;
	لكل (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		عمق++;

	ent = packlist_پیدا_کن(&مقصد_pack, oid);
	إذا (ent && عمق > oe_درخت_عمق(&مقصد_pack, ent))
		oe_مجموعه_درخت_عمق(&مقصد_pack, ent, عمق);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
اندازه_t oe_get_اندازه_slow(بنية packing_داده *pack,
			ثابت بنية object_مدخل *e)
{
	بنية packed_git *p;
	بنية pack_پنجره *w_curs;
	بیعلامت حرف *buf;
	تعداد object_type type;
	اندازه_t استفاده_شده_2, avail, اندازه;

	إذا (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		اندازه_t sz;
		packing_داده_قفل(&مقصد_pack);
		إذا (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_مقصد_hex(&e->idx.oid));
		packing_داده_unlock(&مقصد_pack);
		أرجع sz;
	}

	p = oe_درون_pack(pack, e);
	إذا (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_داده_قفل(&مقصد_pack);
	w_curs = عدم;
	buf = use_pack(p, &w_curs, e->درون_pack_جابه_جایی, &avail);
	استفاده_شده_2 = unpack_object_سرآیند_میانگیر(buf, avail, &type, &اندازه);
	إذا (استفاده_شده_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_مقصد_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_داده_unlock(&مقصد_pack);
	أرجع اندازه;
}

ساكن صحيح try_delta(بنية unpacked *trg, بنية unpacked *src,
		     بیعلامت max_عمق, اندازه_t *mem_usage)
{
	بنية object_مدخل *trg_مدخل = trg->مدخل;
	بنية object_مدخل *src_مدخل = src->مدخل;
	بیعلامت دراز trg_اندازه, src_اندازه, sizediff, max_اندازه, sz;
	اندازه_t delta_اندازه_2;
	بیعلامت ref_عمق;
	تعداد object_type type;
	فراغ *delta_buf;

	/* Don't bother doing diffs between different types */
	إذا (oe_type(trg_مدخل) != oe_type(src_مدخل))
		أرجع -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	إذا (reuse_delta && درون_PACK(trg_مدخل) &&
	    درون_PACK(trg_مدخل) == درون_PACK(src_مدخل) &&
	    !src_مدخل->preferred_پایه &&
	    trg_مدخل->درون_pack_type != OBJ_REF_DELTA &&
	    trg_مدخل->درون_pack_type != OBJ_OFS_DELTA)
		أرجع 0;

	/* Let's not bust the allowed depth. */
	إذا (src->عمق >= max_عمق)
		أرجع 0;

	/* Now some size filtering heuristics. */
	trg_اندازه = اندازه_3(trg_مدخل);
	إذا (!DELTA(trg_مدخل)) {
		max_اندازه = trg_اندازه/2 - the_hash_algo->rawsz;
		ref_عمق = 1;
	} وإلا {
		max_اندازه = DELTA_اندازه(trg_مدخل);
		ref_عمق = trg->عمق;
	}
	max_اندازه = (uint64_t)max_اندازه * (max_عمق - src->عمق) /
						(max_عمق - ref_عمق + 1);
	إذا (max_اندازه == 0)
		أرجع 0;
	src_اندازه = اندازه_3(src_مدخل);
	sizediff = src_اندازه < trg_اندازه ? trg_اندازه - src_اندازه : 0;
	إذا (sizediff >= max_اندازه)
		أرجع 0;
	إذا (trg_اندازه < src_اندازه / 32)
		أرجع 0;

	إذا (!درون_same_island(&trg->مدخل->idx.oid, &src->مدخل->idx.oid))
		أرجع 0;

	/* Load data if not already done */
	إذا (!trg->داده) {
		اندازه_t sz_st = 0;
		packing_داده_قفل(&مقصد_pack);
		trg->داده = odb_read_object(the_repository->objects,
					    &trg_مدخل->idx.oid, &type,
					    &sz_st);
		sz = cast_اندازه_t_مقصد_ulong(sz_st);
		packing_داده_unlock(&مقصد_pack);
		إذا (!trg->داده)
			die(_("object %s cannot be read"),
			    oid_مقصد_hex(&trg_مدخل->idx.oid));
		إذا (sz != trg_اندازه)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_مقصد_hex(&trg_مدخل->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_اندازه);
		*mem_usage += sz;
	}
	إذا (!src->داده) {
		اندازه_t sz_st = 0;
		packing_داده_قفل(&مقصد_pack);
		src->داده = odb_read_object(the_repository->objects,
					    &src_مدخل->idx.oid, &type,
					    &sz_st);
		sz = cast_اندازه_t_مقصد_ulong(sz_st);
		packing_داده_unlock(&مقصد_pack);
		إذا (!src->داده) {
			إذا (src_مدخل->preferred_پایه) {
				ساكن صحيح warned = 0;
				إذا (!warned++)
					warning(_("object %s cannot be read"),
						oid_مقصد_hex(&src_مدخل->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				أرجع 0;
			}
			die(_("object %s cannot be read"),
			    oid_مقصد_hex(&src_مدخل->idx.oid));
		}
		إذا (sz != src_اندازه)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_مقصد_hex(&src_مدخل->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_اندازه);
		*mem_usage += sz;
	}
	إذا (!src->شاخص) {
		src->شاخص = create_delta_شاخص(src->داده, src_اندازه);
		إذا (!src->شاخص) {
			ساكن صحيح warned = 0;
			إذا (!warned++)
				warning(_("suboptimal pack - out of memory"));
			أرجع 0;
		}
		*mem_usage += sizeof_delta_شاخص(src->شاخص);
	}

	delta_buf = create_delta(src->شاخص, trg->داده, trg_اندازه, &delta_اندازه_2, max_اندازه);
	إذا (!delta_buf)
		أرجع 0;

	إذا (DELTA(trg_مدخل)) {
		/* Prefer only shallower same-sized deltas. */
		إذا (delta_اندازه_2 == DELTA_اندازه(trg_مدخل) &&
		    src->عمق + 1 >= trg->عمق) {
			حرر(delta_buf);
			أرجع 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	حرر(trg_مدخل->delta_داده);
	cache_قفل();
	إذا (trg_مدخل->delta_داده) {
		delta_cache_اندازه -= DELTA_اندازه(trg_مدخل);
		trg_مدخل->delta_داده = عدم;
	}
	إذا (delta_cacheable(src_اندازه, trg_اندازه, delta_اندازه_2)) {
		delta_cache_اندازه += delta_اندازه_2;
		cache_unlock();
		trg_مدخل->delta_داده = xrealloc(delta_buf, delta_اندازه_2);
	} وإلا {
		cache_unlock();
		حرر(delta_buf);
	}

	مجموعه_DELTA(trg_مدخل, src_مدخل);
	مجموعه_DELTA_اندازه(trg_مدخل, delta_اندازه_2);
	trg->عمق = src->عمق + 1;

	أرجع 1;
}

ساكن بیعلامت صحيح check_delta_حد(بنية object_مدخل *me, بیعلامت صحيح n)
{
	بنية object_مدخل *فرزند = DELTA_فرزند(me);
	بیعلامت صحيح m = n;
	طالما (فرزند) {
		ثابت بیعلامت صحيح c = check_delta_حد(فرزند, n + 1);
		إذا (m < c)
			m = c;
		فرزند = DELTA_SIBLING(فرزند);
	}
	أرجع m;
}

ساكن اندازه_t free_unpacked(بنية unpacked *n)
{
	اندازه_t freed_mem = sizeof_delta_شاخص(n->شاخص);
	free_delta_شاخص(n->شاخص);
	n->شاخص = عدم;
	إذا (n->داده) {
		freed_mem += اندازه_3(n->مدخل);
		FREE_AND_NULL(n->داده);
	}
	n->مدخل = عدم;
	n->عمق = 0;
	أرجع freed_mem;
}

ساكن فراغ پیدا_کن_deltas(بنية object_مدخل **list, بیعلامت *list_اندازه,
			صحيح پنجره, صحيح عمق, بیعلامت *processed)
{
	uint32_t i, idx = 0, شمار = 0;
	بنية unpacked *array;
	اندازه_t mem_usage = 0;

	CALLOC_ARRAY(array, پنجره);

	لكل (;;) {
		بنية object_مدخل *مدخل;
		بنية unpacked *n = array + idx;
		صحيح j, max_عمق, بهترین_پایه = -1;

		پیشرفت_قفل();
		إذا (!*list_اندازه) {
			پیشرفت_unlock();
			اكسر;
		}
		مدخل = *list++;
		(*list_اندازه)--;
		إذا (!مدخل->preferred_پایه) {
			(*processed)++;
			display_پیشرفت(پیشرفت_state, *processed);
		}
		پیشرفت_unlock();

		mem_usage -= free_unpacked(n);
		n->مدخل = مدخل;

		طالما (پنجره_حافظه_حد &&
		       mem_usage > پنجره_حافظه_حد &&
		       شمار > 1) {
			ثابت uint32_t دم = (idx + پنجره - شمار) % پنجره;
			mem_usage -= free_unpacked(array + دم);
			شمار--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		إذا (مدخل->preferred_پایه)
			اذهب بعدی;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_عمق = عمق;
		إذا (DELTA_فرزند(مدخل)) {
			max_عمق -= check_delta_حد(مدخل, 0);
			إذا (max_عمق <= 0)
				اذهب بعدی;
		}

		j = پنجره;
		طالما (--j > 0) {
			صحيح ret;
			uint32_t other_idx = idx + j;
			بنية unpacked *m;
			إذا (other_idx >= پنجره)
				other_idx -= پنجره;
			m = array + other_idx;
			إذا (!m->مدخل)
				اكسر;
			ret = try_delta(n, m, max_عمق, &mem_usage);
			إذا (ret < 0)
				اكسر;
			وإلا إذا (ret > 0)
				بهترین_پایه = other_idx;
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
		إذا (مدخل->delta_داده && !pack_مقصد_stdout) {
			بیعلامت دراز اندازه;

			اندازه = do_compress(&مدخل->delta_داده, DELTA_اندازه(مدخل));
			إذا (اندازه < (1U << OE_Z_DELTA_بیتها)) {
				مدخل->z_delta_اندازه = اندازه;
				cache_قفل();
				delta_cache_اندازه -= DELTA_اندازه(مدخل);
				delta_cache_اندازه += مدخل->z_delta_اندازه;
				cache_unlock();
			} وإلا {
				FREE_AND_NULL(مدخل->delta_داده);
				مدخل->z_delta_اندازه = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		إذا (DELTA(مدخل) && max_عمق <= n->عمق)
			استمر;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		إذا (DELTA(مدخل)) {
			بنية unpacked جابجا_2 = array[بهترین_پایه];
			صحيح dist = (پنجره + idx - بهترین_پایه) % پنجره;
			صحيح dst = بهترین_پایه;
			طالما (dist--) {
				صحيح src = (dst + 1) % پنجره;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = جابجا_2;
		}

		بعدی:
		idx++;
		إذا (شمار + 1 < پنجره)
			شمار++;
		إذا (idx >= پنجره)
			idx = 0;
	}

	لكل (i = 0; i < پنجره; ++i) {
		free_delta_شاخص(array[i].شاخص);
		حرر(array[i].داده);
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
	بنية object_مدخل **list;
	بنية packing_region *regions;
	بیعلامت list_اندازه;
	بیعلامت مانده;
	صحيح پنجره;
	صحيح عمق;
	صحيح working;
	صحيح داده_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	بیعلامت *processed;
};

ساكن pthread_cond_t پیشرفت_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساكن فراغ init_threaded_جستجو(فراغ)
{
	pthread_mutex_init(&cache_mutex, عدم);
	pthread_mutex_init(&پیشرفت_mutex, عدم);
	pthread_cond_init(&پیشرفت_cond, عدم);
}

ساكن فراغ cleanup_threaded_جستجو(فراغ)
{
	pthread_cond_destroy(&پیشرفت_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&پیشرفت_mutex);
}

ساكن فراغ *threaded_پیدا_کن_deltas(فراغ *arg)
{
	بنية thread_params *me = arg;

	پیشرفت_قفل();
	طالما (me->مانده) {
		پیشرفت_unlock();

		پیدا_کن_deltas(me->list, &me->مانده,
			    me->پنجره, me->عمق, me->processed);

		پیشرفت_قفل();
		me->working = 0;
		pthread_cond_signal(&پیشرفت_cond);
		پیشرفت_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_قفل(&me->mutex);
		طالما (!me->داده_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->داده_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		پیشرفت_قفل();
	}
	پیشرفت_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_پیدا_کن_deltas(بنية object_مدخل **list, بیعلامت list_اندازه,
			   صحيح پنجره, صحيح عمق, بیعلامت *processed)
{
	بنية thread_params *p;
	صحيح i, ret, فعال_نخها = 0;

	init_threaded_جستجو();

	إذا (delta_جستجو_نخها <= 1) {
		پیدا_کن_deltas(list, &list_اندازه, پنجره, عمق, processed);
		cleanup_threaded_جستجو();
		أرجع;
	}
	إذا (پیشرفت > pack_مقصد_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_جستجو_نخها);
	CALLOC_ARRAY(p, delta_جستجو_نخها);

	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_جستجو_نخها; i++) {
		بیعلامت sub_اندازه = list_اندازه / (delta_جستجو_نخها - i);

		/* don't use too small segments or no deltas will be found */
		إذا (sub_اندازه < 2*پنجره && i+1 < delta_جستجو_نخها)
			sub_اندازه = 0;

		p[i].پنجره = پنجره;
		p[i].عمق = عمق;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].داده_ready = 0;

		/* try to split chunks on "path" boundaries */
		طالما (sub_اندازه && sub_اندازه < list_اندازه &&
		       list[sub_اندازه]->hash &&
		       list[sub_اندازه]->hash == list[sub_اندازه-1]->hash)
			sub_اندازه++;

		p[i].list = list;
		p[i].list_اندازه = sub_اندازه;
		p[i].مانده = sub_اندازه;

		list += sub_اندازه;
		list_اندازه -= sub_اندازه;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_جستجو_نخها; i++) {
		إذا (!p[i].list_اندازه)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_پیدا_کن_deltas, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		فعال_نخها++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (فعال_نخها) {
		بنية thread_params *هدف = عدم;
		بنية thread_params *victim = عدم;
		بیعلامت sub_اندازه = 0;

		پیشرفت_قفل();
		لكل (;;) {
			لكل (i = 0; !هدف && i < delta_جستجو_نخها; i++)
				إذا (!p[i].working)
					هدف = &p[i];
			إذا (هدف)
				اكسر;
			pthread_cond_wait(&پیشرفت_cond, &پیشرفت_mutex);
		}

		لكل (i = 0; i < delta_جستجو_نخها; i++)
			إذا (p[i].مانده > 2*پنجره &&
			    (!victim || victim->مانده < p[i].مانده))
				victim = &p[i];
		إذا (victim) {
			sub_اندازه = victim->مانده / 2;
			list = victim->list + victim->list_اندازه - sub_اندازه;
			طالما (sub_اندازه && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_اندازه--;
			}
			إذا (!sub_اندازه) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_اندازه = victim->مانده / 2;
				list -= sub_اندازه;
			}
			هدف->list = list;
			victim->list_اندازه -= sub_اندازه;
			victim->مانده -= sub_اندازه;
		}
		هدف->list_اندازه = sub_اندازه;
		هدف->مانده = sub_اندازه;
		هدف->working = 1;
		پیشرفت_unlock();

		pthread_mutex_قفل(&هدف->mutex);
		هدف->داده_ready = 1;
		pthread_cond_signal(&هدف->cond);
		pthread_mutex_unlock(&هدف->mutex);

		إذا (!sub_اندازه) {
			pthread_join(هدف->thread, عدم);
			pthread_cond_destroy(&هدف->cond);
			pthread_mutex_destroy(&هدف->mutex);
			فعال_نخها--;
		}
	}
	cleanup_threaded_جستجو();
	حرر(p);
}

ساكن صحيح obj_is_packed(ثابت بنية object_id *oid)
{
	أرجع packlist_پیدا_کن(&مقصد_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_شامل_است(bitmap_git, reuse_packfile_bitmap, oid));
}

ساكن فراغ جمع_کن_برچسب_chain(ثابت بنية object_id *oid)
{
	بنية برچسب *برچسب;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	إذا (obj_is_packed(oid))
		أرجع;

	برچسب = lookup_برچسب(the_repository, oid);
	طالما (1) {
		إذا (!برچسب || parse_برچسب(the_repository, برچسب) || !برچسب->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_مقصد_hex(oid));

		جمع_کن_object_مدخل(&برچسب->object.oid, OBJ_برچسب, عدم, 0);

		إذا (برچسب->tagged->type != OBJ_برچسب)
			أرجع;

		برچسب = (بنية برچسب *)برچسب->tagged;
	}
}

ساكن صحيح جمع_کن_ref_برچسب(ثابت بنية reference *ref, فراغ *cb_داده استفادهنشده)
{
	بنية object_id peeled;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		جمع_کن_برچسب_chain(ref->oid);
	أرجع 0;
}

ساكن صحيح should_attempt_deltas(بنية object_مدخل *مدخل)
{
	إذا (DELTA(مدخل))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		أرجع 0;

	إذا (!مدخل->type_valid ||
	    oe_اندازه_less_than(&مقصد_pack, مدخل, 50))
		أرجع 0;

	إذا (مدخل->no_try_delta)
		أرجع 0;

	إذا (!مدخل->preferred_پایه) {
		إذا (oe_type(مدخل) < 0)
			die(_("unable to get type of object %s"),
				oid_مقصد_hex(&مدخل->idx.oid));
	} وإلا إذا (oe_type(مدخل) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		أرجع 0;
	}

	أرجع 1;
}

ساكن فراغ پیدا_کن_deltas_for_region(بنية object_مدخل *list,
				   بنية packing_region *region,
				   بیعلامت صحيح *processed)
{
	بنية object_مدخل **delta_list;
	بیعلامت صحيح delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	لكل (اندازه_t i = 0; i < region->nr; i++) {
		بنية object_مدخل *مدخل = list + region->شروع + i;
		إذا (should_attempt_deltas(مدخل))
			delta_list[delta_list_nr++] = مدخل;
	}

	QSORT(delta_list, delta_list_nr, type_اندازه_مرتب_سازی);
	پیدا_کن_deltas(delta_list, &delta_list_nr, پنجره, عمق, processed);
	حرر(delta_list);
}

ساكن فراغ پیدا_کن_deltas_by_region(بنية object_مدخل *list,
				  بنية packing_region *regions,
				  اندازه_t شروع, اندازه_t nr)
{
	بیعلامت صحيح processed = 0;
	اندازه_t پیشرفت_nr;

	إذا (!nr)
		أرجع;

	پیشرفت_nr = regions[nr - 1].شروع + regions[nr - 1].nr;

	إذا (پیشرفت)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Compressing objects by path"),
						پیشرفت_nr);

	طالما (nr--)
		پیدا_کن_deltas_for_region(list,
				       &regions[شروع++],
				       &processed);

	display_پیشرفت(پیشرفت_state, پیشرفت_nr);
	stop_پیشرفت(&پیشرفت_state);
}

ساكن فراغ *threaded_پیدا_کن_deltas_by_مسیر(فراغ *arg)
{
	بنية thread_params *me = arg;

	پیشرفت_قفل();
	طالما (me->مانده) {
		طالما (me->مانده) {
			پیشرفت_unlock();
			پیدا_کن_deltas_for_region(مقصد_pack.objects,
					       me->regions,
					       me->processed);
			پیشرفت_قفل();
			me->مانده--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&پیشرفت_cond);
		پیشرفت_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_قفل(&me->mutex);
		طالما (!me->داده_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->داده_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		پیشرفت_قفل();
	}
	پیشرفت_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_پیدا_کن_deltas_by_region(بنية object_مدخل *list,
				     بنية packing_region *regions,
				     uint32_t شروع, uint32_t nr)
{
	بنية thread_params *p;
	صحيح i, ret, فعال_نخها = 0;
	بیعلامت صحيح processed = 0;
	uint32_t پیشرفت_nr;
	init_threaded_جستجو();

	إذا (!nr)
		أرجع;

	پیشرفت_nr =  regions[nr - 1].شروع + regions[nr - 1].nr;
	إذا (delta_جستجو_نخها <= 1) {
		پیدا_کن_deltas_by_region(list, regions, شروع, nr);
		cleanup_threaded_جستجو();
		أرجع;
	}

	إذا (پیشرفت > pack_مقصد_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_جستجو_نخها),
			   delta_جستجو_نخها);
	CALLOC_ARRAY(p, delta_جستجو_نخها);

	إذا (پیشرفت)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Compressing objects by path"),
						پیشرفت_nr);
	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_جستجو_نخها; i++) {
		بیعلامت sub_اندازه = nr / (delta_جستجو_نخها - i);

		p[i].پنجره = پنجره;
		p[i].عمق = عمق;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].داده_ready = 0;

		p[i].regions = regions;
		p[i].list_اندازه = sub_اندازه;
		p[i].مانده = sub_اندازه;

		regions += sub_اندازه;
		nr -= sub_اندازه;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_جستجو_نخها; i++) {
		إذا (!p[i].list_اندازه)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_پیدا_کن_deltas_by_مسیر, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		فعال_نخها++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (فعال_نخها) {
		بنية thread_params *هدف = عدم;
		بنية thread_params *victim = عدم;
		بیعلامت sub_اندازه = 0;

		پیشرفت_قفل();
		لكل (;;) {
			لكل (i = 0; !هدف && i < delta_جستجو_نخها; i++)
				إذا (!p[i].working)
					هدف = &p[i];
			إذا (هدف)
				اكسر;
			pthread_cond_wait(&پیشرفت_cond, &پیشرفت_mutex);
		}

		لكل (i = 0; i < delta_جستجو_نخها; i++)
			إذا (p[i].مانده > 2*پنجره &&
			    (!victim || victim->مانده < p[i].مانده))
				victim = &p[i];
		إذا (victim) {
			sub_اندازه = victim->مانده / 2;
			هدف->regions = victim->regions + victim->مانده - sub_اندازه;
			victim->list_اندازه -= sub_اندازه;
			victim->مانده -= sub_اندازه;
		}
		هدف->list_اندازه = sub_اندازه;
		هدف->مانده = sub_اندازه;
		هدف->working = 1;
		پیشرفت_unlock();

		pthread_mutex_قفل(&هدف->mutex);
		هدف->داده_ready = 1;
		pthread_cond_signal(&هدف->cond);
		pthread_mutex_unlock(&هدف->mutex);

		إذا (!sub_اندازه) {
			pthread_join(هدف->thread, عدم);
			pthread_cond_destroy(&هدف->cond);
			pthread_mutex_destroy(&هدف->mutex);
			فعال_نخها--;
		}
	}
	cleanup_threaded_جستجو();
	حرر(p);

	display_پیشرفت(پیشرفت_state, پیشرفت_nr);
	stop_پیشرفت(&پیشرفت_state);
}

ساكن فراغ prepare_pack(صحيح پنجره, صحيح عمق)
{
	بنية object_مدخل **delta_list;
	uint32_t i, nr_deltas;
	بیعلامت n;

	إذا (use_delta_islands)
		resolve_درخت_islands(the_repository, پیشرفت, &مقصد_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	إذا (!pack_مقصد_stdout)
		do_check_packed_object_بررسی_چرخهای = 1;

	إذا (!مقصد_pack.nr_objects || !پنجره || !عمق)
		أرجع;

	إذا (مسیر_walk)
		ll_پیدا_کن_deltas_by_region(مقصد_pack.objects, مقصد_pack.regions,
					 0, مقصد_pack.nr_regions);

	ALLOC_ARRAY(delta_list, مقصد_pack.nr_objects);
	nr_deltas = n = 0;

	لكل (i = 0; i < مقصد_pack.nr_objects; i++) {
		بنية object_مدخل *مدخل = مقصد_pack.objects + i;

		إذا (!should_attempt_deltas(مدخل))
			استمر;

		إذا (!مدخل->preferred_پایه)
			nr_deltas++;

		delta_list[n++] = مدخل;
	}

	إذا (nr_deltas && n > 1) {
		بیعلامت nr_انجامشده = 0;

		إذا (پیشرفت)
			پیشرفت_state = شروع_پیشرفت(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_اندازه_مرتب_سازی);
		ll_پیدا_کن_deltas(delta_list, n, پنجره+1, عمق, &nr_انجامشده);
		stop_پیشرفت(&پیشرفت_state);
		إذا (nr_انجامشده != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	حرر(delta_list);
}

ساكن صحيح git_pack_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(k, "pack.window")) {
		پنجره = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.windowmemory")) {
		پنجره_حافظه_حد = git_config_ulong(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.depth")) {
		عمق = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_اندازه = git_config_اندازه_t(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_اندازه = git_config_int(k, v, ctx->kvi);
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
		use_bitmap_شاخص_default = git_config_bool(k, v);
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
		delta_جستجو_نخها = git_config_int(k, v, ctx->kvi);
		إذا (delta_جستجو_نخها < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_جستجو_نخها);
		إذا (!HAVE_نخها && delta_جستجو_نخها != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_جستجو_نخها = 0;
		}
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.نسخه = git_config_int(k, v, ctx->kvi);
		إذا (pack_idx_opts.نسخه > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.نسخه);
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
		ثابت حرف *oid_انتها, *pack_انتها;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		بنية object_id pack_hash;

		إذا (!v)
			أرجع config_error_nonbool(k);

		ex = xmalloc(حجم(*ex));
		إذا (parse_oid_hex(v, &ex->e.oid, &oid_انتها) ||
		    *oid_انتها != ' ' ||
		    parse_oid_hex(oid_انتها + 1, &pack_hash, &pack_انتها) ||
		    *pack_انتها != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		إذا (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_انتها - oid_انتها);
		کپی_حافظه(ex->pack_hash_hex, oid_انتها + 1, pack_انتها - oid_انتها - 1);
		ex->uri = xstrdup(pack_انتها + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	أرجع git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ساكن صحيح stdin_packs_یافت_nr;
ساكن صحيح stdin_packs_hints_nr;

ساكن صحيح جمع_کن_object_مدخل_from_pack(ثابت بنية object_id *oid,
				      بنية packed_git *p,
				      uint32_t pos,
				      فراغ *داده_2)
{
	off_t ofs;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type = OBJ_NONE;

	display_پیشرفت(پیشرفت_state, ++nr_دیده);

	إذا (have_duplicate_مدخل(oid, 0))
		أرجع 0;

	stdin_packs_یافت_nr++;

	ofs = nth_packed_object_جابه_جایی(p, pos);

	oi.typep = &type;
	إذا (packed_object_info(عدم, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_مقصد_hex(oid), p->pack_name);
	} وإلا إذا (type == OBJ_COMMIT) {
		بنية rev_info *revs = داده_2;
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
		جمع_کن_pending_oid(revs, عدم, oid, 0);
	}

	إذا (!want_object_درون_pack(oid, 0, &p, &ofs))
		أرجع 0;

	create_object_مدخل(oid, type, 0, 0, 0, p, ofs);

	أرجع 0;
}

ساكن فراغ show_object_pack_hint(بنية object *object, ثابت حرف *name,
				  فراغ *داده)
{
	تعداد stdin_packs_نما نما = *(تعداد stdin_packs_نما *)داده;
	إذا (نما == STDIN_PACKS_نما_FOLLOW) {
		إذا (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			أرجع;
		جمع_کن_object_مدخل(&object->oid, object->type, name, 0);
	} وإلا {
		بنية object_مدخل *oe = packlist_پیدا_کن(&مقصد_pack, &object->oid);
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

ساكن فراغ show_commit_pack_hint(بنية commit *commit, فراغ *داده)
{
	تعداد stdin_packs_نما نما = *(تعداد stdin_packs_نما *)داده;

	إذا (نما == STDIN_PACKS_نما_FOLLOW) {
		show_object_pack_hint((بنية object *)commit, "", داده);
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
تعداد stdin_pack_info_نوع {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

بنية stdin_pack_info {
	بنية packed_git *p;
	تعداد stdin_pack_info_نوع نوع;
};

ساكن صحيح pack_mtime_cmp(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية stdin_pack_info *a = ((ثابت بنية string_list_قلم*)_a)->util;
	بنية stdin_pack_info *b = ((ثابت بنية string_list_قلم*)_b)->util;

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

ساكن صحيح stdin_packs_include_check_obj(بنية object *obj, فراغ *داده استفادهنشده)
{
	أرجع !has_object_kept_pack(مقصد_pack.repo, &obj->oid,
				     KEPT_PACK_درون_CORE);
}

ساكن صحيح stdin_packs_include_check(بنية commit *commit, فراغ *داده)
{
	أرجع stdin_packs_include_check_obj((بنية object *)commit, داده);
}

ساكن فراغ stdin_packs_جمع_کن_pack_entries(بنية strmap *packs,
					 بنية rev_info *revs)
{
	بنية string_list کلیدها = STRING_LIST_INIT_NODUP;
	بنية string_list_قلم *قلم;
	بنية hashmap_iter iter;
	بنية strmap_مدخل *مدخل;

	strmap_for_each_مدخل(packs, &iter, مدخل) {
		بنية stdin_pack_info *info = مدخل->مقدار;
		إذا (!info->p)
			die(_("could not find pack '%s'"), مدخل->کلید);

		string_list_append(&کلیدها, مدخل->کلید)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(کلیدها.اقلام, کلیدها.nr, pack_mtime_cmp);

	for_each_string_list_قلم(قلم, &کلیدها) {
		بنية stdin_pack_info *info = قلم->util;

		إذا (info->نوع & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		إذا ((info->نوع & STDIN_PACK_INCLUDE) ||
		    (info->نوع & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_درون_pack(info->p,
						جمع_کن_object_مدخل_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ترتیب);
	}

	string_list_clear(&کلیدها, 0);
}

ساكن فراغ stdin_packs_read_درون_داد(بنية rev_info *revs,
				   تعداد stdin_packs_نما نما)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strmap packs = STRMAP_INIT;
	بنية packed_git *p;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		بنية stdin_pack_info *info;
		تعداد stdin_pack_info_نوع نوع = STDIN_PACK_INCLUDE;
		ثابت حرف *کلید = buf.buf;

		إذا (!*کلید)
			استمر;
		وإلا إذا (*کلید == '^')
			نوع = STDIN_PACK_EXCLUDE_CLOSED;
		وإلا إذا (*کلید == '!' && نما == STDIN_PACKS_نما_FOLLOW)
			نوع = STDIN_PACK_EXCLUDE_OPEN;

		إذا (نوع != STDIN_PACK_INCLUDE)
			کلید++;

		info = strmap_get(&packs, کلید);
		إذا (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, کلید, info);
		}

		info->نوع |= نوع;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		بنية stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		إذا (!info)
			استمر;

		إذا (info->نوع & STDIN_PACK_INCLUDE) {
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

		إذا (info->نوع & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_درون_core = 1;
		}

		إذا (info->نوع & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_درون_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_جمع_کن_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

ساكن فراغ جمع_کن_unreachable_loose_objects(بنية rev_info *revs);

ساكن فراغ read_stdin_packs(بنية repository *repo,
			     تعداد stdin_packs_نما نما, صحيح rev_list_unpacked)
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
	revs.keep_pack_cache_flags |= KEPT_PACK_درون_CORE;
	revs.blob_objects = 1;
	revs.درخت_objects = 1;
	revs.برچسب_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_درون_core = 1;
	إذا (نما == STDIN_PACKS_نما_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_درون_core_open = 1;
	}
	stdin_packs_read_درون_داد(&revs, نما);
	إذا (rev_list_unpacked)
		جمع_کن_unreachable_loose_objects(&revs);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &نما);

	release_revisions(&revs);

	trace2_داده_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_یافت_nr);
	trace2_داده_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

ساكن فراغ جمع_کن_cruft_object_مدخل(ثابت بنية object_id *oid, تعداد object_type type,
				   بنية packed_git *pack, off_t جابه_جایی,
				   ثابت حرف *name, uint32_t mtime)
{
	بنية object_مدخل *مدخل;

	display_پیشرفت(پیشرفت_state, ++nr_دیده);

	مدخل = packlist_پیدا_کن(&مقصد_pack, oid);
	إذا (مدخل) {
		إذا (name) {
			مدخل->hash = pack_name_hash_fn(name);
			مدخل->no_try_delta = no_try_delta(name);
		}
	} وإلا {
		إذا (!want_object_درون_pack_mtime(oid, 0, &pack, &جابه_جایی, mtime))
			أرجع;
		إذا (!pack && type == OBJ_BLOB) {
			بنية odb_منبع_files *files =
				odb_منبع_files_downcast(the_repository->objects->منبع);
			صحيح یافت_2 = 0;

			لكل (بنية odb_files_dir *dir = files->dirs; !یافت_2 && dir; dir = dir->بعدی)
				إذا (!odb_منبع_read_object_info(&dir->loose->پایه, oid, عدم, 0, عدم))
					یافت_2 = 1;

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
			إذا (!یافت_2)
				أرجع;
		}

		مدخل = create_object_مدخل(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, جابه_جایی);
	}

	إذا (mtime > oe_cruft_mtime(&مقصد_pack, مدخل))
		oe_مجموعه_cruft_mtime(&مقصد_pack, مدخل, mtime);
	أرجع;
}

ساكن فراغ show_cruft_object(بنية object *obj, ثابت حرف *name, فراغ *داده استفادهنشده)
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
	جمع_کن_cruft_object_مدخل(&obj->oid, obj->type, عدم, 0, name, cruft_expiration);
}

ساكن فراغ show_cruft_commit(بنية commit *commit, فراغ *داده)
{
	show_cruft_object((بنية object*)commit, عدم, داده);
}

ساكن صحيح cruft_include_check_obj(بنية object *obj, فراغ *داده استفادهنشده)
{
	أرجع !has_object_kept_pack(مقصد_pack.repo, &obj->oid, KEPT_PACK_درون_CORE);
}

ساكن صحيح cruft_include_check(بنية commit *commit, فراغ *داده)
{
	أرجع cruft_include_check_obj((بنية object*)commit, داده);
}

ساكن فراغ مجموعه_cruft_mtime(ثابت بنية object *object,
			    بنية packed_git *pack,
			    off_t جابه_جایی, زمان_t mtime)
{
	جمع_کن_cruft_object_مدخل(&object->oid, object->type, pack, جابه_جایی, عدم,
			       mtime);
}

ساكن فراغ mark_pack_kept_درون_core(بنية string_list *packs, بیعلامت keep)
{
	بنية string_list_قلم *قلم = عدم;
	for_each_string_list_قلم(قلم, packs) {
		بنية packed_git *p = قلم->util;
		إذا (!p)
			die(_("could not find pack '%s'"), قلم->string);
		إذا (p->is_cruft && keep)
			ignore_packed_keep_درون_core_has_cruft = 1;
		p->pack_keep_درون_core = keep;
	}
}

ساكن فراغ جمع_کن_objects_درون_unpacked_packs(فراغ);

ساكن فراغ enumerate_cruft_objects(فراغ)
{
	إذا (پیشرفت)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Enumerating cruft objects"), 0);

	جمع_کن_objects_درون_unpacked_packs();
	جمع_کن_unreachable_loose_objects(عدم);

	stop_پیشرفت(&پیشرفت_state);
}

ساكن فراغ enumerate_and_traverse_cruft_objects(بنية string_list *fresh_packs)
{
	بنية packed_git *p;
	بنية rev_info revs;
	صحيح ret;

	repo_init_revisions(the_repository, &revs, عدم);

	revs.برچسب_objects = 1;
	revs.درخت_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	إذا (پیشرفت)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = جمع_کن_unseen_recent_objects_مقصد_traversal(&revs, cruft_expiration,
						     مجموعه_cruft_mtime, 1);
	stop_پیشرفت(&پیشرفت_state);

	إذا (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_درون_core = 0;
	mark_pack_kept_درون_core(fresh_packs, 1);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	إذا (پیشرفت)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Traversing cruft objects"), 0);
	nr_دیده = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, عدم);

	stop_پیشرفت(&پیشرفت_state);
	release_revisions(&revs);
}

ساكن فراغ read_cruft_objects(فراغ)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list discard_packs = STRING_LIST_INIT_DUP;
	بنية string_list fresh_packs = STRING_LIST_INIT_DUP;
	بنية packed_git *p;

	ignore_packed_keep_درون_core = 1;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		إذا (!buf.len)
			استمر;

		إذا (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		وإلا
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_مرتب_سازی(&discard_packs);
	string_list_مرتب_سازی(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *pack_name = pack_basename(p);
		بنية string_list_قلم *قلم;

		قلم = string_list_lookup(&fresh_packs, pack_name);
		إذا (!قلم)
			قلم = string_list_lookup(&discard_packs, pack_name);

		إذا (قلم) {
			قلم->util = p;
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
			p->pack_keep_درون_core = 1;
		}
	}

	mark_pack_kept_درون_core(&fresh_packs, 1);
	mark_pack_kept_درون_core(&discard_packs, 0);

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
	حرف سطر[GIT_MAX_HEXSZ + 1 + مسیر_MAX + 2];
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
			جمع_کن_preferred_پایه(&oid);
			استمر;
		}
		إذا (parse_oid_hex(سطر, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), سطر);

		جمع_کن_preferred_پایه_object(p + 1);
		جمع_کن_object_مدخل(&oid, OBJ_NONE, p + 1, 0);
	}
}

ساكن فراغ show_commit(بنية commit *commit, فراغ *داده استفادهنشده)
{
	جمع_کن_object_مدخل(&commit->object.oid, OBJ_COMMIT, عدم, 0);

	إذا (write_bitmap_شاخص)
		شاخص_commit_for_bitmap(commit);

	إذا (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ساكن فراغ show_object(بنية object *obj, ثابت حرف *name,
			فراغ *داده استفادهنشده)
{
	جمع_کن_preferred_پایه_object(name);
	جمع_کن_object_مدخل(&obj->oid, obj->type, name, 0);

	إذا (use_delta_islands)
		رکورد_درخت_عمق(&obj->oid, name);
}

ساكن فراغ show_object__ma_allow_any(بنية object *obj, ثابت حرف *name, فراغ *داده)
{
	أكد(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	إذا (!odb_has_object(the_repository->objects, &obj->oid, 0))
		أرجع;

	show_object(obj, name, داده);
}

ساكن فراغ show_object__ma_allow_promisor(بنية object *obj, ثابت حرف *name, فراغ *داده)
{
	أكد(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	إذا (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(مقصد_pack.repo, &obj->oid))
		أرجع;

	show_object(obj, name, داده);
}

ساكن صحيح option_parse_missing_action(ثابت بنية option *opt,
				       ثابت حرف *arg, صحيح unset)
{
	بنية repository *repo = opt->مقدار;

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

ساكن فراغ show_یال(بنية commit *commit)
{
	جمع_کن_preferred_پایه(&commit->object.oid);
}

ساكن صحيح جمع_کن_object_درون_unpacked_pack(ثابت بنية object_id *oid,
				       بنية object_info *oi,
				       فراغ *داده استفادهنشده)
{
	إذا (cruft) {
		جمع_کن_cruft_object_مدخل(oid, OBJ_NONE, oi->منبع_infop->u.packed.pack,
				       oi->منبع_infop->u.packed.جابه_جایی, عدم,
				       *oi->mtimep);
	} وإلا {
		جمع_کن_object_مدخل(oid, OBJ_NONE, "", 0);
	}
	أرجع 0;
}

ساكن فراغ جمع_کن_objects_درون_unpacked_packs(فراغ)
{
	بنية odb_منبع_files *files =
		odb_منبع_files_downcast(مقصد_pack.repo->objects->منبع);
	زمان_t mtime;
	بنية odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ترتیب |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_درون_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	بنية odb_منبع_info منبع_info;
	بنية object_info oi = {
		.mtimep = &mtime,
		.منبع_infop = &منبع_info,
	};

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی) {
		إذا (!dir->local)
			استمر;

		إذا (odb_منبع_for_each_object(&dir->packed->پایه, &oi,
					       جمع_کن_object_درون_unpacked_pack, عدم, &opts))
			die(_("cannot open pack index"));
	}
}

ساكن صحيح جمع_کن_loose_object(ثابت بنية object_id *oid, ثابت حرف *مسیر,
			    فراغ *داده)
{
	بنية rev_info *revs = داده;
	تعداد object_type type = odb_read_object_info(the_repository->objects, oid, عدم);

	إذا (type < 0) {
		warning(_("loose object at %s could not be examined"), مسیر);
		أرجع 0;
	}

	إذا (cruft) {
		بنية stat st;
		إذا (stat(مسیر, &st) < 0) {
			إذا (errno == ENOENT)
				أرجع 0;
			أرجع error_errno("unable to stat %s", oid_مقصد_hex(oid));
		}

		جمع_کن_cruft_object_مدخل(oid, type, عدم, 0, عدم,
				       st.st_mtime);
	} وإلا {
		جمع_کن_object_مدخل(oid, type, "", 0);
	}

	إذا (revs && type == OBJ_COMMIT)
		جمع_کن_pending_oid(revs, عدم, oid, 0);

	أرجع 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ساكن فراغ جمع_کن_unreachable_loose_objects(بنية rev_info *revs)
{
	for_each_loose_file_درون_منبع(the_repository->objects->منبع,
				      جمع_کن_loose_object, عدم, عدم, revs);
}

ساكن صحيح has_sha1_pack_kept_or_nonlocal(ثابت بنية object_id *oid)
{
	ساكن بنية packed_git *آخرین_یافت = عدم;
	بنية packed_git *p;

	إذا (آخرین_یافت && پیدا_کن_pack_مدخل_one(oid, آخرین_یافت))
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		إذا (p == آخرین_یافت)
			استمر;

		إذا ((!p->pack_local || p->pack_keep || p->pack_keep_درون_core) &&
		    پیدا_کن_pack_مدخل_one(oid, p)) {
			آخرین_یافت = p;
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

ساكن صحيح force_object_loose(بنية odb_منبع *منبع,
			      ثابت بنية object_id *oid,
			      ثابت زمان_t *mtime)
{
	بنية odb_منبع_files *files = odb_منبع_files_downcast(منبع);
	ثابت بنية git_hash_algo *compat = منبع->odb->repo->compat_hash_algo;
	بنية object_info oi = OBJECT_INFO_INIT;
	بنية object_id compat_oid, *compat_oid_p = عدم;
	تعداد object_type type;
	فراغ *buf = عدم;
	اندازه_t len;
	صحيح ret;

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بعدی)
		إذا (!odb_منبع_read_object_info(&dir->loose->پایه, oid, عدم, 0, عدم))
			أرجع 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	إذا (odb_read_object_info_extended(منبع->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_مقصد_hex(oid));
		اذهب بیرون;
	}

	إذا (compat) {
		إذا (repo_oid_مقصد_algop(منبع->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_مقصد_hex(oid), compat->name);
			اذهب بیرون;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_منبع_write_object(&files->dirs->loose->پایه, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

بیرون:
	حرر(buf);
	أرجع ret;
}

ساكن فراغ loosen_استفادهنشده_packed_objects(فراغ)
{
	بنية packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	بنية object_id oid;

	repo_for_each_pack(the_repository, p) {
		إذا (!p->pack_local || p->pack_keep || p->pack_keep_درون_core)
			استمر;

		إذا (open_pack_شاخص(p))
			die(_("cannot open pack index"));

		لكل (i = 0; i < p->رقم_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			إذا (!packlist_پیدا_کن(&مقصد_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				إذا (force_object_loose(the_repository->objects->منبع,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_داده_intmax("pack-objects", the_repository,
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
	       pack_مقصد_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_درون_core &&
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
	name_hash_نسخه = 1;

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

		nr_نتیجه += reuse_packfile_objects;
		nr_دیده += reuse_packfile_objects;
		display_پیشرفت(پیشرفت_state, nr_دیده);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &جمع_کن_object_مدخل_from_bitmap);
	أرجع 0;
}

ساكن فراغ رکورد_recent_object(بنية object *obj,
				 ثابت حرف *name استفادهنشده,
				 فراغ *داده استفادهنشده)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ساكن فراغ رکورد_recent_commit(بنية commit *commit, فراغ *داده استفادهنشده)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ساكن صحيح mark_bitmap_preferred_tip(ثابت بنية reference *ref, فراغ *داده استفادهنشده)
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

ساكن صحيح جمع_کن_objects_by_مسیر(ثابت حرف *مسیر,
			       بنية oid_array *oids,
			       تعداد object_type type,
			       فراغ *داده)
{
	اندازه_t oe_شروع = مقصد_pack.nr_objects;
	اندازه_t oe_انتها;
	بیعلامت صحيح *processed = داده;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	لكل (اندازه_t i = 0; i < oids->nr; i++) {
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

		جمع_کن_object_مدخل(oid, type, مسیر, exclude);

		إذا (type == OBJ_COMMIT) {
			بنية commit *commit;

			إذا (!write_bitmap_شاخص && !use_delta_islands)
				استمر;

			commit = lookup_commit(the_repository, oid);
			إذا (!commit)
				die(_("could not find commit %s"), oid_مقصد_hex(oid));
			إذا (write_bitmap_شاخص)
				شاخص_commit_for_bitmap(commit);
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
			رکورد_درخت_عمق(oid, مسیر);
		}
	}

	oe_انتها = مقصد_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	إذا (oe_انتها == oe_شروع || !پنجره)
		أرجع 0;

	ALLOC_GROW(مقصد_pack.regions,
		   مقصد_pack.nr_regions + 1,
		   مقصد_pack.nr_regions_alloc);

	مقصد_pack.regions[مقصد_pack.nr_regions].شروع = oe_شروع;
	مقصد_pack.regions[مقصد_pack.nr_regions].nr = oe_انتها - oe_شروع;
	مقصد_pack.nr_regions++;

	*processed += oids->nr;
	display_پیشرفت(پیشرفت_state, *processed);

	أرجع 0;
}

ساكن صحيح get_object_list_مسیر_walk(بنية rev_info *revs)
{
	بنية مسیر_walk_info info = مسیر_WALK_INFO_INIT;
	بیعلامت صحيح processed = 0;
	صحيح نتیجه;

	info.revs = revs;
	info.مسیر_fn = جمع_کن_objects_by_مسیر;
	info.مسیر_fn_داده = &processed;

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
	info.یال_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	نتیجه = walk_objects_by_مسیر(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	مسیر_walk_info_clear(&info);

	أرجع نتیجه;
}

ساكن فراغ get_object_list(بنية rev_info *revs, بنية strvec *argv)
{
	بنية راهاندازی_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	حرف سطر[1000];
	صحيح flags = 0;
	صحيح save_warning;

	save_commit_میانگیر = 0;
	راهاندازی_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	طالما (fgets(سطر, حجم(سطر), stdin) != عدم) {
		صحيح len = طول_رشته(سطر);
		إذا (len && سطر[len - 1] == '\n')
			سطر[--len] = 0;
		إذا (!len)
			اكسر;
		إذا (*سطر == '-') {
			إذا (!strcmp(سطر, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_شاخص = 0;
				استمر;
			}
			إذا (starts_with(سطر, "--shallow ")) {
				بنية object_id oid;
				إذا (get_oid_hex(سطر + 10, &oid))
					die("not an object name '%s'", سطر + 10);
				ثبات_shallow(the_repository, &oid);
				use_bitmap_شاخص = 0;
				استمر;
			}
			die(_("not a rev '%s'"), سطر);
		}
		إذا (handle_revision_arg(سطر, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), سطر);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	إذا (use_bitmap_شاخص && !get_object_list_from_bitmap(revs))
		أرجع;

	إذا (use_delta_islands)
		بارگیری_delta_islands(the_repository, پیشرفت);

	إذا (write_bitmap_شاخص)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      عدم);

	إذا (!fn_show_object)
		fn_show_object = show_object;

	إذا (مسیر_walk) {
		إذا (get_object_list_مسیر_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			مسیر_walk = 0;
		}
	}

	إذا (!مسیر_walk) {
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_یالها_uninteresting(revs, show_یال, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				عدم);
	}

	إذا (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		إذا (جمع_کن_unseen_recent_objects_مقصد_traversal(revs,
				unpack_unreachable_expiration, عدم, 0))
			die(_("unable to add recent objects"));
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, رکورد_recent_commit,
				     رکورد_recent_object, عدم);
	}

	إذا (keep_unreachable)
		جمع_کن_objects_درون_unpacked_packs();
	إذا (pack_loose_unreachable)
		جمع_کن_unreachable_loose_objects(عدم);
	إذا (unpack_unreachable)
		loosen_استفادهنشده_packed_objects();

	oid_array_clear(&recent_objects);
}

ساكن فراغ جمع_کن_extra_kept_packs(ثابت بنية string_list *نامها)
{
	بنية packed_git *p;

	إذا (!نامها->nr)
		أرجع;

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *name = basename(p->pack_name);
		صحيح i;

		إذا (!p->pack_local)
			استمر;

		لكل (i = 0; i < نامها->nr; i++)
			إذا (!fspathcmp(name, نامها->اقلام[i].string))
				اكسر;

		إذا (i < نامها->nr) {
			p->pack_keep_درون_core = 1;
			ignore_packed_keep_درون_core = 1;
			استمر;
		}
	}
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	صحيح *val = opt->مقدار;

	BUG_ON_OPT_ARG(arg);

	إذا (!unset)
		*val = 0;
	وإلا إذا (!*val)
		*val = 1;
	أرجع 0;
}

ساكن صحيح option_parse_شاخص_نسخه(ثابت بنية option *opt,
				      ثابت حرف *arg, صحيح unset)
{
	بنية pack_idx_option *popts = opt->مقدار;
	حرف *c;
	ثابت حرف *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->نسخه = strtoul(val, &c, 10);
	إذا (popts->نسخه > 2)
		die(_("unsupported index version %s"), val);
	إذا (*c == ',' && c[1])
		popts->off32_حد = strtoul(c+1, &c, 0);
	إذا (*c || popts->off32_حد & 0x80000000)
		die(_("bad index version '%s'"), val);
	أرجع 0;
}

ساكن صحيح option_parse_unpack_unreachable(ثابت بنية option *opt استفادهنشده,
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

ساكن صحيح option_parse_cruft_expiration(ثابت بنية option *opt استفادهنشده,
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

ساكن صحيح is_not_درون_promisor_pack_obj(بنية object *obj, فراغ *داده استفادهنشده)
{
	بنية odb_منبع_info منبع_info;
	بنية object_info info = {
		.منبع_infop = &منبع_info,
	};

	إذا (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	أرجع منبع_info.منبع->type != ODB_منبع_PACKED || !منبع_info.u.packed.pack->pack_promisor;
}

ساكن صحيح is_not_درون_promisor_pack(بنية commit *commit, فراغ *داده) {
	أرجع is_not_درون_promisor_pack_obj((بنية object *) commit, داده);
}

ساكن صحيح parse_stdin_packs_نما(ثابت بنية option *opt, ثابت حرف *arg,
				  صحيح unset)
{
	تعداد stdin_packs_نما *نما = opt->مقدار;

	إذا (unset)
		*نما = STDIN_PACKS_نما_NONE;
	وإلا إذا (!arg || !*arg)
		*نما = STDIN_PACKS_نما_STANDARD;
	وإلا إذا (!strcmp(arg, "follow"))
		*نما = STDIN_PACKS_نما_FOLLOW;
	وإلا
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	أرجع 0;
}

صحيح cmd_pack_objects(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *پیشوند,
		     بنية repository *repo)
{
	صحيح use_داخلی_rev_list = 0;
	صحيح all_پیشرفت_implied = 0;
	بنية strvec rp = STRVEC_INIT;
	صحيح rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	صحيح rev_list_شاخص = 0;
	تعداد stdin_packs_نما stdin_packs = STDIN_PACKS_نما_NONE;
	بنية string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	بنية option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &پیشرفت, عدم,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_مجموعه_INT(0, "progress", &پیشرفت,
			    N_("show progress meter"), 1),
		OPT_مجموعه_INT(0, "all-progress", &پیشرفت,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_پیشرفت_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_شاخص_نسخه),
		OPT_UNSIGNED(0, "max-pack-size", &pack_اندازه_حد,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &پنجره,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &پنجره_حافظه_حد,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &عمق,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_جستجو_نخها,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_خالی,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_داخلی_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_مجموعه_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_مجموعه_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_مجموعه_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_مجموعه_INT_F(0, "indexed-objects", &rev_list_شاخص,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_نما),
		OPT_BOOL(0, "stdout", &pack_مقصد_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_برچسب,
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
		OPT_BOOL(0, "path-walk", &مسیر_walk,
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_شاخص,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_مجموعه_INT(0, "write-bitmap-index", &write_bitmap_شاخص,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_مجموعه_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_شاخص,
			      N_("write a bitmap index if possible"),
			      WRITE_BITMAP_QUIET, PARSE_OPT_HIDDEN),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_CALLBACK_F(0, "missing", repo, N_("action"),
		  N_("handling for missing objects"), PARSE_OPT_NONEG,
		  option_parse_missing_action),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("do not pack objects in promisor packfiles")),
		OPT_BOOL(0, "exclude-promisor-objects-best-effort",
			 &exclude_promisor_objects_بهترین_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_نسخه,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_انتها(),
	};

	إذا (DFS_رقم_حالتها > (1 << OE_DFS_STATE_بیتها))
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
	إذا (git_env_bool(GIT_TEST_NO_WRITE_REV_شاخص, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	پیشرفت = isatty(2);
	argc = parse_options(argc, argv, پیشوند, pack_objects_options,
			     pack_usage, 0);

	إذا (argc) {
		پایه_name = argv[0];
		argc--;
	}
	إذا (pack_مقصد_stdout != !پایه_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	إذا (مسیر_walk < 0) {
		إذا (use_bitmap_شاخص > 0 ||
		    !use_داخلی_rev_list)
			مسیر_walk = 0;
		وإلا إذا (the_repository->gitdir &&
			 the_repository->settings.pack_use_مسیر_walk)
			مسیر_walk = 1;
		وإلا
			مسیر_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	إذا (عمق < 0)
		عمق = 0;
	إذا (عمق >= (1 << OE_عمق_بیتها)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			عمق, (1 << OE_عمق_بیتها) - 1);
		عمق = (1 << OE_عمق_بیتها) - 1;
	}
	إذا (cache_max_small_delta_اندازه >= (1U << OE_Z_DELTA_بیتها)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_بیتها) - 1);
		cache_max_small_delta_اندازه = (1U << OE_Z_DELTA_بیتها) - 1;
	}
	إذا (پنجره < 0)
		پنجره = 0;

	strvec_افزودن(&rp, "pack-objects");

	إذا (مسیر_walk) {
		ثابت حرف *option = عدم;
		إذا (!مسیر_walk_filter_compatible(&filter_options))
			option = "--filter";

		إذا (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			مسیر_walk = 0;
		}
	}
	إذا (مسیر_walk) {
		strvec_افزودن(&rp, "--objects");
	} وإلا إذا (thin) {
		use_داخلی_rev_list = 1;
		strvec_افزودن(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} وإلا
		strvec_افزودن(&rp, "--objects");

	إذا (rev_list_all) {
		use_داخلی_rev_list = 1;
		strvec_افزودن(&rp, "--all");
	}
	إذا (rev_list_reflog) {
		use_داخلی_rev_list = 1;
		strvec_افزودن(&rp, "--reflog");
	}
	إذا (rev_list_شاخص) {
		use_داخلی_rev_list = 1;
		strvec_افزودن(&rp, "--indexed-objects");
	}
	إذا (rev_list_unpacked && !stdin_packs) {
		use_داخلی_rev_list = 1;
		strvec_افزودن(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_بهترین_effort,
				  "--exclude-promisor-objects-best-effort");
	إذا (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		إذا (!stdin_packs) {
			use_داخلی_rev_list = 1;
			strvec_افزودن(&rp, "--exclude-promisor-objects");
		}
	} وإلا إذا (exclude_promisor_objects_بهترین_effort) {
		use_داخلی_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	إذا (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_داخلی_rev_list = 1;

	إذا (!reuse_object)
		reuse_delta = 0;
	إذا (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	وإلا إذا (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_بهترین_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	إذا (!delta_جستجو_نخها)	/* --threads=0 means autodetect */
		delta_جستجو_نخها = online_cpus();

	إذا (!HAVE_نخها && delta_جستجو_نخها != 1)
		warning(_("no threads support, ignoring --threads"));
	إذا (!pack_مقصد_stdout && !pack_اندازه_حد)
		pack_اندازه_حد = pack_اندازه_حد_cfg;
	إذا (pack_مقصد_stdout && pack_اندازه_حد)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	إذا (pack_اندازه_حد && pack_اندازه_حد < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_اندازه_حد = 1024*1024;
	}

	إذا (!pack_مقصد_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	إذا (!rev_list_all || !rev_list_reflog || !rev_list_شاخص)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	إذا (stdin_packs && use_داخلی_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	إذا (cruft) {
		إذا (use_داخلی_rev_list)
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
	إذا (!pack_مقصد_stdout)
		use_bitmap_شاخص_default = 0;

	إذا (use_bitmap_شاخص < 0)
		use_bitmap_شاخص = use_bitmap_شاخص_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	إذا (!use_داخلی_rev_list || (!pack_مقصد_stdout && write_bitmap_شاخص) || is_repository_shallow(the_repository))
		use_bitmap_شاخص = 0;

	إذا (pack_مقصد_stdout || !rev_list_all)
		write_bitmap_شاخص = 0;

	إذا (name_hash_نسخه < 0)
		name_hash_نسخه = (صحيح)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_نسخه();

	إذا (use_delta_islands)
		strvec_افزودن(&rp, "--topo-order");

	إذا (پیشرفت && all_پیشرفت_implied)
		پیشرفت = 2;

	جمع_کن_extra_kept_packs(&keep_pack_list);
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
	prepare_packing_داده(the_repository, &مقصد_pack);

	إذا (پیشرفت && !cruft)
		پیشرفت_state = شروع_پیشرفت(the_repository,
						_("Enumerating objects"), 0);
	إذا (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} وإلا إذا (cruft) {
		read_cruft_objects();
	} وإلا إذا (!use_داخلی_rev_list) {
		read_object_list_from_stdin();
	} وإلا {
		بنية rev_info revs;

		repo_init_revisions(the_repository, &revs, عدم);
		list_objects_filter_copy(&revs.filter, &filter_options);
		إذا (exclude_promisor_objects_بهترین_effort) {
			revs.include_check = is_not_درون_promisor_pack;
			revs.include_check_obj = is_not_درون_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_پایه();
	إذا (include_برچسب && nr_نتیجه)
		refs_for_each_برچسب_ref(get_main_ref_store(the_repository),
				      جمع_کن_ref_برچسب, عدم);
	stop_پیشرفت(&پیشرفت_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	إذا (non_خالی && !nr_نتیجه)
		اذهب cleanup;
	إذا (nr_نتیجه) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(پنجره, عمق);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	إذا (پیشرفت)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_استفاده_شده_nr);

	trace2_داده_intmax("pack-objects", the_repository, "written", written);
	trace2_داده_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_داده_intmax("pack-objects", the_repository, "reused", reused);
	trace2_داده_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_داده_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_داده_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_استفاده_شده_nr);

cleanup:
	clear_packing_داده(&مقصد_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	أرجع 0;
}
