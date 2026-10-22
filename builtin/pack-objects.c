#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن مضمن بنية object_ثبت *oe_delta(
		ثابت بنية packing_معلومات *pack,
		ثابت بنية object_ثبت *e)
{
	إذا (!e->delta_idx)
		أرجع عدم;
	إذا (e->ext_بنسټ)
		أرجع &pack->ext_bases[e->delta_idx - 1];
	وإلا
		أرجع &pack->objects[e->delta_idx - 1];
}

ساكن مضمن اندازه_t oe_delta_اندازه(بنية packing_معلومات *pack,
				   ثابت بنية object_ثبت *e)
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

اندازه_t oe_get_اندازه_slow(بنية packing_معلومات *pack,
			ثابت بنية object_ثبت *e);

ساكن مضمن اندازه_t oe_اندازه(بنية packing_معلومات *pack,
			     ثابت بنية object_ثبت *e)
{
	إذا (e->اندازه_valid)
		أرجع e->اندازه_2;

	أرجع oe_get_اندازه_slow(pack, e);
}

ساكن مضمن فراغ oe_سېټ_delta(بنية packing_معلومات *pack,
				بنية object_ثبت *e,
				بنية object_ثبت *delta)
{
	إذا (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_idx = 0;
}

ساكن مضمن بنية object_ثبت *oe_delta_sibling(
		ثابت بنية packing_معلومات *pack,
		ثابت بنية object_ثبت *e)
{
	إذا (e->delta_sibling_idx)
		أرجع &pack->objects[e->delta_sibling_idx - 1];
	أرجع عدم;
}

ساكن مضمن بنية object_ثبت *oe_delta_ماشوم(
		ثابت بنية packing_معلومات *pack,
		ثابت بنية object_ثبت *e)
{
	إذا (e->delta_ماشوم_idx)
		أرجع &pack->objects[e->delta_ماشوم_idx - 1];
	أرجع عدم;
}

ساكن مضمن فراغ oe_سېټ_delta_ماشوم(بنية packing_معلومات *pack,
				      بنية object_ثبت *e,
				      بنية object_ثبت *delta)
{
	إذا (delta)
		e->delta_ماشوم_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_ماشوم_idx = 0;
}

ساكن مضمن فراغ oe_سېټ_delta_sibling(بنية packing_معلومات *pack,
					بنية object_ثبت *e,
					بنية object_ثبت *delta)
{
	إذا (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	وإلا
		e->delta_sibling_idx = 0;
}

ساكن مضمن فراغ oe_سېټ_اندازه(بنية packing_معلومات *pack,
			       بنية object_ثبت *e,
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

ساكن مضمن فراغ oe_سېټ_delta_اندازه(بنية packing_معلومات *pack,
				     بنية object_ثبت *e,
				     اندازه_t اندازه)
{
	إذا (اندازه < pack->oe_delta_اندازه_حد) {
		e->delta_اندازه = اندازه;
		e->delta_اندازه_valid = 1;
	} وإلا {
		packing_معلومات_قلف(pack);
		إذا (!pack->delta_اندازه_2)
			ALLOC_ARRAY(pack->delta_اندازه_2, pack->nr_alloc);
		packing_معلومات_unlock(pack);

		pack->delta_اندازه_2[e - pack->objects] = اندازه;
		e->delta_اندازه_valid = 0;
	}
}

#تعریف_کړه دننه_PACK(obj) oe_دننه_pack(&منزل_pack, obj)
#تعریف_کړه اندازه_3(obj) oe_اندازه(&منزل_pack, obj)
#تعریف_کړه سېټ_اندازه(obj,اندازه) oe_سېټ_اندازه(&منزل_pack, obj, اندازه)
#تعریف_کړه DELTA_اندازه(obj) oe_delta_اندازه(&منزل_pack, obj)
#تعریف_کړه DELTA(obj) oe_delta(&منزل_pack, obj)
#تعریف_کړه DELTA_ماشوم(obj) oe_delta_ماشوم(&منزل_pack, obj)
#تعریف_کړه DELTA_SIBLING(obj) oe_delta_sibling(&منزل_pack, obj)
#تعریف_کړه سېټ_DELTA(obj, val) oe_سېټ_delta(&منزل_pack, obj, val)
#تعریف_کړه سېټ_DELTA_EXT(obj, oid) oe_سېټ_delta_ext(&منزل_pack, obj, oid)
#تعریف_کړه سېټ_DELTA_اندازه(obj, val) oe_سېټ_delta_اندازه(&منزل_pack, obj, val)
#تعریف_کړه سېټ_DELTA_ماشوم(obj, val) oe_سېټ_delta_ماشوم(&منزل_pack, obj, val)
#تعریف_کړه سېټ_DELTA_SIBLING(obj, val) oe_سېټ_delta_sibling(&منزل_pack, obj, val)

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

ساكن بنية pack_idx_ثبت **written_list;
ساكن uint32_t nr_پایله, nr_written, nr_کتل_شوی;
ساكن بنية bitmap_شاخص *bitmap_git;
ساكن uint32_t write_layer;

ساكن صحيح non_تشه;
ساكن صحيح reuse_delta = 1, reuse_object = 1;
ساكن صحيح keep_unreachable, unpack_unreachable, include_نښان;
ساكن timestamp_t unpack_unreachable_expiration;
ساكن صحيح pack_loose_unreachable;
ساكن صحيح cruft;
ساكن صحيح shallow = 0;
ساكن timestamp_t cruft_expiration;
ساكن صحيح local;
ساكن صحيح have_non_local_packs;
ساكن صحيح incremental;
ساكن صحيح ignore_packed_keep_on_disk;
ساكن صحيح ignore_packed_keep_دننه_core;
ساكن صحيح ignore_packed_keep_دننه_core_open;
ساكن صحيح ignore_packed_keep_دننه_core_has_cruft;
ساكن صحيح allow_ofs_delta;
ساكن بنية pack_idx_option pack_idx_opts;
ساكن ثابت حرف *بنسټ_name;
ساكن صحيح پرمختګ = 1;
ساكن صحيح کړکۍ = 10;
ساكن بینښه اوږد pack_اندازه_حد;
ساكن صحيح ژوروالی = 50;
ساكن صحيح delta_پلټنه_تارونه;
ساكن صحيح pack_منزل_stdout;
ساكن صحيح sparse;
ساكن صحيح thin;
ساكن صحيح لار_walk = -1;
ساكن صحيح عدد_preferred_بنسټ;
ساكن بنية پرمختګ *پرمختګ_state;

ساكن بنية bitmapped_pack *reuse_packfiles;
ساكن اندازه_t reuse_packfiles_nr;
ساكن اندازه_t reuse_packfiles_کارول_شوی_nr;
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
ساكن صحيح exclude_promisor_objects_غوره_effort;

ساكن صحيح use_delta_islands;

ساكن اندازه_t delta_cache_اندازه = 0;
ساكن اندازه_t max_delta_cache_اندازه = DEFAULT_DELTA_CACHE_اندازه;
ساكن بینښه اوږد cache_max_small_delta_اندازه = 1000;

ساكن بینښه اوږد کړکۍ_حافظه_حد = 0;

ساكن بنية string_list uri_protocols = STRING_LIST_INIT_NODUP;

تعداد missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
ساكن تعداد missing_action arg_missing_action;
ساكن show_object_fn fn_show_object;

بنية configured_exclusion {
	بنية oidmap_ثبت e;
	حرف *pack_hash_hex;
	حرف *uri;
};
ساكن بنية oidmap configured_exclusions;

ساكن بنية oidset excluded_by_config;
ساكن صحيح name_hash_نسخه = -1;

تعداد stdin_packs_ډېرپېښ {
	STDIN_PACKS_ډېرپېښ_NONE,
	STDIN_PACKS_ډېرپېښ_STANDARD,
	STDIN_PACKS_ډېرپېښ_FOLLOW,
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
	ساكن صحيح کتل_شوی_نسخه = -1;

	إذا (کتل_شوی_نسخه < 0)
		کتل_شوی_نسخه = name_hash_نسخه;
	وإلا إذا (کتل_شوی_نسخه != name_hash_نسخه)
		BUG("name hash version changed from %d to %d mid-process",
		    کتل_شوی_نسخه, name_hash_نسخه);

	بدل (name_hash_نسخه) {
	حالة 1:
		أرجع pack_name_hash(name);

	حالة 2:
		أرجع pack_name_hash_v2((ثابت بینښه حرف *)name);

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
ساكن بنية commit **شاخصي_commits;
ساكن بینښه صحيح شاخصي_commits_nr;
ساكن بینښه صحيح شاخصي_commits_alloc;

ساكن فراغ شاخص_commit_for_bitmap(بنية commit *commit)
{
	إذا (شاخصي_commits_nr >= شاخصي_commits_alloc) {
		شاخصي_commits_alloc = (شاخصي_commits_alloc + 32) * 2;
		REALLOC_ARRAY(شاخصي_commits, شاخصي_commits_alloc);
	}

	شاخصي_commits[شاخصي_commits_nr++] = commit;
}

ساكن فراغ *get_delta(بنية object_ثبت *ثبت)
{
	بینښه اوږد اندازه, بنسټ_اندازه;
	اندازه_t delta_اندازه_2;
	فراغ *buf, *بنسټ_buf, *delta_buf;
	تعداد object_type type;
	اندازه_t اندازه_st = 0, بنسټ_اندازه_st = 0;

	buf = odb_read_object(the_repository->objects, &ثبت->idx.oid,
			      &type, &اندازه_st);
	اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
	إذا (!buf)
		die(_("unable to read %s"), oid_منزل_hex(&ثبت->idx.oid));
	بنسټ_buf = odb_read_object(the_repository->objects,
				   &DELTA(ثبت)->idx.oid, &type,
				   &بنسټ_اندازه_st);
	بنسټ_اندازه = cast_اندازه_t_منزل_ulong(بنسټ_اندازه_st);
	إذا (!بنسټ_buf)
		die("unable to read %s",
		    oid_منزل_hex(&DELTA(ثبت)->idx.oid));
	delta_buf = diff_delta(بنسټ_buf, بنسټ_اندازه,
			       buf, اندازه, &delta_اندازه_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	إذا (!delta_buf || delta_اندازه_2 != DELTA_اندازه(ثبت))
		BUG("delta size changed");
	حرر(buf);
	حرر(بنسټ_buf);
	أرجع delta_buf;
}

ساكن بینښه اوږد do_compress(فراغ **pptr, بینښه اوږد اندازه)
{
	git_zstream بهیر;
	فراغ *دننه, *بهر;
	بینښه اوږد maxsize;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	git_deflate_init(&بهیر, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&بهیر, اندازه);

	دننه = *pptr;
	بهر = xmalloc(maxsize);
	*pptr = بهر;

	بهیر.بل_دننه = دننه;
	بهیر.avail_دننه = اندازه;
	بهیر.بل_بهر = بهر;
	بهیر.avail_بهر = maxsize;
	طالما (git_deflate(&بهیر, Z_پای) == Z_OK)
		; /* nothing */
	git_deflate_پای_ټکی(&بهیر);

	حرر(دننه);
	أرجع بهیر.ټول_بهر;
}

ساكن بینښه اوږد write_large_blob_معلومات(بنية odb_بهیر *st, بنية hashfile *f,
					   ثابت بنية object_id *oid)
{
	git_zstream بهیر;
	بینښه حرف ibuf[1024 * 16];
	بینښه حرف obuf[1024 * 16];
	بینښه اوږد olen = 0;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	git_deflate_init(&بهیر, cfg->pack_compression_level);

	لكل (;;) {
		ssize_t readlen;
		صحيح zret = Z_OK;
		readlen = odb_بهیر_read(st, ibuf, حجم(ibuf));
		إذا (readlen == -1)
			die(_("unable to read %s"), oid_منزل_hex(oid));

		بهیر.بل_دننه = ibuf;
		بهیر.avail_دننه = readlen;
		طالما ((بهیر.avail_دننه || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			بهیر.بل_بهر = obuf;
			بهیر.avail_بهر = حجم(obuf);
			zret = git_deflate(&بهیر, readlen ? 0 : Z_پای);
			hashwrite(f, obuf, بهیر.بل_بهر - obuf);
			olen += بهیر.بل_بهر - obuf;
		}
		إذا (بهیر.avail_دننه)
			die(_("deflate error (%d)"), zret);
		إذا (readlen == 0) {
			إذا (zret != Z_بهیر_پای_ټکی)
				die(_("deflate error (%d)"), zret);
			اكسر;
		}
	}
	git_deflate_پای_ټکی(&بهیر);
	أرجع olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
ساكن صحيح check_pack_inflate(بنية packed_git *p,
		بنية pack_کړکۍ **w_curs,
		off_t دننه_واټن,
		off_t len,
		اندازه_t expect)
{
	git_zstream بهیر;
	بینښه حرف fakebuf[4096], *دننه;
	صحيح st;

	memset(&بهیر, 0, حجم(بهیر));
	git_inflate_init(&بهیر);
	افعل {
		دننه = use_pack(p, w_curs, دننه_واټن, &بهیر.avail_دننه);
		بهیر.بل_دننه = دننه;
		بهیر.بل_بهر = fakebuf;
		بهیر.avail_بهر = حجم(fakebuf);
		st = git_inflate(&بهیر, Z_پای);
		دننه_واټن += بهیر.بل_دننه - دننه;
	} طالما (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_پای_ټکی(&بهیر);
	أرجع (st == Z_بهیر_پای_ټکی &&
		بهیر.ټول_بهر == expect &&
		بهیر.ټول_دننه == len) ? 0 : -1;
}

ساكن فراغ copy_pack_معلومات(بنية hashfile *f,
		بنية packed_git *p,
		بنية pack_کړکۍ **w_curs,
		off_t دننه_واټن,
		off_t len)
{
	بینښه حرف *دننه;
	اندازه_t avail;

	طالما (len) {
		دننه = use_pack(p, w_curs, دننه_واټن, &avail);
		إذا (avail > len)
			avail = (بینښه اوږد)len;
		hashwrite(f, دننه, avail);
		دننه_واټن += avail;
		len -= avail;
	}
}

ساكن مضمن صحيح oe_اندازه_greater_than(بنية packing_معلومات *pack,
				       ثابت بنية object_ثبت *lhs,
				       اندازه_t rhs)
{
	إذا (lhs->اندازه_valid)
		أرجع lhs->اندازه_2 > rhs;
	إذا (rhs < pack->oe_اندازه_حد) /* rhs < 2^x <= lhs ? */
		أرجع 1;
	أرجع oe_get_اندازه_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
ساكن بینښه اوږد write_no_reuse_object(بنية hashfile *f, بنية object_ثبت *ثبت,
					   بینښه اوږد حد, صحيح usable_delta)
{
	بینښه اوږد اندازه, datalen;
	بینښه حرف سرلیک[MAX_PACK_OBJECT_سرلیک],
		      dheader[MAX_PACK_OBJECT_سرلیک];
	بینښه hdrlen;
	تعداد object_type type;
	فراغ *buf;
	بنية odb_بهیر *st = عدم;
	ثابت بینښه hashsz = the_hash_algo->rawsz;

	إذا (!usable_delta) {
		إذا (oe_type(ثبت) == OBJ_BLOB &&
		    oe_اندازه_greater_than(&منزل_pack, ثبت,
					 repo_settings_get_big_file_پوله(the_repository)) &&
		    (st = odb_بهیر_from_object(the_repository->objects, &ثبت->idx.oid,
						 عدم)) != عدم) {
			buf = عدم;
			type = st->type;
			اندازه = st->اندازه;
		} وإلا {
			اندازه_t اندازه_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &ثبت->idx.oid, &type,
					      &اندازه_st);
			اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
			إذا (!buf)
				die(_("unable to read %s"),
				    oid_منزل_hex(&ثبت->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(ثبت->delta_معلومات);
		ثبت->z_delta_اندازه = 0;
	} وإلا إذا (ثبت->delta_معلومات) {
		اندازه = DELTA_اندازه(ثبت);
		buf = ثبت->delta_معلومات;
		ثبت->delta_معلومات = عدم;
		type = (allow_ofs_delta && DELTA(ثبت)->idx.دننه_واټن) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} وإلا {
		buf = get_delta(ثبت);
		اندازه = DELTA_اندازه(ثبت);
		type = (allow_ofs_delta && DELTA(ثبت)->idx.دننه_واټن) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	إذا (st)	/* large blob case, just assume we don't compress well */
		datalen = اندازه;
	وإلا إذا (ثبت->z_delta_اندازه)
		datalen = ثبت->z_delta_اندازه;
	وإلا
		datalen = do_compress(&buf, اندازه);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_دننه_pack_object_سرلیک(سرلیک, حجم(سرلیک),
					      type, اندازه);

	إذا (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = ثبت->idx.دننه_واټن - DELTA(ثبت)->idx.دننه_واټن;
		بینښه pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد) {
			إذا (st)
				odb_بهیر_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرلیک, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		إذا (حد && hdrlen + hashsz + datalen + hashsz >= حد) {
			إذا (st)
				odb_بهیر_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرلیک, hdrlen);
		hashwrite(f, DELTA(ثبت)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} وإلا {
		إذا (حد && hdrlen + datalen + hashsz >= حد) {
			إذا (st)
				odb_بهیر_close(st);
			حرر(buf);
			أرجع 0;
		}
		hashwrite(f, سرلیک, hdrlen);
	}
	إذا (st) {
		datalen = write_large_blob_معلومات(st, f, &ثبت->idx.oid);
		odb_بهیر_close(st);
	} وإلا {
		hashwrite(f, buf, datalen);
		حرر(buf);
	}

	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_reuse_object(بنية hashfile *f, بنية object_ثبت *ثبت,
				بینښه اوږد حد, صحيح usable_delta)
{
	بنية packed_git *p = دننه_PACK(ثبت);
	بنية pack_کړکۍ *w_curs = عدم;
	uint32_t pos;
	off_t دننه_واټن, cur;
	تعداد object_type type = oe_type(ثبت);
	تعداد object_type دننه_pack_type;
	off_t datalen;
	بینښه حرف سرلیک[MAX_PACK_OBJECT_سرلیک],
		      dheader[MAX_PACK_OBJECT_سرلیک];
	بینښه hdrlen;
	ثابت بینښه hashsz = the_hash_algo->rawsz;
	اندازه_t ثبت_اندازه;

	cur = ثبت->دننه_pack_دننه_واټن;
	دننه_pack_type = unpack_object_سرلیک(p, &w_curs, &cur, &ثبت_اندازه);
	إذا (دننه_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_منزل_hex(&ثبت->idx.oid));

	إذا (DELTA(ثبت))
		type = (allow_ofs_delta && DELTA(ثبت)->idx.دننه_واټن) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_دننه_pack_object_سرلیک(سرلیک, حجم(سرلیک),
					      type, ثبت_اندازه);

	دننه_واټن = ثبت->دننه_pack_دننه_واټن;
	إذا (دننه_واټن_منزل_pack_pos(p, دننه_واټن, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_منزل_hex(&ثبت->idx.oid), (uintmax_t)دننه_واټن,
		    p->pack_name);
	datalen = pack_pos_منزل_دننه_واټن(p, pos + 1) - دننه_واټن;
	إذا (!pack_منزل_stdout && p->شاخص_نسخه > 1 &&
	    check_pack_دوراني_ازموینه(p, &w_curs, دننه_واټن, datalen,
			   pack_pos_منزل_شاخص(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_منزل_hex(&ثبت->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, ثبت, حد, usable_delta);
	}

	دننه_واټن += ثبت->دننه_pack_سرلیک_اندازه;
	datalen -= ثبت->دننه_pack_سرلیک_اندازه;

	إذا (!pack_منزل_stdout && p->شاخص_نسخه == 1 &&
	    check_pack_inflate(p, &w_curs, دننه_واټن, datalen, ثبت_اندازه)) {
		error(_("corrupt packed object for %s"),
		      oid_منزل_hex(&ثبت->idx.oid));
		unuse_pack(&w_curs);
		أرجع write_no_reuse_object(f, ثبت, حد, usable_delta);
	}

	إذا (type == OBJ_OFS_DELTA) {
		off_t ofs = ثبت->idx.دننه_واټن - DELTA(ثبت)->idx.دننه_واټن;
		بینښه pos = حجم(dheader) - 1;
		dheader[pos] = ofs & 127;
		طالما (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		إذا (حد && hdrlen + حجم(dheader) - pos + datalen + hashsz >= حد) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرلیک, hdrlen);
		hashwrite(f, dheader + pos, حجم(dheader) - pos);
		hdrlen += حجم(dheader) - pos;
		reused_delta++;
	} وإلا إذا (type == OBJ_REF_DELTA) {
		إذا (حد && hdrlen + hashsz + datalen + hashsz >= حد) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرلیک, hdrlen);
		hashwrite(f, DELTA(ثبت)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} وإلا {
		إذا (حد && hdrlen + datalen + hashsz >= حد) {
			unuse_pack(&w_curs);
			أرجع 0;
		}
		hashwrite(f, سرلیک, hdrlen);
	}
	copy_pack_معلومات(f, p, &w_curs, دننه_واټن, datalen);
	unuse_pack(&w_curs);
	reused++;
	أرجع hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
ساكن off_t write_object(بنية hashfile *f,
			  بنية object_ثبت *ثبت,
			  off_t write_دننه_واټن)
{
	بینښه اوږد حد;
	off_t len;
	صحيح usable_delta, منزل_reuse;

	إذا (!pack_منزل_stdout)
		دوراني_ازموینه_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	إذا (!pack_اندازه_حد || !nr_written)
		حد = 0;
	وإلا إذا (pack_اندازه_حد <= write_دننه_واټن)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		حد = 1;
	وإلا
		حد = pack_اندازه_حد - write_دننه_واټن;

	إذا (!DELTA(ثبت))
		usable_delta = 0;	/* no delta */
	وإلا إذا (!pack_اندازه_حد)
	       usable_delta = 1;	/* unlimited packfile */
	وإلا إذا (DELTA(ثبت)->idx.دننه_واټن == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	وإلا إذا (DELTA(ثبت)->idx.دننه_واټن)
		usable_delta = 1;	/* base already exists in this pack */
	وإلا
		usable_delta = 0;	/* base could end up in another pack */

	إذا (!reuse_object)
		منزل_reuse = 0;	/* explicit */
	وإلا إذا (!دننه_PACK(ثبت))
		منزل_reuse = 0;	/* can't reuse what we don't have */
	وإلا إذا (oe_type(ثبت) == OBJ_REF_DELTA ||
		 oe_type(ثبت) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		منزل_reuse = usable_delta;
				/* ... but pack split may override that */
	وإلا إذا (oe_type(ثبت) != ثبت->دننه_pack_type)
		منزل_reuse = 0;	/* pack has delta which is unusable */
	وإلا إذا (DELTA(ثبت))
		منزل_reuse = 0;	/* we want to pack afresh */
	وإلا
		منزل_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	إذا (!منزل_reuse)
		len = write_no_reuse_object(f, ثبت, حد, usable_delta);
	وإلا
		len = write_reuse_object(f, ثبت, حد, usable_delta);
	إذا (!len)
		أرجع 0;

	إذا (usable_delta)
		written_delta++;
	written++;
	إذا (!pack_منزل_stdout)
		ثبت->idx.دوراني_ازموینه_32 = دوراني_ازموینه_32_پای_ټکی(f);
	أرجع len;
}

تعداد write_one_حالت {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*نورمال*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

ساكن تعداد write_one_حالت write_one(بنية hashfile *f,
				       بنية object_ثبت *e,
				       off_t *دننه_واټن)
{
	off_t اندازه;
	صحيح recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.دننه_واټن == 1);
	إذا (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_منزل_hex(&e->idx.oid));
		أرجع WRITE_ONE_RECURSIVE;
	} وإلا إذا (e->idx.دننه_واټن || e->preferred_بنسټ) {
		/* offset is non zero if object is written already. */
		أرجع WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	إذا (DELTA(e)) {
		e->idx.دننه_واټن = 1; /* now recurse */
		بدل (write_one(f, DELTA(e), دننه_واټن)) {
		حالة WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			سېټ_DELTA(e, عدم);
			اكسر;
		افتراضي:
			اكسر;
		حالة WRITE_ONE_BREAK:
			e->idx.دننه_واټن = recursing;
			أرجع WRITE_ONE_BREAK;
		}
	}

	e->idx.دننه_واټن = *دننه_واټن;
	اندازه = write_object(f, e, *دننه_واټن);
	إذا (!اندازه) {
		e->idx.دننه_واټن = recursing;
		أرجع WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جمع_کړه_overflows(*دننه_واټن, اندازه))
		die(_("pack too large for current definition of off_t"));
	*دننه_واټن += اندازه;
	أرجع WRITE_ONE_WRITTEN;
}

ساكن صحيح mark_tagged(ثابت بنية reference *ref, فراغ *cb_معلومات ناکارول_شوی)
{
	بنية object_id peeled;
	بنية object_ثبت *ثبت = packlist_پیدا_کړه(&منزل_pack, ref->oid);

	إذا (ثبت)
		ثبت->tagged = 1;
	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		ثبت = packlist_پیدا_کړه(&منزل_pack, &peeled);
		إذا (ثبت)
			ثبت->tagged = 1;
	}
	أرجع 0;
}

ساكن مضمن بینښه حرف oe_layer(بنية packing_معلومات *pack,
				     بنية object_ثبت *e)
{
	إذا (!pack->layer)
		أرجع 0;
	أرجع pack->layer[e - pack->objects];
}

ساكن مضمن فراغ جمع_کړه_منزل_write_ترتیب(بنية object_ثبت **wo,
			       بینښه صحيح *endp,
			       بنية object_ثبت *e)
{
	إذا (e->filled || oe_layer(&منزل_pack, e) != write_layer)
		أرجع;
	wo[(*endp)++] = e;
	e->filled = 1;
}

ساكن فراغ جمع_کړه_descendants_منزل_write_ترتیب(بنية object_ثبت **wo,
					   بینښه صحيح *endp,
					   بنية object_ثبت *e)
{
	صحيح جمع_کړه_منزل_ترتیب = 1;
	طالما (e) {
		إذا (جمع_کړه_منزل_ترتیب) {
			بنية object_ثبت *s;
			/* add this node... */
			جمع_کړه_منزل_write_ترتیب(wo, endp, e);
			/* all its siblings... */
			لكل (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				جمع_کړه_منزل_write_ترتیب(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		إذا (DELTA_ماشوم(e)) {
			جمع_کړه_منزل_ترتیب = 1;
			e = DELTA_ماشوم(e);
		} وإلا {
			جمع_کړه_منزل_ترتیب = 0;
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

ساكن فراغ جمع_کړه_family_منزل_write_ترتیب(بنية object_ثبت **wo,
				      بینښه صحيح *endp,
				      بنية object_ثبت *e)
{
	بنية object_ثبت *ریښه;

	لكل (ریښه = e; DELTA(ریښه); ریښه = DELTA(ریښه))
		; /* nothing */
	جمع_کړه_descendants_منزل_write_ترتیب(wo, endp, ریښه);
}

ساكن فراغ compute_layer_ترتیب(بنية object_ثبت **wo, بینښه صحيح *wo_پای_ټکی)
{
	بینښه صحيح i, وروستی_untagged;
	بنية object_ثبت *objects = منزل_pack.objects;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			اكسر;
		جمع_کړه_منزل_write_ترتیب(wo, wo_پای_ټکی, &objects[i]);
	}
	وروستی_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	لكل (; i < منزل_pack.nr_objects; i++) {
		إذا (objects[i].tagged)
			جمع_کړه_منزل_write_ترتیب(wo, wo_پای_ټکی, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	لكل (i = وروستی_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_نښان)
			استمر;
		جمع_کړه_منزل_write_ترتیب(wo, wo_پای_ټکی, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	لكل (i = وروستی_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (oe_type(&objects[i]) != OBJ_ونه)
			استمر;
		جمع_کړه_منزل_write_ترتیب(wo, wo_پای_ټکی, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	لكل (i = وروستی_untagged; i < منزل_pack.nr_objects; i++) {
		إذا (!objects[i].filled && oe_layer(&منزل_pack, &objects[i]) == write_layer)
			جمع_کړه_family_منزل_write_ترتیب(wo, wo_پای_ټکی, &objects[i]);
	}
}

ساكن بنية object_ثبت **compute_write_ترتیب(فراغ)
{
	uint32_t max_layers = 1;
	بینښه صحيح i, wo_پای_ټکی;

	بنية object_ثبت **wo;
	بنية object_ثبت *objects = منزل_pack.objects;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		سېټ_DELTA_ماشوم(&objects[i], عدم);
		سېټ_DELTA_SIBLING(&objects[i], عدم);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	لكل (i = منزل_pack.nr_objects; i > 0;) {
		بنية object_ثبت *e = &objects[--i];
		إذا (!DELTA(e))
			استمر;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_ماشوم_idx;
		سېټ_DELTA_ماشوم(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_نښان_ref(get_main_ref_store(the_repository), mark_tagged,
			      عدم);

	إذا (use_delta_islands) {
		max_layers = compute_pack_layers(&منزل_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, منزل_pack.nr_objects);
	wo_پای_ټکی = 0;

	لكل (; write_layer < max_layers; ++write_layer)
		compute_layer_ترتیب(wo, &wo_پای_ټکی);

	إذا (wo_پای_ټکی != منزل_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_پای_ټکی, منزل_pack.nr_objects);

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

ساكن فراغ ریکارډ_reused_object(off_t where, off_t دننه_واټن)
{
	إذا (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == دننه_واټن)
		أرجع;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = دننه_واټن;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
ساكن off_t پیدا_کړه_reused_دننه_واټن(off_t where)
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
				  اندازه_t pos, بنية hashfile *بهر,
				  off_t pack_پیل,
				  بنية pack_کړکۍ **w_curs)
{
	off_t دننه_واټن, بل, cur;
	تعداد object_type type;
	اندازه_t اندازه;

	دننه_واټن = pack_pos_منزل_دننه_واټن(reuse_packfile, pos);
	بل = pack_pos_منزل_دننه_واټن(reuse_packfile, pos + 1);

	ریکارډ_reused_object(دننه_واټن,
			     دننه_واټن - (hashfile_ټول(بهر) - pack_پیل));

	cur = دننه_واټن;
	type = unpack_object_سرلیک(reuse_packfile, w_curs, &cur, &اندازه);
	أكد(type >= 0);

	إذا (type == OBJ_OFS_DELTA) {
		off_t بنسټ_دننه_واټن;
		off_t fixup;

		بینښه حرف سرلیک[MAX_PACK_OBJECT_سرلیک];
		بینښه len;

		بنسټ_دننه_واټن = get_delta_بنسټ(reuse_packfile, w_curs, &cur, type, دننه_واټن);
		أكد(بنسټ_دننه_واټن != 0);

		/* Convert to REF_DELTA if we must... */
		إذا (!allow_ofs_delta) {
			uint32_t بنسټ_pos;
			بنية object_id بنسټ_oid;

			إذا (دننه_واټن_منزل_pack_pos(reuse_packfile, بنسټ_دننه_واټن, &بنسټ_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)بنسټ_دننه_واټن,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&بنسټ_oid, reuse_packfile,
					     pack_pos_منزل_شاخص(reuse_packfile, بنسټ_pos));

			len = encode_دننه_pack_object_سرلیک(سرلیک, حجم(سرلیک),
							   OBJ_REF_DELTA, اندازه);
			hashwrite(بهر, سرلیک, len);
			hashwrite(بهر, بنسټ_oid.hash, the_hash_algo->rawsz);
			copy_pack_معلومات(بهر, reuse_packfile, w_curs, cur, بل - cur);
			أرجع;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = پیدا_کړه_reused_دننه_واټن(دننه_واټن) -
			پیدا_کړه_reused_دننه_واټن(بنسټ_دننه_واټن);
		إذا (fixup) {
			بینښه حرف ofs_سرلیک[MAX_PACK_OBJECT_سرلیک];
			بینښه i, ofs_len;
			off_t ofs = دننه_واټن - بنسټ_دننه_واټن - fixup;

			len = encode_دننه_pack_object_سرلیک(سرلیک, حجم(سرلیک),
							   OBJ_OFS_DELTA, اندازه);

			i = حجم(ofs_سرلیک) - 1;
			ofs_سرلیک[i] = ofs & 127;
			طالما (ofs >>= 7)
				ofs_سرلیک[--i] = 128 | (--ofs & 127);

			ofs_len = حجم(ofs_سرلیک) - i;

			hashwrite(بهر, سرلیک, len);
			hashwrite(بهر, ofs_سرلیک + حجم(ofs_سرلیک) - ofs_len, ofs_len);
			copy_pack_معلومات(بهر, reuse_packfile, w_curs, cur, بل - cur);
			أرجع;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_معلومات(بهر, reuse_packfile, w_curs, دننه_واټن, بل - دننه_واټن);
}

ساكن اندازه_t write_reused_pack_verbatim(بنية bitmapped_pack *reuse_packfile,
					 بنية hashfile *بهر,
					 بنية pack_کړکۍ **w_curs)
{
	اندازه_t pos = 0;
	اندازه_t پای_ټکی;

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
		أرجع reuse_packfile->bitmap_pos / بټونه_دننه_EWORD;
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
	پای_ټکی = reuse_packfile->bitmap_nr / بټونه_دننه_EWORD;
	إذا (reuse_packfile_bitmap->کلمه_alloc < پای_ټکی)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	طالما (pos < پای_ټکی && reuse_packfile_bitmap->ټکي[pos] == (eword_t)~0)
		pos++;

	إذا (pos) {
		off_t منزل_write;

		written = (pos * بټونه_دننه_EWORD);
		منزل_write = pack_pos_منزل_دننه_واټن(reuse_packfile->p, written)
			- حجم(بنية pack_سرلیک);

		/* We're recording one chunk, not one object. */
		ریکارډ_reused_object(حجم(بنية pack_سرلیک), 0);
		hashflush(بهر);
		copy_pack_معلومات(بهر, reuse_packfile->p, w_curs,
			حجم(بنية pack_سرلیک), منزل_write);

		display_پرمختګ(پرمختګ_state, written);
	}
	أرجع pos;
}

ساكن فراغ write_reused_pack(بنية bitmapped_pack *reuse_packfile,
			      بنية hashfile *f)
{
	اندازه_t i = reuse_packfile->bitmap_pos / بټونه_دننه_EWORD;
	uint32_t دننه_واټن;
	off_t pack_پیل = hashfile_ټول(f) - حجم(بنية pack_سرلیک);
	بنية pack_کړکۍ *w_curs = عدم;

	إذا (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	لكل (; i < reuse_packfile_bitmap->کلمه_alloc; ++i) {
		eword_t کلمه = reuse_packfile_bitmap->ټکي[i];
		اندازه_t pos = (i * بټونه_دننه_EWORD);

		لكل (دننه_واټن = 0; دننه_واټن < بټونه_دننه_EWORD; ++دننه_واټن) {
			uint32_t pack_pos;
			إذا ((کلمه >> دننه_واټن) == 0)
				اكسر;

			دننه_واټن += ewah_بټ_ctz64(کلمه >> دننه_واټن);
			إذا (pos + دننه_واټن < reuse_packfile->bitmap_pos)
				استمر;
			إذا (pos + دننه_واټن >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				اذهب بشپړ;

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

				midx_pos = pack_pos_منزل_midx(m, pos + دننه_واټن);
				pack_ofs = nth_midxed_دننه_واټن(m, midx_pos);

				إذا (دننه_واټن_منزل_pack_pos(reuse_packfile->p,
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
				pack_pos = pos + دننه_واټن;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_پیل, &w_curs);
			display_پرمختګ(پرمختګ_state, ++written);
		}
	}

بشپړ:
	unuse_pack(&w_curs);
}

ساكن فراغ write_excluded_by_configs(فراغ)
{
	بنية oidset_iter iter;
	ثابت بنية object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	طالما ((oid = oidset_iter_بل(&iter))) {
		بنية configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		إذا (!ex)
			BUG("configured exclusion wasn't configured");
		write_دننه_ډک(1, ex->pack_hash_hex, مزي_اوږدوالی(ex->pack_hash_hex));
		write_دننه_ډک(1, " ", 1);
		write_دننه_ډک(1, ex->uri, مزي_اوږدوالی(ex->uri));
		write_دننه_ډک(1, "\n", 1);
	}
}

ساكن ثابت حرف no_وېشل_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

ساكن فراغ write_pack_file(فراغ)
{
	uint32_t i = 0, j;
	بنية hashfile *f;
	off_t دننه_واټن;
	uint32_t nr_پاتې_شوی = nr_پایله;
	وخت_t وروستی_mtime = 0;
	بنية object_ثبت **write_ترتیب;
	off_t بایټونه_written = 0;

	إذا (پرمختګ > pack_منزل_stdout)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Writing objects"), nr_پایله);
	ALLOC_ARRAY(written_list, منزل_pack.nr_objects);
	write_ترتیب = compute_write_ترتیب();

	افعل {
		بینښه حرف hash[GIT_MAX_RAWSZ];
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
				.پرمختګ = پرمختګ_state,
				.بفر_len = LARGE_PACKET_معلومات_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} وإلا {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		دننه_واټن = write_pack_سرلیک(f, nr_پاتې_شوی);

		إذا (reuse_packfiles_nr) {
			أكد(pack_منزل_stdout);
			لكل (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				إذا (reused_chunks_nr)
					reuse_packfiles_کارول_شوی_nr++;
			}
			دننه_واټن = hashfile_ټول(f);
		}

		nr_written = 0;
		لكل (; i < منزل_pack.nr_objects; i++) {
			بنية object_ثبت *e = write_ترتیب[i];
			إذا (write_one(f, e, &دننه_واټن) == WRITE_ONE_BREAK)
				اكسر;
			display_پرمختګ(پرمختګ_state, written);
		}

		بایټونه_written += hashfile_ټول(f) +
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
					  CSUM_HASH_دننه_بهیر | CSUM_CLOSE);
		} وإلا إذا (nr_written == nr_پاتې_شوی) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_دننه_بهیر | CSUM_FSYNC | CSUM_CLOSE);
		} وإلا {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			صحيح fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_سرلیک_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, دننه_واټن);
			close(fd);
			إذا (write_bitmap_شاخص) {
				إذا (write_bitmap_شاخص != WRITE_BITMAP_QUIET)
					warning(_(no_وېشل_warning));
				write_bitmap_شاخص = 0;
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
			} وإلا إذا (!وروستی_mtime) {
				وروستی_mtime = st.st_mtime;
			} وإلا {
				بنية utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --وروستی_mtime;
				إذا (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", بنسټ_name,
				    hash_منزل_hex(hash));

			إذا (write_bitmap_شاخص) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &منزل_pack,
						   عدم);
				bitmap_writer_سېټ_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_شاخص(&bitmap_writer,
							       written_list);
			}

			إذا (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &منزل_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			إذا (write_bitmap_شاخص) {
				اندازه_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_پرمختګ(&پرمختګ_state);

				bitmap_writer_show_پرمختګ(&bitmap_writer,
							    پرمختګ);
				bitmap_writer_select_commits(&bitmap_writer,
							     شاخصي_commits,
							     شاخصي_commits_nr);
				إذا (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_پای(&bitmap_writer,
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
			puts(hash_منزل_hex(hash));
		}

		/* mark written objects as written to previous pack */
		لكل (j = 0; j < nr_written; j++) {
			written_list[j]->دننه_واټن = (off_t)-1;
		}
		nr_پاتې_شوی -= nr_written;
	} طالما (nr_پاتې_شوی && i < منزل_pack.nr_objects);

	حرر(written_list);
	حرر(write_ترتیب);
	stop_پرمختګ(&پرمختګ_state);
	إذا (written != nr_پایله)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_پایله);
	trace2_معلومات_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_پایله);
	trace2_معلومات_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", بایټونه_written);
}

ساكن صحيح no_try_delta(ثابت حرف *لار)
{
	ساكن بنية attr_check *check;

	إذا (!check)
		check = attr_check_initl("delta", عدم);
	git_check_attr(the_repository->شاخص, لار, check);
	إذا (ATTR_FALSE(check->توکي[0].ارزښت))
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
ساكن صحيح have_duplicate_ثبت(ثابت بنية object_id *oid,
				صحيح exclude)
{
	بنية object_ثبت *ثبت;

	إذا (reuse_packfile_bitmap &&
	    bitmap_walk_لري(bitmap_git, reuse_packfile_bitmap, oid))
		أرجع 1;

	ثبت = packlist_پیدا_کړه(&منزل_pack, oid);
	إذا (!ثبت)
		أرجع 0;

	إذا (exclude) {
		إذا (!ثبت->preferred_بنسټ)
			nr_پایله--;
		ثبت->preferred_بنسټ = 1;
	}

	أرجع 1;
}

ساكن صحيح want_cruft_object_mtime(بنية repository *r,
				   ثابت بنية object_id *oid,
				   بینښه flags, uint32_t mtime)
{
	بنية odb_سرچینه_files *files = odb_سرچینه_files_downcast(r->objects->سرچینه);

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بل) {
		بنية packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		لكل (; *cache; cache++) {
			بنية packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = پیدا_کړه_pack_ثبت_one(oid, p);
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
			إذا (!بارول_pack_mtimes(p)) {
				uint32_t pos;
				إذا (دننه_واټن_منزل_pack_pos(p, ofs, &pos) < 0)
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

ساكن صحيح want_وموندل_object(ثابت بنية object_id *oid, صحيح exclude,
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
	إذا (ignore_packed_keep_on_disk || ignore_packed_keep_دننه_core ||
	    ignore_packed_keep_دننه_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		بینښه flags = 0;
		إذا (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		إذا (ignore_packed_keep_دننه_core)
			flags |= KEPT_PACK_دننه_CORE;
		إذا (ignore_packed_keep_دننه_core_open)
			flags |= KEPT_PACK_دننه_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		إذا (!ignore_packed_keep_دننه_core_has_cruft) {
			إذا (ignore_packed_keep_on_disk && p->pack_keep)
				أرجع 0;
			إذا (ignore_packed_keep_دننه_core && p->pack_keep_دننه_core)
				أرجع 0;
			إذا (ignore_packed_keep_دننه_core_open && p->pack_keep_دننه_core_open)
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

ساكن صحيح want_object_دننه_pack_one(بنية packed_git *p,
				   ثابت بنية object_id *oid,
				   صحيح exclude,
				   بنية packed_git **وموندل_pack,
				   off_t *وموندل_دننه_واټن,
				   uint32_t وموندل_mtime)
{
	off_t دننه_واټن;

	إذا (p == *وموندل_pack)
		دننه_واټن = *وموندل_دننه_واټن;
	وإلا
		دننه_واټن = پیدا_کړه_pack_ثبت_one(oid, p);

	إذا (دننه_واټن) {
		إذا (!*وموندل_pack) {
			إذا (!is_pack_valid(p))
				أرجع -1;
			*وموندل_دننه_واټن = دننه_واټن;
			*وموندل_pack = p;
		}
		أرجع want_وموندل_object(oid, exclude, p, وموندل_mtime);
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
ساكن صحيح want_object_دننه_pack_mtime(ثابت بنية object_id *oid,
				     صحيح exclude,
				     بنية packed_git **وموندل_pack,
				     off_t *وموندل_دننه_واټن,
				     uint32_t وموندل_mtime)
{
	بنية odb_سرچینه_files *files =
		odb_سرچینه_files_downcast(the_repository->objects->سرچینه);
	صحيح want;
	بنية packfile_list_ثبت *e;

	إذا (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		لكل (بنية odb_files_dir *dir = files->dirs->بل; dir; dir = dir->بل)
			إذا (!odb_سرچینه_read_object_info(&dir->loose->بنسټ, oid, عدم, 0, عدم))
				أرجع 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	إذا (*وموندل_pack) {
		want = want_وموندل_object(oid, exclude, *وموندل_pack,
					 وموندل_mtime);
		إذا (want != -1)
			أرجع want;

		*وموندل_pack = عدم;
		*وموندل_دننه_واټن = 0;
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بل) {
		بنية multi_pack_شاخص *m = get_multi_pack_شاخص(dir->packed);
		بنية pack_ثبت e;

		إذا (m && midx_fill_ثبت(m, oid, &e, عدم) == MIDX_FILL_موندنه) {
			want = want_object_دننه_pack_one(e.p, oid, exclude, وموندل_pack, وموندل_دننه_واټن, وموندل_mtime);
			إذا (want != -1)
				أرجع want;
		}
	}

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بل) {
		لكل (e = dir->packed->packs.سر; e; e = e->بل) {
			بنية packed_git *p = e->pack;
			want = want_object_دننه_pack_one(p, oid, exclude, وموندل_pack, وموندل_دننه_واټن, وموندل_mtime);
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
				إذا (skip_مختاړی(ex->uri,
						uri_protocols.توکي[i].string,
						&p) &&
				    *p == ':') {
					oidset_ورننباسه(&excluded_by_config, oid);
					أرجع 0;
				}
			}
		}
	}

	أرجع 1;
}

ساكن مضمن صحيح want_object_دننه_pack(ثابت بنية object_id *oid,
				      صحيح exclude,
				      بنية packed_git **وموندل_pack,
				      off_t *وموندل_دننه_واټن)
{
	أرجع want_object_دننه_pack_mtime(oid, exclude, وموندل_pack, وموندل_دننه_واټن,
					 0);
}

ساكن بنية object_ثبت *create_object_ثبت(ثابت بنية object_id *oid,
						تعداد object_type type,
						uint32_t hash,
						صحيح exclude,
						صحيح no_try_delta,
						بنية packed_git *وموندل_pack,
						off_t وموندل_دننه_واټن)
{
	بنية object_ثبت *ثبت;

	ثبت = packlist_alloc(&منزل_pack, oid);
	ثبت->hash = hash;
	oe_سېټ_type(ثبت, type);
	إذا (exclude)
		ثبت->preferred_بنسټ = 1;
	وإلا
		nr_پایله++;
	إذا (وموندل_pack) {
		oe_سېټ_دننه_pack(&منزل_pack, ثبت, وموندل_pack);
		ثبت->دننه_pack_دننه_واټن = وموندل_دننه_واټن;
	}

	ثبت->no_try_delta = no_try_delta;

	أرجع ثبت;
}

ساكن ثابت حرف no_تړون_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

ساكن فراغ جمع_کړه_object_ثبت(ثابت بنية object_id *oid, تعداد object_type type,
			     ثابت حرف *name, صحيح exclude)
{
	بنية packed_git *وموندل_pack = عدم;
	off_t وموندل_دننه_واټن = 0;

	display_پرمختګ(پرمختګ_state, ++nr_کتل_شوی);

	إذا (have_duplicate_ثبت(oid, exclude))
		أرجع;

	إذا (!want_object_دننه_pack(oid, exclude, &وموندل_pack, &وموندل_دننه_واټن)) {
		/* The pack is missing an object, so it will not have closure */
		إذا (write_bitmap_شاخص) {
			إذا (write_bitmap_شاخص != WRITE_BITMAP_QUIET)
				warning(_(no_تړون_warning));
			write_bitmap_شاخص = 0;
		}
		أرجع;
	}

	create_object_ثبت(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    وموندل_pack, وموندل_دننه_واټن);
}

ساكن صحيح جمع_کړه_object_ثبت_from_bitmap(ثابت بنية object_id *oid,
					تعداد object_type type,
					صحيح flags ناکارول_شوی, uint32_t name_hash,
					بنية packed_git *pack, off_t دننه_واټن,
					فراغ *payload ناکارول_شوی)
{
	display_پرمختګ(پرمختګ_state, ++nr_کتل_شوی);

	إذا (have_duplicate_ثبت(oid, 0))
		أرجع 0;

	إذا (!want_object_دننه_pack(oid, 0, &pack, &دننه_واټن))
		أرجع 0;

	create_object_ثبت(oid, type, name_hash, 0, 0, pack, دننه_واټن);
	أرجع 0;
}

بنية pbase_ونه_cache {
	بنية object_id oid;
	صحيح ref;
	صحيح temporary;
	فراغ *ونه_معلومات;
	بینښه اوږد ونه_اندازه;
};

ساكن بنية pbase_ونه_cache *(pbase_ونه_cache[256]);
ساكن صحيح pbase_ونه_cache_ix(ثابت بنية object_id *oid)
{
	أرجع oid->hash[0] % ARRAY_اندازه(pbase_ونه_cache);
}
ساكن صحيح pbase_ونه_cache_ix_incr(صحيح ix)
{
	أرجع (ix+1) % ARRAY_اندازه(pbase_ونه_cache);
}

ساكن بنية pbase_ونه {
	بنية pbase_ونه *بل;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	بنية pbase_ونه_cache pcache;
} *pbase_ونه;

ساكن بنية pbase_ونه_cache *pbase_ونه_get(ثابت بنية object_id *oid)
{
	بنية pbase_ونه_cache *ent, *nent;
	فراغ *معلومات;
	بینښه اوږد اندازه;
	اندازه_t اندازه_st = 0;
	تعداد object_type type;
	صحيح neigh;
	صحيح my_ix = pbase_ونه_cache_ix(oid);
	صحيح شته_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	لكل (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_ونه_cache[my_ix];
		إذا (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			أرجع ent;
		}
		وإلا إذا (((شته_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= شته_ix) &&
			  (!ent && pbase_ونه_cache[شته_ix])))
			شته_ix = my_ix;
		إذا (!ent)
			اكسر;
		my_ix = pbase_ونه_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	معلومات = odb_read_object(the_repository->objects, oid, &type, &اندازه_st);
	اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
	إذا (!معلومات)
		أرجع عدم;
	إذا (type != OBJ_ونه) {
		حرر(معلومات);
		أرجع عدم;
	}

	/* We need to either cache or return a throwaway copy */

	إذا (شته_ix < 0)
		ent = عدم;
	وإلا {
		ent = pbase_ونه_cache[شته_ix];
		my_ix = شته_ix;
	}

	إذا (!ent) {
		nent = xmalloc(حجم(*nent));
		nent->temporary = (شته_ix < 0);
	}
	وإلا {
		/* evict and reuse */
		حرر(ent->ونه_معلومات);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->ونه_معلومات = معلومات;
	nent->ونه_اندازه = اندازه;
	nent->ref = 1;
	إذا (!nent->temporary)
		pbase_ونه_cache[my_ix] = nent;
	أرجع nent;
}

ساكن فراغ pbase_ونه_put(بنية pbase_ونه_cache *cache)
{
	إذا (!cache->temporary) {
		cache->ref--;
		أرجع;
	}
	حرر(cache->ونه_معلومات);
	حرر(cache);
}

ساكن اندازه_t name_cmp_len(ثابت حرف *name)
{
	أرجع strcspn(name, "\n/");
}

ساكن فراغ جمع_کړه_pbase_object(بنية ونه_desc *ونه,
			     ثابت حرف *name,
			     اندازه_t cmplen,
			     ثابت حرف *fullname)
{
	بنية name_ثبت ثبت;
	صحيح cmp;

	طالما (ونه_ثبت(ونه,&ثبت)) {
		إذا (S_ISGITLINK(ثبت.ډېرپېښ))
			استمر;
		cmp = ونه_ثبت_len(&ثبت) != cmplen ? 1 :
		      memcmp(name, ثبت.لار, cmplen);
		إذا (cmp > 0)
			استمر;
		إذا (cmp < 0)
			أرجع;
		إذا (name[cmplen] != '/') {
			جمع_کړه_object_ثبت(&ثبت.oid,
					 object_type(ثبت.ډېرپېښ),
					 fullname, 1);
			أرجع;
		}
		إذا (S_ISDIR(ثبت.ډېرپېښ)) {
			بنية ونه_desc sub;
			بنية pbase_ونه_cache *ونه;
			ثابت حرف *ښکته = name+cmplen+1;
			اندازه_t downlen = name_cmp_len(ښکته);

			ونه = pbase_ونه_get(&ثبت.oid);
			إذا (!ونه)
				أرجع;
			init_ونه_desc(&sub, &ونه->oid,
				       ونه->ونه_معلومات, ونه->ونه_اندازه);

			جمع_کړه_pbase_object(&sub, ښکته, downlen, fullname);
			pbase_ونه_put(ونه);
		}
	}
}

ساكن بینښه *بشپړ_pbase_paths;
ساكن صحيح بشپړ_pbase_paths_عدد;
ساكن صحيح بشپړ_pbase_paths_alloc;
ساكن صحيح بشپړ_pbase_لار_pos(بینښه hash)
{
	صحيح lo = 0;
	صحيح hi = بشپړ_pbase_paths_عدد;
	طالما (lo < hi) {
		صحيح mi = lo + (hi - lo) / 2;
		إذا (بشپړ_pbase_paths[mi] == hash)
			أرجع mi;
		إذا (بشپړ_pbase_paths[mi] < hash)
			hi = mi;
		وإلا
			lo = mi + 1;
	}
	أرجع -lo-1;
}

ساكن صحيح check_pbase_لار(بینښه hash)
{
	صحيح pos = بشپړ_pbase_لار_pos(hash);
	إذا (0 <= pos)
		أرجع 1;
	pos = -pos - 1;
	ALLOC_GROW(بشپړ_pbase_paths,
		   بشپړ_pbase_paths_عدد + 1,
		   بشپړ_pbase_paths_alloc);
	بشپړ_pbase_paths_عدد++;
	إذا (pos < بشپړ_pbase_paths_عدد)
		MOVE_ARRAY(بشپړ_pbase_paths + pos + 1, بشپړ_pbase_paths + pos,
			   بشپړ_pbase_paths_عدد - pos - 1);
	بشپړ_pbase_paths[pos] = hash;
	أرجع 0;
}

ساكن فراغ جمع_کړه_preferred_بنسټ_object(ثابت حرف *name)
{
	بنية pbase_ونه *it;
	اندازه_t cmplen;
	بینښه hash = pack_name_hash_fn(name);

	إذا (!عدد_preferred_بنسټ || check_pbase_لار(hash))
		أرجع;

	cmplen = name_cmp_len(name);
	لكل (it = pbase_ونه; it; it = it->بل) {
		إذا (cmplen == 0) {
			جمع_کړه_object_ثبت(&it->pcache.oid, OBJ_ونه, عدم, 1);
		}
		وإلا {
			بنية ونه_desc ونه;
			init_ونه_desc(&ونه, &it->pcache.oid,
				       it->pcache.ونه_معلومات, it->pcache.ونه_اندازه);
			جمع_کړه_pbase_object(&ونه, name, cmplen, name);
		}
	}
}

ساكن فراغ جمع_کړه_preferred_بنسټ(بنية object_id *oid)
{
	بنية pbase_ونه *it;
	فراغ *معلومات;
	بینښه اوږد اندازه;
	اندازه_t اندازه_st = 0;
	بنية object_id ونه_oid;

	إذا (کړکۍ <= عدد_preferred_بنسټ++)
		أرجع;

	معلومات = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_ونه, &اندازه_st, &ونه_oid);
	اندازه = cast_اندازه_t_منزل_ulong(اندازه_st);
	إذا (!معلومات)
		أرجع;

	لكل (it = pbase_ونه; it; it = it->بل) {
		إذا (oideq(&it->pcache.oid, &ونه_oid)) {
			حرر(معلومات);
			أرجع;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->بل = pbase_ونه;
	pbase_ونه = it;

	oidcpy(&it->pcache.oid, &ونه_oid);
	it->pcache.ونه_معلومات = معلومات;
	it->pcache.ونه_اندازه = اندازه;
}

ساكن فراغ cleanup_preferred_بنسټ(فراغ)
{
	بنية pbase_ونه *it;
	بینښه i;

	it = pbase_ونه;
	pbase_ونه = عدم;
	طالما (it) {
		بنية pbase_ونه *tmp = it;
		it = tmp->بل;
		حرر(tmp->pcache.ونه_معلومات);
		حرر(tmp);
	}

	لكل (i = 0; i < ARRAY_اندازه(pbase_ونه_cache); i++) {
		إذا (!pbase_ونه_cache[i])
			استمر;
		حرر(pbase_ونه_cache[i]->ونه_معلومات);
		FREE_AND_NULL(pbase_ونه_cache[i]);
	}

	FREE_AND_NULL(بشپړ_pbase_paths);
	بشپړ_pbase_paths_عدد = بشپړ_pbase_paths_alloc = 0;
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
ساكن صحيح can_reuse_delta(ثابت بنية object_id *بنسټ_oid,
			   بنية object_ثبت *delta,
			   بنية object_ثبت **بنسټ_بهر)
{
	بنية object_ثبت *بنسټ;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	بنسټ = packlist_پیدا_کړه(&منزل_pack, بنسټ_oid);
	إذا (بنسټ) {
		إذا (!دننه_same_island(&delta->idx.oid, &بنسټ->idx.oid))
			أرجع 0;
		*بنسټ_بهر = بنسټ;
		أرجع 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	إذا (thin && bitmap_has_oid_دننه_uninteresting(bitmap_git, بنسټ_oid)) {
		إذا (use_delta_islands) {
			إذا (!دننه_same_island(&delta->idx.oid, بنسټ_oid))
				أرجع 0;
		}
		*بنسټ_بهر = عدم;
		أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ prefetch_منزل_pack(uint32_t object_شاخص_پیل) {
	بنية oid_array منزل_fetch = OID_ARRAY_INIT;
	uint32_t i;

	لكل (i = object_شاخص_پیل; i < منزل_pack.nr_objects; i++) {
		بنية object_ثبت *ثبت = منزل_pack.objects + i;

		إذا (!odb_read_object_info_extended(the_repository->objects,
						   &ثبت->idx.oid,
						   عدم,
						   OBJECT_INFO_FOR_PREFETCH))
			استمر;
		oid_array_append(&منزل_fetch, &ثبت->idx.oid);
	}
	promisor_remote_get_مستقیم(the_repository,
				   منزل_fetch.oid, منزل_fetch.nr);
	oid_array_clear(&منزل_fetch);
}

ساكن فراغ check_object(بنية object_ثبت *ثبت, uint32_t object_شاخص)
{
	اندازه_t canonical_اندازه;
	تعداد object_type type;
	بنية object_info oi = {.typep = &type, .sizep = &canonical_اندازه};

	إذا (دننه_PACK(ثبت)) {
		بنية packed_git *p = دننه_PACK(ثبت);
		بنية pack_کړکۍ *w_curs = عدم;
		صحيح have_بنسټ = 0;
		بنية object_id بنسټ_ref;
		بنية object_ثبت *بنسټ_ثبت;
		اندازه_t کارول_شوی_2, کارول_شوی_0, avail;
		off_t ofs;
		بینښه حرف *buf, c;
		تعداد object_type type;
		اندازه_t دننه_pack_اندازه;

		buf = use_pack(p, &w_curs, ثبت->دننه_pack_دننه_واټن, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		کارول_شوی_2 = unpack_object_سرلیک_بفر(buf, avail,
						   &type,
						   &دننه_pack_اندازه);
		إذا (کارول_شوی_2 == 0)
			اذهب give_پورته;

		إذا (type < 0)
			BUG("invalid type %d", type);
		ثبت->دننه_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		بدل (ثبت->دننه_pack_type) {
		افتراضي:
			/* Not a delta hence we've already got all we need. */
			oe_سېټ_type(ثبت, ثبت->دننه_pack_type);
			سېټ_اندازه(ثبت, دننه_pack_اندازه);
			ثبت->دننه_pack_سرلیک_اندازه = کارول_شوی_2;
			إذا (oe_type(ثبت) < OBJ_COMMIT || oe_type(ثبت) > OBJ_BLOB)
				اذهب give_پورته;
			unuse_pack(&w_curs);
			أرجع;
		حالة OBJ_REF_DELTA:
			إذا (reuse_delta && !ثبت->preferred_بنسټ) {
				oidread(&بنسټ_ref,
					use_pack(p, &w_curs,
						 ثبت->دننه_pack_دننه_واټن + کارول_شوی_2,
						 عدم),
					the_repository->hash_algo);
				have_بنسټ = 1;
			}
			ثبت->دننه_pack_سرلیک_اندازه = کارول_شوی_2 + the_hash_algo->rawsz;
			اكسر;
		حالة OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       ثبت->دننه_pack_دننه_واټن + کارول_شوی_2, عدم);
			کارول_شوی_0 = 0;
			c = buf[کارول_شوی_0++];
			ofs = c & 127;
			طالما (c & 128) {
				ofs += 1;
				إذا (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_منزل_hex(&ثبت->idx.oid));
					اذهب give_پورته;
				}
				c = buf[کارول_شوی_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = ثبت->دننه_pack_دننه_واټن - ofs;
			إذا (ofs <= 0 || ofs >= ثبت->دننه_pack_دننه_واټن) {
				error(_("delta base offset out of bound for %s"),
				      oid_منزل_hex(&ثبت->idx.oid));
				اذهب give_پورته;
			}
			إذا (reuse_delta && !ثبت->preferred_بنسټ) {
				uint32_t pos;
				إذا (دننه_واټن_منزل_pack_pos(p, ofs, &pos) < 0)
					اذهب give_پورته;
				إذا (!nth_packed_object_id(&بنسټ_ref, p,
							  pack_pos_منزل_شاخص(p, pos)))
					have_بنسټ = 1;
			}
			ثبت->دننه_pack_سرلیک_اندازه = کارول_شوی_2 + کارول_شوی_0;
			اكسر;
		}

		إذا (have_بنسټ &&
		    can_reuse_delta(&بنسټ_ref, ثبت, &بنسټ_ثبت)) {
			oe_سېټ_type(ثبت, ثبت->دننه_pack_type);
			سېټ_اندازه(ثبت, دننه_pack_اندازه); /* delta size */
			سېټ_DELTA_اندازه(ثبت, دننه_pack_اندازه);

			إذا (بنسټ_ثبت) {
				سېټ_DELTA(ثبت, بنسټ_ثبت);
				ثبت->delta_sibling_idx = بنسټ_ثبت->delta_ماشوم_idx;
				سېټ_DELTA_ماشوم(بنسټ_ثبت, ثبت);
			} وإلا {
				سېټ_DELTA_EXT(ثبت, &بنسټ_ref);
			}

			unuse_pack(&w_curs);
			أرجع;
		}

		إذا (oe_type(ثبت)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = ثبت->دننه_pack_دننه_واټن + ثبت->دننه_pack_سرلیک_اندازه;
			canonical_اندازه = get_اندازه_from_delta(p, &w_curs,
							     delta_pos);
			إذا (canonical_اندازه == 0)
				اذهب give_پورته;
			سېټ_اندازه(ثبت, canonical_اندازه);
			unuse_pack(&w_curs);
			أرجع;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_پورته:
		unuse_pack(&w_curs);
	}

	إذا (odb_read_object_info_extended(the_repository->objects, &ثبت->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		إذا (repo_has_promisor_remote(the_repository)) {
			prefetch_منزل_pack(object_شاخص);
			إذا (odb_read_object_info_extended(the_repository->objects, &ثبت->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} وإلا {
			type = -1;
		}
	}
	oe_سېټ_type(ثبت, type);
	إذا (ثبت->type_valid) {
		سېټ_اندازه(ثبت, canonical_اندازه);
	} وإلا {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

ساكن صحيح pack_دننه_واټن_ترتیبول(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_ثبت *a = *(بنية object_ثبت **)_a;
	ثابت بنية object_ثبت *b = *(بنية object_ثبت **)_b;
	ثابت بنية packed_git *a_دننه_pack = دننه_PACK(a);
	ثابت بنية packed_git *b_دننه_pack = دننه_PACK(b);

	/* avoid filesystem trashing with loose objects */
	إذا (!a_دننه_pack && !b_دننه_pack)
		أرجع oidcmp(&a->idx.oid, &b->idx.oid);

	إذا (a_دننه_pack < b_دننه_pack)
		أرجع -1;
	إذا (a_دننه_pack > b_دننه_pack)
		أرجع 1;
	أرجع a->دننه_pack_دننه_واټن < b->دننه_pack_دننه_واټن ? -1 :
			(a->دننه_pack_دننه_واټن > b->دننه_pack_دننه_واټن);
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
ساكن فراغ drop_reused_delta(بنية object_ثبت *ثبت)
{
	بینښه *idx = &منزل_pack.objects[ثبت->delta_idx - 1].delta_ماشوم_idx;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type;
	اندازه_t اندازه;

	طالما (*idx) {
		بنية object_ثبت *oe = &منزل_pack.objects[*idx - 1];

		إذا (oe == ثبت)
			*idx = oe->delta_sibling_idx;
		وإلا
			idx = &oe->delta_sibling_idx;
	}
	سېټ_DELTA(ثبت, عدم);
	ثبت->ژوروالی = 0;

	oi.sizep = &اندازه;
	oi.typep = &type;
	إذا (packed_object_info(عدم, دننه_PACK(ثبت), ثبت->دننه_pack_دننه_واټن, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_سېټ_type(ثبت,
			    odb_read_object_info(the_repository->objects,
						 &ثبت->idx.oid, &اندازه));
	} وإلا {
		oe_سېټ_type(ثبت, type);
	}
	سېټ_اندازه(ثبت, اندازه);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
ساكن فراغ break_delta_chains(بنية object_ثبت *ثبت)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t ټول_ژوروالی;
	بنية object_ثبت *cur, *بل;

	لكل (cur = ثبت, ټول_ژوروالی = 0;
	     cur;
	     cur = DELTA(cur), ټول_ژوروالی++) {
		إذا (cur->dfs_state == DFS_بشپړ) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			ټول_ژوروالی += cur->ژوروالی;
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
			cur->dfs_state = DFS_بشپړ;
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
			cur->dfs_state = DFS_بشپړ;
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
	لكل (cur = ثبت; cur; cur = بل) {
		بل = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		إذا (cur->dfs_state == DFS_بشپړ)
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
		cur->ژوروالی = (ټول_ژوروالی--) % (ژوروالی + 1);
		إذا (!cur->ژوروالی)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_بشپړ;
	}
}

ساكن فراغ get_object_details(فراغ)
{
	uint32_t i;
	بنية object_ثبت **sorted_by_دننه_واټن;

	إذا (پرمختګ)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Counting objects"),
						منزل_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_دننه_واټن, منزل_pack.nr_objects);
	لكل (i = 0; i < منزل_pack.nr_objects; i++)
		sorted_by_دننه_واټن[i] = منزل_pack.objects + i;
	QSORT(sorted_by_دننه_واټن, منزل_pack.nr_objects, pack_دننه_واټن_ترتیبول);

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		بنية object_ثبت *ثبت = sorted_by_دننه_واټن[i];
		check_object(ثبت, i);
		إذا (ثبت->type_valid &&
		    oe_اندازه_greater_than(&منزل_pack, ثبت,
					 repo_settings_get_big_file_پوله(the_repository)))
			ثبت->no_try_delta = 1;
		display_پرمختګ(پرمختګ_state, i + 1);
	}
	stop_پرمختګ(&پرمختګ_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	لكل (i = 0; i < منزل_pack.nr_objects; i++)
		break_delta_chains(&منزل_pack.objects[i]);

	حرر(sorted_by_دننه_واټن);
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
ساكن صحيح type_اندازه_ترتیبول(ثابت فراغ *_a, ثابت فراغ *_b)
{
	ثابت بنية object_ثبت *a = *(بنية object_ثبت **)_a;
	ثابت بنية object_ثبت *b = *(بنية object_ثبت **)_b;
	ثابت تعداد object_type a_type = oe_type(a);
	ثابت تعداد object_type b_type = oe_type(b);
	ثابت بینښه اوږد a_اندازه = اندازه_3(a);
	ثابت بینښه اوږد b_اندازه = اندازه_3(b);

	إذا (a_type > b_type)
		أرجع -1;
	إذا (a_type < b_type)
		أرجع 1;
	إذا (a->hash > b->hash)
		أرجع -1;
	إذا (a->hash < b->hash)
		أرجع 1;
	إذا (a->preferred_بنسټ > b->preferred_بنسټ)
		أرجع -1;
	إذا (a->preferred_بنسټ < b->preferred_بنسټ)
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
	بنية object_ثبت *ثبت;
	فراغ *معلومات;
	بنية delta_شاخص *شاخص;
	بینښه ژوروالی;
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
#تعریف_کړه cache_قلف()		pthread_mutex_قلف(&cache_mutex)
#تعریف_کړه cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
ساكن pthread_mutex_t پرمختګ_mutex;
#تعریف_کړه پرمختګ_قلف()		pthread_mutex_قلف(&پرمختګ_mutex)
#تعریف_کړه پرمختګ_unlock()	pthread_mutex_unlock(&پرمختګ_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

ساكن مضمن صحيح oe_اندازه_less_than(بنية packing_معلومات *pack,
				    ثابت بنية object_ثبت *lhs,
				    اندازه_t rhs)
{
	إذا (lhs->اندازه_valid)
		أرجع lhs->اندازه_2 < rhs;
	إذا (rhs < pack->oe_اندازه_حد) /* rhs < 2^x <= lhs ? */
		أرجع 0;
	أرجع oe_get_اندازه_slow(pack, lhs) < rhs;
}

ساكن مضمن فراغ oe_سېټ_ونه_ژوروالی(بنية packing_معلومات *pack,
				     بنية object_ثبت *e,
				     بینښه صحيح ونه_ژوروالی)
{
	إذا (!pack->ونه_ژوروالی)
		CALLOC_ARRAY(pack->ونه_ژوروالی, pack->nr_alloc);
	pack->ونه_ژوروالی[e - pack->objects] = ونه_ژوروالی;
}

ساكن فراغ ریکارډ_ونه_ژوروالی(ثابت بنية object_id *oid, ثابت حرف *name)
{
	ثابت حرف *p;
	بینښه ژوروالی;
	بنية object_ثبت *ent;

	/* the empty string is a root tree, which is depth 0 */
	ژوروالی = *name ? 1 : 0;
	لكل (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		ژوروالی++;

	ent = packlist_پیدا_کړه(&منزل_pack, oid);
	إذا (ent && ژوروالی > oe_ونه_ژوروالی(&منزل_pack, ent))
		oe_سېټ_ونه_ژوروالی(&منزل_pack, ent, ژوروالی);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
اندازه_t oe_get_اندازه_slow(بنية packing_معلومات *pack,
			ثابت بنية object_ثبت *e)
{
	بنية packed_git *p;
	بنية pack_کړکۍ *w_curs;
	بینښه حرف *buf;
	تعداد object_type type;
	اندازه_t کارول_شوی_2, avail, اندازه;

	إذا (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		اندازه_t sz;
		packing_معلومات_قلف(&منزل_pack);
		إذا (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_منزل_hex(&e->idx.oid));
		packing_معلومات_unlock(&منزل_pack);
		أرجع sz;
	}

	p = oe_دننه_pack(pack, e);
	إذا (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_معلومات_قلف(&منزل_pack);
	w_curs = عدم;
	buf = use_pack(p, &w_curs, e->دننه_pack_دننه_واټن, &avail);
	کارول_شوی_2 = unpack_object_سرلیک_بفر(buf, avail, &type, &اندازه);
	إذا (کارول_شوی_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_منزل_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_معلومات_unlock(&منزل_pack);
	أرجع اندازه;
}

ساكن صحيح try_delta(بنية unpacked *trg, بنية unpacked *src,
		     بینښه max_ژوروالی, اندازه_t *mem_usage)
{
	بنية object_ثبت *trg_ثبت = trg->ثبت;
	بنية object_ثبت *src_ثبت = src->ثبت;
	بینښه اوږد trg_اندازه, src_اندازه, sizediff, max_اندازه, sz;
	اندازه_t delta_اندازه_2;
	بینښه ref_ژوروالی;
	تعداد object_type type;
	فراغ *delta_buf;

	/* Don't bother doing diffs between different types */
	إذا (oe_type(trg_ثبت) != oe_type(src_ثبت))
		أرجع -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	إذا (reuse_delta && دننه_PACK(trg_ثبت) &&
	    دننه_PACK(trg_ثبت) == دننه_PACK(src_ثبت) &&
	    !src_ثبت->preferred_بنسټ &&
	    trg_ثبت->دننه_pack_type != OBJ_REF_DELTA &&
	    trg_ثبت->دننه_pack_type != OBJ_OFS_DELTA)
		أرجع 0;

	/* Let's not bust the allowed depth. */
	إذا (src->ژوروالی >= max_ژوروالی)
		أرجع 0;

	/* Now some size filtering heuristics. */
	trg_اندازه = اندازه_3(trg_ثبت);
	إذا (!DELTA(trg_ثبت)) {
		max_اندازه = trg_اندازه/2 - the_hash_algo->rawsz;
		ref_ژوروالی = 1;
	} وإلا {
		max_اندازه = DELTA_اندازه(trg_ثبت);
		ref_ژوروالی = trg->ژوروالی;
	}
	max_اندازه = (uint64_t)max_اندازه * (max_ژوروالی - src->ژوروالی) /
						(max_ژوروالی - ref_ژوروالی + 1);
	إذا (max_اندازه == 0)
		أرجع 0;
	src_اندازه = اندازه_3(src_ثبت);
	sizediff = src_اندازه < trg_اندازه ? trg_اندازه - src_اندازه : 0;
	إذا (sizediff >= max_اندازه)
		أرجع 0;
	إذا (trg_اندازه < src_اندازه / 32)
		أرجع 0;

	إذا (!دننه_same_island(&trg->ثبت->idx.oid, &src->ثبت->idx.oid))
		أرجع 0;

	/* Load data if not already done */
	إذا (!trg->معلومات) {
		اندازه_t sz_st = 0;
		packing_معلومات_قلف(&منزل_pack);
		trg->معلومات = odb_read_object(the_repository->objects,
					    &trg_ثبت->idx.oid, &type,
					    &sz_st);
		sz = cast_اندازه_t_منزل_ulong(sz_st);
		packing_معلومات_unlock(&منزل_pack);
		إذا (!trg->معلومات)
			die(_("object %s cannot be read"),
			    oid_منزل_hex(&trg_ثبت->idx.oid));
		إذا (sz != trg_اندازه)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_منزل_hex(&trg_ثبت->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_اندازه);
		*mem_usage += sz;
	}
	إذا (!src->معلومات) {
		اندازه_t sz_st = 0;
		packing_معلومات_قلف(&منزل_pack);
		src->معلومات = odb_read_object(the_repository->objects,
					    &src_ثبت->idx.oid, &type,
					    &sz_st);
		sz = cast_اندازه_t_منزل_ulong(sz_st);
		packing_معلومات_unlock(&منزل_pack);
		إذا (!src->معلومات) {
			إذا (src_ثبت->preferred_بنسټ) {
				ساكن صحيح warned = 0;
				إذا (!warned++)
					warning(_("object %s cannot be read"),
						oid_منزل_hex(&src_ثبت->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				أرجع 0;
			}
			die(_("object %s cannot be read"),
			    oid_منزل_hex(&src_ثبت->idx.oid));
		}
		إذا (sz != src_اندازه)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_منزل_hex(&src_ثبت->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_اندازه);
		*mem_usage += sz;
	}
	إذا (!src->شاخص) {
		src->شاخص = create_delta_شاخص(src->معلومات, src_اندازه);
		إذا (!src->شاخص) {
			ساكن صحيح warned = 0;
			إذا (!warned++)
				warning(_("suboptimal pack - out of memory"));
			أرجع 0;
		}
		*mem_usage += sizeof_delta_شاخص(src->شاخص);
	}

	delta_buf = create_delta(src->شاخص, trg->معلومات, trg_اندازه, &delta_اندازه_2, max_اندازه);
	إذا (!delta_buf)
		أرجع 0;

	إذا (DELTA(trg_ثبت)) {
		/* Prefer only shallower same-sized deltas. */
		إذا (delta_اندازه_2 == DELTA_اندازه(trg_ثبت) &&
		    src->ژوروالی + 1 >= trg->ژوروالی) {
			حرر(delta_buf);
			أرجع 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	حرر(trg_ثبت->delta_معلومات);
	cache_قلف();
	إذا (trg_ثبت->delta_معلومات) {
		delta_cache_اندازه -= DELTA_اندازه(trg_ثبت);
		trg_ثبت->delta_معلومات = عدم;
	}
	إذا (delta_cacheable(src_اندازه, trg_اندازه, delta_اندازه_2)) {
		delta_cache_اندازه += delta_اندازه_2;
		cache_unlock();
		trg_ثبت->delta_معلومات = xrealloc(delta_buf, delta_اندازه_2);
	} وإلا {
		cache_unlock();
		حرر(delta_buf);
	}

	سېټ_DELTA(trg_ثبت, src_ثبت);
	سېټ_DELTA_اندازه(trg_ثبت, delta_اندازه_2);
	trg->ژوروالی = src->ژوروالی + 1;

	أرجع 1;
}

ساكن بینښه صحيح check_delta_حد(بنية object_ثبت *me, بینښه صحيح n)
{
	بنية object_ثبت *ماشوم = DELTA_ماشوم(me);
	بینښه صحيح m = n;
	طالما (ماشوم) {
		ثابت بینښه صحيح c = check_delta_حد(ماشوم, n + 1);
		إذا (m < c)
			m = c;
		ماشوم = DELTA_SIBLING(ماشوم);
	}
	أرجع m;
}

ساكن اندازه_t free_unpacked(بنية unpacked *n)
{
	اندازه_t freed_mem = sizeof_delta_شاخص(n->شاخص);
	free_delta_شاخص(n->شاخص);
	n->شاخص = عدم;
	إذا (n->معلومات) {
		freed_mem += اندازه_3(n->ثبت);
		FREE_AND_NULL(n->معلومات);
	}
	n->ثبت = عدم;
	n->ژوروالی = 0;
	أرجع freed_mem;
}

ساكن فراغ پیدا_کړه_deltas(بنية object_ثبت **list, بینښه *list_اندازه,
			صحيح کړکۍ, صحيح ژوروالی, بینښه *processed)
{
	uint32_t i, idx = 0, شمېر = 0;
	بنية unpacked *array;
	اندازه_t mem_usage = 0;

	CALLOC_ARRAY(array, کړکۍ);

	لكل (;;) {
		بنية object_ثبت *ثبت;
		بنية unpacked *n = array + idx;
		صحيح j, max_ژوروالی, غوره_بنسټ = -1;

		پرمختګ_قلف();
		إذا (!*list_اندازه) {
			پرمختګ_unlock();
			اكسر;
		}
		ثبت = *list++;
		(*list_اندازه)--;
		إذا (!ثبت->preferred_بنسټ) {
			(*processed)++;
			display_پرمختګ(پرمختګ_state, *processed);
		}
		پرمختګ_unlock();

		mem_usage -= free_unpacked(n);
		n->ثبت = ثبت;

		طالما (کړکۍ_حافظه_حد &&
		       mem_usage > کړکۍ_حافظه_حد &&
		       شمېر > 1) {
			ثابت uint32_t لکۍ = (idx + کړکۍ - شمېر) % کړکۍ;
			mem_usage -= free_unpacked(array + لکۍ);
			شمېر--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		إذا (ثبت->preferred_بنسټ)
			اذهب بل;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_ژوروالی = ژوروالی;
		إذا (DELTA_ماشوم(ثبت)) {
			max_ژوروالی -= check_delta_حد(ثبت, 0);
			إذا (max_ژوروالی <= 0)
				اذهب بل;
		}

		j = کړکۍ;
		طالما (--j > 0) {
			صحيح ret;
			uint32_t other_idx = idx + j;
			بنية unpacked *m;
			إذا (other_idx >= کړکۍ)
				other_idx -= کړکۍ;
			m = array + other_idx;
			إذا (!m->ثبت)
				اكسر;
			ret = try_delta(n, m, max_ژوروالی, &mem_usage);
			إذا (ret < 0)
				اكسر;
			وإلا إذا (ret > 0)
				غوره_بنسټ = other_idx;
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
		إذا (ثبت->delta_معلومات && !pack_منزل_stdout) {
			بینښه اوږد اندازه;

			اندازه = do_compress(&ثبت->delta_معلومات, DELTA_اندازه(ثبت));
			إذا (اندازه < (1U << OE_Z_DELTA_بټونه)) {
				ثبت->z_delta_اندازه = اندازه;
				cache_قلف();
				delta_cache_اندازه -= DELTA_اندازه(ثبت);
				delta_cache_اندازه += ثبت->z_delta_اندازه;
				cache_unlock();
			} وإلا {
				FREE_AND_NULL(ثبت->delta_معلومات);
				ثبت->z_delta_اندازه = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		إذا (DELTA(ثبت) && max_ژوروالی <= n->ژوروالی)
			استمر;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		إذا (DELTA(ثبت)) {
			بنية unpacked بدلون_2 = array[غوره_بنسټ];
			صحيح dist = (کړکۍ + idx - غوره_بنسټ) % کړکۍ;
			صحيح dst = غوره_بنسټ;
			طالما (dist--) {
				صحيح src = (dst + 1) % کړکۍ;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = بدلون_2;
		}

		بل:
		idx++;
		إذا (شمېر + 1 < کړکۍ)
			شمېر++;
		إذا (idx >= کړکۍ)
			idx = 0;
	}

	لكل (i = 0; i < کړکۍ; ++i) {
		free_delta_شاخص(array[i].شاخص);
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
	بنية object_ثبت **list;
	بنية packing_region *regions;
	بینښه list_اندازه;
	بینښه پاتې_شوی;
	صحيح کړکۍ;
	صحيح ژوروالی;
	صحيح working;
	صحيح معلومات_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	بینښه *processed;
};

ساكن pthread_cond_t پرمختګ_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساكن فراغ init_threaded_پلټنه(فراغ)
{
	pthread_mutex_init(&cache_mutex, عدم);
	pthread_mutex_init(&پرمختګ_mutex, عدم);
	pthread_cond_init(&پرمختګ_cond, عدم);
}

ساكن فراغ cleanup_threaded_پلټنه(فراغ)
{
	pthread_cond_destroy(&پرمختګ_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&پرمختګ_mutex);
}

ساكن فراغ *threaded_پیدا_کړه_deltas(فراغ *arg)
{
	بنية thread_params *me = arg;

	پرمختګ_قلف();
	طالما (me->پاتې_شوی) {
		پرمختګ_unlock();

		پیدا_کړه_deltas(me->list, &me->پاتې_شوی,
			    me->کړکۍ, me->ژوروالی, me->processed);

		پرمختګ_قلف();
		me->working = 0;
		pthread_cond_signal(&پرمختګ_cond);
		پرمختګ_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_قلف(&me->mutex);
		طالما (!me->معلومات_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->معلومات_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		پرمختګ_قلف();
	}
	پرمختګ_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_پیدا_کړه_deltas(بنية object_ثبت **list, بینښه list_اندازه,
			   صحيح کړکۍ, صحيح ژوروالی, بینښه *processed)
{
	بنية thread_params *p;
	صحيح i, ret, فعال_تارونه = 0;

	init_threaded_پلټنه();

	إذا (delta_پلټنه_تارونه <= 1) {
		پیدا_کړه_deltas(list, &list_اندازه, کړکۍ, ژوروالی, processed);
		cleanup_threaded_پلټنه();
		أرجع;
	}
	إذا (پرمختګ > pack_منزل_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_پلټنه_تارونه);
	CALLOC_ARRAY(p, delta_پلټنه_تارونه);

	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_پلټنه_تارونه; i++) {
		بینښه sub_اندازه = list_اندازه / (delta_پلټنه_تارونه - i);

		/* don't use too small segments or no deltas will be found */
		إذا (sub_اندازه < 2*کړکۍ && i+1 < delta_پلټنه_تارونه)
			sub_اندازه = 0;

		p[i].کړکۍ = کړکۍ;
		p[i].ژوروالی = ژوروالی;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].معلومات_ready = 0;

		/* try to split chunks on "path" boundaries */
		طالما (sub_اندازه && sub_اندازه < list_اندازه &&
		       list[sub_اندازه]->hash &&
		       list[sub_اندازه]->hash == list[sub_اندازه-1]->hash)
			sub_اندازه++;

		p[i].list = list;
		p[i].list_اندازه = sub_اندازه;
		p[i].پاتې_شوی = sub_اندازه;

		list += sub_اندازه;
		list_اندازه -= sub_اندازه;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_پلټنه_تارونه; i++) {
		إذا (!p[i].list_اندازه)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_پیدا_کړه_deltas, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		فعال_تارونه++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (فعال_تارونه) {
		بنية thread_params *موخه = عدم;
		بنية thread_params *victim = عدم;
		بینښه sub_اندازه = 0;

		پرمختګ_قلف();
		لكل (;;) {
			لكل (i = 0; !موخه && i < delta_پلټنه_تارونه; i++)
				إذا (!p[i].working)
					موخه = &p[i];
			إذا (موخه)
				اكسر;
			pthread_cond_wait(&پرمختګ_cond, &پرمختګ_mutex);
		}

		لكل (i = 0; i < delta_پلټنه_تارونه; i++)
			إذا (p[i].پاتې_شوی > 2*کړکۍ &&
			    (!victim || victim->پاتې_شوی < p[i].پاتې_شوی))
				victim = &p[i];
		إذا (victim) {
			sub_اندازه = victim->پاتې_شوی / 2;
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
				sub_اندازه = victim->پاتې_شوی / 2;
				list -= sub_اندازه;
			}
			موخه->list = list;
			victim->list_اندازه -= sub_اندازه;
			victim->پاتې_شوی -= sub_اندازه;
		}
		موخه->list_اندازه = sub_اندازه;
		موخه->پاتې_شوی = sub_اندازه;
		موخه->working = 1;
		پرمختګ_unlock();

		pthread_mutex_قلف(&موخه->mutex);
		موخه->معلومات_ready = 1;
		pthread_cond_signal(&موخه->cond);
		pthread_mutex_unlock(&موخه->mutex);

		إذا (!sub_اندازه) {
			pthread_join(موخه->thread, عدم);
			pthread_cond_destroy(&موخه->cond);
			pthread_mutex_destroy(&موخه->mutex);
			فعال_تارونه--;
		}
	}
	cleanup_threaded_پلټنه();
	حرر(p);
}

ساكن صحيح obj_is_packed(ثابت بنية object_id *oid)
{
	أرجع packlist_پیدا_کړه(&منزل_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_لري(bitmap_git, reuse_packfile_bitmap, oid));
}

ساكن فراغ جمع_کړه_نښان_chain(ثابت بنية object_id *oid)
{
	بنية نښان *نښان;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	إذا (obj_is_packed(oid))
		أرجع;

	نښان = lookup_نښان(the_repository, oid);
	طالما (1) {
		إذا (!نښان || parse_نښان(the_repository, نښان) || !نښان->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_منزل_hex(oid));

		جمع_کړه_object_ثبت(&نښان->object.oid, OBJ_نښان, عدم, 0);

		إذا (نښان->tagged->type != OBJ_نښان)
			أرجع;

		نښان = (بنية نښان *)نښان->tagged;
	}
}

ساكن صحيح جمع_کړه_ref_نښان(ثابت بنية reference *ref, فراغ *cb_معلومات ناکارول_شوی)
{
	بنية object_id peeled;

	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		جمع_کړه_نښان_chain(ref->oid);
	أرجع 0;
}

ساكن صحيح should_attempt_deltas(بنية object_ثبت *ثبت)
{
	إذا (DELTA(ثبت))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		أرجع 0;

	إذا (!ثبت->type_valid ||
	    oe_اندازه_less_than(&منزل_pack, ثبت, 50))
		أرجع 0;

	إذا (ثبت->no_try_delta)
		أرجع 0;

	إذا (!ثبت->preferred_بنسټ) {
		إذا (oe_type(ثبت) < 0)
			die(_("unable to get type of object %s"),
				oid_منزل_hex(&ثبت->idx.oid));
	} وإلا إذا (oe_type(ثبت) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		أرجع 0;
	}

	أرجع 1;
}

ساكن فراغ پیدا_کړه_deltas_for_region(بنية object_ثبت *list,
				   بنية packing_region *region,
				   بینښه صحيح *processed)
{
	بنية object_ثبت **delta_list;
	بینښه صحيح delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	لكل (اندازه_t i = 0; i < region->nr; i++) {
		بنية object_ثبت *ثبت = list + region->پیل + i;
		إذا (should_attempt_deltas(ثبت))
			delta_list[delta_list_nr++] = ثبت;
	}

	QSORT(delta_list, delta_list_nr, type_اندازه_ترتیبول);
	پیدا_کړه_deltas(delta_list, &delta_list_nr, کړکۍ, ژوروالی, processed);
	حرر(delta_list);
}

ساكن فراغ پیدا_کړه_deltas_by_region(بنية object_ثبت *list,
				  بنية packing_region *regions,
				  اندازه_t پیل, اندازه_t nr)
{
	بینښه صحيح processed = 0;
	اندازه_t پرمختګ_nr;

	إذا (!nr)
		أرجع;

	پرمختګ_nr = regions[nr - 1].پیل + regions[nr - 1].nr;

	إذا (پرمختګ)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Compressing objects by path"),
						پرمختګ_nr);

	طالما (nr--)
		پیدا_کړه_deltas_for_region(list,
				       &regions[پیل++],
				       &processed);

	display_پرمختګ(پرمختګ_state, پرمختګ_nr);
	stop_پرمختګ(&پرمختګ_state);
}

ساكن فراغ *threaded_پیدا_کړه_deltas_by_لار(فراغ *arg)
{
	بنية thread_params *me = arg;

	پرمختګ_قلف();
	طالما (me->پاتې_شوی) {
		طالما (me->پاتې_شوی) {
			پرمختګ_unlock();
			پیدا_کړه_deltas_for_region(منزل_pack.objects,
					       me->regions,
					       me->processed);
			پرمختګ_قلف();
			me->پاتې_شوی--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&پرمختګ_cond);
		پرمختګ_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_قلف(&me->mutex);
		طالما (!me->معلومات_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->معلومات_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		پرمختګ_قلف();
	}
	پرمختګ_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	أرجع عدم;
}

ساكن فراغ ll_پیدا_کړه_deltas_by_region(بنية object_ثبت *list,
				     بنية packing_region *regions,
				     uint32_t پیل, uint32_t nr)
{
	بنية thread_params *p;
	صحيح i, ret, فعال_تارونه = 0;
	بینښه صحيح processed = 0;
	uint32_t پرمختګ_nr;
	init_threaded_پلټنه();

	إذا (!nr)
		أرجع;

	پرمختګ_nr =  regions[nr - 1].پیل + regions[nr - 1].nr;
	إذا (delta_پلټنه_تارونه <= 1) {
		پیدا_کړه_deltas_by_region(list, regions, پیل, nr);
		cleanup_threaded_پلټنه();
		أرجع;
	}

	إذا (پرمختګ > pack_منزل_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_پلټنه_تارونه),
			   delta_پلټنه_تارونه);
	CALLOC_ARRAY(p, delta_پلټنه_تارونه);

	إذا (پرمختګ)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Compressing objects by path"),
						پرمختګ_nr);
	/* Partition the work amongst work threads. */
	لكل (i = 0; i < delta_پلټنه_تارونه; i++) {
		بینښه sub_اندازه = nr / (delta_پلټنه_تارونه - i);

		p[i].کړکۍ = کړکۍ;
		p[i].ژوروالی = ژوروالی;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].معلومات_ready = 0;

		p[i].regions = regions;
		p[i].list_اندازه = sub_اندازه;
		p[i].پاتې_شوی = sub_اندازه;

		regions += sub_اندازه;
		nr -= sub_اندازه;
	}

	/* Start work threads. */
	لكل (i = 0; i < delta_پلټنه_تارونه; i++) {
		إذا (!p[i].list_اندازه)
			استمر;
		pthread_mutex_init(&p[i].mutex, عدم);
		pthread_cond_init(&p[i].cond, عدم);
		ret = pthread_create(&p[i].thread, عدم,
				     threaded_پیدا_کړه_deltas_by_لار, &p[i]);
		إذا (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		فعال_تارونه++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	طالما (فعال_تارونه) {
		بنية thread_params *موخه = عدم;
		بنية thread_params *victim = عدم;
		بینښه sub_اندازه = 0;

		پرمختګ_قلف();
		لكل (;;) {
			لكل (i = 0; !موخه && i < delta_پلټنه_تارونه; i++)
				إذا (!p[i].working)
					موخه = &p[i];
			إذا (موخه)
				اكسر;
			pthread_cond_wait(&پرمختګ_cond, &پرمختګ_mutex);
		}

		لكل (i = 0; i < delta_پلټنه_تارونه; i++)
			إذا (p[i].پاتې_شوی > 2*کړکۍ &&
			    (!victim || victim->پاتې_شوی < p[i].پاتې_شوی))
				victim = &p[i];
		إذا (victim) {
			sub_اندازه = victim->پاتې_شوی / 2;
			موخه->regions = victim->regions + victim->پاتې_شوی - sub_اندازه;
			victim->list_اندازه -= sub_اندازه;
			victim->پاتې_شوی -= sub_اندازه;
		}
		موخه->list_اندازه = sub_اندازه;
		موخه->پاتې_شوی = sub_اندازه;
		موخه->working = 1;
		پرمختګ_unlock();

		pthread_mutex_قلف(&موخه->mutex);
		موخه->معلومات_ready = 1;
		pthread_cond_signal(&موخه->cond);
		pthread_mutex_unlock(&موخه->mutex);

		إذا (!sub_اندازه) {
			pthread_join(موخه->thread, عدم);
			pthread_cond_destroy(&موخه->cond);
			pthread_mutex_destroy(&موخه->mutex);
			فعال_تارونه--;
		}
	}
	cleanup_threaded_پلټنه();
	حرر(p);

	display_پرمختګ(پرمختګ_state, پرمختګ_nr);
	stop_پرمختګ(&پرمختګ_state);
}

ساكن فراغ prepare_pack(صحيح کړکۍ, صحيح ژوروالی)
{
	بنية object_ثبت **delta_list;
	uint32_t i, nr_deltas;
	بینښه n;

	إذا (use_delta_islands)
		resolve_ونه_islands(the_repository, پرمختګ, &منزل_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	إذا (!pack_منزل_stdout)
		do_check_packed_object_دوراني_ازموینه = 1;

	إذا (!منزل_pack.nr_objects || !کړکۍ || !ژوروالی)
		أرجع;

	إذا (لار_walk)
		ll_پیدا_کړه_deltas_by_region(منزل_pack.objects, منزل_pack.regions,
					 0, منزل_pack.nr_regions);

	ALLOC_ARRAY(delta_list, منزل_pack.nr_objects);
	nr_deltas = n = 0;

	لكل (i = 0; i < منزل_pack.nr_objects; i++) {
		بنية object_ثبت *ثبت = منزل_pack.objects + i;

		إذا (!should_attempt_deltas(ثبت))
			استمر;

		إذا (!ثبت->preferred_بنسټ)
			nr_deltas++;

		delta_list[n++] = ثبت;
	}

	إذا (nr_deltas && n > 1) {
		بینښه nr_بشپړ = 0;

		إذا (پرمختګ)
			پرمختګ_state = پیل_پرمختګ(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_اندازه_ترتیبول);
		ll_پیدا_کړه_deltas(delta_list, n, کړکۍ+1, ژوروالی, &nr_بشپړ);
		stop_پرمختګ(&پرمختګ_state);
		إذا (nr_بشپړ != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	حرر(delta_list);
}

ساكن صحيح git_pack_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(k, "pack.window")) {
		کړکۍ = git_config_int(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.windowmemory")) {
		کړکۍ_حافظه_حد = git_config_ulong(k, v, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.depth")) {
		ژوروالی = git_config_int(k, v, ctx->kvi);
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
		delta_پلټنه_تارونه = git_config_int(k, v, ctx->kvi);
		إذا (delta_پلټنه_تارونه < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_پلټنه_تارونه);
		إذا (!HAVE_تارونه && delta_پلټنه_تارونه != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_پلټنه_تارونه = 0;
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
		ثابت حرف *oid_پای_ټکی, *pack_پای_ټکی;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		بنية object_id pack_hash;

		إذا (!v)
			أرجع config_error_nonbool(k);

		ex = xmalloc(حجم(*ex));
		إذا (parse_oid_hex(v, &ex->e.oid, &oid_پای_ټکی) ||
		    *oid_پای_ټکی != ' ' ||
		    parse_oid_hex(oid_پای_ټکی + 1, &pack_hash, &pack_پای_ټکی) ||
		    *pack_پای_ټکی != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		إذا (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_پای_ټکی - oid_پای_ټکی);
		حافظه_نقل(ex->pack_hash_hex, oid_پای_ټکی + 1, pack_پای_ټکی - oid_پای_ټکی - 1);
		ex->uri = xstrdup(pack_پای_ټکی + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	أرجع git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
ساكن صحيح stdin_packs_وموندل_nr;
ساكن صحيح stdin_packs_hints_nr;

ساكن صحيح جمع_کړه_object_ثبت_from_pack(ثابت بنية object_id *oid,
				      بنية packed_git *p,
				      uint32_t pos,
				      فراغ *معلومات_2)
{
	off_t ofs;
	بنية object_info oi = OBJECT_INFO_INIT;
	تعداد object_type type = OBJ_NONE;

	display_پرمختګ(پرمختګ_state, ++nr_کتل_شوی);

	إذا (have_duplicate_ثبت(oid, 0))
		أرجع 0;

	stdin_packs_وموندل_nr++;

	ofs = nth_packed_object_دننه_واټن(p, pos);

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
		جمع_کړه_pending_oid(revs, عدم, oid, 0);
	}

	إذا (!want_object_دننه_pack(oid, 0, &p, &ofs))
		أرجع 0;

	create_object_ثبت(oid, type, 0, 0, 0, p, ofs);

	أرجع 0;
}

ساكن فراغ show_object_pack_hint(بنية object *object, ثابت حرف *name,
				  فراغ *معلومات)
{
	تعداد stdin_packs_ډېرپېښ ډېرپېښ = *(تعداد stdin_packs_ډېرپېښ *)معلومات;
	إذا (ډېرپېښ == STDIN_PACKS_ډېرپېښ_FOLLOW) {
		إذا (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			أرجع;
		جمع_کړه_object_ثبت(&object->oid, object->type, name, 0);
	} وإلا {
		بنية object_ثبت *oe = packlist_پیدا_کړه(&منزل_pack, &object->oid);
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
	تعداد stdin_packs_ډېرپېښ ډېرپېښ = *(تعداد stdin_packs_ډېرپېښ *)معلومات;

	إذا (ډېرپېښ == STDIN_PACKS_ډېرپېښ_FOLLOW) {
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
تعداد stdin_pack_info_ډول {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

بنية stdin_pack_info {
	بنية packed_git *p;
	تعداد stdin_pack_info_ډول ډول;
};

ساكن صحيح pack_mtime_cmp(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية stdin_pack_info *a = ((ثابت بنية string_list_توکی*)_a)->util;
	بنية stdin_pack_info *b = ((ثابت بنية string_list_توکی*)_b)->util;

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

ساكن صحيح stdin_packs_include_check_obj(بنية object *obj, فراغ *معلومات ناکارول_شوی)
{
	أرجع !has_object_kept_pack(منزل_pack.repo, &obj->oid,
				     KEPT_PACK_دننه_CORE);
}

ساكن صحيح stdin_packs_include_check(بنية commit *commit, فراغ *معلومات)
{
	أرجع stdin_packs_include_check_obj((بنية object *)commit, معلومات);
}

ساكن فراغ stdin_packs_جمع_کړه_pack_entries(بنية strmap *packs,
					 بنية rev_info *revs)
{
	بنية string_list کلیګانې = STRING_LIST_INIT_NODUP;
	بنية string_list_توکی *توکی;
	بنية hashmap_iter iter;
	بنية strmap_ثبت *ثبت;

	strmap_for_each_ثبت(packs, &iter, ثبت) {
		بنية stdin_pack_info *info = ثبت->ارزښت;
		إذا (!info->p)
			die(_("could not find pack '%s'"), ثبت->کلی);

		string_list_append(&کلیګانې, ثبت->کلی)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(کلیګانې.توکي, کلیګانې.nr, pack_mtime_cmp);

	for_each_string_list_توکی(توکی, &کلیګانې) {
		بنية stdin_pack_info *info = توکی->util;

		إذا (info->ډول & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		إذا ((info->ډول & STDIN_PACK_INCLUDE) ||
		    (info->ډول & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_دننه_pack(info->p,
						جمع_کړه_object_ثبت_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ترتیب);
	}

	string_list_clear(&کلیګانې, 0);
}

ساكن فراغ stdin_packs_read_ننوت(بنية rev_info *revs,
				   تعداد stdin_packs_ډېرپېښ ډېرپېښ)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strmap packs = STRMAP_INIT;
	بنية packed_git *p;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		بنية stdin_pack_info *info;
		تعداد stdin_pack_info_ډول ډول = STDIN_PACK_INCLUDE;
		ثابت حرف *کلی = buf.buf;

		إذا (!*کلی)
			استمر;
		وإلا إذا (*کلی == '^')
			ډول = STDIN_PACK_EXCLUDE_CLOSED;
		وإلا إذا (*کلی == '!' && ډېرپېښ == STDIN_PACKS_ډېرپېښ_FOLLOW)
			ډول = STDIN_PACK_EXCLUDE_OPEN;

		إذا (ډول != STDIN_PACK_INCLUDE)
			کلی++;

		info = strmap_get(&packs, کلی);
		إذا (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, کلی, info);
		}

		info->ډول |= ډول;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		بنية stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		إذا (!info)
			استمر;

		إذا (info->ډول & STDIN_PACK_INCLUDE) {
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

		إذا (info->ډول & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_دننه_core = 1;
		}

		إذا (info->ډول & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_دننه_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_جمع_کړه_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

ساكن فراغ جمع_کړه_unreachable_loose_objects(بنية rev_info *revs);

ساكن فراغ read_stdin_packs(بنية repository *repo,
			     تعداد stdin_packs_ډېرپېښ ډېرپېښ, صحيح rev_list_unpacked)
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
	revs.keep_pack_cache_flags |= KEPT_PACK_دننه_CORE;
	revs.blob_objects = 1;
	revs.ونه_objects = 1;
	revs.نښان_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_دننه_core = 1;
	إذا (ډېرپېښ == STDIN_PACKS_ډېرپېښ_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_دننه_core_open = 1;
	}
	stdin_packs_read_ننوت(&revs, ډېرپېښ);
	إذا (rev_list_unpacked)
		جمع_کړه_unreachable_loose_objects(&revs);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &ډېرپېښ);

	release_revisions(&revs);

	trace2_معلومات_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_وموندل_nr);
	trace2_معلومات_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

ساكن فراغ جمع_کړه_cruft_object_ثبت(ثابت بنية object_id *oid, تعداد object_type type,
				   بنية packed_git *pack, off_t دننه_واټن,
				   ثابت حرف *name, uint32_t mtime)
{
	بنية object_ثبت *ثبت;

	display_پرمختګ(پرمختګ_state, ++nr_کتل_شوی);

	ثبت = packlist_پیدا_کړه(&منزل_pack, oid);
	إذا (ثبت) {
		إذا (name) {
			ثبت->hash = pack_name_hash_fn(name);
			ثبت->no_try_delta = no_try_delta(name);
		}
	} وإلا {
		إذا (!want_object_دننه_pack_mtime(oid, 0, &pack, &دننه_واټن, mtime))
			أرجع;
		إذا (!pack && type == OBJ_BLOB) {
			بنية odb_سرچینه_files *files =
				odb_سرچینه_files_downcast(the_repository->objects->سرچینه);
			صحيح وموندل_2 = 0;

			لكل (بنية odb_files_dir *dir = files->dirs; !وموندل_2 && dir; dir = dir->بل)
				إذا (!odb_سرچینه_read_object_info(&dir->loose->بنسټ, oid, عدم, 0, عدم))
					وموندل_2 = 1;

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
			إذا (!وموندل_2)
				أرجع;
		}

		ثبت = create_object_ثبت(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, دننه_واټن);
	}

	إذا (mtime > oe_cruft_mtime(&منزل_pack, ثبت))
		oe_سېټ_cruft_mtime(&منزل_pack, ثبت, mtime);
	أرجع;
}

ساكن فراغ show_cruft_object(بنية object *obj, ثابت حرف *name, فراغ *معلومات ناکارول_شوی)
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
	جمع_کړه_cruft_object_ثبت(&obj->oid, obj->type, عدم, 0, name, cruft_expiration);
}

ساكن فراغ show_cruft_commit(بنية commit *commit, فراغ *معلومات)
{
	show_cruft_object((بنية object*)commit, عدم, معلومات);
}

ساكن صحيح cruft_include_check_obj(بنية object *obj, فراغ *معلومات ناکارول_شوی)
{
	أرجع !has_object_kept_pack(منزل_pack.repo, &obj->oid, KEPT_PACK_دننه_CORE);
}

ساكن صحيح cruft_include_check(بنية commit *commit, فراغ *معلومات)
{
	أرجع cruft_include_check_obj((بنية object*)commit, معلومات);
}

ساكن فراغ سېټ_cruft_mtime(ثابت بنية object *object,
			    بنية packed_git *pack,
			    off_t دننه_واټن, وخت_t mtime)
{
	جمع_کړه_cruft_object_ثبت(&object->oid, object->type, pack, دننه_واټن, عدم,
			       mtime);
}

ساكن فراغ mark_pack_kept_دننه_core(بنية string_list *packs, بینښه keep)
{
	بنية string_list_توکی *توکی = عدم;
	for_each_string_list_توکی(توکی, packs) {
		بنية packed_git *p = توکی->util;
		إذا (!p)
			die(_("could not find pack '%s'"), توکی->string);
		إذا (p->is_cruft && keep)
			ignore_packed_keep_دننه_core_has_cruft = 1;
		p->pack_keep_دننه_core = keep;
	}
}

ساكن فراغ جمع_کړه_objects_دننه_unpacked_packs(فراغ);

ساكن فراغ enumerate_cruft_objects(فراغ)
{
	إذا (پرمختګ)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Enumerating cruft objects"), 0);

	جمع_کړه_objects_دننه_unpacked_packs();
	جمع_کړه_unreachable_loose_objects(عدم);

	stop_پرمختګ(&پرمختګ_state);
}

ساكن فراغ enumerate_and_traverse_cruft_objects(بنية string_list *fresh_packs)
{
	بنية packed_git *p;
	بنية rev_info revs;
	صحيح ret;

	repo_init_revisions(the_repository, &revs, عدم);

	revs.نښان_objects = 1;
	revs.ونه_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	إذا (پرمختګ)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = جمع_کړه_unseen_recent_objects_منزل_traversal(&revs, cruft_expiration,
						     سېټ_cruft_mtime, 1);
	stop_پرمختګ(&پرمختګ_state);

	إذا (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_دننه_core = 0;
	mark_pack_kept_دننه_core(fresh_packs, 1);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	إذا (پرمختګ)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Traversing cruft objects"), 0);
	nr_کتل_شوی = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, عدم);

	stop_پرمختګ(&پرمختګ_state);
	release_revisions(&revs);
}

ساكن فراغ read_cruft_objects(فراغ)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list discard_packs = STRING_LIST_INIT_DUP;
	بنية string_list fresh_packs = STRING_LIST_INIT_DUP;
	بنية packed_git *p;

	ignore_packed_keep_دننه_core = 1;

	طالما (strbuf_getline(&buf, stdin) != EOF) {
		إذا (!buf.len)
			استمر;

		إذا (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		وإلا
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_ترتیبول(&discard_packs);
	string_list_ترتیبول(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *pack_name = pack_basename(p);
		بنية string_list_توکی *توکی;

		توکی = string_list_lookup(&fresh_packs, pack_name);
		إذا (!توکی)
			توکی = string_list_lookup(&discard_packs, pack_name);

		إذا (توکی) {
			توکی->util = p;
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
			p->pack_keep_دننه_core = 1;
		}
	}

	mark_pack_kept_دننه_core(&fresh_packs, 1);
	mark_pack_kept_دننه_core(&discard_packs, 0);

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
	حرف کرښه[GIT_MAX_HEXSZ + 1 + لار_MAX + 2];
	بنية object_id oid;
	ثابت حرف *p;

	لكل (;;) {
		إذا (!fgets(کرښه, حجم(کرښه), stdin)) {
			إذا (feof(stdin))
				اكسر;
			إذا (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			إذا (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			استمر;
		}
		إذا (کرښه[0] == '-') {
			إذا (get_oid_hex(کرښه+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    کرښه);
			جمع_کړه_preferred_بنسټ(&oid);
			استمر;
		}
		إذا (parse_oid_hex(کرښه, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), کرښه);

		جمع_کړه_preferred_بنسټ_object(p + 1);
		جمع_کړه_object_ثبت(&oid, OBJ_NONE, p + 1, 0);
	}
}

ساكن فراغ show_commit(بنية commit *commit, فراغ *معلومات ناکارول_شوی)
{
	جمع_کړه_object_ثبت(&commit->object.oid, OBJ_COMMIT, عدم, 0);

	إذا (write_bitmap_شاخص)
		شاخص_commit_for_bitmap(commit);

	إذا (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

ساكن فراغ show_object(بنية object *obj, ثابت حرف *name,
			فراغ *معلومات ناکارول_شوی)
{
	جمع_کړه_preferred_بنسټ_object(name);
	جمع_کړه_object_ثبت(&obj->oid, obj->type, name, 0);

	إذا (use_delta_islands)
		ریکارډ_ونه_ژوروالی(&obj->oid, name);
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
	بنية repository *repo = opt->ارزښت;

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

ساكن فراغ show_څنډه(بنية commit *commit)
{
	جمع_کړه_preferred_بنسټ(&commit->object.oid);
}

ساكن صحيح جمع_کړه_object_دننه_unpacked_pack(ثابت بنية object_id *oid,
				       بنية object_info *oi,
				       فراغ *معلومات ناکارول_شوی)
{
	إذا (cruft) {
		جمع_کړه_cruft_object_ثبت(oid, OBJ_NONE, oi->سرچینه_infop->u.packed.pack,
				       oi->سرچینه_infop->u.packed.دننه_واټن, عدم,
				       *oi->mtimep);
	} وإلا {
		جمع_کړه_object_ثبت(oid, OBJ_NONE, "", 0);
	}
	أرجع 0;
}

ساكن فراغ جمع_کړه_objects_دننه_unpacked_packs(فراغ)
{
	بنية odb_سرچینه_files *files =
		odb_سرچینه_files_downcast(منزل_pack.repo->objects->سرچینه);
	وخت_t mtime;
	بنية odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ترتیب |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_دننه_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	بنية odb_سرچینه_info سرچینه_info;
	بنية object_info oi = {
		.mtimep = &mtime,
		.سرچینه_infop = &سرچینه_info,
	};

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بل) {
		إذا (!dir->local)
			استمر;

		إذا (odb_سرچینه_for_each_object(&dir->packed->بنسټ, &oi,
					       جمع_کړه_object_دننه_unpacked_pack, عدم, &opts))
			die(_("cannot open pack index"));
	}
}

ساكن صحيح جمع_کړه_loose_object(ثابت بنية object_id *oid, ثابت حرف *لار,
			    فراغ *معلومات)
{
	بنية rev_info *revs = معلومات;
	تعداد object_type type = odb_read_object_info(the_repository->objects, oid, عدم);

	إذا (type < 0) {
		warning(_("loose object at %s could not be examined"), لار);
		أرجع 0;
	}

	إذا (cruft) {
		بنية stat st;
		إذا (stat(لار, &st) < 0) {
			إذا (errno == ENOENT)
				أرجع 0;
			أرجع error_errno("unable to stat %s", oid_منزل_hex(oid));
		}

		جمع_کړه_cruft_object_ثبت(oid, type, عدم, 0, عدم,
				       st.st_mtime);
	} وإلا {
		جمع_کړه_object_ثبت(oid, type, "", 0);
	}

	إذا (revs && type == OBJ_COMMIT)
		جمع_کړه_pending_oid(revs, عدم, oid, 0);

	أرجع 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
ساكن فراغ جمع_کړه_unreachable_loose_objects(بنية rev_info *revs)
{
	for_each_loose_file_دننه_سرچینه(the_repository->objects->سرچینه,
				      جمع_کړه_loose_object, عدم, عدم, revs);
}

ساكن صحيح has_sha1_pack_kept_or_nonlocal(ثابت بنية object_id *oid)
{
	ساكن بنية packed_git *وروستی_وموندل = عدم;
	بنية packed_git *p;

	إذا (وروستی_وموندل && پیدا_کړه_pack_ثبت_one(oid, وروستی_وموندل))
		أرجع 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		إذا (p == وروستی_وموندل)
			استمر;

		إذا ((!p->pack_local || p->pack_keep || p->pack_keep_دننه_core) &&
		    پیدا_کړه_pack_ثبت_one(oid, p)) {
			وروستی_وموندل = p;
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

ساكن صحيح force_object_loose(بنية odb_سرچینه *سرچینه,
			      ثابت بنية object_id *oid,
			      ثابت وخت_t *mtime)
{
	بنية odb_سرچینه_files *files = odb_سرچینه_files_downcast(سرچینه);
	ثابت بنية git_hash_algo *compat = سرچینه->odb->repo->compat_hash_algo;
	بنية object_info oi = OBJECT_INFO_INIT;
	بنية object_id compat_oid, *compat_oid_p = عدم;
	تعداد object_type type;
	فراغ *buf = عدم;
	اندازه_t len;
	صحيح ret;

	لكل (بنية odb_files_dir *dir = files->dirs; dir; dir = dir->بل)
		إذا (!odb_سرچینه_read_object_info(&dir->loose->بنسټ, oid, عدم, 0, عدم))
			أرجع 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	إذا (odb_read_object_info_extended(سرچینه->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_منزل_hex(oid));
		اذهب بهر;
	}

	إذا (compat) {
		إذا (repo_oid_منزل_algop(سرچینه->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_منزل_hex(oid), compat->name);
			اذهب بهر;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_سرچینه_write_object(&files->dirs->loose->بنسټ, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

بهر:
	حرر(buf);
	أرجع ret;
}

ساكن فراغ loosen_ناکارول_شوی_packed_objects(فراغ)
{
	بنية packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	بنية object_id oid;

	repo_for_each_pack(the_repository, p) {
		إذا (!p->pack_local || p->pack_keep || p->pack_keep_دننه_core)
			استمر;

		إذا (open_pack_شاخص(p))
			die(_("cannot open pack index"));

		لكل (i = 0; i < p->عدد_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			إذا (!packlist_پیدا_کړه(&منزل_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				إذا (force_object_loose(the_repository->objects->سرچینه,
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
	       !ignore_packed_keep_دننه_core &&
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

		nr_پایله += reuse_packfile_objects;
		nr_کتل_شوی += reuse_packfile_objects;
		display_پرمختګ(پرمختګ_state, nr_کتل_شوی);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &جمع_کړه_object_ثبت_from_bitmap);
	أرجع 0;
}

ساكن فراغ ریکارډ_recent_object(بنية object *obj,
				 ثابت حرف *name ناکارول_شوی,
				 فراغ *معلومات ناکارول_شوی)
{
	oid_array_append(&recent_objects, &obj->oid);
}

ساكن فراغ ریکارډ_recent_commit(بنية commit *commit, فراغ *معلومات ناکارول_شوی)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

ساكن صحيح mark_bitmap_preferred_tip(ثابت بنية reference *ref, فراغ *معلومات ناکارول_شوی)
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

ساكن صحيح جمع_کړه_objects_by_لار(ثابت حرف *لار,
			       بنية oid_array *oids,
			       تعداد object_type type,
			       فراغ *معلومات)
{
	اندازه_t oe_پیل = منزل_pack.nr_objects;
	اندازه_t oe_پای_ټکی;
	بینښه صحيح *processed = معلومات;

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

		جمع_کړه_object_ثبت(oid, type, لار, exclude);

		إذا (type == OBJ_COMMIT) {
			بنية commit *commit;

			إذا (!write_bitmap_شاخص && !use_delta_islands)
				استمر;

			commit = lookup_commit(the_repository, oid);
			إذا (!commit)
				die(_("could not find commit %s"), oid_منزل_hex(oid));
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
		} وإلا إذا (type == OBJ_ونه && use_delta_islands) {
			ریکارډ_ونه_ژوروالی(oid, لار);
		}
	}

	oe_پای_ټکی = منزل_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	إذا (oe_پای_ټکی == oe_پیل || !کړکۍ)
		أرجع 0;

	ALLOC_GROW(منزل_pack.regions,
		   منزل_pack.nr_regions + 1,
		   منزل_pack.nr_regions_alloc);

	منزل_pack.regions[منزل_pack.nr_regions].پیل = oe_پیل;
	منزل_pack.regions[منزل_pack.nr_regions].nr = oe_پای_ټکی - oe_پیل;
	منزل_pack.nr_regions++;

	*processed += oids->nr;
	display_پرمختګ(پرمختګ_state, *processed);

	أرجع 0;
}

ساكن صحيح get_object_list_لار_walk(بنية rev_info *revs)
{
	بنية لار_walk_info info = لار_WALK_INFO_INIT;
	بینښه صحيح processed = 0;
	صحيح پایله;

	info.revs = revs;
	info.لار_fn = جمع_کړه_objects_by_لار;
	info.لار_fn_معلومات = &processed;

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
	info.څنډه_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	پایله = walk_objects_by_لار(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	لار_walk_info_clear(&info);

	أرجع پایله;
}

ساكن فراغ get_object_list(بنية rev_info *revs, بنية strvec *argv)
{
	بنية تیاری_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);
	حرف کرښه[1000];
	صحيح flags = 0;
	صحيح save_warning;

	save_commit_بفر = 0;
	تیاری_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	طالما (fgets(کرښه, حجم(کرښه), stdin) != عدم) {
		صحيح len = مزي_اوږدوالی(کرښه);
		إذا (len && کرښه[len - 1] == '\n')
			کرښه[--len] = 0;
		إذا (!len)
			اكسر;
		إذا (*کرښه == '-') {
			إذا (!strcmp(کرښه, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_شاخص = 0;
				استمر;
			}
			إذا (starts_with(کرښه, "--shallow ")) {
				بنية object_id oid;
				إذا (get_oid_hex(کرښه + 10, &oid))
					die("not an object name '%s'", کرښه + 10);
				ثبتوونکی_shallow(the_repository, &oid);
				use_bitmap_شاخص = 0;
				استمر;
			}
			die(_("not a rev '%s'"), کرښه);
		}
		إذا (handle_revision_arg(کرښه, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), کرښه);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	إذا (use_bitmap_شاخص && !get_object_list_from_bitmap(revs))
		أرجع;

	إذا (use_delta_islands)
		بارول_delta_islands(the_repository, پرمختګ);

	إذا (write_bitmap_شاخص)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      عدم);

	إذا (!fn_show_object)
		fn_show_object = show_object;

	إذا (لار_walk) {
		إذا (get_object_list_لار_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			لار_walk = 0;
		}
	}

	إذا (!لار_walk) {
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_څنډې_uninteresting(revs, show_څنډه, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				عدم);
	}

	إذا (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		إذا (جمع_کړه_unseen_recent_objects_منزل_traversal(revs,
				unpack_unreachable_expiration, عدم, 0))
			die(_("unable to add recent objects"));
		إذا (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, ریکارډ_recent_commit,
				     ریکارډ_recent_object, عدم);
	}

	إذا (keep_unreachable)
		جمع_کړه_objects_دننه_unpacked_packs();
	إذا (pack_loose_unreachable)
		جمع_کړه_unreachable_loose_objects(عدم);
	إذا (unpack_unreachable)
		loosen_ناکارول_شوی_packed_objects();

	oid_array_clear(&recent_objects);
}

ساكن فراغ جمع_کړه_extra_kept_packs(ثابت بنية string_list *نومونه)
{
	بنية packed_git *p;

	إذا (!نومونه->nr)
		أرجع;

	repo_for_each_pack(the_repository, p) {
		ثابت حرف *name = basename(p->pack_name);
		صحيح i;

		إذا (!p->pack_local)
			استمر;

		لكل (i = 0; i < نومونه->nr; i++)
			إذا (!fspathcmp(name, نومونه->توکي[i].string))
				اكسر;

		إذا (i < نومونه->nr) {
			p->pack_keep_دننه_core = 1;
			ignore_packed_keep_دننه_core = 1;
			استمر;
		}
	}
}

ساكن صحيح option_parse_quiet(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	صحيح *val = opt->ارزښت;

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
	بنية pack_idx_option *popts = opt->ارزښت;
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

ساكن صحيح option_parse_unpack_unreachable(ثابت بنية option *opt ناکارول_شوی,
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

ساكن صحيح option_parse_cruft_expiration(ثابت بنية option *opt ناکارول_شوی,
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

ساكن صحيح is_not_دننه_promisor_pack_obj(بنية object *obj, فراغ *معلومات ناکارول_شوی)
{
	بنية odb_سرچینه_info سرچینه_info;
	بنية object_info info = {
		.سرچینه_infop = &سرچینه_info,
	};

	إذا (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	أرجع سرچینه_info.سرچینه->type != ODB_سرچینه_PACKED || !سرچینه_info.u.packed.pack->pack_promisor;
}

ساكن صحيح is_not_دننه_promisor_pack(بنية commit *commit, فراغ *معلومات) {
	أرجع is_not_دننه_promisor_pack_obj((بنية object *) commit, معلومات);
}

ساكن صحيح parse_stdin_packs_ډېرپېښ(ثابت بنية option *opt, ثابت حرف *arg,
				  صحيح unset)
{
	تعداد stdin_packs_ډېرپېښ *ډېرپېښ = opt->ارزښت;

	إذا (unset)
		*ډېرپېښ = STDIN_PACKS_ډېرپېښ_NONE;
	وإلا إذا (!arg || !*arg)
		*ډېرپېښ = STDIN_PACKS_ډېرپېښ_STANDARD;
	وإلا إذا (!strcmp(arg, "follow"))
		*ډېرپېښ = STDIN_PACKS_ډېرپېښ_FOLLOW;
	وإلا
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	أرجع 0;
}

صحيح cmd_pack_objects(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *مختاړی,
		     بنية repository *repo)
{
	صحيح use_دننی_rev_list = 0;
	صحيح all_پرمختګ_implied = 0;
	بنية strvec rp = STRVEC_INIT;
	صحيح rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	صحيح rev_list_شاخص = 0;
	تعداد stdin_packs_ډېرپېښ stdin_packs = STDIN_PACKS_ډېرپېښ_NONE;
	بنية string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	بنية list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	بنية option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &پرمختګ, عدم,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_سېټ_INT(0, "progress", &پرمختګ,
			    N_("show progress meter"), 1),
		OPT_سېټ_INT(0, "all-progress", &پرمختګ,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_پرمختګ_implied,
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
		OPT_INTEGER(0, "window", &کړکۍ,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &کړکۍ_حافظه_حد,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &ژوروالی,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_پلټنه_تارونه,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_تشه,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_دننی_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_سېټ_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_سېټ_INT_F(0, "indexed-objects", &rev_list_شاخص,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_ډېرپېښ),
		OPT_BOOL(0, "stdout", &pack_منزل_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_نښان,
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
		OPT_BOOL(0, "path-walk", &لار_walk,
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
		OPT_سېټ_INT(0, "write-bitmap-index", &write_bitmap_شاخص,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_سېټ_INT_F(0, "write-bitmap-index-quiet",
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
			 &exclude_promisor_objects_غوره_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_نسخه,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_پای_ټکی(),
	};

	إذا (DFS_عدد_حالتونه > (1 << OE_DFS_STATE_بټونه))
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

	پرمختګ = isatty(2);
	argc = parse_options(argc, argv, مختاړی, pack_objects_options,
			     pack_usage, 0);

	إذا (argc) {
		بنسټ_name = argv[0];
		argc--;
	}
	إذا (pack_منزل_stdout != !بنسټ_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	إذا (لار_walk < 0) {
		إذا (use_bitmap_شاخص > 0 ||
		    !use_دننی_rev_list)
			لار_walk = 0;
		وإلا إذا (the_repository->gitdir &&
			 the_repository->settings.pack_use_لار_walk)
			لار_walk = 1;
		وإلا
			لار_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	إذا (ژوروالی < 0)
		ژوروالی = 0;
	إذا (ژوروالی >= (1 << OE_ژوروالی_بټونه)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			ژوروالی, (1 << OE_ژوروالی_بټونه) - 1);
		ژوروالی = (1 << OE_ژوروالی_بټونه) - 1;
	}
	إذا (cache_max_small_delta_اندازه >= (1U << OE_Z_DELTA_بټونه)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_بټونه) - 1);
		cache_max_small_delta_اندازه = (1U << OE_Z_DELTA_بټونه) - 1;
	}
	إذا (کړکۍ < 0)
		کړکۍ = 0;

	strvec_ټېل(&rp, "pack-objects");

	إذا (لار_walk) {
		ثابت حرف *option = عدم;
		إذا (!لار_walk_filter_compatible(&filter_options))
			option = "--filter";

		إذا (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			لار_walk = 0;
		}
	}
	إذا (لار_walk) {
		strvec_ټېل(&rp, "--objects");
	} وإلا إذا (thin) {
		use_دننی_rev_list = 1;
		strvec_ټېل(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} وإلا
		strvec_ټېل(&rp, "--objects");

	إذا (rev_list_all) {
		use_دننی_rev_list = 1;
		strvec_ټېل(&rp, "--all");
	}
	إذا (rev_list_reflog) {
		use_دننی_rev_list = 1;
		strvec_ټېل(&rp, "--reflog");
	}
	إذا (rev_list_شاخص) {
		use_دننی_rev_list = 1;
		strvec_ټېل(&rp, "--indexed-objects");
	}
	إذا (rev_list_unpacked && !stdin_packs) {
		use_دننی_rev_list = 1;
		strvec_ټېل(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_غوره_effort,
				  "--exclude-promisor-objects-best-effort");
	إذا (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		إذا (!stdin_packs) {
			use_دننی_rev_list = 1;
			strvec_ټېل(&rp, "--exclude-promisor-objects");
		}
	} وإلا إذا (exclude_promisor_objects_غوره_effort) {
		use_دننی_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	إذا (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_دننی_rev_list = 1;

	إذا (!reuse_object)
		reuse_delta = 0;
	إذا (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	وإلا إذا (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_غوره_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	إذا (!delta_پلټنه_تارونه)	/* --threads=0 means autodetect */
		delta_پلټنه_تارونه = online_cpus();

	إذا (!HAVE_تارونه && delta_پلټنه_تارونه != 1)
		warning(_("no threads support, ignoring --threads"));
	إذا (!pack_منزل_stdout && !pack_اندازه_حد)
		pack_اندازه_حد = pack_اندازه_حد_cfg;
	إذا (pack_منزل_stdout && pack_اندازه_حد)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	إذا (pack_اندازه_حد && pack_اندازه_حد < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_اندازه_حد = 1024*1024;
	}

	إذا (!pack_منزل_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	إذا (!rev_list_all || !rev_list_reflog || !rev_list_شاخص)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	إذا (stdin_packs && use_دننی_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	إذا (cruft) {
		إذا (use_دننی_rev_list)
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
		use_bitmap_شاخص_default = 0;

	إذا (use_bitmap_شاخص < 0)
		use_bitmap_شاخص = use_bitmap_شاخص_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	إذا (!use_دننی_rev_list || (!pack_منزل_stdout && write_bitmap_شاخص) || is_repository_shallow(the_repository))
		use_bitmap_شاخص = 0;

	إذا (pack_منزل_stdout || !rev_list_all)
		write_bitmap_شاخص = 0;

	إذا (name_hash_نسخه < 0)
		name_hash_نسخه = (صحيح)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_نسخه();

	إذا (use_delta_islands)
		strvec_ټېل(&rp, "--topo-order");

	إذا (پرمختګ && all_پرمختګ_implied)
		پرمختګ = 2;

	جمع_کړه_extra_kept_packs(&keep_pack_list);
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

	إذا (پرمختګ && !cruft)
		پرمختګ_state = پیل_پرمختګ(the_repository,
						_("Enumerating objects"), 0);
	إذا (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} وإلا إذا (cruft) {
		read_cruft_objects();
	} وإلا إذا (!use_دننی_rev_list) {
		read_object_list_from_stdin();
	} وإلا {
		بنية rev_info revs;

		repo_init_revisions(the_repository, &revs, عدم);
		list_objects_filter_copy(&revs.filter, &filter_options);
		إذا (exclude_promisor_objects_غوره_effort) {
			revs.include_check = is_not_دننه_promisor_pack;
			revs.include_check_obj = is_not_دننه_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_بنسټ();
	إذا (include_نښان && nr_پایله)
		refs_for_each_نښان_ref(get_main_ref_store(the_repository),
				      جمع_کړه_ref_نښان, عدم);
	stop_پرمختګ(&پرمختګ_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	إذا (non_تشه && !nr_پایله)
		اذهب cleanup;
	إذا (nr_پایله) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(کړکۍ, ژوروالی);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	إذا (پرمختګ)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_کارول_شوی_nr);

	trace2_معلومات_intmax("pack-objects", the_repository, "written", written);
	trace2_معلومات_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_معلومات_intmax("pack-objects", the_repository, "reused", reused);
	trace2_معلومات_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_معلومات_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_معلومات_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_کارول_شوی_nr);

cleanup:
	clear_packing_معلومات(&منزل_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	أرجع 0;
}
