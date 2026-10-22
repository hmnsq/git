#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "attr.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "delta.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "pack-revindex.h"
#अन्तर्भाव "csum-file.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "list-objects.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "pack-objects.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "thread-utils.h"
#अन्तर्भाव "pack-bitmap.h"
#अन्तर्भाव "delta-islands.h"
#अन्तर्भाव "reachable.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "strmap.h"
#अन्तर्भाव "list.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-file-convert.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "midx.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "shallow.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "pack-mtimes.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
स्थिर संरचना packing_दत्त गन्तव्यम्_pack;

स्थिर अन्तःस्थ संरचना object_प्रविष्टिः *oe_delta(
		ध्रुव संरचना packing_दत्त *pack,
		ध्रुव संरचना object_प्रविष्टिः *e)
{
	यदि (!e->delta_idx)
		प्रत्यागम रिक्त;
	यदि (e->ext_आधारः)
		प्रत्यागम &pack->ext_bases[e->delta_idx - 1];
	अन्यथा
		प्रत्यागम &pack->objects[e->delta_idx - 1];
}

स्थिर अन्तःस्थ आकार_t oe_delta_आकार(संरचना packing_दत्त *pack,
				   ध्रुव संरचना object_प्रविष्टिः *e)
{
	यदि (e->delta_आकार_valid)
		प्रत्यागम e->delta_आकार;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	प्रत्यागम pack->delta_आकार_2[e - pack->objects];
}

आकार_t oe_get_आकार_slow(संरचना packing_दत्त *pack,
			ध्रुव संरचना object_प्रविष्टिः *e);

स्थिर अन्तःस्थ आकार_t oe_आकार(संरचना packing_दत्त *pack,
			     ध्रुव संरचना object_प्रविष्टिः *e)
{
	यदि (e->आकार_valid)
		प्रत्यागम e->आकार_2;

	प्रत्यागम oe_get_आकार_slow(pack, e);
}

स्थिर अन्तःस्थ शून्य oe_समुच्चयः_delta(संरचना packing_दत्त *pack,
				संरचना object_प्रविष्टिः *e,
				संरचना object_प्रविष्टिः *delta)
{
	यदि (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_idx = 0;
}

स्थिर अन्तःस्थ संरचना object_प्रविष्टिः *oe_delta_sibling(
		ध्रुव संरचना packing_दत्त *pack,
		ध्रुव संरचना object_प्रविष्टिः *e)
{
	यदि (e->delta_sibling_idx)
		प्रत्यागम &pack->objects[e->delta_sibling_idx - 1];
	प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ संरचना object_प्रविष्टिः *oe_delta_child(
		ध्रुव संरचना packing_दत्त *pack,
		ध्रुव संरचना object_प्रविष्टिः *e)
{
	यदि (e->delta_child_idx)
		प्रत्यागम &pack->objects[e->delta_child_idx - 1];
	प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ शून्य oe_समुच्चयः_delta_child(संरचना packing_दत्त *pack,
				      संरचना object_प्रविष्टिः *e,
				      संरचना object_प्रविष्टिः *delta)
{
	यदि (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_child_idx = 0;
}

स्थिर अन्तःस्थ शून्य oe_समुच्चयः_delta_sibling(संरचना packing_दत्त *pack,
					संरचना object_प्रविष्टिः *e,
					संरचना object_प्रविष्टिः *delta)
{
	यदि (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_sibling_idx = 0;
}

स्थिर अन्तःस्थ शून्य oe_समुच्चयः_आकार(संरचना packing_दत्त *pack,
			       संरचना object_प्रविष्टिः *e,
			       आकार_t आकार)
{
	यदि (आकार < pack->oe_आकार_सीमा) {
		e->आकार_2 = आकार;
		e->आकार_valid = 1;
	} अन्यथा {
		e->आकार_valid = 0;
		यदि (oe_get_आकार_slow(pack, e) != आकार)
			BUG("'size' is supposed to be the object size!");
	}
}

स्थिर अन्तःस्थ शून्य oe_समुच्चयः_delta_आकार(संरचना packing_दत्त *pack,
				     संरचना object_प्रविष्टिः *e,
				     आकार_t आकार)
{
	यदि (आकार < pack->oe_delta_आकार_सीमा) {
		e->delta_आकार = आकार;
		e->delta_आकार_valid = 1;
	} अन्यथा {
		packing_दत्त_तालकम्(pack);
		यदि (!pack->delta_आकार_2)
			ALLOC_ARRAY(pack->delta_आकार_2, pack->nr_alloc);
		packing_दत्त_unlock(pack);

		pack->delta_आकार_2[e - pack->objects] = आकार;
		e->delta_आकार_valid = 0;
	}
}

#परिभाषय अन्तर्_PACK(obj) oe_अन्तर्_pack(&गन्तव्यम्_pack, obj)
#परिभाषय आकार_3(obj) oe_आकार(&गन्तव्यम्_pack, obj)
#परिभाषय समुच्चयः_आकार(obj,आकार) oe_समुच्चयः_आकार(&गन्तव्यम्_pack, obj, आकार)
#परिभाषय DELTA_आकार(obj) oe_delta_आकार(&गन्तव्यम्_pack, obj)
#परिभाषय DELTA(obj) oe_delta(&गन्तव्यम्_pack, obj)
#परिभाषय DELTA_CHILD(obj) oe_delta_child(&गन्तव्यम्_pack, obj)
#परिभाषय DELTA_SIBLING(obj) oe_delta_sibling(&गन्तव्यम्_pack, obj)
#परिभाषय समुच्चयः_DELTA(obj, val) oe_समुच्चयः_delta(&गन्तव्यम्_pack, obj, val)
#परिभाषय समुच्चयः_DELTA_EXT(obj, oid) oe_समुच्चयः_delta_ext(&गन्तव्यम्_pack, obj, oid)
#परिभाषय समुच्चयः_DELTA_आकार(obj, val) oe_समुच्चयः_delta_आकार(&गन्तव्यम्_pack, obj, val)
#परिभाषय समुच्चयः_DELTA_CHILD(obj, val) oe_समुच्चयः_delta_child(&गन्तव्यम्_pack, obj, val)
#परिभाषय समुच्चयः_DELTA_SIBLING(obj, val) oe_समुच्चयः_delta_sibling(&गन्तव्यम्_pack, obj, val)

स्थिर ध्रुव वर्ण *ध्रुव pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	रिक्त
};

स्थिर संरचना pack_idx_प्रविष्टिः **written_list;
स्थिर uint32_t nr_परिणाम, nr_written, nr_आलोकितम्;
स्थिर संरचना bitmap_सूचकांक *bitmap_git;
स्थिर uint32_t write_layer;

स्थिर पूर्णाङ्क non_रिक्तानि;
स्थिर पूर्णाङ्क reuse_delta = 1, reuse_object = 1;
स्थिर पूर्णाङ्क keep_unreachable, unpack_unreachable, include_चिह्नकम्;
स्थिर timestamp_t unpack_unreachable_expiration;
स्थिर पूर्णाङ्क pack_loose_unreachable;
स्थिर पूर्णाङ्क cruft;
स्थिर पूर्णाङ्क shallow = 0;
स्थिर timestamp_t cruft_expiration;
स्थिर पूर्णाङ्क local;
स्थिर पूर्णाङ्क have_non_local_packs;
स्थिर पूर्णाङ्क incremental;
स्थिर पूर्णाङ्क ignore_packed_keep_on_disk;
स्थिर पूर्णाङ्क ignore_packed_keep_अन्तर्_core;
स्थिर पूर्णाङ्क ignore_packed_keep_अन्तर्_core_open;
स्थिर पूर्णाङ्क ignore_packed_keep_अन्तर्_core_has_cruft;
स्थिर पूर्णाङ्क allow_ofs_delta;
स्थिर संरचना pack_idx_option pack_idx_opts;
स्थिर ध्रुव वर्ण *आधारः_name;
स्थिर पूर्णाङ्क प्रगतिः = 1;
स्थिर पूर्णाङ्क गवाक्षः = 10;
स्थिर अचिह्नित दीर्घ pack_आकार_सीमा;
स्थिर पूर्णाङ्क गभीरता = 50;
स्थिर पूर्णाङ्क delta_अन्वेषणम्_तन्तवः;
स्थिर पूर्णाङ्क pack_गन्तव्यम्_stdout;
स्थिर पूर्णाङ्क sparse;
स्थिर पूर्णाङ्क thin;
स्थिर पूर्णाङ्क मार्गः_walk = -1;
स्थिर पूर्णाङ्क अङ्कः_preferred_आधारः;
स्थिर संरचना प्रगतिः *प्रगतिः_state;

स्थिर संरचना bitmapped_pack *reuse_packfiles;
स्थिर आकार_t reuse_packfiles_nr;
स्थिर आकार_t reuse_packfiles_प्रयुक्तम्_nr;
स्थिर uint32_t reuse_packfile_objects;
स्थिर संरचना bitmap *reuse_packfile_bitmap;

स्थिर पूर्णाङ्क use_bitmap_सूचकांक_default = 1;
स्थिर पूर्णाङ्क use_bitmap_सूचकांक = -1;
स्थिर गणना {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
स्थिर गणना {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_सूचकांक;
स्थिर uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

स्थिर पूर्णाङ्क exclude_promisor_objects;
स्थिर पूर्णाङ्क exclude_promisor_objects_best_effort;

स्थिर पूर्णाङ्क use_delta_islands;

स्थिर आकार_t delta_cache_आकार = 0;
स्थिर आकार_t max_delta_cache_आकार = DEFAULT_DELTA_CACHE_आकार;
स्थिर अचिह्नित दीर्घ cache_max_small_delta_आकार = 1000;

स्थिर अचिह्नित दीर्घ गवाक्षः_स्मृतिः_सीमा = 0;

स्थिर संरचना string_list uri_protocols = STRING_LIST_INIT_NODUP;

गणना missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
स्थिर गणना missing_action arg_missing_action;
स्थिर show_object_fn fn_show_object;

संरचना configured_exclusion {
	संरचना oidmap_प्रविष्टिः e;
	वर्ण *pack_hash_hex;
	वर्ण *uri;
};
स्थिर संरचना oidmap configured_exclusions;

स्थिर संरचना oidset excluded_by_config;
स्थिर पूर्णाङ्क name_hash_संस्करणम् = -1;

गणना stdin_packs_बहुलकम् {
	STDIN_PACKS_बहुलकम्_NONE,
	STDIN_PACKS_बहुलकम्_STANDARD,
	STDIN_PACKS_बहुलकम्_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
स्थिर शून्य validate_name_hash_संस्करणम्(शून्य)
{
	यदि (name_hash_संस्करणम् < 1 || name_hash_संस्करणम् > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_संस्करणम्);
	यदि (write_bitmap_सूचकांक && name_hash_संस्करणम् != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_संस्करणम् = 1;
	}
}

स्थिर अन्तःस्थ uint32_t pack_name_hash_fn(ध्रुव वर्ण *name)
{
	स्थिर पूर्णाङ्क आलोकितम्_संस्करणम् = -1;

	यदि (आलोकितम्_संस्करणम् < 0)
		आलोकितम्_संस्करणम् = name_hash_संस्करणम्;
	अन्यथा यदि (आलोकितम्_संस्करणम् != name_hash_संस्करणम्)
		BUG("name hash version changed from %d to %d mid-process",
		    आलोकितम्_संस्करणम्, name_hash_संस्करणम्);

	विकल्प (name_hash_संस्करणम्) {
	पक्ष 1:
		प्रत्यागम pack_name_hash(name);

	पक्ष 2:
		प्रत्यागम pack_name_hash_v2((ध्रुव अचिह्नित वर्ण *)name);

	स्वभाव:
		BUG("invalid name-hash version: %d", name_hash_संस्करणम्);
	}
}

/*
 * stats
 */
स्थिर uint32_t written, written_delta;
स्थिर uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
स्थिर संरचना commit **अनुक्रमितम्_commits;
स्थिर अचिह्नित पूर्णाङ्क अनुक्रमितम्_commits_nr;
स्थिर अचिह्नित पूर्णाङ्क अनुक्रमितम्_commits_alloc;

स्थिर शून्य सूचकांक_commit_for_bitmap(संरचना commit *commit)
{
	यदि (अनुक्रमितम्_commits_nr >= अनुक्रमितम्_commits_alloc) {
		अनुक्रमितम्_commits_alloc = (अनुक्रमितम्_commits_alloc + 32) * 2;
		REALLOC_ARRAY(अनुक्रमितम्_commits, अनुक्रमितम्_commits_alloc);
	}

	अनुक्रमितम्_commits[अनुक्रमितम्_commits_nr++] = commit;
}

स्थिर शून्य *get_delta(संरचना object_प्रविष्टिः *प्रविष्टिः)
{
	अचिह्नित दीर्घ आकार, आधारः_आकार;
	आकार_t delta_आकार_2;
	शून्य *buf, *आधारः_buf, *delta_buf;
	गणना object_type type;
	आकार_t आकार_st = 0, आधारः_आकार_st = 0;

	buf = odb_read_object(the_repository->objects, &प्रविष्टिः->idx.oid,
			      &type, &आकार_st);
	आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
	यदि (!buf)
		die(_("unable to read %s"), oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
	आधारः_buf = odb_read_object(the_repository->objects,
				   &DELTA(प्रविष्टिः)->idx.oid, &type,
				   &आधारः_आकार_st);
	आधारः_आकार = cast_आकार_t_गन्तव्यम्_ulong(आधारः_आकार_st);
	यदि (!आधारः_buf)
		die("unable to read %s",
		    oid_गन्तव्यम्_hex(&DELTA(प्रविष्टिः)->idx.oid));
	delta_buf = diff_delta(आधारः_buf, आधारः_आकार,
			       buf, आकार, &delta_आकार_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	यदि (!delta_buf || delta_आकार_2 != DELTA_आकार(प्रविष्टिः))
		BUG("delta size changed");
	मुक्त(buf);
	मुक्त(आधारः_buf);
	प्रत्यागम delta_buf;
}

स्थिर अचिह्नित दीर्घ do_compress(शून्य **pptr, अचिह्नित दीर्घ आकार)
{
	git_zstream प्रवाहः;
	शून्य *अन्तर्, *बहिर्गमनम्;
	अचिह्नित दीर्घ maxsize;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	git_deflate_init(&प्रवाहः, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&प्रवाहः, आकार);

	अन्तर् = *pptr;
	बहिर्गमनम् = xmalloc(maxsize);
	*pptr = बहिर्गमनम्;

	प्रवाहः.अग्र_अन्तर् = अन्तर्;
	प्रवाहः.avail_अन्तर् = आकार;
	प्रवाहः.अग्र_बहिर्गमनम् = बहिर्गमनम्;
	प्रवाहः.avail_बहिर्गमनम् = maxsize;
	यावत् (git_deflate(&प्रवाहः, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_अन्तः(&प्रवाहः);

	मुक्त(अन्तर्);
	प्रत्यागम प्रवाहः.योग_बहिर्गमनम्;
}

स्थिर अचिह्नित दीर्घ write_large_blob_दत्त(संरचना odb_प्रवाहः *st, संरचना hashfile *f,
					   ध्रुव संरचना object_id *oid)
{
	git_zstream प्रवाहः;
	अचिह्नित वर्ण ibuf[1024 * 16];
	अचिह्नित वर्ण obuf[1024 * 16];
	अचिह्नित दीर्घ olen = 0;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	git_deflate_init(&प्रवाहः, cfg->pack_compression_level);

	प्रति (;;) {
		ssize_t readlen;
		पूर्णाङ्क zret = Z_OK;
		readlen = odb_प्रवाहः_read(st, ibuf, परिमाण(ibuf));
		यदि (readlen == -1)
			die(_("unable to read %s"), oid_गन्तव्यम्_hex(oid));

		प्रवाहः.अग्र_अन्तर् = ibuf;
		प्रवाहः.avail_अन्तर् = readlen;
		यावत् ((प्रवाहः.avail_अन्तर् || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			प्रवाहः.अग्र_बहिर्गमनम् = obuf;
			प्रवाहः.avail_बहिर्गमनम् = परिमाण(obuf);
			zret = git_deflate(&प्रवाहः, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, प्रवाहः.अग्र_बहिर्गमनम् - obuf);
			olen += प्रवाहः.अग्र_बहिर्गमनम् - obuf;
		}
		यदि (प्रवाहः.avail_अन्तर्)
			die(_("deflate error (%d)"), zret);
		यदि (readlen == 0) {
			यदि (zret != Z_प्रवाहः_अन्तः)
				die(_("deflate error (%d)"), zret);
			भङ्ग;
		}
	}
	git_deflate_अन्तः(&प्रवाहः);
	प्रत्यागम olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
स्थिर पूर्णाङ्क check_pack_inflate(संरचना packed_git *p,
		संरचना pack_गवाक्षः **w_curs,
		off_t विस्थापनम्,
		off_t len,
		आकार_t expect)
{
	git_zstream प्रवाहः;
	अचिह्नित वर्ण fakebuf[4096], *अन्तर्;
	पूर्णाङ्क st;

	स्मृतिं_पूरय(&प्रवाहः, 0, परिमाण(प्रवाहः));
	git_inflate_init(&प्रवाहः);
	कुरु {
		अन्तर् = use_pack(p, w_curs, विस्थापनम्, &प्रवाहः.avail_अन्तर्);
		प्रवाहः.अग्र_अन्तर् = अन्तर्;
		प्रवाहः.अग्र_बहिर्गमनम् = fakebuf;
		प्रवाहः.avail_बहिर्गमनम् = परिमाण(fakebuf);
		st = git_inflate(&प्रवाहः, Z_FINISH);
		विस्थापनम् += प्रवाहः.अग्र_अन्तर् - अन्तर्;
	} यावत् (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_अन्तः(&प्रवाहः);
	प्रत्यागम (st == Z_प्रवाहः_अन्तः &&
		प्रवाहः.योग_बहिर्गमनम् == expect &&
		प्रवाहः.योग_अन्तर् == len) ? 0 : -1;
}

स्थिर शून्य copy_pack_दत्त(संरचना hashfile *f,
		संरचना packed_git *p,
		संरचना pack_गवाक्षः **w_curs,
		off_t विस्थापनम्,
		off_t len)
{
	अचिह्नित वर्ण *अन्तर्;
	आकार_t avail;

	यावत् (len) {
		अन्तर् = use_pack(p, w_curs, विस्थापनम्, &avail);
		यदि (avail > len)
			avail = (अचिह्नित दीर्घ)len;
		hashwrite(f, अन्तर्, avail);
		विस्थापनम् += avail;
		len -= avail;
	}
}

स्थिर अन्तःस्थ पूर्णाङ्क oe_आकार_greater_than(संरचना packing_दत्त *pack,
				       ध्रुव संरचना object_प्रविष्टिः *lhs,
				       आकार_t rhs)
{
	यदि (lhs->आकार_valid)
		प्रत्यागम lhs->आकार_2 > rhs;
	यदि (rhs < pack->oe_आकार_सीमा) /* rhs < 2^x <= lhs ? */
		प्रत्यागम 1;
	प्रत्यागम oe_get_आकार_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
स्थिर अचिह्नित दीर्घ write_no_reuse_object(संरचना hashfile *f, संरचना object_प्रविष्टिः *प्रविष्टिः,
					   अचिह्नित दीर्घ सीमा, पूर्णाङ्क usable_delta)
{
	अचिह्नित दीर्घ आकार, datalen;
	अचिह्नित वर्ण शीर्षकम्[MAX_PACK_OBJECT_शीर्षकम्],
		      dheader[MAX_PACK_OBJECT_शीर्षकम्];
	अचिह्नित hdrlen;
	गणना object_type type;
	शून्य *buf;
	संरचना odb_प्रवाहः *st = रिक्त;
	ध्रुव अचिह्नित hashsz = the_hash_algo->rawsz;

	यदि (!usable_delta) {
		यदि (oe_type(प्रविष्टिः) == OBJ_BLOB &&
		    oe_आकार_greater_than(&गन्तव्यम्_pack, प्रविष्टिः,
					 repo_settings_get_big_file_देहली(the_repository)) &&
		    (st = odb_प्रवाहः_from_object(the_repository->objects, &प्रविष्टिः->idx.oid,
						 रिक्त)) != रिक्त) {
			buf = रिक्त;
			type = st->type;
			आकार = st->आकार;
		} अन्यथा {
			आकार_t आकार_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &प्रविष्टिः->idx.oid, &type,
					      &आकार_st);
			आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
			यदि (!buf)
				die(_("unable to read %s"),
				    oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(प्रविष्टिः->delta_दत्त);
		प्रविष्टिः->z_delta_आकार = 0;
	} अन्यथा यदि (प्रविष्टिः->delta_दत्त) {
		आकार = DELTA_आकार(प्रविष्टिः);
		buf = प्रविष्टिः->delta_दत्त;
		प्रविष्टिः->delta_दत्त = रिक्त;
		type = (allow_ofs_delta && DELTA(प्रविष्टिः)->idx.विस्थापनम्) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} अन्यथा {
		buf = get_delta(प्रविष्टिः);
		आकार = DELTA_आकार(प्रविष्टिः);
		type = (allow_ofs_delta && DELTA(प्रविष्टिः)->idx.विस्थापनम्) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	यदि (st)	/* large blob case, just assume we don't compress well */
		datalen = आकार;
	अन्यथा यदि (प्रविष्टिः->z_delta_आकार)
		datalen = प्रविष्टिः->z_delta_आकार;
	अन्यथा
		datalen = do_compress(&buf, आकार);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_अन्तर्_pack_object_शीर्षकम्(शीर्षकम्, परिमाण(शीर्षकम्),
					      type, आकार);

	यदि (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = प्रविष्टिः->idx.विस्थापनम् - DELTA(प्रविष्टिः)->idx.विस्थापनम्;
		अचिह्नित pos = परिमाण(dheader) - 1;
		dheader[pos] = ofs & 127;
		यावत् (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		यदि (सीमा && hdrlen + परिमाण(dheader) - pos + datalen + hashsz >= सीमा) {
			यदि (st)
				odb_प्रवाहः_close(st);
			मुक्त(buf);
			प्रत्यागम 0;
		}
		hashwrite(f, शीर्षकम्, hdrlen);
		hashwrite(f, dheader + pos, परिमाण(dheader) - pos);
		hdrlen += परिमाण(dheader) - pos;
	} अन्यथा यदि (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		यदि (सीमा && hdrlen + hashsz + datalen + hashsz >= सीमा) {
			यदि (st)
				odb_प्रवाहः_close(st);
			मुक्त(buf);
			प्रत्यागम 0;
		}
		hashwrite(f, शीर्षकम्, hdrlen);
		hashwrite(f, DELTA(प्रविष्टिः)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} अन्यथा {
		यदि (सीमा && hdrlen + datalen + hashsz >= सीमा) {
			यदि (st)
				odb_प्रवाहः_close(st);
			मुक्त(buf);
			प्रत्यागम 0;
		}
		hashwrite(f, शीर्षकम्, hdrlen);
	}
	यदि (st) {
		datalen = write_large_blob_दत्त(st, f, &प्रविष्टिः->idx.oid);
		odb_प्रवाहः_close(st);
	} अन्यथा {
		hashwrite(f, buf, datalen);
		मुक्त(buf);
	}

	प्रत्यागम hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
स्थिर off_t write_reuse_object(संरचना hashfile *f, संरचना object_प्रविष्टिः *प्रविष्टिः,
				अचिह्नित दीर्घ सीमा, पूर्णाङ्क usable_delta)
{
	संरचना packed_git *p = अन्तर्_PACK(प्रविष्टिः);
	संरचना pack_गवाक्षः *w_curs = रिक्त;
	uint32_t pos;
	off_t विस्थापनम्, cur;
	गणना object_type type = oe_type(प्रविष्टिः);
	गणना object_type अन्तर्_pack_type;
	off_t datalen;
	अचिह्नित वर्ण शीर्षकम्[MAX_PACK_OBJECT_शीर्षकम्],
		      dheader[MAX_PACK_OBJECT_शीर्षकम्];
	अचिह्नित hdrlen;
	ध्रुव अचिह्नित hashsz = the_hash_algo->rawsz;
	आकार_t प्रविष्टिः_आकार;

	cur = प्रविष्टिः->अन्तर्_pack_विस्थापनम्;
	अन्तर्_pack_type = unpack_object_शीर्षकम्(p, &w_curs, &cur, &प्रविष्टिः_आकार);
	यदि (अन्तर्_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));

	यदि (DELTA(प्रविष्टिः))
		type = (allow_ofs_delta && DELTA(प्रविष्टिः)->idx.विस्थापनम्) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_अन्तर्_pack_object_शीर्षकम्(शीर्षकम्, परिमाण(शीर्षकम्),
					      type, प्रविष्टिः_आकार);

	विस्थापनम् = प्रविष्टिः->अन्तर्_pack_विस्थापनम्;
	यदि (विस्थापनम्_गन्तव्यम्_pack_pos(p, विस्थापनम्, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid), (uintmax_t)विस्थापनम्,
		    p->pack_name);
	datalen = pack_pos_गन्तव्यम्_विस्थापनम्(p, pos + 1) - विस्थापनम्;
	यदि (!pack_गन्तव्यम्_stdout && p->सूचकांक_संस्करणम् > 1 &&
	    check_pack_crc(p, &w_curs, विस्थापनम्, datalen,
			   pack_pos_गन्तव्यम्_सूचकांक(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
		unuse_pack(&w_curs);
		प्रत्यागम write_no_reuse_object(f, प्रविष्टिः, सीमा, usable_delta);
	}

	विस्थापनम् += प्रविष्टिः->अन्तर्_pack_शीर्षकम्_आकार;
	datalen -= प्रविष्टिः->अन्तर्_pack_शीर्षकम्_आकार;

	यदि (!pack_गन्तव्यम्_stdout && p->सूचकांक_संस्करणम् == 1 &&
	    check_pack_inflate(p, &w_curs, विस्थापनम्, datalen, प्रविष्टिः_आकार)) {
		error(_("corrupt packed object for %s"),
		      oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
		unuse_pack(&w_curs);
		प्रत्यागम write_no_reuse_object(f, प्रविष्टिः, सीमा, usable_delta);
	}

	यदि (type == OBJ_OFS_DELTA) {
		off_t ofs = प्रविष्टिः->idx.विस्थापनम् - DELTA(प्रविष्टिः)->idx.विस्थापनम्;
		अचिह्नित pos = परिमाण(dheader) - 1;
		dheader[pos] = ofs & 127;
		यावत् (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		यदि (सीमा && hdrlen + परिमाण(dheader) - pos + datalen + hashsz >= सीमा) {
			unuse_pack(&w_curs);
			प्रत्यागम 0;
		}
		hashwrite(f, शीर्षकम्, hdrlen);
		hashwrite(f, dheader + pos, परिमाण(dheader) - pos);
		hdrlen += परिमाण(dheader) - pos;
		reused_delta++;
	} अन्यथा यदि (type == OBJ_REF_DELTA) {
		यदि (सीमा && hdrlen + hashsz + datalen + hashsz >= सीमा) {
			unuse_pack(&w_curs);
			प्रत्यागम 0;
		}
		hashwrite(f, शीर्षकम्, hdrlen);
		hashwrite(f, DELTA(प्रविष्टिः)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} अन्यथा {
		यदि (सीमा && hdrlen + datalen + hashsz >= सीमा) {
			unuse_pack(&w_curs);
			प्रत्यागम 0;
		}
		hashwrite(f, शीर्षकम्, hdrlen);
	}
	copy_pack_दत्त(f, p, &w_curs, विस्थापनम्, datalen);
	unuse_pack(&w_curs);
	reused++;
	प्रत्यागम hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
स्थिर off_t write_object(संरचना hashfile *f,
			  संरचना object_प्रविष्टिः *प्रविष्टिः,
			  off_t write_विस्थापनम्)
{
	अचिह्नित दीर्घ सीमा;
	off_t len;
	पूर्णाङ्क usable_delta, गन्तव्यम्_reuse;

	यदि (!pack_गन्तव्यम्_stdout)
		चक्रीयपरीक्षा_32_begin(f);

	/* apply size limit if limited packsize and not first object */
	यदि (!pack_आकार_सीमा || !nr_written)
		सीमा = 0;
	अन्यथा यदि (pack_आकार_सीमा <= write_विस्थापनम्)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		सीमा = 1;
	अन्यथा
		सीमा = pack_आकार_सीमा - write_विस्थापनम्;

	यदि (!DELTA(प्रविष्टिः))
		usable_delta = 0;	/* no delta */
	अन्यथा यदि (!pack_आकार_सीमा)
	       usable_delta = 1;	/* unlimited packfile */
	अन्यथा यदि (DELTA(प्रविष्टिः)->idx.विस्थापनम् == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	अन्यथा यदि (DELTA(प्रविष्टिः)->idx.विस्थापनम्)
		usable_delta = 1;	/* base already exists in this pack */
	अन्यथा
		usable_delta = 0;	/* base could end up in another pack */

	यदि (!reuse_object)
		गन्तव्यम्_reuse = 0;	/* explicit */
	अन्यथा यदि (!अन्तर्_PACK(प्रविष्टिः))
		गन्तव्यम्_reuse = 0;	/* can't reuse what we don't have */
	अन्यथा यदि (oe_type(प्रविष्टिः) == OBJ_REF_DELTA ||
		 oe_type(प्रविष्टिः) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		गन्तव्यम्_reuse = usable_delta;
				/* ... but pack split may override that */
	अन्यथा यदि (oe_type(प्रविष्टिः) != प्रविष्टिः->अन्तर्_pack_type)
		गन्तव्यम्_reuse = 0;	/* pack has delta which is unusable */
	अन्यथा यदि (DELTA(प्रविष्टिः))
		गन्तव्यम्_reuse = 0;	/* we want to pack afresh */
	अन्यथा
		गन्तव्यम्_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	यदि (!गन्तव्यम्_reuse)
		len = write_no_reuse_object(f, प्रविष्टिः, सीमा, usable_delta);
	अन्यथा
		len = write_reuse_object(f, प्रविष्टिः, सीमा, usable_delta);
	यदि (!len)
		प्रत्यागम 0;

	यदि (usable_delta)
		written_delta++;
	written++;
	यदि (!pack_गन्तव्यम्_stdout)
		प्रविष्टिः->idx.चक्रीयपरीक्षा_32 = चक्रीयपरीक्षा_32_अन्तः(f);
	प्रत्यागम len;
}

गणना write_one_स्थितिः {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /*प्रसामान्यम्*/
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

स्थिर गणना write_one_स्थितिः write_one(संरचना hashfile *f,
				       संरचना object_प्रविष्टिः *e,
				       off_t *विस्थापनम्)
{
	off_t आकार;
	पूर्णाङ्क recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.विस्थापनम् == 1);
	यदि (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_गन्तव्यम्_hex(&e->idx.oid));
		प्रत्यागम WRITE_ONE_RECURSIVE;
	} अन्यथा यदि (e->idx.विस्थापनम् || e->preferred_आधारः) {
		/* offset is non zero if object is written already. */
		प्रत्यागम WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	यदि (DELTA(e)) {
		e->idx.विस्थापनम् = 1; /* now recurse */
		विकल्प (write_one(f, DELTA(e), विस्थापनम्)) {
		पक्ष WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			समुच्चयः_DELTA(e, रिक्त);
			भङ्ग;
		स्वभाव:
			भङ्ग;
		पक्ष WRITE_ONE_BREAK:
			e->idx.विस्थापनम् = recursing;
			प्रत्यागम WRITE_ONE_BREAK;
		}
	}

	e->idx.विस्थापनम् = *विस्थापनम्;
	आकार = write_object(f, e, *विस्थापनम्);
	यदि (!आकार) {
		e->idx.विस्थापनम् = recursing;
		प्रत्यागम WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	यदि (signed_सङ्कलय_overflows(*विस्थापनम्, आकार))
		die(_("pack too large for current definition of off_t"));
	*विस्थापनम् += आकार;
	प्रत्यागम WRITE_ONE_WRITTEN;
}

स्थिर पूर्णाङ्क mark_tagged(ध्रुव संरचना reference *ref, शून्य *cb_दत्त अप्रयुक्तम्)
{
	संरचना object_id peeled;
	संरचना object_प्रविष्टिः *प्रविष्टिः = packlist_find(&गन्तव्यम्_pack, ref->oid);

	यदि (प्रविष्टिः)
		प्रविष्टिः->tagged = 1;
	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		प्रविष्टिः = packlist_find(&गन्तव्यम्_pack, &peeled);
		यदि (प्रविष्टिः)
			प्रविष्टिः->tagged = 1;
	}
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ अचिह्नित वर्ण oe_layer(संरचना packing_दत्त *pack,
				     संरचना object_प्रविष्टिः *e)
{
	यदि (!pack->layer)
		प्रत्यागम 0;
	प्रत्यागम pack->layer[e - pack->objects];
}

स्थिर अन्तःस्थ शून्य सङ्कलय_गन्तव्यम्_write_क्रमः(संरचना object_प्रविष्टिः **wo,
			       अचिह्नित पूर्णाङ्क *endp,
			       संरचना object_प्रविष्टिः *e)
{
	यदि (e->filled || oe_layer(&गन्तव्यम्_pack, e) != write_layer)
		प्रत्यागम;
	wo[(*endp)++] = e;
	e->filled = 1;
}

स्थिर शून्य सङ्कलय_descendants_गन्तव्यम्_write_क्रमः(संरचना object_प्रविष्टिः **wo,
					   अचिह्नित पूर्णाङ्क *endp,
					   संरचना object_प्रविष्टिः *e)
{
	पूर्णाङ्क सङ्कलय_गन्तव्यम्_क्रमः = 1;
	यावत् (e) {
		यदि (सङ्कलय_गन्तव्यम्_क्रमः) {
			संरचना object_प्रविष्टिः *s;
			/* add this node... */
			सङ्कलय_गन्तव्यम्_write_क्रमः(wo, endp, e);
			/* all its siblings... */
			प्रति (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				सङ्कलय_गन्तव्यम्_write_क्रमः(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		यदि (DELTA_CHILD(e)) {
			सङ्कलय_गन्तव्यम्_क्रमः = 1;
			e = DELTA_CHILD(e);
		} अन्यथा {
			सङ्कलय_गन्तव्यम्_क्रमः = 0;
			/* our sibling might have some children, it is next */
			यदि (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				अनुवर्त;
			}
			/* go back to our parent node */
			e = DELTA(e);
			यावत् (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			यदि (!e) {
				/* done- we hit our original root node */
				प्रत्यागम;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

स्थिर शून्य सङ्कलय_family_गन्तव्यम्_write_क्रमः(संरचना object_प्रविष्टिः **wo,
				      अचिह्नित पूर्णाङ्क *endp,
				      संरचना object_प्रविष्टिः *e)
{
	संरचना object_प्रविष्टिः *मूलम्;

	प्रति (मूलम् = e; DELTA(मूलम्); मूलम् = DELTA(मूलम्))
		; /* nothing */
	सङ्कलय_descendants_गन्तव्यम्_write_क्रमः(wo, endp, मूलम्);
}

स्थिर शून्य compute_layer_क्रमः(संरचना object_प्रविष्टिः **wo, अचिह्नित पूर्णाङ्क *wo_अन्तः)
{
	अचिह्नित पूर्णाङ्क i, चरमः_untagged;
	संरचना object_प्रविष्टिः *objects = गन्तव्यम्_pack.objects;

	प्रति (i = 0; i < गन्तव्यम्_pack.nr_objects; i++) {
		यदि (objects[i].tagged)
			भङ्ग;
		सङ्कलय_गन्तव्यम्_write_क्रमः(wo, wo_अन्तः, &objects[i]);
	}
	चरमः_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	प्रति (; i < गन्तव्यम्_pack.nr_objects; i++) {
		यदि (objects[i].tagged)
			सङ्कलय_गन्तव्यम्_write_क्रमः(wo, wo_अन्तः, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	प्रति (i = चरमः_untagged; i < गन्तव्यम्_pack.nr_objects; i++) {
		यदि (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_चिह्नकम्)
			अनुवर्त;
		सङ्कलय_गन्तव्यम्_write_क्रमः(wo, wo_अन्तः, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	प्रति (i = चरमः_untagged; i < गन्तव्यम्_pack.nr_objects; i++) {
		यदि (oe_type(&objects[i]) != OBJ_वृक्षः)
			अनुवर्त;
		सङ्कलय_गन्तव्यम्_write_क्रमः(wo, wo_अन्तः, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	प्रति (i = चरमः_untagged; i < गन्तव्यम्_pack.nr_objects; i++) {
		यदि (!objects[i].filled && oe_layer(&गन्तव्यम्_pack, &objects[i]) == write_layer)
			सङ्कलय_family_गन्तव्यम्_write_क्रमः(wo, wo_अन्तः, &objects[i]);
	}
}

स्थिर संरचना object_प्रविष्टिः **compute_write_क्रमः(शून्य)
{
	uint32_t max_layers = 1;
	अचिह्नित पूर्णाङ्क i, wo_अन्तः;

	संरचना object_प्रविष्टिः **wo;
	संरचना object_प्रविष्टिः *objects = गन्तव्यम्_pack.objects;

	प्रति (i = 0; i < गन्तव्यम्_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		समुच्चयः_DELTA_CHILD(&objects[i], रिक्त);
		समुच्चयः_DELTA_SIBLING(&objects[i], रिक्त);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	प्रति (i = गन्तव्यम्_pack.nr_objects; i > 0;) {
		संरचना object_प्रविष्टिः *e = &objects[--i];
		यदि (!DELTA(e))
			अनुवर्त;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		समुच्चयः_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_चिह्नकम्_ref(get_main_ref_store(the_repository), mark_tagged,
			      रिक्त);

	यदि (use_delta_islands) {
		max_layers = compute_pack_layers(&गन्तव्यम्_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, गन्तव्यम्_pack.nr_objects);
	wo_अन्तः = 0;

	प्रति (; write_layer < max_layers; ++write_layer)
		compute_layer_क्रमः(wo, &wo_अन्तः);

	यदि (wo_अन्तः != गन्तव्यम्_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_अन्तः, गन्तव्यम्_pack.nr_objects);

	प्रत्यागम wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

स्थिर संरचना reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
स्थिर पूर्णाङ्क reused_chunks_nr;
स्थिर पूर्णाङ्क reused_chunks_alloc;

स्थिर शून्य लेखः_reused_object(off_t where, off_t विस्थापनम्)
{
	यदि (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == विस्थापनम्)
		प्रत्यागम;

	ALLOC_GROW(reused_chunks, reused_chunks_nr + 1,
		   reused_chunks_alloc);
	reused_chunks[reused_chunks_nr].original = where;
	reused_chunks[reused_chunks_nr].difference = विस्थापनम्;
	reused_chunks_nr++;
}

/*
 * Binary search to find the chunk that "where" is in. Note
 * that we're not looking for an exact match, just the first
 * chunk that contains it (which implicitly ends at the start
 * of the next chunk.
 */
स्थिर off_t find_reused_विस्थापनम्(off_t where)
{
	पूर्णाङ्क lo = 0, hi = reused_chunks_nr;
	यावत् (lo < hi) {
		पूर्णाङ्क mi = lo + ((hi - lo) / 2);
		यदि (where == reused_chunks[mi].original)
			प्रत्यागम reused_chunks[mi].difference;
		यदि (where < reused_chunks[mi].original)
			hi = mi;
		अन्यथा
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	प्रतिज्ञा(lo);
	प्रत्यागम reused_chunks[lo-1].difference;
}

स्थिर शून्य write_reused_pack_one(संरचना packed_git *reuse_packfile,
				  आकार_t pos, संरचना hashfile *बहिर्गमनम्,
				  off_t pack_start,
				  संरचना pack_गवाक्षः **w_curs)
{
	off_t विस्थापनम्, अग्र, cur;
	गणना object_type type;
	आकार_t आकार;

	विस्थापनम् = pack_pos_गन्तव्यम्_विस्थापनम्(reuse_packfile, pos);
	अग्र = pack_pos_गन्तव्यम्_विस्थापनम्(reuse_packfile, pos + 1);

	लेखः_reused_object(विस्थापनम्,
			     विस्थापनम् - (hashfile_योग(बहिर्गमनम्) - pack_start));

	cur = विस्थापनम्;
	type = unpack_object_शीर्षकम्(reuse_packfile, w_curs, &cur, &आकार);
	प्रतिज्ञा(type >= 0);

	यदि (type == OBJ_OFS_DELTA) {
		off_t आधारः_विस्थापनम्;
		off_t fixup;

		अचिह्नित वर्ण शीर्षकम्[MAX_PACK_OBJECT_शीर्षकम्];
		अचिह्नित len;

		आधारः_विस्थापनम् = get_delta_आधारः(reuse_packfile, w_curs, &cur, type, विस्थापनम्);
		प्रतिज्ञा(आधारः_विस्थापनम् != 0);

		/* Convert to REF_DELTA if we must... */
		यदि (!allow_ofs_delta) {
			uint32_t आधारः_pos;
			संरचना object_id आधारः_oid;

			यदि (विस्थापनम्_गन्तव्यम्_pack_pos(reuse_packfile, आधारः_विस्थापनम्, &आधारः_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)आधारः_विस्थापनम्,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&आधारः_oid, reuse_packfile,
					     pack_pos_गन्तव्यम्_सूचकांक(reuse_packfile, आधारः_pos));

			len = encode_अन्तर्_pack_object_शीर्षकम्(शीर्षकम्, परिमाण(शीर्षकम्),
							   OBJ_REF_DELTA, आकार);
			hashwrite(बहिर्गमनम्, शीर्षकम्, len);
			hashwrite(बहिर्गमनम्, आधारः_oid.hash, the_hash_algo->rawsz);
			copy_pack_दत्त(बहिर्गमनम्, reuse_packfile, w_curs, cur, अग्र - cur);
			प्रत्यागम;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_विस्थापनम्(विस्थापनम्) -
			find_reused_विस्थापनम्(आधारः_विस्थापनम्);
		यदि (fixup) {
			अचिह्नित वर्ण ofs_शीर्षकम्[MAX_PACK_OBJECT_शीर्षकम्];
			अचिह्नित i, ofs_len;
			off_t ofs = विस्थापनम् - आधारः_विस्थापनम् - fixup;

			len = encode_अन्तर्_pack_object_शीर्षकम्(शीर्षकम्, परिमाण(शीर्षकम्),
							   OBJ_OFS_DELTA, आकार);

			i = परिमाण(ofs_शीर्षकम्) - 1;
			ofs_शीर्षकम्[i] = ofs & 127;
			यावत् (ofs >>= 7)
				ofs_शीर्षकम्[--i] = 128 | (--ofs & 127);

			ofs_len = परिमाण(ofs_शीर्षकम्) - i;

			hashwrite(बहिर्गमनम्, शीर्षकम्, len);
			hashwrite(बहिर्गमनम्, ofs_शीर्षकम् + परिमाण(ofs_शीर्षकम्) - ofs_len, ofs_len);
			copy_pack_दत्त(बहिर्गमनम्, reuse_packfile, w_curs, cur, अग्र - cur);
			प्रत्यागम;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_दत्त(बहिर्गमनम्, reuse_packfile, w_curs, विस्थापनम्, अग्र - विस्थापनम्);
}

स्थिर आकार_t write_reused_pack_verbatim(संरचना bitmapped_pack *reuse_packfile,
					 संरचना hashfile *बहिर्गमनम्,
					 संरचना pack_गवाक्षः **w_curs)
{
	आकार_t pos = 0;
	आकार_t अन्तः;

	यदि (reuse_packfile->bitmap_pos) {
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
		प्रत्यागम reuse_packfile->bitmap_pos / द्वयङ्काः_अन्तर्_EWORD;
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
	अन्तः = reuse_packfile->bitmap_nr / द्वयङ्काः_अन्तर्_EWORD;
	यदि (reuse_packfile_bitmap->word_alloc < अन्तः)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	यावत् (pos < अन्तः && reuse_packfile_bitmap->शब्दाः[pos] == (eword_t)~0)
		pos++;

	यदि (pos) {
		off_t गन्तव्यम्_write;

		written = (pos * द्वयङ्काः_अन्तर्_EWORD);
		गन्तव्यम्_write = pack_pos_गन्तव्यम्_विस्थापनम्(reuse_packfile->p, written)
			- परिमाण(संरचना pack_शीर्षकम्);

		/* We're recording one chunk, not one object. */
		लेखः_reused_object(परिमाण(संरचना pack_शीर्षकम्), 0);
		hashflush(बहिर्गमनम्);
		copy_pack_दत्त(बहिर्गमनम्, reuse_packfile->p, w_curs,
			परिमाण(संरचना pack_शीर्षकम्), गन्तव्यम्_write);

		display_प्रगतिः(प्रगतिः_state, written);
	}
	प्रत्यागम pos;
}

स्थिर शून्य write_reused_pack(संरचना bitmapped_pack *reuse_packfile,
			      संरचना hashfile *f)
{
	आकार_t i = reuse_packfile->bitmap_pos / द्वयङ्काः_अन्तर्_EWORD;
	uint32_t विस्थापनम्;
	off_t pack_start = hashfile_योग(f) - परिमाण(संरचना pack_शीर्षकम्);
	संरचना pack_गवाक्षः *w_curs = रिक्त;

	यदि (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	प्रति (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->शब्दाः[i];
		आकार_t pos = (i * द्वयङ्काः_अन्तर्_EWORD);

		प्रति (विस्थापनम् = 0; विस्थापनम् < द्वयङ्काः_अन्तर्_EWORD; ++विस्थापनम्) {
			uint32_t pack_pos;
			यदि ((word >> विस्थापनम्) == 0)
				भङ्ग;

			विस्थापनम् += ewah_bit_ctz64(word >> विस्थापनम्);
			यदि (pos + विस्थापनम् < reuse_packfile->bitmap_pos)
				अनुवर्त;
			यदि (pos + विस्थापनम् >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				गच्छ समाप्तम्;

			यदि (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				संरचना multi_pack_सूचकांक *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				यदि (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_गन्तव्यम्_midx(m, pos + विस्थापनम्);
				pack_ofs = nth_midxed_विस्थापनम्(m, midx_pos);

				यदि (विस्थापनम्_गन्तव्यम्_pack_pos(reuse_packfile->p,
						       pack_ofs, &pack_pos) < 0)
					BUG("could not find expected object at offset %"PRIuMAX" in pack %s",
					    (uintmax_t)pack_ofs,
					    pack_basename(reuse_packfile->p));
			} अन्यथा {
				/*
				 * Can use bit positions directly, even for MIDX
				 * bitmaps. See comment in try_partial_reuse()
				 * for why.
				 */
				pack_pos = pos + विस्थापनम्;
			}

			write_reused_pack_one(reuse_packfile->p, pack_pos, f,
					      pack_start, &w_curs);
			display_प्रगतिः(प्रगतिः_state, ++written);
		}
	}

समाप्तम्:
	unuse_pack(&w_curs);
}

स्थिर शून्य write_excluded_by_configs(शून्य)
{
	संरचना oidset_iter iter;
	ध्रुव संरचना object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	यावत् ((oid = oidset_iter_अग्र(&iter))) {
		संरचना configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		यदि (!ex)
			BUG("configured exclusion wasn't configured");
		write_अन्तर्_पूर्णम्(1, ex->pack_hash_hex, वाक्यदैर्घ्यम्(ex->pack_hash_hex));
		write_अन्तर्_पूर्णम्(1, " ", 1);
		write_अन्तर्_पूर्णम्(1, ex->uri, वाक्यदैर्घ्यम्(ex->uri));
		write_अन्तर्_पूर्णम्(1, "\n", 1);
	}
}

स्थिर ध्रुव वर्ण no_विभज_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

स्थिर शून्य write_pack_file(शून्य)
{
	uint32_t i = 0, j;
	संरचना hashfile *f;
	off_t विस्थापनम्;
	uint32_t nr_remaining = nr_परिणाम;
	time_t चरमः_mtime = 0;
	संरचना object_प्रविष्टिः **write_क्रमः;
	off_t अष्टकानि_written = 0;

	यदि (प्रगतिः > pack_गन्तव्यम्_stdout)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Writing objects"), nr_परिणाम);
	ALLOC_ARRAY(written_list, गन्तव्यम्_pack.nr_objects);
	write_क्रमः = compute_write_क्रमः();

	कुरु {
		अचिह्नित वर्ण hash[GIT_MAX_RAWSZ];
		वर्ण *pack_tmp_name = रिक्त;

		यदि (pack_गन्तव्यम्_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			संरचना hashfd_options opts = {
				.प्रगतिः = प्रगतिः_state,
				.बफर_len = LARGE_PACKET_दत्त_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} अन्यथा {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		विस्थापनम् = write_pack_शीर्षकम्(f, nr_remaining);

		यदि (reuse_packfiles_nr) {
			प्रतिज्ञा(pack_गन्तव्यम्_stdout);
			प्रति (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				यदि (reused_chunks_nr)
					reuse_packfiles_प्रयुक्तम्_nr++;
			}
			विस्थापनम् = hashfile_योग(f);
		}

		nr_written = 0;
		प्रति (; i < गन्तव्यम्_pack.nr_objects; i++) {
			संरचना object_प्रविष्टिः *e = write_क्रमः[i];
			यदि (write_one(f, e, &विस्थापनम्) == WRITE_ONE_BREAK)
				भङ्ग;
			display_प्रगतिः(प्रगतिः_state, written);
		}

		अष्टकानि_written += hashfile_योग(f) +
			the_repository->hash_algo->rawsz;
		यदि (pack_गन्तव्यम्_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_अन्तर्_प्रवाहः | CSUM_CLOSE);
		} अन्यथा यदि (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_अन्तर्_प्रवाहः | CSUM_FSYNC | CSUM_CLOSE);
		} अन्यथा {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			पूर्णाङ्क fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_शीर्षकम्_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, विस्थापनम्);
			close(fd);
			यदि (write_bitmap_सूचकांक) {
				यदि (write_bitmap_सूचकांक != WRITE_BITMAP_QUIET)
					warning(_(no_विभज_warning));
				write_bitmap_सूचकांक = 0;
			}
		}

		यदि (!pack_गन्तव्यम्_stdout) {
			संरचना stat st;
			संरचना strbuf tmpname = STRBUF_INIT;
			संरचना bitmap_writer bitmap_writer;
			वर्ण *idx_tmp_name = रिक्त;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			यदि (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} अन्यथा यदि (!चरमः_mtime) {
				चरमः_mtime = st.st_mtime;
			} अन्यथा {
				संरचना utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --चरमः_mtime;
				यदि (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", आधारः_name,
				    hash_गन्तव्यम्_hex(hash));

			यदि (write_bitmap_सूचकांक) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &गन्तव्यम्_pack,
						   रिक्त);
				bitmap_writer_समुच्चयः_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_सूचकांक(&bitmap_writer,
							       written_list);
			}

			यदि (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &गन्तव्यम्_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			यदि (write_bitmap_सूचकांक) {
				आकार_t tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_प्रगतिः(&प्रगतिः_state);

				bitmap_writer_show_प्रगतिः(&bitmap_writer,
							    प्रगतिः);
				bitmap_writer_select_commits(&bitmap_writer,
							     अनुक्रमितम्_commits,
							     अनुक्रमितम्_commits_nr);
				यदि (bitmap_writer_build(&bitmap_writer) < 0)
					die(_("failed to write bitmap index"));
				bitmap_writer_finish(&bitmap_writer,
						     written_list,
						     tmpname.buf, write_bitmap_options);
				bitmap_writer_free(&bitmap_writer);
				write_bitmap_सूचकांक = 0;
				strbuf_setlen(&tmpname, tmpname_len);
			}

			rename_tmp_packfile_idx(the_repository, &tmpname, &idx_tmp_name);

			मुक्त(idx_tmp_name);
			strbuf_release(&tmpname);
			मुक्त(pack_tmp_name);
			पङ्क्तिं_लिख(hash_गन्तव्यम्_hex(hash));
		}

		/* mark written objects as written to previous pack */
		प्रति (j = 0; j < nr_written; j++) {
			written_list[j]->विस्थापनम् = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} यावत् (nr_remaining && i < गन्तव्यम्_pack.nr_objects);

	मुक्त(written_list);
	मुक्त(write_क्रमः);
	stop_प्रगतिः(&प्रगतिः_state);
	यदि (written != nr_परिणाम)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_परिणाम);
	trace2_दत्त_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_परिणाम);
	trace2_दत्त_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", अष्टकानि_written);
}

स्थिर पूर्णाङ्क no_try_delta(ध्रुव वर्ण *मार्गः)
{
	स्थिर संरचना attr_check *check;

	यदि (!check)
		check = attr_check_initl("delta", रिक्त);
	git_check_attr(the_repository->सूचकांक, मार्गः, check);
	यदि (ATTR_FALSE(check->वस्तूनि[0].मानम्))
		प्रत्यागम 1;
	प्रत्यागम 0;
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
स्थिर पूर्णाङ्क have_duplicate_प्रविष्टिः(ध्रुव संरचना object_id *oid,
				पूर्णाङ्क exclude)
{
	संरचना object_प्रविष्टिः *प्रविष्टिः;

	यदि (reuse_packfile_bitmap &&
	    bitmap_walk_अन्तर्भवति(bitmap_git, reuse_packfile_bitmap, oid))
		प्रत्यागम 1;

	प्रविष्टिः = packlist_find(&गन्तव्यम्_pack, oid);
	यदि (!प्रविष्टिः)
		प्रत्यागम 0;

	यदि (exclude) {
		यदि (!प्रविष्टिः->preferred_आधारः)
			nr_परिणाम--;
		प्रविष्टिः->preferred_आधारः = 1;
	}

	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क want_cruft_object_mtime(संरचना repository *r,
				   ध्रुव संरचना object_id *oid,
				   अचिह्नित flags, uint32_t mtime)
{
	संरचना odb_source_files *files = odb_source_files_downcast(r->objects->source);

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		संरचना packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		प्रति (; *cache; cache++) {
			संरचना packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_प्रविष्टिः_one(oid, p);
			यदि (!ofs)
				अनुवर्त;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			यदि (!p->is_cruft)
				प्रत्यागम 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			यदि (!आरोपय_pack_mtimes(p)) {
				uint32_t pos;
				यदि (विस्थापनम्_गन्तव्यम्_pack_pos(p, ofs, &pos) < 0)
					अनुवर्त;
				candidate_mtime = nth_packed_mtime(p, pos);
			} अन्यथा {
				candidate_mtime = p->mtime;
			}

			/*
			 * We have a surviving copy of the object in a cruft
			 * pack whose mtime is greater than or equal to the one
			 * we are considering. We can thus avoid packing an
			 * additional copy of that object.
			 */
			यदि (mtime <= candidate_mtime)
				प्रत्यागम 0;
		}
	}

	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क want_प्राप्तम्_object(ध्रुव संरचना object_id *oid, पूर्णाङ्क exclude,
			     संरचना packed_git *p, uint32_t mtime)
{
	यदि (exclude)
		प्रत्यागम 1;
	यदि (incremental)
		प्रत्यागम 0;

	यदि (!is_pack_valid(p))
		प्रत्यागम -1;

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
	यदि (local && !p->pack_local)
		प्रत्यागम 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	यदि (ignore_packed_keep_on_disk || ignore_packed_keep_अन्तर्_core ||
	    ignore_packed_keep_अन्तर्_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		अचिह्नित flags = 0;
		यदि (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		यदि (ignore_packed_keep_अन्तर्_core)
			flags |= KEPT_PACK_अन्तर्_CORE;
		यदि (ignore_packed_keep_अन्तर्_core_open)
			flags |= KEPT_PACK_अन्तर्_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		यदि (!ignore_packed_keep_अन्तर्_core_has_cruft) {
			यदि (ignore_packed_keep_on_disk && p->pack_keep)
				प्रत्यागम 0;
			यदि (ignore_packed_keep_अन्तर्_core && p->pack_keep_अन्तर्_core)
				प्रत्यागम 0;
			यदि (ignore_packed_keep_अन्तर्_core_open && p->pack_keep_अन्तर्_core_open)
				प्रत्यागम 0;
			यदि (has_object_kept_pack(p->repo, oid, flags))
				प्रत्यागम 0;
		} अन्यथा {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			यदि (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				प्रत्यागम 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	यदि (!local || !have_non_local_packs)
		प्रत्यागम 1;

	/* we don't know yet; keep looking for more packs */
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क want_object_अन्तर्_pack_one(संरचना packed_git *p,
				   ध्रुव संरचना object_id *oid,
				   पूर्णाङ्क exclude,
				   संरचना packed_git **प्राप्तम्_pack,
				   off_t *प्राप्तम्_विस्थापनम्,
				   uint32_t प्राप्तम्_mtime)
{
	off_t विस्थापनम्;

	यदि (p == *प्राप्तम्_pack)
		विस्थापनम् = *प्राप्तम्_विस्थापनम्;
	अन्यथा
		विस्थापनम् = find_pack_प्रविष्टिः_one(oid, p);

	यदि (विस्थापनम्) {
		यदि (!*प्राप्तम्_pack) {
			यदि (!is_pack_valid(p))
				प्रत्यागम -1;
			*प्राप्तम्_विस्थापनम् = विस्थापनम्;
			*प्राप्तम्_pack = p;
		}
		प्रत्यागम want_प्राप्तम्_object(oid, exclude, p, प्राप्तम्_mtime);
	}
	प्रत्यागम -1;
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
स्थिर पूर्णाङ्क want_object_अन्तर्_pack_mtime(ध्रुव संरचना object_id *oid,
				     पूर्णाङ्क exclude,
				     संरचना packed_git **प्राप्तम्_pack,
				     off_t *प्राप्तम्_विस्थापनम्,
				     uint32_t प्राप्तम्_mtime)
{
	संरचना odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	पूर्णाङ्क want;
	संरचना packfile_list_प्रविष्टिः *e;

	यदि (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		प्रति (संरचना odb_files_dir *dir = files->dirs->अग्र; dir; dir = dir->अग्र)
			यदि (!odb_source_read_object_info(&dir->loose->आधारः, oid, रिक्त, 0, रिक्त))
				प्रत्यागम 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	यदि (*प्राप्तम्_pack) {
		want = want_प्राप्तम्_object(oid, exclude, *प्राप्तम्_pack,
					 प्राप्तम्_mtime);
		यदि (want != -1)
			प्रत्यागम want;

		*प्राप्तम्_pack = रिक्त;
		*प्राप्तम्_विस्थापनम् = 0;
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		संरचना multi_pack_सूचकांक *m = get_multi_pack_सूचकांक(dir->packed);
		संरचना pack_प्रविष्टिः e;

		यदि (m && midx_fill_प्रविष्टिः(m, oid, &e, रिक्त) == MIDX_FILL_लब्धम्) {
			want = want_object_अन्तर्_pack_one(e.p, oid, exclude, प्राप्तम्_pack, प्राप्तम्_विस्थापनम्, प्राप्तम्_mtime);
			यदि (want != -1)
				प्रत्यागम want;
		}
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		प्रति (e = dir->packed->packs.शीर्ष; e; e = e->अग्र) {
			संरचना packed_git *p = e->pack;
			want = want_object_अन्तर्_pack_one(p, oid, exclude, प्राप्तम्_pack, प्राप्तम्_विस्थापनम्, प्राप्तम्_mtime);
			यदि (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			यदि (want != -1)
				प्रत्यागम want;
		}
	}

	यदि (uri_protocols.nr) {
		संरचना configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		पूर्णाङ्क i;
		ध्रुव वर्ण *p;

		यदि (ex) {
			प्रति (i = 0; i < uri_protocols.nr; i++) {
				यदि (skip_उपसर्गः(ex->uri,
						uri_protocols.वस्तूनि[i].string,
						&p) &&
				    *p == ':') {
					oidset_निवेशय(&excluded_by_config, oid);
					प्रत्यागम 0;
				}
			}
		}
	}

	प्रत्यागम 1;
}

स्थिर अन्तःस्थ पूर्णाङ्क want_object_अन्तर्_pack(ध्रुव संरचना object_id *oid,
				      पूर्णाङ्क exclude,
				      संरचना packed_git **प्राप्तम्_pack,
				      off_t *प्राप्तम्_विस्थापनम्)
{
	प्रत्यागम want_object_अन्तर्_pack_mtime(oid, exclude, प्राप्तम्_pack, प्राप्तम्_विस्थापनम्,
					 0);
}

स्थिर संरचना object_प्रविष्टिः *create_object_प्रविष्टिः(ध्रुव संरचना object_id *oid,
						गणना object_type type,
						uint32_t hash,
						पूर्णाङ्क exclude,
						पूर्णाङ्क no_try_delta,
						संरचना packed_git *प्राप्तम्_pack,
						off_t प्राप्तम्_विस्थापनम्)
{
	संरचना object_प्रविष्टिः *प्रविष्टिः;

	प्रविष्टिः = packlist_alloc(&गन्तव्यम्_pack, oid);
	प्रविष्टिः->hash = hash;
	oe_समुच्चयः_type(प्रविष्टिः, type);
	यदि (exclude)
		प्रविष्टिः->preferred_आधारः = 1;
	अन्यथा
		nr_परिणाम++;
	यदि (प्राप्तम्_pack) {
		oe_समुच्चयः_अन्तर्_pack(&गन्तव्यम्_pack, प्रविष्टिः, प्राप्तम्_pack);
		प्रविष्टिः->अन्तर्_pack_विस्थापनम् = प्राप्तम्_विस्थापनम्;
	}

	प्रविष्टिः->no_try_delta = no_try_delta;

	प्रत्यागम प्रविष्टिः;
}

स्थिर ध्रुव वर्ण no_संवृतिः_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

स्थिर शून्य सङ्कलय_object_प्रविष्टिः(ध्रुव संरचना object_id *oid, गणना object_type type,
			     ध्रुव वर्ण *name, पूर्णाङ्क exclude)
{
	संरचना packed_git *प्राप्तम्_pack = रिक्त;
	off_t प्राप्तम्_विस्थापनम् = 0;

	display_प्रगतिः(प्रगतिः_state, ++nr_आलोकितम्);

	यदि (have_duplicate_प्रविष्टिः(oid, exclude))
		प्रत्यागम;

	यदि (!want_object_अन्तर्_pack(oid, exclude, &प्राप्तम्_pack, &प्राप्तम्_विस्थापनम्)) {
		/* The pack is missing an object, so it will not have closure */
		यदि (write_bitmap_सूचकांक) {
			यदि (write_bitmap_सूचकांक != WRITE_BITMAP_QUIET)
				warning(_(no_संवृतिः_warning));
			write_bitmap_सूचकांक = 0;
		}
		प्रत्यागम;
	}

	create_object_प्रविष्टिः(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    प्राप्तम्_pack, प्राप्तम्_विस्थापनम्);
}

स्थिर पूर्णाङ्क सङ्कलय_object_प्रविष्टिः_from_bitmap(ध्रुव संरचना object_id *oid,
					गणना object_type type,
					पूर्णाङ्क flags अप्रयुक्तम्, uint32_t name_hash,
					संरचना packed_git *pack, off_t विस्थापनम्,
					शून्य *payload अप्रयुक्तम्)
{
	display_प्रगतिः(प्रगतिः_state, ++nr_आलोकितम्);

	यदि (have_duplicate_प्रविष्टिः(oid, 0))
		प्रत्यागम 0;

	यदि (!want_object_अन्तर्_pack(oid, 0, &pack, &विस्थापनम्))
		प्रत्यागम 0;

	create_object_प्रविष्टिः(oid, type, name_hash, 0, 0, pack, विस्थापनम्);
	प्रत्यागम 0;
}

संरचना pbase_वृक्षः_cache {
	संरचना object_id oid;
	पूर्णाङ्क ref;
	पूर्णाङ्क temporary;
	शून्य *वृक्षः_दत्त;
	अचिह्नित दीर्घ वृक्षः_आकार;
};

स्थिर संरचना pbase_वृक्षः_cache *(pbase_वृक्षः_cache[256]);
स्थिर पूर्णाङ्क pbase_वृक्षः_cache_ix(ध्रुव संरचना object_id *oid)
{
	प्रत्यागम oid->hash[0] % ARRAY_आकार(pbase_वृक्षः_cache);
}
स्थिर पूर्णाङ्क pbase_वृक्षः_cache_ix_incr(पूर्णाङ्क ix)
{
	प्रत्यागम (ix+1) % ARRAY_आकार(pbase_वृक्षः_cache);
}

स्थिर संरचना pbase_वृक्षः {
	संरचना pbase_वृक्षः *अग्र;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	संरचना pbase_वृक्षः_cache pcache;
} *pbase_वृक्षः;

स्थिर संरचना pbase_वृक्षः_cache *pbase_वृक्षः_get(ध्रुव संरचना object_id *oid)
{
	संरचना pbase_वृक्षः_cache *ent, *nent;
	शून्य *दत्त;
	अचिह्नित दीर्घ आकार;
	आकार_t आकार_st = 0;
	गणना object_type type;
	पूर्णाङ्क neigh;
	पूर्णाङ्क my_ix = pbase_वृक्षः_cache_ix(oid);
	पूर्णाङ्क उपलब्धम्_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	प्रति (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_वृक्षः_cache[my_ix];
		यदि (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			प्रत्यागम ent;
		}
		अन्यथा यदि (((उपलब्धम्_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= उपलब्धम्_ix) &&
			  (!ent && pbase_वृक्षः_cache[उपलब्धम्_ix])))
			उपलब्धम्_ix = my_ix;
		यदि (!ent)
			भङ्ग;
		my_ix = pbase_वृक्षः_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	दत्त = odb_read_object(the_repository->objects, oid, &type, &आकार_st);
	आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
	यदि (!दत्त)
		प्रत्यागम रिक्त;
	यदि (type != OBJ_वृक्षः) {
		मुक्त(दत्त);
		प्रत्यागम रिक्त;
	}

	/* We need to either cache or return a throwaway copy */

	यदि (उपलब्धम्_ix < 0)
		ent = रिक्त;
	अन्यथा {
		ent = pbase_वृक्षः_cache[उपलब्धम्_ix];
		my_ix = उपलब्धम्_ix;
	}

	यदि (!ent) {
		nent = xmalloc(परिमाण(*nent));
		nent->temporary = (उपलब्धम्_ix < 0);
	}
	अन्यथा {
		/* evict and reuse */
		मुक्त(ent->वृक्षः_दत्त);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->वृक्षः_दत्त = दत्त;
	nent->वृक्षः_आकार = आकार;
	nent->ref = 1;
	यदि (!nent->temporary)
		pbase_वृक्षः_cache[my_ix] = nent;
	प्रत्यागम nent;
}

स्थिर शून्य pbase_वृक्षः_put(संरचना pbase_वृक्षः_cache *cache)
{
	यदि (!cache->temporary) {
		cache->ref--;
		प्रत्यागम;
	}
	मुक्त(cache->वृक्षः_दत्त);
	मुक्त(cache);
}

स्थिर आकार_t name_cmp_len(ध्रुव वर्ण *name)
{
	प्रत्यागम strcspn(name, "\n/");
}

स्थिर शून्य सङ्कलय_pbase_object(संरचना वृक्षः_desc *वृक्षः,
			     ध्रुव वर्ण *name,
			     आकार_t cmplen,
			     ध्रुव वर्ण *fullname)
{
	संरचना name_प्रविष्टिः प्रविष्टिः;
	पूर्णाङ्क cmp;

	यावत् (वृक्षः_प्रविष्टिः(वृक्षः,&प्रविष्टिः)) {
		यदि (S_ISGITLINK(प्रविष्टिः.बहुलकम्))
			अनुवर्त;
		cmp = वृक्षः_प्रविष्टिः_len(&प्रविष्टिः) != cmplen ? 1 :
		      memcmp(name, प्रविष्टिः.मार्गः, cmplen);
		यदि (cmp > 0)
			अनुवर्त;
		यदि (cmp < 0)
			प्रत्यागम;
		यदि (name[cmplen] != '/') {
			सङ्कलय_object_प्रविष्टिः(&प्रविष्टिः.oid,
					 object_type(प्रविष्टिः.बहुलकम्),
					 fullname, 1);
			प्रत्यागम;
		}
		यदि (S_ISDIR(प्रविष्टिः.बहुलकम्)) {
			संरचना वृक्षः_desc sub;
			संरचना pbase_वृक्षः_cache *वृक्षः;
			ध्रुव वर्ण *अधस्तात् = name+cmplen+1;
			आकार_t downlen = name_cmp_len(अधस्तात्);

			वृक्षः = pbase_वृक्षः_get(&प्रविष्टिः.oid);
			यदि (!वृक्षः)
				प्रत्यागम;
			init_वृक्षः_desc(&sub, &वृक्षः->oid,
				       वृक्षः->वृक्षः_दत्त, वृक्षः->वृक्षः_आकार);

			सङ्कलय_pbase_object(&sub, अधस्तात्, downlen, fullname);
			pbase_वृक्षः_put(वृक्षः);
		}
	}
}

स्थिर अचिह्नित *समाप्तम्_pbase_paths;
स्थिर पूर्णाङ्क समाप्तम्_pbase_paths_अङ्कः;
स्थिर पूर्णाङ्क समाप्तम्_pbase_paths_alloc;
स्थिर पूर्णाङ्क समाप्तम्_pbase_मार्गः_pos(अचिह्नित hash)
{
	पूर्णाङ्क lo = 0;
	पूर्णाङ्क hi = समाप्तम्_pbase_paths_अङ्कः;
	यावत् (lo < hi) {
		पूर्णाङ्क mi = lo + (hi - lo) / 2;
		यदि (समाप्तम्_pbase_paths[mi] == hash)
			प्रत्यागम mi;
		यदि (समाप्तम्_pbase_paths[mi] < hash)
			hi = mi;
		अन्यथा
			lo = mi + 1;
	}
	प्रत्यागम -lo-1;
}

स्थिर पूर्णाङ्क check_pbase_मार्गः(अचिह्नित hash)
{
	पूर्णाङ्क pos = समाप्तम्_pbase_मार्गः_pos(hash);
	यदि (0 <= pos)
		प्रत्यागम 1;
	pos = -pos - 1;
	ALLOC_GROW(समाप्तम्_pbase_paths,
		   समाप्तम्_pbase_paths_अङ्कः + 1,
		   समाप्तम्_pbase_paths_alloc);
	समाप्तम्_pbase_paths_अङ्कः++;
	यदि (pos < समाप्तम्_pbase_paths_अङ्कः)
		MOVE_ARRAY(समाप्तम्_pbase_paths + pos + 1, समाप्तम्_pbase_paths + pos,
			   समाप्तम्_pbase_paths_अङ्कः - pos - 1);
	समाप्तम्_pbase_paths[pos] = hash;
	प्रत्यागम 0;
}

स्थिर शून्य सङ्कलय_preferred_आधारः_object(ध्रुव वर्ण *name)
{
	संरचना pbase_वृक्षः *it;
	आकार_t cmplen;
	अचिह्नित hash = pack_name_hash_fn(name);

	यदि (!अङ्कः_preferred_आधारः || check_pbase_मार्गः(hash))
		प्रत्यागम;

	cmplen = name_cmp_len(name);
	प्रति (it = pbase_वृक्षः; it; it = it->अग्र) {
		यदि (cmplen == 0) {
			सङ्कलय_object_प्रविष्टिः(&it->pcache.oid, OBJ_वृक्षः, रिक्त, 1);
		}
		अन्यथा {
			संरचना वृक्षः_desc वृक्षः;
			init_वृक्षः_desc(&वृक्षः, &it->pcache.oid,
				       it->pcache.वृक्षः_दत्त, it->pcache.वृक्षः_आकार);
			सङ्कलय_pbase_object(&वृक्षः, name, cmplen, name);
		}
	}
}

स्थिर शून्य सङ्कलय_preferred_आधारः(संरचना object_id *oid)
{
	संरचना pbase_वृक्षः *it;
	शून्य *दत्त;
	अचिह्नित दीर्घ आकार;
	आकार_t आकार_st = 0;
	संरचना object_id वृक्षः_oid;

	यदि (गवाक्षः <= अङ्कः_preferred_आधारः++)
		प्रत्यागम;

	दत्त = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_वृक्षः, &आकार_st, &वृक्षः_oid);
	आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
	यदि (!दत्त)
		प्रत्यागम;

	प्रति (it = pbase_वृक्षः; it; it = it->अग्र) {
		यदि (oideq(&it->pcache.oid, &वृक्षः_oid)) {
			मुक्त(दत्त);
			प्रत्यागम;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->अग्र = pbase_वृक्षः;
	pbase_वृक्षः = it;

	oidcpy(&it->pcache.oid, &वृक्षः_oid);
	it->pcache.वृक्षः_दत्त = दत्त;
	it->pcache.वृक्षः_आकार = आकार;
}

स्थिर शून्य cleanup_preferred_आधारः(शून्य)
{
	संरचना pbase_वृक्षः *it;
	अचिह्नित i;

	it = pbase_वृक्षः;
	pbase_वृक्षः = रिक्त;
	यावत् (it) {
		संरचना pbase_वृक्षः *tmp = it;
		it = tmp->अग्र;
		मुक्त(tmp->pcache.वृक्षः_दत्त);
		मुक्त(tmp);
	}

	प्रति (i = 0; i < ARRAY_आकार(pbase_वृक्षः_cache); i++) {
		यदि (!pbase_वृक्षः_cache[i])
			अनुवर्त;
		मुक्त(pbase_वृक्षः_cache[i]->वृक्षः_दत्त);
		FREE_AND_NULL(pbase_वृक्षः_cache[i]);
	}

	FREE_AND_NULL(समाप्तम्_pbase_paths);
	समाप्तम्_pbase_paths_अङ्कः = समाप्तम्_pbase_paths_alloc = 0;
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
स्थिर पूर्णाङ्क can_reuse_delta(ध्रुव संरचना object_id *आधारः_oid,
			   संरचना object_प्रविष्टिः *delta,
			   संरचना object_प्रविष्टिः **आधारः_बहिर्गमनम्)
{
	संरचना object_प्रविष्टिः *आधारः;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	आधारः = packlist_find(&गन्तव्यम्_pack, आधारः_oid);
	यदि (आधारः) {
		यदि (!अन्तर्_same_island(&delta->idx.oid, &आधारः->idx.oid))
			प्रत्यागम 0;
		*आधारः_बहिर्गमनम् = आधारः;
		प्रत्यागम 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	यदि (thin && bitmap_has_oid_अन्तर्_uninteresting(bitmap_git, आधारः_oid)) {
		यदि (use_delta_islands) {
			यदि (!अन्तर्_same_island(&delta->idx.oid, आधारः_oid))
				प्रत्यागम 0;
		}
		*आधारः_बहिर्गमनम् = रिक्त;
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर शून्य prefetch_गन्तव्यम्_pack(uint32_t object_सूचकांक_start) {
	संरचना oid_array गन्तव्यम्_fetch = OID_ARRAY_INIT;
	uint32_t i;

	प्रति (i = object_सूचकांक_start; i < गन्तव्यम्_pack.nr_objects; i++) {
		संरचना object_प्रविष्टिः *प्रविष्टिः = गन्तव्यम्_pack.objects + i;

		यदि (!odb_read_object_info_extended(the_repository->objects,
						   &प्रविष्टिः->idx.oid,
						   रिक्त,
						   OBJECT_INFO_FOR_PREFETCH))
			अनुवर्त;
		oid_array_append(&गन्तव्यम्_fetch, &प्रविष्टिः->idx.oid);
	}
	promisor_remote_get_प्रत्यक्षम्(the_repository,
				   गन्तव्यम्_fetch.oid, गन्तव्यम्_fetch.nr);
	oid_array_clear(&गन्तव्यम्_fetch);
}

स्थिर शून्य check_object(संरचना object_प्रविष्टिः *प्रविष्टिः, uint32_t object_सूचकांक)
{
	आकार_t canonical_आकार;
	गणना object_type type;
	संरचना object_info oi = {.typep = &type, .sizep = &canonical_आकार};

	यदि (अन्तर्_PACK(प्रविष्टिः)) {
		संरचना packed_git *p = अन्तर्_PACK(प्रविष्टिः);
		संरचना pack_गवाक्षः *w_curs = रिक्त;
		पूर्णाङ्क have_आधारः = 0;
		संरचना object_id आधारः_ref;
		संरचना object_प्रविष्टिः *आधारः_प्रविष्टिः;
		आकार_t प्रयुक्तम्_2, प्रयुक्तम्_0, avail;
		off_t ofs;
		अचिह्नित वर्ण *buf, c;
		गणना object_type type;
		आकार_t अन्तर्_pack_आकार;

		buf = use_pack(p, &w_curs, प्रविष्टिः->अन्तर्_pack_विस्थापनम्, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		प्रयुक्तम्_2 = unpack_object_शीर्षकम्_बफर(buf, avail,
						   &type,
						   &अन्तर्_pack_आकार);
		यदि (प्रयुक्तम्_2 == 0)
			गच्छ give_ऊर्ध्वम्;

		यदि (type < 0)
			BUG("invalid type %d", type);
		प्रविष्टिः->अन्तर्_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		विकल्प (प्रविष्टिः->अन्तर्_pack_type) {
		स्वभाव:
			/* Not a delta hence we've already got all we need. */
			oe_समुच्चयः_type(प्रविष्टिः, प्रविष्टिः->अन्तर्_pack_type);
			समुच्चयः_आकार(प्रविष्टिः, अन्तर्_pack_आकार);
			प्रविष्टिः->अन्तर्_pack_शीर्षकम्_आकार = प्रयुक्तम्_2;
			यदि (oe_type(प्रविष्टिः) < OBJ_COMMIT || oe_type(प्रविष्टिः) > OBJ_BLOB)
				गच्छ give_ऊर्ध्वम्;
			unuse_pack(&w_curs);
			प्रत्यागम;
		पक्ष OBJ_REF_DELTA:
			यदि (reuse_delta && !प्रविष्टिः->preferred_आधारः) {
				oidread(&आधारः_ref,
					use_pack(p, &w_curs,
						 प्रविष्टिः->अन्तर्_pack_विस्थापनम् + प्रयुक्तम्_2,
						 रिक्त),
					the_repository->hash_algo);
				have_आधारः = 1;
			}
			प्रविष्टिः->अन्तर्_pack_शीर्षकम्_आकार = प्रयुक्तम्_2 + the_hash_algo->rawsz;
			भङ्ग;
		पक्ष OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       प्रविष्टिः->अन्तर्_pack_विस्थापनम् + प्रयुक्तम्_2, रिक्त);
			प्रयुक्तम्_0 = 0;
			c = buf[प्रयुक्तम्_0++];
			ofs = c & 127;
			यावत् (c & 128) {
				ofs += 1;
				यदि (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
					गच्छ give_ऊर्ध्वम्;
				}
				c = buf[प्रयुक्तम्_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = प्रविष्टिः->अन्तर्_pack_विस्थापनम् - ofs;
			यदि (ofs <= 0 || ofs >= प्रविष्टिः->अन्तर्_pack_विस्थापनम्) {
				error(_("delta base offset out of bound for %s"),
				      oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
				गच्छ give_ऊर्ध्वम्;
			}
			यदि (reuse_delta && !प्रविष्टिः->preferred_आधारः) {
				uint32_t pos;
				यदि (विस्थापनम्_गन्तव्यम्_pack_pos(p, ofs, &pos) < 0)
					गच्छ give_ऊर्ध्वम्;
				यदि (!nth_packed_object_id(&आधारः_ref, p,
							  pack_pos_गन्तव्यम्_सूचकांक(p, pos)))
					have_आधारः = 1;
			}
			प्रविष्टिः->अन्तर्_pack_शीर्षकम्_आकार = प्रयुक्तम्_2 + प्रयुक्तम्_0;
			भङ्ग;
		}

		यदि (have_आधारः &&
		    can_reuse_delta(&आधारः_ref, प्रविष्टिः, &आधारः_प्रविष्टिः)) {
			oe_समुच्चयः_type(प्रविष्टिः, प्रविष्टिः->अन्तर्_pack_type);
			समुच्चयः_आकार(प्रविष्टिः, अन्तर्_pack_आकार); /* delta size */
			समुच्चयः_DELTA_आकार(प्रविष्टिः, अन्तर्_pack_आकार);

			यदि (आधारः_प्रविष्टिः) {
				समुच्चयः_DELTA(प्रविष्टिः, आधारः_प्रविष्टिः);
				प्रविष्टिः->delta_sibling_idx = आधारः_प्रविष्टिः->delta_child_idx;
				समुच्चयः_DELTA_CHILD(आधारः_प्रविष्टिः, प्रविष्टिः);
			} अन्यथा {
				समुच्चयः_DELTA_EXT(प्रविष्टिः, &आधारः_ref);
			}

			unuse_pack(&w_curs);
			प्रत्यागम;
		}

		यदि (oe_type(प्रविष्टिः)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = प्रविष्टिः->अन्तर्_pack_विस्थापनम् + प्रविष्टिः->अन्तर्_pack_शीर्षकम्_आकार;
			canonical_आकार = get_आकार_from_delta(p, &w_curs,
							     delta_pos);
			यदि (canonical_आकार == 0)
				गच्छ give_ऊर्ध्वम्;
			समुच्चयः_आकार(प्रविष्टिः, canonical_आकार);
			unuse_pack(&w_curs);
			प्रत्यागम;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_ऊर्ध्वम्:
		unuse_pack(&w_curs);
	}

	यदि (odb_read_object_info_extended(the_repository->objects, &प्रविष्टिः->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		यदि (repo_has_promisor_remote(the_repository)) {
			prefetch_गन्तव्यम्_pack(object_सूचकांक);
			यदि (odb_read_object_info_extended(the_repository->objects, &प्रविष्टिः->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} अन्यथा {
			type = -1;
		}
	}
	oe_समुच्चयः_type(प्रविष्टिः, type);
	यदि (प्रविष्टिः->type_valid) {
		समुच्चयः_आकार(प्रविष्टिः, canonical_आकार);
	} अन्यथा {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

स्थिर पूर्णाङ्क pack_विस्थापनम्_क्रमय(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	ध्रुव संरचना object_प्रविष्टिः *a = *(संरचना object_प्रविष्टिः **)_a;
	ध्रुव संरचना object_प्रविष्टिः *b = *(संरचना object_प्रविष्टिः **)_b;
	ध्रुव संरचना packed_git *a_अन्तर्_pack = अन्तर्_PACK(a);
	ध्रुव संरचना packed_git *b_अन्तर्_pack = अन्तर्_PACK(b);

	/* avoid filesystem trashing with loose objects */
	यदि (!a_अन्तर्_pack && !b_अन्तर्_pack)
		प्रत्यागम oidcmp(&a->idx.oid, &b->idx.oid);

	यदि (a_अन्तर्_pack < b_अन्तर्_pack)
		प्रत्यागम -1;
	यदि (a_अन्तर्_pack > b_अन्तर्_pack)
		प्रत्यागम 1;
	प्रत्यागम a->अन्तर्_pack_विस्थापनम् < b->अन्तर्_pack_विस्थापनम् ? -1 :
			(a->अन्तर्_pack_विस्थापनम् > b->अन्तर्_pack_विस्थापनम्);
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
स्थिर शून्य drop_reused_delta(संरचना object_प्रविष्टिः *प्रविष्टिः)
{
	अचिह्नित *idx = &गन्तव्यम्_pack.objects[प्रविष्टिः->delta_idx - 1].delta_child_idx;
	संरचना object_info oi = OBJECT_INFO_INIT;
	गणना object_type type;
	आकार_t आकार;

	यावत् (*idx) {
		संरचना object_प्रविष्टिः *oe = &गन्तव्यम्_pack.objects[*idx - 1];

		यदि (oe == प्रविष्टिः)
			*idx = oe->delta_sibling_idx;
		अन्यथा
			idx = &oe->delta_sibling_idx;
	}
	समुच्चयः_DELTA(प्रविष्टिः, रिक्त);
	प्रविष्टिः->गभीरता = 0;

	oi.sizep = &आकार;
	oi.typep = &type;
	यदि (packed_object_info(रिक्त, अन्तर्_PACK(प्रविष्टिः), प्रविष्टिः->अन्तर्_pack_विस्थापनम्, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_समुच्चयः_type(प्रविष्टिः,
			    odb_read_object_info(the_repository->objects,
						 &प्रविष्टिः->idx.oid, &आकार));
	} अन्यथा {
		oe_समुच्चयः_type(प्रविष्टिः, type);
	}
	समुच्चयः_आकार(प्रविष्टिः, आकार);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
स्थिर शून्य break_delta_chains(संरचना object_प्रविष्टिः *प्रविष्टिः)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t योग_गभीरता;
	संरचना object_प्रविष्टिः *cur, *अग्र;

	प्रति (cur = प्रविष्टिः, योग_गभीरता = 0;
	     cur;
	     cur = DELTA(cur), योग_गभीरता++) {
		यदि (cur->dfs_state == DFS_समाप्तम्) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			योग_गभीरता += cur->गभीरता;
			भङ्ग;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		यदि (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		यदि (!DELTA(cur)) {
			cur->dfs_state = DFS_समाप्तम्;
			भङ्ग;
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
		cur->dfs_state = DFS_सक्रिय;
		यदि (DELTA(cur)->dfs_state == DFS_सक्रिय) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_समाप्तम्;
			भङ्ग;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	प्रति (cur = प्रविष्टिः; cur; cur = अग्र) {
		अग्र = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		यदि (cur->dfs_state == DFS_समाप्तम्)
			भङ्ग;
		अन्यथा यदि (cur->dfs_state != DFS_सक्रिय)
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
		cur->गभीरता = (योग_गभीरता--) % (गभीरता + 1);
		यदि (!cur->गभीरता)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_समाप्तम्;
	}
}

स्थिर शून्य get_object_details(शून्य)
{
	uint32_t i;
	संरचना object_प्रविष्टिः **sorted_by_विस्थापनम्;

	यदि (प्रगतिः)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Counting objects"),
						गन्तव्यम्_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_विस्थापनम्, गन्तव्यम्_pack.nr_objects);
	प्रति (i = 0; i < गन्तव्यम्_pack.nr_objects; i++)
		sorted_by_विस्थापनम्[i] = गन्तव्यम्_pack.objects + i;
	QSORT(sorted_by_विस्थापनम्, गन्तव्यम्_pack.nr_objects, pack_विस्थापनम्_क्रमय);

	प्रति (i = 0; i < गन्तव्यम्_pack.nr_objects; i++) {
		संरचना object_प्रविष्टिः *प्रविष्टिः = sorted_by_विस्थापनम्[i];
		check_object(प्रविष्टिः, i);
		यदि (प्रविष्टिः->type_valid &&
		    oe_आकार_greater_than(&गन्तव्यम्_pack, प्रविष्टिः,
					 repo_settings_get_big_file_देहली(the_repository)))
			प्रविष्टिः->no_try_delta = 1;
		display_प्रगतिः(प्रगतिः_state, i + 1);
	}
	stop_प्रगतिः(&प्रगतिः_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	प्रति (i = 0; i < गन्तव्यम्_pack.nr_objects; i++)
		break_delta_chains(&गन्तव्यम्_pack.objects[i]);

	मुक्त(sorted_by_विस्थापनम्);
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
स्थिर पूर्णाङ्क type_आकार_क्रमय(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	ध्रुव संरचना object_प्रविष्टिः *a = *(संरचना object_प्रविष्टिः **)_a;
	ध्रुव संरचना object_प्रविष्टिः *b = *(संरचना object_प्रविष्टिः **)_b;
	ध्रुव गणना object_type a_type = oe_type(a);
	ध्रुव गणना object_type b_type = oe_type(b);
	ध्रुव अचिह्नित दीर्घ a_आकार = आकार_3(a);
	ध्रुव अचिह्नित दीर्घ b_आकार = आकार_3(b);

	यदि (a_type > b_type)
		प्रत्यागम -1;
	यदि (a_type < b_type)
		प्रत्यागम 1;
	यदि (a->hash > b->hash)
		प्रत्यागम -1;
	यदि (a->hash < b->hash)
		प्रत्यागम 1;
	यदि (a->preferred_आधारः > b->preferred_आधारः)
		प्रत्यागम -1;
	यदि (a->preferred_आधारः < b->preferred_आधारः)
		प्रत्यागम 1;
	यदि (use_delta_islands) {
		ध्रुव पूर्णाङ्क island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		यदि (island_cmp)
			प्रत्यागम island_cmp;
	}
	यदि (a_आकार > b_आकार)
		प्रत्यागम -1;
	यदि (a_आकार < b_आकार)
		प्रत्यागम 1;
	प्रत्यागम a < b ? -1 : (a > b);  /* newest first */
}

संरचना unpacked {
	संरचना object_प्रविष्टिः *प्रविष्टिः;
	शून्य *दत्त;
	संरचना delta_सूचकांक *सूचकांक;
	अचिह्नित गभीरता;
};

स्थिर पूर्णाङ्क delta_cacheable(आकार_t src_आकार, आकार_t trg_आकार,
			   आकार_t delta_आकार_2)
{
	यदि (max_delta_cache_आकार && delta_cache_आकार + delta_आकार_2 > max_delta_cache_आकार)
		प्रत्यागम 0;

	यदि (delta_आकार_2 < cache_max_small_delta_आकार)
		प्रत्यागम 1;

	/* cache delta, if objects are large enough compared to delta size */
	यदि ((src_आकार >> 20) + (trg_आकार >> 21) > (delta_आकार_2 >> 10))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

/* Protect delta_cache_size */
स्थिर pthread_mutex_t cache_mutex;
#परिभाषय cache_तालकम्()		pthread_mutex_तालकम्(&cache_mutex)
#परिभाषय cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
स्थिर pthread_mutex_t प्रगतिः_mutex;
#परिभाषय प्रगतिः_तालकम्()		pthread_mutex_तालकम्(&प्रगतिः_mutex)
#परिभाषय प्रगतिः_unlock()	pthread_mutex_unlock(&प्रगतिः_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

स्थिर अन्तःस्थ पूर्णाङ्क oe_आकार_less_than(संरचना packing_दत्त *pack,
				    ध्रुव संरचना object_प्रविष्टिः *lhs,
				    आकार_t rhs)
{
	यदि (lhs->आकार_valid)
		प्रत्यागम lhs->आकार_2 < rhs;
	यदि (rhs < pack->oe_आकार_सीमा) /* rhs < 2^x <= lhs ? */
		प्रत्यागम 0;
	प्रत्यागम oe_get_आकार_slow(pack, lhs) < rhs;
}

स्थिर अन्तःस्थ शून्य oe_समुच्चयः_वृक्षः_गभीरता(संरचना packing_दत्त *pack,
				     संरचना object_प्रविष्टिः *e,
				     अचिह्नित पूर्णाङ्क वृक्षः_गभीरता)
{
	यदि (!pack->वृक्षः_गभीरता)
		CALLOC_ARRAY(pack->वृक्षः_गभीरता, pack->nr_alloc);
	pack->वृक्षः_गभीरता[e - pack->objects] = वृक्षः_गभीरता;
}

स्थिर शून्य लेखः_वृक्षः_गभीरता(ध्रुव संरचना object_id *oid, ध्रुव वर्ण *name)
{
	ध्रुव वर्ण *p;
	अचिह्नित गभीरता;
	संरचना object_प्रविष्टिः *ent;

	/* the empty string is a root tree, which is depth 0 */
	गभीरता = *name ? 1 : 0;
	प्रति (p = strchr(name, '/'); p; p = strchr(p + 1, '/'))
		गभीरता++;

	ent = packlist_find(&गन्तव्यम्_pack, oid);
	यदि (ent && गभीरता > oe_वृक्षः_गभीरता(&गन्तव्यम्_pack, ent))
		oe_समुच्चयः_वृक्षः_गभीरता(&गन्तव्यम्_pack, ent, गभीरता);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
आकार_t oe_get_आकार_slow(संरचना packing_दत्त *pack,
			ध्रुव संरचना object_प्रविष्टिः *e)
{
	संरचना packed_git *p;
	संरचना pack_गवाक्षः *w_curs;
	अचिह्नित वर्ण *buf;
	गणना object_type type;
	आकार_t प्रयुक्तम्_2, avail, आकार;

	यदि (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		आकार_t sz;
		packing_दत्त_तालकम्(&गन्तव्यम्_pack);
		यदि (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_गन्तव्यम्_hex(&e->idx.oid));
		packing_दत्त_unlock(&गन्तव्यम्_pack);
		प्रत्यागम sz;
	}

	p = oe_अन्तर्_pack(pack, e);
	यदि (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_दत्त_तालकम्(&गन्तव्यम्_pack);
	w_curs = रिक्त;
	buf = use_pack(p, &w_curs, e->अन्तर्_pack_विस्थापनम्, &avail);
	प्रयुक्तम्_2 = unpack_object_शीर्षकम्_बफर(buf, avail, &type, &आकार);
	यदि (प्रयुक्तम्_2 == 0)
		die(_("unable to parse object header of %s"),
		    oid_गन्तव्यम्_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_दत्त_unlock(&गन्तव्यम्_pack);
	प्रत्यागम आकार;
}

स्थिर पूर्णाङ्क try_delta(संरचना unpacked *trg, संरचना unpacked *src,
		     अचिह्नित max_गभीरता, आकार_t *mem_usage)
{
	संरचना object_प्रविष्टिः *trg_प्रविष्टिः = trg->प्रविष्टिः;
	संरचना object_प्रविष्टिः *src_प्रविष्टिः = src->प्रविष्टिः;
	अचिह्नित दीर्घ trg_आकार, src_आकार, sizediff, max_आकार, sz;
	आकार_t delta_आकार_2;
	अचिह्नित ref_गभीरता;
	गणना object_type type;
	शून्य *delta_buf;

	/* Don't bother doing diffs between different types */
	यदि (oe_type(trg_प्रविष्टिः) != oe_type(src_प्रविष्टिः))
		प्रत्यागम -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	यदि (reuse_delta && अन्तर्_PACK(trg_प्रविष्टिः) &&
	    अन्तर्_PACK(trg_प्रविष्टिः) == अन्तर्_PACK(src_प्रविष्टिः) &&
	    !src_प्रविष्टिः->preferred_आधारः &&
	    trg_प्रविष्टिः->अन्तर्_pack_type != OBJ_REF_DELTA &&
	    trg_प्रविष्टिः->अन्तर्_pack_type != OBJ_OFS_DELTA)
		प्रत्यागम 0;

	/* Let's not bust the allowed depth. */
	यदि (src->गभीरता >= max_गभीरता)
		प्रत्यागम 0;

	/* Now some size filtering heuristics. */
	trg_आकार = आकार_3(trg_प्रविष्टिः);
	यदि (!DELTA(trg_प्रविष्टिः)) {
		max_आकार = trg_आकार/2 - the_hash_algo->rawsz;
		ref_गभीरता = 1;
	} अन्यथा {
		max_आकार = DELTA_आकार(trg_प्रविष्टिः);
		ref_गभीरता = trg->गभीरता;
	}
	max_आकार = (uint64_t)max_आकार * (max_गभीरता - src->गभीरता) /
						(max_गभीरता - ref_गभीरता + 1);
	यदि (max_आकार == 0)
		प्रत्यागम 0;
	src_आकार = आकार_3(src_प्रविष्टिः);
	sizediff = src_आकार < trg_आकार ? trg_आकार - src_आकार : 0;
	यदि (sizediff >= max_आकार)
		प्रत्यागम 0;
	यदि (trg_आकार < src_आकार / 32)
		प्रत्यागम 0;

	यदि (!अन्तर्_same_island(&trg->प्रविष्टिः->idx.oid, &src->प्रविष्टिः->idx.oid))
		प्रत्यागम 0;

	/* Load data if not already done */
	यदि (!trg->दत्त) {
		आकार_t sz_st = 0;
		packing_दत्त_तालकम्(&गन्तव्यम्_pack);
		trg->दत्त = odb_read_object(the_repository->objects,
					    &trg_प्रविष्टिः->idx.oid, &type,
					    &sz_st);
		sz = cast_आकार_t_गन्तव्यम्_ulong(sz_st);
		packing_दत्त_unlock(&गन्तव्यम्_pack);
		यदि (!trg->दत्त)
			die(_("object %s cannot be read"),
			    oid_गन्तव्यम्_hex(&trg_प्रविष्टिः->idx.oid));
		यदि (sz != trg_आकार)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_गन्तव्यम्_hex(&trg_प्रविष्टिः->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_आकार);
		*mem_usage += sz;
	}
	यदि (!src->दत्त) {
		आकार_t sz_st = 0;
		packing_दत्त_तालकम्(&गन्तव्यम्_pack);
		src->दत्त = odb_read_object(the_repository->objects,
					    &src_प्रविष्टिः->idx.oid, &type,
					    &sz_st);
		sz = cast_आकार_t_गन्तव्यम्_ulong(sz_st);
		packing_दत्त_unlock(&गन्तव्यम्_pack);
		यदि (!src->दत्त) {
			यदि (src_प्रविष्टिः->preferred_आधारः) {
				स्थिर पूर्णाङ्क warned = 0;
				यदि (!warned++)
					warning(_("object %s cannot be read"),
						oid_गन्तव्यम्_hex(&src_प्रविष्टिः->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				प्रत्यागम 0;
			}
			die(_("object %s cannot be read"),
			    oid_गन्तव्यम्_hex(&src_प्रविष्टिः->idx.oid));
		}
		यदि (sz != src_आकार)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_गन्तव्यम्_hex(&src_प्रविष्टिः->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_आकार);
		*mem_usage += sz;
	}
	यदि (!src->सूचकांक) {
		src->सूचकांक = create_delta_सूचकांक(src->दत्त, src_आकार);
		यदि (!src->सूचकांक) {
			स्थिर पूर्णाङ्क warned = 0;
			यदि (!warned++)
				warning(_("suboptimal pack - out of memory"));
			प्रत्यागम 0;
		}
		*mem_usage += sizeof_delta_सूचकांक(src->सूचकांक);
	}

	delta_buf = create_delta(src->सूचकांक, trg->दत्त, trg_आकार, &delta_आकार_2, max_आकार);
	यदि (!delta_buf)
		प्रत्यागम 0;

	यदि (DELTA(trg_प्रविष्टिः)) {
		/* Prefer only shallower same-sized deltas. */
		यदि (delta_आकार_2 == DELTA_आकार(trg_प्रविष्टिः) &&
		    src->गभीरता + 1 >= trg->गभीरता) {
			मुक्त(delta_buf);
			प्रत्यागम 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	मुक्त(trg_प्रविष्टिः->delta_दत्त);
	cache_तालकम्();
	यदि (trg_प्रविष्टिः->delta_दत्त) {
		delta_cache_आकार -= DELTA_आकार(trg_प्रविष्टिः);
		trg_प्रविष्टिः->delta_दत्त = रिक्त;
	}
	यदि (delta_cacheable(src_आकार, trg_आकार, delta_आकार_2)) {
		delta_cache_आकार += delta_आकार_2;
		cache_unlock();
		trg_प्रविष्टिः->delta_दत्त = xrealloc(delta_buf, delta_आकार_2);
	} अन्यथा {
		cache_unlock();
		मुक्त(delta_buf);
	}

	समुच्चयः_DELTA(trg_प्रविष्टिः, src_प्रविष्टिः);
	समुच्चयः_DELTA_आकार(trg_प्रविष्टिः, delta_आकार_2);
	trg->गभीरता = src->गभीरता + 1;

	प्रत्यागम 1;
}

स्थिर अचिह्नित पूर्णाङ्क check_delta_सीमा(संरचना object_प्रविष्टिः *me, अचिह्नित पूर्णाङ्क n)
{
	संरचना object_प्रविष्टिः *child = DELTA_CHILD(me);
	अचिह्नित पूर्णाङ्क m = n;
	यावत् (child) {
		ध्रुव अचिह्नित पूर्णाङ्क c = check_delta_सीमा(child, n + 1);
		यदि (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	प्रत्यागम m;
}

स्थिर आकार_t free_unpacked(संरचना unpacked *n)
{
	आकार_t freed_mem = sizeof_delta_सूचकांक(n->सूचकांक);
	free_delta_सूचकांक(n->सूचकांक);
	n->सूचकांक = रिक्त;
	यदि (n->दत्त) {
		freed_mem += आकार_3(n->प्रविष्टिः);
		FREE_AND_NULL(n->दत्त);
	}
	n->प्रविष्टिः = रिक्त;
	n->गभीरता = 0;
	प्रत्यागम freed_mem;
}

स्थिर शून्य find_deltas(संरचना object_प्रविष्टिः **list, अचिह्नित *list_आकार,
			पूर्णाङ्क गवाक्षः, पूर्णाङ्क गभीरता, अचिह्नित *processed)
{
	uint32_t i, idx = 0, गणनम् = 0;
	संरचना unpacked *array;
	आकार_t mem_usage = 0;

	CALLOC_ARRAY(array, गवाक्षः);

	प्रति (;;) {
		संरचना object_प्रविष्टिः *प्रविष्टिः;
		संरचना unpacked *n = array + idx;
		पूर्णाङ्क j, max_गभीरता, best_आधारः = -1;

		प्रगतिः_तालकम्();
		यदि (!*list_आकार) {
			प्रगतिः_unlock();
			भङ्ग;
		}
		प्रविष्टिः = *list++;
		(*list_आकार)--;
		यदि (!प्रविष्टिः->preferred_आधारः) {
			(*processed)++;
			display_प्रगतिः(प्रगतिः_state, *processed);
		}
		प्रगतिः_unlock();

		mem_usage -= free_unpacked(n);
		n->प्रविष्टिः = प्रविष्टिः;

		यावत् (गवाक्षः_स्मृतिः_सीमा &&
		       mem_usage > गवाक्षः_स्मृतिः_सीमा &&
		       गणनम् > 1) {
			ध्रुव uint32_t पुच्छ = (idx + गवाक्षः - गणनम्) % गवाक्षः;
			mem_usage -= free_unpacked(array + पुच्छ);
			गणनम्--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		यदि (प्रविष्टिः->preferred_आधारः)
			गच्छ अग्र;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_गभीरता = गभीरता;
		यदि (DELTA_CHILD(प्रविष्टिः)) {
			max_गभीरता -= check_delta_सीमा(प्रविष्टिः, 0);
			यदि (max_गभीरता <= 0)
				गच्छ अग्र;
		}

		j = गवाक्षः;
		यावत् (--j > 0) {
			पूर्णाङ्क ret;
			uint32_t other_idx = idx + j;
			संरचना unpacked *m;
			यदि (other_idx >= गवाक्षः)
				other_idx -= गवाक्षः;
			m = array + other_idx;
			यदि (!m->प्रविष्टिः)
				भङ्ग;
			ret = try_delta(n, m, max_गभीरता, &mem_usage);
			यदि (ret < 0)
				भङ्ग;
			अन्यथा यदि (ret > 0)
				best_आधारः = other_idx;
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
		यदि (प्रविष्टिः->delta_दत्त && !pack_गन्तव्यम्_stdout) {
			अचिह्नित दीर्घ आकार;

			आकार = do_compress(&प्रविष्टिः->delta_दत्त, DELTA_आकार(प्रविष्टिः));
			यदि (आकार < (1U << OE_Z_DELTA_द्वयङ्काः)) {
				प्रविष्टिः->z_delta_आकार = आकार;
				cache_तालकम्();
				delta_cache_आकार -= DELTA_आकार(प्रविष्टिः);
				delta_cache_आकार += प्रविष्टिः->z_delta_आकार;
				cache_unlock();
			} अन्यथा {
				FREE_AND_NULL(प्रविष्टिः->delta_दत्त);
				प्रविष्टिः->z_delta_आकार = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		यदि (DELTA(प्रविष्टिः) && max_गभीरता <= n->गभीरता)
			अनुवर्त;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		यदि (DELTA(प्रविष्टिः)) {
			संरचना unpacked swap = array[best_आधारः];
			पूर्णाङ्क dist = (गवाक्षः + idx - best_आधारः) % गवाक्षः;
			पूर्णाङ्क dst = best_आधारः;
			यावत् (dist--) {
				पूर्णाङ्क src = (dst + 1) % गवाक्षः;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		अग्र:
		idx++;
		यदि (गणनम् + 1 < गवाक्षः)
			गणनम्++;
		यदि (idx >= गवाक्षः)
			idx = 0;
	}

	प्रति (i = 0; i < गवाक्षः; ++i) {
		free_delta_सूचकांक(array[i].सूचकांक);
		मुक्त(array[i].दत्त);
	}
	मुक्त(array);
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

संरचना thread_params {
	pthread_t thread;
	संरचना object_प्रविष्टिः **list;
	संरचना packing_region *regions;
	अचिह्नित list_आकार;
	अचिह्नित remaining;
	पूर्णाङ्क गवाक्षः;
	पूर्णाङ्क गभीरता;
	पूर्णाङ्क working;
	पूर्णाङ्क दत्त_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	अचिह्नित *processed;
};

स्थिर pthread_cond_t प्रगतिः_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
स्थिर शून्य init_threaded_अन्वेषणम्(शून्य)
{
	pthread_mutex_init(&cache_mutex, रिक्त);
	pthread_mutex_init(&प्रगतिः_mutex, रिक्त);
	pthread_cond_init(&प्रगतिः_cond, रिक्त);
}

स्थिर शून्य cleanup_threaded_अन्वेषणम्(शून्य)
{
	pthread_cond_destroy(&प्रगतिः_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&प्रगतिः_mutex);
}

स्थिर शून्य *threaded_find_deltas(शून्य *arg)
{
	संरचना thread_params *me = arg;

	प्रगतिः_तालकम्();
	यावत् (me->remaining) {
		प्रगतिः_unlock();

		find_deltas(me->list, &me->remaining,
			    me->गवाक्षः, me->गभीरता, me->processed);

		प्रगतिः_तालकम्();
		me->working = 0;
		pthread_cond_signal(&प्रगतिः_cond);
		प्रगतिः_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_तालकम्(&me->mutex);
		यावत् (!me->दत्त_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->दत्त_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		प्रगतिः_तालकम्();
	}
	प्रगतिः_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	प्रत्यागम रिक्त;
}

स्थिर शून्य ll_find_deltas(संरचना object_प्रविष्टिः **list, अचिह्नित list_आकार,
			   पूर्णाङ्क गवाक्षः, पूर्णाङ्क गभीरता, अचिह्नित *processed)
{
	संरचना thread_params *p;
	पूर्णाङ्क i, ret, सक्रिय_तन्तवः = 0;

	init_threaded_अन्वेषणम्();

	यदि (delta_अन्वेषणम्_तन्तवः <= 1) {
		find_deltas(list, &list_आकार, गवाक्षः, गभीरता, processed);
		cleanup_threaded_अन्वेषणम्();
		प्रत्यागम;
	}
	यदि (प्रगतिः > pack_गन्तव्यम्_stdout)
		fprintf_ln(stderr, _("Delta compression using up to %d threads"),
			   delta_अन्वेषणम्_तन्तवः);
	CALLOC_ARRAY(p, delta_अन्वेषणम्_तन्तवः);

	/* Partition the work amongst work threads. */
	प्रति (i = 0; i < delta_अन्वेषणम्_तन्तवः; i++) {
		अचिह्नित sub_आकार = list_आकार / (delta_अन्वेषणम्_तन्तवः - i);

		/* don't use too small segments or no deltas will be found */
		यदि (sub_आकार < 2*गवाक्षः && i+1 < delta_अन्वेषणम्_तन्तवः)
			sub_आकार = 0;

		p[i].गवाक्षः = गवाक्षः;
		p[i].गभीरता = गभीरता;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].दत्त_ready = 0;

		/* try to split chunks on "path" boundaries */
		यावत् (sub_आकार && sub_आकार < list_आकार &&
		       list[sub_आकार]->hash &&
		       list[sub_आकार]->hash == list[sub_आकार-1]->hash)
			sub_आकार++;

		p[i].list = list;
		p[i].list_आकार = sub_आकार;
		p[i].remaining = sub_आकार;

		list += sub_आकार;
		list_आकार -= sub_आकार;
	}

	/* Start work threads. */
	प्रति (i = 0; i < delta_अन्वेषणम्_तन्तवः; i++) {
		यदि (!p[i].list_आकार)
			अनुवर्त;
		pthread_mutex_init(&p[i].mutex, रिक्त);
		pthread_cond_init(&p[i].cond, रिक्त);
		ret = pthread_create(&p[i].thread, रिक्त,
				     threaded_find_deltas, &p[i]);
		यदि (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		सक्रिय_तन्तवः++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	यावत् (सक्रिय_तन्तवः) {
		संरचना thread_params *target = रिक्त;
		संरचना thread_params *victim = रिक्त;
		अचिह्नित sub_आकार = 0;

		प्रगतिः_तालकम्();
		प्रति (;;) {
			प्रति (i = 0; !target && i < delta_अन्वेषणम्_तन्तवः; i++)
				यदि (!p[i].working)
					target = &p[i];
			यदि (target)
				भङ्ग;
			pthread_cond_wait(&प्रगतिः_cond, &प्रगतिः_mutex);
		}

		प्रति (i = 0; i < delta_अन्वेषणम्_तन्तवः; i++)
			यदि (p[i].remaining > 2*गवाक्षः &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		यदि (victim) {
			sub_आकार = victim->remaining / 2;
			list = victim->list + victim->list_आकार - sub_आकार;
			यावत् (sub_आकार && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_आकार--;
			}
			यदि (!sub_आकार) {
				/*
				 * It is possible for some "paths" to have
				 * so many objects that no hash boundary
				 * might be found.  Let's just steal the
				 * exact half in that case.
				 */
				sub_आकार = victim->remaining / 2;
				list -= sub_आकार;
			}
			target->list = list;
			victim->list_आकार -= sub_आकार;
			victim->remaining -= sub_आकार;
		}
		target->list_आकार = sub_आकार;
		target->remaining = sub_आकार;
		target->working = 1;
		प्रगतिः_unlock();

		pthread_mutex_तालकम्(&target->mutex);
		target->दत्त_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		यदि (!sub_आकार) {
			pthread_join(target->thread, रिक्त);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			सक्रिय_तन्तवः--;
		}
	}
	cleanup_threaded_अन्वेषणम्();
	मुक्त(p);
}

स्थिर पूर्णाङ्क obj_is_packed(ध्रुव संरचना object_id *oid)
{
	प्रत्यागम packlist_find(&गन्तव्यम्_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_अन्तर्भवति(bitmap_git, reuse_packfile_bitmap, oid));
}

स्थिर शून्य सङ्कलय_चिह्नकम्_chain(ध्रुव संरचना object_id *oid)
{
	संरचना चिह्नकम् *चिह्नकम्;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	यदि (obj_is_packed(oid))
		प्रत्यागम;

	चिह्नकम् = lookup_चिह्नकम्(the_repository, oid);
	यावत् (1) {
		यदि (!चिह्नकम् || parse_चिह्नकम्(the_repository, चिह्नकम्) || !चिह्नकम्->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_गन्तव्यम्_hex(oid));

		सङ्कलय_object_प्रविष्टिः(&चिह्नकम्->object.oid, OBJ_चिह्नकम्, रिक्त, 0);

		यदि (चिह्नकम्->tagged->type != OBJ_चिह्नकम्)
			प्रत्यागम;

		चिह्नकम् = (संरचना चिह्नकम् *)चिह्नकम्->tagged;
	}
}

स्थिर पूर्णाङ्क सङ्कलय_ref_चिह्नकम्(ध्रुव संरचना reference *ref, शून्य *cb_दत्त अप्रयुक्तम्)
{
	संरचना object_id peeled;

	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		सङ्कलय_चिह्नकम्_chain(ref->oid);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क should_attempt_deltas(संरचना object_प्रविष्टिः *प्रविष्टिः)
{
	यदि (DELTA(प्रविष्टिः))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		प्रत्यागम 0;

	यदि (!प्रविष्टिः->type_valid ||
	    oe_आकार_less_than(&गन्तव्यम्_pack, प्रविष्टिः, 50))
		प्रत्यागम 0;

	यदि (प्रविष्टिः->no_try_delta)
		प्रत्यागम 0;

	यदि (!प्रविष्टिः->preferred_आधारः) {
		यदि (oe_type(प्रविष्टिः) < 0)
			die(_("unable to get type of object %s"),
				oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
	} अन्यथा यदि (oe_type(प्रविष्टिः) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर शून्य find_deltas_for_region(संरचना object_प्रविष्टिः *list,
				   संरचना packing_region *region,
				   अचिह्नित पूर्णाङ्क *processed)
{
	संरचना object_प्रविष्टिः **delta_list;
	अचिह्नित पूर्णाङ्क delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	प्रति (आकार_t i = 0; i < region->nr; i++) {
		संरचना object_प्रविष्टिः *प्रविष्टिः = list + region->start + i;
		यदि (should_attempt_deltas(प्रविष्टिः))
			delta_list[delta_list_nr++] = प्रविष्टिः;
	}

	QSORT(delta_list, delta_list_nr, type_आकार_क्रमय);
	find_deltas(delta_list, &delta_list_nr, गवाक्षः, गभीरता, processed);
	मुक्त(delta_list);
}

स्थिर शून्य find_deltas_by_region(संरचना object_प्रविष्टिः *list,
				  संरचना packing_region *regions,
				  आकार_t start, आकार_t nr)
{
	अचिह्नित पूर्णाङ्क processed = 0;
	आकार_t प्रगतिः_nr;

	यदि (!nr)
		प्रत्यागम;

	प्रगतिः_nr = regions[nr - 1].start + regions[nr - 1].nr;

	यदि (प्रगतिः)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Compressing objects by path"),
						प्रगतिः_nr);

	यावत् (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_प्रगतिः(प्रगतिः_state, प्रगतिः_nr);
	stop_प्रगतिः(&प्रगतिः_state);
}

स्थिर शून्य *threaded_find_deltas_by_मार्गः(शून्य *arg)
{
	संरचना thread_params *me = arg;

	प्रगतिः_तालकम्();
	यावत् (me->remaining) {
		यावत् (me->remaining) {
			प्रगतिः_unlock();
			find_deltas_for_region(गन्तव्यम्_pack.objects,
					       me->regions,
					       me->processed);
			प्रगतिः_तालकम्();
			me->remaining--;
			me->regions++;
		}

		me->working = 0;
		pthread_cond_signal(&प्रगतिः_cond);
		प्रगतिः_unlock();

		/*
		 * We must not set ->data_ready before we wait on the
		 * condition because the main thread may have set it to 1
		 * before we get here. In order to be sure that new
		 * work is available if we see 1 in ->data_ready, it
		 * was initialized to 0 before this thread was spawned
		 * and we reset it to 0 right away.
		 */
		pthread_mutex_तालकम्(&me->mutex);
		यावत् (!me->दत्त_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->दत्त_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		प्रगतिः_तालकम्();
	}
	प्रगतिः_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	प्रत्यागम रिक्त;
}

स्थिर शून्य ll_find_deltas_by_region(संरचना object_प्रविष्टिः *list,
				     संरचना packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	संरचना thread_params *p;
	पूर्णाङ्क i, ret, सक्रिय_तन्तवः = 0;
	अचिह्नित पूर्णाङ्क processed = 0;
	uint32_t प्रगतिः_nr;
	init_threaded_अन्वेषणम्();

	यदि (!nr)
		प्रत्यागम;

	प्रगतिः_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	यदि (delta_अन्वेषणम्_तन्तवः <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_अन्वेषणम्();
		प्रत्यागम;
	}

	यदि (प्रगतिः > pack_गन्तव्यम्_stdout)
		fprintf_ln(stderr,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_अन्वेषणम्_तन्तवः),
			   delta_अन्वेषणम्_तन्तवः);
	CALLOC_ARRAY(p, delta_अन्वेषणम्_तन्तवः);

	यदि (प्रगतिः)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Compressing objects by path"),
						प्रगतिः_nr);
	/* Partition the work amongst work threads. */
	प्रति (i = 0; i < delta_अन्वेषणम्_तन्तवः; i++) {
		अचिह्नित sub_आकार = nr / (delta_अन्वेषणम्_तन्तवः - i);

		p[i].गवाक्षः = गवाक्षः;
		p[i].गभीरता = गभीरता;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].दत्त_ready = 0;

		p[i].regions = regions;
		p[i].list_आकार = sub_आकार;
		p[i].remaining = sub_आकार;

		regions += sub_आकार;
		nr -= sub_आकार;
	}

	/* Start work threads. */
	प्रति (i = 0; i < delta_अन्वेषणम्_तन्तवः; i++) {
		यदि (!p[i].list_आकार)
			अनुवर्त;
		pthread_mutex_init(&p[i].mutex, रिक्त);
		pthread_cond_init(&p[i].cond, रिक्त);
		ret = pthread_create(&p[i].thread, रिक्त,
				     threaded_find_deltas_by_मार्गः, &p[i]);
		यदि (ret)
			die(_("unable to create thread: %s"), strerror(ret));
		सक्रिय_तन्तवः++;
	}

	/*
	 * Now let's wait for work completion.  Each time a thread is done
	 * with its work, we steal half of the remaining work from the
	 * thread with the largest number of unprocessed objects and give
	 * it to that newly idle thread.  This ensure good load balancing
	 * until the remaining object list segments are simply too short
	 * to be worth splitting anymore.
	 */
	यावत् (सक्रिय_तन्तवः) {
		संरचना thread_params *target = रिक्त;
		संरचना thread_params *victim = रिक्त;
		अचिह्नित sub_आकार = 0;

		प्रगतिः_तालकम्();
		प्रति (;;) {
			प्रति (i = 0; !target && i < delta_अन्वेषणम्_तन्तवः; i++)
				यदि (!p[i].working)
					target = &p[i];
			यदि (target)
				भङ्ग;
			pthread_cond_wait(&प्रगतिः_cond, &प्रगतिः_mutex);
		}

		प्रति (i = 0; i < delta_अन्वेषणम्_तन्तवः; i++)
			यदि (p[i].remaining > 2*गवाक्षः &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		यदि (victim) {
			sub_आकार = victim->remaining / 2;
			target->regions = victim->regions + victim->remaining - sub_आकार;
			victim->list_आकार -= sub_आकार;
			victim->remaining -= sub_आकार;
		}
		target->list_आकार = sub_आकार;
		target->remaining = sub_आकार;
		target->working = 1;
		प्रगतिः_unlock();

		pthread_mutex_तालकम्(&target->mutex);
		target->दत्त_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		यदि (!sub_आकार) {
			pthread_join(target->thread, रिक्त);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			सक्रिय_तन्तवः--;
		}
	}
	cleanup_threaded_अन्वेषणम्();
	मुक्त(p);

	display_प्रगतिः(प्रगतिः_state, प्रगतिः_nr);
	stop_प्रगतिः(&प्रगतिः_state);
}

स्थिर शून्य prepare_pack(पूर्णाङ्क गवाक्षः, पूर्णाङ्क गभीरता)
{
	संरचना object_प्रविष्टिः **delta_list;
	uint32_t i, nr_deltas;
	अचिह्नित n;

	यदि (use_delta_islands)
		resolve_वृक्षः_islands(the_repository, प्रगतिः, &गन्तव्यम्_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	यदि (!pack_गन्तव्यम्_stdout)
		do_check_packed_object_crc = 1;

	यदि (!गन्तव्यम्_pack.nr_objects || !गवाक्षः || !गभीरता)
		प्रत्यागम;

	यदि (मार्गः_walk)
		ll_find_deltas_by_region(गन्तव्यम्_pack.objects, गन्तव्यम्_pack.regions,
					 0, गन्तव्यम्_pack.nr_regions);

	ALLOC_ARRAY(delta_list, गन्तव्यम्_pack.nr_objects);
	nr_deltas = n = 0;

	प्रति (i = 0; i < गन्तव्यम्_pack.nr_objects; i++) {
		संरचना object_प्रविष्टिः *प्रविष्टिः = गन्तव्यम्_pack.objects + i;

		यदि (!should_attempt_deltas(प्रविष्टिः))
			अनुवर्त;

		यदि (!प्रविष्टिः->preferred_आधारः)
			nr_deltas++;

		delta_list[n++] = प्रविष्टिः;
	}

	यदि (nr_deltas && n > 1) {
		अचिह्नित nr_समाप्तम् = 0;

		यदि (प्रगतिः)
			प्रगतिः_state = start_प्रगतिः(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_आकार_क्रमय);
		ll_find_deltas(delta_list, n, गवाक्षः+1, गभीरता, &nr_समाप्तम्);
		stop_प्रगतिः(&प्रगतिः_state);
		यदि (nr_समाप्तम् != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	मुक्त(delta_list);
}

स्थिर पूर्णाङ्क git_pack_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			   ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(k, "pack.window")) {
		गवाक्षः = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.windowmemory")) {
		गवाक्षः_स्मृतिः_सीमा = git_config_ulong(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.depth")) {
		गभीरता = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.deltacachesize")) {
		max_delta_cache_आकार = git_config_आकार_t(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.deltacachelimit")) {
		cache_max_small_delta_आकार = git_config_int(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.writebitmaphashcache")) {
		यदि (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		अन्यथा
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	यदि (!वाक्यं_तुलय(k, "pack.writebitmaplookuptable")) {
		यदि (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_सारणी;
		अन्यथा
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_सारणी;
	}

	यदि (!वाक्यं_तुलय(k, "pack.usebitmaps")) {
		use_bitmap_सूचकांक_default = git_config_bool(k, v);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.allowpackreuse")) {
		पूर्णाङ्क res = git_parse_maybe_bool_text(v);
		यदि (res < 0) {
			यदि (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			अन्यथा यदि (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			अन्यथा
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} अन्यथा यदि (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} अन्यथा {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.threads")) {
		delta_अन्वेषणम्_तन्तवः = git_config_int(k, v, ctx->kvi);
		यदि (delta_अन्वेषणम्_तन्तवः < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_अन्वेषणम्_तन्तवः);
		यदि (!HAVE_तन्तवः && delta_अन्वेषणम्_तन्तवः != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_अन्वेषणम्_तन्तवः = 0;
		}
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.indexversion")) {
		pack_idx_opts.संस्करणम् = git_config_int(k, v, ctx->kvi);
		यदि (pack_idx_opts.संस्करणम् > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.संस्करणम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.writereverseindex")) {
		यदि (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		अन्यथा
			pack_idx_opts.flags &= ~WRITE_REV;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "uploadpack.blobpackfileuri")) {
		संरचना configured_exclusion *ex;
		ध्रुव वर्ण *oid_अन्तः, *pack_अन्तः;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		संरचना object_id pack_hash;

		यदि (!v)
			प्रत्यागम config_error_nonbool(k);

		ex = xmalloc(परिमाण(*ex));
		यदि (parse_oid_hex(v, &ex->e.oid, &oid_अन्तः) ||
		    *oid_अन्तः != ' ' ||
		    parse_oid_hex(oid_अन्तः + 1, &pack_hash, &pack_अन्तः) ||
		    *pack_अन्तः != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		यदि (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_अन्तः - oid_अन्तः);
		स्मृतिं_प्रतिलिख(ex->pack_hash_hex, oid_अन्तः + 1, pack_अन्तः - oid_अन्तः - 1);
		ex->uri = xstrdup(pack_अन्तः + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	प्रत्यागम git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
स्थिर पूर्णाङ्क stdin_packs_प्राप्तम्_nr;
स्थिर पूर्णाङ्क stdin_packs_hints_nr;

स्थिर पूर्णाङ्क सङ्कलय_object_प्रविष्टिः_from_pack(ध्रुव संरचना object_id *oid,
				      संरचना packed_git *p,
				      uint32_t pos,
				      शून्य *दत्त_2)
{
	off_t ofs;
	संरचना object_info oi = OBJECT_INFO_INIT;
	गणना object_type type = OBJ_NONE;

	display_प्रगतिः(प्रगतिः_state, ++nr_आलोकितम्);

	यदि (have_duplicate_प्रविष्टिः(oid, 0))
		प्रत्यागम 0;

	stdin_packs_प्राप्तम्_nr++;

	ofs = nth_packed_object_विस्थापनम्(p, pos);

	oi.typep = &type;
	यदि (packed_object_info(रिक्त, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_गन्तव्यम्_hex(oid), p->pack_name);
	} अन्यथा यदि (type == OBJ_COMMIT) {
		संरचना rev_info *revs = दत्त_2;
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
		सङ्कलय_pending_oid(revs, रिक्त, oid, 0);
	}

	यदि (!want_object_अन्तर्_pack(oid, 0, &p, &ofs))
		प्रत्यागम 0;

	create_object_प्रविष्टिः(oid, type, 0, 0, 0, p, ofs);

	प्रत्यागम 0;
}

स्थिर शून्य show_object_pack_hint(संरचना object *object, ध्रुव वर्ण *name,
				  शून्य *दत्त)
{
	गणना stdin_packs_बहुलकम् बहुलकम् = *(गणना stdin_packs_बहुलकम् *)दत्त;
	यदि (बहुलकम् == STDIN_PACKS_बहुलकम्_FOLLOW) {
		यदि (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			प्रत्यागम;
		सङ्कलय_object_प्रविष्टिः(&object->oid, object->type, name, 0);
	} अन्यथा {
		संरचना object_प्रविष्टिः *oe = packlist_find(&गन्तव्यम्_pack, &object->oid);
		यदि (!oe)
			प्रत्यागम;

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

स्थिर शून्य show_commit_pack_hint(संरचना commit *commit, शून्य *दत्त)
{
	गणना stdin_packs_बहुलकम् बहुलकम् = *(गणना stdin_packs_बहुलकम् *)दत्त;

	यदि (बहुलकम् == STDIN_PACKS_बहुलकम्_FOLLOW) {
		show_object_pack_hint((संरचना object *)commit, "", दत्त);
		प्रत्यागम;
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
गणना stdin_pack_info_विधा {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

संरचना stdin_pack_info {
	संरचना packed_git *p;
	गणना stdin_pack_info_विधा विधा;
};

स्थिर पूर्णाङ्क pack_mtime_cmp(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना stdin_pack_info *a = ((ध्रुव संरचना string_list_item*)_a)->util;
	संरचना stdin_pack_info *b = ((ध्रुव संरचना string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	यदि (a->p->mtime < b->p->mtime)
		प्रत्यागम 1;
	अन्यथा यदि (b->p->mtime < a->p->mtime)
		प्रत्यागम -1;
	अन्यथा
		प्रत्यागम 0;
}

स्थिर पूर्णाङ्क stdin_packs_include_check_obj(संरचना object *obj, शून्य *दत्त अप्रयुक्तम्)
{
	प्रत्यागम !has_object_kept_pack(गन्तव्यम्_pack.repo, &obj->oid,
				     KEPT_PACK_अन्तर्_CORE);
}

स्थिर पूर्णाङ्क stdin_packs_include_check(संरचना commit *commit, शून्य *दत्त)
{
	प्रत्यागम stdin_packs_include_check_obj((संरचना object *)commit, दत्त);
}

स्थिर शून्य stdin_packs_सङ्कलय_pack_entries(संरचना strmap *packs,
					 संरचना rev_info *revs)
{
	संरचना string_list कुञ्चिकाः = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *item;
	संरचना hashmap_iter iter;
	संरचना strmap_प्रविष्टिः *प्रविष्टिः;

	strmap_for_each_प्रविष्टिः(packs, &iter, प्रविष्टिः) {
		संरचना stdin_pack_info *info = प्रविष्टिः->मानम्;
		यदि (!info->p)
			die(_("could not find pack '%s'"), प्रविष्टिः->कुञ्चिका);

		string_list_append(&कुञ्चिकाः, प्रविष्टिः->कुञ्चिका)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(कुञ्चिकाः.वस्तूनि, कुञ्चिकाः.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &कुञ्चिकाः) {
		संरचना stdin_pack_info *info = item->util;

		यदि (info->विधा & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		यदि ((info->विधा & STDIN_PACK_INCLUDE) ||
		    (info->विधा & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_अन्तर्_pack(info->p,
						सङ्कलय_object_प्रविष्टिः_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_क्रमः);
	}

	string_list_clear(&कुञ्चिकाः, 0);
}

स्थिर शून्य stdin_packs_read_निवेशः(संरचना rev_info *revs,
				   गणना stdin_packs_बहुलकम् बहुलकम्)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strmap packs = STRMAP_INIT;
	संरचना packed_git *p;

	यावत् (strbuf_getline(&buf, stdin) != EOF) {
		संरचना stdin_pack_info *info;
		गणना stdin_pack_info_विधा विधा = STDIN_PACK_INCLUDE;
		ध्रुव वर्ण *कुञ्चिका = buf.buf;

		यदि (!*कुञ्चिका)
			अनुवर्त;
		अन्यथा यदि (*कुञ्चिका == '^')
			विधा = STDIN_PACK_EXCLUDE_CLOSED;
		अन्यथा यदि (*कुञ्चिका == '!' && बहुलकम् == STDIN_PACKS_बहुलकम्_FOLLOW)
			विधा = STDIN_PACK_EXCLUDE_OPEN;

		यदि (विधा != STDIN_PACK_INCLUDE)
			कुञ्चिका++;

		info = strmap_get(&packs, कुञ्चिका);
		यदि (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, कुञ्चिका, info);
		}

		info->विधा |= विधा;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		संरचना stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		यदि (!info)
			अनुवर्त;

		यदि (info->विधा & STDIN_PACK_INCLUDE) {
			यदि (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			यदि (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		यदि (info->विधा & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_अन्तर्_core = 1;
		}

		यदि (info->विधा & STDIN_PACK_EXCLUDE_OPEN) {
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
			p->pack_keep_अन्तर्_core_open = 1;
		}

		info->p = p;
	}

	stdin_packs_सङ्कलय_pack_entries(&packs, revs);

	strbuf_release(&buf);
	strmap_clear(&packs, 1);
}

स्थिर शून्य सङ्कलय_unreachable_loose_objects(संरचना rev_info *revs);

स्थिर शून्य read_stdin_packs(संरचना repository *repo,
			     गणना stdin_packs_बहुलकम् बहुलकम्, पूर्णाङ्क rev_list_unpacked)
{
	पूर्णाङ्क पूर्व_fetch_if_missing = repo->fetch_if_missing;
	संरचना rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, रिक्त);
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
	revs.keep_pack_cache_flags |= KEPT_PACK_अन्तर्_CORE;
	revs.blob_objects = 1;
	revs.वृक्षः_objects = 1;
	revs.चिह्नकम्_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_अन्तर्_core = 1;
	यदि (बहुलकम् == STDIN_PACKS_बहुलकम्_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_अन्तर्_core_open = 1;
	}
	stdin_packs_read_निवेशः(&revs, बहुलकम्);
	यदि (rev_list_unpacked)
		सङ्कलय_unreachable_loose_objects(&revs);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &बहुलकम्);

	release_revisions(&revs);

	trace2_दत्त_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_प्राप्तम्_nr);
	trace2_दत्त_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = पूर्व_fetch_if_missing;
}

स्थिर शून्य सङ्कलय_cruft_object_प्रविष्टिः(ध्रुव संरचना object_id *oid, गणना object_type type,
				   संरचना packed_git *pack, off_t विस्थापनम्,
				   ध्रुव वर्ण *name, uint32_t mtime)
{
	संरचना object_प्रविष्टिः *प्रविष्टिः;

	display_प्रगतिः(प्रगतिः_state, ++nr_आलोकितम्);

	प्रविष्टिः = packlist_find(&गन्तव्यम्_pack, oid);
	यदि (प्रविष्टिः) {
		यदि (name) {
			प्रविष्टिः->hash = pack_name_hash_fn(name);
			प्रविष्टिः->no_try_delta = no_try_delta(name);
		}
	} अन्यथा {
		यदि (!want_object_अन्तर्_pack_mtime(oid, 0, &pack, &विस्थापनम्, mtime))
			प्रत्यागम;
		यदि (!pack && type == OBJ_BLOB) {
			संरचना odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			पूर्णाङ्क प्राप्तम्_2 = 0;

			प्रति (संरचना odb_files_dir *dir = files->dirs; !प्राप्तम्_2 && dir; dir = dir->अग्र)
				यदि (!odb_source_read_object_info(&dir->loose->आधारः, oid, रिक्त, 0, रिक्त))
					प्राप्तम्_2 = 1;

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
			यदि (!प्राप्तम्_2)
				प्रत्यागम;
		}

		प्रविष्टिः = create_object_प्रविष्टिः(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, विस्थापनम्);
	}

	यदि (mtime > oe_cruft_mtime(&गन्तव्यम्_pack, प्रविष्टिः))
		oe_समुच्चयः_cruft_mtime(&गन्तव्यम्_pack, प्रविष्टिः, mtime);
	प्रत्यागम;
}

स्थिर शून्य show_cruft_object(संरचना object *obj, ध्रुव वर्ण *name, शून्य *दत्त अप्रयुक्तम्)
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
	सङ्कलय_cruft_object_प्रविष्टिः(&obj->oid, obj->type, रिक्त, 0, name, cruft_expiration);
}

स्थिर शून्य show_cruft_commit(संरचना commit *commit, शून्य *दत्त)
{
	show_cruft_object((संरचना object*)commit, रिक्त, दत्त);
}

स्थिर पूर्णाङ्क cruft_include_check_obj(संरचना object *obj, शून्य *दत्त अप्रयुक्तम्)
{
	प्रत्यागम !has_object_kept_pack(गन्तव्यम्_pack.repo, &obj->oid, KEPT_PACK_अन्तर्_CORE);
}

स्थिर पूर्णाङ्क cruft_include_check(संरचना commit *commit, शून्य *दत्त)
{
	प्रत्यागम cruft_include_check_obj((संरचना object*)commit, दत्त);
}

स्थिर शून्य समुच्चयः_cruft_mtime(ध्रुव संरचना object *object,
			    संरचना packed_git *pack,
			    off_t विस्थापनम्, time_t mtime)
{
	सङ्कलय_cruft_object_प्रविष्टिः(&object->oid, object->type, pack, विस्थापनम्, रिक्त,
			       mtime);
}

स्थिर शून्य mark_pack_kept_अन्तर्_core(संरचना string_list *packs, अचिह्नित keep)
{
	संरचना string_list_item *item = रिक्त;
	for_each_string_list_item(item, packs) {
		संरचना packed_git *p = item->util;
		यदि (!p)
			die(_("could not find pack '%s'"), item->string);
		यदि (p->is_cruft && keep)
			ignore_packed_keep_अन्तर्_core_has_cruft = 1;
		p->pack_keep_अन्तर्_core = keep;
	}
}

स्थिर शून्य सङ्कलय_objects_अन्तर्_unpacked_packs(शून्य);

स्थिर शून्य enumerate_cruft_objects(शून्य)
{
	यदि (प्रगतिः)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Enumerating cruft objects"), 0);

	सङ्कलय_objects_अन्तर्_unpacked_packs();
	सङ्कलय_unreachable_loose_objects(रिक्त);

	stop_प्रगतिः(&प्रगतिः_state);
}

स्थिर शून्य enumerate_and_traverse_cruft_objects(संरचना string_list *fresh_packs)
{
	संरचना packed_git *p;
	संरचना rev_info revs;
	पूर्णाङ्क ret;

	repo_init_revisions(the_repository, &revs, रिक्त);

	revs.चिह्नकम्_objects = 1;
	revs.वृक्षः_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	यदि (प्रगतिः)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = सङ्कलय_unseen_recent_objects_गन्तव्यम्_traversal(&revs, cruft_expiration,
						     समुच्चयः_cruft_mtime, 1);
	stop_प्रगतिः(&प्रगतिः_state);

	यदि (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_अन्तर्_core = 0;
	mark_pack_kept_अन्तर्_core(fresh_packs, 1);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	यदि (प्रगतिः)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Traversing cruft objects"), 0);
	nr_आलोकितम् = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, रिक्त);

	stop_प्रगतिः(&प्रगतिः_state);
	release_revisions(&revs);
}

स्थिर शून्य read_cruft_objects(शून्य)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list discard_packs = STRING_LIST_INIT_DUP;
	संरचना string_list fresh_packs = STRING_LIST_INIT_DUP;
	संरचना packed_git *p;

	ignore_packed_keep_अन्तर्_core = 1;

	यावत् (strbuf_getline(&buf, stdin) != EOF) {
		यदि (!buf.len)
			अनुवर्त;

		यदि (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		अन्यथा
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_क्रमय(&discard_packs);
	string_list_क्रमय(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		ध्रुव वर्ण *pack_name = pack_basename(p);
		संरचना string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		यदि (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		यदि (item) {
			item->util = p;
		} अन्यथा {
			/*
			 * This pack wasn't mentioned in either the "fresh" or
			 * "discard" list, so the caller didn't know about it.
			 *
			 * Mark it as kept so that its objects are ignored by
			 * add_unseen_recent_objects_to_traversal(). We'll
			 * unmark it before starting the traversal so it doesn't
			 * halt the traversal early.
			 */
			p->pack_keep_अन्तर्_core = 1;
		}
	}

	mark_pack_kept_अन्तर्_core(&fresh_packs, 1);
	mark_pack_kept_अन्तर्_core(&discard_packs, 0);

	यदि (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	अन्यथा
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

स्थिर शून्य read_object_list_from_stdin(शून्य)
{
	वर्ण पङ्क्तिः[GIT_MAX_HEXSZ + 1 + मार्गः_MAX + 2];
	संरचना object_id oid;
	ध्रुव वर्ण *p;

	प्रति (;;) {
		यदि (!fgets(पङ्क्तिः, परिमाण(पङ्क्तिः), stdin)) {
			यदि (feof(stdin))
				भङ्ग;
			यदि (!ferror(stdin))
				BUG("fgets returned NULL, not EOF, not error!");
			यदि (errno != EINTR)
				die_errno("fgets");
			clearerr(stdin);
			अनुवर्त;
		}
		यदि (पङ्क्तिः[0] == '-') {
			यदि (get_oid_hex(पङ्क्तिः+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    पङ्क्तिः);
			सङ्कलय_preferred_आधारः(&oid);
			अनुवर्त;
		}
		यदि (parse_oid_hex(पङ्क्तिः, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), पङ्क्तिः);

		सङ्कलय_preferred_आधारः_object(p + 1);
		सङ्कलय_object_प्रविष्टिः(&oid, OBJ_NONE, p + 1, 0);
	}
}

स्थिर शून्य show_commit(संरचना commit *commit, शून्य *दत्त अप्रयुक्तम्)
{
	सङ्कलय_object_प्रविष्टिः(&commit->object.oid, OBJ_COMMIT, रिक्त, 0);

	यदि (write_bitmap_सूचकांक)
		सूचकांक_commit_for_bitmap(commit);

	यदि (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

स्थिर शून्य show_object(संरचना object *obj, ध्रुव वर्ण *name,
			शून्य *दत्त अप्रयुक्तम्)
{
	सङ्कलय_preferred_आधारः_object(name);
	सङ्कलय_object_प्रविष्टिः(&obj->oid, obj->type, name, 0);

	यदि (use_delta_islands)
		लेखः_वृक्षः_गभीरता(&obj->oid, name);
}

स्थिर शून्य show_object__ma_allow_any(संरचना object *obj, ध्रुव वर्ण *name, शून्य *दत्त)
{
	प्रतिज्ञा(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	यदि (!odb_has_object(the_repository->objects, &obj->oid, 0))
		प्रत्यागम;

	show_object(obj, name, दत्त);
}

स्थिर शून्य show_object__ma_allow_promisor(संरचना object *obj, ध्रुव वर्ण *name, शून्य *दत्त)
{
	प्रतिज्ञा(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	यदि (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(गन्तव्यम्_pack.repo, &obj->oid))
		प्रत्यागम;

	show_object(obj, name, दत्त);
}

स्थिर पूर्णाङ्क option_parse_missing_action(ध्रुव संरचना option *opt,
				       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना repository *repo = opt->मानम्;

	प्रतिज्ञा(arg);
	प्रतिज्ञा(!unset);

	यदि (!वाक्यं_तुलय(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		प्रत्यागम 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	प्रत्यागम 0;
}

स्थिर शून्य show_edge(संरचना commit *commit)
{
	सङ्कलय_preferred_आधारः(&commit->object.oid);
}

स्थिर पूर्णाङ्क सङ्कलय_object_अन्तर्_unpacked_pack(ध्रुव संरचना object_id *oid,
				       संरचना object_info *oi,
				       शून्य *दत्त अप्रयुक्तम्)
{
	यदि (cruft) {
		सङ्कलय_cruft_object_प्रविष्टिः(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.विस्थापनम्, रिक्त,
				       *oi->mtimep);
	} अन्यथा {
		सङ्कलय_object_प्रविष्टिः(oid, OBJ_NONE, "", 0);
	}
	प्रत्यागम 0;
}

स्थिर शून्य सङ्कलय_objects_अन्तर्_unpacked_packs(शून्य)
{
	संरचना odb_source_files *files =
		odb_source_files_downcast(गन्तव्यम्_pack.repo->objects->source);
	time_t mtime;
	संरचना odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_क्रमः |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_अन्तर्_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	संरचना odb_source_info source_info;
	संरचना object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		यदि (!dir->local)
			अनुवर्त;

		यदि (odb_source_for_each_object(&dir->packed->आधारः, &oi,
					       सङ्कलय_object_अन्तर्_unpacked_pack, रिक्त, &opts))
			die(_("cannot open pack index"));
	}
}

स्थिर पूर्णाङ्क सङ्कलय_loose_object(ध्रुव संरचना object_id *oid, ध्रुव वर्ण *मार्गः,
			    शून्य *दत्त)
{
	संरचना rev_info *revs = दत्त;
	गणना object_type type = odb_read_object_info(the_repository->objects, oid, रिक्त);

	यदि (type < 0) {
		warning(_("loose object at %s could not be examined"), मार्गः);
		प्रत्यागम 0;
	}

	यदि (cruft) {
		संरचना stat st;
		यदि (stat(मार्गः, &st) < 0) {
			यदि (errno == ENOENT)
				प्रत्यागम 0;
			प्रत्यागम error_errno("unable to stat %s", oid_गन्तव्यम्_hex(oid));
		}

		सङ्कलय_cruft_object_प्रविष्टिः(oid, type, रिक्त, 0, रिक्त,
				       st.st_mtime);
	} अन्यथा {
		सङ्कलय_object_प्रविष्टिः(oid, type, "", 0);
	}

	यदि (revs && type == OBJ_COMMIT)
		सङ्कलय_pending_oid(revs, रिक्त, oid, 0);

	प्रत्यागम 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
स्थिर शून्य सङ्कलय_unreachable_loose_objects(संरचना rev_info *revs)
{
	for_each_loose_file_अन्तर्_source(the_repository->objects->source,
				      सङ्कलय_loose_object, रिक्त, रिक्त, revs);
}

स्थिर पूर्णाङ्क has_sha1_pack_kept_or_nonlocal(ध्रुव संरचना object_id *oid)
{
	स्थिर संरचना packed_git *चरमः_प्राप्तम् = रिक्त;
	संरचना packed_git *p;

	यदि (चरमः_प्राप्तम् && find_pack_प्रविष्टिः_one(oid, चरमः_प्राप्तम्))
		प्रत्यागम 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		यदि (p == चरमः_प्राप्तम्)
			अनुवर्त;

		यदि ((!p->pack_local || p->pack_keep || p->pack_keep_अन्तर्_core) &&
		    find_pack_प्रविष्टिः_one(oid, p)) {
			चरमः_प्राप्तम् = p;
			प्रत्यागम 1;
		}
	}

	प्रत्यागम 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
स्थिर संरचना oid_array recent_objects;

स्थिर पूर्णाङ्क loosened_object_can_be_discarded(ध्रुव संरचना object_id *oid,
					    timestamp_t mtime)
{
	यदि (!unpack_unreachable_expiration)
		प्रत्यागम 0;
	यदि (mtime > unpack_unreachable_expiration)
		प्रत्यागम 0;
	यदि (oid_array_lookup(&recent_objects, oid) >= 0)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क force_object_loose(संरचना odb_source *source,
			      ध्रुव संरचना object_id *oid,
			      ध्रुव time_t *mtime)
{
	संरचना odb_source_files *files = odb_source_files_downcast(source);
	ध्रुव संरचना git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	संरचना object_info oi = OBJECT_INFO_INIT;
	संरचना object_id compat_oid, *compat_oid_p = रिक्त;
	गणना object_type type;
	शून्य *buf = रिक्त;
	आकार_t len;
	पूर्णाङ्क ret;

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र)
		यदि (!odb_source_read_object_info(&dir->loose->आधारः, oid, रिक्त, 0, रिक्त))
			प्रत्यागम 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	यदि (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_गन्तव्यम्_hex(oid));
		गच्छ बहिर्गमनम्;
	}

	यदि (compat) {
		यदि (repo_oid_गन्तव्यम्_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_गन्तव्यम्_hex(oid), compat->name);
			गच्छ बहिर्गमनम्;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->आधारः, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

बहिर्गमनम्:
	मुक्त(buf);
	प्रत्यागम ret;
}

स्थिर शून्य loosen_अप्रयुक्तम्_packed_objects(शून्य)
{
	संरचना packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	संरचना object_id oid;

	repo_for_each_pack(the_repository, p) {
		यदि (!p->pack_local || p->pack_keep || p->pack_keep_अन्तर्_core)
			अनुवर्त;

		यदि (open_pack_सूचकांक(p))
			die(_("cannot open pack index"));

		प्रति (i = 0; i < p->अङ्कः_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			यदि (!packlist_find(&गन्तव्यम्_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				यदि (force_object_loose(the_repository->objects->source,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_दत्त_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
स्थिर पूर्णाङ्क pack_options_allow_reuse(शून्य)
{
	प्रत्यागम allow_pack_reuse != NO_PACK_REUSE &&
	       pack_गन्तव्यम्_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_अन्तर्_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

स्थिर पूर्णाङ्क get_object_list_from_bitmap(संरचना rev_info *revs)
{
	यदि (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		प्रत्यागम -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_संस्करणम् = 1;

	यदि (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	यदि (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		यदि (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_परिणाम += reuse_packfile_objects;
		nr_आलोकितम् += reuse_packfile_objects;
		display_प्रगतिः(प्रगतिः_state, nr_आलोकितम्);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &सङ्कलय_object_प्रविष्टिः_from_bitmap);
	प्रत्यागम 0;
}

स्थिर शून्य लेखः_recent_object(संरचना object *obj,
				 ध्रुव वर्ण *name अप्रयुक्तम्,
				 शून्य *दत्त अप्रयुक्तम्)
{
	oid_array_append(&recent_objects, &obj->oid);
}

स्थिर शून्य लेखः_recent_commit(संरचना commit *commit, शून्य *दत्त अप्रयुक्तम्)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

स्थिर पूर्णाङ्क mark_bitmap_preferred_tip(ध्रुव संरचना reference *ref, शून्य *दत्त अप्रयुक्तम्)
{
	ध्रुव संरचना object_id *maybe_peeled = ref->oid;
	संरचना object_id peeled;
	संरचना object *object;

	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	यदि (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	प्रत्यागम 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क is_oid_uninteresting(संरचना repository *repo,
				       संरचना object_id *oid)
{
	संरचना object *o = lookup_object(repo, oid);
	प्रत्यागम !o || (o->flags & UNINTERESTING);
}

स्थिर पूर्णाङ्क सङ्कलय_objects_by_मार्गः(ध्रुव वर्ण *मार्गः,
			       संरचना oid_array *oids,
			       गणना object_type type,
			       शून्य *दत्त)
{
	आकार_t oe_start = गन्तव्यम्_pack.nr_objects;
	आकार_t oe_अन्तः;
	अचिह्नित पूर्णाङ्क *processed = दत्त;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	प्रति (आकार_t i = 0; i < oids->nr; i++) {
		पूर्णाङ्क exclude;
		संरचना object_info oi = OBJECT_INFO_INIT;
		संरचना object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		यदि ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			अनुवर्त;

		exclude = is_oid_uninteresting(the_repository, oid);

		यदि (exclude && !thin)
			अनुवर्त;

		सङ्कलय_object_प्रविष्टिः(oid, type, मार्गः, exclude);

		यदि (type == OBJ_COMMIT) {
			संरचना commit *commit;

			यदि (!write_bitmap_सूचकांक && !use_delta_islands)
				अनुवर्त;

			commit = lookup_commit(the_repository, oid);
			यदि (!commit)
				die(_("could not find commit %s"), oid_गन्तव्यम्_hex(oid));
			यदि (write_bitmap_सूचकांक)
				सूचकांक_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			यदि (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} अन्यथा यदि (type == OBJ_वृक्षः && use_delta_islands) {
			लेखः_वृक्षः_गभीरता(oid, मार्गः);
		}
	}

	oe_अन्तः = गन्तव्यम्_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	यदि (oe_अन्तः == oe_start || !गवाक्षः)
		प्रत्यागम 0;

	ALLOC_GROW(गन्तव्यम्_pack.regions,
		   गन्तव्यम्_pack.nr_regions + 1,
		   गन्तव्यम्_pack.nr_regions_alloc);

	गन्तव्यम्_pack.regions[गन्तव्यम्_pack.nr_regions].start = oe_start;
	गन्तव्यम्_pack.regions[गन्तव्यम्_pack.nr_regions].nr = oe_अन्तः - oe_start;
	गन्तव्यम्_pack.nr_regions++;

	*processed += oids->nr;
	display_प्रगतिः(प्रगतिः_state, *processed);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_object_list_मार्गः_walk(संरचना rev_info *revs)
{
	संरचना मार्गः_walk_info info = मार्गः_WALK_INFO_INIT;
	अचिह्नित पूर्णाङ्क processed = 0;
	पूर्णाङ्क परिणाम;

	info.revs = revs;
	info.मार्गः_fn = सङ्कलय_objects_by_मार्गः;
	info.मार्गः_fn_दत्त = &processed;

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
	परिणाम = walk_objects_by_मार्गः(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	मार्गः_walk_info_clear(&info);

	प्रत्यागम परिणाम;
}

स्थिर शून्य get_object_list(संरचना rev_info *revs, संरचना strvec *argv)
{
	संरचना सज्जता_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);
	वर्ण पङ्क्तिः[1000];
	पूर्णाङ्क flags = 0;
	पूर्णाङ्क save_warning;

	save_commit_बफर = 0;
	सज्जता_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	यावत् (fgets(पङ्क्तिः, परिमाण(पङ्क्तिः), stdin) != रिक्त) {
		पूर्णाङ्क len = वाक्यदैर्घ्यम्(पङ्क्तिः);
		यदि (len && पङ्क्तिः[len - 1] == '\n')
			पङ्क्तिः[--len] = 0;
		यदि (!len)
			भङ्ग;
		यदि (*पङ्क्तिः == '-') {
			यदि (!वाक्यं_तुलय(पङ्क्तिः, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_सूचकांक = 0;
				अनुवर्त;
			}
			यदि (starts_with(पङ्क्तिः, "--shallow ")) {
				संरचना object_id oid;
				यदि (get_oid_hex(पङ्क्तिः + 10, &oid))
					die("not an object name '%s'", पङ्क्तिः + 10);
				पञ्जिकाकोशः_shallow(the_repository, &oid);
				use_bitmap_सूचकांक = 0;
				अनुवर्त;
			}
			die(_("not a rev '%s'"), पङ्क्तिः);
		}
		यदि (handle_revision_arg(पङ्क्तिः, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), पङ्क्तिः);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	यदि (use_bitmap_सूचकांक && !get_object_list_from_bitmap(revs))
		प्रत्यागम;

	यदि (use_delta_islands)
		आरोपय_delta_islands(the_repository, प्रगतिः);

	यदि (write_bitmap_सूचकांक)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      रिक्त);

	यदि (!fn_show_object)
		fn_show_object = show_object;

	यदि (मार्गः_walk) {
		यदि (get_object_list_मार्गः_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			मार्गः_walk = 0;
		}
	}

	यदि (!मार्गः_walk) {
		यदि (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_धाराः_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				रिक्त);
	}

	यदि (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		यदि (सङ्कलय_unseen_recent_objects_गन्तव्यम्_traversal(revs,
				unpack_unreachable_expiration, रिक्त, 0))
			die(_("unable to add recent objects"));
		यदि (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, लेखः_recent_commit,
				     लेखः_recent_object, रिक्त);
	}

	यदि (keep_unreachable)
		सङ्कलय_objects_अन्तर्_unpacked_packs();
	यदि (pack_loose_unreachable)
		सङ्कलय_unreachable_loose_objects(रिक्त);
	यदि (unpack_unreachable)
		loosen_अप्रयुक्तम्_packed_objects();

	oid_array_clear(&recent_objects);
}

स्थिर शून्य सङ्कलय_extra_kept_packs(ध्रुव संरचना string_list *नामानि)
{
	संरचना packed_git *p;

	यदि (!नामानि->nr)
		प्रत्यागम;

	repo_for_each_pack(the_repository, p) {
		ध्रुव वर्ण *name = basename(p->pack_name);
		पूर्णाङ्क i;

		यदि (!p->pack_local)
			अनुवर्त;

		प्रति (i = 0; i < नामानि->nr; i++)
			यदि (!fspathcmp(name, नामानि->वस्तूनि[i].string))
				भङ्ग;

		यदि (i < नामानि->nr) {
			p->pack_keep_अन्तर्_core = 1;
			ignore_packed_keep_अन्तर्_core = 1;
			अनुवर्त;
		}
	}
}

स्थिर पूर्णाङ्क option_parse_quiet(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			      पूर्णाङ्क unset)
{
	पूर्णाङ्क *val = opt->मानम्;

	BUG_ON_OPT_ARG(arg);

	यदि (!unset)
		*val = 0;
	अन्यथा यदि (!*val)
		*val = 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_सूचकांक_संस्करणम्(ध्रुव संरचना option *opt,
				      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना pack_idx_option *popts = opt->मानम्;
	वर्ण *c;
	ध्रुव वर्ण *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->संस्करणम् = strtoul(val, &c, 10);
	यदि (popts->संस्करणम् > 2)
		die(_("unsupported index version %s"), val);
	यदि (*c == ',' && c[1])
		popts->off32_सीमा = strtoul(c+1, &c, 0);
	यदि (*c || popts->off32_सीमा & 0x80000000)
		die(_("bad index version '%s'"), val);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_unpack_unreachable(ध्रुव संरचना option *opt अप्रयुक्तम्,
					   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	यदि (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	अन्यथा {
		unpack_unreachable = 1;
		यदि (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_cruft_expiration(ध्रुव संरचना option *opt अप्रयुक्तम्,
					 ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	यदि (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} अन्यथा {
		cruft = 1;
		यदि (arg)
			cruft_expiration = approxidate(arg);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क is_not_अन्तर्_promisor_pack_obj(संरचना object *obj, शून्य *दत्त अप्रयुक्तम्)
{
	संरचना odb_source_info source_info;
	संरचना object_info info = {
		.source_infop = &source_info,
	};

	यदि (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	प्रत्यागम source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

स्थिर पूर्णाङ्क is_not_अन्तर्_promisor_pack(संरचना commit *commit, शून्य *दत्त) {
	प्रत्यागम is_not_अन्तर्_promisor_pack_obj((संरचना object *) commit, दत्त);
}

स्थिर पूर्णाङ्क parse_stdin_packs_बहुलकम्(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				  पूर्णाङ्क unset)
{
	गणना stdin_packs_बहुलकम् *बहुलकम् = opt->मानम्;

	यदि (unset)
		*बहुलकम् = STDIN_PACKS_बहुलकम्_NONE;
	अन्यथा यदि (!arg || !*arg)
		*बहुलकम् = STDIN_PACKS_बहुलकम्_STANDARD;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "follow"))
		*बहुलकम् = STDIN_PACKS_बहुलकम्_FOLLOW;
	अन्यथा
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_pack_objects(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *उपसर्गः,
		     संरचना repository *repo)
{
	पूर्णाङ्क use_आन्तरिकम्_rev_list = 0;
	पूर्णाङ्क all_प्रगतिः_implied = 0;
	संरचना strvec rp = STRVEC_INIT;
	पूर्णाङ्क rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	पूर्णाङ्क rev_list_सूचकांक = 0;
	गणना stdin_packs_बहुलकम् stdin_packs = STDIN_PACKS_बहुलकम्_NONE;
	संरचना string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	संरचना list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	संरचना option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &प्रगतिः, रिक्त,
			       N_("do not show progress meter"),
			       PARSE_OPT_NOARG, option_parse_quiet),
		OPT_समुच्चयः_INT(0, "progress", &प्रगतिः,
			    N_("show progress meter"), 1),
		OPT_समुच्चयः_INT(0, "all-progress", &प्रगतिः,
			    N_("show progress meter during object writing phase"), 2),
		OPT_BOOL(0, "all-progress-implied",
			 &all_प्रगतिः_implied,
			 N_("similar to --all-progress when progress meter is shown")),
		OPT_CALLBACK_F(0, "index-version", &pack_idx_opts, N_("<version>[,<offset>]"),
		  N_("write the pack index file in the specified idx format version"),
		  PARSE_OPT_NONEG, option_parse_सूचकांक_संस्करणम्),
		OPT_UNSIGNED(0, "max-pack-size", &pack_आकार_सीमा,
			     N_("maximum size of each output pack file")),
		OPT_BOOL(0, "local", &local,
			 N_("ignore borrowed objects from alternate object store")),
		OPT_BOOL(0, "incremental", &incremental,
			 N_("ignore packed objects")),
		OPT_INTEGER(0, "window", &गवाक्षः,
			    N_("limit pack window by objects")),
		OPT_UNSIGNED(0, "window-memory", &गवाक्षः_स्मृतिः_सीमा,
			     N_("limit pack window by memory in addition to object limit")),
		OPT_INTEGER(0, "depth", &गभीरता,
			    N_("maximum length of delta chain allowed in the resulting pack")),
		OPT_BOOL(0, "reuse-delta", &reuse_delta,
			 N_("reuse existing deltas")),
		OPT_BOOL(0, "reuse-object", &reuse_object,
			 N_("reuse existing objects")),
		OPT_BOOL(0, "delta-base-offset", &allow_ofs_delta,
			 N_("use OFS_DELTA objects")),
		OPT_INTEGER(0, "threads", &delta_अन्वेषणम्_तन्तवः,
			    N_("use threads when searching for best delta matches")),
		OPT_BOOL(0, "non-empty", &non_रिक्तानि,
			 N_("do not create an empty pack output")),
		OPT_BOOL(0, "revs", &use_आन्तरिकम्_rev_list,
			 N_("read revision arguments from standard input")),
		OPT_समुच्चयः_INT_F(0, "unpacked", &rev_list_unpacked,
			      N_("limit the objects to those that are not yet packed"),
			      1, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "all", &rev_list_all,
			      N_("include objects reachable from any reference"),
			      1, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "reflog", &rev_list_reflog,
			      N_("include objects referred by reflog entries"),
			      1, PARSE_OPT_NONEG),
		OPT_समुच्चयः_INT_F(0, "indexed-objects", &rev_list_सूचकांक,
			      N_("include objects referred to by the index"),
			      1, PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "stdin-packs", &stdin_packs, N_("mode"),
			     N_("read packs from stdin"),
			     PARSE_OPT_OPTARG, parse_stdin_packs_बहुलकम्),
		OPT_BOOL(0, "stdout", &pack_गन्तव्यम्_stdout,
			 N_("output pack to stdout")),
		OPT_BOOL(0, "include-tag", &include_चिह्नकम्,
			 N_("include tag objects that refer to objects to be packed")),
		OPT_BOOL(0, "keep-unreachable", &keep_unreachable,
			 N_("keep unreachable objects")),
		OPT_BOOL(0, "pack-loose-unreachable", &pack_loose_unreachable,
			 N_("pack loose unreachable objects")),
		OPT_CALLBACK_F(0, "unpack-unreachable", रिक्त, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", रिक्त, N_("time"),
		  N_("expire cruft objects older than <time>"),
		  PARSE_OPT_OPTARG, option_parse_cruft_expiration),
		OPT_BOOL(0, "sparse", &sparse,
			 N_("use the sparse reachability algorithm")),
		OPT_BOOL(0, "thin", &thin,
			 N_("create thin packs")),
		OPT_BOOL(0, "path-walk", &मार्गः_walk,
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_सूचकांक,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_समुच्चयः_INT(0, "write-bitmap-index", &write_bitmap_सूचकांक,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_समुच्चयः_INT_F(0, "write-bitmap-index-quiet",
			      &write_bitmap_सूचकांक,
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
		OPT_INTEGER(0, "name-hash-version", &name_hash_संस्करणम्,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_अन्तः(),
	};

	यदि (DFS_अङ्कः_अवस्थाः > (1 << OE_DFS_STATE_द्वयङ्काः))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		यदि (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		यदि (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, रिक्त);
	यदि (git_env_bool(GIT_TEST_NO_WRITE_REV_सूचकांक, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	प्रगतिः = isatty(2);
	argc = parse_options(argc, argv, उपसर्गः, pack_objects_options,
			     pack_usage, 0);

	यदि (argc) {
		आधारः_name = argv[0];
		argc--;
	}
	यदि (pack_गन्तव्यम्_stdout != !आधारः_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	यदि (मार्गः_walk < 0) {
		यदि (use_bitmap_सूचकांक > 0 ||
		    !use_आन्तरिकम्_rev_list)
			मार्गः_walk = 0;
		अन्यथा यदि (the_repository->gitdir &&
			 the_repository->settings.pack_use_मार्गः_walk)
			मार्गः_walk = 1;
		अन्यथा
			मार्गः_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	यदि (गभीरता < 0)
		गभीरता = 0;
	यदि (गभीरता >= (1 << OE_गभीरता_द्वयङ्काः)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			गभीरता, (1 << OE_गभीरता_द्वयङ्काः) - 1);
		गभीरता = (1 << OE_गभीरता_द्वयङ्काः) - 1;
	}
	यदि (cache_max_small_delta_आकार >= (1U << OE_Z_DELTA_द्वयङ्काः)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_द्वयङ्काः) - 1);
		cache_max_small_delta_आकार = (1U << OE_Z_DELTA_द्वयङ्काः) - 1;
	}
	यदि (गवाक्षः < 0)
		गवाक्षः = 0;

	strvec_प्रक्षिप(&rp, "pack-objects");

	यदि (मार्गः_walk) {
		ध्रुव वर्ण *option = रिक्त;
		यदि (!मार्गः_walk_filter_compatible(&filter_options))
			option = "--filter";

		यदि (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			मार्गः_walk = 0;
		}
	}
	यदि (मार्गः_walk) {
		strvec_प्रक्षिप(&rp, "--objects");
	} अन्यथा यदि (thin) {
		use_आन्तरिकम्_rev_list = 1;
		strvec_प्रक्षिप(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} अन्यथा
		strvec_प्रक्षिप(&rp, "--objects");

	यदि (rev_list_all) {
		use_आन्तरिकम्_rev_list = 1;
		strvec_प्रक्षिप(&rp, "--all");
	}
	यदि (rev_list_reflog) {
		use_आन्तरिकम्_rev_list = 1;
		strvec_प्रक्षिप(&rp, "--reflog");
	}
	यदि (rev_list_सूचकांक) {
		use_आन्तरिकम्_rev_list = 1;
		strvec_प्रक्षिप(&rp, "--indexed-objects");
	}
	यदि (rev_list_unpacked && !stdin_packs) {
		use_आन्तरिकम्_rev_list = 1;
		strvec_प्रक्षिप(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	यदि (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		यदि (!stdin_packs) {
			use_आन्तरिकम्_rev_list = 1;
			strvec_प्रक्षिप(&rp, "--exclude-promisor-objects");
		}
	} अन्यथा यदि (exclude_promisor_objects_best_effort) {
		use_आन्तरिकम्_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	यदि (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_आन्तरिकम्_rev_list = 1;

	यदि (!reuse_object)
		reuse_delta = 0;
	यदि (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	अन्यथा यदि (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	यदि (!delta_अन्वेषणम्_तन्तवः)	/* --threads=0 means autodetect */
		delta_अन्वेषणम्_तन्तवः = online_cpus();

	यदि (!HAVE_तन्तवः && delta_अन्वेषणम्_तन्तवः != 1)
		warning(_("no threads support, ignoring --threads"));
	यदि (!pack_गन्तव्यम्_stdout && !pack_आकार_सीमा)
		pack_आकार_सीमा = pack_आकार_सीमा_cfg;
	यदि (pack_गन्तव्यम्_stdout && pack_आकार_सीमा)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	यदि (pack_आकार_सीमा && pack_आकार_सीमा < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_आकार_सीमा = 1024*1024;
	}

	यदि (!pack_गन्तव्यम्_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	यदि (!rev_list_all || !rev_list_reflog || !rev_list_सूचकांक)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	यदि (stdin_packs && use_आन्तरिकम्_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	यदि (cruft) {
		यदि (use_आन्तरिकम्_rev_list)
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
	यदि (!pack_गन्तव्यम्_stdout)
		use_bitmap_सूचकांक_default = 0;

	यदि (use_bitmap_सूचकांक < 0)
		use_bitmap_सूचकांक = use_bitmap_सूचकांक_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	यदि (!use_आन्तरिकम्_rev_list || (!pack_गन्तव्यम्_stdout && write_bitmap_सूचकांक) || is_repository_shallow(the_repository))
		use_bitmap_सूचकांक = 0;

	यदि (pack_गन्तव्यम्_stdout || !rev_list_all)
		write_bitmap_सूचकांक = 0;

	यदि (name_hash_संस्करणम् < 0)
		name_hash_संस्करणम् = (पूर्णाङ्क)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_संस्करणम्();

	यदि (use_delta_islands)
		strvec_प्रक्षिप(&rp, "--topo-order");

	यदि (प्रगतिः && all_प्रगतिः_implied)
		प्रगतिः = 2;

	सङ्कलय_extra_kept_packs(&keep_pack_list);
	यदि (ignore_packed_keep_on_disk) {
		संरचना packed_git *p;

		repo_for_each_pack(the_repository, p)
			यदि (p->pack_local && p->pack_keep)
				भङ्ग;
		यदि (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	यदि (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		संरचना packed_git *p;

		repo_for_each_pack(the_repository, p) {
			यदि (!p->pack_local) {
				have_non_local_packs = 1;
				भङ्ग;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_दत्त(the_repository, &गन्तव्यम्_pack);

	यदि (प्रगतिः && !cruft)
		प्रगतिः_state = start_प्रगतिः(the_repository,
						_("Enumerating objects"), 0);
	यदि (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} अन्यथा यदि (cruft) {
		read_cruft_objects();
	} अन्यथा यदि (!use_आन्तरिकम्_rev_list) {
		read_object_list_from_stdin();
	} अन्यथा {
		संरचना rev_info revs;

		repo_init_revisions(the_repository, &revs, रिक्त);
		list_objects_filter_copy(&revs.filter, &filter_options);
		यदि (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_अन्तर्_promisor_pack;
			revs.include_check_obj = is_not_अन्तर्_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_आधारः();
	यदि (include_चिह्नकम् && nr_परिणाम)
		refs_for_each_चिह्नकम्_ref(get_main_ref_store(the_repository),
				      सङ्कलय_ref_चिह्नकम्, रिक्त);
	stop_प्रगतिः(&प्रगतिः_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	यदि (non_रिक्तानि && !nr_परिणाम)
		गच्छ cleanup;
	यदि (nr_परिणाम) {
		trace2_region_enter("pack-objects", "prepare-pack",
				    the_repository);
		prepare_pack(गवाक्षः, गभीरता);
		trace2_region_leave("pack-objects", "prepare-pack",
				    the_repository);
	}

	trace2_region_enter("pack-objects", "write-pack-file", the_repository);
	write_excluded_by_configs();
	write_pack_file();
	trace2_region_leave("pack-objects", "write-pack-file", the_repository);

	यदि (प्रगतिः)
		fprintf_ln(stderr,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_प्रयुक्तम्_nr);

	trace2_दत्त_intmax("pack-objects", the_repository, "written", written);
	trace2_दत्त_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_दत्त_intmax("pack-objects", the_repository, "reused", reused);
	trace2_दत्त_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_दत्त_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_दत्त_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_प्रयुक्तम्_nr);

cleanup:
	clear_packing_दत्त(&गन्तव्यम्_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	प्रत्यागम 0;
}
