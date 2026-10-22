#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "attr.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "delta.h"
#అన్తర్భావ "pack.h"
#అన్తర్భావ "pack-revindex.h"
#అన్తర్భావ "csum-file.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "list-objects.h"
#అన్తర్భావ "list-objects-filter-options.h"
#అన్తర్భావ "pack-objects.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "thread-utils.h"
#అన్తర్భావ "pack-bitmap.h"
#అన్తర్భావ "delta-islands.h"
#అన్తర్భావ "reachable.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "strmap.h"
#అన్తర్భావ "list.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-file-convert.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/streaming.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "midx.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "shallow.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "pack-mtimes.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
స్థిర నిర్మాణం packing_దత్తాంశం గమ్యం_pack;

స్థిర అంతఃవరుస నిర్మాణం object_నమోదు *oe_delta(
		స్థిరరాశి నిర్మాణం packing_దత్తాంశం *pack,
		స్థిరరాశి నిర్మాణం object_నమోదు *e)
{
	ఒకవేళ (!e->delta_idx)
		తిరిగి ఖాళీ;
	ఒకవేళ (e->ext_పునాది)
		తిరిగి &pack->ext_bases[e->delta_idx - 1];
	లేకపోతే
		తిరిగి &pack->objects[e->delta_idx - 1];
}

స్థిర అంతఃవరుస కొలత_t oe_delta_కొలత(నిర్మాణం packing_దత్తాంశం *pack,
				   స్థిరరాశి నిర్మాణం object_నమోదు *e)
{
	ఒకవేళ (e->delta_కొలత_valid)
		తిరిగి e->delta_కొలత;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	తిరిగి pack->delta_కొలత_2[e - pack->objects];
}

కొలత_t oe_get_కొలత_slow(నిర్మాణం packing_దత్తాంశం *pack,
			స్థిరరాశి నిర్మాణం object_నమోదు *e);

స్థిర అంతఃవరుస కొలత_t oe_కొలత(నిర్మాణం packing_దత్తాంశం *pack,
			     స్థిరరాశి నిర్మాణం object_నమోదు *e)
{
	ఒకవేళ (e->కొలత_valid)
		తిరిగి e->కొలత_2;

	తిరిగి oe_get_కొలత_slow(pack, e);
}

స్థిర అంతఃవరుస శూన్య oe_సమితి_delta(నిర్మాణం packing_దత్తాంశం *pack,
				నిర్మాణం object_నమోదు *e,
				నిర్మాణం object_నమోదు *delta)
{
	ఒకవేళ (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	లేకపోతే
		e->delta_idx = 0;
}

స్థిర అంతఃవరుస నిర్మాణం object_నమోదు *oe_delta_sibling(
		స్థిరరాశి నిర్మాణం packing_దత్తాంశం *pack,
		స్థిరరాశి నిర్మాణం object_నమోదు *e)
{
	ఒకవేళ (e->delta_sibling_idx)
		తిరిగి &pack->objects[e->delta_sibling_idx - 1];
	తిరిగి ఖాళీ;
}

స్థిర అంతఃవరుస నిర్మాణం object_నమోదు *oe_delta_బిడ్డ(
		స్థిరరాశి నిర్మాణం packing_దత్తాంశం *pack,
		స్థిరరాశి నిర్మాణం object_నమోదు *e)
{
	ఒకవేళ (e->delta_బిడ్డ_idx)
		తిరిగి &pack->objects[e->delta_బిడ్డ_idx - 1];
	తిరిగి ఖాళీ;
}

స్థిర అంతఃవరుస శూన్య oe_సమితి_delta_బిడ్డ(నిర్మాణం packing_దత్తాంశం *pack,
				      నిర్మాణం object_నమోదు *e,
				      నిర్మాణం object_నమోదు *delta)
{
	ఒకవేళ (delta)
		e->delta_బిడ్డ_idx = (delta - pack->objects) + 1;
	లేకపోతే
		e->delta_బిడ్డ_idx = 0;
}

స్థిర అంతఃవరుస శూన్య oe_సమితి_delta_sibling(నిర్మాణం packing_దత్తాంశం *pack,
					నిర్మాణం object_నమోదు *e,
					నిర్మాణం object_నమోదు *delta)
{
	ఒకవేళ (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	లేకపోతే
		e->delta_sibling_idx = 0;
}

స్థిర అంతఃవరుస శూన్య oe_సమితి_కొలత(నిర్మాణం packing_దత్తాంశం *pack,
			       నిర్మాణం object_నమోదు *e,
			       కొలత_t కొలత)
{
	ఒకవేళ (కొలత < pack->oe_కొలత_హద్దు) {
		e->కొలత_2 = కొలత;
		e->కొలత_valid = 1;
	} లేకపోతే {
		e->కొలత_valid = 0;
		ఒకవేళ (oe_get_కొలత_slow(pack, e) != కొలత)
			BUG("'size' is supposed to be the object size!");
	}
}

స్థిర అంతఃవరుస శూన్య oe_సమితి_delta_కొలత(నిర్మాణం packing_దత్తాంశం *pack,
				     నిర్మాణం object_నమోదు *e,
				     కొలత_t కొలత)
{
	ఒకవేళ (కొలత < pack->oe_delta_కొలత_హద్దు) {
		e->delta_కొలత = కొలత;
		e->delta_కొలత_valid = 1;
	} లేకపోతే {
		packing_దత్తాంశం_తాళం(pack);
		ఒకవేళ (!pack->delta_కొలత_2)
			ALLOC_ARRAY(pack->delta_కొలత_2, pack->nr_alloc);
		packing_దత్తాంశం_unlock(pack);

		pack->delta_కొలత_2[e - pack->objects] = కొలత;
		e->delta_కొలత_valid = 0;
	}
}

#నిర్వచించు లోపల_PACK(obj) oe_లోపల_pack(&గమ్యం_pack, obj)
#నిర్వచించు కొలత_3(obj) oe_కొలత(&గమ్యం_pack, obj)
#నిర్వచించు సమితి_కొలత(obj,కొలత) oe_సమితి_కొలత(&గమ్యం_pack, obj, కొలత)
#నిర్వచించు DELTA_కొలత(obj) oe_delta_కొలత(&గమ్యం_pack, obj)
#నిర్వచించు DELTA(obj) oe_delta(&గమ్యం_pack, obj)
#నిర్వచించు DELTA_బిడ్డ(obj) oe_delta_బిడ్డ(&గమ్యం_pack, obj)
#నిర్వచించు DELTA_SIBLING(obj) oe_delta_sibling(&గమ్యం_pack, obj)
#నిర్వచించు సమితి_DELTA(obj, val) oe_సమితి_delta(&గమ్యం_pack, obj, val)
#నిర్వచించు సమితి_DELTA_EXT(obj, oid) oe_సమితి_delta_ext(&గమ్యం_pack, obj, oid)
#నిర్వచించు సమితి_DELTA_కొలత(obj, val) oe_సమితి_delta_కొలత(&గమ్యం_pack, obj, val)
#నిర్వచించు సమితి_DELTA_బిడ్డ(obj, val) oe_సమితి_delta_బిడ్డ(&గమ్యం_pack, obj, val)
#నిర్వచించు సమితి_DELTA_SIBLING(obj, val) oe_సమితి_delta_sibling(&గమ్యం_pack, obj, val)

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	ఖాళీ
};

స్థిర నిర్మాణం pack_idx_నమోదు **written_list;
స్థిర uint32_t nr_ఫలితం, nr_written, nr_చూసిన;
స్థిర నిర్మాణం bitmap_సూచీ *bitmap_git;
స్థిర uint32_t write_layer;

స్థిర పూర్ణాంకం non_ఖాళీలు;
స్థిర పూర్ణాంకం reuse_delta = 1, reuse_object = 1;
స్థిర పూర్ణాంకం keep_unreachable, unpack_unreachable, include_ట్యాగ్;
స్థిర timestamp_t unpack_unreachable_expiration;
స్థిర పూర్ణాంకం pack_loose_unreachable;
స్థిర పూర్ణాంకం cruft;
స్థిర పూర్ణాంకం shallow = 0;
స్థిర timestamp_t cruft_expiration;
స్థిర పూర్ణాంకం local;
స్థిర పూర్ణాంకం have_non_local_packs;
స్థిర పూర్ణాంకం incremental;
స్థిర పూర్ణాంకం ignore_packed_keep_on_disk;
స్థిర పూర్ణాంకం ignore_packed_keep_లోపల_core;
స్థిర పూర్ణాంకం ignore_packed_keep_లోపల_core_open;
స్థిర పూర్ణాంకం ignore_packed_keep_లోపల_core_has_cruft;
స్థిర పూర్ణాంకం allow_ofs_delta;
స్థిర నిర్మాణం pack_idx_option pack_idx_opts;
స్థిర స్థిరరాశి అక్షరం *పునాది_name;
స్థిర పూర్ణాంకం పురోగతి = 1;
స్థిర పూర్ణాంకం కిటికీ = 10;
స్థిర గుర్తులేని దీర్ఘ pack_కొలత_హద్దు;
స్థిర పూర్ణాంకం లోతు = 50;
స్థిర పూర్ణాంకం delta_శోధన_దారాలు;
స్థిర పూర్ణాంకం pack_గమ్యం_stdout;
స్థిర పూర్ణాంకం sparse;
స్థిర పూర్ణాంకం thin;
స్థిర పూర్ణాంకం మార్గం_walk = -1;
స్థిర పూర్ణాంకం అంకె_preferred_పునాది;
స్థిర నిర్మాణం పురోగతి *పురోగతి_state;

స్థిర నిర్మాణం bitmapped_pack *reuse_packfiles;
స్థిర కొలత_t reuse_packfiles_nr;
స్థిర కొలత_t reuse_packfiles_వాడిన_nr;
స్థిర uint32_t reuse_packfile_objects;
స్థిర నిర్మాణం bitmap *reuse_packfile_bitmap;

స్థిర పూర్ణాంకం use_bitmap_సూచీ_default = 1;
స్థిర పూర్ణాంకం use_bitmap_సూచీ = -1;
స్థిర గణన {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
స్థిర గణన {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_సూచీ;
స్థిర uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

స్థిర పూర్ణాంకం exclude_promisor_objects;
స్థిర పూర్ణాంకం exclude_promisor_objects_ఉత్తమ_effort;

స్థిర పూర్ణాంకం use_delta_islands;

స్థిర కొలత_t delta_cache_కొలత = 0;
స్థిర కొలత_t max_delta_cache_కొలత = DEFAULT_DELTA_CACHE_కొలత;
స్థిర గుర్తులేని దీర్ఘ cache_max_small_delta_కొలత = 1000;

స్థిర గుర్తులేని దీర్ఘ కిటికీ_జ్ఞాపకం_హద్దు = 0;

స్థిర నిర్మాణం string_list uri_protocols = STRING_LIST_INIT_NODUP;

గణన missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
స్థిర గణన missing_action arg_missing_action;
స్థిర show_object_fn fn_show_object;

నిర్మాణం configured_exclusion {
	నిర్మాణం oidmap_నమోదు e;
	అక్షరం *pack_hash_hex;
	అక్షరం *uri;
};
స్థిర నిర్మాణం oidmap configured_exclusions;

స్థిర నిర్మాణం oidset excluded_by_config;
స్థిర పూర్ణాంకం name_hash_సంస్కరణ = -1;

గణన stdin_packs_బహుళకం {
	STDIN_PACKS_బహుళకం_NONE,
	STDIN_PACKS_బహుళకం_STANDARD,
	STDIN_PACKS_బహుళకం_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
స్థిర శూన్య validate_name_hash_సంస్కరణ(శూన్య)
{
	ఒకవేళ (name_hash_సంస్కరణ < 1 || name_hash_సంస్కరణ > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_సంస్కరణ);
	ఒకవేళ (write_bitmap_సూచీ && name_hash_సంస్కరణ != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_సంస్కరణ = 1;
	}
}

స్థిర అంతఃవరుస uint32_t pack_name_hash_fn(స్థిరరాశి అక్షరం *name)
{
	స్థిర పూర్ణాంకం చూసిన_సంస్కరణ = -1;

	ఒకవేళ (చూసిన_సంస్కరణ < 0)
		చూసిన_సంస్కరణ = name_hash_సంస్కరణ;
	లేకపోతే ఒకవేళ (చూసిన_సంస్కరణ != name_hash_సంస్కరణ)
		BUG("name hash version changed from %d to %d mid-process",
		    చూసిన_సంస్కరణ, name_hash_సంస్కరణ);

	ఎంపిక (name_hash_సంస్కరణ) {
	స్థితి 1:
		తిరిగి pack_name_hash(name);

	స్థితి 2:
		తిరిగి pack_name_hash_v2((స్థిరరాశి గుర్తులేని అక్షరం *)name);

	అప్రమేయ:
		BUG("invalid name-hash version: %d", name_hash_సంస్కరణ);
	}
}

/*
 * stats
 */
స్థిర uint32_t written, written_delta;
స్థిర uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
స్థిర నిర్మాణం commit **సూచీకృత_commits;
స్థిర గుర్తులేని పూర్ణాంకం సూచీకృత_commits_nr;
స్థిర గుర్తులేని పూర్ణాంకం సూచీకృత_commits_alloc;

స్థిర శూన్య సూచీ_commit_for_bitmap(నిర్మాణం commit *commit)
{
	ఒకవేళ (సూచీకృత_commits_nr >= సూచీకృత_commits_alloc) {
		సూచీకృత_commits_alloc = (సూచీకృత_commits_alloc + 32) * 2;
		REALLOC_ARRAY(సూచీకృత_commits, సూచీకృత_commits_alloc);
	}

	సూచీకృత_commits[సూచీకృత_commits_nr++] = commit;
}

స్థిర శూన్య *get_delta(నిర్మాణం object_నమోదు *నమోదు_2)
{
	గుర్తులేని దీర్ఘ కొలత, పునాది_కొలత;
	కొలత_t delta_కొలత_2;
	శూన్య *buf, *పునాది_buf, *delta_buf;
	గణన object_type type;
	కొలత_t కొలత_st = 0, పునాది_కొలత_st = 0;

	buf = odb_read_object(the_repository->objects, &నమోదు_2->idx.oid,
			      &type, &కొలత_st);
	కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
	ఒకవేళ (!buf)
		die(_("unable to read %s"), oid_గమ్యం_hex(&నమోదు_2->idx.oid));
	పునాది_buf = odb_read_object(the_repository->objects,
				   &DELTA(నమోదు_2)->idx.oid, &type,
				   &పునాది_కొలత_st);
	పునాది_కొలత = cast_కొలత_t_గమ్యం_ulong(పునాది_కొలత_st);
	ఒకవేళ (!పునాది_buf)
		die("unable to read %s",
		    oid_గమ్యం_hex(&DELTA(నమోదు_2)->idx.oid));
	delta_buf = diff_delta(పునాది_buf, పునాది_కొలత,
			       buf, కొలత, &delta_కొలత_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	ఒకవేళ (!delta_buf || delta_కొలత_2 != DELTA_కొలత(నమోదు_2))
		BUG("delta size changed");
	విడుదల(buf);
	విడుదల(పునాది_buf);
	తిరిగి delta_buf;
}

స్థిర గుర్తులేని దీర్ఘ do_compress(శూన్య **pptr, గుర్తులేని దీర్ఘ కొలత)
{
	git_zstream ప్రవాహం;
	శూన్య *లోపల, *బహిర్గతం;
	గుర్తులేని దీర్ఘ maxsize;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	git_deflate_init(&ప్రవాహం, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&ప్రవాహం, కొలత);

	లోపల = *pptr;
	బహిర్గతం = xmalloc(maxsize);
	*pptr = బహిర్గతం;

	ప్రవాహం.తదుపరి_లోపల = లోపల;
	ప్రవాహం.avail_లోపల = కొలత;
	ప్రవాహం.తదుపరి_బహిర్గతం = బహిర్గతం;
	ప్రవాహం.avail_బహిర్గతం = maxsize;
	అయితే (git_deflate(&ప్రవాహం, Z_ముగింపు) == Z_OK)
		; /* nothing */
	git_deflate_చివర(&ప్రవాహం);

	విడుదల(లోపల);
	తిరిగి ప్రవాహం.మొత్తం_బహిర్గతం;
}

స్థిర గుర్తులేని దీర్ఘ write_large_blob_దత్తాంశం(నిర్మాణం odb_ప్రవాహం *st, నిర్మాణం hashfile *f,
					   స్థిరరాశి నిర్మాణం object_id *oid)
{
	git_zstream ప్రవాహం;
	గుర్తులేని అక్షరం ibuf[1024 * 16];
	గుర్తులేని అక్షరం obuf[1024 * 16];
	గుర్తులేని దీర్ఘ olen = 0;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	git_deflate_init(&ప్రవాహం, cfg->pack_compression_level);

	కొరకు (;;) {
		ssize_t readlen;
		పూర్ణాంకం zret = Z_OK;
		readlen = odb_ప్రవాహం_read(st, ibuf, పరిమాణం(ibuf));
		ఒకవేళ (readlen == -1)
			die(_("unable to read %s"), oid_గమ్యం_hex(oid));

		ప్రవాహం.తదుపరి_లోపల = ibuf;
		ప్రవాహం.avail_లోపల = readlen;
		అయితే ((ప్రవాహం.avail_లోపల || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			ప్రవాహం.తదుపరి_బహిర్గతం = obuf;
			ప్రవాహం.avail_బహిర్గతం = పరిమాణం(obuf);
			zret = git_deflate(&ప్రవాహం, readlen ? 0 : Z_ముగింపు);
			hashwrite(f, obuf, ప్రవాహం.తదుపరి_బహిర్గతం - obuf);
			olen += ప్రవాహం.తదుపరి_బహిర్గతం - obuf;
		}
		ఒకవేళ (ప్రవాహం.avail_లోపల)
			die(_("deflate error (%d)"), zret);
		ఒకవేళ (readlen == 0) {
			ఒకవేళ (zret != Z_ప్రవాహం_చివర)
				die(_("deflate error (%d)"), zret);
			ఆపు;
		}
	}
	git_deflate_చివర(&ప్రవాహం);
	తిరిగి olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
స్థిర పూర్ణాంకం check_pack_inflate(నిర్మాణం packed_git *p,
		నిర్మాణం pack_కిటికీ **w_curs,
		off_t స్థానభ్రంశం,
		off_t len,
		కొలత_t expect)
{
	git_zstream ప్రవాహం;
	గుర్తులేని అక్షరం fakebuf[4096], *లోపల;
	పూర్ణాంకం st;

	memset(&ప్రవాహం, 0, పరిమాణం(ప్రవాహం));
	git_inflate_init(&ప్రవాహం);
	చేయి {
		లోపల = use_pack(p, w_curs, స్థానభ్రంశం, &ప్రవాహం.avail_లోపల);
		ప్రవాహం.తదుపరి_లోపల = లోపల;
		ప్రవాహం.తదుపరి_బహిర్గతం = fakebuf;
		ప్రవాహం.avail_బహిర్గతం = పరిమాణం(fakebuf);
		st = git_inflate(&ప్రవాహం, Z_ముగింపు);
		స్థానభ్రంశం += ప్రవాహం.తదుపరి_లోపల - లోపల;
	} అయితే (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_చివర(&ప్రవాహం);
	తిరిగి (st == Z_ప్రవాహం_చివర &&
		ప్రవాహం.మొత్తం_బహిర్గతం == expect &&
		ప్రవాహం.మొత్తం_లోపల == len) ? 0 : -1;
}

స్థిర శూన్య copy_pack_దత్తాంశం(నిర్మాణం hashfile *f,
		నిర్మాణం packed_git *p,
		నిర్మాణం pack_కిటికీ **w_curs,
		off_t స్థానభ్రంశం,
		off_t len)
{
	గుర్తులేని అక్షరం *లోపల;
	కొలత_t avail;

	అయితే (len) {
		లోపల = use_pack(p, w_curs, స్థానభ్రంశం, &avail);
		ఒకవేళ (avail > len)
			avail = (గుర్తులేని దీర్ఘ)len;
		hashwrite(f, లోపల, avail);
		స్థానభ్రంశం += avail;
		len -= avail;
	}
}

స్థిర అంతఃవరుస పూర్ణాంకం oe_కొలత_greater_than(నిర్మాణం packing_దత్తాంశం *pack,
				       స్థిరరాశి నిర్మాణం object_నమోదు *lhs,
				       కొలత_t rhs)
{
	ఒకవేళ (lhs->కొలత_valid)
		తిరిగి lhs->కొలత_2 > rhs;
	ఒకవేళ (rhs < pack->oe_కొలత_హద్దు) /* rhs < 2^x <= lhs ? */
		తిరిగి 1;
	తిరిగి oe_get_కొలత_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
స్థిర గుర్తులేని దీర్ఘ write_no_reuse_object(నిర్మాణం hashfile *f, నిర్మాణం object_నమోదు *నమోదు_2,
					   గుర్తులేని దీర్ఘ హద్దు, పూర్ణాంకం usable_delta)
{
	గుర్తులేని దీర్ఘ కొలత, datalen;
	గుర్తులేని అక్షరం శీర్షిక[MAX_PACK_OBJECT_శీర్షిక],
		      dheader[MAX_PACK_OBJECT_శీర్షిక];
	గుర్తులేని hdrlen;
	గణన object_type type;
	శూన్య *buf;
	నిర్మాణం odb_ప్రవాహం *st = ఖాళీ;
	స్థిరరాశి గుర్తులేని hashsz = the_hash_algo->rawsz;

	ఒకవేళ (!usable_delta) {
		ఒకవేళ (oe_type(నమోదు_2) == OBJ_BLOB &&
		    oe_కొలత_greater_than(&గమ్యం_pack, నమోదు_2,
					 repo_settings_get_big_file_పరిమితి(the_repository)) &&
		    (st = odb_ప్రవాహం_from_object(the_repository->objects, &నమోదు_2->idx.oid,
						 ఖాళీ)) != ఖాళీ) {
			buf = ఖాళీ;
			type = st->type;
			కొలత = st->కొలత;
		} లేకపోతే {
			కొలత_t కొలత_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &నమోదు_2->idx.oid, &type,
					      &కొలత_st);
			కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
			ఒకవేళ (!buf)
				die(_("unable to read %s"),
				    oid_గమ్యం_hex(&నమోదు_2->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(నమోదు_2->delta_దత్తాంశం);
		నమోదు_2->z_delta_కొలత = 0;
	} లేకపోతే ఒకవేళ (నమోదు_2->delta_దత్తాంశం) {
		కొలత = DELTA_కొలత(నమోదు_2);
		buf = నమోదు_2->delta_దత్తాంశం;
		నమోదు_2->delta_దత్తాంశం = ఖాళీ;
		type = (allow_ofs_delta && DELTA(నమోదు_2)->idx.స్థానభ్రంశం) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} లేకపోతే {
		buf = get_delta(నమోదు_2);
		కొలత = DELTA_కొలత(నమోదు_2);
		type = (allow_ofs_delta && DELTA(నమోదు_2)->idx.స్థానభ్రంశం) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	ఒకవేళ (st)	/* large blob case, just assume we don't compress well */
		datalen = కొలత;
	లేకపోతే ఒకవేళ (నమోదు_2->z_delta_కొలత)
		datalen = నమోదు_2->z_delta_కొలత;
	లేకపోతే
		datalen = do_compress(&buf, కొలత);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_లోపల_pack_object_శీర్షిక(శీర్షిక, పరిమాణం(శీర్షిక),
					      type, కొలత);

	ఒకవేళ (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = నమోదు_2->idx.స్థానభ్రంశం - DELTA(నమోదు_2)->idx.స్థానభ్రంశం;
		గుర్తులేని pos = పరిమాణం(dheader) - 1;
		dheader[pos] = ofs & 127;
		అయితే (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ఒకవేళ (హద్దు && hdrlen + పరిమాణం(dheader) - pos + datalen + hashsz >= హద్దు) {
			ఒకవేళ (st)
				odb_ప్రవాహం_close(st);
			విడుదల(buf);
			తిరిగి 0;
		}
		hashwrite(f, శీర్షిక, hdrlen);
		hashwrite(f, dheader + pos, పరిమాణం(dheader) - pos);
		hdrlen += పరిమాణం(dheader) - pos;
	} లేకపోతే ఒకవేళ (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		ఒకవేళ (హద్దు && hdrlen + hashsz + datalen + hashsz >= హద్దు) {
			ఒకవేళ (st)
				odb_ప్రవాహం_close(st);
			విడుదల(buf);
			తిరిగి 0;
		}
		hashwrite(f, శీర్షిక, hdrlen);
		hashwrite(f, DELTA(నమోదు_2)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} లేకపోతే {
		ఒకవేళ (హద్దు && hdrlen + datalen + hashsz >= హద్దు) {
			ఒకవేళ (st)
				odb_ప్రవాహం_close(st);
			విడుదల(buf);
			తిరిగి 0;
		}
		hashwrite(f, శీర్షిక, hdrlen);
	}
	ఒకవేళ (st) {
		datalen = write_large_blob_దత్తాంశం(st, f, &నమోదు_2->idx.oid);
		odb_ప్రవాహం_close(st);
	} లేకపోతే {
		hashwrite(f, buf, datalen);
		విడుదల(buf);
	}

	తిరిగి hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
స్థిర off_t write_reuse_object(నిర్మాణం hashfile *f, నిర్మాణం object_నమోదు *నమోదు_2,
				గుర్తులేని దీర్ఘ హద్దు, పూర్ణాంకం usable_delta)
{
	నిర్మాణం packed_git *p = లోపల_PACK(నమోదు_2);
	నిర్మాణం pack_కిటికీ *w_curs = ఖాళీ;
	uint32_t pos;
	off_t స్థానభ్రంశం, cur;
	గణన object_type type = oe_type(నమోదు_2);
	గణన object_type లోపల_pack_type;
	off_t datalen;
	గుర్తులేని అక్షరం శీర్షిక[MAX_PACK_OBJECT_శీర్షిక],
		      dheader[MAX_PACK_OBJECT_శీర్షిక];
	గుర్తులేని hdrlen;
	స్థిరరాశి గుర్తులేని hashsz = the_hash_algo->rawsz;
	కొలత_t నమోదు_కొలత;

	cur = నమోదు_2->లోపల_pack_స్థానభ్రంశం;
	లోపల_pack_type = unpack_object_శీర్షిక(p, &w_curs, &cur, &నమోదు_కొలత);
	ఒకవేళ (లోపల_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_గమ్యం_hex(&నమోదు_2->idx.oid));

	ఒకవేళ (DELTA(నమోదు_2))
		type = (allow_ofs_delta && DELTA(నమోదు_2)->idx.స్థానభ్రంశం) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_లోపల_pack_object_శీర్షిక(శీర్షిక, పరిమాణం(శీర్షిక),
					      type, నమోదు_కొలత);

	స్థానభ్రంశం = నమోదు_2->లోపల_pack_స్థానభ్రంశం;
	ఒకవేళ (స్థానభ్రంశం_గమ్యం_pack_pos(p, స్థానభ్రంశం, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_గమ్యం_hex(&నమోదు_2->idx.oid), (uintmax_t)స్థానభ్రంశం,
		    p->pack_name);
	datalen = pack_pos_గమ్యం_స్థానభ్రంశం(p, pos + 1) - స్థానభ్రంశం;
	ఒకవేళ (!pack_గమ్యం_stdout && p->సూచీ_సంస్కరణ > 1 &&
	    check_pack_చక్రీయ_తనిఖీ(p, &w_curs, స్థానభ్రంశం, datalen,
			   pack_pos_గమ్యం_సూచీ(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_గమ్యం_hex(&నమోదు_2->idx.oid));
		unuse_pack(&w_curs);
		తిరిగి write_no_reuse_object(f, నమోదు_2, హద్దు, usable_delta);
	}

	స్థానభ్రంశం += నమోదు_2->లోపల_pack_శీర్షిక_కొలత;
	datalen -= నమోదు_2->లోపల_pack_శీర్షిక_కొలత;

	ఒకవేళ (!pack_గమ్యం_stdout && p->సూచీ_సంస్కరణ == 1 &&
	    check_pack_inflate(p, &w_curs, స్థానభ్రంశం, datalen, నమోదు_కొలత)) {
		error(_("corrupt packed object for %s"),
		      oid_గమ్యం_hex(&నమోదు_2->idx.oid));
		unuse_pack(&w_curs);
		తిరిగి write_no_reuse_object(f, నమోదు_2, హద్దు, usable_delta);
	}

	ఒకవేళ (type == OBJ_OFS_DELTA) {
		off_t ofs = నమోదు_2->idx.స్థానభ్రంశం - DELTA(నమోదు_2)->idx.స్థానభ్రంశం;
		గుర్తులేని pos = పరిమాణం(dheader) - 1;
		dheader[pos] = ofs & 127;
		అయితే (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		ఒకవేళ (హద్దు && hdrlen + పరిమాణం(dheader) - pos + datalen + hashsz >= హద్దు) {
			unuse_pack(&w_curs);
			తిరిగి 0;
		}
		hashwrite(f, శీర్షిక, hdrlen);
		hashwrite(f, dheader + pos, పరిమాణం(dheader) - pos);
		hdrlen += పరిమాణం(dheader) - pos;
		reused_delta++;
	} లేకపోతే ఒకవేళ (type == OBJ_REF_DELTA) {
		ఒకవేళ (హద్దు && hdrlen + hashsz + datalen + hashsz >= హద్దు) {
			unuse_pack(&w_curs);
			తిరిగి 0;
		}
		hashwrite(f, శీర్షిక, hdrlen);
		hashwrite(f, DELTA(నమోదు_2)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} లేకపోతే {
		ఒకవేళ (హద్దు && hdrlen + datalen + hashsz >= హద్దు) {
			unuse_pack(&w_curs);
			తిరిగి 0;
		}
		hashwrite(f, శీర్షిక, hdrlen);
	}
	copy_pack_దత్తాంశం(f, p, &w_curs, స్థానభ్రంశం, datalen);
	unuse_pack(&w_curs);
	reused++;
	తిరిగి hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
స్థిర off_t write_object(నిర్మాణం hashfile *f,
			  నిర్మాణం object_నమోదు *నమోదు_2,
			  off_t write_స్థానభ్రంశం)
{
	గుర్తులేని దీర్ఘ హద్దు;
	off_t len;
	పూర్ణాంకం usable_delta, గమ్యం_reuse;

	ఒకవేళ (!pack_గమ్యం_stdout)
		చక్రీయ_తనిఖీ_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	ఒకవేళ (!pack_కొలత_హద్దు || !nr_written)
		హద్దు = 0;
	లేకపోతే ఒకవేళ (pack_కొలత_హద్దు <= write_స్థానభ్రంశం)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		హద్దు = 1;
	లేకపోతే
		హద్దు = pack_కొలత_హద్దు - write_స్థానభ్రంశం;

	ఒకవేళ (!DELTA(నమోదు_2))
		usable_delta = 0;	/* no delta */
	లేకపోతే ఒకవేళ (!pack_కొలత_హద్దు)
	       usable_delta = 1;	/* unlimited packfile */
	లేకపోతే ఒకవేళ (DELTA(నమోదు_2)->idx.స్థానభ్రంశం == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	లేకపోతే ఒకవేళ (DELTA(నమోదు_2)->idx.స్థానభ్రంశం)
		usable_delta = 1;	/* base already exists in this pack */
	లేకపోతే
		usable_delta = 0;	/* base could end up in another pack */

	ఒకవేళ (!reuse_object)
		గమ్యం_reuse = 0;	/* explicit */
	లేకపోతే ఒకవేళ (!లోపల_PACK(నమోదు_2))
		గమ్యం_reuse = 0;	/* can't reuse what we don't have */
	లేకపోతే ఒకవేళ (oe_type(నమోదు_2) == OBJ_REF_DELTA ||
		 oe_type(నమోదు_2) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		గమ్యం_reuse = usable_delta;
				/* ... but pack split may override that */
	లేకపోతే ఒకవేళ (oe_type(నమోదు_2) != నమోదు_2->లోపల_pack_type)
		గమ్యం_reuse = 0;	/* pack has delta which is unusable */
	లేకపోతే ఒకవేళ (DELTA(నమోదు_2))
		గమ్యం_reuse = 0;	/* we want to pack afresh */
	లేకపోతే
		గమ్యం_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	ఒకవేళ (!గమ్యం_reuse)
		len = write_no_reuse_object(f, నమోదు_2, హద్దు, usable_delta);
	లేకపోతే
		len = write_reuse_object(f, నమోదు_2, హద్దు, usable_delta);
	ఒకవేళ (!len)
		తిరిగి 0;

	ఒకవేళ (usable_delta)
		written_delta++;
	written++;
	ఒకవేళ (!pack_గమ్యం_stdout)
		నమోదు_2->idx.చక్రీయ_తనిఖీ_32 = చక్రీయ_తనిఖీ_32_చివర(f);
	తిరిగి len;
}

గణన write_one_స్థితి {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*సాధారణ*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

స్థిర గణన write_one_స్థితి write_one(నిర్మాణం hashfile *f,
				       నిర్మాణం object_నమోదు *e,
				       off_t *స్థానభ్రంశం)
{
	off_t కొలత;
	పూర్ణాంకం recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.స్థానభ్రంశం == 1);
	ఒకవేళ (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_గమ్యం_hex(&e->idx.oid));
		తిరిగి WRITE_ONE_RECURSIVE;
	} లేకపోతే ఒకవేళ (e->idx.స్థానభ్రంశం || e->preferred_పునాది) {
		/* offset is non zero if object is written already. */
		తిరిగి WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	ఒకవేళ (DELTA(e)) {
		e->idx.స్థానభ్రంశం = 1; /* now recurse */
		ఎంపిక (write_one(f, DELTA(e), స్థానభ్రంశం)) {
		స్థితి WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			సమితి_DELTA(e, ఖాళీ);
			ఆపు;
		అప్రమేయ:
			ఆపు;
		స్థితి WRITE_ONE_BREAK:
			e->idx.స్థానభ్రంశం = recursing;
			తిరిగి WRITE_ONE_BREAK;
		}
	}

	e->idx.స్థానభ్రంశం = *స్థానభ్రంశం;
	కొలత = write_object(f, e, *స్థానభ్రంశం);
	ఒకవేళ (!కొలత) {
		e->idx.స్థానభ్రంశం = recursing;
		తిరిగి WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	ఒకవేళ (signed_కూడు_overflows(*స్థానభ్రంశం, కొలత))
		die(_("pack too large for current definition of off_t"));
	*స్థానభ్రంశం += కొలత;
	తిరిగి WRITE_ONE_WRITTEN;
}

స్థిర పూర్ణాంకం mark_tagged(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం వాడని)
{
	నిర్మాణం object_id peeled;
	నిర్మాణం object_నమోదు *నమోదు_2 = packlist_కనుగొను(&గమ్యం_pack, ref->oid);

	ఒకవేళ (నమోదు_2)
		నమోదు_2->tagged = 1;
	ఒకవేళ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		నమోదు_2 = packlist_కనుగొను(&గమ్యం_pack, &peeled);
		ఒకవేళ (నమోదు_2)
			నమోదు_2->tagged = 1;
	}
	తిరిగి 0;
}

స్థిర అంతఃవరుస గుర్తులేని అక్షరం oe_layer(నిర్మాణం packing_దత్తాంశం *pack,
				     నిర్మాణం object_నమోదు *e)
{
	ఒకవేళ (!pack->layer)
		తిరిగి 0;
	తిరిగి pack->layer[e - pack->objects];
}

స్థిర అంతఃవరుస శూన్య కూడు_గమ్యం_write_క్రమం(నిర్మాణం object_నమోదు **wo,
			       గుర్తులేని పూర్ణాంకం *endp,
			       నిర్మాణం object_నమోదు *e)
{
	ఒకవేళ (e->filled || oe_layer(&గమ్యం_pack, e) != write_layer)
		తిరిగి;
	wo[(*endp)++] = e;
	e->filled = 1;
}

స్థిర శూన్య కూడు_descendants_గమ్యం_write_క్రమం(నిర్మాణం object_నమోదు **wo,
					   గుర్తులేని పూర్ణాంకం *endp,
					   నిర్మాణం object_నమోదు *e)
{
	పూర్ణాంకం కూడు_గమ్యం_క్రమం = 1;
	అయితే (e) {
		ఒకవేళ (కూడు_గమ్యం_క్రమం) {
			నిర్మాణం object_నమోదు *s;
			/* add this node... */
			కూడు_గమ్యం_write_క్రమం(wo, endp, e);
			/* all its siblings... */
			కొరకు (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				కూడు_గమ్యం_write_క్రమం(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		ఒకవేళ (DELTA_బిడ్డ(e)) {
			కూడు_గమ్యం_క్రమం = 1;
			e = DELTA_బిడ్డ(e);
		} లేకపోతే {
			కూడు_గమ్యం_క్రమం = 0;
			/* our sibling might have some children, it is next */
			ఒకవేళ (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				కొనసాగు;
			}
			/* go back to our parent node */
			e = DELTA(e);
			అయితే (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			ఒకవేళ (!e) {
				/* done- we hit our original root node */
				తిరిగి;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

స్థిర శూన్య కూడు_family_గమ్యం_write_క్రమం(నిర్మాణం object_నమోదు **wo,
				      గుర్తులేని పూర్ణాంకం *endp,
				      నిర్మాణం object_నమోదు *e)
{
	నిర్మాణం object_నమోదు *మూలం;

	కొరకు (మూలం = e; DELTA(మూలం); మూలం = DELTA(మూలం))
		; /* nothing */
	కూడు_descendants_గమ్యం_write_క్రమం(wo, endp, మూలం);
}

స్థిర శూన్య compute_layer_క్రమం(నిర్మాణం object_నమోదు **wo, గుర్తులేని పూర్ణాంకం *wo_చివర)
{
	గుర్తులేని పూర్ణాంకం i, చివరి_untagged;
	నిర్మాణం object_నమోదు *objects = గమ్యం_pack.objects;

	కొరకు (i = 0; i < గమ్యం_pack.nr_objects; i++) {
		ఒకవేళ (objects[i].tagged)
			ఆపు;
		కూడు_గమ్యం_write_క్రమం(wo, wo_చివర, &objects[i]);
	}
	చివరి_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	కొరకు (; i < గమ్యం_pack.nr_objects; i++) {
		ఒకవేళ (objects[i].tagged)
			కూడు_గమ్యం_write_క్రమం(wo, wo_చివర, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	కొరకు (i = చివరి_untagged; i < గమ్యం_pack.nr_objects; i++) {
		ఒకవేళ (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_ట్యాగ్)
			కొనసాగు;
		కూడు_గమ్యం_write_క్రమం(wo, wo_చివర, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	కొరకు (i = చివరి_untagged; i < గమ్యం_pack.nr_objects; i++) {
		ఒకవేళ (oe_type(&objects[i]) != OBJ_వృక్షం)
			కొనసాగు;
		కూడు_గమ్యం_write_క్రమం(wo, wo_చివర, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	కొరకు (i = చివరి_untagged; i < గమ్యం_pack.nr_objects; i++) {
		ఒకవేళ (!objects[i].filled && oe_layer(&గమ్యం_pack, &objects[i]) == write_layer)
			కూడు_family_గమ్యం_write_క్రమం(wo, wo_చివర, &objects[i]);
	}
}

స్థిర నిర్మాణం object_నమోదు **compute_write_క్రమం(శూన్య)
{
	uint32_t max_layers = 1;
	గుర్తులేని పూర్ణాంకం i, wo_చివర;

	నిర్మాణం object_నమోదు **wo;
	నిర్మాణం object_నమోదు *objects = గమ్యం_pack.objects;

	కొరకు (i = 0; i < గమ్యం_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		సమితి_DELTA_బిడ్డ(&objects[i], ఖాళీ);
		సమితి_DELTA_SIBLING(&objects[i], ఖాళీ);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	కొరకు (i = గమ్యం_pack.nr_objects; i > 0;) {
		నిర్మాణం object_నమోదు *e = &objects[--i];
		ఒకవేళ (!DELTA(e))
			కొనసాగు;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_బిడ్డ_idx;
		సమితి_DELTA_బిడ్డ(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_ట్యాగ్_ref(get_main_ref_store(the_repository), mark_tagged,
			      ఖాళీ);

	ఒకవేళ (use_delta_islands) {
		max_layers = compute_pack_layers(&గమ్యం_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, గమ్యం_pack.nr_objects);
	wo_చివర = 0;

	కొరకు (; write_layer < max_layers; ++write_layer)
		compute_layer_క్రమం(wo, &wo_చివర);

	ఒకవేళ (wo_చివర != గమ్యం_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_చివర, గమ్యం_pack.nr_objects);

	తిరిగి wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

స్థిర నిర్మాణం reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
స్థిర పూర్ణాంకం reused_chunks_nr;
స్థిర పూర్ణాంకం reused_chunks_alloc;

స్థిర శూన్య రికార్డు_reused_object(off_t where, off_t స్థానభ్రంశం)
{
	ఒకవేళ (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == స్థానభ్రంశం)
		తిరిగి;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = స్థానభ్రంశం;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
స్థిర off_t కనుగొను_reused_స్థానభ్రంశం(off_t where)
{
	పూర్ణాంకం lo = 0, hi = reused_chunks_nr;
	అయితే (lo < hi) {
		పూర్ణాంకం mi = lo + ((hi - lo) / 2);
		ఒకవేళ (where == reused_chunks[mi].original)
			తిరిగి reused_chunks[mi].difference;
		ఒకవేళ (where < reused_chunks[mi].original)
			hi = mi;
		లేకపోతే
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	ప్రతిజ్ఞా(lo);
	తిరిగి reused_chunks[lo-1].difference;
}

స్థిర శూన్య write_reused_pack_one(నిర్మాణం packed_git *reuse_packfile,
				  కొలత_t pos, నిర్మాణం hashfile *బహిర్గతం,
				  off_t pack_ప్రారంభం,
				  నిర్మాణం pack_కిటికీ **w_curs)
{
	off_t స్థానభ్రంశం, తదుపరి, cur;
	గణన object_type type;
	కొలత_t కొలత;

	స్థానభ్రంశం = pack_pos_గమ్యం_స్థానభ్రంశం(reuse_packfile, pos);
	తదుపరి = pack_pos_గమ్యం_స్థానభ్రంశం(reuse_packfile, pos + 1);

	రికార్డు_reused_object(స్థానభ్రంశం,
			     స్థానభ్రంశం - (hashfile_మొత్తం(బహిర్గతం) - pack_ప్రారంభం));

	cur = స్థానభ్రంశం;
	type = unpack_object_శీర్షిక(reuse_packfile, w_curs, &cur, &కొలత);
	ప్రతిజ్ఞా(type >= 0);

	ఒకవేళ (type == OBJ_OFS_DELTA) {
		off_t పునాది_స్థానభ్రంశం;
		off_t fixup;

		గుర్తులేని అక్షరం శీర్షిక[MAX_PACK_OBJECT_శీర్షిక];
		గుర్తులేని len;

		పునాది_స్థానభ్రంశం = get_delta_పునాది(reuse_packfile, w_curs, &cur, type, స్థానభ్రంశం);
		ప్రతిజ్ఞా(పునాది_స్థానభ్రంశం != 0);

		/* Convert to REF_DELTA if we must... */
		ఒకవేళ (!allow_ofs_delta) {
			uint32_t పునాది_pos;
			నిర్మాణం object_id పునాది_oid;

			ఒకవేళ (స్థానభ్రంశం_గమ్యం_pack_pos(reuse_packfile, పునాది_స్థానభ్రంశం, &పునాది_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)పునాది_స్థానభ్రంశం,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&పునాది_oid, reuse_packfile,
					     pack_pos_గమ్యం_సూచీ(reuse_packfile, పునాది_pos));

			len = encode_లోపల_pack_object_శీర్షిక(శీర్షిక, పరిమాణం(శీర్షిక),
							   OBJ_REF_DELTA, కొలత);
			hashwrite(బహిర్గతం, శీర్షిక, len);
			hashwrite(బహిర్గతం, పునాది_oid.hash, the_hash_algo->rawsz);
			copy_pack_దత్తాంశం(బహిర్గతం, reuse_packfile, w_curs, cur, తదుపరి - cur);
			తిరిగి;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = కనుగొను_reused_స్థానభ్రంశం(స్థానభ్రంశం) -
			కనుగొను_reused_స్థానభ్రంశం(పునాది_స్థానభ్రంశం);
		ఒకవేళ (fixup) {
			గుర్తులేని అక్షరం ofs_శీర్షిక[MAX_PACK_OBJECT_శీర్షిక];
			గుర్తులేని i, ofs_len;
			off_t ofs = స్థానభ్రంశం - పునాది_స్థానభ్రంశం - fixup;

			len = encode_లోపల_pack_object_శీర్షిక(శీర్షిక, పరిమాణం(శీర్షిక),
							   OBJ_OFS_DELTA, కొలత);

			i = పరిమాణం(ofs_శీర్షిక) - 1;
			ofs_శీర్షిక[i] = ofs & 127;
			అయితే (ofs >>= 7)
				ofs_శీర్షిక[--i] = 128 | (--ofs & 127);

			ofs_len = పరిమాణం(ofs_శీర్షిక) - i;

			hashwrite(బహిర్గతం, శీర్షిక, len);
			hashwrite(బహిర్గతం, ofs_శీర్షిక + పరిమాణం(ofs_శీర్షిక) - ofs_len, ofs_len);
			copy_pack_దత్తాంశం(బహిర్గతం, reuse_packfile, w_curs, cur, తదుపరి - cur);
			తిరిగి;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_దత్తాంశం(బహిర్గతం, reuse_packfile, w_curs, స్థానభ్రంశం, తదుపరి - స్థానభ్రంశం);
}

స్థిర కొలత_t write_reused_pack_verbatim(నిర్మాణం bitmapped_pack *reuse_packfile,
					 నిర్మాణం hashfile *బహిర్గతం,
					 నిర్మాణం pack_కిటికీ **w_curs)
{
	కొలత_t pos = 0;
	కొలత_t చివర;

	ఒకవేళ (reuse_packfile->bitmap_pos) {
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
		తిరిగి reuse_packfile->bitmap_pos / బిట్లు_లోపల_EWORD;
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
	చివర = reuse_packfile->bitmap_nr / బిట్లు_లోపల_EWORD;
	ఒకవేళ (reuse_packfile_bitmap->మాట_alloc < చివర)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	అయితే (pos < చివర && reuse_packfile_bitmap->పదాలు[pos] == (eword_t)~0)
		pos++;

	ఒకవేళ (pos) {
		off_t గమ్యం_write;

		written = (pos * బిట్లు_లోపల_EWORD);
		గమ్యం_write = pack_pos_గమ్యం_స్థానభ్రంశం(reuse_packfile->p, written)
			- పరిమాణం(నిర్మాణం pack_శీర్షిక);

		/* We're recording one chunk, not one object. */
		రికార్డు_reused_object(పరిమాణం(నిర్మాణం pack_శీర్షిక), 0);
		hashflush(బహిర్గతం);
		copy_pack_దత్తాంశం(బహిర్గతం, reuse_packfile->p, w_curs,
			పరిమాణం(నిర్మాణం pack_శీర్షిక), గమ్యం_write);

		display_పురోగతి(పురోగతి_state, written);
	}
	తిరిగి pos;
}

స్థిర శూన్య write_reused_pack(నిర్మాణం bitmapped_pack *reuse_packfile,
			      నిర్మాణం hashfile *f)
{
	కొలత_t i = reuse_packfile->bitmap_pos / బిట్లు_లోపల_EWORD;
	uint32_t స్థానభ్రంశం;
	off_t pack_ప్రారంభం = hashfile_మొత్తం(f) - పరిమాణం(నిర్మాణం pack_శీర్షిక);
	నిర్మాణం pack_కిటికీ *w_curs = ఖాళీ;

	ఒకవేళ (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	కొరకు (; i < reuse_packfile_bitmap->మాట_alloc; ++i) {
		eword_t మాట = reuse_packfile_bitmap->పదాలు[i];
		కొలత_t pos = (i * బిట్లు_లోపల_EWORD);

		కొరకు (స్థానభ్రంశం = 0; స్థానభ్రంశం < బిట్లు_లోపల_EWORD; ++స్థానభ్రంశం) {
			uint32_t pack_pos;
			ఒకవేళ ((మాట >> స్థానభ్రంశం) == 0)
				ఆపు;

			స్థానభ్రంశం += ewah_బిట్టు_ctz64(మాట >> స్థానభ్రంశం);
			ఒకవేళ (pos + స్థానభ్రంశం < reuse_packfile->bitmap_pos)
				కొనసాగు;
			ఒకవేళ (pos + స్థానభ్రంశం >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				వెళ్ళు పూర్తి;

			ఒకవేళ (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				నిర్మాణం multi_pack_సూచీ *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				ఒకవేళ (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_గమ్యం_midx(m, pos + స్థానభ్రంశం);
				pack_ofs = nth_midxed_స్థానభ్రంశం(m, midx_pos);

				ఒకవేళ (స్థానభ్రంశం_గమ్యం_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} లేకపోతే {
				/*
				 * Can use bit positions directly, even for MIDX
				 * bitmaps. See comment in try_partial_reuse()
				 * for why.
				 */
				pack_pos = pos + స్థానభ్రంశం;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_ప్రారంభం, &w_curs);
			display_పురోగతి(పురోగతి_state, ++written);
		}
	}

పూర్తి:
	unuse_pack(&w_curs);
}

స్థిర శూన్య write_excluded_by_configs(శూన్య)
{
	నిర్మాణం oidset_iter iter;
	స్థిరరాశి నిర్మాణం object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	అయితే ((oid = oidset_iter_తదుపరి(&iter))) {
		నిర్మాణం configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		ఒకవేళ (!ex)
			BUG("configured exclusion wasn't configured");
		write_లోపల_నిండిన(1, ex->pack_hash_hex, అక్షరశ్రేణి_పొడవు(ex->pack_hash_hex));
		write_లోపల_నిండిన(1, " ", 1);
		write_లోపల_నిండిన(1, ex->uri, అక్షరశ్రేణి_పొడవు(ex->uri));
		write_లోపల_నిండిన(1, "\n", 1);
	}
}

స్థిర స్థిరరాశి అక్షరం no_విభజించు_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

స్థిర శూన్య write_pack_file(శూన్య)
{
	uint32_t i = 0, j;
	నిర్మాణం hashfile *f;
	off_t స్థానభ్రంశం;
	uint32_t nr_మిగిలిన = nr_ఫలితం;
	సమయం_t చివరి_mtime = 0;
	నిర్మాణం object_నమోదు **write_క్రమం;
	off_t బైట్లు_written = 0;

	ఒకవేళ (పురోగతి > pack_గమ్యం_stdout)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Writing objects"), nr_ఫలితం);
	ALLOC_ARRAY(written_list, గమ్యం_pack.nr_objects);
	write_క్రమం = compute_write_క్రమం();

	చేయి {
		గుర్తులేని అక్షరం hash[GIT_MAX_RAWSZ];
		అక్షరం *pack_tmp_name = ఖాళీ;

		ఒకవేళ (pack_గమ్యం_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			నిర్మాణం hashfd_options opts = {
				.పురోగతి = పురోగతి_state,
				.బఫరు_len = LARGE_PACKET_దత్తాంశం_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} లేకపోతే {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		స్థానభ్రంశం = write_pack_శీర్షిక(f, nr_మిగిలిన);

		ఒకవేళ (reuse_packfiles_nr) {
			ప్రతిజ్ఞా(pack_గమ్యం_stdout);
			కొరకు (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				ఒకవేళ (reused_chunks_nr)
					reuse_packfiles_వాడిన_nr++;
			}
			స్థానభ్రంశం = hashfile_మొత్తం(f);
		}

		nr_written = 0;
		కొరకు (; i < గమ్యం_pack.nr_objects; i++) {
			నిర్మాణం object_నమోదు *e = write_క్రమం[i];
			ఒకవేళ (write_one(f, e, &స్థానభ్రంశం) == WRITE_ONE_BREAK)
				ఆపు;
			display_పురోగతి(పురోగతి_state, written);
		}

		బైట్లు_written += hashfile_మొత్తం(f) +
			the_repository->hash_algo->rawsz;
		ఒకవేళ (pack_గమ్యం_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_లోపల_ప్రవాహం | CSUM_CLOSE);
		} లేకపోతే ఒకవేళ (nr_written == nr_మిగిలిన) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_లోపల_ప్రవాహం | CSUM_FSYNC | CSUM_CLOSE);
		} లేకపోతే {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			పూర్ణాంకం fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_శీర్షిక_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, స్థానభ్రంశం);
			close(fd);
			ఒకవేళ (write_bitmap_సూచీ) {
				ఒకవేళ (write_bitmap_సూచీ != WRITE_BITMAP_QUIET)
					warning(_(no_విభజించు_warning));
				write_bitmap_సూచీ = 0;
			}
		}

		ఒకవేళ (!pack_గమ్యం_stdout) {
			నిర్మాణం stat st;
			నిర్మాణం strbuf tmpname = STRBUF_INIT;
			నిర్మాణం bitmap_writer bitmap_writer;
			అక్షరం *idx_tmp_name = ఖాళీ;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			ఒకవేళ (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} లేకపోతే ఒకవేళ (!చివరి_mtime) {
				చివరి_mtime = st.st_mtime;
			} లేకపోతే {
				నిర్మాణం utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --చివరి_mtime;
				ఒకవేళ (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", పునాది_name,
				    hash_గమ్యం_hex(hash));

			ఒకవేళ (write_bitmap_సూచీ) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &గమ్యం_pack,
						   ఖాళీ);
				bitmap_writer_సమితి_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_సూచీ(&bitmap_writer,
							       written_list);
			}

			ఒకవేళ (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &గమ్యం_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			ఒకవేళ (write_bitmap_సూచీ) {
				కొలత_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_పురోగతి(&పురోగతి_state);

				bitmap_writer_show_పురోగతి(&bitmap_writer,
							    పురోగతి);
				bitmap_writer_select_commits(&bitmap_writer,
							     సూచీకృత_commits,
							     సూచీకృత_commits_nr);
				ఒకవేళ (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_ముగింపు(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_సూచీ = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			విడుదల(idx_tmp_name);
			strbuf_release(&tmpname);
			విడుదల(pack_tmp_name);
			puts(hash_గమ్యం_hex(hash));
		}

		/* mark written objects as written to previous pack */
		కొరకు (j = 0; j < nr_written; j++) {
			written_list[j]->స్థానభ్రంశం = (off_t)-1;
		}
		nr_మిగిలిన -= nr_written;
	} అయితే (nr_మిగిలిన && i < గమ్యం_pack.nr_objects);

	విడుదల(written_list);
	విడుదల(write_క్రమం);
	stop_పురోగతి(&పురోగతి_state);
	ఒకవేళ (written != nr_ఫలితం)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_ఫలితం);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_ఫలితం);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", బైట్లు_written);
}

స్థిర పూర్ణాంకం no_try_delta(స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిర నిర్మాణం attr_check *check;

	ఒకవేళ (!check)
		check = attr_check_initl("delta", ఖాళీ);
	git_check_attr(the_repository->సూచీ, మార్గం, check);
	ఒకవేళ (ATTR_FALSE(check->వస్తువులు[0].విలువ))
		తిరిగి 1;
	తిరిగి 0;
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
స్థిర పూర్ణాంకం have_duplicate_నమోదు(స్థిరరాశి నిర్మాణం object_id *oid,
				పూర్ణాంకం exclude)
{
	నిర్మాణం object_నమోదు *నమోదు_2;

	ఒకవేళ (reuse_packfile_bitmap &&
	    bitmap_walk_కలిగియుంది(bitmap_git, reuse_packfile_bitmap, oid))
		తిరిగి 1;

	నమోదు_2 = packlist_కనుగొను(&గమ్యం_pack, oid);
	ఒకవేళ (!నమోదు_2)
		తిరిగి 0;

	ఒకవేళ (exclude) {
		ఒకవేళ (!నమోదు_2->preferred_పునాది)
			nr_ఫలితం--;
		నమోదు_2->preferred_పునాది = 1;
	}

	తిరిగి 1;
}

స్థిర పూర్ణాంకం want_cruft_object_mtime(నిర్మాణం repository *r,
				   స్థిరరాశి నిర్మాణం object_id *oid,
				   గుర్తులేని flags, uint32_t mtime)
{
	నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(r->objects->ఆధారం);

	కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి) {
		నిర్మాణం packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		కొరకు (; *cache; cache++) {
			నిర్మాణం packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = కనుగొను_pack_నమోదు_one(oid, p);
			ఒకవేళ (!ofs)
				కొనసాగు;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			ఒకవేళ (!p->is_cruft)
				తిరిగి 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			ఒకవేళ (!ఎక్కించు_pack_mtimes(p)) {
				uint32_t pos;
				ఒకవేళ (స్థానభ్రంశం_గమ్యం_pack_pos(p, ofs, &pos) < 0)
					కొనసాగు;
				candidate_mtime = nth_packed_mtime(p, pos);
			} లేకపోతే {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			ఒకవేళ (mtime <= candidate_mtime)
				తిరిగి 0;
		}
	}

	తిరిగి -1;
}

స్థిర పూర్ణాంకం want_దొరికింది_object(స్థిరరాశి నిర్మాణం object_id *oid, పూర్ణాంకం exclude,
			     నిర్మాణం packed_git *p, uint32_t mtime)
{
	ఒకవేళ (exclude)
		తిరిగి 1;
	ఒకవేళ (incremental)
		తిరిగి 0;

	ఒకవేళ (!is_pack_valid(p))
		తిరిగి -1;

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
	ఒకవేళ (local && !p->pack_local)
		తిరిగి 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	ఒకవేళ (ignore_packed_keep_on_disk || ignore_packed_keep_లోపల_core ||
	    ignore_packed_keep_లోపల_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		గుర్తులేని flags = 0;
		ఒకవేళ (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		ఒకవేళ (ignore_packed_keep_లోపల_core)
			flags |= KEPT_PACK_లోపల_CORE;
		ఒకవేళ (ignore_packed_keep_లోపల_core_open)
			flags |= KEPT_PACK_లోపల_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		ఒకవేళ (!ignore_packed_keep_లోపల_core_has_cruft) {
			ఒకవేళ (ignore_packed_keep_on_disk && p->pack_keep)
				తిరిగి 0;
			ఒకవేళ (ignore_packed_keep_లోపల_core && p->pack_keep_లోపల_core)
				తిరిగి 0;
			ఒకవేళ (ignore_packed_keep_లోపల_core_open && p->pack_keep_లోపల_core_open)
				తిరిగి 0;
			ఒకవేళ (has_object_kept_pack(p->repo, oid, flags))
				తిరిగి 0;
		} లేకపోతే {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			ఒకవేళ (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				తిరిగి 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	ఒకవేళ (!local || !have_non_local_packs)
		తిరిగి 1;

	/* we don't know yet; keep looking for more packs */
	తిరిగి -1;
}

స్థిర పూర్ణాంకం want_object_లోపల_pack_one(నిర్మాణం packed_git *p,
				   స్థిరరాశి నిర్మాణం object_id *oid,
				   పూర్ణాంకం exclude,
				   నిర్మాణం packed_git **దొరికింది_pack,
				   off_t *దొరికింది_స్థానభ్రంశం,
				   uint32_t దొరికింది_mtime)
{
	off_t స్థానభ్రంశం;

	ఒకవేళ (p == *దొరికింది_pack)
		స్థానభ్రంశం = *దొరికింది_స్థానభ్రంశం;
	లేకపోతే
		స్థానభ్రంశం = కనుగొను_pack_నమోదు_one(oid, p);

	ఒకవేళ (స్థానభ్రంశం) {
		ఒకవేళ (!*దొరికింది_pack) {
			ఒకవేళ (!is_pack_valid(p))
				తిరిగి -1;
			*దొరికింది_స్థానభ్రంశం = స్థానభ్రంశం;
			*దొరికింది_pack = p;
		}
		తిరిగి want_దొరికింది_object(oid, exclude, p, దొరికింది_mtime);
	}
	తిరిగి -1;
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
స్థిర పూర్ణాంకం want_object_లోపల_pack_mtime(స్థిరరాశి నిర్మాణం object_id *oid,
				     పూర్ణాంకం exclude,
				     నిర్మాణం packed_git **దొరికింది_pack,
				     off_t *దొరికింది_స్థానభ్రంశం,
				     uint32_t దొరికింది_mtime)
{
	నిర్మాణం odb_ఆధారం_files *files =
		odb_ఆధారం_files_downcast(the_repository->objects->ఆధారం);
	పూర్ణాంకం want;
	నిర్మాణం packfile_list_నమోదు *e;

	ఒకవేళ (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs->తదుపరి; dir; dir = dir->తదుపరి)
			ఒకవేళ (!odb_ఆధారం_read_object_info(&dir->loose->పునాది, oid, ఖాళీ, 0, ఖాళీ))
				తిరిగి 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	ఒకవేళ (*దొరికింది_pack) {
		want = want_దొరికింది_object(oid, exclude, *దొరికింది_pack,
					 దొరికింది_mtime);
		ఒకవేళ (want != -1)
			తిరిగి want;

		*దొరికింది_pack = ఖాళీ;
		*దొరికింది_స్థానభ్రంశం = 0;
	}

	కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి) {
		నిర్మాణం multi_pack_సూచీ *m = get_multi_pack_సూచీ(dir->packed);
		నిర్మాణం pack_నమోదు e;

		ఒకవేళ (m && midx_fill_నమోదు(m, oid, &e, ఖాళీ) == MIDX_FILL_తగిలింది) {
			want = want_object_లోపల_pack_one(e.p, oid, exclude, దొరికింది_pack, దొరికింది_స్థానభ్రంశం, దొరికింది_mtime);
			ఒకవేళ (want != -1)
				తిరిగి want;
		}
	}

	కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి) {
		కొరకు (e = dir->packed->packs.తల; e; e = e->తదుపరి) {
			నిర్మాణం packed_git *p = e->pack;
			want = want_object_లోపల_pack_one(p, oid, exclude, దొరికింది_pack, దొరికింది_స్థానభ్రంశం, దొరికింది_mtime);
			ఒకవేళ (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			ఒకవేళ (want != -1)
				తిరిగి want;
		}
	}

	ఒకవేళ (uri_protocols.nr) {
		నిర్మాణం configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		పూర్ణాంకం i;
		స్థిరరాశి అక్షరం *p;

		ఒకవేళ (ex) {
			కొరకు (i = 0; i < uri_protocols.nr; i++) {
				ఒకవేళ (skip_ఉపసర్గ(ex->uri,
						uri_protocols.వస్తువులు[i].string,
						&p) &&
				    *p == ':') {
					oidset_చొప్పించు(&excluded_by_config, oid);
					తిరిగి 0;
				}
			}
		}
	}

	తిరిగి 1;
}

స్థిర అంతఃవరుస పూర్ణాంకం want_object_లోపల_pack(స్థిరరాశి నిర్మాణం object_id *oid,
				      పూర్ణాంకం exclude,
				      నిర్మాణం packed_git **దొరికింది_pack,
				      off_t *దొరికింది_స్థానభ్రంశం)
{
	తిరిగి want_object_లోపల_pack_mtime(oid, exclude, దొరికింది_pack, దొరికింది_స్థానభ్రంశం,
					 0);
}

స్థిర నిర్మాణం object_నమోదు *create_object_నమోదు(స్థిరరాశి నిర్మాణం object_id *oid,
						గణన object_type type,
						uint32_t hash,
						పూర్ణాంకం exclude,
						పూర్ణాంకం no_try_delta,
						నిర్మాణం packed_git *దొరికింది_pack,
						off_t దొరికింది_స్థానభ్రంశం)
{
	నిర్మాణం object_నమోదు *నమోదు_2;

	నమోదు_2 = packlist_alloc(&గమ్యం_pack, oid);
	నమోదు_2->hash = hash;
	oe_సమితి_type(నమోదు_2, type);
	ఒకవేళ (exclude)
		నమోదు_2->preferred_పునాది = 1;
	లేకపోతే
		nr_ఫలితం++;
	ఒకవేళ (దొరికింది_pack) {
		oe_సమితి_లోపల_pack(&గమ్యం_pack, నమోదు_2, దొరికింది_pack);
		నమోదు_2->లోపల_pack_స్థానభ్రంశం = దొరికింది_స్థానభ్రంశం;
	}

	నమోదు_2->no_try_delta = no_try_delta;

	తిరిగి నమోదు_2;
}

స్థిర స్థిరరాశి అక్షరం no_సంవృతం_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

స్థిర శూన్య కూడు_object_నమోదు(స్థిరరాశి నిర్మాణం object_id *oid, గణన object_type type,
			     స్థిరరాశి అక్షరం *name, పూర్ణాంకం exclude)
{
	నిర్మాణం packed_git *దొరికింది_pack = ఖాళీ;
	off_t దొరికింది_స్థానభ్రంశం = 0;

	display_పురోగతి(పురోగతి_state, ++nr_చూసిన);

	ఒకవేళ (have_duplicate_నమోదు(oid, exclude))
		తిరిగి;

	ఒకవేళ (!want_object_లోపల_pack(oid, exclude, &దొరికింది_pack, &దొరికింది_స్థానభ్రంశం)) {
		/* The pack is missing an object, so it will not have closure */
		ఒకవేళ (write_bitmap_సూచీ) {
			ఒకవేళ (write_bitmap_సూచీ != WRITE_BITMAP_QUIET)
				warning(_(no_సంవృతం_warning));
			write_bitmap_సూచీ = 0;
		}
		తిరిగి;
	}

	create_object_నమోదు(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    దొరికింది_pack, దొరికింది_స్థానభ్రంశం);
}

స్థిర పూర్ణాంకం కూడు_object_నమోదు_from_bitmap(స్థిరరాశి నిర్మాణం object_id *oid,
					గణన object_type type,
					పూర్ణాంకం flags వాడని, uint32_t name_hash,
					నిర్మాణం packed_git *pack, off_t స్థానభ్రంశం,
					శూన్య *payload వాడని)
{
	display_పురోగతి(పురోగతి_state, ++nr_చూసిన);

	ఒకవేళ (have_duplicate_నమోదు(oid, 0))
		తిరిగి 0;

	ఒకవేళ (!want_object_లోపల_pack(oid, 0, &pack, &స్థానభ్రంశం))
		తిరిగి 0;

	create_object_నమోదు(oid, type, name_hash, 0, 0, pack, స్థానభ్రంశం);
	తిరిగి 0;
}

నిర్మాణం pbase_వృక్షం_cache {
	నిర్మాణం object_id oid;
	పూర్ణాంకం ref;
	పూర్ణాంకం temporary;
	శూన్య *వృక్షం_దత్తాంశం;
	గుర్తులేని దీర్ఘ వృక్షం_కొలత;
};

స్థిర నిర్మాణం pbase_వృక్షం_cache *(pbase_వృక్షం_cache[256]);
స్థిర పూర్ణాంకం pbase_వృక్షం_cache_ix(స్థిరరాశి నిర్మాణం object_id *oid)
{
	తిరిగి oid->hash[0] % ARRAY_కొలత(pbase_వృక్షం_cache);
}
స్థిర పూర్ణాంకం pbase_వృక్షం_cache_ix_incr(పూర్ణాంకం ix)
{
	తిరిగి (ix+1) % ARRAY_కొలత(pbase_వృక్షం_cache);
}

స్థిర నిర్మాణం pbase_వృక్షం {
	నిర్మాణం pbase_వృక్షం *తదుపరి;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	నిర్మాణం pbase_వృక్షం_cache pcache;
} *pbase_వృక్షం;

స్థిర నిర్మాణం pbase_వృక్షం_cache *pbase_వృక్షం_get(స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం pbase_వృక్షం_cache *ent, *nent;
	శూన్య *దత్తాంశం;
	గుర్తులేని దీర్ఘ కొలత;
	కొలత_t కొలత_st = 0;
	గణన object_type type;
	పూర్ణాంకం neigh;
	పూర్ణాంకం my_ix = pbase_వృక్షం_cache_ix(oid);
	పూర్ణాంకం అందుబాటు_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	కొరకు (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_వృక్షం_cache[my_ix];
		ఒకవేళ (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			తిరిగి ent;
		}
		లేకపోతే ఒకవేళ (((అందుబాటు_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= అందుబాటు_ix) &&
			  (!ent && pbase_వృక్షం_cache[అందుబాటు_ix])))
			అందుబాటు_ix = my_ix;
		ఒకవేళ (!ent)
			ఆపు;
		my_ix = pbase_వృక్షం_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	దత్తాంశం = odb_read_object(the_repository->objects, oid, &type, &కొలత_st);
	కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
	ఒకవేళ (!దత్తాంశం)
		తిరిగి ఖాళీ;
	ఒకవేళ (type != OBJ_వృక్షం) {
		విడుదల(దత్తాంశం);
		తిరిగి ఖాళీ;
	}

	/* We need to either cache or return a throwaway copy */

	ఒకవేళ (అందుబాటు_ix < 0)
		ent = ఖాళీ;
	లేకపోతే {
		ent = pbase_వృక్షం_cache[అందుబాటు_ix];
		my_ix = అందుబాటు_ix;
	}

	ఒకవేళ (!ent) {
		nent = xmalloc(పరిమాణం(*nent));
		nent->temporary = (అందుబాటు_ix < 0);
	}
	లేకపోతే {
		/* evict and reuse */
		విడుదల(ent->వృక్షం_దత్తాంశం);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->వృక్షం_దత్తాంశం = దత్తాంశం;
	nent->వృక్షం_కొలత = కొలత;
	nent->ref = 1;
	ఒకవేళ (!nent->temporary)
		pbase_వృక్షం_cache[my_ix] = nent;
	తిరిగి nent;
}

స్థిర శూన్య pbase_వృక్షం_put(నిర్మాణం pbase_వృక్షం_cache *cache)
{
	ఒకవేళ (!cache->temporary) {
		cache->ref--;
		తిరిగి;
	}
	విడుదల(cache->వృక్షం_దత్తాంశం);
	విడుదల(cache);
}

స్థిర కొలత_t name_cmp_len(స్థిరరాశి అక్షరం *name)
{
	తిరిగి strcspn(name, "\n/");
}

స్థిర శూన్య కూడు_pbase_object(నిర్మాణం వృక్షం_desc *వృక్షం,
			     స్థిరరాశి అక్షరం *name,
			     కొలత_t cmplen,
			     స్థిరరాశి అక్షరం *fullname)
{
	నిర్మాణం name_నమోదు నమోదు_2;
	పూర్ణాంకం cmp;

	అయితే (వృక్షం_నమోదు(వృక్షం,&నమోదు_2)) {
		ఒకవేళ (S_ISGITLINK(నమోదు_2.బహుళకం))
			కొనసాగు;
		cmp = వృక్షం_నమోదు_len(&నమోదు_2) != cmplen ? 1 :
		      memcmp(name, నమోదు_2.మార్గం, cmplen);
		ఒకవేళ (cmp > 0)
			కొనసాగు;
		ఒకవేళ (cmp < 0)
			తిరిగి;
		ఒకవేళ (name[cmplen] != '/') {
			కూడు_object_నమోదు(&నమోదు_2.oid,
					 object_type(నమోదు_2.బహుళకం),
					 fullname, 1);
			తిరిగి;
		}
		ఒకవేళ (S_ISDIR(నమోదు_2.బహుళకం)) {
			నిర్మాణం వృక్షం_desc sub;
			నిర్మాణం pbase_వృక్షం_cache *వృక్షం;
			స్థిరరాశి అక్షరం *కిందికి = name+cmplen+1;
			కొలత_t downlen = name_cmp_len(కిందికి);

			వృక్షం = pbase_వృక్షం_get(&నమోదు_2.oid);
			ఒకవేళ (!వృక్షం)
				తిరిగి;
			init_వృక్షం_desc(&sub, &వృక్షం->oid,
				       వృక్షం->వృక్షం_దత్తాంశం, వృక్షం->వృక్షం_కొలత);

			కూడు_pbase_object(&sub, కిందికి, downlen, fullname);
			pbase_వృక్షం_put(వృక్షం);
		}
	}
}

స్థిర గుర్తులేని *పూర్తి_pbase_paths;
స్థిర పూర్ణాంకం పూర్తి_pbase_paths_అంకె;
స్థిర పూర్ణాంకం పూర్తి_pbase_paths_alloc;
స్థిర పూర్ణాంకం పూర్తి_pbase_మార్గం_pos(గుర్తులేని hash)
{
	పూర్ణాంకం lo = 0;
	పూర్ణాంకం hi = పూర్తి_pbase_paths_అంకె;
	అయితే (lo < hi) {
		పూర్ణాంకం mi = lo + (hi - lo) / 2;
		ఒకవేళ (పూర్తి_pbase_paths[mi] == hash)
			తిరిగి mi;
		ఒకవేళ (పూర్తి_pbase_paths[mi] < hash)
			hi = mi;
		లేకపోతే
			lo = mi + 1;
	}
	తిరిగి -lo-1;
}

స్థిర పూర్ణాంకం check_pbase_మార్గం(గుర్తులేని hash)
{
	పూర్ణాంకం pos = పూర్తి_pbase_మార్గం_pos(hash);
	ఒకవేళ (0 <= pos)
		తిరిగి 1;
	pos = -pos - 1;
	ALLOC_GROW(పూర్తి_pbase_paths,
		   పూర్తి_pbase_paths_అంకె + 1,
		   పూర్తి_pbase_paths_alloc);
	పూర్తి_pbase_paths_అంకె++;
	ఒకవేళ (pos < పూర్తి_pbase_paths_అంకె)
		MOVE_ARRAY(పూర్తి_pbase_paths + pos + 1, పూర్తి_pbase_paths + pos,
			   పూర్తి_pbase_paths_అంకె - pos - 1);
	పూర్తి_pbase_paths[pos] = hash;
	తిరిగి 0;
}

స్థిర శూన్య కూడు_preferred_పునాది_object(స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం pbase_వృక్షం *it;
	కొలత_t cmplen;
	గుర్తులేని hash = pack_name_hash_fn(name);

	ఒకవేళ (!అంకె_preferred_పునాది || check_pbase_మార్గం(hash))
		తిరిగి;

	cmplen = name_cmp_len(name);
	కొరకు (it = pbase_వృక్షం; it; it = it->తదుపరి) {
		ఒకవేళ (cmplen == 0) {
			కూడు_object_నమోదు(&it->pcache.oid, OBJ_వృక్షం, ఖాళీ, 1);
		}
		లేకపోతే {
			నిర్మాణం వృక్షం_desc వృక్షం;
			init_వృక్షం_desc(&వృక్షం, &it->pcache.oid,
				       it->pcache.వృక్షం_దత్తాంశం, it->pcache.వృక్షం_కొలత);
			కూడు_pbase_object(&వృక్షం, name, cmplen, name);
		}
	}
}

స్థిర శూన్య కూడు_preferred_పునాది(నిర్మాణం object_id *oid)
{
	నిర్మాణం pbase_వృక్షం *it;
	శూన్య *దత్తాంశం;
	గుర్తులేని దీర్ఘ కొలత;
	కొలత_t కొలత_st = 0;
	నిర్మాణం object_id వృక్షం_oid;

	ఒకవేళ (కిటికీ <= అంకె_preferred_పునాది++)
		తిరిగి;

	దత్తాంశం = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_వృక్షం, &కొలత_st, &వృక్షం_oid);
	కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
	ఒకవేళ (!దత్తాంశం)
		తిరిగి;

	కొరకు (it = pbase_వృక్షం; it; it = it->తదుపరి) {
		ఒకవేళ (oideq(&it->pcache.oid, &వృక్షం_oid)) {
			విడుదల(దత్తాంశం);
			తిరిగి;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->తదుపరి = pbase_వృక్షం;
	pbase_వృక్షం = it;

	oidcpy(&it->pcache.oid, &వృక్షం_oid);
	it->pcache.వృక్షం_దత్తాంశం = దత్తాంశం;
	it->pcache.వృక్షం_కొలత = కొలత;
}

స్థిర శూన్య cleanup_preferred_పునాది(శూన్య)
{
	నిర్మాణం pbase_వృక్షం *it;
	గుర్తులేని i;

	it = pbase_వృక్షం;
	pbase_వృక్షం = ఖాళీ;
	అయితే (it) {
		నిర్మాణం pbase_వృక్షం *tmp = it;
		it = tmp->తదుపరి;
		విడుదల(tmp->pcache.వృక్షం_దత్తాంశం);
		విడుదల(tmp);
	}

	కొరకు (i = 0; i < ARRAY_కొలత(pbase_వృక్షం_cache); i++) {
		ఒకవేళ (!pbase_వృక్షం_cache[i])
			కొనసాగు;
		విడుదల(pbase_వృక్షం_cache[i]->వృక్షం_దత్తాంశం);
		FREE_AND_NULL(pbase_వృక్షం_cache[i]);
	}

	FREE_AND_NULL(పూర్తి_pbase_paths);
	పూర్తి_pbase_paths_అంకె = పూర్తి_pbase_paths_alloc = 0;
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
స్థిర పూర్ణాంకం can_reuse_delta(స్థిరరాశి నిర్మాణం object_id *పునాది_oid,
			   నిర్మాణం object_నమోదు *delta,
			   నిర్మాణం object_నమోదు **పునాది_బహిర్గతం)
{
	నిర్మాణం object_నమోదు *పునాది;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	పునాది = packlist_కనుగొను(&గమ్యం_pack, పునాది_oid);
	ఒకవేళ (పునాది) {
		ఒకవేళ (!లోపల_same_island(&delta->idx.oid, &పునాది->idx.oid))
			తిరిగి 0;
		*పునాది_బహిర్గతం = పునాది;
		తిరిగి 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	ఒకవేళ (thin && bitmap_has_oid_లోపల_uninteresting(bitmap_git, పునాది_oid)) {
		ఒకవేళ (use_delta_islands) {
			ఒకవేళ (!లోపల_same_island(&delta->idx.oid, పునాది_oid))
				తిరిగి 0;
		}
		*పునాది_బహిర్గతం = ఖాళీ;
		తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర శూన్య prefetch_గమ్యం_pack(uint32_t object_సూచీ_ప్రారంభం) {
	నిర్మాణం oid_array గమ్యం_fetch = OID_ARRAY_INIT;
	uint32_t i;

	కొరకు (i = object_సూచీ_ప్రారంభం; i < గమ్యం_pack.nr_objects; i++) {
		నిర్మాణం object_నమోదు *నమోదు_2 = గమ్యం_pack.objects + i;

		ఒకవేళ (!odb_read_object_info_extended(the_repository->objects,
						   &నమోదు_2->idx.oid,
						   ఖాళీ,
						   OBJECT_INFO_FOR_PREFETCH))
			కొనసాగు;
		oid_array_append(&గమ్యం_fetch, &నమోదు_2->idx.oid);
	}
	promisor_remote_get_ప్రత్యక్ష(the_repository,
				   గమ్యం_fetch.oid, గమ్యం_fetch.nr);
	oid_array_clear(&గమ్యం_fetch);
}

స్థిర శూన్య check_object(నిర్మాణం object_నమోదు *నమోదు_2, uint32_t object_సూచీ)
{
	కొలత_t canonical_కొలత;
	గణన object_type type;
	నిర్మాణం object_info oi = {.typep = &type, .sizep = &canonical_కొలత};

	ఒకవేళ (లోపల_PACK(నమోదు_2)) {
		నిర్మాణం packed_git *p = లోపల_PACK(నమోదు_2);
		నిర్మాణం pack_కిటికీ *w_curs = ఖాళీ;
		పూర్ణాంకం have_పునాది = 0;
		నిర్మాణం object_id పునాది_ref;
		నిర్మాణం object_నమోదు *పునాది_నమోదు;
		కొలత_t వాడిన_2, వాడిన_0, avail;
		off_t ofs;
		గుర్తులేని అక్షరం *buf, c;
		గణన object_type type;
		కొలత_t లోపల_pack_కొలత;

		buf = use_pack(p, &w_curs, నమోదు_2->లోపల_pack_స్థానభ్రంశం, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		వాడిన_2 = unpack_object_శీర్షిక_బఫరు(buf, avail,
						   &type,
						   &లోపల_pack_కొలత);
		ఒకవేళ (వాడిన_2 == 0)
			వెళ్ళు give_పైకి;

		ఒకవేళ (type < 0)
			BUG("invalid type %d", type);
		నమోదు_2->లోపల_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		ఎంపిక (నమోదు_2->లోపల_pack_type) {
		అప్రమేయ:
			/* Not a delta hence we've already got all we need. */
			oe_సమితి_type(నమోదు_2, నమోదు_2->లోపల_pack_type);
			సమితి_కొలత(నమోదు_2, లోపల_pack_కొలత);
			నమోదు_2->లోపల_pack_శీర్షిక_కొలత = వాడిన_2;
			ఒకవేళ (oe_type(నమోదు_2) < OBJ_COMMIT || oe_type(నమోదు_2) > OBJ_BLOB)
				వెళ్ళు give_పైకి;
			unuse_pack(&w_curs);
			తిరిగి;
		స్థితి OBJ_REF_DELTA:
			ఒకవేళ (reuse_delta && !నమోదు_2->preferred_పునాది) {
				oidread(&పునాది_ref,
					use_pack(p, &w_curs,
						 నమోదు_2->లోపల_pack_స్థానభ్రంశం + వాడిన_2,
						 ఖాళీ),
					the_repository->hash_algo);
				have_పునాది = 1;
			}
			నమోదు_2->లోపల_pack_శీర్షిక_కొలత = వాడిన_2 + the_hash_algo->rawsz;
			ఆపు;
		స్థితి OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       నమోదు_2->లోపల_pack_స్థానభ్రంశం + వాడిన_2, ఖాళీ);
			వాడిన_0 = 0;
			c = buf[వాడిన_0++];
			ofs = c & 127;
			అయితే (c & 128) {
				ofs += 1;
				ఒకవేళ (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_గమ్యం_hex(&నమోదు_2->idx.oid));
					వెళ్ళు give_పైకి;
				}
				c = buf[వాడిన_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = నమోదు_2->లోపల_pack_స్థానభ్రంశం - ofs;
			ఒకవేళ (ofs <= 0 || ofs >= నమోదు_2->లోపల_pack_స్థానభ్రంశం) {
				error(_("delta base offset out of bound for %s"),
				      oid_గమ్యం_hex(&నమోదు_2->idx.oid));
				వెళ్ళు give_పైకి;
			}
			ఒకవేళ (reuse_delta && !నమోదు_2->preferred_పునాది) {
				uint32_t pos;
				ఒకవేళ (స్థానభ్రంశం_గమ్యం_pack_pos(p, ofs, &pos) < 0)
					వెళ్ళు give_పైకి;
				ఒకవేళ (!nth_packed_object_id(&పునాది_ref, p,
							  pack_pos_గమ్యం_సూచీ(p, pos)))
					have_పునాది = 1;
			}
			నమోదు_2->లోపల_pack_శీర్షిక_కొలత = వాడిన_2 + వాడిన_0;
			ఆపు;
		}

		ఒకవేళ (have_పునాది &&
		    can_reuse_delta(&పునాది_ref, నమోదు_2, &పునాది_నమోదు)) {
			oe_సమితి_type(నమోదు_2, నమోదు_2->లోపల_pack_type);
			సమితి_కొలత(నమోదు_2, లోపల_pack_కొలత); /* delta size */
			సమితి_DELTA_కొలత(నమోదు_2, లోపల_pack_కొలత);

			ఒకవేళ (పునాది_నమోదు) {
				సమితి_DELTA(నమోదు_2, పునాది_నమోదు);
				నమోదు_2->delta_sibling_idx = పునాది_నమోదు->delta_బిడ్డ_idx;
				సమితి_DELTA_బిడ్డ(పునాది_నమోదు, నమోదు_2);
			} లేకపోతే {
				సమితి_DELTA_EXT(నమోదు_2, &పునాది_ref);
			}

			unuse_pack(&w_curs);
			తిరిగి;
		}

		ఒకవేళ (oe_type(నమోదు_2)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = నమోదు_2->లోపల_pack_స్థానభ్రంశం + నమోదు_2->లోపల_pack_శీర్షిక_కొలత;
			canonical_కొలత = get_కొలత_from_delta(p, &w_curs,
							     delta_pos);
			ఒకవేళ (canonical_కొలత == 0)
				వెళ్ళు give_పైకి;
			సమితి_కొలత(నమోదు_2, canonical_కొలత);
			unuse_pack(&w_curs);
			తిరిగి;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_పైకి:
		unuse_pack(&w_curs);
	}

	ఒకవేళ (odb_read_object_info_extended(the_repository->objects, &నమోదు_2->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		ఒకవేళ (repo_has_promisor_remote(the_repository)) {
			prefetch_గమ్యం_pack(object_సూచీ);
			ఒకవేళ (odb_read_object_info_extended(the_repository->objects, &నమోదు_2->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} లేకపోతే {
			type = -1;
		}
	}
	oe_సమితి_type(నమోదు_2, type);
	ఒకవేళ (నమోదు_2->type_valid) {
		సమితి_కొలత(నమోదు_2, canonical_కొలత);
	} లేకపోతే {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

స్థిర పూర్ణాంకం pack_స్థానభ్రంశం_క్రమీకరించు(స్థిరరాశి శూన్య *_a, స్థిరరాశి శూన్య *_b)
{
	స్థిరరాశి నిర్మాణం object_నమోదు *a = *(నిర్మాణం object_నమోదు **)_a;
	స్థిరరాశి నిర్మాణం object_నమోదు *b = *(నిర్మాణం object_నమోదు **)_b;
	స్థిరరాశి నిర్మాణం packed_git *a_లోపల_pack = లోపల_PACK(a);
	స్థిరరాశి నిర్మాణం packed_git *b_లోపల_pack = లోపల_PACK(b);

	/* avoid filesystem trashing with loose objects */
	ఒకవేళ (!a_లోపల_pack && !b_లోపల_pack)
		తిరిగి oidcmp(&a->idx.oid, &b->idx.oid);

	ఒకవేళ (a_లోపల_pack < b_లోపల_pack)
		తిరిగి -1;
	ఒకవేళ (a_లోపల_pack > b_లోపల_pack)
		తిరిగి 1;
	తిరిగి a->లోపల_pack_స్థానభ్రంశం < b->లోపల_pack_స్థానభ్రంశం ? -1 :
			(a->లోపల_pack_స్థానభ్రంశం > b->లోపల_pack_స్థానభ్రంశం);
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
స్థిర శూన్య drop_reused_delta(నిర్మాణం object_నమోదు *నమోదు_2)
{
	గుర్తులేని *idx = &గమ్యం_pack.objects[నమోదు_2->delta_idx - 1].delta_బిడ్డ_idx;
	నిర్మాణం object_info oi = OBJECT_INFO_INIT;
	గణన object_type type;
	కొలత_t కొలత;

	అయితే (*idx) {
		నిర్మాణం object_నమోదు *oe = &గమ్యం_pack.objects[*idx - 1];

		ఒకవేళ (oe == నమోదు_2)
			*idx = oe->delta_sibling_idx;
		లేకపోతే
			idx = &oe->delta_sibling_idx;
	}
	సమితి_DELTA(నమోదు_2, ఖాళీ);
	నమోదు_2->లోతు = 0;

	oi.sizep = &కొలత;
	oi.typep = &type;
	ఒకవేళ (packed_object_info(ఖాళీ, లోపల_PACK(నమోదు_2), నమోదు_2->లోపల_pack_స్థానభ్రంశం, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_సమితి_type(నమోదు_2,
			    odb_read_object_info(the_repository->objects,
						 &నమోదు_2->idx.oid, &కొలత));
	} లేకపోతే {
		oe_సమితి_type(నమోదు_2, type);
	}
	సమితి_కొలత(నమోదు_2, కొలత);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
స్థిర శూన్య break_delta_chains(నిర్మాణం object_నమోదు *నమోదు_2)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t మొత్తం_లోతు;
	నిర్మాణం object_నమోదు *cur, *తదుపరి;

	కొరకు (cur = నమోదు_2, మొత్తం_లోతు = 0;
	     cur;
	     cur = DELTA(cur), మొత్తం_లోతు++) {
		ఒకవేళ (cur->dfs_state == DFS_పూర్తి) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			మొత్తం_లోతు += cur->లోతు;
			ఆపు;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		ఒకవేళ (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		ఒకవేళ (!DELTA(cur)) {
			cur->dfs_state = DFS_పూర్తి;
			ఆపు;
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
		cur->dfs_state = DFS_చురుకైన;
		ఒకవేళ (DELTA(cur)->dfs_state == DFS_చురుకైన) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_పూర్తి;
			ఆపు;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	కొరకు (cur = నమోదు_2; cur; cur = తదుపరి) {
		తదుపరి = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		ఒకవేళ (cur->dfs_state == DFS_పూర్తి)
			ఆపు;
		లేకపోతే ఒకవేళ (cur->dfs_state != DFS_చురుకైన)
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
		cur->లోతు = (మొత్తం_లోతు--) % (లోతు + 1);
		ఒకవేళ (!cur->లోతు)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_పూర్తి;
	}
}

స్థిర శూన్య get_object_details(శూన్య)
{
	uint32_t i;
	నిర్మాణం object_నమోదు **sorted_by_స్థానభ్రంశం;

	ఒకవేళ (పురోగతి)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Counting objects"),
						గమ్యం_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_స్థానభ్రంశం, గమ్యం_pack.nr_objects);
	కొరకు (i = 0; i < గమ్యం_pack.nr_objects; i++)
		sorted_by_స్థానభ్రంశం[i] = గమ్యం_pack.objects + i;
	QSORT(sorted_by_స్థానభ్రంశం, గమ్యం_pack.nr_objects, pack_స్థానభ్రంశం_క్రమీకరించు);

	కొరకు (i = 0; i < గమ్యం_pack.nr_objects; i++) {
		నిర్మాణం object_నమోదు *నమోదు_2 = sorted_by_స్థానభ్రంశం[i];
		check_object(నమోదు_2, i);
		ఒకవేళ (నమోదు_2->type_valid &&
		    oe_కొలత_greater_than(&గమ్యం_pack, నమోదు_2,
					 repo_settings_get_big_file_పరిమితి(the_repository)))
			నమోదు_2->no_try_delta = 1;
		display_పురోగతి(పురోగతి_state, i + 1);
	}
	stop_పురోగతి(&పురోగతి_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	కొరకు (i = 0; i < గమ్యం_pack.nr_objects; i++)
		break_delta_chains(&గమ్యం_pack.objects[i]);

	విడుదల(sorted_by_స్థానభ్రంశం);
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
స్థిర పూర్ణాంకం type_కొలత_క్రమీకరించు(స్థిరరాశి శూన్య *_a, స్థిరరాశి శూన్య *_b)
{
	స్థిరరాశి నిర్మాణం object_నమోదు *a = *(నిర్మాణం object_నమోదు **)_a;
	స్థిరరాశి నిర్మాణం object_నమోదు *b = *(నిర్మాణం object_నమోదు **)_b;
	స్థిరరాశి గణన object_type a_type = oe_type(a);
	స్థిరరాశి గణన object_type b_type = oe_type(b);
	స్థిరరాశి గుర్తులేని దీర్ఘ a_కొలత = కొలత_3(a);
	స్థిరరాశి గుర్తులేని దీర్ఘ b_కొలత = కొలత_3(b);

	ఒకవేళ (a_type > b_type)
		తిరిగి -1;
	ఒకవేళ (a_type < b_type)
		తిరిగి 1;
	ఒకవేళ (a->hash > b->hash)
		తిరిగి -1;
	ఒకవేళ (a->hash < b->hash)
		తిరిగి 1;
	ఒకవేళ (a->preferred_పునాది > b->preferred_పునాది)
		తిరిగి -1;
	ఒకవేళ (a->preferred_పునాది < b->preferred_పునాది)
		తిరిగి 1;
	ఒకవేళ (use_delta_islands) {
		స్థిరరాశి పూర్ణాంకం island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		ఒకవేళ (island_cmp)
			తిరిగి island_cmp;
	}
	ఒకవేళ (a_కొలత > b_కొలత)
		తిరిగి -1;
	ఒకవేళ (a_కొలత < b_కొలత)
		తిరిగి 1;
	తిరిగి a < b ? -1 : (a > b);  /* newest first */
}

నిర్మాణం unpacked {
	నిర్మాణం object_నమోదు *నమోదు_2;
	శూన్య *దత్తాంశం;
	నిర్మాణం delta_సూచీ *సూచీ;
	గుర్తులేని లోతు;
};

స్థిర పూర్ణాంకం delta_cacheable(కొలత_t src_కొలత, కొలత_t trg_కొలత,
			   కొలత_t delta_కొలత_2)
{
	ఒకవేళ (max_delta_cache_కొలత && delta_cache_కొలత + delta_కొలత_2 > max_delta_cache_కొలత)
		తిరిగి 0;

	ఒకవేళ (delta_కొలత_2 < cache_max_small_delta_కొలత)
		తిరిగి 1;

	/* cache delta, if objects are large enough compared to delta size */
	ఒకవేళ ((src_కొలత >> 20) + (trg_కొలత >> 21) > (delta_కొలత_2 >> 10))
		తిరిగి 1;

	తిరిగి 0;
}

/* Protect delta_cache_size */
స్థిర pthread_mutex_t cache_mutex;
#నిర్వచించు cache_తాళం()		pthread_mutex_తాళం(&cache_mutex)
#నిర్వచించు cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
స్థిర pthread_mutex_t పురోగతి_mutex;
#నిర్వచించు పురోగతి_తాళం()		pthread_mutex_తాళం(&పురోగతి_mutex)
#నిర్వచించు పురోగతి_unlock()	pthread_mutex_unlock(&పురోగతి_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

స్థిర అంతఃవరుస పూర్ణాంకం oe_కొలత_less_than(నిర్మాణం packing_దత్తాంశం *pack,
				    స్థిరరాశి నిర్మాణం object_నమోదు *lhs,
				    కొలత_t rhs)
{
	ఒకవేళ (lhs->కొలత_valid)
		తిరిగి lhs->కొలత_2 < rhs;
	ఒకవేళ (rhs < pack->oe_కొలత_హద్దు) /* rhs < 2^x <= lhs ? */
		తిరిగి 0;
	తిరిగి oe_get_కొలత_slow(pack, lhs) < rhs;
}

స్థిర అంతఃవరుస శూన్య oe_సమితి_వృక్షం_లోతు(నిర్మాణం packing_దత్తాంశం *pack,
				     నిర్మాణం object_నమోదు *e,
				     గుర్తులేని పూర్ణాంకం వృక్షం_లోతు)
{
	ఒకవేళ (!pack->వృక్షం_లోతు)
		CALLOC_ARRAY(pack->వృక్షం_లోతు, pack->nr_alloc);
	pack->వృక్షం_లోతు[e - pack->objects] = వృక్షం_లోతు;
}

స్థిర శూన్య రికార్డు_వృక్షం_లోతు(స్థిరరాశి నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *name)
{
	స్థిరరాశి అక్షరం *p;
	గుర్తులేని లోతు;
	నిర్మాణం object_నమోదు *ent;

	/* the empty string is a root tree, which is depth 0 */
	లోతు = *name ? 1 : 0;
	కొరకు (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		లోతు++;

	ent = packlist_కనుగొను(&గమ్యం_pack, oid);
	ఒకవేళ (ent && లోతు > oe_వృక్షం_లోతు(&గమ్యం_pack, ent))
		oe_సమితి_వృక్షం_లోతు(&గమ్యం_pack, ent, లోతు);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
కొలత_t oe_get_కొలత_slow(నిర్మాణం packing_దత్తాంశం *pack,
			స్థిరరాశి నిర్మాణం object_నమోదు *e)
{
	నిర్మాణం packed_git *p;
	నిర్మాణం pack_కిటికీ *w_curs;
	గుర్తులేని అక్షరం *buf;
	గణన object_type type;
	కొలత_t వాడిన_2, avail, కొలత;

	ఒకవేళ (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		కొలత_t sz;
		packing_దత్తాంశం_తాళం(&గమ్యం_pack);
		ఒకవేళ (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_గమ్యం_hex(&e->idx.oid));
		packing_దత్తాంశం_unlock(&గమ్యం_pack);
		తిరిగి sz;
	}

	p = oe_లోపల_pack(pack, e);
	ఒకవేళ (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_దత్తాంశం_తాళం(&గమ్యం_pack);
	w_curs = ఖాళీ;
	buf = use_pack(p, &w_curs, e->లోపల_pack_స్థానభ్రంశం, &avail);
	వాడిన_2 = unpack_object_శీర్షిక_బఫరు(buf, avail, &type, &కొలత);
	ఒకవేళ (వాడిన_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_గమ్యం_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_దత్తాంశం_unlock(&గమ్యం_pack);
	తిరిగి కొలత;
}

స్థిర పూర్ణాంకం try_delta(నిర్మాణం unpacked *trg, నిర్మాణం unpacked *src,
		     గుర్తులేని max_లోతు, కొలత_t *mem_usage)
{
	నిర్మాణం object_నమోదు *trg_నమోదు = trg->నమోదు_2;
	నిర్మాణం object_నమోదు *src_నమోదు = src->నమోదు_2;
	గుర్తులేని దీర్ఘ trg_కొలత, src_కొలత, sizediff, max_కొలత, sz;
	కొలత_t delta_కొలత_2;
	గుర్తులేని ref_లోతు;
	గణన object_type type;
	శూన్య *delta_buf;

	/* Don't bother doing diffs between different types */
	ఒకవేళ (oe_type(trg_నమోదు) != oe_type(src_నమోదు))
		తిరిగి -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	ఒకవేళ (reuse_delta && లోపల_PACK(trg_నమోదు) &&
	    లోపల_PACK(trg_నమోదు) == లోపల_PACK(src_నమోదు) &&
	    !src_నమోదు->preferred_పునాది &&
	    trg_నమోదు->లోపల_pack_type != OBJ_REF_DELTA &&
	    trg_నమోదు->లోపల_pack_type != OBJ_OFS_DELTA)
		తిరిగి 0;

	/* Let's not bust the allowed depth. */
	ఒకవేళ (src->లోతు >= max_లోతు)
		తిరిగి 0;

	/* Now some size filtering heuristics. */
	trg_కొలత = కొలత_3(trg_నమోదు);
	ఒకవేళ (!DELTA(trg_నమోదు)) {
		max_కొలత = trg_కొలత/2 - the_hash_algo->rawsz;
		ref_లోతు = 1;
	} లేకపోతే {
		max_కొలత = DELTA_కొలత(trg_నమోదు);
		ref_లోతు = trg->లోతు;
	}
	max_కొలత = (uint64_t)max_కొలత * (max_లోతు - src->లోతు) /
						(max_లోతు - ref_లోతు + 1);
	ఒకవేళ (max_కొలత == 0)
		తిరిగి 0;
	src_కొలత = కొలత_3(src_నమోదు);
	sizediff = src_కొలత < trg_కొలత ? trg_కొలత - src_కొలత : 0;
	ఒకవేళ (sizediff >= max_కొలత)
		తిరిగి 0;
	ఒకవేళ (trg_కొలత < src_కొలత / 32)
		తిరిగి 0;

	ఒకవేళ (!లోపల_same_island(&trg->నమోదు_2->idx.oid, &src->నమోదు_2->idx.oid))
		తిరిగి 0;

	/* Load data if not already done */
	ఒకవేళ (!trg->దత్తాంశం) {
		కొలత_t sz_st = 0;
		packing_దత్తాంశం_తాళం(&గమ్యం_pack);
		trg->దత్తాంశం = odb_read_object(the_repository->objects,
					    &trg_నమోదు->idx.oid, &type,
					    &sz_st);
		sz = cast_కొలత_t_గమ్యం_ulong(sz_st);
		packing_దత్తాంశం_unlock(&గమ్యం_pack);
		ఒకవేళ (!trg->దత్తాంశం)
			die(_("object %s cannot be read"),
			    oid_గమ్యం_hex(&trg_నమోదు->idx.oid));
		ఒకవేళ (sz != trg_కొలత)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_గమ్యం_hex(&trg_నమోదు->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_కొలత);
		*mem_usage += sz;
	}
	ఒకవేళ (!src->దత్తాంశం) {
		కొలత_t sz_st = 0;
		packing_దత్తాంశం_తాళం(&గమ్యం_pack);
		src->దత్తాంశం = odb_read_object(the_repository->objects,
					    &src_నమోదు->idx.oid, &type,
					    &sz_st);
		sz = cast_కొలత_t_గమ్యం_ulong(sz_st);
		packing_దత్తాంశం_unlock(&గమ్యం_pack);
		ఒకవేళ (!src->దత్తాంశం) {
			ఒకవేళ (src_నమోదు->preferred_పునాది) {
				స్థిర పూర్ణాంకం warned = 0;
				ఒకవేళ (!warned++)
					warning(_("object %s cannot be read"),
						oid_గమ్యం_hex(&src_నమోదు->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				తిరిగి 0;
			}
			die(_("object %s cannot be read"),
			    oid_గమ్యం_hex(&src_నమోదు->idx.oid));
		}
		ఒకవేళ (sz != src_కొలత)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_గమ్యం_hex(&src_నమోదు->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_కొలత);
		*mem_usage += sz;
	}
	ఒకవేళ (!src->సూచీ) {
		src->సూచీ = create_delta_సూచీ(src->దత్తాంశం, src_కొలత);
		ఒకవేళ (!src->సూచీ) {
			స్థిర పూర్ణాంకం warned = 0;
			ఒకవేళ (!warned++)
				warning(_("suboptimal pack - out of memory"));
			తిరిగి 0;
		}
		*mem_usage += sizeof_delta_సూచీ(src->సూచీ);
	}

	delta_buf = create_delta(src->సూచీ, trg->దత్తాంశం, trg_కొలత, &delta_కొలత_2, max_కొలత);
	ఒకవేళ (!delta_buf)
		తిరిగి 0;

	ఒకవేళ (DELTA(trg_నమోదు)) {
		/* Prefer only shallower same-sized deltas. */
		ఒకవేళ (delta_కొలత_2 == DELTA_కొలత(trg_నమోదు) &&
		    src->లోతు + 1 >= trg->లోతు) {
			విడుదల(delta_buf);
			తిరిగి 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	విడుదల(trg_నమోదు->delta_దత్తాంశం);
	cache_తాళం();
	ఒకవేళ (trg_నమోదు->delta_దత్తాంశం) {
		delta_cache_కొలత -= DELTA_కొలత(trg_నమోదు);
		trg_నమోదు->delta_దత్తాంశం = ఖాళీ;
	}
	ఒకవేళ (delta_cacheable(src_కొలత, trg_కొలత, delta_కొలత_2)) {
		delta_cache_కొలత += delta_కొలత_2;
		cache_unlock();
		trg_నమోదు->delta_దత్తాంశం = xrealloc(delta_buf, delta_కొలత_2);
	} లేకపోతే {
		cache_unlock();
		విడుదల(delta_buf);
	}

	సమితి_DELTA(trg_నమోదు, src_నమోదు);
	సమితి_DELTA_కొలత(trg_నమోదు, delta_కొలత_2);
	trg->లోతు = src->లోతు + 1;

	తిరిగి 1;
}

స్థిర గుర్తులేని పూర్ణాంకం check_delta_హద్దు(నిర్మాణం object_నమోదు *me, గుర్తులేని పూర్ణాంకం n)
{
	నిర్మాణం object_నమోదు *బిడ్డ = DELTA_బిడ్డ(me);
	గుర్తులేని పూర్ణాంకం m = n;
	అయితే (బిడ్డ) {
		స్థిరరాశి గుర్తులేని పూర్ణాంకం c = check_delta_హద్దు(బిడ్డ, n + 1);
		ఒకవేళ (m < c)
			m = c;
		బిడ్డ = DELTA_SIBLING(బిడ్డ);
	}
	తిరిగి m;
}

స్థిర కొలత_t free_unpacked(నిర్మాణం unpacked *n)
{
	కొలత_t freed_mem = sizeof_delta_సూచీ(n->సూచీ);
	free_delta_సూచీ(n->సూచీ);
	n->సూచీ = ఖాళీ;
	ఒకవేళ (n->దత్తాంశం) {
		freed_mem += కొలత_3(n->నమోదు_2);
		FREE_AND_NULL(n->దత్తాంశం);
	}
	n->నమోదు_2 = ఖాళీ;
	n->లోతు = 0;
	తిరిగి freed_mem;
}

స్థిర శూన్య కనుగొను_deltas(నిర్మాణం object_నమోదు **list, గుర్తులేని *list_కొలత,
			పూర్ణాంకం కిటికీ, పూర్ణాంకం లోతు, గుర్తులేని *processed)
{
	uint32_t i, idx = 0, లెక్క = 0;
	నిర్మాణం unpacked *array;
	కొలత_t mem_usage = 0;

	CALLOC_ARRAY(array, కిటికీ);

	కొరకు (;;) {
		నిర్మాణం object_నమోదు *నమోదు_2;
		నిర్మాణం unpacked *n = array + idx;
		పూర్ణాంకం j, max_లోతు, ఉత్తమ_పునాది = -1;

		పురోగతి_తాళం();
		ఒకవేళ (!*list_కొలత) {
			పురోగతి_unlock();
			ఆపు;
		}
		నమోదు_2 = *list++;
		(*list_కొలత)--;
		ఒకవేళ (!నమోదు_2->preferred_పునాది) {
			(*processed)++;
			display_పురోగతి(పురోగతి_state, *processed);
		}
		పురోగతి_unlock();

		mem_usage -= free_unpacked(n);
		n->నమోదు_2 = నమోదు_2;

		అయితే (కిటికీ_జ్ఞాపకం_హద్దు &&
		       mem_usage > కిటికీ_జ్ఞాపకం_హద్దు &&
		       లెక్క > 1) {
			స్థిరరాశి uint32_t తోక = (idx + కిటికీ - లెక్క) % కిటికీ;
			mem_usage -= free_unpacked(array + తోక);
			లెక్క--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		ఒకవేళ (నమోదు_2->preferred_పునాది)
			వెళ్ళు తదుపరి;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_లోతు = లోతు;
		ఒకవేళ (DELTA_బిడ్డ(నమోదు_2)) {
			max_లోతు -= check_delta_హద్దు(నమోదు_2, 0);
			ఒకవేళ (max_లోతు <= 0)
				వెళ్ళు తదుపరి;
		}

		j = కిటికీ;
		అయితే (--j > 0) {
			పూర్ణాంకం ret;
			uint32_t other_idx = idx + j;
			నిర్మాణం unpacked *m;
			ఒకవేళ (other_idx >= కిటికీ)
				other_idx -= కిటికీ;
			m = array + other_idx;
			ఒకవేళ (!m->నమోదు_2)
				ఆపు;
			ret = try_delta(n, m, max_లోతు, &mem_usage);
			ఒకవేళ (ret < 0)
				ఆపు;
			లేకపోతే ఒకవేళ (ret > 0)
				ఉత్తమ_పునాది = other_idx;
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
		ఒకవేళ (నమోదు_2->delta_దత్తాంశం && !pack_గమ్యం_stdout) {
			గుర్తులేని దీర్ఘ కొలత;

			కొలత = do_compress(&నమోదు_2->delta_దత్తాంశం, DELTA_కొలత(నమోదు_2));
			ఒకవేళ (కొలత < (1U << OE_Z_DELTA_బిట్లు)) {
				నమోదు_2->z_delta_కొలత = కొలత;
				cache_తాళం();
				delta_cache_కొలత -= DELTA_కొలత(నమోదు_2);
				delta_cache_కొలత += నమోదు_2->z_delta_కొలత;
				cache_unlock();
			} లేకపోతే {
				FREE_AND_NULL(నమోదు_2->delta_దత్తాంశం);
				నమోదు_2->z_delta_కొలత = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		ఒకవేళ (DELTA(నమోదు_2) && max_లోతు <= n->లోతు)
			కొనసాగు;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		ఒకవేళ (DELTA(నమోదు_2)) {
			నిర్మాణం unpacked మార్పిడి_2 = array[ఉత్తమ_పునాది];
			పూర్ణాంకం dist = (కిటికీ + idx - ఉత్తమ_పునాది) % కిటికీ;
			పూర్ణాంకం dst = ఉత్తమ_పునాది;
			అయితే (dist--) {
				పూర్ణాంకం src = (dst + 1) % కిటికీ;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = మార్పిడి_2;
		}

		తదుపరి:
		idx++;
		ఒకవేళ (లెక్క + 1 < కిటికీ)
			లెక్క++;
		ఒకవేళ (idx >= కిటికీ)
			idx = 0;
	}

	కొరకు (i = 0; i < కిటికీ; ++i) {
		free_delta_సూచీ(array[i].సూచీ);
		విడుదల(array[i].దత్తాంశం);
	}
	విడుదల(array);
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

నిర్మాణం thread_params {
	pthread_t thread;
	నిర్మాణం object_నమోదు **list;
	నిర్మాణం packing_region *regions;
	గుర్తులేని list_కొలత;
	గుర్తులేని మిగిలిన;
	పూర్ణాంకం కిటికీ;
	పూర్ణాంకం లోతు;
	పూర్ణాంకం working;
	పూర్ణాంకం దత్తాంశం_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	గుర్తులేని *processed;
};

స్థిర pthread_cond_t పురోగతి_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
స్థిర శూన్య init_threaded_శోధన(శూన్య)
{
	pthread_mutex_init(&cache_mutex, ఖాళీ);
	pthread_mutex_init(&పురోగతి_mutex, ఖాళీ);
	pthread_cond_init(&పురోగతి_cond, ఖాళీ);
}

స్థిర శూన్య cleanup_threaded_శోధన(శూన్య)
{
	pthread_cond_destroy(&పురోగతి_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&పురోగతి_mutex);
}

స్థిర శూన్య *threaded_కనుగొను_deltas(శూన్య *arg)
{
	నిర్మాణం thread_params *me = arg;

	పురోగతి_తాళం();
	అయితే (me->మిగిలిన) {
		పురోగతి_unlock();

		కనుగొను_deltas(me->list, &me->మిగిలిన,
			    me->కిటికీ, me->లోతు, me->processed);

		పురోగతి_తాళం();
		me->working = 0;
		pthread_cond_signal(&పురోగతి_cond);
		పురోగతి_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_తాళం(&me->mutex);
		అయితే (!me->దత్తాంశం_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->దత్తాంశం_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		పురోగతి_తాళం();
	}
	పురోగతి_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	తిరిగి ఖాళీ;
}

స్థిర శూన్య ll_కనుగొను_deltas(నిర్మాణం object_నమోదు **list, గుర్తులేని list_కొలత,
			   పూర్ణాంకం కిటికీ, పూర్ణాంకం లోతు, గుర్తులేని *processed)
{
	నిర్మాణం thread_params *p;
	పూర్ణాంకం i, ret, చురుకైన_దారాలు = 0;

	init_threaded_శోధన();

	ఒకవేళ (delta_శోధన_దారాలు <= 1) {
		కనుగొను_deltas(list, &list_కొలత, కిటికీ, లోతు, processed);
		cleanup_threaded_శోధన();
		తిరిగి;
	}
	ఒకవేళ (పురోగతి > pack_గమ్యం_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_శోధన_దారాలు);
	CALLOC_ARRAY(p, delta_శోధన_దారాలు);

	/* Partition the work amongst work threads. */
	కొరకు (i = 0; i < delta_శోధన_దారాలు; i++) {
		గుర్తులేని sub_కొలత = list_కొలత / (delta_శోధన_దారాలు - i);

		/* don't use too small segments or no deltas will be found */
		ఒకవేళ (sub_కొలత < 2*కిటికీ && i+1 < delta_శోధన_దారాలు)
			sub_కొలత = 0;

		p[i].కిటికీ = కిటికీ;
		p[i].లోతు = లోతు;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].దత్తాంశం_ready = 0;

		/* try to split chunks on "path" boundaries */
		అయితే (sub_కొలత && sub_కొలత < list_కొలత &&
		       list[sub_కొలత]->hash &&
		       list[sub_కొలత]->hash == list[sub_కొలత-1]->hash)
			sub_కొలత++;

		p[i].list = list;
		p[i].list_కొలత = sub_కొలత;
		p[i].మిగిలిన = sub_కొలత;

		list += sub_కొలత;
		list_కొలత -= sub_కొలత;
	}

	/* Start work threads. */
	కొరకు (i = 0; i < delta_శోధన_దారాలు; i++) {
		ఒకవేళ (!p[i].list_కొలత)
			కొనసాగు;
		pthread_mutex_init(&p[i].mutex, ఖాళీ);
		pthread_cond_init(&p[i].cond, ఖాళీ);
		ret = pthread_create(&p[i].thread, ఖాళీ,
				     threaded_కనుగొను_deltas, &p[i]);
		ఒకవేళ (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		చురుకైన_దారాలు++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	అయితే (చురుకైన_దారాలు) {
		నిర్మాణం thread_params *లక్ష్యం = ఖాళీ;
		నిర్మాణం thread_params *victim = ఖాళీ;
		గుర్తులేని sub_కొలత = 0;

		పురోగతి_తాళం();
		కొరకు (;;) {
			కొరకు (i = 0; !లక్ష్యం && i < delta_శోధన_దారాలు; i++)
				ఒకవేళ (!p[i].working)
					లక్ష్యం = &p[i];
			ఒకవేళ (లక్ష్యం)
				ఆపు;
			pthread_cond_wait(&పురోగతి_cond, &పురోగతి_mutex);
		}

		కొరకు (i = 0; i < delta_శోధన_దారాలు; i++)
			ఒకవేళ (p[i].మిగిలిన > 2*కిటికీ &&
			    (!victim || victim->మిగిలిన < p[i].మిగిలిన))
				victim = &p[i];
		ఒకవేళ (victim) {
			sub_కొలత = victim->మిగిలిన / 2;
			list = victim->list + victim->list_కొలత - sub_కొలత;
			అయితే (sub_కొలత && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_కొలత--;
			}
			ఒకవేళ (!sub_కొలత) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_కొలత = victim->మిగిలిన / 2;
				list -= sub_కొలత;
			}
			లక్ష్యం->list = list;
			victim->list_కొలత -= sub_కొలత;
			victim->మిగిలిన -= sub_కొలత;
		}
		లక్ష్యం->list_కొలత = sub_కొలత;
		లక్ష్యం->మిగిలిన = sub_కొలత;
		లక్ష్యం->working = 1;
		పురోగతి_unlock();

		pthread_mutex_తాళం(&లక్ష్యం->mutex);
		లక్ష్యం->దత్తాంశం_ready = 1;
		pthread_cond_signal(&లక్ష్యం->cond);
		pthread_mutex_unlock(&లక్ష్యం->mutex);

		ఒకవేళ (!sub_కొలత) {
			pthread_join(లక్ష్యం->thread, ఖాళీ);
			pthread_cond_destroy(&లక్ష్యం->cond);
			pthread_mutex_destroy(&లక్ష్యం->mutex);
			చురుకైన_దారాలు--;
		}
	}
	cleanup_threaded_శోధన();
	విడుదల(p);
}

స్థిర పూర్ణాంకం obj_is_packed(స్థిరరాశి నిర్మాణం object_id *oid)
{
	తిరిగి packlist_కనుగొను(&గమ్యం_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_కలిగియుంది(bitmap_git, reuse_packfile_bitmap, oid));
}

స్థిర శూన్య కూడు_ట్యాగ్_chain(స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం ట్యాగ్ *ట్యాగ్;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	ఒకవేళ (obj_is_packed(oid))
		తిరిగి;

	ట్యాగ్ = lookup_ట్యాగ్(the_repository, oid);
	అయితే (1) {
		ఒకవేళ (!ట్యాగ్ || parse_ట్యాగ్(the_repository, ట్యాగ్) || !ట్యాగ్->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_గమ్యం_hex(oid));

		కూడు_object_నమోదు(&ట్యాగ్->object.oid, OBJ_ట్యాగ్, ఖాళీ, 0);

		ఒకవేళ (ట్యాగ్->tagged->type != OBJ_ట్యాగ్)
			తిరిగి;

		ట్యాగ్ = (నిర్మాణం ట్యాగ్ *)ట్యాగ్->tagged;
	}
}

స్థిర పూర్ణాంకం కూడు_ref_ట్యాగ్(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం వాడని)
{
	నిర్మాణం object_id peeled;

	ఒకవేళ (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		కూడు_ట్యాగ్_chain(ref->oid);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం should_attempt_deltas(నిర్మాణం object_నమోదు *నమోదు_2)
{
	ఒకవేళ (DELTA(నమోదు_2))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		తిరిగి 0;

	ఒకవేళ (!నమోదు_2->type_valid ||
	    oe_కొలత_less_than(&గమ్యం_pack, నమోదు_2, 50))
		తిరిగి 0;

	ఒకవేళ (నమోదు_2->no_try_delta)
		తిరిగి 0;

	ఒకవేళ (!నమోదు_2->preferred_పునాది) {
		ఒకవేళ (oe_type(నమోదు_2) < 0)
			die(_("unable to get type of object %s"),
				oid_గమ్యం_hex(&నమోదు_2->idx.oid));
	} లేకపోతే ఒకవేళ (oe_type(నమోదు_2) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		తిరిగి 0;
	}

	తిరిగి 1;
}

స్థిర శూన్య కనుగొను_deltas_for_region(నిర్మాణం object_నమోదు *list,
				   నిర్మాణం packing_region *region,
				   గుర్తులేని పూర్ణాంకం *processed)
{
	నిర్మాణం object_నమోదు **delta_list;
	గుర్తులేని పూర్ణాంకం delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	కొరకు (కొలత_t i = 0; i < region->nr; i++) {
		నిర్మాణం object_నమోదు *నమోదు_2 = list + region->ప్రారంభం + i;
		ఒకవేళ (should_attempt_deltas(నమోదు_2))
			delta_list[delta_list_nr++] = నమోదు_2;
	}

	QSORT(delta_list, delta_list_nr, type_కొలత_క్రమీకరించు);
	కనుగొను_deltas(delta_list, &delta_list_nr, కిటికీ, లోతు, processed);
	విడుదల(delta_list);
}

స్థిర శూన్య కనుగొను_deltas_by_region(నిర్మాణం object_నమోదు *list,
				  నిర్మాణం packing_region *regions,
				  కొలత_t ప్రారంభం, కొలత_t nr)
{
	గుర్తులేని పూర్ణాంకం processed = 0;
	కొలత_t పురోగతి_nr;

	ఒకవేళ (!nr)
		తిరిగి;

	పురోగతి_nr = regions[nr - 1].ప్రారంభం + regions[nr - 1].nr;

	ఒకవేళ (పురోగతి)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Compressing objects by path"),
						పురోగతి_nr);

	అయితే (nr--)
		కనుగొను_deltas_for_region(list,
				       &regions[ప్రారంభం++],
				       &processed);

	display_పురోగతి(పురోగతి_state, పురోగతి_nr);
	stop_పురోగతి(&పురోగతి_state);
}

స్థిర శూన్య *threaded_కనుగొను_deltas_by_మార్గం(శూన్య *arg)
{
	నిర్మాణం thread_params *me = arg;

	పురోగతి_తాళం();
	అయితే (me->మిగిలిన) {
		అయితే (me->మిగిలిన) {
			పురోగతి_unlock();
			కనుగొను_deltas_for_region(గమ్యం_pack.objects,
					       me->regions,
					       me->processed);
			పురోగతి_తాళం();
			me->మిగిలిన--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&పురోగతి_cond);
		పురోగతి_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_తాళం(&me->mutex);
		అయితే (!me->దత్తాంశం_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->దత్తాంశం_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		పురోగతి_తాళం();
	}
	పురోగతి_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	తిరిగి ఖాళీ;
}

స్థిర శూన్య ll_కనుగొను_deltas_by_region(నిర్మాణం object_నమోదు *list,
				     నిర్మాణం packing_region *regions,
				     uint32_t ప్రారంభం, uint32_t nr)
{
	నిర్మాణం thread_params *p;
	పూర్ణాంకం i, ret, చురుకైన_దారాలు = 0;
	గుర్తులేని పూర్ణాంకం processed = 0;
	uint32_t పురోగతి_nr;
	init_threaded_శోధన();

	ఒకవేళ (!nr)
		తిరిగి;

	పురోగతి_nr =  regions[nr - 1].ప్రారంభం + regions[nr - 1].nr;
	ఒకవేళ (delta_శోధన_దారాలు <= 1) {
		కనుగొను_deltas_by_region(list, regions, ప్రారంభం, nr);
		cleanup_threaded_శోధన();
		తిరిగి;
	}

	ఒకవేళ (పురోగతి > pack_గమ్యం_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_శోధన_దారాలు),
			   delta_శోధన_దారాలు);
	CALLOC_ARRAY(p, delta_శోధన_దారాలు);

	ఒకవేళ (పురోగతి)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Compressing objects by path"),
						పురోగతి_nr);
	/* Partition the work amongst work threads. */
	కొరకు (i = 0; i < delta_శోధన_దారాలు; i++) {
		గుర్తులేని sub_కొలత = nr / (delta_శోధన_దారాలు - i);

		p[i].కిటికీ = కిటికీ;
		p[i].లోతు = లోతు;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].దత్తాంశం_ready = 0;

		p[i].regions = regions;
		p[i].list_కొలత = sub_కొలత;
		p[i].మిగిలిన = sub_కొలత;

		regions += sub_కొలత;
		nr -= sub_కొలత;
	}

	/* Start work threads. */
	కొరకు (i = 0; i < delta_శోధన_దారాలు; i++) {
		ఒకవేళ (!p[i].list_కొలత)
			కొనసాగు;
		pthread_mutex_init(&p[i].mutex, ఖాళీ);
		pthread_cond_init(&p[i].cond, ఖాళీ);
		ret = pthread_create(&p[i].thread, ఖాళీ,
				     threaded_కనుగొను_deltas_by_మార్గం, &p[i]);
		ఒకవేళ (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		చురుకైన_దారాలు++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	అయితే (చురుకైన_దారాలు) {
		నిర్మాణం thread_params *లక్ష్యం = ఖాళీ;
		నిర్మాణం thread_params *victim = ఖాళీ;
		గుర్తులేని sub_కొలత = 0;

		పురోగతి_తాళం();
		కొరకు (;;) {
			కొరకు (i = 0; !లక్ష్యం && i < delta_శోధన_దారాలు; i++)
				ఒకవేళ (!p[i].working)
					లక్ష్యం = &p[i];
			ఒకవేళ (లక్ష్యం)
				ఆపు;
			pthread_cond_wait(&పురోగతి_cond, &పురోగతి_mutex);
		}

		కొరకు (i = 0; i < delta_శోధన_దారాలు; i++)
			ఒకవేళ (p[i].మిగిలిన > 2*కిటికీ &&
			    (!victim || victim->మిగిలిన < p[i].మిగిలిన))
				victim = &p[i];
		ఒకవేళ (victim) {
			sub_కొలత = victim->మిగిలిన / 2;
			లక్ష్యం->regions = victim->regions + victim->మిగిలిన - sub_కొలత;
			victim->list_కొలత -= sub_కొలత;
			victim->మిగిలిన -= sub_కొలత;
		}
		లక్ష్యం->list_కొలత = sub_కొలత;
		లక్ష్యం->మిగిలిన = sub_కొలత;
		లక్ష్యం->working = 1;
		పురోగతి_unlock();

		pthread_mutex_తాళం(&లక్ష్యం->mutex);
		లక్ష్యం->దత్తాంశం_ready = 1;
		pthread_cond_signal(&లక్ష్యం->cond);
		pthread_mutex_unlock(&లక్ష్యం->mutex);

		ఒకవేళ (!sub_కొలత) {
			pthread_join(లక్ష్యం->thread, ఖాళీ);
			pthread_cond_destroy(&లక్ష్యం->cond);
			pthread_mutex_destroy(&లక్ష్యం->mutex);
			చురుకైన_దారాలు--;
		}
	}
	cleanup_threaded_శోధన();
	విడుదల(p);

	display_పురోగతి(పురోగతి_state, పురోగతి_nr);
	stop_పురోగతి(&పురోగతి_state);
}

స్థిర శూన్య prepare_pack(పూర్ణాంకం కిటికీ, పూర్ణాంకం లోతు)
{
	నిర్మాణం object_నమోదు **delta_list;
	uint32_t i, nr_deltas;
	గుర్తులేని n;

	ఒకవేళ (use_delta_islands)
		resolve_వృక్షం_islands(the_repository, పురోగతి, &గమ్యం_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	ఒకవేళ (!pack_గమ్యం_stdout)
		do_check_packed_object_చక్రీయ_తనిఖీ = 1;

	ఒకవేళ (!గమ్యం_pack.nr_objects || !కిటికీ || !లోతు)
		తిరిగి;

	ఒకవేళ (మార్గం_walk)
		ll_కనుగొను_deltas_by_region(గమ్యం_pack.objects, గమ్యం_pack.regions,
					 0, గమ్యం_pack.nr_regions);

	ALLOC_ARRAY(delta_list, గమ్యం_pack.nr_objects);
	nr_deltas = n = 0;

	కొరకు (i = 0; i < గమ్యం_pack.nr_objects; i++) {
		నిర్మాణం object_నమోదు *నమోదు_2 = గమ్యం_pack.objects + i;

		ఒకవేళ (!should_attempt_deltas(నమోదు_2))
			కొనసాగు;

		ఒకవేళ (!నమోదు_2->preferred_పునాది)
			nr_deltas++;

		delta_list[n++] = నమోదు_2;
	}

	ఒకవేళ (nr_deltas && n > 1) {
		గుర్తులేని nr_పూర్తి = 0;

		ఒకవేళ (పురోగతి)
			పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_కొలత_క్రమీకరించు);
		ll_కనుగొను_deltas(delta_list, n, కిటికీ+1, లోతు, &nr_పూర్తి);
		stop_పురోగతి(&పురోగతి_state);
		ఒకవేళ (nr_పూర్తి != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	విడుదల(delta_list);
}

స్థిర పూర్ణాంకం git_pack_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			   స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(k, "pack.window")) {
		కిటికీ = git_config_int(k, v, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.windowmemory")) {
		కిటికీ_జ్ఞాపకం_హద్దు = git_config_ulong(k, v, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.depth")) {
		లోతు = git_config_int(k, v, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.deltacachesize")) {
		max_delta_cache_కొలత = git_config_కొలత_t(k, v, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.deltacachelimit")) {
		cache_max_small_delta_కొలత = git_config_int(k, v, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.writebitmaphashcache")) {
		ఒకవేళ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		లేకపోతే
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	ఒకవేళ (!strcmp(k, "pack.writebitmaplookuptable")) {
		ఒకవేళ (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_పట్టిక;
		లేకపోతే
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_పట్టిక;
	}

	ఒకవేళ (!strcmp(k, "pack.usebitmaps")) {
		use_bitmap_సూచీ_default = git_config_bool(k, v);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.allowpackreuse")) {
		పూర్ణాంకం res = git_parse_maybe_bool_పాఠ్యం(v);
		ఒకవేళ (res < 0) {
			ఒకవేళ (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			లేకపోతే ఒకవేళ (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			లేకపోతే
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} లేకపోతే ఒకవేళ (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} లేకపోతే {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.threads")) {
		delta_శోధన_దారాలు = git_config_int(k, v, ctx->kvi);
		ఒకవేళ (delta_శోధన_దారాలు < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_శోధన_దారాలు);
		ఒకవేళ (!HAVE_దారాలు && delta_శోధన_దారాలు != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_శోధన_దారాలు = 0;
		}
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.indexversion")) {
		pack_idx_opts.సంస్కరణ = git_config_int(k, v, ctx->kvi);
		ఒకవేళ (pack_idx_opts.సంస్కరణ > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.సంస్కరణ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.writereverseindex")) {
		ఒకవేళ (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		లేకపోతే
			pack_idx_opts.flags &= ~WRITE_REV;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "uploadpack.blobpackfileuri")) {
		నిర్మాణం configured_exclusion *ex;
		స్థిరరాశి అక్షరం *oid_చివర, *pack_చివర;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		నిర్మాణం object_id pack_hash;

		ఒకవేళ (!v)
			తిరిగి config_error_nonbool(k);

		ex = xmalloc(పరిమాణం(*ex));
		ఒకవేళ (parse_oid_hex(v, &ex->e.oid, &oid_చివర) ||
		    *oid_చివర != ' ' ||
		    parse_oid_hex(oid_చివర + 1, &pack_hash, &pack_చివర) ||
		    *pack_చివర != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		ఒకవేళ (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_చివర - oid_చివర);
		జ్ఞాపకం_నకలు(ex->pack_hash_hex, oid_చివర + 1, pack_చివర - oid_చివర - 1);
		ex->uri = xstrdup(pack_చివర + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	తిరిగి git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
స్థిర పూర్ణాంకం stdin_packs_దొరికింది_nr;
స్థిర పూర్ణాంకం stdin_packs_hints_nr;

స్థిర పూర్ణాంకం కూడు_object_నమోదు_from_pack(స్థిరరాశి నిర్మాణం object_id *oid,
				      నిర్మాణం packed_git *p,
				      uint32_t pos,
				      శూన్య *దత్తాంశం_2)
{
	off_t ofs;
	నిర్మాణం object_info oi = OBJECT_INFO_INIT;
	గణన object_type type = OBJ_NONE;

	display_పురోగతి(పురోగతి_state, ++nr_చూసిన);

	ఒకవేళ (have_duplicate_నమోదు(oid, 0))
		తిరిగి 0;

	stdin_packs_దొరికింది_nr++;

	ofs = nth_packed_object_స్థానభ్రంశం(p, pos);

	oi.typep = &type;
	ఒకవేళ (packed_object_info(ఖాళీ, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_గమ్యం_hex(oid), p->pack_name);
	} లేకపోతే ఒకవేళ (type == OBJ_COMMIT) {
		నిర్మాణం rev_info *revs = దత్తాంశం_2;
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
		కూడు_pending_oid(revs, ఖాళీ, oid, 0);
	}

	ఒకవేళ (!want_object_లోపల_pack(oid, 0, &p, &ofs))
		తిరిగి 0;

	create_object_నమోదు(oid, type, 0, 0, 0, p, ofs);

	తిరిగి 0;
}

స్థిర శూన్య show_object_pack_hint(నిర్మాణం object *object, స్థిరరాశి అక్షరం *name,
				  శూన్య *దత్తాంశం)
{
	గణన stdin_packs_బహుళకం బహుళకం = *(గణన stdin_packs_బహుళకం *)దత్తాంశం;
	ఒకవేళ (బహుళకం == STDIN_PACKS_బహుళకం_FOLLOW) {
		ఒకవేళ (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			తిరిగి;
		కూడు_object_నమోదు(&object->oid, object->type, name, 0);
	} లేకపోతే {
		నిర్మాణం object_నమోదు *oe = packlist_కనుగొను(&గమ్యం_pack, &object->oid);
		ఒకవేళ (!oe)
			తిరిగి;

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

స్థిర శూన్య show_commit_pack_hint(నిర్మాణం commit *commit, శూన్య *దత్తాంశం)
{
	గణన stdin_packs_బహుళకం బహుళకం = *(గణన stdin_packs_బహుళకం *)దత్తాంశం;

	ఒకవేళ (బహుళకం == STDIN_PACKS_బహుళకం_FOLLOW) {
		show_object_pack_hint((నిర్మాణం object *)commit, "", దత్తాంశం);
		తిరిగి;
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
గణన stdin_pack_info_రకం {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

నిర్మాణం stdin_pack_info {
	నిర్మాణం packed_git *p;
	గణన stdin_pack_info_రకం రకం;
};

స్థిర పూర్ణాంకం pack_mtime_cmp(స్థిరరాశి శూన్య *_a, స్థిరరాశి శూన్య *_b)
{
	నిర్మాణం stdin_pack_info *a = ((స్థిరరాశి నిర్మాణం string_list_అంశం*)_a)->util;
	నిర్మాణం stdin_pack_info *b = ((స్థిరరాశి నిర్మాణం string_list_అంశం*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	ఒకవేళ (a->p->mtime < b->p->mtime)
		తిరిగి 1;
	లేకపోతే ఒకవేళ (b->p->mtime < a->p->mtime)
		తిరిగి -1;
	లేకపోతే
		తిరిగి 0;
}

స్థిర పూర్ణాంకం stdin_packs_include_check_obj(నిర్మాణం object *obj, శూన్య *దత్తాంశం వాడని)
{
	తిరిగి !has_object_kept_pack(గమ్యం_pack.repo, &obj->oid,
				     KEPT_PACK_లోపల_CORE);
}

స్థిర పూర్ణాంకం stdin_packs_include_check(నిర్మాణం commit *commit, శూన్య *దత్తాంశం)
{
	తిరిగి stdin_packs_include_check_obj((నిర్మాణం object *)commit, దత్తాంశం);
}

స్థిర శూన్య stdin_packs_కూడు_pack_entries(నిర్మాణం strmap *packs,
					 నిర్మాణం rev_info *revs)
{
	నిర్మాణం string_list కీలకాలు = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం strmap_నమోదు *నమోదు_2;

	strmap_for_each_నమోదు(packs, &iter, నమోదు_2) {
		నిర్మాణం stdin_pack_info *info = నమోదు_2->విలువ;
		ఒకవేళ (!info->p)
			die(_("could not find pack '%s'"), నమోదు_2->కీలకం);

		string_list_append(&కీలకాలు, నమోదు_2->కీలకం)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(కీలకాలు.వస్తువులు, కీలకాలు.nr, pack_mtime_cmp);

	for_each_string_list_అంశం(అంశం, &కీలకాలు) {
		నిర్మాణం stdin_pack_info *info = అంశం->util;

		ఒకవేళ (info->రకం & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		ఒకవేళ ((info->రకం & STDIN_PACK_INCLUDE) ||
		    (info->రకం & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_లోపల_pack(info->p,
						కూడు_object_నమోదు_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_క్రమం);
	}

	string_list_clear(&కీలకాలు, 0);
}

స్థిర శూన్య stdin_packs_read_ఆదానం(నిర్మాణం rev_info *revs,
				   గణన stdin_packs_బహుళకం బహుళకం)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strmap packs = STRMAP_INIT;
	నిర్మాణం packed_git *p;

	అయితే (strbuf_getline(&buf, stdin) != EOF) {
		నిర్మాణం stdin_pack_info *info;
		గణన stdin_pack_info_రకం రకం = STDIN_PACK_INCLUDE;
		స్థిరరాశి అక్షరం *కీలకం = buf.buf;

		ఒకవేళ (!*కీలకం)
			కొనసాగు;
		లేకపోతే ఒకవేళ (*కీలకం == '^')
			రకం = STDIN_PACK_EXCLUDE_CLOSED;
		లేకపోతే ఒకవేళ (*కీలకం == '!' && బహుళకం == STDIN_PACKS_బహుళకం_FOLLOW)
			రకం = STDIN_PACK_EXCLUDE_OPEN;

		ఒకవేళ (రకం != STDIN_PACK_INCLUDE)
			కీలకం++;

		info = strmap_get(&packs, కీలకం);
		ఒకవేళ (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, కీలకం, info);
		}

		info->రకం |= రకం;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		నిర్మాణం stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		ఒకవేళ (!info)
			కొనసాగు;

		ఒకవేళ (info->రకం & STDIN_PACK_INCLUDE) {
			ఒకవేళ (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			ఒకవేళ (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		ఒకవేళ (info->రకం & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_లోపల_core = 1;
		}

		ఒకవేళ (info->రకం & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_లోపల_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_కూడు_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

స్థిర శూన్య కూడు_unreachable_loose_objects(నిర్మాణం rev_info *revs);

స్థిర శూన్య read_stdin_packs(నిర్మాణం repository *repo,
			     గణన stdin_packs_బహుళకం బహుళకం, పూర్ణాంకం rev_list_unpacked)
{
	పూర్ణాంకం prev_fetch_if_missing = repo->fetch_if_missing;
	నిర్మాణం rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, ఖాళీ);
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
	revs.keep_pack_cache_flags |= KEPT_PACK_లోపల_CORE;
	revs.blob_objects = 1;
	revs.వృక్షం_objects = 1;
	revs.ట్యాగ్_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_లోపల_core = 1;
	ఒకవేళ (బహుళకం == STDIN_PACKS_బహుళకం_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_లోపల_core_open = 1;
	}
	stdin_packs_read_ఆదానం(&revs, బహుళకం);
	ఒకవేళ (rev_list_unpacked)
		కూడు_unreachable_loose_objects(&revs);

	ఒకవేళ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &బహుళకం);

	release_revisions(&revs);

	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_దొరికింది_nr);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = prev_fetch_if_missing;
}

స్థిర శూన్య కూడు_cruft_object_నమోదు(స్థిరరాశి నిర్మాణం object_id *oid, గణన object_type type,
				   నిర్మాణం packed_git *pack, off_t స్థానభ్రంశం,
				   స్థిరరాశి అక్షరం *name, uint32_t mtime)
{
	నిర్మాణం object_నమోదు *నమోదు_2;

	display_పురోగతి(పురోగతి_state, ++nr_చూసిన);

	నమోదు_2 = packlist_కనుగొను(&గమ్యం_pack, oid);
	ఒకవేళ (నమోదు_2) {
		ఒకవేళ (name) {
			నమోదు_2->hash = pack_name_hash_fn(name);
			నమోదు_2->no_try_delta = no_try_delta(name);
		}
	} లేకపోతే {
		ఒకవేళ (!want_object_లోపల_pack_mtime(oid, 0, &pack, &స్థానభ్రంశం, mtime))
			తిరిగి;
		ఒకవేళ (!pack && type == OBJ_BLOB) {
			నిర్మాణం odb_ఆధారం_files *files =
				odb_ఆధారం_files_downcast(the_repository->objects->ఆధారం);
			పూర్ణాంకం దొరికింది_2 = 0;

			కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; !దొరికింది_2 && dir; dir = dir->తదుపరి)
				ఒకవేళ (!odb_ఆధారం_read_object_info(&dir->loose->పునాది, oid, ఖాళీ, 0, ఖాళీ))
					దొరికింది_2 = 1;

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
			ఒకవేళ (!దొరికింది_2)
				తిరిగి;
		}

		నమోదు_2 = create_object_నమోదు(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, స్థానభ్రంశం);
	}

	ఒకవేళ (mtime > oe_cruft_mtime(&గమ్యం_pack, నమోదు_2))
		oe_సమితి_cruft_mtime(&గమ్యం_pack, నమోదు_2, mtime);
	తిరిగి;
}

స్థిర శూన్య show_cruft_object(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name, శూన్య *దత్తాంశం వాడని)
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
	కూడు_cruft_object_నమోదు(&obj->oid, obj->type, ఖాళీ, 0, name, cruft_expiration);
}

స్థిర శూన్య show_cruft_commit(నిర్మాణం commit *commit, శూన్య *దత్తాంశం)
{
	show_cruft_object((నిర్మాణం object*)commit, ఖాళీ, దత్తాంశం);
}

స్థిర పూర్ణాంకం cruft_include_check_obj(నిర్మాణం object *obj, శూన్య *దత్తాంశం వాడని)
{
	తిరిగి !has_object_kept_pack(గమ్యం_pack.repo, &obj->oid, KEPT_PACK_లోపల_CORE);
}

స్థిర పూర్ణాంకం cruft_include_check(నిర్మాణం commit *commit, శూన్య *దత్తాంశం)
{
	తిరిగి cruft_include_check_obj((నిర్మాణం object*)commit, దత్తాంశం);
}

స్థిర శూన్య సమితి_cruft_mtime(స్థిరరాశి నిర్మాణం object *object,
			    నిర్మాణం packed_git *pack,
			    off_t స్థానభ్రంశం, సమయం_t mtime)
{
	కూడు_cruft_object_నమోదు(&object->oid, object->type, pack, స్థానభ్రంశం, ఖాళీ,
			       mtime);
}

స్థిర శూన్య mark_pack_kept_లోపల_core(నిర్మాణం string_list *packs, గుర్తులేని keep)
{
	నిర్మాణం string_list_అంశం *అంశం = ఖాళీ;
	for_each_string_list_అంశం(అంశం, packs) {
		నిర్మాణం packed_git *p = అంశం->util;
		ఒకవేళ (!p)
			die(_("could not find pack '%s'"), అంశం->string);
		ఒకవేళ (p->is_cruft && keep)
			ignore_packed_keep_లోపల_core_has_cruft = 1;
		p->pack_keep_లోపల_core = keep;
	}
}

స్థిర శూన్య కూడు_objects_లోపల_unpacked_packs(శూన్య);

స్థిర శూన్య enumerate_cruft_objects(శూన్య)
{
	ఒకవేళ (పురోగతి)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Enumerating cruft objects"), 0);

	కూడు_objects_లోపల_unpacked_packs();
	కూడు_unreachable_loose_objects(ఖాళీ);

	stop_పురోగతి(&పురోగతి_state);
}

స్థిర శూన్య enumerate_and_traverse_cruft_objects(నిర్మాణం string_list *fresh_packs)
{
	నిర్మాణం packed_git *p;
	నిర్మాణం rev_info revs;
	పూర్ణాంకం ret;

	repo_init_revisions(the_repository, &revs, ఖాళీ);

	revs.ట్యాగ్_objects = 1;
	revs.వృక్షం_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	ఒకవేళ (పురోగతి)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = కూడు_unseen_recent_objects_గమ్యం_traversal(&revs, cruft_expiration,
						     సమితి_cruft_mtime, 1);
	stop_పురోగతి(&పురోగతి_state);

	ఒకవేళ (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_లోపల_core = 0;
	mark_pack_kept_లోపల_core(fresh_packs, 1);

	ఒకవేళ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ఒకవేళ (పురోగతి)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Traversing cruft objects"), 0);
	nr_చూసిన = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, ఖాళీ);

	stop_పురోగతి(&పురోగతి_state);
	release_revisions(&revs);
}

స్థిర శూన్య read_cruft_objects(శూన్య)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం string_list discard_packs = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list fresh_packs = STRING_LIST_INIT_DUP;
	నిర్మాణం packed_git *p;

	ignore_packed_keep_లోపల_core = 1;

	అయితే (strbuf_getline(&buf, stdin) != EOF) {
		ఒకవేళ (!buf.len)
			కొనసాగు;

		ఒకవేళ (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		లేకపోతే
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_క్రమీకరించు(&discard_packs);
	string_list_క్రమీకరించు(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		స్థిరరాశి అక్షరం *pack_name = pack_basename(p);
		నిర్మాణం string_list_అంశం *అంశం;

		అంశం = string_list_lookup(&fresh_packs, pack_name);
		ఒకవేళ (!అంశం)
			అంశం = string_list_lookup(&discard_packs, pack_name);

		ఒకవేళ (అంశం) {
			అంశం->util = p;
		} లేకపోతే {
			/*
			 * This pack wasn't mentioned in either the "fresh" or
			 * "discard" list, so the caller didn't know about it.
			 *
			 * Mark it as kept so that its objects are ignored by
			 * add_unseen_recent_objects_to_traversal(). We'll
			 * unmark it before starting the traversal so it doesn't
			 * halt the traversal early.
			 */
			p->pack_keep_లోపల_core = 1;
		}
	}

	mark_pack_kept_లోపల_core(&fresh_packs, 1);
	mark_pack_kept_లోపల_core(&discard_packs, 0);

	ఒకవేళ (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	లేకపోతే
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

స్థిర శూన్య read_object_list_from_stdin(శూన్య)
{
	అక్షరం పంక్తి[GIT_MAX_HEXSZ + 1 + మార్గం_MAX + 2];
	నిర్మాణం object_id oid;
	స్థిరరాశి అక్షరం *p;

	కొరకు (;;) {
		ఒకవేళ (!fgets(పంక్తి, పరిమాణం(పంక్తి), stdin)) {
			ఒకవేళ (feof(stdin))
				ఆపు;
			ఒకవేళ (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			ఒకవేళ (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			కొనసాగు;
		}
		ఒకవేళ (పంక్తి[0] == '-') {
			ఒకవేళ (get_oid_hex(పంక్తి+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    పంక్తి);
			కూడు_preferred_పునాది(&oid);
			కొనసాగు;
		}
		ఒకవేళ (parse_oid_hex(పంక్తి, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), పంక్తి);

		కూడు_preferred_పునాది_object(p + 1);
		కూడు_object_నమోదు(&oid, OBJ_NONE, p + 1, 0);
	}
}

స్థిర శూన్య show_commit(నిర్మాణం commit *commit, శూన్య *దత్తాంశం వాడని)
{
	కూడు_object_నమోదు(&commit->object.oid, OBJ_COMMIT, ఖాళీ, 0);

	ఒకవేళ (write_bitmap_సూచీ)
		సూచీ_commit_for_bitmap(commit);

	ఒకవేళ (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

స్థిర శూన్య show_object(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name,
			శూన్య *దత్తాంశం వాడని)
{
	కూడు_preferred_పునాది_object(name);
	కూడు_object_నమోదు(&obj->oid, obj->type, name, 0);

	ఒకవేళ (use_delta_islands)
		రికార్డు_వృక్షం_లోతు(&obj->oid, name);
}

స్థిర శూన్య show_object__ma_allow_any(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name, శూన్య *దత్తాంశం)
{
	ప్రతిజ్ఞా(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ఒకవేళ (!odb_has_object(the_repository->objects, &obj->oid, 0))
		తిరిగి;

	show_object(obj, name, దత్తాంశం);
}

స్థిర శూన్య show_object__ma_allow_promisor(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *name, శూన్య *దత్తాంశం)
{
	ప్రతిజ్ఞా(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	ఒకవేళ (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(గమ్యం_pack.repo, &obj->oid))
		తిరిగి;

	show_object(obj, name, దత్తాంశం);
}

స్థిర పూర్ణాంకం option_parse_missing_action(స్థిరరాశి నిర్మాణం option *opt,
				       స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం repository *repo = opt->విలువ;

	ప్రతిజ్ఞా(arg);
	ప్రతిజ్ఞా(!unset);

	ఒకవేళ (!strcmp(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		తిరిగి 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	తిరిగి 0;
}

స్థిర శూన్య show_అంచు(నిర్మాణం commit *commit)
{
	కూడు_preferred_పునాది(&commit->object.oid);
}

స్థిర పూర్ణాంకం కూడు_object_లోపల_unpacked_pack(స్థిరరాశి నిర్మాణం object_id *oid,
				       నిర్మాణం object_info *oi,
				       శూన్య *దత్తాంశం వాడని)
{
	ఒకవేళ (cruft) {
		కూడు_cruft_object_నమోదు(oid, OBJ_NONE, oi->ఆధారం_infop->u.packed.pack,
				       oi->ఆధారం_infop->u.packed.స్థానభ్రంశం, ఖాళీ,
				       *oi->mtimep);
	} లేకపోతే {
		కూడు_object_నమోదు(oid, OBJ_NONE, "", 0);
	}
	తిరిగి 0;
}

స్థిర శూన్య కూడు_objects_లోపల_unpacked_packs(శూన్య)
{
	నిర్మాణం odb_ఆధారం_files *files =
		odb_ఆధారం_files_downcast(గమ్యం_pack.repo->objects->ఆధారం);
	సమయం_t mtime;
	నిర్మాణం odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_క్రమం |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_లోపల_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	నిర్మాణం odb_ఆధారం_info ఆధారం_info;
	నిర్మాణం object_info oi = {
		.mtimep = &mtime,
		.ఆధారం_infop = &ఆధారం_info,
	};

	కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి) {
		ఒకవేళ (!dir->local)
			కొనసాగు;

		ఒకవేళ (odb_ఆధారం_for_each_object(&dir->packed->పునాది, &oi,
					       కూడు_object_లోపల_unpacked_pack, ఖాళీ, &opts))
			die(_("cannot open pack index"));
	}
}

స్థిర పూర్ణాంకం కూడు_loose_object(స్థిరరాశి నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *మార్గం,
			    శూన్య *దత్తాంశం)
{
	నిర్మాణం rev_info *revs = దత్తాంశం;
	గణన object_type type = odb_read_object_info(the_repository->objects, oid, ఖాళీ);

	ఒకవేళ (type < 0) {
		warning(_("loose object at %s could not be examined"), మార్గం);
		తిరిగి 0;
	}

	ఒకవేళ (cruft) {
		నిర్మాణం stat st;
		ఒకవేళ (stat(మార్గం, &st) < 0) {
			ఒకవేళ (errno == ENOENT)
				తిరిగి 0;
			తిరిగి error_errno("unable to stat %s", oid_గమ్యం_hex(oid));
		}

		కూడు_cruft_object_నమోదు(oid, type, ఖాళీ, 0, ఖాళీ,
				       st.st_mtime);
	} లేకపోతే {
		కూడు_object_నమోదు(oid, type, "", 0);
	}

	ఒకవేళ (revs && type == OBJ_COMMIT)
		కూడు_pending_oid(revs, ఖాళీ, oid, 0);

	తిరిగి 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
స్థిర శూన్య కూడు_unreachable_loose_objects(నిర్మాణం rev_info *revs)
{
	for_each_loose_file_లోపల_ఆధారం(the_repository->objects->ఆధారం,
				      కూడు_loose_object, ఖాళీ, ఖాళీ, revs);
}

స్థిర పూర్ణాంకం has_sha1_pack_kept_or_nonlocal(స్థిరరాశి నిర్మాణం object_id *oid)
{
	స్థిర నిర్మాణం packed_git *చివరి_దొరికింది = ఖాళీ;
	నిర్మాణం packed_git *p;

	ఒకవేళ (చివరి_దొరికింది && కనుగొను_pack_నమోదు_one(oid, చివరి_దొరికింది))
		తిరిగి 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		ఒకవేళ (p == చివరి_దొరికింది)
			కొనసాగు;

		ఒకవేళ ((!p->pack_local || p->pack_keep || p->pack_keep_లోపల_core) &&
		    కనుగొను_pack_నమోదు_one(oid, p)) {
			చివరి_దొరికింది = p;
			తిరిగి 1;
		}
	}

	తిరిగి 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
స్థిర నిర్మాణం oid_array recent_objects;

స్థిర పూర్ణాంకం loosened_object_can_be_discarded(స్థిరరాశి నిర్మాణం object_id *oid,
					    timestamp_t mtime)
{
	ఒకవేళ (!unpack_unreachable_expiration)
		తిరిగి 0;
	ఒకవేళ (mtime > unpack_unreachable_expiration)
		తిరిగి 0;
	ఒకవేళ (oid_array_lookup(&recent_objects, oid) >= 0)
		తిరిగి 0;
	తిరిగి 1;
}

స్థిర పూర్ణాంకం force_object_loose(నిర్మాణం odb_ఆధారం *ఆధారం,
			      స్థిరరాశి నిర్మాణం object_id *oid,
			      స్థిరరాశి సమయం_t *mtime)
{
	నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(ఆధారం);
	స్థిరరాశి నిర్మాణం git_hash_algo *compat = ఆధారం->odb->repo->compat_hash_algo;
	నిర్మాణం object_info oi = OBJECT_INFO_INIT;
	నిర్మాణం object_id compat_oid, *compat_oid_p = ఖాళీ;
	గణన object_type type;
	శూన్య *buf = ఖాళీ;
	కొలత_t len;
	పూర్ణాంకం ret;

	కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి)
		ఒకవేళ (!odb_ఆధారం_read_object_info(&dir->loose->పునాది, oid, ఖాళీ, 0, ఖాళీ))
			తిరిగి 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	ఒకవేళ (odb_read_object_info_extended(ఆధారం->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_గమ్యం_hex(oid));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (compat) {
		ఒకవేళ (repo_oid_గమ్యం_algop(ఆధారం->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_గమ్యం_hex(oid), compat->name);
			వెళ్ళు బహిర్గతం;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_ఆధారం_write_object(&files->dirs->loose->పునాది, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

బహిర్గతం:
	విడుదల(buf);
	తిరిగి ret;
}

స్థిర శూన్య loosen_వాడని_packed_objects(శూన్య)
{
	నిర్మాణం packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	నిర్మాణం object_id oid;

	repo_for_each_pack(the_repository, p) {
		ఒకవేళ (!p->pack_local || p->pack_keep || p->pack_keep_లోపల_core)
			కొనసాగు;

		ఒకవేళ (open_pack_సూచీ(p))
			die(_("cannot open pack index"));

		కొరకు (i = 0; i < p->అంకె_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			ఒకవేళ (!packlist_కనుగొను(&గమ్యం_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				ఒకవేళ (force_object_loose(the_repository->objects->ఆధారం,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_దత్తాంశం_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
స్థిర పూర్ణాంకం pack_options_allow_reuse(శూన్య)
{
	తిరిగి allow_pack_reuse != NO_PACK_REUSE &&
	       pack_గమ్యం_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_లోపల_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

స్థిర పూర్ణాంకం get_object_list_from_bitmap(నిర్మాణం rev_info *revs)
{
	ఒకవేళ (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		తిరిగి -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_సంస్కరణ = 1;

	ఒకవేళ (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	ఒకవేళ (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		ఒకవేళ (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_ఫలితం += reuse_packfile_objects;
		nr_చూసిన += reuse_packfile_objects;
		display_పురోగతి(పురోగతి_state, nr_చూసిన);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &కూడు_object_నమోదు_from_bitmap);
	తిరిగి 0;
}

స్థిర శూన్య రికార్డు_recent_object(నిర్మాణం object *obj,
				 స్థిరరాశి అక్షరం *name వాడని,
				 శూన్య *దత్తాంశం వాడని)
{
	oid_array_append(&recent_objects, &obj->oid);
}

స్థిర శూన్య రికార్డు_recent_commit(నిర్మాణం commit *commit, శూన్య *దత్తాంశం వాడని)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

స్థిర పూర్ణాంకం mark_bitmap_preferred_tip(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *దత్తాంశం వాడని)
{
	స్థిరరాశి నిర్మాణం object_id *maybe_peeled = ref->oid;
	నిర్మాణం object_id peeled;
	నిర్మాణం object *object;

	ఒకవేళ (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	ఒకవేళ (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	తిరిగి 0;
}

స్థిర అంతఃవరుస పూర్ణాంకం is_oid_uninteresting(నిర్మాణం repository *repo,
				       నిర్మాణం object_id *oid)
{
	నిర్మాణం object *o = lookup_object(repo, oid);
	తిరిగి !o || (o->flags & UNINTERESTING);
}

స్థిర పూర్ణాంకం కూడు_objects_by_మార్గం(స్థిరరాశి అక్షరం *మార్గం,
			       నిర్మాణం oid_array *oids,
			       గణన object_type type,
			       శూన్య *దత్తాంశం)
{
	కొలత_t oe_ప్రారంభం = గమ్యం_pack.nr_objects;
	కొలత_t oe_చివర;
	గుర్తులేని పూర్ణాంకం *processed = దత్తాంశం;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	కొరకు (కొలత_t i = 0; i < oids->nr; i++) {
		పూర్ణాంకం exclude;
		నిర్మాణం object_info oi = OBJECT_INFO_INIT;
		నిర్మాణం object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		ఒకవేళ ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			కొనసాగు;

		exclude = is_oid_uninteresting(the_repository, oid);

		ఒకవేళ (exclude && !thin)
			కొనసాగు;

		కూడు_object_నమోదు(oid, type, మార్గం, exclude);

		ఒకవేళ (type == OBJ_COMMIT) {
			నిర్మాణం commit *commit;

			ఒకవేళ (!write_bitmap_సూచీ && !use_delta_islands)
				కొనసాగు;

			commit = lookup_commit(the_repository, oid);
			ఒకవేళ (!commit)
				die(_("could not find commit %s"), oid_గమ్యం_hex(oid));
			ఒకవేళ (write_bitmap_సూచీ)
				సూచీ_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			ఒకవేళ (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} లేకపోతే ఒకవేళ (type == OBJ_వృక్షం && use_delta_islands) {
			రికార్డు_వృక్షం_లోతు(oid, మార్గం);
		}
	}

	oe_చివర = గమ్యం_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	ఒకవేళ (oe_చివర == oe_ప్రారంభం || !కిటికీ)
		తిరిగి 0;

	ALLOC_GROW(గమ్యం_pack.regions,
		   గమ్యం_pack.nr_regions + 1,
		   గమ్యం_pack.nr_regions_alloc);

	గమ్యం_pack.regions[గమ్యం_pack.nr_regions].ప్రారంభం = oe_ప్రారంభం;
	గమ్యం_pack.regions[గమ్యం_pack.nr_regions].nr = oe_చివర - oe_ప్రారంభం;
	గమ్యం_pack.nr_regions++;

	*processed += oids->nr;
	display_పురోగతి(పురోగతి_state, *processed);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_object_list_మార్గం_walk(నిర్మాణం rev_info *revs)
{
	నిర్మాణం మార్గం_walk_info info = మార్గం_WALK_INFO_INIT;
	గుర్తులేని పూర్ణాంకం processed = 0;
	పూర్ణాంకం ఫలితం;

	info.revs = revs;
	info.మార్గం_fn = కూడు_objects_by_మార్గం;
	info.మార్గం_fn_దత్తాంశం = &processed;

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
	info.అంచు_aggressive = shallow;

	trace2_region_enter("pack-objects", "path-walk", revs->repo);
	ఫలితం = walk_objects_by_మార్గం(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	మార్గం_walk_info_clear(&info);

	తిరిగి ఫలితం;
}

స్థిర శూన్య get_object_list(నిర్మాణం rev_info *revs, నిర్మాణం strvec *argv)
{
	నిర్మాణం అమరిక_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	అక్షరం పంక్తి[1000];
	పూర్ణాంకం flags = 0;
	పూర్ణాంకం save_warning;

	save_commit_బఫరు = 0;
	అమరిక_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	అయితే (fgets(పంక్తి, పరిమాణం(పంక్తి), stdin) != ఖాళీ) {
		పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(పంక్తి);
		ఒకవేళ (len && పంక్తి[len - 1] == '\n')
			పంక్తి[--len] = 0;
		ఒకవేళ (!len)
			ఆపు;
		ఒకవేళ (*పంక్తి == '-') {
			ఒకవేళ (!strcmp(పంక్తి, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_సూచీ = 0;
				కొనసాగు;
			}
			ఒకవేళ (starts_with(పంక్తి, "--shallow ")) {
				నిర్మాణం object_id oid;
				ఒకవేళ (get_oid_hex(పంక్తి + 10, &oid))
					die("not an object name '%s'", పంక్తి + 10);
				రిజిస్టరు_shallow(the_repository, &oid);
				use_bitmap_సూచీ = 0;
				కొనసాగు;
			}
			die(_("not a rev '%s'"), పంక్తి);
		}
		ఒకవేళ (handle_revision_arg(పంక్తి, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), పంక్తి);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	ఒకవేళ (use_bitmap_సూచీ && !get_object_list_from_bitmap(revs))
		తిరిగి;

	ఒకవేళ (use_delta_islands)
		ఎక్కించు_delta_islands(the_repository, పురోగతి);

	ఒకవేళ (write_bitmap_సూచీ)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      ఖాళీ);

	ఒకవేళ (!fn_show_object)
		fn_show_object = show_object;

	ఒకవేళ (మార్గం_walk) {
		ఒకవేళ (get_object_list_మార్గం_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			మార్గం_walk = 0;
		}
	}

	ఒకవేళ (!మార్గం_walk) {
		ఒకవేళ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_అంచులు_uninteresting(revs, show_అంచు, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				ఖాళీ);
	}

	ఒకవేళ (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		ఒకవేళ (కూడు_unseen_recent_objects_గమ్యం_traversal(revs,
				unpack_unreachable_expiration, ఖాళీ, 0))
			die(_("unable to add recent objects"));
		ఒకవేళ (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, రికార్డు_recent_commit,
				     రికార్డు_recent_object, ఖాళీ);
	}

	ఒకవేళ (keep_unreachable)
		కూడు_objects_లోపల_unpacked_packs();
	ఒకవేళ (pack_loose_unreachable)
		కూడు_unreachable_loose_objects(ఖాళీ);
	ఒకవేళ (unpack_unreachable)
		loosen_వాడని_packed_objects();

	oid_array_clear(&recent_objects);
}

స్థిర శూన్య కూడు_extra_kept_packs(స్థిరరాశి నిర్మాణం string_list *పేర్లు)
{
	నిర్మాణం packed_git *p;

	ఒకవేళ (!పేర్లు->nr)
		తిరిగి;

	repo_for_each_pack(the_repository, p) {
		స్థిరరాశి అక్షరం *name = basename(p->pack_name);
		పూర్ణాంకం i;

		ఒకవేళ (!p->pack_local)
			కొనసాగు;

		కొరకు (i = 0; i < పేర్లు->nr; i++)
			ఒకవేళ (!fspathcmp(name, పేర్లు->వస్తువులు[i].string))
				ఆపు;

		ఒకవేళ (i < పేర్లు->nr) {
			p->pack_keep_లోపల_core = 1;
			ignore_packed_keep_లోపల_core = 1;
			కొనసాగు;
		}
	}
}

స్థిర పూర్ణాంకం option_parse_quiet(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			      పూర్ణాంకం unset)
{
	పూర్ణాంకం *val = opt->విలువ;

	BUG_ON_OPT_ARG(arg);

	ఒకవేళ (!unset)
		*val = 0;
	లేకపోతే ఒకవేళ (!*val)
		*val = 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_సూచీ_సంస్కరణ(స్థిరరాశి నిర్మాణం option *opt,
				      స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం pack_idx_option *popts = opt->విలువ;
	అక్షరం *c;
	స్థిరరాశి అక్షరం *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->సంస్కరణ = strtoul(val, &c, 10);
	ఒకవేళ (popts->సంస్కరణ > 2)
		die(_("unsupported index version %s"), val);
	ఒకవేళ (*c == ',' && c[1])
		popts->off32_హద్దు = strtoul(c+1, &c, 0);
	ఒకవేళ (*c || popts->off32_హద్దు & 0x80000000)
		die(_("bad index version '%s'"), val);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_unpack_unreachable(స్థిరరాశి నిర్మాణం option *opt వాడని,
					   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	ఒకవేళ (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	లేకపోతే {
		unpack_unreachable = 1;
		ఒకవేళ (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_cruft_expiration(స్థిరరాశి నిర్మాణం option *opt వాడని,
					 స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	ఒకవేళ (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} లేకపోతే {
		cruft = 1;
		ఒకవేళ (arg)
			cruft_expiration = approxidate(arg);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం is_not_లోపల_promisor_pack_obj(నిర్మాణం object *obj, శూన్య *దత్తాంశం వాడని)
{
	నిర్మాణం odb_ఆధారం_info ఆధారం_info;
	నిర్మాణం object_info info = {
		.ఆధారం_infop = &ఆధారం_info,
	};

	ఒకవేళ (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	తిరిగి ఆధారం_info.ఆధారం->type != ODB_ఆధారం_PACKED || !ఆధారం_info.u.packed.pack->pack_promisor;
}

స్థిర పూర్ణాంకం is_not_లోపల_promisor_pack(నిర్మాణం commit *commit, శూన్య *దత్తాంశం) {
	తిరిగి is_not_లోపల_promisor_pack_obj((నిర్మాణం object *) commit, దత్తాంశం);
}

స్థిర పూర్ణాంకం parse_stdin_packs_బహుళకం(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				  పూర్ణాంకం unset)
{
	గణన stdin_packs_బహుళకం *బహుళకం = opt->విలువ;

	ఒకవేళ (unset)
		*బహుళకం = STDIN_PACKS_బహుళకం_NONE;
	లేకపోతే ఒకవేళ (!arg || !*arg)
		*బహుళకం = STDIN_PACKS_బహుళకం_STANDARD;
	లేకపోతే ఒకవేళ (!strcmp(arg, "follow"))
		*బహుళకం = STDIN_PACKS_బహుళకం_FOLLOW;
	లేకపోతే
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	తిరిగి 0;
}

పూర్ణాంకం cmd_pack_objects(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo)
{
	పూర్ణాంకం use_అంతర్గత_rev_list = 0;
	పూర్ణాంకం all_పురోగతి_implied = 0;
	నిర్మాణం strvec rp = STRVEC_INIT;
	పూర్ణాంకం rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	పూర్ణాంకం rev_list_సూచీ = 0;
	గణన stdin_packs_బహుళకం stdin_packs = STDIN_PACKS_బహుళకం_NONE;
	నిర్మాణం string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	నిర్మాణం list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	నిర్మాణం option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &పురోగతి, ఖాళీ,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_సమితి_INT(0, "progress", &పురోగతి,
			    N_("show progress meter"), 1),
		OPT_సమితి_INT(0, "all-progress", &పురోగతి,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_పురోగతి_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_సూచీ_సంస్కరణ),
		OPT_UNSIGNED(0, "max-pack-size", &pack_కొలత_హద్దు,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &కిటికీ,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &కిటికీ_జ్ఞాపకం_హద్దు,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &లోతు,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_శోధన_దారాలు,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_ఖాళీలు,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_అంతర్గత_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_సమితి_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_సమితి_INT_F(0, "indexed-objects", &rev_list_సూచీ,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_బహుళకం),
		OPT_BOOL(0, "stdout", &pack_గమ్యం_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_ట్యాగ్,
			 N_("include tag objects that refer to objects to be packed")),
		OPT_BOOL(0, "keep-unreachable", &keep_unreachable,
			 N_("keep unreachable objects")),
		OPT_BOOL(0, "pack-loose-unreachable", &pack_loose_unreachable,
			 N_("pack loose unreachable objects")),
		OPT_CALLBACK_F(0, "unpack-unreachable", ఖాళీ, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", ఖాళీ, N_("time"),
		  N_("expire cruft objects older than <time>"),
		  PARSE_OPT_OPTARG, option_parse_cruft_expiration),
		OPT_BOOL(0, "sparse", &sparse,
			 N_("use the sparse reachability algorithm")),
		OPT_BOOL(0, "thin", &thin,
			 N_("create thin packs")),
		OPT_BOOL(0, "path-walk", &మార్గం_walk,
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_సూచీ,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_సమితి_INT(0, "write-bitmap-index", &write_bitmap_సూచీ,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_సమితి_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_సూచీ,
			      N_("write a bitmap index if possible"),
			      WRITE_BITMAP_QUIET, PARSE_OPT_HIDDEN),
		OPT_PARSE_LIST_OBJECTS_FILTER(&filter_options),
		OPT_CALLBACK_F(0, "missing", repo, N_("action"),
		  N_("handling for missing objects"), PARSE_OPT_NONEG,
		  option_parse_missing_action),
		OPT_BOOL(0, "exclude-promisor-objects", &exclude_promisor_objects,
			 N_("do not pack objects in promisor packfiles")),
		OPT_BOOL(0, "exclude-promisor-objects-best-effort",
			 &exclude_promisor_objects_ఉత్తమ_effort,
			 N_("implies --missing=allow-any")),
		OPT_BOOL(0, "delta-islands", &use_delta_islands,
			 N_("respect islands during delta compression")),
		OPT_STRING_LIST(0, "uri-protocol", &uri_protocols,
				N_("protocol"),
				N_("exclude any configured uploadpack.blobpackfileuri with this protocol")),
		OPT_INTEGER(0, "name-hash-version", &name_hash_సంస్కరణ,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_చివర(),
	};

	ఒకవేళ (DFS_అంకె_స్థితులు > (1 << OE_DFS_STATE_బిట్లు))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	ఒకవేళ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		ఒకవేళ (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		ఒకవేళ (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, ఖాళీ);
	ఒకవేళ (git_env_bool(GIT_TEST_NO_WRITE_REV_సూచీ, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	పురోగతి = isatty(2);
	argc = parse_options(argc, argv, ఉపసర్గ, pack_objects_options,
			     pack_usage, 0);

	ఒకవేళ (argc) {
		పునాది_name = argv[0];
		argc--;
	}
	ఒకవేళ (pack_గమ్యం_stdout != !పునాది_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	ఒకవేళ (మార్గం_walk < 0) {
		ఒకవేళ (use_bitmap_సూచీ > 0 ||
		    !use_అంతర్గత_rev_list)
			మార్గం_walk = 0;
		లేకపోతే ఒకవేళ (the_repository->gitdir &&
			 the_repository->settings.pack_use_మార్గం_walk)
			మార్గం_walk = 1;
		లేకపోతే
			మార్గం_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	ఒకవేళ (లోతు < 0)
		లోతు = 0;
	ఒకవేళ (లోతు >= (1 << OE_లోతు_బిట్లు)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			లోతు, (1 << OE_లోతు_బిట్లు) - 1);
		లోతు = (1 << OE_లోతు_బిట్లు) - 1;
	}
	ఒకవేళ (cache_max_small_delta_కొలత >= (1U << OE_Z_DELTA_బిట్లు)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_బిట్లు) - 1);
		cache_max_small_delta_కొలత = (1U << OE_Z_DELTA_బిట్లు) - 1;
	}
	ఒకవేళ (కిటికీ < 0)
		కిటికీ = 0;

	strvec_నెట్టు(&rp, "pack-objects");

	ఒకవేళ (మార్గం_walk) {
		స్థిరరాశి అక్షరం *option = ఖాళీ;
		ఒకవేళ (!మార్గం_walk_filter_compatible(&filter_options))
			option = "--filter";

		ఒకవేళ (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			మార్గం_walk = 0;
		}
	}
	ఒకవేళ (మార్గం_walk) {
		strvec_నెట్టు(&rp, "--objects");
	} లేకపోతే ఒకవేళ (thin) {
		use_అంతర్గత_rev_list = 1;
		strvec_నెట్టు(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} లేకపోతే
		strvec_నెట్టు(&rp, "--objects");

	ఒకవేళ (rev_list_all) {
		use_అంతర్గత_rev_list = 1;
		strvec_నెట్టు(&rp, "--all");
	}
	ఒకవేళ (rev_list_reflog) {
		use_అంతర్గత_rev_list = 1;
		strvec_నెట్టు(&rp, "--reflog");
	}
	ఒకవేళ (rev_list_సూచీ) {
		use_అంతర్గత_rev_list = 1;
		strvec_నెట్టు(&rp, "--indexed-objects");
	}
	ఒకవేళ (rev_list_unpacked && !stdin_packs) {
		use_అంతర్గత_rev_list = 1;
		strvec_నెట్టు(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_ఉత్తమ_effort,
				  "--exclude-promisor-objects-best-effort");
	ఒకవేళ (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		ఒకవేళ (!stdin_packs) {
			use_అంతర్గత_rev_list = 1;
			strvec_నెట్టు(&rp, "--exclude-promisor-objects");
		}
	} లేకపోతే ఒకవేళ (exclude_promisor_objects_ఉత్తమ_effort) {
		use_అంతర్గత_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	ఒకవేళ (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_అంతర్గత_rev_list = 1;

	ఒకవేళ (!reuse_object)
		reuse_delta = 0;
	ఒకవేళ (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	లేకపోతే ఒకవేళ (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_ఉత్తమ_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	ఒకవేళ (!delta_శోధన_దారాలు)	/* --threads=0 means autodetect */
		delta_శోధన_దారాలు = online_cpus();

	ఒకవేళ (!HAVE_దారాలు && delta_శోధన_దారాలు != 1)
		warning(_("no threads support, ignoring --threads"));
	ఒకవేళ (!pack_గమ్యం_stdout && !pack_కొలత_హద్దు)
		pack_కొలత_హద్దు = pack_కొలత_హద్దు_cfg;
	ఒకవేళ (pack_గమ్యం_stdout && pack_కొలత_హద్దు)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	ఒకవేళ (pack_కొలత_హద్దు && pack_కొలత_హద్దు < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_కొలత_హద్దు = 1024*1024;
	}

	ఒకవేళ (!pack_గమ్యం_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	ఒకవేళ (!rev_list_all || !rev_list_reflog || !rev_list_సూచీ)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	ఒకవేళ (stdin_packs && use_అంతర్గత_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	ఒకవేళ (cruft) {
		ఒకవేళ (use_అంతర్గత_rev_list)
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
	ఒకవేళ (!pack_గమ్యం_stdout)
		use_bitmap_సూచీ_default = 0;

	ఒకవేళ (use_bitmap_సూచీ < 0)
		use_bitmap_సూచీ = use_bitmap_సూచీ_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	ఒకవేళ (!use_అంతర్గత_rev_list || (!pack_గమ్యం_stdout && write_bitmap_సూచీ) || is_repository_shallow(the_repository))
		use_bitmap_సూచీ = 0;

	ఒకవేళ (pack_గమ్యం_stdout || !rev_list_all)
		write_bitmap_సూచీ = 0;

	ఒకవేళ (name_hash_సంస్కరణ < 0)
		name_hash_సంస్కరణ = (పూర్ణాంకం)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_సంస్కరణ();

	ఒకవేళ (use_delta_islands)
		strvec_నెట్టు(&rp, "--topo-order");

	ఒకవేళ (పురోగతి && all_పురోగతి_implied)
		పురోగతి = 2;

	కూడు_extra_kept_packs(&keep_pack_list);
	ఒకవేళ (ignore_packed_keep_on_disk) {
		నిర్మాణం packed_git *p;

		repo_for_each_pack(the_repository, p)
			ఒకవేళ (p->pack_local && p->pack_keep)
				ఆపు;
		ఒకవేళ (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	ఒకవేళ (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		నిర్మాణం packed_git *p;

		repo_for_each_pack(the_repository, p) {
			ఒకవేళ (!p->pack_local) {
				have_non_local_packs = 1;
				ఆపు;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_దత్తాంశం(the_repository, &గమ్యం_pack);

	ఒకవేళ (పురోగతి && !cruft)
		పురోగతి_state = ప్రారంభం_పురోగతి(the_repository,
						_("Enumerating objects"), 0);
	ఒకవేళ (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} లేకపోతే ఒకవేళ (cruft) {
		read_cruft_objects();
	} లేకపోతే ఒకవేళ (!use_అంతర్గత_rev_list) {
		read_object_list_from_stdin();
	} లేకపోతే {
		నిర్మాణం rev_info revs;

		repo_init_revisions(the_repository, &revs, ఖాళీ);
		list_objects_filter_copy(&revs.filter, &filter_options);
		ఒకవేళ (exclude_promisor_objects_ఉత్తమ_effort) {
			revs.include_check = is_not_లోపల_promisor_pack;
			revs.include_check_obj = is_not_లోపల_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_పునాది();
	ఒకవేళ (include_ట్యాగ్ && nr_ఫలితం)
		refs_for_each_ట్యాగ్_ref(get_main_ref_store(the_repository),
				      కూడు_ref_ట్యాగ్, ఖాళీ);
	stop_పురోగతి(&పురోగతి_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	ఒకవేళ (non_ఖాళీలు && !nr_ఫలితం)
		వెళ్ళు cleanup;
	ఒకవేళ (nr_ఫలితం) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(కిటికీ, లోతు);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	ఒకవేళ (పురోగతి)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_వాడిన_nr);

	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "written", written);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "reused", reused);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_దత్తాంశం_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_వాడిన_nr);

cleanup:
	clear_packing_దత్తాంశం(&గమ్యం_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	తిరిగి 0;
}
