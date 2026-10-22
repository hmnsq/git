#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "config.h"
#समावेश "attr.h"
#समावेश "object.h"
#समावेश "commit.h"
#समावेश "tag.h"
#समावेश "delta.h"
#समावेश "pack.h"
#समावेश "pack-revindex.h"
#समावेश "csum-file.h"
#समावेश "tree-walk.h"
#समावेश "diff.h"
#समावेश "revision.h"
#समावेश "list-objects.h"
#समावेश "list-objects-filter-options.h"
#समावेश "pack-objects.h"
#समावेश "progress.h"
#समावेश "refs.h"
#समावेश "thread-utils.h"
#समावेश "pack-bitmap.h"
#समावेश "delta-islands.h"
#समावेश "reachable.h"
#समावेश "oid-array.h"
#समावेश "strvec.h"
#समावेश "strmap.h"
#समावेश "list.h"
#समावेश "packfile.h"
#समावेश "object-file.h"
#समावेश "object-file-convert.h"
#समावेश "odb.h"
#समावेश "odb/streaming.h"
#समावेश "replace-object.h"
#समावेश "dir.h"
#समावेश "midx.h"
#समावेश "trace2.h"
#समावेश "shallow.h"
#समावेश "promisor-remote.h"
#समावेश "pack-mtimes.h"
#समावेश "parse-options.h"
#समावेश "pkt-line.h"
#समावेश "blob.h"
#समावेश "tree.h"
#समावेश "path-walk.h"

/*
 * Objects we are going to pack are collected in the `to_pack` structure.
 * It contains an array (dynamically expanded) of the object data, and a map
 * that can resolve SHA1s to their position in the array.
 */
अटल काष्ठा packing_तथ्य to_pack;

अटल अंतरभूत काष्ठा object_entry *oe_delta(
		स्थिर काष्ठा packing_तथ्य *pack,
		स्थिर काष्ठा object_entry *e)
{
	अगर (!e->delta_idx)
		वापस शून्य;
	अगर (e->ext_base)
		वापस &pack->ext_bases[e->delta_idx - 1];
	अन्यथा
		वापस &pack->objects[e->delta_idx - 1];
}

अटल अंतरभूत माप_प्रकार oe_delta_आकार(काष्ठा packing_तथ्य *pack,
				   स्थिर काष्ठा object_entry *e)
{
	अगर (e->delta_आकार_valid)
		वापस e->delta_आकार;

	/*
	 * pack->delta_size[] can't be NULL because oe_set_delta_size()
	 * must have been called when a new delta is saved with
	 * oe_set_delta().
	 * If oe_delta() returns NULL (i.e. default state, which means
	 * delta_size_valid is also false), then the caller must never
	 * call oe_delta_size().
	 */
	वापस pack->delta_आकार_2[e - pack->objects];
}

माप_प्रकार oe_get_आकार_slow(काष्ठा packing_तथ्य *pack,
			स्थिर काष्ठा object_entry *e);

अटल अंतरभूत माप_प्रकार oe_आकार(काष्ठा packing_तथ्य *pack,
			     स्थिर काष्ठा object_entry *e)
{
	अगर (e->आकार_valid)
		वापस e->आकार_2;

	वापस oe_get_आकार_slow(pack, e);
}

अटल अंतरभूत व्योम oe_set_delta(काष्ठा packing_तथ्य *pack,
				काष्ठा object_entry *e,
				काष्ठा object_entry *delta)
{
	अगर (delta)
		e->delta_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_idx = 0;
}

अटल अंतरभूत काष्ठा object_entry *oe_delta_sibling(
		स्थिर काष्ठा packing_तथ्य *pack,
		स्थिर काष्ठा object_entry *e)
{
	अगर (e->delta_sibling_idx)
		वापस &pack->objects[e->delta_sibling_idx - 1];
	वापस शून्य;
}

अटल अंतरभूत काष्ठा object_entry *oe_delta_child(
		स्थिर काष्ठा packing_तथ्य *pack,
		स्थिर काष्ठा object_entry *e)
{
	अगर (e->delta_child_idx)
		वापस &pack->objects[e->delta_child_idx - 1];
	वापस शून्य;
}

अटल अंतरभूत व्योम oe_set_delta_child(काष्ठा packing_तथ्य *pack,
				      काष्ठा object_entry *e,
				      काष्ठा object_entry *delta)
{
	अगर (delta)
		e->delta_child_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_child_idx = 0;
}

अटल अंतरभूत व्योम oe_set_delta_sibling(काष्ठा packing_तथ्य *pack,
					काष्ठा object_entry *e,
					काष्ठा object_entry *delta)
{
	अगर (delta)
		e->delta_sibling_idx = (delta - pack->objects) + 1;
	अन्यथा
		e->delta_sibling_idx = 0;
}

अटल अंतरभूत व्योम oe_set_आकार(काष्ठा packing_तथ्य *pack,
			       काष्ठा object_entry *e,
			       माप_प्रकार आकार)
{
	अगर (आकार < pack->oe_आकार_limit) {
		e->आकार_2 = आकार;
		e->आकार_valid = 1;
	} अन्यथा {
		e->आकार_valid = 0;
		अगर (oe_get_आकार_slow(pack, e) != आकार)
			BUG("'size' is supposed to be the object size!");
	}
}

अटल अंतरभूत व्योम oe_set_delta_आकार(काष्ठा packing_तथ्य *pack,
				     काष्ठा object_entry *e,
				     माप_प्रकार आकार)
{
	अगर (आकार < pack->oe_delta_आकार_limit) {
		e->delta_आकार = आकार;
		e->delta_आकार_valid = 1;
	} अन्यथा {
		packing_तथ्य_lock(pack);
		अगर (!pack->delta_आकार_2)
			ALLOC_ARRAY(pack->delta_आकार_2, pack->nr_alloc);
		packing_तथ्य_unlock(pack);

		pack->delta_आकार_2[e - pack->objects] = आकार;
		e->delta_आकार_valid = 0;
	}
}

#घोषणा IN_PACK(obj) oe_in_pack(&to_pack, obj)
#घोषणा आकार_3(obj) oe_आकार(&to_pack, obj)
#घोषणा SET_आकार(obj,आकार) oe_set_आकार(&to_pack, obj, आकार)
#घोषणा DELTA_आकार(obj) oe_delta_आकार(&to_pack, obj)
#घोषणा DELTA(obj) oe_delta(&to_pack, obj)
#घोषणा DELTA_CHILD(obj) oe_delta_child(&to_pack, obj)
#घोषणा DELTA_SIBLING(obj) oe_delta_sibling(&to_pack, obj)
#घोषणा SET_DELTA(obj, val) oe_set_delta(&to_pack, obj, val)
#घोषणा SET_DELTA_EXT(obj, oid) oe_set_delta_ext(&to_pack, obj, oid)
#घोषणा SET_DELTA_आकार(obj, val) oe_set_delta_आकार(&to_pack, obj, val)
#घोषणा SET_DELTA_CHILD(obj, val) oe_set_delta_child(&to_pack, obj, val)
#घोषणा SET_DELTA_SIBLING(obj, val) oe_set_delta_sibling(&to_pack, obj, val)

अटल स्थिर अक्षर *स्थिर pack_usage[] = {
	N_("git pack-objects [-q | --progress | --all-progress] [--all-progress-implied]\n"
	   "                 [--no-reuse-delta] [--delta-base-offset] [--non-empty]\n"
	   "                 [--local] [--incremental] [--window=<n>] [--depth=<n>]\n"
	   "                 [--revs [--unpacked | --all]] [--keep-pack=<pack-name>]\n"
	   "                 [--cruft] [--cruft-expiration=<time>]\n"
	   "                 [--stdout [--filter=<filter-spec>] | <base-name>]\n"
	   "                 [--shallow] [--keep-true-parents] [--[no-]sparse]\n"
	   "                 [--name-hash-version=<n>] [--path-walk] < <object-list>"),
	शून्य
};

अटल काष्ठा pack_idx_entry **written_list;
अटल uint32_t nr_परिणाम, nr_written, nr_seen;
अटल काष्ठा bitmap_सूचकांक *bitmap_git;
अटल uint32_t write_layer;

अटल पूर्णांक non_empty;
अटल पूर्णांक reuse_delta = 1, reuse_object = 1;
अटल पूर्णांक keep_unreachable, unpack_unreachable, include_tag;
अटल timestamp_t unpack_unreachable_expiration;
अटल पूर्णांक pack_loose_unreachable;
अटल पूर्णांक cruft;
अटल पूर्णांक shallow = 0;
अटल timestamp_t cruft_expiration;
अटल पूर्णांक local;
अटल पूर्णांक have_non_local_packs;
अटल पूर्णांक incremental;
अटल पूर्णांक ignore_packed_keep_on_disk;
अटल पूर्णांक ignore_packed_keep_in_core;
अटल पूर्णांक ignore_packed_keep_in_core_open;
अटल पूर्णांक ignore_packed_keep_in_core_has_cruft;
अटल पूर्णांक allow_ofs_delta;
अटल काष्ठा pack_idx_option pack_idx_opts;
अटल स्थिर अक्षर *base_name;
अटल पूर्णांक progress = 1;
अटल पूर्णांक window = 10;
अटल अचिन्हित दीर्घ pack_आकार_limit;
अटल पूर्णांक depth = 50;
अटल पूर्णांक delta_खोज_threads;
अटल पूर्णांक pack_to_stdout;
अटल पूर्णांक sparse;
अटल पूर्णांक thin;
अटल पूर्णांक path_walk = -1;
अटल पूर्णांक अंक_preferred_base;
अटल काष्ठा progress *progress_state;

अटल काष्ठा bitmapped_pack *reuse_packfiles;
अटल माप_प्रकार reuse_packfiles_nr;
अटल माप_प्रकार reuse_packfiles_used_nr;
अटल uint32_t reuse_packfile_objects;
अटल काष्ठा bitmap *reuse_packfile_bitmap;

अटल पूर्णांक use_bitmap_सूचकांक_default = 1;
अटल पूर्णांक use_bitmap_सूचकांक = -1;
अटल क्रमागत {
	NO_PACK_REUSE = 0,
	SINGLE_PACK_REUSE,
	MULTI_PACK_REUSE,
} allow_pack_reuse = SINGLE_PACK_REUSE;
अटल क्रमागत {
	WRITE_BITMAP_FALSE = 0,
	WRITE_BITMAP_QUIET,
	WRITE_BITMAP_TRUE,
} write_bitmap_सूचकांक;
अटल uint16_t write_bitmap_options = BITMAP_OPT_HASH_CACHE;

अटल पूर्णांक exclude_promisor_objects;
अटल पूर्णांक exclude_promisor_objects_best_effort;

अटल पूर्णांक use_delta_islands;

अटल माप_प्रकार delta_cache_आकार = 0;
अटल माप_प्रकार max_delta_cache_आकार = DEFAULT_DELTA_CACHE_आकार;
अटल अचिन्हित दीर्घ cache_max_small_delta_आकार = 1000;

अटल अचिन्हित दीर्घ window_memory_limit = 0;

अटल काष्ठा string_list uri_protocols = STRING_LIST_INIT_NODUP;

क्रमागत missing_action {
	MA_ERROR = 0,      /* fail if any missing objects are encountered */
	MA_ALLOW_ANY,      /* silently allow ALL missing objects */
	MA_ALLOW_PROMISOR, /* silently allow all missing PROMISOR objects */
};
अटल क्रमागत missing_action arg_missing_action;
अटल show_object_fn fn_show_object;

काष्ठा configured_exclusion {
	काष्ठा oidmap_entry e;
	अक्षर *pack_hash_hex;
	अक्षर *uri;
};
अटल काष्ठा oidmap configured_exclusions;

अटल काष्ठा oidset excluded_by_config;
अटल पूर्णांक name_hash_version = -1;

क्रमागत stdin_packs_mode {
	STDIN_PACKS_MODE_NONE,
	STDIN_PACKS_MODE_STANDARD,
	STDIN_PACKS_MODE_FOLLOW,
};

/**
 * Check whether the name_hash_version chosen by user input is appropriate,
 * and also validate whether it is compatible with other features.
 */
अटल व्योम validate_name_hash_version(व्योम)
{
	अगर (name_hash_version < 1 || name_hash_version > 2)
		die(_("invalid --name-hash-version option: %d"), name_hash_version);
	अगर (write_bitmap_सूचकांक && name_hash_version != 1) {
		warning(_("currently, --write-bitmap-index requires --name-hash-version=1"));
		name_hash_version = 1;
	}
}

अटल अंतरभूत uint32_t pack_name_hash_fn(स्थिर अक्षर *name)
{
	अटल पूर्णांक seen_version = -1;

	अगर (seen_version < 0)
		seen_version = name_hash_version;
	अन्यथा अगर (seen_version != name_hash_version)
		BUG("name hash version changed from %d to %d mid-process",
		    seen_version, name_hash_version);

	चयन (name_hash_version) {
	हाल 1:
		वापस pack_name_hash(name);

	हाल 2:
		वापस pack_name_hash_v2((स्थिर अचिन्हित अक्षर *)name);

	शेष:
		BUG("invalid name-hash version: %d", name_hash_version);
	}
}

/*
 * stats
 */
अटल uint32_t written, written_delta;
अटल uint32_t reused, reused_delta;

/*
 * Indexed commits
 */
अटल काष्ठा commit **indexed_commits;
अटल अचिन्हित पूर्णांक indexed_commits_nr;
अटल अचिन्हित पूर्णांक indexed_commits_alloc;

अटल व्योम सूचकांक_commit_for_bitmap(काष्ठा commit *commit)
{
	अगर (indexed_commits_nr >= indexed_commits_alloc) {
		indexed_commits_alloc = (indexed_commits_alloc + 32) * 2;
		REALLOC_ARRAY(indexed_commits, indexed_commits_alloc);
	}

	indexed_commits[indexed_commits_nr++] = commit;
}

अटल व्योम *get_delta(काष्ठा object_entry *entry)
{
	अचिन्हित दीर्घ आकार, base_आकार;
	माप_प्रकार delta_आकार_2;
	व्योम *buf, *base_buf, *delta_buf;
	क्रमागत object_type type;
	माप_प्रकार आकार_st = 0, base_आकार_st = 0;

	buf = odb_read_object(the_repository->objects, &entry->idx.oid,
			      &type, &आकार_st);
	आकार = cast_आकार_t_to_ulong(आकार_st);
	अगर (!buf)
		die(_("unable to read %s"), oid_to_hex(&entry->idx.oid));
	base_buf = odb_read_object(the_repository->objects,
				   &DELTA(entry)->idx.oid, &type,
				   &base_आकार_st);
	base_आकार = cast_आकार_t_to_ulong(base_आकार_st);
	अगर (!base_buf)
		die("unable to read %s",
		    oid_to_hex(&DELTA(entry)->idx.oid));
	delta_buf = diff_delta(base_buf, base_आकार,
			       buf, आकार, &delta_आकार_2, 0);
	/*
	 * We successfully computed this delta once but dropped it for
	 * memory reasons. Something is very wrong if this time we
	 * recompute and create a different delta.
	 */
	अगर (!delta_buf || delta_आकार_2 != DELTA_आकार(entry))
		BUG("delta size changed");
	मुक्त(buf);
	मुक्त(base_buf);
	वापस delta_buf;
}

अटल अचिन्हित दीर्घ do_compress(व्योम **pptr, अचिन्हित दीर्घ आकार)
{
	git_zstream stream;
	व्योम *in, *out;
	अचिन्हित दीर्घ maxsize;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);
	maxsize = git_deflate_bound(&stream, आकार);

	in = *pptr;
	out = xmalloc(maxsize);
	*pptr = out;

	stream.अग्र_in = in;
	stream.avail_in = आकार;
	stream.अग्र_out = out;
	stream.avail_out = maxsize;
	जबतक (git_deflate(&stream, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_end(&stream);

	मुक्त(in);
	वापस stream.योग_out;
}

अटल अचिन्हित दीर्घ write_large_blob_तथ्य(काष्ठा odb_stream *st, काष्ठा hashfile *f,
					   स्थिर काष्ठा object_id *oid)
{
	git_zstream stream;
	अचिन्हित अक्षर ibuf[1024 * 16];
	अचिन्हित अक्षर obuf[1024 * 16];
	अचिन्हित दीर्घ olen = 0;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	git_deflate_init(&stream, cfg->pack_compression_level);

	क्रम (;;) {
		ssize_t readlen;
		पूर्णांक zret = Z_OK;
		readlen = odb_stream_read(st, ibuf, माप(ibuf));
		अगर (readlen == -1)
			die(_("unable to read %s"), oid_to_hex(oid));

		stream.अग्र_in = ibuf;
		stream.avail_in = readlen;
		जबतक ((stream.avail_in || readlen == 0) &&
		       (zret == Z_OK || zret == Z_BUF_ERROR)) {
			stream.अग्र_out = obuf;
			stream.avail_out = माप(obuf);
			zret = git_deflate(&stream, readlen ? 0 : Z_FINISH);
			hashwrite(f, obuf, stream.अग्र_out - obuf);
			olen += stream.अग्र_out - obuf;
		}
		अगर (stream.avail_in)
			die(_("deflate error (%d)"), zret);
		अगर (readlen == 0) {
			अगर (zret != Z_STREAM_END)
				die(_("deflate error (%d)"), zret);
			अवरोध;
		}
	}
	git_deflate_end(&stream);
	वापस olen;
}

/*
 * we are going to reuse the existing object data as is.  make
 * sure it is not corrupt.
 */
अटल पूर्णांक check_pack_inflate(काष्ठा packed_git *p,
		काष्ठा pack_window **w_curs,
		off_t offset,
		off_t len,
		माप_प्रकार expect)
{
	git_zstream stream;
	अचिन्हित अक्षर fakebuf[4096], *in;
	पूर्णांक st;

	स_रखो(&stream, 0, माप(stream));
	git_inflate_init(&stream);
	करो {
		in = use_pack(p, w_curs, offset, &stream.avail_in);
		stream.अग्र_in = in;
		stream.अग्र_out = fakebuf;
		stream.avail_out = माप(fakebuf);
		st = git_inflate(&stream, Z_FINISH);
		offset += stream.अग्र_in - in;
	} जबतक (st == Z_OK || st == Z_BUF_ERROR);
	git_inflate_end(&stream);
	वापस (st == Z_STREAM_END &&
		stream.योग_out == expect &&
		stream.योग_in == len) ? 0 : -1;
}

अटल व्योम copy_pack_तथ्य(काष्ठा hashfile *f,
		काष्ठा packed_git *p,
		काष्ठा pack_window **w_curs,
		off_t offset,
		off_t len)
{
	अचिन्हित अक्षर *in;
	माप_प्रकार avail;

	जबतक (len) {
		in = use_pack(p, w_curs, offset, &avail);
		अगर (avail > len)
			avail = (अचिन्हित दीर्घ)len;
		hashwrite(f, in, avail);
		offset += avail;
		len -= avail;
	}
}

अटल अंतरभूत पूर्णांक oe_आकार_greater_than(काष्ठा packing_तथ्य *pack,
				       स्थिर काष्ठा object_entry *lhs,
				       माप_प्रकार rhs)
{
	अगर (lhs->आकार_valid)
		वापस lhs->आकार_2 > rhs;
	अगर (rhs < pack->oe_आकार_limit) /* rhs < 2^x <= lhs ? */
		वापस 1;
	वापस oe_get_आकार_slow(pack, lhs) > rhs;
}

/* Return 0 if we will bust the pack-size limit */
अटल अचिन्हित दीर्घ write_no_reuse_object(काष्ठा hashfile *f, काष्ठा object_entry *entry,
					   अचिन्हित दीर्घ limit, पूर्णांक usable_delta)
{
	अचिन्हित दीर्घ आकार, datalen;
	अचिन्हित अक्षर header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	अचिन्हित hdrlen;
	क्रमागत object_type type;
	व्योम *buf;
	काष्ठा odb_stream *st = शून्य;
	स्थिर अचिन्हित hashsz = the_hash_algo->rawsz;

	अगर (!usable_delta) {
		अगर (oe_type(entry) == OBJ_BLOB &&
		    oe_आकार_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)) &&
		    (st = odb_stream_from_object(the_repository->objects, &entry->idx.oid,
						 शून्य)) != शून्य) {
			buf = शून्य;
			type = st->type;
			आकार = st->आकार;
		} अन्यथा {
			माप_प्रकार आकार_st = 0;
			buf = odb_read_object(the_repository->objects,
					      &entry->idx.oid, &type,
					      &आकार_st);
			आकार = cast_आकार_t_to_ulong(आकार_st);
			अगर (!buf)
				die(_("unable to read %s"),
				    oid_to_hex(&entry->idx.oid));
		}
		/*
		 * make sure no cached delta data remains from a
		 * previous attempt before a pack split occurred.
		 */
		FREE_AND_NULL(entry->delta_तथ्य);
		entry->z_delta_आकार = 0;
	} अन्यथा अगर (entry->delta_तथ्य) {
		आकार = DELTA_आकार(entry);
		buf = entry->delta_तथ्य;
		entry->delta_तथ्य = शून्य;
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	} अन्यथा {
		buf = get_delta(entry);
		आकार = DELTA_आकार(entry);
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	}

	अगर (st)	/* large blob case, just assume we don't compress well */
		datalen = आकार;
	अन्यथा अगर (entry->z_delta_आकार)
		datalen = entry->z_delta_आकार;
	अन्यथा
		datalen = do_compress(&buf, आकार);

	/*
	 * The object header is a byte of 'type' followed by zero or
	 * more bytes of length.
	 */
	hdrlen = encode_in_pack_object_header(header, माप(header),
					      type, आकार);

	अगर (type == OBJ_OFS_DELTA) {
		/*
		 * Deltas with relative base contain an additional
		 * encoding of the relative offset for the delta
		 * base from this object's position in the pack.
		 */
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		अचिन्हित pos = माप(dheader) - 1;
		dheader[pos] = ofs & 127;
		जबतक (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		अगर (limit && hdrlen + माप(dheader) - pos + datalen + hashsz >= limit) {
			अगर (st)
				odb_stream_close(st);
			मुक्त(buf);
			वापस 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, माप(dheader) - pos);
		hdrlen += माप(dheader) - pos;
	} अन्यथा अगर (type == OBJ_REF_DELTA) {
		/*
		 * Deltas with a base reference contain
		 * additional bytes for the base object ID.
		 */
		अगर (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			अगर (st)
				odb_stream_close(st);
			मुक्त(buf);
			वापस 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
	} अन्यथा {
		अगर (limit && hdrlen + datalen + hashsz >= limit) {
			अगर (st)
				odb_stream_close(st);
			मुक्त(buf);
			वापस 0;
		}
		hashwrite(f, header, hdrlen);
	}
	अगर (st) {
		datalen = write_large_blob_तथ्य(st, f, &entry->idx.oid);
		odb_stream_close(st);
	} अन्यथा {
		hashwrite(f, buf, datalen);
		मुक्त(buf);
	}

	वापस hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
अटल off_t write_reuse_object(काष्ठा hashfile *f, काष्ठा object_entry *entry,
				अचिन्हित दीर्घ limit, पूर्णांक usable_delta)
{
	काष्ठा packed_git *p = IN_PACK(entry);
	काष्ठा pack_window *w_curs = शून्य;
	uint32_t pos;
	off_t offset, cur;
	क्रमागत object_type type = oe_type(entry);
	क्रमागत object_type in_pack_type;
	off_t datalen;
	अचिन्हित अक्षर header[MAX_PACK_OBJECT_HEADER],
		      dheader[MAX_PACK_OBJECT_HEADER];
	अचिन्हित hdrlen;
	स्थिर अचिन्हित hashsz = the_hash_algo->rawsz;
	माप_प्रकार entry_आकार;

	cur = entry->in_pack_offset;
	in_pack_type = unpack_object_header(p, &w_curs, &cur, &entry_आकार);
	अगर (in_pack_type < 0)
		die(_("write_reuse_object: unable to parse object header of %s"),
		    oid_to_hex(&entry->idx.oid));

	अगर (DELTA(entry))
		type = (allow_ofs_delta && DELTA(entry)->idx.offset) ?
			OBJ_OFS_DELTA : OBJ_REF_DELTA;
	hdrlen = encode_in_pack_object_header(header, माप(header),
					      type, entry_आकार);

	offset = entry->in_pack_offset;
	अगर (offset_to_pack_pos(p, offset, &pos) < 0)
		die(_("write_reuse_object: could not locate %s, expected at "
		      "offset %"PRIuMAX" in pack %s"),
		    oid_to_hex(&entry->idx.oid), (uintmax_t)offset,
		    p->pack_name);
	datalen = pack_pos_to_offset(p, pos + 1) - offset;
	अगर (!pack_to_stdout && p->सूचकांक_version > 1 &&
	    check_pack_crc(p, &w_curs, offset, datalen,
			   pack_pos_to_सूचकांक(p, pos))) {
		error(_("bad packed object CRC for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		वापस write_no_reuse_object(f, entry, limit, usable_delta);
	}

	offset += entry->in_pack_header_आकार;
	datalen -= entry->in_pack_header_आकार;

	अगर (!pack_to_stdout && p->सूचकांक_version == 1 &&
	    check_pack_inflate(p, &w_curs, offset, datalen, entry_आकार)) {
		error(_("corrupt packed object for %s"),
		      oid_to_hex(&entry->idx.oid));
		unuse_pack(&w_curs);
		वापस write_no_reuse_object(f, entry, limit, usable_delta);
	}

	अगर (type == OBJ_OFS_DELTA) {
		off_t ofs = entry->idx.offset - DELTA(entry)->idx.offset;
		अचिन्हित pos = माप(dheader) - 1;
		dheader[pos] = ofs & 127;
		जबतक (ofs >>= 7)
			dheader[--pos] = 128 | (--ofs & 127);
		अगर (limit && hdrlen + माप(dheader) - pos + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			वापस 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, dheader + pos, माप(dheader) - pos);
		hdrlen += माप(dheader) - pos;
		reused_delta++;
	} अन्यथा अगर (type == OBJ_REF_DELTA) {
		अगर (limit && hdrlen + hashsz + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			वापस 0;
		}
		hashwrite(f, header, hdrlen);
		hashwrite(f, DELTA(entry)->idx.oid.hash, hashsz);
		hdrlen += hashsz;
		reused_delta++;
	} अन्यथा {
		अगर (limit && hdrlen + datalen + hashsz >= limit) {
			unuse_pack(&w_curs);
			वापस 0;
		}
		hashwrite(f, header, hdrlen);
	}
	copy_pack_तथ्य(f, p, &w_curs, offset, datalen);
	unuse_pack(&w_curs);
	reused++;
	वापस hdrlen + datalen;
}

/* Return 0 if we will bust the pack-size limit */
अटल off_t write_object(काष्ठा hashfile *f,
			  काष्ठा object_entry *entry,
			  off_t write_offset)
{
	अचिन्हित दीर्घ limit;
	off_t len;
	पूर्णांक usable_delta, to_reuse;

	अगर (!pack_to_stdout)
		crc32_begin(f);

	/* apply size limit if limited packsize and not first object */
	अगर (!pack_आकार_limit || !nr_written)
		limit = 0;
	अन्यथा अगर (pack_आकार_limit <= write_offset)
		/*
		 * the earlier object did not fit the limit; avoid
		 * mistaking this with unlimited (i.e. limit = 0).
		 */
		limit = 1;
	अन्यथा
		limit = pack_आकार_limit - write_offset;

	अगर (!DELTA(entry))
		usable_delta = 0;	/* no delta */
	अन्यथा अगर (!pack_आकार_limit)
	       usable_delta = 1;	/* unlimited packfile */
	अन्यथा अगर (DELTA(entry)->idx.offset == (off_t)-1)
		usable_delta = 0;	/* base was written to another pack */
	अन्यथा अगर (DELTA(entry)->idx.offset)
		usable_delta = 1;	/* base already exists in this pack */
	अन्यथा
		usable_delta = 0;	/* base could end up in another pack */

	अगर (!reuse_object)
		to_reuse = 0;	/* explicit */
	अन्यथा अगर (!IN_PACK(entry))
		to_reuse = 0;	/* can't reuse what we don't have */
	अन्यथा अगर (oe_type(entry) == OBJ_REF_DELTA ||
		 oe_type(entry) == OBJ_OFS_DELTA)
				/* check_object() decided it for us ... */
		to_reuse = usable_delta;
				/* ... but pack split may override that */
	अन्यथा अगर (oe_type(entry) != entry->in_pack_type)
		to_reuse = 0;	/* pack has delta which is unusable */
	अन्यथा अगर (DELTA(entry))
		to_reuse = 0;	/* we want to pack afresh */
	अन्यथा
		to_reuse = 1;	/* we have it in-pack undeltified,
				 * and we do not need to deltify it.
				 */

	अगर (!to_reuse)
		len = write_no_reuse_object(f, entry, limit, usable_delta);
	अन्यथा
		len = write_reuse_object(f, entry, limit, usable_delta);
	अगर (!len)
		वापस 0;

	अगर (usable_delta)
		written_delta++;
	written++;
	अगर (!pack_to_stdout)
		entry->idx.crc32 = crc32_end(f);
	वापस len;
}

क्रमागत write_one_status {
	WRITE_ONE_SKIP = -1, /* already written */
	WRITE_ONE_BREAK = 0, /* writing this will bust the limit; not written */
	WRITE_ONE_WRITTEN = 1, /* normal */
	WRITE_ONE_RECURSIVE = 2 /* already scheduled to be written */
};

अटल क्रमागत write_one_status write_one(काष्ठा hashfile *f,
				       काष्ठा object_entry *e,
				       off_t *offset)
{
	off_t आकार;
	पूर्णांक recursing;

	/*
	 * we set offset to 1 (which is an impossible value) to mark
	 * the fact that this object is involved in "write its base
	 * first before writing a deltified object" recursion.
	 */
	recursing = (e->idx.offset == 1);
	अगर (recursing) {
		warning(_("recursive delta detected for object %s"),
			oid_to_hex(&e->idx.oid));
		वापस WRITE_ONE_RECURSIVE;
	} अन्यथा अगर (e->idx.offset || e->preferred_base) {
		/* offset is non zero if object is written already. */
		वापस WRITE_ONE_SKIP;
	}

	/* if we are deltified, write out base object first. */
	अगर (DELTA(e)) {
		e->idx.offset = 1; /* now recurse */
		चयन (write_one(f, DELTA(e), offset)) {
		हाल WRITE_ONE_RECURSIVE:
			/* we cannot depend on this one */
			SET_DELTA(e, शून्य);
			अवरोध;
		शेष:
			अवरोध;
		हाल WRITE_ONE_BREAK:
			e->idx.offset = recursing;
			वापस WRITE_ONE_BREAK;
		}
	}

	e->idx.offset = *offset;
	आकार = write_object(f, e, *offset);
	अगर (!आकार) {
		e->idx.offset = recursing;
		वापस WRITE_ONE_BREAK;
	}
	written_list[nr_written++] = &e->idx;

	/* make sure off_t is sufficiently large not to wrap */
	अगर (signed_add_overflows(*offset, आकार))
		die(_("pack too large for current definition of off_t"));
	*offset += आकार;
	वापस WRITE_ONE_WRITTEN;
}

अटल पूर्णांक mark_tagged(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य UNUSED)
{
	काष्ठा object_id peeled;
	काष्ठा object_entry *entry = packlist_find(&to_pack, ref->oid);

	अगर (entry)
		entry->tagged = 1;
	अगर (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		entry = packlist_find(&to_pack, &peeled);
		अगर (entry)
			entry->tagged = 1;
	}
	वापस 0;
}

अटल अंतरभूत अचिन्हित अक्षर oe_layer(काष्ठा packing_तथ्य *pack,
				     काष्ठा object_entry *e)
{
	अगर (!pack->layer)
		वापस 0;
	वापस pack->layer[e - pack->objects];
}

अटल अंतरभूत व्योम add_to_write_order(काष्ठा object_entry **wo,
			       अचिन्हित पूर्णांक *endp,
			       काष्ठा object_entry *e)
{
	अगर (e->filled || oe_layer(&to_pack, e) != write_layer)
		वापस;
	wo[(*endp)++] = e;
	e->filled = 1;
}

अटल व्योम add_descendants_to_write_order(काष्ठा object_entry **wo,
					   अचिन्हित पूर्णांक *endp,
					   काष्ठा object_entry *e)
{
	पूर्णांक add_to_order = 1;
	जबतक (e) {
		अगर (add_to_order) {
			काष्ठा object_entry *s;
			/* add this node... */
			add_to_write_order(wo, endp, e);
			/* all its siblings... */
			क्रम (s = DELTA_SIBLING(e); s; s = DELTA_SIBLING(s)) {
				add_to_write_order(wo, endp, s);
			}
		}
		/* drop down a level to add left subtree nodes if possible */
		अगर (DELTA_CHILD(e)) {
			add_to_order = 1;
			e = DELTA_CHILD(e);
		} अन्यथा {
			add_to_order = 0;
			/* our sibling might have some children, it is next */
			अगर (DELTA_SIBLING(e)) {
				e = DELTA_SIBLING(e);
				जारी;
			}
			/* go back to our parent node */
			e = DELTA(e);
			जबतक (e && !DELTA_SIBLING(e)) {
				/* we're on the right side of a subtree, keep
				 * going up until we can go right again */
				e = DELTA(e);
			}
			अगर (!e) {
				/* done- we hit our original root node */
				वापस;
			}
			/* pass it off to sibling at this level */
			e = DELTA_SIBLING(e);
		}
	};
}

अटल व्योम add_family_to_write_order(काष्ठा object_entry **wo,
				      अचिन्हित पूर्णांक *endp,
				      काष्ठा object_entry *e)
{
	काष्ठा object_entry *मूल;

	क्रम (मूल = e; DELTA(मूल); मूल = DELTA(मूल))
		; /* nothing */
	add_descendants_to_write_order(wo, endp, मूल);
}

अटल व्योम compute_layer_order(काष्ठा object_entry **wo, अचिन्हित पूर्णांक *wo_end)
{
	अचिन्हित पूर्णांक i, last_untagged;
	काष्ठा object_entry *objects = to_pack.objects;

	क्रम (i = 0; i < to_pack.nr_objects; i++) {
		अगर (objects[i].tagged)
			अवरोध;
		add_to_write_order(wo, wo_end, &objects[i]);
	}
	last_untagged = i;

	/*
	 * Then fill all the tagged tips.
	 */
	क्रम (; i < to_pack.nr_objects; i++) {
		अगर (objects[i].tagged)
			add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all remaining commits and tags.
	 */
	क्रम (i = last_untagged; i < to_pack.nr_objects; i++) {
		अगर (oe_type(&objects[i]) != OBJ_COMMIT &&
		    oe_type(&objects[i]) != OBJ_TAG)
			जारी;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * And then all the trees.
	 */
	क्रम (i = last_untagged; i < to_pack.nr_objects; i++) {
		अगर (oe_type(&objects[i]) != OBJ_वृक्ष)
			जारी;
		add_to_write_order(wo, wo_end, &objects[i]);
	}

	/*
	 * Finally all the rest in really tight order
	 */
	क्रम (i = last_untagged; i < to_pack.nr_objects; i++) {
		अगर (!objects[i].filled && oe_layer(&to_pack, &objects[i]) == write_layer)
			add_family_to_write_order(wo, wo_end, &objects[i]);
	}
}

अटल काष्ठा object_entry **compute_write_order(व्योम)
{
	uint32_t max_layers = 1;
	अचिन्हित पूर्णांक i, wo_end;

	काष्ठा object_entry **wo;
	काष्ठा object_entry *objects = to_pack.objects;

	क्रम (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		SET_DELTA_CHILD(&objects[i], शून्य);
		SET_DELTA_SIBLING(&objects[i], शून्य);
	}

	/*
	 * Fully connect delta_child/delta_sibling network.
	 * Make sure delta_sibling is sorted in the original
	 * recency order.
	 */
	क्रम (i = to_pack.nr_objects; i > 0;) {
		काष्ठा object_entry *e = &objects[--i];
		अगर (!DELTA(e))
			जारी;
		/* Mark me as the first child */
		e->delta_sibling_idx = DELTA(e)->delta_child_idx;
		SET_DELTA_CHILD(DELTA(e), e);
	}

	/*
	 * Mark objects that are at the tip of tags.
	 */
	refs_for_each_tag_ref(get_main_ref_store(the_repository), mark_tagged,
			      शून्य);

	अगर (use_delta_islands) {
		max_layers = compute_pack_layers(&to_pack);
		free_island_marks();
	}

	ALLOC_ARRAY(wo, to_pack.nr_objects);
	wo_end = 0;

	क्रम (; write_layer < max_layers; ++write_layer)
		compute_layer_order(wo, &wo_end);

	अगर (wo_end != to_pack.nr_objects)
		die(_("ordered %u objects, expected %"PRIu32),
		    wo_end, to_pack.nr_objects);

	वापस wo;
}


/*
 * A reused set of objects. All objects in a chunk have the same
 * relative position in the original packfile and the generated
 * packfile.
 */

अटल काष्ठा reused_chunk {
	/* The offset of the first object of this chunk in the original
	 * packfile. */
	off_t original;
	/* The difference for "original" minus the offset of the first object of
	 * this chunk in the generated packfile. */
	off_t difference;
} *reused_chunks;
अटल पूर्णांक reused_chunks_nr;
अटल पूर्णांक reused_chunks_alloc;

अटल व्योम record_reused_object(off_t where, off_t offset)
{
	अगर (reused_chunks_nr && reused_chunks[reused_chunks_nr-1].difference == offset)
		वापस;

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
अटल off_t find_reused_offset(off_t where)
{
	पूर्णांक lo = 0, hi = reused_chunks_nr;
	जबतक (lo < hi) {
		पूर्णांक mi = lo + ((hi - lo) / 2);
		अगर (where == reused_chunks[mi].original)
			वापस reused_chunks[mi].difference;
		अगर (where < reused_chunks[mi].original)
			hi = mi;
		अन्यथा
			lo = mi + 1;
	}

	/*
	 * The first chunk starts at zero, so we can't have gone below
	 * there.
	 */
	निश्चित(lo);
	वापस reused_chunks[lo-1].difference;
}

अटल व्योम write_reused_pack_one(काष्ठा packed_git *reuse_packfile,
				  माप_प्रकार pos, काष्ठा hashfile *out,
				  off_t pack_start,
				  काष्ठा pack_window **w_curs)
{
	off_t offset, अग्र, cur;
	क्रमागत object_type type;
	माप_प्रकार आकार;

	offset = pack_pos_to_offset(reuse_packfile, pos);
	अग्र = pack_pos_to_offset(reuse_packfile, pos + 1);

	record_reused_object(offset,
			     offset - (hashfile_योग(out) - pack_start));

	cur = offset;
	type = unpack_object_header(reuse_packfile, w_curs, &cur, &आकार);
	निश्चित(type >= 0);

	अगर (type == OBJ_OFS_DELTA) {
		off_t base_offset;
		off_t fixup;

		अचिन्हित अक्षर header[MAX_PACK_OBJECT_HEADER];
		अचिन्हित len;

		base_offset = get_delta_base(reuse_packfile, w_curs, &cur, type, offset);
		निश्चित(base_offset != 0);

		/* Convert to REF_DELTA if we must... */
		अगर (!allow_ofs_delta) {
			uint32_t base_pos;
			काष्ठा object_id base_oid;

			अगर (offset_to_pack_pos(reuse_packfile, base_offset, &base_pos) < 0)
				die(_("expected object at offset %"PRIuMAX" "
				      "in pack %s"),
				    (uintmax_t)base_offset,
				    reuse_packfile->pack_name);

			nth_packed_object_id(&base_oid, reuse_packfile,
					     pack_pos_to_सूचकांक(reuse_packfile, base_pos));

			len = encode_in_pack_object_header(header, माप(header),
							   OBJ_REF_DELTA, आकार);
			hashwrite(out, header, len);
			hashwrite(out, base_oid.hash, the_hash_algo->rawsz);
			copy_pack_तथ्य(out, reuse_packfile, w_curs, cur, अग्र - cur);
			वापस;
		}

		/* Otherwise see if we need to rewrite the offset... */
		fixup = find_reused_offset(offset) -
			find_reused_offset(base_offset);
		अगर (fixup) {
			अचिन्हित अक्षर ofs_header[MAX_PACK_OBJECT_HEADER];
			अचिन्हित i, ofs_len;
			off_t ofs = offset - base_offset - fixup;

			len = encode_in_pack_object_header(header, माप(header),
							   OBJ_OFS_DELTA, आकार);

			i = माप(ofs_header) - 1;
			ofs_header[i] = ofs & 127;
			जबतक (ofs >>= 7)
				ofs_header[--i] = 128 | (--ofs & 127);

			ofs_len = माप(ofs_header) - i;

			hashwrite(out, header, len);
			hashwrite(out, ofs_header + माप(ofs_header) - ofs_len, ofs_len);
			copy_pack_तथ्य(out, reuse_packfile, w_curs, cur, अग्र - cur);
			वापस;
		}

		/* ...otherwise we have no fixup, and can write it verbatim */
	}

	copy_pack_तथ्य(out, reuse_packfile, w_curs, offset, अग्र - offset);
}

अटल माप_प्रकार write_reused_pack_verbatim(काष्ठा bitmapped_pack *reuse_packfile,
					 काष्ठा hashfile *out,
					 काष्ठा pack_window **w_curs)
{
	माप_प्रकार pos = 0;
	माप_प्रकार end;

	अगर (reuse_packfile->bitmap_pos) {
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
		वापस reuse_packfile->bitmap_pos / BITS_IN_EWORD;
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
	अगर (reuse_packfile_bitmap->word_alloc < end)
		BUG("fewer words than expected in reuse_packfile_bitmap");

	जबतक (pos < end && reuse_packfile_bitmap->शब्दगण[pos] == (eword_t)~0)
		pos++;

	अगर (pos) {
		off_t to_write;

		written = (pos * BITS_IN_EWORD);
		to_write = pack_pos_to_offset(reuse_packfile->p, written)
			- माप(काष्ठा pack_header);

		/* We're recording one chunk, not one object. */
		record_reused_object(माप(काष्ठा pack_header), 0);
		hashflush(out);
		copy_pack_तथ्य(out, reuse_packfile->p, w_curs,
			माप(काष्ठा pack_header), to_write);

		display_progress(progress_state, written);
	}
	वापस pos;
}

अटल व्योम write_reused_pack(काष्ठा bitmapped_pack *reuse_packfile,
			      काष्ठा hashfile *f)
{
	माप_प्रकार i = reuse_packfile->bitmap_pos / BITS_IN_EWORD;
	uint32_t offset;
	off_t pack_start = hashfile_योग(f) - माप(काष्ठा pack_header);
	काष्ठा pack_window *w_curs = शून्य;

	अगर (allow_ofs_delta)
		i = write_reused_pack_verbatim(reuse_packfile, f, &w_curs);

	क्रम (; i < reuse_packfile_bitmap->word_alloc; ++i) {
		eword_t word = reuse_packfile_bitmap->शब्दगण[i];
		माप_प्रकार pos = (i * BITS_IN_EWORD);

		क्रम (offset = 0; offset < BITS_IN_EWORD; ++offset) {
			uint32_t pack_pos;
			अगर ((word >> offset) == 0)
				अवरोध;

			offset += ewah_bit_ctz64(word >> offset);
			अगर (pos + offset < reuse_packfile->bitmap_pos)
				जारी;
			अगर (pos + offset >= reuse_packfile->bitmap_pos + reuse_packfile->bitmap_nr)
				जाओ done;

			अगर (reuse_packfile->bitmap_pos) {
				/*
				 * When doing multi-pack reuse on a
				 * non-preferred pack, translate bit positions
				 * from the MIDX pseudo-pack order back to their
				 * pack-relative positions before attempting
				 * reuse.
				 */
				काष्ठा multi_pack_सूचकांक *m = reuse_packfile->from_midx;
				uint32_t midx_pos;
				off_t pack_ofs;

				अगर (!m)
					BUG("non-zero bitmap position without MIDX");

				midx_pos = pack_pos_to_midx(m, pos + offset);
				pack_ofs = nth_midxed_offset(m, midx_pos);

				अगर (offset_to_pack_pos(reuse_packfile->p,
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

अटल व्योम write_excluded_by_configs(व्योम)
{
	काष्ठा oidset_iter iter;
	स्थिर काष्ठा object_id *oid;

	oidset_iter_init(&excluded_by_config, &iter);
	जबतक ((oid = oidset_iter_अग्र(&iter))) {
		काष्ठा configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);

		अगर (!ex)
			BUG("configured exclusion wasn't configured");
		write_in_full(1, ex->pack_hash_hex, म_माप(ex->pack_hash_hex));
		write_in_full(1, " ", 1);
		write_in_full(1, ex->uri, म_माप(ex->uri));
		write_in_full(1, "\n", 1);
	}
}

अटल स्थिर अक्षर no_split_warning[] = N_(
"disabling bitmap writing, packs are split due to pack.packSizeLimit"
);

अटल व्योम write_pack_file(व्योम)
{
	uint32_t i = 0, j;
	काष्ठा hashfile *f;
	off_t offset;
	uint32_t nr_remaining = nr_परिणाम;
	समय_प्रकार last_mtime = 0;
	काष्ठा object_entry **write_order;
	off_t bytes_written = 0;

	अगर (progress > pack_to_stdout)
		progress_state = start_progress(the_repository,
						_("Writing objects"), nr_परिणाम);
	ALLOC_ARRAY(written_list, to_pack.nr_objects);
	write_order = compute_write_order();

	करो {
		अचिन्हित अक्षर hash[GIT_MAX_RAWSZ];
		अक्षर *pack_tmp_name = शून्य;

		अगर (pack_to_stdout) {
			/*
			 * This command is most often invoked via
			 * git-upload-pack(1), which will typically chunk data
			 * into pktlines. As such, we use the maximum data
			 * length of them as buffer length.
			 *
			 * Note that we need to subtract one though to
			 * accommodate for the sideband byte.
			 */
			काष्ठा hashfd_options opts = {
				.progress = progress_state,
				.बफर_len = LARGE_PACKET_तथ्य_MAX - 1,
			};
			f = hashfd_ext(the_repository->hash_algo, 1,
				       "<stdout>", &opts);
		} अन्यथा {
			f = create_tmp_packfile(the_repository, &pack_tmp_name);
		}

		offset = write_pack_header(f, nr_remaining);

		अगर (reuse_packfiles_nr) {
			निश्चित(pack_to_stdout);
			क्रम (j = 0; j < reuse_packfiles_nr; j++) {
				reused_chunks_nr = 0;
				write_reused_pack(&reuse_packfiles[j], f);
				अगर (reused_chunks_nr)
					reuse_packfiles_used_nr++;
			}
			offset = hashfile_योग(f);
		}

		nr_written = 0;
		क्रम (; i < to_pack.nr_objects; i++) {
			काष्ठा object_entry *e = write_order[i];
			अगर (write_one(f, e, &offset) == WRITE_ONE_BREAK)
				अवरोध;
			display_progress(progress_state, written);
		}

		bytes_written += hashfile_योग(f) +
			the_repository->hash_algo->rawsz;
		अगर (pack_to_stdout) {
			/*
			 * We never fsync when writing to stdout since we may
			 * not be writing to an actual pack file. For instance,
			 * the upload-pack code passes a pipe here. Calling
			 * fsync on a pipe results in unnecessary
			 * synchronization with the reader on some platforms.
			 */
			finalize_hashfile(f, hash, FSYNC_COMPONENT_NONE,
					  CSUM_HASH_IN_STREAM | CSUM_CLOSE);
		} अन्यथा अगर (nr_written == nr_remaining) {
			finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK,
					  CSUM_HASH_IN_STREAM | CSUM_FSYNC | CSUM_CLOSE);
		} अन्यथा {
			/*
			 * If we wrote the wrong number of entries in the
			 * header, rewrite it like in fast-import.
			 */

			पूर्णांक fd = finalize_hashfile(f, hash, FSYNC_COMPONENT_PACK, 0);
			fixup_pack_header_footer(the_hash_algo, fd, hash,
						 pack_tmp_name, nr_written,
						 hash, offset);
			बंद(fd);
			अगर (write_bitmap_सूचकांक) {
				अगर (write_bitmap_सूचकांक != WRITE_BITMAP_QUIET)
					warning(_(no_split_warning));
				write_bitmap_सूचकांक = 0;
			}
		}

		अगर (!pack_to_stdout) {
			काष्ठा stat st;
			काष्ठा strbuf tmpname = STRBUF_INIT;
			काष्ठा bitmap_writer bitmap_writer;
			अक्षर *idx_tmp_name = शून्य;

			/*
			 * Packs are runtime accessed in their mtime
			 * order since newer packs are more likely to contain
			 * younger objects.  So if we are creating multiple
			 * packs then we should modify the mtime of later ones
			 * to preserve this property.
			 */
			अगर (stat(pack_tmp_name, &st) < 0) {
				warning_errno(_("failed to stat %s"), pack_tmp_name);
			} अन्यथा अगर (!last_mtime) {
				last_mtime = st.st_mtime;
			} अन्यथा {
				काष्ठा utimbuf utb;
				utb.actime = st.st_atime;
				utb.modtime = --last_mtime;
				अगर (utime(pack_tmp_name, &utb) < 0)
					warning_errno(_("failed utime() on %s"), pack_tmp_name);
			}

			strbuf_addf(&tmpname, "%s-%s.", base_name,
				    hash_to_hex(hash));

			अगर (write_bitmap_सूचकांक) {
				bitmap_writer_init(&bitmap_writer,
						   the_repository, &to_pack,
						   शून्य);
				bitmap_writer_set_checksum(&bitmap_writer, hash);
				bitmap_writer_build_type_सूचकांक(&bitmap_writer,
							       written_list);
			}

			अगर (cruft)
				pack_idx_opts.flags |= WRITE_MTIMES;

			stage_tmp_packfiles(the_repository, &tmpname,
					    pack_tmp_name, written_list,
					    nr_written, &to_pack,
					    &pack_idx_opts, hash,
					    &idx_tmp_name);

			अगर (write_bitmap_सूचकांक) {
				माप_प्रकार tmpname_len = tmpname.len;

				strbuf_addstr(&tmpname, "bitmap");
				stop_progress(&progress_state);

				bitmap_writer_show_progress(&bitmap_writer,
							    progress);
				bitmap_writer_select_commits(&bitmap_writer,
							     indexed_commits,
							     indexed_commits_nr);
				अगर (bitmap_writer_build(&bitmap_writer) < 0)
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
			माला_दो(hash_to_hex(hash));
		}

		/* mark written objects as written to previous pack */
		क्रम (j = 0; j < nr_written; j++) {
			written_list[j]->offset = (off_t)-1;
		}
		nr_remaining -= nr_written;
	} जबतक (nr_remaining && i < to_pack.nr_objects);

	मुक्त(written_list);
	मुक्त(write_order);
	stop_progress(&progress_state);
	अगर (written != nr_परिणाम)
		die(_("wrote %"PRIu32" objects while expecting %"PRIu32),
		    written, nr_परिणाम);
	trace2_तथ्य_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote", nr_परिणाम);
	trace2_तथ्य_intmax("pack-objects", the_repository,
			   "write_pack_file/wrote_bytes", bytes_written);
}

अटल पूर्णांक no_try_delta(स्थिर अक्षर *path)
{
	अटल काष्ठा attr_check *check;

	अगर (!check)
		check = attr_check_initl("delta", शून्य);
	git_check_attr(the_repository->सूचकांक, path, check);
	अगर (ATTR_FALSE(check->वस्तुएँ[0].मूल्य))
		वापस 1;
	वापस 0;
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
अटल पूर्णांक have_duplicate_entry(स्थिर काष्ठा object_id *oid,
				पूर्णांक exclude)
{
	काष्ठा object_entry *entry;

	अगर (reuse_packfile_bitmap &&
	    bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid))
		वापस 1;

	entry = packlist_find(&to_pack, oid);
	अगर (!entry)
		वापस 0;

	अगर (exclude) {
		अगर (!entry->preferred_base)
			nr_परिणाम--;
		entry->preferred_base = 1;
	}

	वापस 1;
}

अटल पूर्णांक want_cruft_object_mtime(काष्ठा repository *r,
				   स्थिर काष्ठा object_id *oid,
				   अचिन्हित flags, uint32_t mtime)
{
	काष्ठा odb_source_files *files = odb_source_files_downcast(r->objects->source);

	क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		काष्ठा packed_git **cache = packfile_store_get_kept_pack_cache(dir->packed, flags);

		क्रम (; *cache; cache++) {
			काष्ठा packed_git *p = *cache;
			off_t ofs;
			uint32_t candidate_mtime;

			ofs = find_pack_entry_one(oid, p);
			अगर (!ofs)
				जारी;

			/*
			 * We have a copy of the object 'oid' in a non-cruft
			 * pack. We can avoid packing an additional copy
			 * regardless of what the existing copy's mtime is since
			 * it is outside of a cruft pack.
			 */
			अगर (!p->is_cruft)
				वापस 0;

			/*
			 * If we have a copy of the object 'oid' in a cruft
			 * pack, then either read the cruft pack's mtime for
			 * that object, or, if that can't be loaded, assume the
			 * pack's mtime itself.
			 */
			अगर (!load_pack_mtimes(p)) {
				uint32_t pos;
				अगर (offset_to_pack_pos(p, ofs, &pos) < 0)
					जारी;
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
			अगर (mtime <= candidate_mtime)
				वापस 0;
		}
	}

	वापस -1;
}

अटल पूर्णांक want_found_object(स्थिर काष्ठा object_id *oid, पूर्णांक exclude,
			     काष्ठा packed_git *p, uint32_t mtime)
{
	अगर (exclude)
		वापस 1;
	अगर (incremental)
		वापस 0;

	अगर (!is_pack_valid(p))
		वापस -1;

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
	अगर (local && !p->pack_local)
		वापस 0;

	/*
	 * Then handle .keep first, as we have a fast(er) path there.
	 */
	अगर (ignore_packed_keep_on_disk || ignore_packed_keep_in_core ||
	    ignore_packed_keep_in_core_open) {
		/*
		 * Set the flags for the kept-pack cache to be the ones we want
		 * to ignore.
		 *
		 * That is, if we are ignoring objects in on-disk keep packs,
		 * then we want to search through the on-disk keep and ignore
		 * the in-core ones.
		 */
		अचिन्हित flags = 0;
		अगर (ignore_packed_keep_on_disk)
			flags |= KEPT_PACK_ON_DISK;
		अगर (ignore_packed_keep_in_core)
			flags |= KEPT_PACK_IN_CORE;
		अगर (ignore_packed_keep_in_core_open)
			flags |= KEPT_PACK_IN_CORE_OPEN;

		/*
		 * If the object is in a pack that we want to ignore, *and* we
		 * don't have any cruft packs that are being retained, we can
		 * abort quickly.
		 */
		अगर (!ignore_packed_keep_in_core_has_cruft) {
			अगर (ignore_packed_keep_on_disk && p->pack_keep)
				वापस 0;
			अगर (ignore_packed_keep_in_core && p->pack_keep_in_core)
				वापस 0;
			अगर (ignore_packed_keep_in_core_open && p->pack_keep_in_core_open)
				वापस 0;
			अगर (has_object_kept_pack(p->repo, oid, flags))
				वापस 0;
		} अन्यथा {
			/*
			 * But if there is at least one cruft pack which
			 * is being kept, we only want to include the
			 * provided object if it has a strictly greater
			 * mtime than any existing cruft copy.
			 */
			अगर (!want_cruft_object_mtime(p->repo, oid, flags,
						     mtime))
				वापस 0;
		}
	}

	/*
	 * At this point we know definitively that either we don't care about
	 * keep-packs, or the object is not in one. Keep checking other
	 * conditions...
	 */
	अगर (!local || !have_non_local_packs)
		वापस 1;

	/* we don't know yet; keep looking for more packs */
	वापस -1;
}

अटल पूर्णांक want_object_in_pack_one(काष्ठा packed_git *p,
				   स्थिर काष्ठा object_id *oid,
				   पूर्णांक exclude,
				   काष्ठा packed_git **found_pack,
				   off_t *found_offset,
				   uint32_t found_mtime)
{
	off_t offset;

	अगर (p == *found_pack)
		offset = *found_offset;
	अन्यथा
		offset = find_pack_entry_one(oid, p);

	अगर (offset) {
		अगर (!*found_pack) {
			अगर (!is_pack_valid(p))
				वापस -1;
			*found_offset = offset;
			*found_pack = p;
		}
		वापस want_found_object(oid, exclude, p, found_mtime);
	}
	वापस -1;
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
अटल पूर्णांक want_object_in_pack_mtime(स्थिर काष्ठा object_id *oid,
				     पूर्णांक exclude,
				     काष्ठा packed_git **found_pack,
				     off_t *found_offset,
				     uint32_t found_mtime)
{
	काष्ठा odb_source_files *files =
		odb_source_files_downcast(the_repository->objects->source);
	पूर्णांक want;
	काष्ठा packfile_list_entry *e;

	अगर (!exclude && local) {
		/*
		 * Note that we start iterating at `dirs->next` so that we
		 * skip the local object directory.
		 */
		क्रम (काष्ठा odb_files_dir *dir = files->dirs->अग्र; dir; dir = dir->अग्र)
			अगर (!odb_source_read_object_info(&dir->loose->base, oid, शून्य, 0, शून्य))
				वापस 0;
	}

	/*
	 * If we already know the pack object lives in, start checks from that
	 * pack - in the usual case when neither --local was given nor .keep files
	 * are present we will determine the answer right now.
	 */
	अगर (*found_pack) {
		want = want_found_object(oid, exclude, *found_pack,
					 found_mtime);
		अगर (want != -1)
			वापस want;

		*found_pack = शून्य;
		*found_offset = 0;
	}

	क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		काष्ठा multi_pack_सूचकांक *m = get_multi_pack_सूचकांक(dir->packed);
		काष्ठा pack_entry e;

		अगर (m && midx_fill_entry(m, oid, &e, शून्य) == MIDX_FILL_HIT) {
			want = want_object_in_pack_one(e.p, oid, exclude, found_pack, found_offset, found_mtime);
			अगर (want != -1)
				वापस want;
		}
	}

	क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		क्रम (e = dir->packed->packs.शीर्ष; e; e = e->अग्र) {
			काष्ठा packed_git *p = e->pack;
			want = want_object_in_pack_one(p, oid, exclude, found_pack, found_offset, found_mtime);
			अगर (!exclude && want > 0)
				packfile_list_prepend(&dir->packed->packs, p);
			अगर (want != -1)
				वापस want;
		}
	}

	अगर (uri_protocols.nr) {
		काष्ठा configured_exclusion *ex =
			oidmap_get(&configured_exclusions, oid);
		पूर्णांक i;
		स्थिर अक्षर *p;

		अगर (ex) {
			क्रम (i = 0; i < uri_protocols.nr; i++) {
				अगर (skip_prefix(ex->uri,
						uri_protocols.वस्तुएँ[i].string,
						&p) &&
				    *p == ':') {
					oidset_जोड़ो(&excluded_by_config, oid);
					वापस 0;
				}
			}
		}
	}

	वापस 1;
}

अटल अंतरभूत पूर्णांक want_object_in_pack(स्थिर काष्ठा object_id *oid,
				      पूर्णांक exclude,
				      काष्ठा packed_git **found_pack,
				      off_t *found_offset)
{
	वापस want_object_in_pack_mtime(oid, exclude, found_pack, found_offset,
					 0);
}

अटल काष्ठा object_entry *create_object_entry(स्थिर काष्ठा object_id *oid,
						क्रमागत object_type type,
						uint32_t hash,
						पूर्णांक exclude,
						पूर्णांक no_try_delta,
						काष्ठा packed_git *found_pack,
						off_t found_offset)
{
	काष्ठा object_entry *entry;

	entry = packlist_alloc(&to_pack, oid);
	entry->hash = hash;
	oe_set_type(entry, type);
	अगर (exclude)
		entry->preferred_base = 1;
	अन्यथा
		nr_परिणाम++;
	अगर (found_pack) {
		oe_set_in_pack(&to_pack, entry, found_pack);
		entry->in_pack_offset = found_offset;
	}

	entry->no_try_delta = no_try_delta;

	वापस entry;
}

अटल स्थिर अक्षर no_closure_warning[] = N_(
"disabling bitmap writing, as some objects are not being packed"
);

अटल व्योम add_object_entry(स्थिर काष्ठा object_id *oid, क्रमागत object_type type,
			     स्थिर अक्षर *name, पूर्णांक exclude)
{
	काष्ठा packed_git *found_pack = शून्य;
	off_t found_offset = 0;

	display_progress(progress_state, ++nr_seen);

	अगर (have_duplicate_entry(oid, exclude))
		वापस;

	अगर (!want_object_in_pack(oid, exclude, &found_pack, &found_offset)) {
		/* The pack is missing an object, so it will not have closure */
		अगर (write_bitmap_सूचकांक) {
			अगर (write_bitmap_सूचकांक != WRITE_BITMAP_QUIET)
				warning(_(no_closure_warning));
			write_bitmap_सूचकांक = 0;
		}
		वापस;
	}

	create_object_entry(oid, type, pack_name_hash_fn(name),
			    exclude, name && no_try_delta(name),
			    found_pack, found_offset);
}

अटल पूर्णांक add_object_entry_from_bitmap(स्थिर काष्ठा object_id *oid,
					क्रमागत object_type type,
					पूर्णांक flags UNUSED, uint32_t name_hash,
					काष्ठा packed_git *pack, off_t offset,
					व्योम *payload UNUSED)
{
	display_progress(progress_state, ++nr_seen);

	अगर (have_duplicate_entry(oid, 0))
		वापस 0;

	अगर (!want_object_in_pack(oid, 0, &pack, &offset))
		वापस 0;

	create_object_entry(oid, type, name_hash, 0, 0, pack, offset);
	वापस 0;
}

काष्ठा pbase_वृक्ष_cache {
	काष्ठा object_id oid;
	पूर्णांक ref;
	पूर्णांक temporary;
	व्योम *वृक्ष_तथ्य;
	अचिन्हित दीर्घ वृक्ष_आकार;
};

अटल काष्ठा pbase_वृक्ष_cache *(pbase_वृक्ष_cache[256]);
अटल पूर्णांक pbase_वृक्ष_cache_ix(स्थिर काष्ठा object_id *oid)
{
	वापस oid->hash[0] % ARRAY_आकार(pbase_वृक्ष_cache);
}
अटल पूर्णांक pbase_वृक्ष_cache_ix_incr(पूर्णांक ix)
{
	वापस (ix+1) % ARRAY_आकार(pbase_वृक्ष_cache);
}

अटल काष्ठा pbase_वृक्ष {
	काष्ठा pbase_वृक्ष *अग्र;
	/* This is a phony "cache" entry; we are not
	 * going to evict it or find it through _get()
	 * mechanism -- this is for the toplevel node that
	 * would almost always change with any commit.
	 */
	काष्ठा pbase_वृक्ष_cache pcache;
} *pbase_वृक्ष;

अटल काष्ठा pbase_वृक्ष_cache *pbase_वृक्ष_get(स्थिर काष्ठा object_id *oid)
{
	काष्ठा pbase_वृक्ष_cache *ent, *nent;
	व्योम *तथ्य;
	अचिन्हित दीर्घ आकार;
	माप_प्रकार आकार_st = 0;
	क्रमागत object_type type;
	पूर्णांक neigh;
	पूर्णांक my_ix = pbase_वृक्ष_cache_ix(oid);
	पूर्णांक available_ix = -1;

	/* pbase-tree-cache acts as a limited hashtable.
	 * your object will be found at your index or within a few
	 * slots after that slot if it is cached.
	 */
	क्रम (neigh = 0; neigh < 8; neigh++) {
		ent = pbase_वृक्ष_cache[my_ix];
		अगर (ent && oideq(&ent->oid, oid)) {
			ent->ref++;
			वापस ent;
		}
		अन्यथा अगर (((available_ix < 0) && (!ent || !ent->ref)) ||
			 ((0 <= available_ix) &&
			  (!ent && pbase_वृक्ष_cache[available_ix])))
			available_ix = my_ix;
		अगर (!ent)
			अवरोध;
		my_ix = pbase_वृक्ष_cache_ix_incr(my_ix);
	}

	/* Did not find one.  Either we got a bogus request or
	 * we need to read and perhaps cache.
	 */
	तथ्य = odb_read_object(the_repository->objects, oid, &type, &आकार_st);
	आकार = cast_आकार_t_to_ulong(आकार_st);
	अगर (!तथ्य)
		वापस शून्य;
	अगर (type != OBJ_वृक्ष) {
		मुक्त(तथ्य);
		वापस शून्य;
	}

	/* We need to either cache or return a throwaway copy */

	अगर (available_ix < 0)
		ent = शून्य;
	अन्यथा {
		ent = pbase_वृक्ष_cache[available_ix];
		my_ix = available_ix;
	}

	अगर (!ent) {
		nent = xmalloc(माप(*nent));
		nent->temporary = (available_ix < 0);
	}
	अन्यथा {
		/* evict and reuse */
		मुक्त(ent->वृक्ष_तथ्य);
		nent = ent;
	}
	oidcpy(&nent->oid, oid);
	nent->वृक्ष_तथ्य = तथ्य;
	nent->वृक्ष_आकार = आकार;
	nent->ref = 1;
	अगर (!nent->temporary)
		pbase_वृक्ष_cache[my_ix] = nent;
	वापस nent;
}

अटल व्योम pbase_वृक्ष_put(काष्ठा pbase_वृक्ष_cache *cache)
{
	अगर (!cache->temporary) {
		cache->ref--;
		वापस;
	}
	मुक्त(cache->वृक्ष_तथ्य);
	मुक्त(cache);
}

अटल माप_प्रकार name_cmp_len(स्थिर अक्षर *name)
{
	वापस म_खोज(name, "\n/");
}

अटल व्योम add_pbase_object(काष्ठा वृक्ष_desc *वृक्ष,
			     स्थिर अक्षर *name,
			     माप_प्रकार cmplen,
			     स्थिर अक्षर *fullname)
{
	काष्ठा name_entry entry;
	पूर्णांक cmp;

	जबतक (वृक्ष_entry(वृक्ष,&entry)) {
		अगर (S_ISGITLINK(entry.mode))
			जारी;
		cmp = वृक्ष_entry_len(&entry) != cmplen ? 1 :
		      स_भेद(name, entry.path, cmplen);
		अगर (cmp > 0)
			जारी;
		अगर (cmp < 0)
			वापस;
		अगर (name[cmplen] != '/') {
			add_object_entry(&entry.oid,
					 object_type(entry.mode),
					 fullname, 1);
			वापस;
		}
		अगर (S_ISDIR(entry.mode)) {
			काष्ठा वृक्ष_desc sub;
			काष्ठा pbase_वृक्ष_cache *वृक्ष;
			स्थिर अक्षर *down = name+cmplen+1;
			माप_प्रकार downlen = name_cmp_len(down);

			वृक्ष = pbase_वृक्ष_get(&entry.oid);
			अगर (!वृक्ष)
				वापस;
			init_वृक्ष_desc(&sub, &वृक्ष->oid,
				       वृक्ष->वृक्ष_तथ्य, वृक्ष->वृक्ष_आकार);

			add_pbase_object(&sub, down, downlen, fullname);
			pbase_वृक्ष_put(वृक्ष);
		}
	}
}

अटल अचिन्हित *done_pbase_paths;
अटल पूर्णांक done_pbase_paths_अंक;
अटल पूर्णांक done_pbase_paths_alloc;
अटल पूर्णांक done_pbase_path_pos(अचिन्हित hash)
{
	पूर्णांक lo = 0;
	पूर्णांक hi = done_pbase_paths_अंक;
	जबतक (lo < hi) {
		पूर्णांक mi = lo + (hi - lo) / 2;
		अगर (done_pbase_paths[mi] == hash)
			वापस mi;
		अगर (done_pbase_paths[mi] < hash)
			hi = mi;
		अन्यथा
			lo = mi + 1;
	}
	वापस -lo-1;
}

अटल पूर्णांक check_pbase_path(अचिन्हित hash)
{
	पूर्णांक pos = done_pbase_path_pos(hash);
	अगर (0 <= pos)
		वापस 1;
	pos = -pos - 1;
	ALLOC_GROW(done_pbase_paths,
		   done_pbase_paths_अंक + 1,
		   done_pbase_paths_alloc);
	done_pbase_paths_अंक++;
	अगर (pos < done_pbase_paths_अंक)
		MOVE_ARRAY(done_pbase_paths + pos + 1, done_pbase_paths + pos,
			   done_pbase_paths_अंक - pos - 1);
	done_pbase_paths[pos] = hash;
	वापस 0;
}

अटल व्योम add_preferred_base_object(स्थिर अक्षर *name)
{
	काष्ठा pbase_वृक्ष *it;
	माप_प्रकार cmplen;
	अचिन्हित hash = pack_name_hash_fn(name);

	अगर (!अंक_preferred_base || check_pbase_path(hash))
		वापस;

	cmplen = name_cmp_len(name);
	क्रम (it = pbase_वृक्ष; it; it = it->अग्र) {
		अगर (cmplen == 0) {
			add_object_entry(&it->pcache.oid, OBJ_वृक्ष, शून्य, 1);
		}
		अन्यथा {
			काष्ठा वृक्ष_desc वृक्ष;
			init_वृक्ष_desc(&वृक्ष, &it->pcache.oid,
				       it->pcache.वृक्ष_तथ्य, it->pcache.वृक्ष_आकार);
			add_pbase_object(&वृक्ष, name, cmplen, name);
		}
	}
}

अटल व्योम add_preferred_base(काष्ठा object_id *oid)
{
	काष्ठा pbase_वृक्ष *it;
	व्योम *तथ्य;
	अचिन्हित दीर्घ आकार;
	माप_प्रकार आकार_st = 0;
	काष्ठा object_id वृक्ष_oid;

	अगर (window <= अंक_preferred_base++)
		वापस;

	तथ्य = odb_read_object_peeled(the_repository->objects, oid,
				      OBJ_वृक्ष, &आकार_st, &वृक्ष_oid);
	आकार = cast_आकार_t_to_ulong(आकार_st);
	अगर (!तथ्य)
		वापस;

	क्रम (it = pbase_वृक्ष; it; it = it->अग्र) {
		अगर (oideq(&it->pcache.oid, &वृक्ष_oid)) {
			मुक्त(तथ्य);
			वापस;
		}
	}

	CALLOC_ARRAY(it, 1);
	it->अग्र = pbase_वृक्ष;
	pbase_वृक्ष = it;

	oidcpy(&it->pcache.oid, &वृक्ष_oid);
	it->pcache.वृक्ष_तथ्य = तथ्य;
	it->pcache.वृक्ष_आकार = आकार;
}

अटल व्योम cleanup_preferred_base(व्योम)
{
	काष्ठा pbase_वृक्ष *it;
	अचिन्हित i;

	it = pbase_वृक्ष;
	pbase_वृक्ष = शून्य;
	जबतक (it) {
		काष्ठा pbase_वृक्ष *tmp = it;
		it = tmp->अग्र;
		मुक्त(tmp->pcache.वृक्ष_तथ्य);
		मुक्त(tmp);
	}

	क्रम (i = 0; i < ARRAY_आकार(pbase_वृक्ष_cache); i++) {
		अगर (!pbase_वृक्ष_cache[i])
			जारी;
		मुक्त(pbase_वृक्ष_cache[i]->वृक्ष_तथ्य);
		FREE_AND_NULL(pbase_वृक्ष_cache[i]);
	}

	FREE_AND_NULL(done_pbase_paths);
	done_pbase_paths_अंक = done_pbase_paths_alloc = 0;
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
अटल पूर्णांक can_reuse_delta(स्थिर काष्ठा object_id *base_oid,
			   काष्ठा object_entry *delta,
			   काष्ठा object_entry **base_out)
{
	काष्ठा object_entry *base;

	/*
	 * First see if we're already sending the base (or it's explicitly in
	 * our "excluded" list).
	 */
	base = packlist_find(&to_pack, base_oid);
	अगर (base) {
		अगर (!in_same_island(&delta->idx.oid, &base->idx.oid))
			वापस 0;
		*base_out = base;
		वापस 1;
	}

	/*
	 * Otherwise, reachability bitmaps may tell us if the receiver has it,
	 * even if it was buried too deep in history to make it into the
	 * packing list.
	 */
	अगर (thin && bitmap_has_oid_in_uninteresting(bitmap_git, base_oid)) {
		अगर (use_delta_islands) {
			अगर (!in_same_island(&delta->idx.oid, base_oid))
				वापस 0;
		}
		*base_out = शून्य;
		वापस 1;
	}

	वापस 0;
}

अटल व्योम prefetch_to_pack(uint32_t object_सूचकांक_start) {
	काष्ठा oid_array to_fetch = OID_ARRAY_INIT;
	uint32_t i;

	क्रम (i = object_सूचकांक_start; i < to_pack.nr_objects; i++) {
		काष्ठा object_entry *entry = to_pack.objects + i;

		अगर (!odb_read_object_info_extended(the_repository->objects,
						   &entry->idx.oid,
						   शून्य,
						   OBJECT_INFO_FOR_PREFETCH))
			जारी;
		oid_array_append(&to_fetch, &entry->idx.oid);
	}
	promisor_remote_get_direct(the_repository,
				   to_fetch.oid, to_fetch.nr);
	oid_array_clear(&to_fetch);
}

अटल व्योम check_object(काष्ठा object_entry *entry, uint32_t object_सूचकांक)
{
	माप_प्रकार canonical_आकार;
	क्रमागत object_type type;
	काष्ठा object_info oi = {.typep = &type, .sizep = &canonical_आकार};

	अगर (IN_PACK(entry)) {
		काष्ठा packed_git *p = IN_PACK(entry);
		काष्ठा pack_window *w_curs = शून्य;
		पूर्णांक have_base = 0;
		काष्ठा object_id base_ref;
		काष्ठा object_entry *base_entry;
		माप_प्रकार used, used_0, avail;
		off_t ofs;
		अचिन्हित अक्षर *buf, c;
		क्रमागत object_type type;
		माप_प्रकार in_pack_आकार;

		buf = use_pack(p, &w_curs, entry->in_pack_offset, &avail);

		/*
		 * We want in_pack_type even if we do not reuse delta
		 * since non-delta representations could still be reused.
		 */
		used = unpack_object_header_बफर(buf, avail,
						   &type,
						   &in_pack_आकार);
		अगर (used == 0)
			जाओ give_up;

		अगर (type < 0)
			BUG("invalid type %d", type);
		entry->in_pack_type = type;

		/*
		 * Determine if this is a delta and if so whether we can
		 * reuse it or not.  Otherwise let's find out as cheaply as
		 * possible what the actual type and size for this object is.
		 */
		चयन (entry->in_pack_type) {
		शेष:
			/* Not a delta hence we've already got all we need. */
			oe_set_type(entry, entry->in_pack_type);
			SET_आकार(entry, in_pack_आकार);
			entry->in_pack_header_आकार = used;
			अगर (oe_type(entry) < OBJ_COMMIT || oe_type(entry) > OBJ_BLOB)
				जाओ give_up;
			unuse_pack(&w_curs);
			वापस;
		हाल OBJ_REF_DELTA:
			अगर (reuse_delta && !entry->preferred_base) {
				oidread(&base_ref,
					use_pack(p, &w_curs,
						 entry->in_pack_offset + used,
						 शून्य),
					the_repository->hash_algo);
				have_base = 1;
			}
			entry->in_pack_header_आकार = used + the_hash_algo->rawsz;
			अवरोध;
		हाल OBJ_OFS_DELTA:
			buf = use_pack(p, &w_curs,
				       entry->in_pack_offset + used, शून्य);
			used_0 = 0;
			c = buf[used_0++];
			ofs = c & 127;
			जबतक (c & 128) {
				ofs += 1;
				अगर (!ofs || MSB(ofs, 7)) {
					error(_("delta base offset overflow in pack for %s"),
					      oid_to_hex(&entry->idx.oid));
					जाओ give_up;
				}
				c = buf[used_0++];
				ofs = (ofs << 7) + (c & 127);
			}
			ofs = entry->in_pack_offset - ofs;
			अगर (ofs <= 0 || ofs >= entry->in_pack_offset) {
				error(_("delta base offset out of bound for %s"),
				      oid_to_hex(&entry->idx.oid));
				जाओ give_up;
			}
			अगर (reuse_delta && !entry->preferred_base) {
				uint32_t pos;
				अगर (offset_to_pack_pos(p, ofs, &pos) < 0)
					जाओ give_up;
				अगर (!nth_packed_object_id(&base_ref, p,
							  pack_pos_to_सूचकांक(p, pos)))
					have_base = 1;
			}
			entry->in_pack_header_आकार = used + used_0;
			अवरोध;
		}

		अगर (have_base &&
		    can_reuse_delta(&base_ref, entry, &base_entry)) {
			oe_set_type(entry, entry->in_pack_type);
			SET_आकार(entry, in_pack_आकार); /* delta size */
			SET_DELTA_आकार(entry, in_pack_आकार);

			अगर (base_entry) {
				SET_DELTA(entry, base_entry);
				entry->delta_sibling_idx = base_entry->delta_child_idx;
				SET_DELTA_CHILD(base_entry, entry);
			} अन्यथा {
				SET_DELTA_EXT(entry, &base_ref);
			}

			unuse_pack(&w_curs);
			वापस;
		}

		अगर (oe_type(entry)) {
			off_t delta_pos;

			/*
			 * This must be a delta and we already know what the
			 * final object type is.  Let's extract the actual
			 * object size from the delta header.
			 */
			delta_pos = entry->in_pack_offset + entry->in_pack_header_आकार;
			canonical_आकार = get_आकार_from_delta(p, &w_curs,
							     delta_pos);
			अगर (canonical_आकार == 0)
				जाओ give_up;
			SET_आकार(entry, canonical_आकार);
			unuse_pack(&w_curs);
			वापस;
		}

		/*
		 * No choice but to fall back to the recursive delta walk
		 * with odb_read_object_info() to find about the object type
		 * at this point...
		 */
		give_up:
		unuse_pack(&w_curs);
	}

	अगर (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
					  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0) {
		अगर (repo_has_promisor_remote(the_repository)) {
			prefetch_to_pack(object_सूचकांक);
			अगर (odb_read_object_info_extended(the_repository->objects, &entry->idx.oid, &oi,
							  OBJECT_INFO_SKIP_FETCH_OBJECT | OBJECT_INFO_LOOKUP_REPLACE) < 0)
				type = -1;
		} अन्यथा {
			type = -1;
		}
	}
	oe_set_type(entry, type);
	अगर (entry->type_valid) {
		SET_आकार(entry, canonical_आकार);
	} अन्यथा {
		/*
		 * Bad object type is checked in prepare_pack().  This is
		 * to permit a missing preferred base object to be ignored
		 * as a preferred base.  Doing so can result in a larger
		 * pack file, but the transfer will still take place.
		 */
	}
}

अटल पूर्णांक pack_offset_sort(स्थिर व्योम *_a, स्थिर व्योम *_b)
{
	स्थिर काष्ठा object_entry *a = *(काष्ठा object_entry **)_a;
	स्थिर काष्ठा object_entry *b = *(काष्ठा object_entry **)_b;
	स्थिर काष्ठा packed_git *a_in_pack = IN_PACK(a);
	स्थिर काष्ठा packed_git *b_in_pack = IN_PACK(b);

	/* avoid filesystem trashing with loose objects */
	अगर (!a_in_pack && !b_in_pack)
		वापस oidcmp(&a->idx.oid, &b->idx.oid);

	अगर (a_in_pack < b_in_pack)
		वापस -1;
	अगर (a_in_pack > b_in_pack)
		वापस 1;
	वापस a->in_pack_offset < b->in_pack_offset ? -1 :
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
अटल व्योम drop_reused_delta(काष्ठा object_entry *entry)
{
	अचिन्हित *idx = &to_pack.objects[entry->delta_idx - 1].delta_child_idx;
	काष्ठा object_info oi = OBJECT_INFO_INIT;
	क्रमागत object_type type;
	माप_प्रकार आकार;

	जबतक (*idx) {
		काष्ठा object_entry *oe = &to_pack.objects[*idx - 1];

		अगर (oe == entry)
			*idx = oe->delta_sibling_idx;
		अन्यथा
			idx = &oe->delta_sibling_idx;
	}
	SET_DELTA(entry, शून्य);
	entry->depth = 0;

	oi.sizep = &आकार;
	oi.typep = &type;
	अगर (packed_object_info(शून्य, IN_PACK(entry), entry->in_pack_offset, &oi) < 0) {
		/*
		 * We failed to get the info from this pack for some reason;
		 * fall back to odb_read_object_info, which may find another copy.
		 * And if that fails, the error will be recorded in oe_type(entry)
		 * and dealt with in prepare_pack().
		 */
		oe_set_type(entry,
			    odb_read_object_info(the_repository->objects,
						 &entry->idx.oid, &आकार));
	} अन्यथा {
		oe_set_type(entry, type);
	}
	SET_आकार(entry, आकार);
}

/*
 * Follow the chain of deltas from this entry onward, throwing away any links
 * that cause us to hit a cycle (as determined by the DFS state flags in
 * the entries).
 *
 * We also detect too-long reused chains that would violate our --depth
 * limit.
 */
अटल व्योम break_delta_chains(काष्ठा object_entry *entry)
{
	/*
	 * The actual depth of each object we will write is stored as an int,
	 * as it cannot exceed our int "depth" limit. But before we break
	 * changes based no that limit, we may potentially go as deep as the
	 * number of objects, which is elsewhere bounded to a uint32_t.
	 */
	uint32_t योग_depth;
	काष्ठा object_entry *cur, *अग्र;

	क्रम (cur = entry, योग_depth = 0;
	     cur;
	     cur = DELTA(cur), योग_depth++) {
		अगर (cur->dfs_state == DFS_DONE) {
			/*
			 * We've already seen this object and know it isn't
			 * part of a cycle. We do need to append its depth
			 * to our count.
			 */
			योग_depth += cur->depth;
			अवरोध;
		}

		/*
		 * We break cycles before looping, so an ACTIVE state (or any
		 * other cruft which made its way into the state variable)
		 * is a bug.
		 */
		अगर (cur->dfs_state != DFS_NONE)
			BUG("confusing delta dfs state in first pass: %d",
			    cur->dfs_state);

		/*
		 * Now we know this is the first time we've seen the object. If
		 * it's not a delta, we're done traversing, but we'll mark it
		 * done to save time on future traversals.
		 */
		अगर (!DELTA(cur)) {
			cur->dfs_state = DFS_DONE;
			अवरोध;
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
		अगर (DELTA(cur)->dfs_state == DFS_ACTIVE) {
			drop_reused_delta(cur);
			cur->dfs_state = DFS_DONE;
			अवरोध;
		}
	}

	/*
	 * And now that we've gone all the way to the bottom of the chain, we
	 * need to clear the active flags and set the depth fields as
	 * appropriate. Unlike the loop above, which can quit when it drops a
	 * delta, we need to keep going to look for more depth cuts. So we need
	 * an extra "next" pointer to keep going after we reset cur->delta.
	 */
	क्रम (cur = entry; cur; cur = अग्र) {
		अग्र = DELTA(cur);

		/*
		 * We should have a chain of zero or more ACTIVE states down to
		 * a final DONE. We can quit after the DONE, because either it
		 * has no bases, or we've already handled them in a previous
		 * call.
		 */
		अगर (cur->dfs_state == DFS_DONE)
			अवरोध;
		अन्यथा अगर (cur->dfs_state != DFS_ACTIVE)
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
		cur->depth = (योग_depth--) % (depth + 1);
		अगर (!cur->depth)
			drop_reused_delta(cur);

		cur->dfs_state = DFS_DONE;
	}
}

अटल व्योम get_object_details(व्योम)
{
	uint32_t i;
	काष्ठा object_entry **sorted_by_offset;

	अगर (progress)
		progress_state = start_progress(the_repository,
						_("Counting objects"),
						to_pack.nr_objects);

	CALLOC_ARRAY(sorted_by_offset, to_pack.nr_objects);
	क्रम (i = 0; i < to_pack.nr_objects; i++)
		sorted_by_offset[i] = to_pack.objects + i;
	QSORT(sorted_by_offset, to_pack.nr_objects, pack_offset_sort);

	क्रम (i = 0; i < to_pack.nr_objects; i++) {
		काष्ठा object_entry *entry = sorted_by_offset[i];
		check_object(entry, i);
		अगर (entry->type_valid &&
		    oe_आकार_greater_than(&to_pack, entry,
					 repo_settings_get_big_file_threshold(the_repository)))
			entry->no_try_delta = 1;
		display_progress(progress_state, i + 1);
	}
	stop_progress(&progress_state);

	/*
	 * This must happen in a second pass, since we rely on the delta
	 * information for the whole list being completed.
	 */
	क्रम (i = 0; i < to_pack.nr_objects; i++)
		break_delta_chains(&to_pack.objects[i]);

	मुक्त(sorted_by_offset);
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
अटल पूर्णांक type_आकार_sort(स्थिर व्योम *_a, स्थिर व्योम *_b)
{
	स्थिर काष्ठा object_entry *a = *(काष्ठा object_entry **)_a;
	स्थिर काष्ठा object_entry *b = *(काष्ठा object_entry **)_b;
	स्थिर क्रमागत object_type a_type = oe_type(a);
	स्थिर क्रमागत object_type b_type = oe_type(b);
	स्थिर अचिन्हित दीर्घ a_आकार = आकार_3(a);
	स्थिर अचिन्हित दीर्घ b_आकार = आकार_3(b);

	अगर (a_type > b_type)
		वापस -1;
	अगर (a_type < b_type)
		वापस 1;
	अगर (a->hash > b->hash)
		वापस -1;
	अगर (a->hash < b->hash)
		वापस 1;
	अगर (a->preferred_base > b->preferred_base)
		वापस -1;
	अगर (a->preferred_base < b->preferred_base)
		वापस 1;
	अगर (use_delta_islands) {
		स्थिर पूर्णांक island_cmp = island_delta_cmp(&a->idx.oid, &b->idx.oid);
		अगर (island_cmp)
			वापस island_cmp;
	}
	अगर (a_आकार > b_आकार)
		वापस -1;
	अगर (a_आकार < b_आकार)
		वापस 1;
	वापस a < b ? -1 : (a > b);  /* newest first */
}

काष्ठा unpacked {
	काष्ठा object_entry *entry;
	व्योम *तथ्य;
	काष्ठा delta_सूचकांक *सूचकांक;
	अचिन्हित depth;
};

अटल पूर्णांक delta_cacheable(माप_प्रकार src_आकार, माप_प्रकार trg_आकार,
			   माप_प्रकार delta_आकार_2)
{
	अगर (max_delta_cache_आकार && delta_cache_आकार + delta_आकार_2 > max_delta_cache_आकार)
		वापस 0;

	अगर (delta_आकार_2 < cache_max_small_delta_आकार)
		वापस 1;

	/* cache delta, if objects are large enough compared to delta size */
	अगर ((src_आकार >> 20) + (trg_आकार >> 21) > (delta_आकार_2 >> 10))
		वापस 1;

	वापस 0;
}

/* Protect delta_cache_size */
अटल pthread_mutex_t cache_mutex;
#घोषणा cache_lock()		pthread_mutex_lock(&cache_mutex)
#घोषणा cache_unlock()		pthread_mutex_unlock(&cache_mutex)

/*
 * Protect object list partitioning (e.g. struct thread_param) and
 * progress_state
 */
अटल pthread_mutex_t progress_mutex;
#घोषणा progress_lock()		pthread_mutex_lock(&progress_mutex)
#घोषणा progress_unlock()	pthread_mutex_unlock(&progress_mutex)

/*
 * Access to struct object_entry is unprotected since each thread owns
 * a portion of the main object list. Just don't access object entries
 * ahead in the list because they can be stolen and would need
 * progress_mutex for protection.
 */

अटल अंतरभूत पूर्णांक oe_आकार_less_than(काष्ठा packing_तथ्य *pack,
				    स्थिर काष्ठा object_entry *lhs,
				    माप_प्रकार rhs)
{
	अगर (lhs->आकार_valid)
		वापस lhs->आकार_2 < rhs;
	अगर (rhs < pack->oe_आकार_limit) /* rhs < 2^x <= lhs ? */
		वापस 0;
	वापस oe_get_आकार_slow(pack, lhs) < rhs;
}

अटल अंतरभूत व्योम oe_set_वृक्ष_depth(काष्ठा packing_तथ्य *pack,
				     काष्ठा object_entry *e,
				     अचिन्हित पूर्णांक वृक्ष_depth)
{
	अगर (!pack->वृक्ष_depth)
		CALLOC_ARRAY(pack->वृक्ष_depth, pack->nr_alloc);
	pack->वृक्ष_depth[e - pack->objects] = वृक्ष_depth;
}

अटल व्योम record_वृक्ष_depth(स्थिर काष्ठा object_id *oid, स्थिर अक्षर *name)
{
	स्थिर अक्षर *p;
	अचिन्हित depth;
	काष्ठा object_entry *ent;

	/* the empty string is a root tree, which is depth 0 */
	depth = *name ? 1 : 0;
	क्रम (p = म_अक्षर(name, '/'); p; p = म_अक्षर(p + 1, '/'))
		depth++;

	ent = packlist_find(&to_pack, oid);
	अगर (ent && depth > oe_वृक्ष_depth(&to_pack, ent))
		oe_set_वृक्ष_depth(&to_pack, ent, depth);
}

/*
 * Return the size of the object without doing any delta
 * reconstruction (so non-deltas are true object sizes, but deltas
 * return the size of the delta data).
 */
माप_प्रकार oe_get_आकार_slow(काष्ठा packing_तथ्य *pack,
			स्थिर काष्ठा object_entry *e)
{
	काष्ठा packed_git *p;
	काष्ठा pack_window *w_curs;
	अचिन्हित अक्षर *buf;
	क्रमागत object_type type;
	माप_प्रकार used, avail, आकार;

	अगर (e->type_ != OBJ_OFS_DELTA && e->type_ != OBJ_REF_DELTA) {
		माप_प्रकार sz;
		packing_तथ्य_lock(&to_pack);
		अगर (odb_read_object_info(the_repository->objects,
					 &e->idx.oid, &sz) < 0)
			die(_("unable to get size of %s"),
			    oid_to_hex(&e->idx.oid));
		packing_तथ्य_unlock(&to_pack);
		वापस sz;
	}

	p = oe_in_pack(pack, e);
	अगर (!p)
		BUG("when e->type is a delta, it must belong to a pack");

	packing_तथ्य_lock(&to_pack);
	w_curs = शून्य;
	buf = use_pack(p, &w_curs, e->in_pack_offset, &avail);
	used = unpack_object_header_बफर(buf, avail, &type, &आकार);
	अगर (used == 0)
		die(_("unable to parse object header of %s"),
		    oid_to_hex(&e->idx.oid));

	unuse_pack(&w_curs);
	packing_तथ्य_unlock(&to_pack);
	वापस आकार;
}

अटल पूर्णांक try_delta(काष्ठा unpacked *trg, काष्ठा unpacked *src,
		     अचिन्हित max_depth, माप_प्रकार *mem_usage)
{
	काष्ठा object_entry *trg_entry = trg->entry;
	काष्ठा object_entry *src_entry = src->entry;
	अचिन्हित दीर्घ trg_आकार, src_आकार, sizediff, max_आकार, sz;
	माप_प्रकार delta_आकार_2;
	अचिन्हित ref_depth;
	क्रमागत object_type type;
	व्योम *delta_buf;

	/* Don't bother doing diffs between different types */
	अगर (oe_type(trg_entry) != oe_type(src_entry))
		वापस -1;

	/*
	 * We do not bother to try a delta that we discarded on an
	 * earlier try, but only when reusing delta data.  Note that
	 * src_entry that is marked as the preferred_base should always
	 * be considered, as even if we produce a suboptimal delta against
	 * it, we will still save the transfer cost, as we already know
	 * the other side has it and we won't send src_entry at all.
	 */
	अगर (reuse_delta && IN_PACK(trg_entry) &&
	    IN_PACK(trg_entry) == IN_PACK(src_entry) &&
	    !src_entry->preferred_base &&
	    trg_entry->in_pack_type != OBJ_REF_DELTA &&
	    trg_entry->in_pack_type != OBJ_OFS_DELTA)
		वापस 0;

	/* Let's not bust the allowed depth. */
	अगर (src->depth >= max_depth)
		वापस 0;

	/* Now some size filtering heuristics. */
	trg_आकार = आकार_3(trg_entry);
	अगर (!DELTA(trg_entry)) {
		max_आकार = trg_आकार/2 - the_hash_algo->rawsz;
		ref_depth = 1;
	} अन्यथा {
		max_आकार = DELTA_आकार(trg_entry);
		ref_depth = trg->depth;
	}
	max_आकार = (uint64_t)max_आकार * (max_depth - src->depth) /
						(max_depth - ref_depth + 1);
	अगर (max_आकार == 0)
		वापस 0;
	src_आकार = आकार_3(src_entry);
	sizediff = src_आकार < trg_आकार ? trg_आकार - src_आकार : 0;
	अगर (sizediff >= max_आकार)
		वापस 0;
	अगर (trg_आकार < src_आकार / 32)
		वापस 0;

	अगर (!in_same_island(&trg->entry->idx.oid, &src->entry->idx.oid))
		वापस 0;

	/* Load data if not already done */
	अगर (!trg->तथ्य) {
		माप_प्रकार sz_st = 0;
		packing_तथ्य_lock(&to_pack);
		trg->तथ्य = odb_read_object(the_repository->objects,
					    &trg_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_आकार_t_to_ulong(sz_st);
		packing_तथ्य_unlock(&to_pack);
		अगर (!trg->तथ्य)
			die(_("object %s cannot be read"),
			    oid_to_hex(&trg_entry->idx.oid));
		अगर (sz != trg_आकार)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&trg_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)trg_आकार);
		*mem_usage += sz;
	}
	अगर (!src->तथ्य) {
		माप_प्रकार sz_st = 0;
		packing_तथ्य_lock(&to_pack);
		src->तथ्य = odb_read_object(the_repository->objects,
					    &src_entry->idx.oid, &type,
					    &sz_st);
		sz = cast_आकार_t_to_ulong(sz_st);
		packing_तथ्य_unlock(&to_pack);
		अगर (!src->तथ्य) {
			अगर (src_entry->preferred_base) {
				अटल पूर्णांक warned = 0;
				अगर (!warned++)
					warning(_("object %s cannot be read"),
						oid_to_hex(&src_entry->idx.oid));
				/*
				 * Those objects are not included in the
				 * resulting pack.  Be resilient and ignore
				 * them if they can't be read, in case the
				 * pack could be created nevertheless.
				 */
				वापस 0;
			}
			die(_("object %s cannot be read"),
			    oid_to_hex(&src_entry->idx.oid));
		}
		अगर (sz != src_आकार)
			die(_("object %s inconsistent object length (%"PRIuMAX" vs %"PRIuMAX")"),
			    oid_to_hex(&src_entry->idx.oid), (uintmax_t)sz,
			    (uintmax_t)src_आकार);
		*mem_usage += sz;
	}
	अगर (!src->सूचकांक) {
		src->सूचकांक = create_delta_सूचकांक(src->तथ्य, src_आकार);
		अगर (!src->सूचकांक) {
			अटल पूर्णांक warned = 0;
			अगर (!warned++)
				warning(_("suboptimal pack - out of memory"));
			वापस 0;
		}
		*mem_usage += sizeof_delta_सूचकांक(src->सूचकांक);
	}

	delta_buf = create_delta(src->सूचकांक, trg->तथ्य, trg_आकार, &delta_आकार_2, max_आकार);
	अगर (!delta_buf)
		वापस 0;

	अगर (DELTA(trg_entry)) {
		/* Prefer only shallower same-sized deltas. */
		अगर (delta_आकार_2 == DELTA_आकार(trg_entry) &&
		    src->depth + 1 >= trg->depth) {
			मुक्त(delta_buf);
			वापस 0;
		}
	}

	/*
	 * Handle memory allocation outside of the cache
	 * accounting lock.  Compiler will optimize the strangeness
	 * away when NO_PTHREADS is defined.
	 */
	मुक्त(trg_entry->delta_तथ्य);
	cache_lock();
	अगर (trg_entry->delta_तथ्य) {
		delta_cache_आकार -= DELTA_आकार(trg_entry);
		trg_entry->delta_तथ्य = शून्य;
	}
	अगर (delta_cacheable(src_आकार, trg_आकार, delta_आकार_2)) {
		delta_cache_आकार += delta_आकार_2;
		cache_unlock();
		trg_entry->delta_तथ्य = xrealloc(delta_buf, delta_आकार_2);
	} अन्यथा {
		cache_unlock();
		मुक्त(delta_buf);
	}

	SET_DELTA(trg_entry, src_entry);
	SET_DELTA_आकार(trg_entry, delta_आकार_2);
	trg->depth = src->depth + 1;

	वापस 1;
}

अटल अचिन्हित पूर्णांक check_delta_limit(काष्ठा object_entry *me, अचिन्हित पूर्णांक n)
{
	काष्ठा object_entry *child = DELTA_CHILD(me);
	अचिन्हित पूर्णांक m = n;
	जबतक (child) {
		स्थिर अचिन्हित पूर्णांक c = check_delta_limit(child, n + 1);
		अगर (m < c)
			m = c;
		child = DELTA_SIBLING(child);
	}
	वापस m;
}

अटल माप_प्रकार free_unpacked(काष्ठा unpacked *n)
{
	माप_प्रकार freed_mem = sizeof_delta_सूचकांक(n->सूचकांक);
	free_delta_सूचकांक(n->सूचकांक);
	n->सूचकांक = शून्य;
	अगर (n->तथ्य) {
		freed_mem += आकार_3(n->entry);
		FREE_AND_NULL(n->तथ्य);
	}
	n->entry = शून्य;
	n->depth = 0;
	वापस freed_mem;
}

अटल व्योम find_deltas(काष्ठा object_entry **list, अचिन्हित *list_आकार,
			पूर्णांक window, पूर्णांक depth, अचिन्हित *processed)
{
	uint32_t i, idx = 0, गिनती = 0;
	काष्ठा unpacked *array;
	माप_प्रकार mem_usage = 0;

	CALLOC_ARRAY(array, window);

	क्रम (;;) {
		काष्ठा object_entry *entry;
		काष्ठा unpacked *n = array + idx;
		पूर्णांक j, max_depth, best_base = -1;

		progress_lock();
		अगर (!*list_आकार) {
			progress_unlock();
			अवरोध;
		}
		entry = *list++;
		(*list_आकार)--;
		अगर (!entry->preferred_base) {
			(*processed)++;
			display_progress(progress_state, *processed);
		}
		progress_unlock();

		mem_usage -= free_unpacked(n);
		n->entry = entry;

		जबतक (window_memory_limit &&
		       mem_usage > window_memory_limit &&
		       गिनती > 1) {
			स्थिर uint32_t पुच्छ = (idx + window - गिनती) % window;
			mem_usage -= free_unpacked(array + पुच्छ);
			गिनती--;
		}

		/* We do not compute delta to *create* objects we are not
		 * going to pack.
		 */
		अगर (entry->preferred_base)
			जाओ अग्र;

		/*
		 * If the current object is at pack edge, take the depth the
		 * objects that depend on the current object into account
		 * otherwise they would become too deep.
		 */
		max_depth = depth;
		अगर (DELTA_CHILD(entry)) {
			max_depth -= check_delta_limit(entry, 0);
			अगर (max_depth <= 0)
				जाओ अग्र;
		}

		j = window;
		जबतक (--j > 0) {
			पूर्णांक ret;
			uint32_t other_idx = idx + j;
			काष्ठा unpacked *m;
			अगर (other_idx >= window)
				other_idx -= window;
			m = array + other_idx;
			अगर (!m->entry)
				अवरोध;
			ret = try_delta(n, m, max_depth, &mem_usage);
			अगर (ret < 0)
				अवरोध;
			अन्यथा अगर (ret > 0)
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
		अगर (entry->delta_तथ्य && !pack_to_stdout) {
			अचिन्हित दीर्घ आकार;

			आकार = do_compress(&entry->delta_तथ्य, DELTA_आकार(entry));
			अगर (आकार < (1U << OE_Z_DELTA_BITS)) {
				entry->z_delta_आकार = आकार;
				cache_lock();
				delta_cache_आकार -= DELTA_आकार(entry);
				delta_cache_आकार += entry->z_delta_आकार;
				cache_unlock();
			} अन्यथा {
				FREE_AND_NULL(entry->delta_तथ्य);
				entry->z_delta_आकार = 0;
			}
		}

		/* if we made n a delta, and if n is already at max
		 * depth, leaving it in the window is pointless.  we
		 * should evict it first.
		 */
		अगर (DELTA(entry) && max_depth <= n->depth)
			जारी;

		/*
		 * Move the best delta base up in the window, after the
		 * currently deltified object, to keep it longer.  It will
		 * be the first base object to be attempted next.
		 */
		अगर (DELTA(entry)) {
			काष्ठा unpacked swap = array[best_base];
			पूर्णांक dist = (window + idx - best_base) % window;
			पूर्णांक dst = best_base;
			जबतक (dist--) {
				पूर्णांक src = (dst + 1) % window;
				array[dst] = array[src];
				dst = src;
			}
			array[dst] = swap;
		}

		अग्र:
		idx++;
		अगर (गिनती + 1 < window)
			गिनती++;
		अगर (idx >= window)
			idx = 0;
	}

	क्रम (i = 0; i < window; ++i) {
		free_delta_सूचकांक(array[i].सूचकांक);
		मुक्त(array[i].तथ्य);
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

काष्ठा thread_params {
	pthread_t thread;
	काष्ठा object_entry **list;
	काष्ठा packing_region *regions;
	अचिन्हित list_आकार;
	अचिन्हित remaining;
	पूर्णांक window;
	पूर्णांक depth;
	पूर्णांक working;
	पूर्णांक तथ्य_ready;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	अचिन्हित *processed;
};

अटल pthread_cond_t progress_cond;

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
अटल व्योम init_threaded_खोज(व्योम)
{
	pthread_mutex_init(&cache_mutex, शून्य);
	pthread_mutex_init(&progress_mutex, शून्य);
	pthread_cond_init(&progress_cond, शून्य);
}

अटल व्योम cleanup_threaded_खोज(व्योम)
{
	pthread_cond_destroy(&progress_cond);
	pthread_mutex_destroy(&cache_mutex);
	pthread_mutex_destroy(&progress_mutex);
}

अटल व्योम *threaded_find_deltas(व्योम *arg)
{
	काष्ठा thread_params *me = arg;

	progress_lock();
	जबतक (me->remaining) {
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
		जबतक (!me->तथ्य_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->तथ्य_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	वापस शून्य;
}

अटल व्योम ll_find_deltas(काष्ठा object_entry **list, अचिन्हित list_आकार,
			   पूर्णांक window, पूर्णांक depth, अचिन्हित *processed)
{
	काष्ठा thread_params *p;
	पूर्णांक i, ret, active_threads = 0;

	init_threaded_खोज();

	अगर (delta_खोज_threads <= 1) {
		find_deltas(list, &list_आकार, window, depth, processed);
		cleanup_threaded_खोज();
		वापस;
	}
	अगर (progress > pack_to_stdout)
		fprintf_ln(मानक_त्रुटि, _("Delta compression using up to %d threads"),
			   delta_खोज_threads);
	CALLOC_ARRAY(p, delta_खोज_threads);

	/* Partition the work amongst work threads. */
	क्रम (i = 0; i < delta_खोज_threads; i++) {
		अचिन्हित sub_आकार = list_आकार / (delta_खोज_threads - i);

		/* don't use too small segments or no deltas will be found */
		अगर (sub_आकार < 2*window && i+1 < delta_खोज_threads)
			sub_आकार = 0;

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = processed;
		p[i].working = 1;
		p[i].तथ्य_ready = 0;

		/* try to split chunks on "path" boundaries */
		जबतक (sub_आकार && sub_आकार < list_आकार &&
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
	क्रम (i = 0; i < delta_खोज_threads; i++) {
		अगर (!p[i].list_आकार)
			जारी;
		pthread_mutex_init(&p[i].mutex, शून्य);
		pthread_cond_init(&p[i].cond, शून्य);
		ret = pthread_create(&p[i].thread, शून्य,
				     threaded_find_deltas, &p[i]);
		अगर (ret)
			die(_("unable to create thread: %s"), म_त्रुटि(ret));
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
	जबतक (active_threads) {
		काष्ठा thread_params *target = शून्य;
		काष्ठा thread_params *victim = शून्य;
		अचिन्हित sub_आकार = 0;

		progress_lock();
		क्रम (;;) {
			क्रम (i = 0; !target && i < delta_खोज_threads; i++)
				अगर (!p[i].working)
					target = &p[i];
			अगर (target)
				अवरोध;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		क्रम (i = 0; i < delta_खोज_threads; i++)
			अगर (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		अगर (victim) {
			sub_आकार = victim->remaining / 2;
			list = victim->list + victim->list_आकार - sub_आकार;
			जबतक (sub_आकार && list[0]->hash &&
			       list[0]->hash == list[-1]->hash) {
				list++;
				sub_आकार--;
			}
			अगर (!sub_आकार) {
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
		progress_unlock();

		pthread_mutex_lock(&target->mutex);
		target->तथ्य_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		अगर (!sub_आकार) {
			pthread_join(target->thread, शून्य);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_खोज();
	मुक्त(p);
}

अटल पूर्णांक obj_is_packed(स्थिर काष्ठा object_id *oid)
{
	वापस packlist_find(&to_pack, oid) ||
		(reuse_packfile_bitmap &&
		 bitmap_walk_contains(bitmap_git, reuse_packfile_bitmap, oid));
}

अटल व्योम add_tag_chain(स्थिर काष्ठा object_id *oid)
{
	काष्ठा tag *tag;

	/*
	 * We catch duplicates already in add_object_entry(), but we'd
	 * prefer to do this extra check to avoid having to parse the
	 * tag at all if we already know that it's being packed (e.g., if
	 * it was included via bitmaps, we would not have parsed it
	 * previously).
	 */
	अगर (obj_is_packed(oid))
		वापस;

	tag = lookup_tag(the_repository, oid);
	जबतक (1) {
		अगर (!tag || parse_tag(the_repository, tag) || !tag->tagged)
			die(_("unable to pack objects reachable from tag %s"),
			    oid_to_hex(oid));

		add_object_entry(&tag->object.oid, OBJ_TAG, शून्य, 0);

		अगर (tag->tagged->type != OBJ_TAG)
			वापस;

		tag = (काष्ठा tag *)tag->tagged;
	}
}

अटल पूर्णांक add_ref_tag(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य UNUSED)
{
	काष्ठा object_id peeled;

	अगर (!reference_get_peeled_oid(the_repository, ref, &peeled) &&
	    obj_is_packed(&peeled))
		add_tag_chain(ref->oid);
	वापस 0;
}

अटल पूर्णांक should_attempt_deltas(काष्ठा object_entry *entry)
{
	अगर (DELTA(entry))
		/* This happens if we decided to reuse existing
		 * delta from a pack. "reuse_delta &&" is implied.
		 */
		वापस 0;

	अगर (!entry->type_valid ||
	    oe_आकार_less_than(&to_pack, entry, 50))
		वापस 0;

	अगर (entry->no_try_delta)
		वापस 0;

	अगर (!entry->preferred_base) {
		अगर (oe_type(entry) < 0)
			die(_("unable to get type of object %s"),
				oid_to_hex(&entry->idx.oid));
	} अन्यथा अगर (oe_type(entry) < 0) {
		/*
		 * This object is not found, but we
		 * don't have to include it anyway.
		 */
		वापस 0;
	}

	वापस 1;
}

अटल व्योम find_deltas_for_region(काष्ठा object_entry *list,
				   काष्ठा packing_region *region,
				   अचिन्हित पूर्णांक *processed)
{
	काष्ठा object_entry **delta_list;
	अचिन्हित पूर्णांक delta_list_nr = 0;

	ALLOC_ARRAY(delta_list, region->nr);
	क्रम (माप_प्रकार i = 0; i < region->nr; i++) {
		काष्ठा object_entry *entry = list + region->start + i;
		अगर (should_attempt_deltas(entry))
			delta_list[delta_list_nr++] = entry;
	}

	QSORT(delta_list, delta_list_nr, type_आकार_sort);
	find_deltas(delta_list, &delta_list_nr, window, depth, processed);
	मुक्त(delta_list);
}

अटल व्योम find_deltas_by_region(काष्ठा object_entry *list,
				  काष्ठा packing_region *regions,
				  माप_प्रकार start, माप_प्रकार nr)
{
	अचिन्हित पूर्णांक processed = 0;
	माप_प्रकार progress_nr;

	अगर (!nr)
		वापस;

	progress_nr = regions[nr - 1].start + regions[nr - 1].nr;

	अगर (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);

	जबतक (nr--)
		find_deltas_for_region(list,
				       &regions[start++],
				       &processed);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

अटल व्योम *threaded_find_deltas_by_path(व्योम *arg)
{
	काष्ठा thread_params *me = arg;

	progress_lock();
	जबतक (me->remaining) {
		जबतक (me->remaining) {
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
		जबतक (!me->तथ्य_ready)
			pthread_cond_wait(&me->cond, &me->mutex);
		me->तथ्य_ready = 0;
		pthread_mutex_unlock(&me->mutex);

		progress_lock();
	}
	progress_unlock();
	/* leave ->working 1 so that this doesn't get more work assigned */
	वापस शून्य;
}

अटल व्योम ll_find_deltas_by_region(काष्ठा object_entry *list,
				     काष्ठा packing_region *regions,
				     uint32_t start, uint32_t nr)
{
	काष्ठा thread_params *p;
	पूर्णांक i, ret, active_threads = 0;
	अचिन्हित पूर्णांक processed = 0;
	uint32_t progress_nr;
	init_threaded_खोज();

	अगर (!nr)
		वापस;

	progress_nr =  regions[nr - 1].start + regions[nr - 1].nr;
	अगर (delta_खोज_threads <= 1) {
		find_deltas_by_region(list, regions, start, nr);
		cleanup_threaded_खोज();
		वापस;
	}

	अगर (progress > pack_to_stdout)
		fprintf_ln(मानक_त्रुटि,
			   Q_("Path-based delta compression using up to %d thread",
			      "Path-based delta compression using up to %d threads",
			      delta_खोज_threads),
			   delta_खोज_threads);
	CALLOC_ARRAY(p, delta_खोज_threads);

	अगर (progress)
		progress_state = start_progress(the_repository,
						_("Compressing objects by path"),
						progress_nr);
	/* Partition the work amongst work threads. */
	क्रम (i = 0; i < delta_खोज_threads; i++) {
		अचिन्हित sub_आकार = nr / (delta_खोज_threads - i);

		p[i].window = window;
		p[i].depth = depth;
		p[i].processed = &processed;
		p[i].working = 1;
		p[i].तथ्य_ready = 0;

		p[i].regions = regions;
		p[i].list_आकार = sub_आकार;
		p[i].remaining = sub_आकार;

		regions += sub_आकार;
		nr -= sub_आकार;
	}

	/* Start work threads. */
	क्रम (i = 0; i < delta_खोज_threads; i++) {
		अगर (!p[i].list_आकार)
			जारी;
		pthread_mutex_init(&p[i].mutex, शून्य);
		pthread_cond_init(&p[i].cond, शून्य);
		ret = pthread_create(&p[i].thread, शून्य,
				     threaded_find_deltas_by_path, &p[i]);
		अगर (ret)
			die(_("unable to create thread: %s"), म_त्रुटि(ret));
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
	जबतक (active_threads) {
		काष्ठा thread_params *target = शून्य;
		काष्ठा thread_params *victim = शून्य;
		अचिन्हित sub_आकार = 0;

		progress_lock();
		क्रम (;;) {
			क्रम (i = 0; !target && i < delta_खोज_threads; i++)
				अगर (!p[i].working)
					target = &p[i];
			अगर (target)
				अवरोध;
			pthread_cond_wait(&progress_cond, &progress_mutex);
		}

		क्रम (i = 0; i < delta_खोज_threads; i++)
			अगर (p[i].remaining > 2*window &&
			    (!victim || victim->remaining < p[i].remaining))
				victim = &p[i];
		अगर (victim) {
			sub_आकार = victim->remaining / 2;
			target->regions = victim->regions + victim->remaining - sub_आकार;
			victim->list_आकार -= sub_आकार;
			victim->remaining -= sub_आकार;
		}
		target->list_आकार = sub_आकार;
		target->remaining = sub_आकार;
		target->working = 1;
		progress_unlock();

		pthread_mutex_lock(&target->mutex);
		target->तथ्य_ready = 1;
		pthread_cond_signal(&target->cond);
		pthread_mutex_unlock(&target->mutex);

		अगर (!sub_आकार) {
			pthread_join(target->thread, शून्य);
			pthread_cond_destroy(&target->cond);
			pthread_mutex_destroy(&target->mutex);
			active_threads--;
		}
	}
	cleanup_threaded_खोज();
	मुक्त(p);

	display_progress(progress_state, progress_nr);
	stop_progress(&progress_state);
}

अटल व्योम prepare_pack(पूर्णांक window, पूर्णांक depth)
{
	काष्ठा object_entry **delta_list;
	uint32_t i, nr_deltas;
	अचिन्हित n;

	अगर (use_delta_islands)
		resolve_वृक्ष_islands(the_repository, progress, &to_pack);

	get_object_details();

	/*
	 * If we're locally repacking then we need to be doubly careful
	 * from now on in order to make sure no stealth corruption gets
	 * propagated to the new pack.  Clients receiving streamed packs
	 * should validate everything they get anyway so no need to incur
	 * the additional cost here in that case.
	 */
	अगर (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	अगर (!to_pack.nr_objects || !window || !depth)
		वापस;

	अगर (path_walk)
		ll_find_deltas_by_region(to_pack.objects, to_pack.regions,
					 0, to_pack.nr_regions);

	ALLOC_ARRAY(delta_list, to_pack.nr_objects);
	nr_deltas = n = 0;

	क्रम (i = 0; i < to_pack.nr_objects; i++) {
		काष्ठा object_entry *entry = to_pack.objects + i;

		अगर (!should_attempt_deltas(entry))
			जारी;

		अगर (!entry->preferred_base)
			nr_deltas++;

		delta_list[n++] = entry;
	}

	अगर (nr_deltas && n > 1) {
		अचिन्हित nr_done = 0;

		अगर (progress)
			progress_state = start_progress(the_repository,
							_("Compressing objects"),
							nr_deltas);
		QSORT(delta_list, n, type_आकार_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		अगर (nr_done != nr_deltas)
			die(_("inconsistency with delta count"));
	}
	मुक्त(delta_list);
}

अटल पूर्णांक git_pack_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			   स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(k, "pack.window")) {
		window = git_config_int(k, v, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.windowmemory")) {
		window_memory_limit = git_config_ulong(k, v, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.depth")) {
		depth = git_config_int(k, v, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.deltacachesize")) {
		max_delta_cache_आकार = git_config_आकार_t(k, v, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.deltacachelimit")) {
		cache_max_small_delta_आकार = git_config_int(k, v, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.writebitmaphashcache")) {
		अगर (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_HASH_CACHE;
		अन्यथा
			write_bitmap_options &= ~BITMAP_OPT_HASH_CACHE;
	}

	अगर (!म_भेद(k, "pack.writebitmaplookuptable")) {
		अगर (git_config_bool(k, v))
			write_bitmap_options |= BITMAP_OPT_LOOKUP_TABLE;
		अन्यथा
			write_bitmap_options &= ~BITMAP_OPT_LOOKUP_TABLE;
	}

	अगर (!म_भेद(k, "pack.usebitmaps")) {
		use_bitmap_सूचकांक_default = git_config_bool(k, v);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.allowpackreuse")) {
		पूर्णांक res = git_parse_maybe_bool_text(v);
		अगर (res < 0) {
			अगर (!strcasecmp(v, "single"))
				allow_pack_reuse = SINGLE_PACK_REUSE;
			अन्यथा अगर (!strcasecmp(v, "multi"))
				allow_pack_reuse = MULTI_PACK_REUSE;
			अन्यथा
				die(_("invalid pack.allowPackReuse value: '%s'"), v);
		} अन्यथा अगर (res) {
			allow_pack_reuse = SINGLE_PACK_REUSE;
		} अन्यथा {
			allow_pack_reuse = NO_PACK_REUSE;
		}
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.threads")) {
		delta_खोज_threads = git_config_int(k, v, ctx->kvi);
		अगर (delta_खोज_threads < 0)
			die(_("invalid number of threads specified (%d)"),
			    delta_खोज_threads);
		अगर (!HAVE_THREADS && delta_खोज_threads != 1) {
			warning(_("no threads support, ignoring %s"), k);
			delta_खोज_threads = 0;
		}
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.indexversion")) {
		pack_idx_opts.version = git_config_int(k, v, ctx->kvi);
		अगर (pack_idx_opts.version > 2)
			die(_("bad pack.indexVersion=%"PRIu32),
			    pack_idx_opts.version);
		वापस 0;
	}
	अगर (!म_भेद(k, "pack.writereverseindex")) {
		अगर (git_config_bool(k, v))
			pack_idx_opts.flags |= WRITE_REV;
		अन्यथा
			pack_idx_opts.flags &= ~WRITE_REV;
		वापस 0;
	}
	अगर (!म_भेद(k, "uploadpack.blobpackfileuri")) {
		काष्ठा configured_exclusion *ex;
		स्थिर अक्षर *oid_end, *pack_end;
		/*
		 * Stores the pack hash. This is not a true object ID, but is
		 * of the same form.
		 */
		काष्ठा object_id pack_hash;

		अगर (!v)
			वापस config_error_nonbool(k);

		ex = xmalloc(माप(*ex));
		अगर (parse_oid_hex(v, &ex->e.oid, &oid_end) ||
		    *oid_end != ' ' ||
		    parse_oid_hex(oid_end + 1, &pack_hash, &pack_end) ||
		    *pack_end != ' ')
			die(_("value of uploadpack.blobpackfileuri must be "
			      "of the form '<object-hash> <pack-hash> <uri>' (got '%s')"), v);
		अगर (oidmap_get(&configured_exclusions, &ex->e.oid))
			die(_("object already configured in another "
			      "uploadpack.blobpackfileuri (got '%s')"), v);
		ex->pack_hash_hex = xcalloc(1, pack_end - oid_end);
		स_नकल(ex->pack_hash_hex, oid_end + 1, pack_end - oid_end - 1);
		ex->uri = xstrdup(pack_end + 1);
		oidmap_put(&configured_exclusions, ex);
	}
	वापस git_default_config(k, v, ctx, cb);
}

/* Counters for trace2 output when in --stdin-packs mode. */
अटल पूर्णांक stdin_packs_found_nr;
अटल पूर्णांक stdin_packs_hints_nr;

अटल पूर्णांक add_object_entry_from_pack(स्थिर काष्ठा object_id *oid,
				      काष्ठा packed_git *p,
				      uint32_t pos,
				      व्योम *तथ्य_2)
{
	off_t ofs;
	काष्ठा object_info oi = OBJECT_INFO_INIT;
	क्रमागत object_type type = OBJ_NONE;

	display_progress(progress_state, ++nr_seen);

	अगर (have_duplicate_entry(oid, 0))
		वापस 0;

	stdin_packs_found_nr++;

	ofs = nth_packed_object_offset(p, pos);

	oi.typep = &type;
	अगर (packed_object_info(शून्य, p, ofs, &oi) < 0) {
		die(_("could not get type of object %s in pack %s"),
		    oid_to_hex(oid), p->pack_name);
	} अन्यथा अगर (type == OBJ_COMMIT) {
		काष्ठा rev_info *revs = तथ्य_2;
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
		add_pending_oid(revs, शून्य, oid, 0);
	}

	अगर (!want_object_in_pack(oid, 0, &p, &ofs))
		वापस 0;

	create_object_entry(oid, type, 0, 0, 0, p, ofs);

	वापस 0;
}

अटल व्योम show_object_pack_hint(काष्ठा object *object, स्थिर अक्षर *name,
				  व्योम *तथ्य)
{
	क्रमागत stdin_packs_mode mode = *(क्रमागत stdin_packs_mode *)तथ्य;
	अगर (mode == STDIN_PACKS_MODE_FOLLOW) {
		अगर (object->type == OBJ_BLOB &&
		    !odb_has_object(the_repository->objects, &object->oid, 0))
			वापस;
		add_object_entry(&object->oid, object->type, name, 0);
	} अन्यथा {
		काष्ठा object_entry *oe = packlist_find(&to_pack, &object->oid);
		अगर (!oe)
			वापस;

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

अटल व्योम show_commit_pack_hint(काष्ठा commit *commit, व्योम *तथ्य)
{
	क्रमागत stdin_packs_mode mode = *(क्रमागत stdin_packs_mode *)तथ्य;

	अगर (mode == STDIN_PACKS_MODE_FOLLOW) {
		show_object_pack_hint((काष्ठा object *)commit, "", तथ्य);
		वापस;
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
क्रमागत stdin_pack_info_kind {
	STDIN_PACK_INCLUDE = (1<<0),
	STDIN_PACK_EXCLUDE_CLOSED = (1<<1),
	STDIN_PACK_EXCLUDE_OPEN = (1<<2),
};

काष्ठा stdin_pack_info {
	काष्ठा packed_git *p;
	क्रमागत stdin_pack_info_kind kind;
};

अटल पूर्णांक pack_mtime_cmp(स्थिर व्योम *_a, स्थिर व्योम *_b)
{
	काष्ठा stdin_pack_info *a = ((स्थिर काष्ठा string_list_item*)_a)->util;
	काष्ठा stdin_pack_info *b = ((स्थिर काष्ठा string_list_item*)_b)->util;

	/*
	 * order packs by descending mtime so that objects are laid out
	 * roughly as newest-to-oldest
	 */
	अगर (a->p->mtime < b->p->mtime)
		वापस 1;
	अन्यथा अगर (b->p->mtime < a->p->mtime)
		वापस -1;
	अन्यथा
		वापस 0;
}

अटल पूर्णांक stdin_packs_include_check_obj(काष्ठा object *obj, व्योम *तथ्य UNUSED)
{
	वापस !has_object_kept_pack(to_pack.repo, &obj->oid,
				     KEPT_PACK_IN_CORE);
}

अटल पूर्णांक stdin_packs_include_check(काष्ठा commit *commit, व्योम *तथ्य)
{
	वापस stdin_packs_include_check_obj((काष्ठा object *)commit, तथ्य);
}

अटल व्योम stdin_packs_add_pack_entries(काष्ठा strmap *packs,
					 काष्ठा rev_info *revs)
{
	काष्ठा string_list कुंजियाँ = STRING_LIST_INIT_NODUP;
	काष्ठा string_list_item *item;
	काष्ठा hashmap_iter iter;
	काष्ठा strmap_entry *entry;

	strmap_for_each_entry(packs, &iter, entry) {
		काष्ठा stdin_pack_info *info = entry->मूल्य;
		अगर (!info->p)
			die(_("could not find pack '%s'"), entry->कुंजी);

		string_list_append(&कुंजियाँ, entry->कुंजी)->util = info;
	}

	/*
	 * Order packs by ascending mtime; use QSORT directly to access the
	 * string_list_item's ->util pointer, which string_list_sort() does not
	 * provide.
	 */
	QSORT(कुंजियाँ.वस्तुएँ, कुंजियाँ.nr, pack_mtime_cmp);

	for_each_string_list_item(item, &कुंजियाँ) {
		काष्ठा stdin_pack_info *info = item->util;

		अगर (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
			/*
			 * When open-excluded packs ("!") are present, stop
			 * the parent walk at closed-excluded ("^") packs.
			 * Objects behind a "^" boundary are guaranteed to
			 * have closure and should not be rescued.
			 */
			revs->include_check = stdin_packs_include_check;
			revs->include_check_obj = stdin_packs_include_check_obj;
		}

		अगर ((info->kind & STDIN_PACK_INCLUDE) ||
		    (info->kind & STDIN_PACK_EXCLUDE_OPEN))
			for_each_object_in_pack(info->p,
						add_object_entry_from_pack,
						revs,
						ODB_FOR_EACH_OBJECT_PACK_ORDER);
	}

	string_list_clear(&कुंजियाँ, 0);
}

अटल व्योम stdin_packs_read_निवेश(काष्ठा rev_info *revs,
				   क्रमागत stdin_packs_mode mode)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strmap packs = STRMAP_INIT;
	काष्ठा packed_git *p;

	जबतक (strbuf_getline(&buf, मानक_निवेश) != खातापूर्ण) {
		काष्ठा stdin_pack_info *info;
		क्रमागत stdin_pack_info_kind kind = STDIN_PACK_INCLUDE;
		स्थिर अक्षर *कुंजी = buf.buf;

		अगर (!*कुंजी)
			जारी;
		अन्यथा अगर (*कुंजी == '^')
			kind = STDIN_PACK_EXCLUDE_CLOSED;
		अन्यथा अगर (*कुंजी == '!' && mode == STDIN_PACKS_MODE_FOLLOW)
			kind = STDIN_PACK_EXCLUDE_OPEN;

		अगर (kind != STDIN_PACK_INCLUDE)
			कुंजी++;

		info = strmap_get(&packs, कुंजी);
		अगर (!info) {
			CALLOC_ARRAY(info, 1);
			strmap_put(&packs, कुंजी, info);
		}

		info->kind |= kind;

		strbuf_reset(&buf);
	}

	repo_for_each_pack(the_repository, p) {
		काष्ठा stdin_pack_info *info;

		info = strmap_get(&packs, pack_basename(p));
		अगर (!info)
			जारी;

		अगर (info->kind & STDIN_PACK_INCLUDE) {
			अगर (exclude_promisor_objects && p->pack_promisor)
				die(_("packfile %s is a promisor but --exclude-promisor-objects was given"), p->pack_name);

			/*
			 * Arguments we got on stdin may not even be
			 * packs. First check that to avoid segfaulting
			 * later on in e.g.  pack_mtime_cmp(), excluded
			 * packs are handled below.
			 */
			अगर (!is_pack_valid(p))
				die(_("packfile %s cannot be accessed"), p->pack_name);
		}

		अगर (info->kind & STDIN_PACK_EXCLUDE_CLOSED) {
			/*
			 * Marking excluded packs as kept in-core so
			 * that later calls to add_object_entry()
			 * discards any objects that are also found in
			 * excluded packs.
			 */
			p->pack_keep_in_core = 1;
		}

		अगर (info->kind & STDIN_PACK_EXCLUDE_OPEN) {
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

अटल व्योम add_unreachable_loose_objects(काष्ठा rev_info *revs);

अटल व्योम read_stdin_packs(काष्ठा repository *repo,
			     क्रमागत stdin_packs_mode mode, पूर्णांक rev_list_unpacked)
{
	पूर्णांक पूर्व_fetch_if_missing = repo->fetch_if_missing;
	काष्ठा rev_info revs;

	/*
	 * The revision walk may hit objects that are promised, only. As the
	 * walk is best-effort though we don't want to perform backfill fetches
	 * for them.
	 */
	repo->fetch_if_missing = 0;

	repo_init_revisions(repo, &revs, शून्य);
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
	revs.वृक्ष_objects = 1;
	revs.tag_objects = 1;
	revs.ignore_missing_links = 1;
	revs.exclude_promisor_objects = exclude_promisor_objects;

	/* avoids adding objects in excluded packs */
	ignore_packed_keep_in_core = 1;
	अगर (mode == STDIN_PACKS_MODE_FOLLOW) {
		/*
		 * In '--stdin-packs=follow' mode, additionally ignore
		 * objects in excluded-open packs to prevent them from
		 * appearing in the resulting pack.
		 */
		ignore_packed_keep_in_core_open = 1;
	}
	stdin_packs_read_निवेश(&revs, mode);
	अगर (rev_list_unpacked)
		add_unreachable_loose_objects(&revs);

	अगर (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	traverse_commit_list(&revs,
			     show_commit_pack_hint,
			     show_object_pack_hint,
			     &mode);

	release_revisions(&revs);

	trace2_तथ्य_intmax("pack-objects", the_repository, "stdin_packs_found",
			   stdin_packs_found_nr);
	trace2_तथ्य_intmax("pack-objects", the_repository, "stdin_packs_hints",
			   stdin_packs_hints_nr);

	repo->fetch_if_missing = पूर्व_fetch_if_missing;
}

अटल व्योम add_cruft_object_entry(स्थिर काष्ठा object_id *oid, क्रमागत object_type type,
				   काष्ठा packed_git *pack, off_t offset,
				   स्थिर अक्षर *name, uint32_t mtime)
{
	काष्ठा object_entry *entry;

	display_progress(progress_state, ++nr_seen);

	entry = packlist_find(&to_pack, oid);
	अगर (entry) {
		अगर (name) {
			entry->hash = pack_name_hash_fn(name);
			entry->no_try_delta = no_try_delta(name);
		}
	} अन्यथा {
		अगर (!want_object_in_pack_mtime(oid, 0, &pack, &offset, mtime))
			वापस;
		अगर (!pack && type == OBJ_BLOB) {
			काष्ठा odb_source_files *files =
				odb_source_files_downcast(the_repository->objects->source);
			पूर्णांक found = 0;

			क्रम (काष्ठा odb_files_dir *dir = files->dirs; !found && dir; dir = dir->अग्र)
				अगर (!odb_source_read_object_info(&dir->loose->base, oid, शून्य, 0, शून्य))
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
			अगर (!found)
				वापस;
		}

		entry = create_object_entry(oid, type, pack_name_hash_fn(name),
					    0, name && no_try_delta(name),
					    pack, offset);
	}

	अगर (mtime > oe_cruft_mtime(&to_pack, entry))
		oe_set_cruft_mtime(&to_pack, entry, mtime);
	वापस;
}

अटल व्योम show_cruft_object(काष्ठा object *obj, स्थिर अक्षर *name, व्योम *तथ्य UNUSED)
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
	add_cruft_object_entry(&obj->oid, obj->type, शून्य, 0, name, cruft_expiration);
}

अटल व्योम show_cruft_commit(काष्ठा commit *commit, व्योम *तथ्य)
{
	show_cruft_object((काष्ठा object*)commit, शून्य, तथ्य);
}

अटल पूर्णांक cruft_include_check_obj(काष्ठा object *obj, व्योम *तथ्य UNUSED)
{
	वापस !has_object_kept_pack(to_pack.repo, &obj->oid, KEPT_PACK_IN_CORE);
}

अटल पूर्णांक cruft_include_check(काष्ठा commit *commit, व्योम *तथ्य)
{
	वापस cruft_include_check_obj((काष्ठा object*)commit, तथ्य);
}

अटल व्योम set_cruft_mtime(स्थिर काष्ठा object *object,
			    काष्ठा packed_git *pack,
			    off_t offset, समय_प्रकार mtime)
{
	add_cruft_object_entry(&object->oid, object->type, pack, offset, शून्य,
			       mtime);
}

अटल व्योम mark_pack_kept_in_core(काष्ठा string_list *packs, अचिन्हित keep)
{
	काष्ठा string_list_item *item = शून्य;
	for_each_string_list_item(item, packs) {
		काष्ठा packed_git *p = item->util;
		अगर (!p)
			die(_("could not find pack '%s'"), item->string);
		अगर (p->is_cruft && keep)
			ignore_packed_keep_in_core_has_cruft = 1;
		p->pack_keep_in_core = keep;
	}
}

अटल व्योम add_objects_in_unpacked_packs(व्योम);

अटल व्योम enumerate_cruft_objects(व्योम)
{
	अगर (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);

	add_objects_in_unpacked_packs();
	add_unreachable_loose_objects(शून्य);

	stop_progress(&progress_state);
}

अटल व्योम enumerate_and_traverse_cruft_objects(काष्ठा string_list *fresh_packs)
{
	काष्ठा packed_git *p;
	काष्ठा rev_info revs;
	पूर्णांक ret;

	repo_init_revisions(the_repository, &revs, शून्य);

	revs.tag_objects = 1;
	revs.वृक्ष_objects = 1;
	revs.blob_objects = 1;

	revs.include_check = cruft_include_check;
	revs.include_check_obj = cruft_include_check_obj;

	revs.ignore_missing_links = 1;

	अगर (progress)
		progress_state = start_progress(the_repository,
						_("Enumerating cruft objects"), 0);
	ret = add_unseen_recent_objects_to_traversal(&revs, cruft_expiration,
						     set_cruft_mtime, 1);
	stop_progress(&progress_state);

	अगर (ret)
		die(_("unable to add cruft objects"));

	/*
	 * Re-mark only the fresh packs as kept so that objects in
	 * unknown packs do not halt the reachability traversal early.
	 */
	repo_for_each_pack(the_repository, p)
		p->pack_keep_in_core = 0;
	mark_pack_kept_in_core(fresh_packs, 1);

	अगर (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	अगर (progress)
		progress_state = start_progress(the_repository,
						_("Traversing cruft objects"), 0);
	nr_seen = 0;
	traverse_commit_list(&revs, show_cruft_commit, show_cruft_object, शून्य);

	stop_progress(&progress_state);
	release_revisions(&revs);
}

अटल व्योम read_cruft_objects(व्योम)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा string_list discard_packs = STRING_LIST_INIT_DUP;
	काष्ठा string_list fresh_packs = STRING_LIST_INIT_DUP;
	काष्ठा packed_git *p;

	ignore_packed_keep_in_core = 1;

	जबतक (strbuf_getline(&buf, मानक_निवेश) != खातापूर्ण) {
		अगर (!buf.len)
			जारी;

		अगर (*buf.buf == '-')
			string_list_append(&discard_packs, buf.buf + 1);
		अन्यथा
			string_list_append(&fresh_packs, buf.buf);
	}

	string_list_sort(&discard_packs);
	string_list_sort(&fresh_packs);

	repo_for_each_pack(the_repository, p) {
		स्थिर अक्षर *pack_name = pack_basename(p);
		काष्ठा string_list_item *item;

		item = string_list_lookup(&fresh_packs, pack_name);
		अगर (!item)
			item = string_list_lookup(&discard_packs, pack_name);

		अगर (item) {
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
			p->pack_keep_in_core = 1;
		}
	}

	mark_pack_kept_in_core(&fresh_packs, 1);
	mark_pack_kept_in_core(&discard_packs, 0);

	अगर (cruft_expiration)
		enumerate_and_traverse_cruft_objects(&fresh_packs);
	अन्यथा
		enumerate_cruft_objects();

	strbuf_release(&buf);
	string_list_clear(&discard_packs, 0);
	string_list_clear(&fresh_packs, 0);
}

अटल व्योम read_object_list_from_stdin(व्योम)
{
	अक्षर पंक्ति[GIT_MAX_HEXSZ + 1 + PATH_MAX + 2];
	काष्ठा object_id oid;
	स्थिर अक्षर *p;

	क्रम (;;) {
		अगर (!ख_माला_लो(पंक्ति, माप(पंक्ति), मानक_निवेश)) {
			अगर (ख_पूर्ण(मानक_निवेश))
				अवरोध;
			अगर (!ख_त्रुटि(मानक_निवेश))
				BUG("fgets returned NULL, not EOF, not error!");
			अगर (त्रुटि_सं != EINTR)
				die_errno("fgets");
			साफ_त्रुटि(मानक_निवेश);
			जारी;
		}
		अगर (पंक्ति[0] == '-') {
			अगर (get_oid_hex(पंक्ति+1, &oid))
				die(_("expected edge object ID, got garbage:\n %s"),
				    पंक्ति);
			add_preferred_base(&oid);
			जारी;
		}
		अगर (parse_oid_hex(पंक्ति, &oid, &p))
			die(_("expected object ID, got garbage:\n %s"), पंक्ति);

		add_preferred_base_object(p + 1);
		add_object_entry(&oid, OBJ_NONE, p + 1, 0);
	}
}

अटल व्योम show_commit(काष्ठा commit *commit, व्योम *तथ्य UNUSED)
{
	add_object_entry(&commit->object.oid, OBJ_COMMIT, शून्य, 0);

	अगर (write_bitmap_सूचकांक)
		सूचकांक_commit_for_bitmap(commit);

	अगर (use_delta_islands)
		propagate_island_marks(the_repository, commit);
}

अटल व्योम show_object(काष्ठा object *obj, स्थिर अक्षर *name,
			व्योम *तथ्य UNUSED)
{
	add_preferred_base_object(name);
	add_object_entry(&obj->oid, obj->type, name, 0);

	अगर (use_delta_islands)
		record_वृक्ष_depth(&obj->oid, name);
}

अटल व्योम show_object__ma_allow_any(काष्ठा object *obj, स्थिर अक्षर *name, व्योम *तथ्य)
{
	निश्चित(arg_missing_action == MA_ALLOW_ANY);

	/*
	 * Quietly ignore ALL missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	अगर (!odb_has_object(the_repository->objects, &obj->oid, 0))
		वापस;

	show_object(obj, name, तथ्य);
}

अटल व्योम show_object__ma_allow_promisor(काष्ठा object *obj, स्थिर अक्षर *name, व्योम *तथ्य)
{
	निश्चित(arg_missing_action == MA_ALLOW_PROMISOR);

	/*
	 * Quietly ignore EXPECTED missing objects.  This avoids problems with
	 * staging them now and getting an odd error later.
	 */
	अगर (!odb_has_object(the_repository->objects, &obj->oid, 0) &&
	    is_promisor_object(to_pack.repo, &obj->oid))
		वापस;

	show_object(obj, name, तथ्य);
}

अटल पूर्णांक option_parse_missing_action(स्थिर काष्ठा option *opt,
				       स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा repository *repo = opt->मूल्य;

	निश्चित(arg);
	निश्चित(!unset);

	अगर (!म_भेद(arg, "error")) {
		arg_missing_action = MA_ERROR;
		fn_show_object = show_object;
		वापस 0;
	}

	अगर (!म_भेद(arg, "allow-any")) {
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		वापस 0;
	}

	अगर (!म_भेद(arg, "allow-promisor")) {
		arg_missing_action = MA_ALLOW_PROMISOR;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_promisor;
		वापस 0;
	}

	die(_("invalid value for '%s': '%s'"), "--missing", arg);
	वापस 0;
}

अटल व्योम show_edge(काष्ठा commit *commit)
{
	add_preferred_base(&commit->object.oid);
}

अटल पूर्णांक add_object_in_unpacked_pack(स्थिर काष्ठा object_id *oid,
				       काष्ठा object_info *oi,
				       व्योम *तथ्य UNUSED)
{
	अगर (cruft) {
		add_cruft_object_entry(oid, OBJ_NONE, oi->source_infop->u.packed.pack,
				       oi->source_infop->u.packed.offset, शून्य,
				       *oi->mtimep);
	} अन्यथा {
		add_object_entry(oid, OBJ_NONE, "", 0);
	}
	वापस 0;
}

अटल व्योम add_objects_in_unpacked_packs(व्योम)
{
	काष्ठा odb_source_files *files =
		odb_source_files_downcast(to_pack.repo->objects->source);
	समय_प्रकार mtime;
	काष्ठा odb_for_each_object_options opts = {
		.flags = ODB_FOR_EACH_OBJECT_PACK_ORDER |
			 ODB_FOR_EACH_OBJECT_LOCAL_ONLY |
			 ODB_FOR_EACH_OBJECT_SKIP_IN_CORE_KEPT_PACKS |
			 ODB_FOR_EACH_OBJECT_SKIP_ON_DISK_KEPT_PACKS,
	};
	काष्ठा odb_source_info source_info;
	काष्ठा object_info oi = {
		.mtimep = &mtime,
		.source_infop = &source_info,
	};

	क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		अगर (!dir->local)
			जारी;

		अगर (odb_source_for_each_object(&dir->packed->base, &oi,
					       add_object_in_unpacked_pack, शून्य, &opts))
			die(_("cannot open pack index"));
	}
}

अटल पूर्णांक add_loose_object(स्थिर काष्ठा object_id *oid, स्थिर अक्षर *path,
			    व्योम *तथ्य)
{
	काष्ठा rev_info *revs = तथ्य;
	क्रमागत object_type type = odb_read_object_info(the_repository->objects, oid, शून्य);

	अगर (type < 0) {
		warning(_("loose object at %s could not be examined"), path);
		वापस 0;
	}

	अगर (cruft) {
		काष्ठा stat st;
		अगर (stat(path, &st) < 0) {
			अगर (त्रुटि_सं == ENOENT)
				वापस 0;
			वापस error_errno("unable to stat %s", oid_to_hex(oid));
		}

		add_cruft_object_entry(oid, type, शून्य, 0, शून्य,
				       st.st_mtime);
	} अन्यथा {
		add_object_entry(oid, type, "", 0);
	}

	अगर (revs && type == OBJ_COMMIT)
		add_pending_oid(revs, शून्य, oid, 0);

	वापस 0;
}

/*
 * We actually don't even have to worry about reachability here.
 * add_object_entry will weed out duplicates, so we just add every
 * loose object we find.
 */
अटल व्योम add_unreachable_loose_objects(काष्ठा rev_info *revs)
{
	for_each_loose_file_in_source(the_repository->objects->source,
				      add_loose_object, शून्य, शून्य, revs);
}

अटल पूर्णांक has_sha1_pack_kept_or_nonlocal(स्थिर काष्ठा object_id *oid)
{
	अटल काष्ठा packed_git *last_found = शून्य;
	काष्ठा packed_git *p;

	अगर (last_found && find_pack_entry_one(oid, last_found))
		वापस 1;

	repo_for_each_pack(the_repository, p) {
		/*
		 * We have already checked `last_found`, so there is no need to
		 * re-check here.
		 */
		अगर (p == last_found)
			जारी;

		अगर ((!p->pack_local || p->pack_keep || p->pack_keep_in_core) &&
		    find_pack_entry_one(oid, p)) {
			last_found = p;
			वापस 1;
		}
	}

	वापस 0;
}

/*
 * Store a list of sha1s that are should not be discarded
 * because they are either written too recently, or are
 * reachable from another object that was.
 *
 * This is filled by get_object_list.
 */
अटल काष्ठा oid_array recent_objects;

अटल पूर्णांक loosened_object_can_be_discarded(स्थिर काष्ठा object_id *oid,
					    timestamp_t mtime)
{
	अगर (!unpack_unreachable_expiration)
		वापस 0;
	अगर (mtime > unpack_unreachable_expiration)
		वापस 0;
	अगर (oid_array_lookup(&recent_objects, oid) >= 0)
		वापस 0;
	वापस 1;
}

अटल पूर्णांक force_object_loose(काष्ठा odb_source *source,
			      स्थिर काष्ठा object_id *oid,
			      स्थिर समय_प्रकार *mtime)
{
	काष्ठा odb_source_files *files = odb_source_files_downcast(source);
	स्थिर काष्ठा git_hash_algo *compat = source->odb->repo->compat_hash_algo;
	काष्ठा object_info oi = OBJECT_INFO_INIT;
	काष्ठा object_id compat_oid, *compat_oid_p = शून्य;
	क्रमागत object_type type;
	व्योम *buf = शून्य;
	माप_प्रकार len;
	पूर्णांक ret;

	क्रम (काष्ठा odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र)
		अगर (!odb_source_read_object_info(&dir->loose->base, oid, शून्य, 0, शून्य))
			वापस 0;

	oi.typep = &type;
	oi.sizep = &len;
	oi.contentp = &buf;
	अगर (odb_read_object_info_extended(source->odb, oid, &oi, 0)) {
		ret = error(_("cannot read object for %s"), oid_to_hex(oid));
		जाओ out;
	}

	अगर (compat) {
		अगर (repo_oid_to_algop(source->odb->repo, oid, compat, &compat_oid)) {
			ret = error(_("cannot map object %s to %s"),
				    oid_to_hex(oid), compat->name);
			जाओ out;
		}

		compat_oid_p = &compat_oid;
	}

	ret = odb_source_write_object(&files->dirs->loose->base, buf, len, type, oid,
				      compat_oid_p, mtime, 0);

out:
	मुक्त(buf);
	वापस ret;
}

अटल व्योम loosen_unused_packed_objects(व्योम)
{
	काष्ठा packed_git *p;
	uint32_t i;
	uint32_t loosened_objects_nr = 0;
	काष्ठा object_id oid;

	repo_for_each_pack(the_repository, p) {
		अगर (!p->pack_local || p->pack_keep || p->pack_keep_in_core)
			जारी;

		अगर (open_pack_सूचकांक(p))
			die(_("cannot open pack index"));

		क्रम (i = 0; i < p->अंक_objects; i++) {
			nth_packed_object_id(&oid, p, i);
			अगर (!packlist_find(&to_pack, &oid) &&
			    !has_sha1_pack_kept_or_nonlocal(&oid) &&
			    !loosened_object_can_be_discarded(&oid, p->mtime)) {
				अगर (force_object_loose(the_repository->objects->source,
						       &oid, &p->mtime))
					die(_("unable to force loose object"));
				loosened_objects_nr++;
			}
		}
	}

	trace2_तथ्य_intmax("pack-objects", the_repository,
			   "loosen_unused_packed_objects/loosened", loosened_objects_nr);
}

/*
 * This tracks any options which pack-reuse code expects to be on, or which a
 * reader of the pack might not understand, and which would therefore prevent
 * blind reuse of what we have on disk.
 */
अटल पूर्णांक pack_options_allow_reuse(व्योम)
{
	वापस allow_pack_reuse != NO_PACK_REUSE &&
	       pack_to_stdout &&
	       !ignore_packed_keep_on_disk &&
	       !ignore_packed_keep_in_core &&
	       (!local || !have_non_local_packs) &&
	       !incremental;
}

अटल पूर्णांक get_object_list_from_bitmap(काष्ठा rev_info *revs)
{
	अगर (!(bitmap_git = prepare_bitmap_walk(revs, 0)))
		वापस -1;

	/*
	 * For now, force the name-hash version to be 1 since that
	 * is the version implied by the bitmap format. Later, the
	 * format can include this version explicitly in its format,
	 * allowing readers to know the version that was used during
	 * the bitmap write.
	 */
	name_hash_version = 1;

	अगर (pack_options_allow_reuse())
		reuse_partial_packfile_from_bitmap(bitmap_git,
						   &reuse_packfiles,
						   &reuse_packfiles_nr,
						   &reuse_packfile_bitmap,
						   allow_pack_reuse == MULTI_PACK_REUSE);

	अगर (reuse_packfiles) {
		reuse_packfile_objects = bitmap_popcount(reuse_packfile_bitmap);
		अगर (!reuse_packfile_objects)
			BUG("expected non-empty reuse bitmap");

		nr_परिणाम += reuse_packfile_objects;
		nr_seen += reuse_packfile_objects;
		display_progress(progress_state, nr_seen);
	}

	traverse_bitmap_commit_list(bitmap_git, revs,
				    &add_object_entry_from_bitmap);
	वापस 0;
}

अटल व्योम record_recent_object(काष्ठा object *obj,
				 स्थिर अक्षर *name UNUSED,
				 व्योम *तथ्य UNUSED)
{
	oid_array_append(&recent_objects, &obj->oid);
}

अटल व्योम record_recent_commit(काष्ठा commit *commit, व्योम *तथ्य UNUSED)
{
	oid_array_append(&recent_objects, &commit->object.oid);
}

अटल पूर्णांक mark_bitmap_preferred_tip(स्थिर काष्ठा reference *ref, व्योम *तथ्य UNUSED)
{
	स्थिर काष्ठा object_id *maybe_peeled = ref->oid;
	काष्ठा object_id peeled;
	काष्ठा object *object;

	अगर (!reference_get_peeled_oid(the_repository, ref, &peeled))
		maybe_peeled = &peeled;

	object = parse_object_or_die(the_repository, maybe_peeled, ref->name);
	अगर (object->type == OBJ_COMMIT)
		object->flags |= NEEDS_BITMAP;

	वापस 0;
}

अटल अंतरभूत पूर्णांक is_oid_uninteresting(काष्ठा repository *repo,
				       काष्ठा object_id *oid)
{
	काष्ठा object *o = lookup_object(repo, oid);
	वापस !o || (o->flags & UNINTERESTING);
}

अटल पूर्णांक add_objects_by_path(स्थिर अक्षर *path,
			       काष्ठा oid_array *oids,
			       क्रमागत object_type type,
			       व्योम *तथ्य)
{
	माप_प्रकार oe_start = to_pack.nr_objects;
	माप_प्रकार oe_end;
	अचिन्हित पूर्णांक *processed = तथ्य;

	/*
	 * First, add all objects to the packing data, including the ones
	 * marked UNINTERESTING (translated to 'exclude') as they can be
	 * used as delta bases.
	 */
	क्रम (माप_प्रकार i = 0; i < oids->nr; i++) {
		पूर्णांक exclude;
		काष्ठा object_info oi = OBJECT_INFO_INIT;
		काष्ठा object_id *oid = &oids->oid[i];

		/* Skip objects that do not exist locally. */
		अगर ((exclude_promisor_objects || arg_missing_action != MA_ERROR) &&
		    odb_read_object_info_extended(the_repository->objects, oid, &oi,
						  OBJECT_INFO_FOR_PREFETCH) < 0)
			जारी;

		exclude = is_oid_uninteresting(the_repository, oid);

		अगर (exclude && !thin)
			जारी;

		add_object_entry(oid, type, path, exclude);

		अगर (type == OBJ_COMMIT) {
			काष्ठा commit *commit;

			अगर (!write_bitmap_सूचकांक && !use_delta_islands)
				जारी;

			commit = lookup_commit(the_repository, oid);
			अगर (!commit)
				die(_("could not find commit %s"), oid_to_hex(oid));
			अगर (write_bitmap_सूचकांक)
				सूचकांक_commit_for_bitmap(commit);
			/*
			 * Skip island propagation for boundary commits.
			 * The regular traversal's show_commit() is only
			 * called for interesting commits; matching that
			 * here keeps path-walk from doing extra work that
			 * would only be a no-op anyway (boundary commits
			 * are not in island_marks).
			 */
			अगर (use_delta_islands && !exclude)
				propagate_island_marks(the_repository, commit);
		} अन्यथा अगर (type == OBJ_वृक्ष && use_delta_islands) {
			record_वृक्ष_depth(oid, path);
		}
	}

	oe_end = to_pack.nr_objects;

	/* We can skip delta calculations if it is a no-op. */
	अगर (oe_end == oe_start || !window)
		वापस 0;

	ALLOC_GROW(to_pack.regions,
		   to_pack.nr_regions + 1,
		   to_pack.nr_regions_alloc);

	to_pack.regions[to_pack.nr_regions].start = oe_start;
	to_pack.regions[to_pack.nr_regions].nr = oe_end - oe_start;
	to_pack.nr_regions++;

	*processed += oids->nr;
	display_progress(progress_state, *processed);

	वापस 0;
}

अटल पूर्णांक get_object_list_path_walk(काष्ठा rev_info *revs)
{
	काष्ठा path_walk_info info = PATH_WALK_INFO_INIT;
	अचिन्हित पूर्णांक processed = 0;
	पूर्णांक परिणाम;

	info.revs = revs;
	info.path_fn = add_objects_by_path;
	info.path_fn_तथ्य = &processed;

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
	परिणाम = walk_objects_by_path(&info);
	trace2_region_leave("pack-objects", "path-walk", revs->repo);

	path_walk_info_clear(&info);

	वापस परिणाम;
}

अटल व्योम get_object_list(काष्ठा rev_info *revs, काष्ठा strvec *argv)
{
	काष्ठा setup_revision_opt s_r_opt = {
		.allow_exclude_promisor_objects = 1,
	};
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	अक्षर पंक्ति[1000];
	पूर्णांक flags = 0;
	पूर्णांक save_warning;

	save_commit_बफर = 0;
	setup_revisions_from_strvec(argv, revs, &s_r_opt);

	/* make sure shallows are read */
	is_repository_shallow(the_repository);

	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	जबतक (ख_माला_लो(पंक्ति, माप(पंक्ति), मानक_निवेश) != शून्य) {
		पूर्णांक len = म_माप(पंक्ति);
		अगर (len && पंक्ति[len - 1] == '\n')
			पंक्ति[--len] = 0;
		अगर (!len)
			अवरोध;
		अगर (*पंक्ति == '-') {
			अगर (!म_भेद(पंक्ति, "--not")) {
				flags ^= UNINTERESTING;
				write_bitmap_सूचकांक = 0;
				जारी;
			}
			अगर (starts_with(पंक्ति, "--shallow ")) {
				काष्ठा object_id oid;
				अगर (get_oid_hex(पंक्ति + 10, &oid))
					die("not an object name '%s'", पंक्ति + 10);
				register_shallow(the_repository, &oid);
				use_bitmap_सूचकांक = 0;
				जारी;
			}
			die(_("not a rev '%s'"), पंक्ति);
		}
		अगर (handle_revision_arg(पंक्ति, revs, flags, REVARG_CANNOT_BE_FILENAME))
			die(_("bad revision '%s'"), पंक्ति);
	}

	cfg->warn_on_object_refname_ambiguity = save_warning;

	अगर (use_bitmap_सूचकांक && !get_object_list_from_bitmap(revs))
		वापस;

	अगर (use_delta_islands)
		load_delta_islands(the_repository, progress);

	अगर (write_bitmap_सूचकांक)
		for_each_preferred_bitmap_tip(the_repository, mark_bitmap_preferred_tip,
					      शून्य);

	अगर (!fn_show_object)
		fn_show_object = show_object;

	अगर (path_walk) {
		अगर (get_object_list_path_walk(revs)) {
			warning(_("failed to pack objects via path-walk"));
			path_walk = 0;
		}
	}

	अगर (!path_walk) {
		अगर (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		mark_edges_uninteresting(revs, show_edge, sparse);
		traverse_commit_list(revs,
				show_commit, fn_show_object,
				शून्य);
	}

	अगर (unpack_unreachable_expiration) {
		revs->ignore_missing_links = 1;
		अगर (add_unseen_recent_objects_to_traversal(revs,
				unpack_unreachable_expiration, शून्य, 0))
			die(_("unable to add recent objects"));
		अगर (prepare_revision_walk(revs))
			die(_("revision walk setup failed"));
		traverse_commit_list(revs, record_recent_commit,
				     record_recent_object, शून्य);
	}

	अगर (keep_unreachable)
		add_objects_in_unpacked_packs();
	अगर (pack_loose_unreachable)
		add_unreachable_loose_objects(शून्य);
	अगर (unpack_unreachable)
		loosen_unused_packed_objects();

	oid_array_clear(&recent_objects);
}

अटल व्योम add_extra_kept_packs(स्थिर काष्ठा string_list *names)
{
	काष्ठा packed_git *p;

	अगर (!names->nr)
		वापस;

	repo_for_each_pack(the_repository, p) {
		स्थिर अक्षर *name = basename(p->pack_name);
		पूर्णांक i;

		अगर (!p->pack_local)
			जारी;

		क्रम (i = 0; i < names->nr; i++)
			अगर (!fspathcmp(name, names->वस्तुएँ[i].string))
				अवरोध;

		अगर (i < names->nr) {
			p->pack_keep_in_core = 1;
			ignore_packed_keep_in_core = 1;
			जारी;
		}
	}
}

अटल पूर्णांक option_parse_quiet(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			      पूर्णांक unset)
{
	पूर्णांक *val = opt->मूल्य;

	BUG_ON_OPT_ARG(arg);

	अगर (!unset)
		*val = 0;
	अन्यथा अगर (!*val)
		*val = 1;
	वापस 0;
}

अटल पूर्णांक option_parse_सूचकांक_version(स्थिर काष्ठा option *opt,
				      स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा pack_idx_option *popts = opt->मूल्य;
	अक्षर *c;
	स्थिर अक्षर *val = arg;

	BUG_ON_OPT_NEG(unset);

	popts->version = म_से_अदीर्घ(val, &c, 10);
	अगर (popts->version > 2)
		die(_("unsupported index version %s"), val);
	अगर (*c == ',' && c[1])
		popts->off32_limit = म_से_अदीर्घ(c+1, &c, 0);
	अगर (*c || popts->off32_limit & 0x80000000)
		die(_("bad index version '%s'"), val);
	वापस 0;
}

अटल पूर्णांक option_parse_unpack_unreachable(स्थिर काष्ठा option *opt UNUSED,
					   स्थिर अक्षर *arg, पूर्णांक unset)
{
	अगर (unset) {
		unpack_unreachable = 0;
		unpack_unreachable_expiration = 0;
	}
	अन्यथा {
		unpack_unreachable = 1;
		अगर (arg)
			unpack_unreachable_expiration = approxidate(arg);
	}
	वापस 0;
}

अटल पूर्णांक option_parse_cruft_expiration(स्थिर काष्ठा option *opt UNUSED,
					 स्थिर अक्षर *arg, पूर्णांक unset)
{
	अगर (unset) {
		cruft = 0;
		cruft_expiration = 0;
	} अन्यथा {
		cruft = 1;
		अगर (arg)
			cruft_expiration = approxidate(arg);
	}
	वापस 0;
}

अटल पूर्णांक is_not_in_promisor_pack_obj(काष्ठा object *obj, व्योम *तथ्य UNUSED)
{
	काष्ठा odb_source_info source_info;
	काष्ठा object_info info = {
		.source_infop = &source_info,
	};

	अगर (odb_read_object_info_extended(the_repository->objects, &obj->oid, &info, 0))
		BUG("should_include_obj should only be called on existing objects");
	वापस source_info.source->type != ODB_SOURCE_PACKED || !source_info.u.packed.pack->pack_promisor;
}

अटल पूर्णांक is_not_in_promisor_pack(काष्ठा commit *commit, व्योम *तथ्य) {
	वापस is_not_in_promisor_pack_obj((काष्ठा object *) commit, तथ्य);
}

अटल पूर्णांक parse_stdin_packs_mode(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				  पूर्णांक unset)
{
	क्रमागत stdin_packs_mode *mode = opt->मूल्य;

	अगर (unset)
		*mode = STDIN_PACKS_MODE_NONE;
	अन्यथा अगर (!arg || !*arg)
		*mode = STDIN_PACKS_MODE_STANDARD;
	अन्यथा अगर (!म_भेद(arg, "follow"))
		*mode = STDIN_PACKS_MODE_FOLLOW;
	अन्यथा
		die(_("invalid value for '%s': '%s'"), opt->long_name, arg);

	वापस 0;
}

पूर्णांक cmd_pack_objects(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo)
{
	पूर्णांक use_internal_rev_list = 0;
	पूर्णांक all_progress_implied = 0;
	काष्ठा strvec rp = STRVEC_INIT;
	पूर्णांक rev_list_unpacked = 0, rev_list_all = 0, rev_list_reflog = 0;
	पूर्णांक rev_list_सूचकांक = 0;
	क्रमागत stdin_packs_mode stdin_packs = STDIN_PACKS_MODE_NONE;
	काष्ठा string_list keep_pack_list = STRING_LIST_INIT_NODUP;
	काष्ठा list_objects_filter_options filter_options =
		LIST_OBJECTS_FILTER_INIT;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	काष्ठा option pack_objects_options[] = {
		OPT_CALLBACK_F('q', "quiet", &progress, शून्य,
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
		  PARSE_OPT_NONEG, option_parse_सूचकांक_version),
		OPT_UNSIGNED(0, "max-pack-size", &pack_आकार_limit,
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
		OPT_INTEGER(0, "threads", &delta_खोज_threads,
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
		OPT_SET_INT_F(0, "indexed-objects", &rev_list_सूचकांक,
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
		OPT_CALLBACK_F(0, "unpack-unreachable", शून्य, N_("time"),
		  N_("unpack unreachable objects newer than <time>"),
		  PARSE_OPT_OPTARG, option_parse_unpack_unreachable),
		OPT_BOOL(0, "cruft", &cruft, N_("create a cruft pack")),
		OPT_CALLBACK_F(0, "cruft-expiration", शून्य, N_("time"),
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
		OPT_BOOL(0, "use-bitmap-index", &use_bitmap_सूचकांक,
			 N_("use a bitmap index if available to speed up counting objects")),
		OPT_SET_INT(0, "write-bitmap-index", &write_bitmap_सूचकांक,
			    N_("write a bitmap index together with the pack index"),
			    WRITE_BITMAP_TRUE),
		OPT_SET_INT_F(0, "write-bitmap-index-quiet",
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
		OPT_INTEGER(0, "name-hash-version", &name_hash_version,
			 N_("use the specified name-hash function to group similar objects")),
		OPT_END(),
	};

	अगर (DFS_अंक_STATES > (1 << OE_DFS_STATE_BITS))
		BUG("too many dfs states, increase OE_DFS_STATE_BITS");

	disable_replace_refs();

	sparse = git_env_bool("GIT_TEST_PACK_SPARSE", -1);
	अगर (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		अगर (sparse < 0)
			sparse = the_repository->settings.pack_use_sparse;
		अगर (the_repository->settings.pack_use_multi_pack_reuse)
			allow_pack_reuse = MULTI_PACK_REUSE;
	}

	reset_pack_idx_option(&pack_idx_opts);
	pack_idx_opts.flags |= WRITE_REV;
	repo_config(the_repository, git_pack_config, शून्य);
	अगर (git_env_bool(GIT_TEST_NO_WRITE_REV_सूचकांक, 0))
		pack_idx_opts.flags &= ~WRITE_REV;

	progress = isatty(2);
	argc = parse_options(argc, argv, prefix, pack_objects_options,
			     pack_usage, 0);

	अगर (argc) {
		base_name = argv[0];
		argc--;
	}
	अगर (pack_to_stdout != !base_name || argc)
		usage_with_options(pack_usage, pack_objects_options);

	अगर (path_walk < 0) {
		अगर (use_bitmap_सूचकांक > 0 ||
		    !use_internal_rev_list)
			path_walk = 0;
		अन्यथा अगर (the_repository->gitdir &&
			 the_repository->settings.pack_use_path_walk)
			path_walk = 1;
		अन्यथा
			path_walk = git_env_bool("GIT_TEST_PACK_PATH_WALK", 0);
	}

	अगर (depth < 0)
		depth = 0;
	अगर (depth >= (1 << OE_DEPTH_BITS)) {
		warning(_("delta chain depth %d is too deep, forcing %d"),
			depth, (1 << OE_DEPTH_BITS) - 1);
		depth = (1 << OE_DEPTH_BITS) - 1;
	}
	अगर (cache_max_small_delta_आकार >= (1U << OE_Z_DELTA_BITS)) {
		warning(_("pack.deltaCacheLimit is too high, forcing %d"),
			(1U << OE_Z_DELTA_BITS) - 1);
		cache_max_small_delta_आकार = (1U << OE_Z_DELTA_BITS) - 1;
	}
	अगर (window < 0)
		window = 0;

	strvec_धकेलो(&rp, "pack-objects");

	अगर (path_walk) {
		स्थिर अक्षर *option = शून्य;
		अगर (!path_walk_filter_compatible(&filter_options))
			option = "--filter";

		अगर (option) {
			warning(_("cannot use %s with %s"),
				option, "--path-walk");
			path_walk = 0;
		}
	}
	अगर (path_walk) {
		strvec_धकेलो(&rp, "--objects");
	} अन्यथा अगर (thin) {
		use_internal_rev_list = 1;
		strvec_धकेलो(&rp, shallow
				? "--objects-edge-aggressive"
				: "--objects-edge");
	} अन्यथा
		strvec_धकेलो(&rp, "--objects");

	अगर (rev_list_all) {
		use_internal_rev_list = 1;
		strvec_धकेलो(&rp, "--all");
	}
	अगर (rev_list_reflog) {
		use_internal_rev_list = 1;
		strvec_धकेलो(&rp, "--reflog");
	}
	अगर (rev_list_सूचकांक) {
		use_internal_rev_list = 1;
		strvec_धकेलो(&rp, "--indexed-objects");
	}
	अगर (rev_list_unpacked && !stdin_packs) {
		use_internal_rev_list = 1;
		strvec_धकेलो(&rp, "--unpacked");
	}

	die_for_incompatible_opt2(exclude_promisor_objects,
				  "--exclude-promisor-objects",
				  exclude_promisor_objects_best_effort,
				  "--exclude-promisor-objects-best-effort");
	अगर (exclude_promisor_objects) {
		repo->fetch_if_missing = 0;

		/* --stdin-packs handles promisor objects separately. */
		अगर (!stdin_packs) {
			use_internal_rev_list = 1;
			strvec_धकेलो(&rp, "--exclude-promisor-objects");
		}
	} अन्यथा अगर (exclude_promisor_objects_best_effort) {
		use_internal_rev_list = 1;
		arg_missing_action = MA_ALLOW_ANY;
		repo->fetch_if_missing = 0;
		fn_show_object = show_object__ma_allow_any;
		/* revs configured below */
	}
	अगर (unpack_unreachable || keep_unreachable || pack_loose_unreachable)
		use_internal_rev_list = 1;

	अगर (!reuse_object)
		reuse_delta = 0;
	अगर (cfg->pack_compression_level == -1)
		cfg->pack_compression_level = Z_DEFAULT_COMPRESSION;
	अन्यथा अगर (cfg->pack_compression_level < 0 || cfg->pack_compression_level > Z_BEST_COMPRESSION)
		die(_("bad pack compression level %d"), cfg->pack_compression_level);

	अगर (!delta_खोज_threads)	/* --threads=0 means autodetect */
		delta_खोज_threads = online_cpus();

	अगर (!HAVE_THREADS && delta_खोज_threads != 1)
		warning(_("no threads support, ignoring --threads"));
	अगर (!pack_to_stdout && !pack_आकार_limit)
		pack_आकार_limit = pack_आकार_limit_cfg;
	अगर (pack_to_stdout && pack_आकार_limit)
		die(_("--max-pack-size cannot be used to build a pack for transfer"));
	अगर (pack_आकार_limit && pack_आकार_limit < 1024*1024) {
		warning(_("minimum pack size limit is 1 MiB"));
		pack_आकार_limit = 1024*1024;
	}

	अगर (!pack_to_stdout && thin)
		die(_("--thin cannot be used to build an indexable pack"));

	die_for_incompatible_opt2(keep_unreachable, "--keep-unreachable",
				  unpack_unreachable, "--unpack-unreachable");
	अगर (!rev_list_all || !rev_list_reflog || !rev_list_सूचकांक)
		unpack_unreachable_expiration = 0;

	die_for_incompatible_opt2(stdin_packs, "--stdin-packs",
				  filter_options.choice, "--filter");


	अगर (stdin_packs && use_internal_rev_list)
		die(_("cannot use internal rev list with --stdin-packs"));

	अगर (cruft) {
		अगर (use_internal_rev_list)
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
	अगर (!pack_to_stdout)
		use_bitmap_सूचकांक_default = 0;

	अगर (use_bitmap_सूचकांक < 0)
		use_bitmap_सूचकांक = use_bitmap_सूचकांक_default;

	/* "hard" reasons not to use bitmaps; these just won't work at all */
	अगर (!use_internal_rev_list || (!pack_to_stdout && write_bitmap_सूचकांक) || is_repository_shallow(the_repository))
		use_bitmap_सूचकांक = 0;

	अगर (pack_to_stdout || !rev_list_all)
		write_bitmap_सूचकांक = 0;

	अगर (name_hash_version < 0)
		name_hash_version = (पूर्णांक)git_env_ulong("GIT_TEST_NAME_HASH_VERSION", 1);

	validate_name_hash_version();

	अगर (use_delta_islands)
		strvec_धकेलो(&rp, "--topo-order");

	अगर (progress && all_progress_implied)
		progress = 2;

	add_extra_kept_packs(&keep_pack_list);
	अगर (ignore_packed_keep_on_disk) {
		काष्ठा packed_git *p;

		repo_for_each_pack(the_repository, p)
			अगर (p->pack_local && p->pack_keep)
				अवरोध;
		अगर (!p) /* no keep-able packs found */
			ignore_packed_keep_on_disk = 0;
	}
	अगर (local) {
		/*
		 * unlike ignore_packed_keep_on_disk above, we do not
		 * want to unset "local" based on looking at packs, as
		 * it also covers non-local objects
		 */
		काष्ठा packed_git *p;

		repo_for_each_pack(the_repository, p) {
			अगर (!p->pack_local) {
				have_non_local_packs = 1;
				अवरोध;
			}
		}
	}

	trace2_region_enter("pack-objects", "enumerate-objects",
			    the_repository);
	prepare_packing_तथ्य(the_repository, &to_pack);

	अगर (progress && !cruft)
		progress_state = start_progress(the_repository,
						_("Enumerating objects"), 0);
	अगर (stdin_packs) {
		read_stdin_packs(repo, stdin_packs, rev_list_unpacked);
	} अन्यथा अगर (cruft) {
		read_cruft_objects();
	} अन्यथा अगर (!use_internal_rev_list) {
		read_object_list_from_stdin();
	} अन्यथा {
		काष्ठा rev_info revs;

		repo_init_revisions(the_repository, &revs, शून्य);
		list_objects_filter_copy(&revs.filter, &filter_options);
		अगर (exclude_promisor_objects_best_effort) {
			revs.include_check = is_not_in_promisor_pack;
			revs.include_check_obj = is_not_in_promisor_pack_obj;
		}
		get_object_list(&revs, &rp);
		release_revisions(&revs);
	}
	cleanup_preferred_base();
	अगर (include_tag && nr_परिणाम)
		refs_for_each_tag_ref(get_main_ref_store(the_repository),
				      add_ref_tag, शून्य);
	stop_progress(&progress_state);
	trace2_region_leave("pack-objects", "enumerate-objects",
			    the_repository);

	अगर (non_empty && !nr_परिणाम)
		जाओ cleanup;
	अगर (nr_परिणाम) {
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

	अगर (progress)
		fprintf_ln(मानक_त्रुटि,
			   _("Total %"PRIu32" (delta %"PRIu32"),"
			     " reused %"PRIu32" (delta %"PRIu32"),"
			     " pack-reused %"PRIu32" (from %"PRIuMAX")"),
			   written, written_delta, reused, reused_delta,
			   reuse_packfile_objects,
			   (uintmax_t)reuse_packfiles_used_nr);

	trace2_तथ्य_intmax("pack-objects", the_repository, "written", written);
	trace2_तथ्य_intmax("pack-objects", the_repository, "written/delta", written_delta);
	trace2_तथ्य_intmax("pack-objects", the_repository, "reused", reused);
	trace2_तथ्य_intmax("pack-objects", the_repository, "reused/delta", reused_delta);
	trace2_तथ्य_intmax("pack-objects", the_repository, "pack-reused", reuse_packfile_objects);
	trace2_तथ्य_intmax("pack-objects", the_repository, "packs-reused", reuse_packfiles_used_nr);

cleanup:
	clear_packing_तथ्य(&to_pack);
	list_objects_filter_release(&filter_options);
	string_list_clear(&keep_pack_list, 0);
	strvec_clear(&rp);

	वापस 0;
}
