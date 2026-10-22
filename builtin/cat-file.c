/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ident.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects-filter-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "userdiff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/streaming.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "alias.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "transport.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ALLOWED_OBJ_ਹੱਦਬੰਦੀ 10000

/* Maximum input size permitted for the remote-object-info command. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_REMOTE_OBJ_INFO_ਸਤਰ \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_ਹੱਦਬੰਦੀ * (GIT_MAX_HEXSZ + 1))

ਗਿਣਤੀ batch_ਬਹੁਲਕ {
	BATCH_ਬਹੁਲਕ_CONTENTS,
	BATCH_ਬਹੁਲਕ_INFO,
	BATCH_ਬਹੁਲਕ_ਕਤਾਰ_AND_DISPATCH,
};

ਬਣਤਰ batch_options {
	ਬਣਤਰ list_objects_filter_options objects_filter;
	ਪੂਰਨ enabled;
	ਪੂਰਨ follow_symlinks;
	ਗਿਣਤੀ batch_ਬਹੁਲਕ batch_ਬਹੁਲਕ;
	ਪੂਰਨ ਬਫਰ_output;
	ਪੂਰਨ all_objects;
	ਪੂਰਨ unordered;
	ਪੂਰਨ transform_ਬਹੁਲਕ; /* may be 'w' or 'c' for --filters or --textconv */
	ਅੱਖਰ ਨਿਵੇਸ਼_delim;
	ਅੱਖਰ output_delim;
	ਅਚਲ ਅੱਖਰ *format;
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *force_ਰਸਤਾ;

ਸਥਿਰ ਬਣਤਰ string_list mailmap = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਪੂਰਨ use_mailmap;

ਸਥਿਰ ਅੱਖਰ *replace_idents_using_mailmap(ਅੱਖਰ *, ਮਾਪ_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ਸਥਿਰ ਖਾਲੀ ਲੱਦੋ_mailmap(ਖਾਲੀ)
{
	ਜੇ (mailmap.strdup_ਲੜੀਆਂ)
		ਵਾਪਸ;

	read_mailmap(the_repository, &mailmap);
}

ਸਥਿਰ ਅੱਖਰ *replace_idents_using_mailmap(ਅੱਖਰ *object_buf, ਮਾਪ_t *ਮਾਪ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖਾਂ[] = { "author ", "committer ", "tagger ", ਨਿਰਾ };

	strbuf_attach(&sb, object_buf, *ਮਾਪ, *ਮਾਪ + 1);
	apply_mailmap_ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ(&sb, ਸਿਰਲੇਖਾਂ, &mailmap);
	*ਮਾਪ = sb.len;
	ਵਾਪਸ strbuf_detach(&sb, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ filter_object(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ,
			 ਅਚਲ ਬਣਤਰ object_id *oid,
			 ਅੱਖਰ **buf, ਮਾਪ_t *ਮਾਪ)
{
	ਗਿਣਤੀ object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, ਮਾਪ);
	ਜੇ (!*buf)
		ਵਾਪਸ error(_("cannot read object %s '%s'"),
			     oid_ਮੰਜ਼ਿਲ_hex(oid), ਰਸਤਾ);
	ਜੇ ((type == OBJ_BLOB) && S_ISREG(ਬਹੁਲਕ)) {
		ਬਣਤਰ strbuf strbuf = STRBUF_INIT;
		ਬਣਤਰ checkout_metadata meta;

		init_checkout_metadata(&meta, ਨਿਰਾ, ਨਿਰਾ, oid);
		ਜੇ (convert_ਮੰਜ਼ਿਲ_working_ਰੁੱਖ(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ, *buf, *ਮਾਪ, &strbuf, &meta)) {
			ਮੁਕਤ(*buf);
			*ਮਾਪ = strbuf.len;
			*buf = strbuf_detach(&strbuf, ਨਿਰਾ);
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਧਾਰਾ_blob(ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਜੇ (odb_ਧਾਰਾ_blob_ਮੰਜ਼ਿਲ_fd(the_repository->objects, 1, oid, ਨਿਰਾ, 0))
		die("unable to stream %s to stdout", oid_ਮੰਜ਼ਿਲ_hex(oid));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cat_one_file(ਪੂਰਨ opt, ਅਚਲ ਅੱਖਰ *exp_type, ਅਚਲ ਅੱਖਰ *obj_name)
{
	ਪੂਰਨ ret;
	ਬਣਤਰ object_id oid;
	ਗਿਣਤੀ object_type type;
	ਅੱਖਰ *buf;
	ਮਾਪ_t ਮਾਪ;
	ਬਣਤਰ object_context obj_context = {0};
	ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = OBJECT_INFO_LOOKUP_REPLACE;
	ਬਿਨਾਨਿਸ਼ਾਨੀ get_oid_flags =
		GET_OID_ਰਿਕਾਰਡ_ਰਸਤਾ |
		GET_OID_ONLY_ਮੰਜ਼ਿਲ_DIE |
		GET_OID_HASH_ANY;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = force_ਰਸਤਾ;
	ਅਚਲ ਪੂਰਨ opt_cw = (opt == 'c' || opt == 'w');
	ਜੇ (!ਰਸਤਾ && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_ਰਸਤਾ;

	ਜੇ (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	ਜੇ (!ਰਸਤਾ)
		ਰਸਤਾ = obj_context.ਰਸਤਾ;
	ਜੇ (obj_context.ਬਹੁਲਕ == S_IFINVALID)
		obj_context.ਬਹੁਲਕ = 0100644;

	buf = ਨਿਰਾ;
	ਚੋਣ (opt) {
	ਹਾਲਤ 't':
		oi.typep = &type;
		ਜੇ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		ਲਿਖੋ("%s\n", type_name(type));
		ret = 0;
		ਜਾਓ cleanup;

	ਹਾਲਤ 's':
		oi.sizep = &ਮਾਪ;

		ਜੇ (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (ਖਾਲੀ**)&buf;
		}

		ਜੇ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		ਜੇ (use_mailmap && (type == OBJ_COMMIT || type == OBJ_ਟੈਗ))
			buf = replace_idents_using_mailmap(buf, &ਮਾਪ);

		ਲਿਖੋ("%"PRIuMAX"\n", (uintmax_t)ਮਾਪ);
		ret = 0;
		ਜਾਓ cleanup;

	ਹਾਲਤ 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		ਜਾਓ cleanup;

	ਹਾਲਤ 'w':

		ਜੇ (filter_object(ਰਸਤਾ, obj_context.ਬਹੁਲਕ,
				  &oid, &buf, &ਮਾਪ)) {
			ret = -1;
			ਜਾਓ cleanup;
		}
		ਰੋਕੋ;

	ਹਾਲਤ 'c':
	{
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ_ul = 0;
		ਪੂਰਨ textconv_ret = textconv_object(the_repository, ਰਸਤਾ,
						   obj_context.ਬਹੁਲਕ, &oid, 1,
						   &buf, &ਮਾਪ_ul);
		ਮਾਪ = ਮਾਪ_ul;
		ਜੇ (textconv_ret)
			ਰੋਕੋ;
	}
		/* else fallthrough */

	ਹਾਲਤ 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ਨਿਰਾ);
		ਜੇ (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		ਜੇ (type == OBJ_ਰੁੱਖ) {
			ਅਚਲ ਅੱਖਰ *ls_ਦਲੀਲਾਂ[3] = { ਨਿਰਾ };
			ls_ਦਲੀਲਾਂ[0] =  "ls-tree";
			ls_ਦਲੀਲਾਂ[1] =  obj_name;
			ret = cmd_ls_ਰੁੱਖ(2, ls_ਦਲੀਲਾਂ, ਨਿਰਾ, the_repository);
			ਜਾਓ cleanup;
		}

		ਜੇ (type == OBJ_BLOB) {
			ret = ਧਾਰਾ_blob(&oid);
			ਜਾਓ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &ਮਾਪ);
		ਜੇ (!buf)
			die("Cannot read object %s", obj_name);

		ਜੇ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &ਮਾਪ);

		/* otherwise just spit out the data */
		ਰੋਕੋ;

	ਹਾਲਤ 0:
	{
		ਗਿਣਤੀ object_type exp_type_id = type_from_string(exp_type);

		ਜੇ (exp_type_id == OBJ_BLOB) {
			ਬਣਤਰ object_id blob_oid;
			ਜੇ (odb_read_object_info(the_repository->objects,
						 &oid, ਨਿਰਾ) == OBJ_ਟੈਗ) {
				ਅੱਖਰ *ਬਫਰ = odb_read_object(the_repository->objects,
							       &oid, &type, &ਮਾਪ);
				ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ;

				ਜੇ (!ਬਫਰ)
					die(_("unable to read %s"), oid_ਮੰਜ਼ਿਲ_hex(&oid));

				ਜੇ (!skip_ਅਗੇਤਰ(ਬਫਰ, "object ", &ਨਿਸ਼ਾਨਾ) ||
				    get_oid_hex_algop(ਨਿਸ਼ਾਨਾ, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_ਮੰਜ਼ਿਲ_hex(&oid));
				ਮੁਕਤ(ਬਫਰ);
			} ਨਹੀਂਤਾਂ
				oidcpy(&blob_oid, &oid);

			ਜੇ (odb_read_object_info(the_repository->objects,
						 &blob_oid, ਨਿਰਾ) == OBJ_BLOB) {
				ret = ਧਾਰਾ_blob(&blob_oid);
				ਜਾਓ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &ਮਾਪ, ਨਿਰਾ);

		ਜੇ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &ਮਾਪ);
		ਰੋਕੋ;
	}
	ਮੂਲ:
		die("git cat-file: unknown option: %s", exp_type);
	}

	ਜੇ (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, ਮਾਪ);
	ret = 0;
cleanup:
	ਮੁਕਤ(buf);
	object_context_release(&obj_context);
	ਵਾਪਸ ret;
}

ਬਣਤਰ expand_ਅੰਕੜੇ {
	ਬਣਤਰ object_id oid;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਬਹੁਲਕ;
	off_t disk_ਮਾਪ;
	ਅਚਲ ਅੱਖਰ *rest;
	ਬਣਤਰ object_id delta_ਅਧਾਰ_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	ਪੂਰਨ mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	ਪੂਰਨ ਵੰਡੋ_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	ਬਣਤਰ object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	ਬਣਤਰ string_list remote_allowed_atoms;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXPAND_ਅੰਕੜੇ_INIT  {  \
	.ਬਹੁਲਕ = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ਸਥਿਰ ਪੂਰਨ is_atom(ਅਚਲ ਅੱਖਰ *atom, ਅਚਲ ਅੱਖਰ *s, ਪੂਰਨ slen)
{
	ਪੂਰਨ alen = ਲੜੀ_ਲੰਬਾਈ(atom);
	ਵਾਪਸ alen == slen && !memcmp(atom, s, alen);
}

ਸਥਿਰ ਪੂਰਨ expand_atom(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *atom, ਪੂਰਨ len,
		       ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਜੇ (ਅੰਕੜੇ->is_remote) {
		ਮਾਪ_t i;
		ਲਈ (i = 0; i < ਅੰਕੜੇ->remote_allowed_atoms.nr; i++)
			ਜੇ (is_atom(ਅੰਕੜੇ->remote_allowed_atoms.ਚੀਜ਼ਾਂ[i].string,
				    atom, len))
				ਰੋਕੋ;
		ਜੇ (i == ਅੰਕੜੇ->remote_allowed_atoms.nr)
			ਵਾਪਸ 1;
	}

	ਜੇ (is_atom("objectname", atom, len)) {
		ਜੇ (!ਅੰਕੜੇ->mark_query)
			strbuf_ਜੋੜੋ_oid_hex(sb, &ਅੰਕੜੇ->oid);
	} ਨਹੀਂਤਾਂ ਜੇ (is_atom("objecttype", atom, len)) {
		ਜੇ (ਅੰਕੜੇ->mark_query) {
			ਅੰਕੜੇ->info.typep = &ਅੰਕੜੇ->type;
		} ਨਹੀਂਤਾਂ {
			ਅਚਲ ਅੱਖਰ *t = type_name(ਅੰਕੜੇ->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} ਨਹੀਂਤਾਂ ਜੇ (is_atom("objectsize", atom, len)) {
		ਜੇ (ਅੰਕੜੇ->mark_query)
			ਅੰਕੜੇ->info.sizep = &ਅੰਕੜੇ->ਮਾਪ;
		ਨਹੀਂਤਾਂ
			strbuf_ਜੋੜੋ_uint(sb, ਅੰਕੜੇ->ਮਾਪ);
	} ਨਹੀਂਤਾਂ ਜੇ (is_atom("objectsize:disk", atom, len)) {
		ਜੇ (ਅੰਕੜੇ->mark_query)
			ਅੰਕੜੇ->info.disk_sizep = &ਅੰਕੜੇ->disk_ਮਾਪ;
		ਨਹੀਂਤਾਂ
			strbuf_ਜੋੜੋ_uint(sb, ਅੰਕੜੇ->disk_ਮਾਪ);
	} ਨਹੀਂਤਾਂ ਜੇ (is_atom("rest", atom, len)) {
		ਜੇ (ਅੰਕੜੇ->mark_query)
			ਅੰਕੜੇ->ਵੰਡੋ_on_whitespace = 1;
		ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕੜੇ->rest)
			strbuf_addstr(sb, ਅੰਕੜੇ->rest);
	} ਨਹੀਂਤਾਂ ਜੇ (is_atom("deltabase", atom, len)) {
		ਜੇ (ਅੰਕੜੇ->mark_query)
			ਅੰਕੜੇ->info.delta_ਅਧਾਰ_oid = &ਅੰਕੜੇ->delta_ਅਧਾਰ_oid;
		ਨਹੀਂਤਾਂ
			strbuf_ਜੋੜੋ_oid_hex(sb, &ਅੰਕੜੇ->delta_ਅਧਾਰ_oid);
	} ਨਹੀਂਤਾਂ ਜੇ (is_atom("objectmode", atom, len)) {
		ਜੇ (!ਅੰਕੜੇ->mark_query && !(S_IFINVALID == ਅੰਕੜੇ->ਬਹੁਲਕ))
			strbuf_addf(sb, "%06o", ਅੰਕੜੇ->ਬਹੁਲਕ);
	} ਨਹੀਂਤਾਂ
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ expand_format(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ,
			  ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਜਦੋਂ (strbuf_expand_ਕਦਮ(sb, &ਸ਼ੁਰੂ)) {
		ਅਚਲ ਅੱਖਰ *ਸਿਰਾ;

		ਜੇ (skip_ਅਗੇਤਰ(ਸ਼ੁਰੂ, "%", &ਸ਼ੁਰੂ) || *ਸ਼ੁਰੂ != '(')
			strbuf_addch(sb, '%');
		ਨਹੀਂਤਾਂ ਜੇ ((ਸਿਰਾ = strchr(ਸ਼ੁਰੂ + 1, ')')) &&
			 expand_atom(sb, ਸ਼ੁਰੂ + 1, ਸਿਰਾ - ਸ਼ੁਰੂ - 1, ਅੰਕੜੇ))
			ਸ਼ੁਰੂ = ਸਿਰਾ + 1;
		ਨਹੀਂਤਾਂ
			strbuf_expand_bad_format(ਸ਼ੁਰੂ, "cat-file");
	}
}

ਸਥਿਰ ਖਾਲੀ batch_write(ਬਣਤਰ batch_options *opt, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਪੂਰਨ len)
{
	ਜੇ (opt->ਬਫਰ_output) {
		ਜੇ (fwrite(ਅੰਕੜੇ, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} ਨਹੀਂਤਾਂ
		write_or_die(1, ਅੰਕੜੇ, len);
}

ਸਥਿਰ ਖਾਲੀ print_object_or_die(ਬਣਤਰ batch_options *opt, ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਅਚਲ ਬਣਤਰ object_id *oid = &ਅੰਕੜੇ->oid;

	ਪ੍ਰਤਿਜ੍ਞਾ(ਅੰਕੜੇ->info.typep);

	ਜੇ (ਅੰਕੜੇ->type == OBJ_BLOB) {
		ਜੇ (opt->ਬਫਰ_output)
			fflush(stdout);
		ਜੇ (opt->transform_ਬਹੁਲਕ) {
			ਅੱਖਰ *contents;
			ਮਾਪ_t ਮਾਪ;

			ਜੇ (!ਅੰਕੜੇ->rest)
				die("missing path for '%s'", oid_ਮੰਜ਼ਿਲ_hex(oid));

			ਜੇ (opt->transform_ਬਹੁਲਕ == 'w') {
				ਜੇ (filter_object(ਅੰਕੜੇ->rest, 0100644, oid,
						  &contents, &ਮਾਪ))
					die("could not convert '%s' %s",
					    oid_ਮੰਜ਼ਿਲ_hex(oid), ਅੰਕੜੇ->rest);
			} ਨਹੀਂਤਾਂ ਜੇ (opt->transform_ਬਹੁਲਕ == 'c') {
				ਗਿਣਤੀ object_type type;
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ_ul = 0;
				ਜੇ (textconv_object(the_repository,
						    ਅੰਕੜੇ->rest, 0100644, oid,
						    1, &contents, &ਮਾਪ_ul))
					ਮਾਪ = ਮਾਪ_ul;
				ਨਹੀਂਤਾਂ
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &ਮਾਪ);
				ਜੇ (!contents)
					die("could not convert '%s' %s",
					    oid_ਮੰਜ਼ਿਲ_hex(oid), ਅੰਕੜੇ->rest);
			} ਨਹੀਂਤਾਂ
				BUG("invalid transform_mode: %c", opt->transform_ਬਹੁਲਕ);
			batch_write(opt, contents, ਮਾਪ);
			ਮੁਕਤ(contents);
		} ਨਹੀਂਤਾਂ {
			ਧਾਰਾ_blob(oid);
		}
	}
	ਨਹੀਂਤਾਂ {
		ਗਿਣਤੀ object_type type;
		ਮਾਪ_t ਮਾਪ;
		ਖਾਲੀ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &ਮਾਪ);
		ਜੇ (!contents)
			die("object %s disappeared", oid_ਮੰਜ਼ਿਲ_hex(oid));

		ਜੇ (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &ਮਾਪ);

		ਜੇ (type != ਅੰਕੜੇ->type)
			die("object %s changed type!?", oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜੇ (ਅੰਕੜੇ->info.sizep && ਮਾਪ != ਅੰਕੜੇ->ਮਾਪ && !use_mailmap)
			die("object %s changed size!?", oid_ਮੰਜ਼ਿਲ_hex(oid));

		batch_write(opt, contents, ਮਾਪ);
		ਮੁਕਤ(contents);
	}
}

ਸਥਿਰ ਖਾਲੀ print_default_format(ਬਣਤਰ strbuf *scratch, ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ,
				 ਬਣਤਰ batch_options *opt)
{
	strbuf_ਜੋੜੋ_oid_hex(scratch, &ਅੰਕੜੇ->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(ਅੰਕੜੇ->type));
	strbuf_addch(scratch, ' ');
	strbuf_ਜੋੜੋ_uint(scratch, ਅੰਕੜੇ->ਮਾਪ);
	strbuf_addch(scratch, opt->output_delim);
}

ਸਥਿਰ ਖਾਲੀ report_object_ਸਥਿਤੀ(ਬਣਤਰ batch_options *opt,
				 ਅਚਲ ਅੱਖਰ *obj_name,
				 ਅਚਲ ਬਣਤਰ object_id *oid,
				 ਅਚਲ ਅੱਖਰ *ਸਥਿਤੀ)
{
	ਲਿਖੋ("%s %s%c", obj_name ? obj_name : oid_ਮੰਜ਼ਿਲ_hex(oid),
	       ਸਥਿਤੀ, opt->output_delim);
	fflush(stdout);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
ਸਥਿਰ ਖਾਲੀ batch_object_write(ਅਚਲ ਅੱਖਰ *obj_name,
			       ਬਣਤਰ strbuf *scratch,
			       ਬਣਤਰ batch_options *opt,
			       ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ,
			       ਬਣਤਰ packed_git *pack,
			       off_t ਵਿੱਥ)
{
	ਜੇ (!ਅੰਕੜੇ->skip_object_info) {
		ਪੂਰਨ ret;

		ਜੇ (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_ਹੱਦਬੰਦੀ ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			ਅੰਕੜੇ->info.typep = &ਅੰਕੜੇ->type;
		ਜੇ (opt->objects_filter.choice == LOFC_BLOB_ਹੱਦਬੰਦੀ)
			ਅੰਕੜੇ->info.sizep = &ਅੰਕੜੇ->ਮਾਪ;

		ਜੇ (pack)
			ret = packed_object_info(ਨਿਰਾ, pack, ਵਿੱਥ, &ਅੰਕੜੇ->info);
		ਨਹੀਂਤਾਂ
			ret = odb_read_object_info_extended(the_repository->objects,
							    &ਅੰਕੜੇ->oid, &ਅੰਕੜੇ->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		ਜੇ (ret < 0) {
			ਜੇ (ਅੰਕੜੇ->ਬਹੁਲਕ == S_IFGITLINK)
				report_object_ਸਥਿਤੀ(opt, ਨਿਰਾ, &ਅੰਕੜੇ->oid, "submodule");
			ਨਹੀਂਤਾਂ
				report_object_ਸਥਿਤੀ(opt, obj_name, &ਅੰਕੜੇ->oid, "missing");
			ਵਾਪਸ;
		}

		ਚੋਣ (opt->objects_filter.choice) {
		ਹਾਲਤ LOFC_DISABLED:
			ਰੋਕੋ;
		ਹਾਲਤ LOFC_BLOB_NONE:
			ਜੇ (ਅੰਕੜੇ->type == OBJ_BLOB) {
				ਜੇ (!opt->all_objects)
					report_object_ਸਥਿਤੀ(opt, obj_name,
							     &ਅੰਕੜੇ->oid, "excluded");
				ਵਾਪਸ;
			}
			ਰੋਕੋ;
		ਹਾਲਤ LOFC_BLOB_ਹੱਦਬੰਦੀ:
			ਜੇ (ਅੰਕੜੇ->type == OBJ_BLOB &&
			    ਅੰਕੜੇ->ਮਾਪ >= opt->objects_filter.blob_ਹੱਦਬੰਦੀ_ਮੁੱਲ) {
				ਜੇ (!opt->all_objects)
					report_object_ਸਥਿਤੀ(opt, obj_name,
							     &ਅੰਕੜੇ->oid, "excluded");
				ਵਾਪਸ;
			}
			ਰੋਕੋ;
		ਹਾਲਤ LOFC_OBJECT_TYPE:
			ਜੇ (ਅੰਕੜੇ->type != opt->objects_filter.object_type) {
				ਜੇ (!opt->all_objects)
					report_object_ਸਥਿਤੀ(opt, obj_name,
							     &ਅੰਕੜੇ->oid, "excluded");
				ਵਾਪਸ;
			}
			ਰੋਕੋ;
		ਮੂਲ:
			BUG("unsupported objects filter");
		}

		ਜੇ (use_mailmap && (ਅੰਕੜੇ->type == OBJ_COMMIT || ਅੰਕੜੇ->type == OBJ_ਟੈਗ)) {
			ਅੱਖਰ *buf = ਨਿਰਾ;

			buf = odb_read_object(the_repository->objects, &ਅੰਕੜੇ->oid,
					      &ਅੰਕੜੇ->type, &ਅੰਕੜੇ->ਮਾਪ);
			ਜੇ (!buf)
				die(_("unable to read %s"), oid_ਮੰਜ਼ਿਲ_hex(&ਅੰਕੜੇ->oid));
			buf = replace_idents_using_mailmap(buf, &ਅੰਕੜੇ->ਮਾਪ);

			ਮੁਕਤ(buf);
		}
	}

	strbuf_reset(scratch);

	ਜੇ (!opt->format) {
		print_default_format(scratch, ਅੰਕੜੇ, opt);
	} ਨਹੀਂਤਾਂ {
		expand_format(scratch, opt->format, ਅੰਕੜੇ);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	ਜੇ (opt->batch_ਬਹੁਲਕ == BATCH_ਬਹੁਲਕ_CONTENTS) {
		print_object_or_die(opt, ਅੰਕੜੇ);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ਸਥਿਰ ਖਾਲੀ batch_one_object(ਅਚਲ ਅੱਖਰ *obj_name,
			     ਬਣਤਰ strbuf *scratch,
			     ਬਣਤਰ batch_options *opt,
			     ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ object_context ctx = {0};
	ਪੂਰਨ flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	ਗਿਣਤੀ get_oid_ਨਤੀਜਾ ਨਤੀਜਾ;

	ਨਤੀਜਾ = get_oid_with_context(the_repository, obj_name,
				      flags, &ਅੰਕੜੇ->oid, &ctx);
	ਜੇ (ਨਤੀਜਾ != ਮਿਲਿਆ) {
		ਚੋਣ (ਨਤੀਜਾ) {
		ਹਾਲਤ MISSING_OBJECT:
			report_object_ਸਥਿਤੀ(opt, obj_name, &ਅੰਕੜੇ->oid, "missing");
			ਰੋਕੋ;
		ਹਾਲਤ SHORT_NAME_AMBIGUOUS:
			report_object_ਸਥਿਤੀ(opt, obj_name, &ਅੰਕੜੇ->oid, "ambiguous");
			ਰੋਕੋ;
		ਹਾਲਤ DANGLING_SYMLINK:
			ਲਿਖੋ("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)ਲੜੀ_ਲੰਬਾਈ(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ਰੋਕੋ;
		ਹਾਲਤ SYMLINK_LOOP:
			ਲਿਖੋ("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)ਲੜੀ_ਲੰਬਾਈ(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ਰੋਕੋ;
		ਹਾਲਤ NOT_DIR:
			ਲਿਖੋ("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)ਲੜੀ_ਲੰਬਾਈ(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ਰੋਕੋ;
		ਮੂਲ:
			BUG("unknown get_sha1_with_context result %d\n",
			       ਨਤੀਜਾ);
			ਰੋਕੋ;
		}
		fflush(stdout);

		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (ctx.ਬਹੁਲਕ == 0) {
		ਲਿਖੋ("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_ਰਸਤਾ.len,
		       opt->output_delim, ctx.symlink_ਰਸਤਾ.buf, opt->output_delim);
		fflush(stdout);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਅੰਕੜੇ->ਬਹੁਲਕ = ctx.ਬਹੁਲਕ;
	batch_object_write(obj_name, scratch, opt, ਅੰਕੜੇ, ਨਿਰਾ, 0);

ਬਾਹਰਲਾ:
	object_context_release(&ctx);
}

ਸਥਿਰ ਪੂਰਨ get_remote_info(ਪੂਰਨ argc,
			   ਅਚਲ ਅੱਖਰ **argv,
			   ਬਣਤਰ fetch_object_info_results *results,
			   ਬਣਤਰ oid_array *object_info_oids)
{
	ਪੂਰਨ retval = 0;
	ਬਣਤਰ remote *remote = ਨਿਰਾ;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ transport *gtransport;

	remote = remote_get(argv[0]);
	ਜੇ (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	ਲਈ (ਮਾਪ_t i = 1; i < argc; i++) {
		ਜੇ (get_oid_hex(argv[i], &oid)) {
			ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(argv[i]);

			ਜੇ (len < the_hash_algo->hexsz && len >= 4) {
				ਮਾਪ_t j;
				ਲਈ (j = 0; j < len; j++)
					ਜੇ (!isxdigit(argv[i][j]))
						ਰੋਕੋ;
				ਜੇ (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (ਪੂਰਨ)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	ਜੇ (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ਨਿਰਾ);

	ਜੇ (!gtransport->smart_options) {
		retval = -1;
		ਜਾਓ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ਵਾਪਸ retval;
}

ਬਣਤਰ object_cb_ਅੰਕੜੇ {
	ਬਣਤਰ batch_options *opt;
	ਬਣਤਰ expand_ਅੰਕੜੇ *expand;
	ਬਣਤਰ oidset *ਵੇਖਿਆ;
	ਬਣਤਰ strbuf *scratch;
};

ਸਥਿਰ ਪੂਰਨ batch_object_cb(ਅਚਲ ਬਣਤਰ object_id *oid, ਖਾਲੀ *vdata)
{
	ਬਣਤਰ object_cb_ਅੰਕੜੇ *ਅੰਕੜੇ = vdata;
	oidcpy(&ਅੰਕੜੇ->expand->oid, oid);
	batch_object_write(ਨਿਰਾ, ਅੰਕੜੇ->scratch, ਅੰਕੜੇ->opt, ਅੰਕੜੇ->expand,
			   ਨਿਰਾ, 0);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ collect_object(ਅਚਲ ਬਣਤਰ object_id *oid,
			  ਬਣਤਰ packed_git *pack ਅਣਵਰਤਿਆ,
			  off_t ਵਿੱਥ ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *ਅੰਕੜੇ)
{
	oid_array_append(ਅੰਕੜੇ, oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ batch_unordered_object(ਅਚਲ ਬਣਤਰ object_id *oid,
				  ਬਣਤਰ packed_git *pack,
				  off_t ਵਿੱਥ,
				  ਖਾਲੀ *vdata)
{
	ਬਣਤਰ object_cb_ਅੰਕੜੇ *ਅੰਕੜੇ = vdata;

	ਜੇ (oidset_ਪਾਓ(ਅੰਕੜੇ->ਵੇਖਿਆ, oid))
		ਵਾਪਸ 0;

	oidcpy(&ਅੰਕੜੇ->expand->oid, oid);
	batch_object_write(ਨਿਰਾ, ਅੰਕੜੇ->scratch, ਅੰਕੜੇ->opt, ਅੰਕੜੇ->expand,
			   pack, ਵਿੱਥ);
	ਵਾਪਸ 0;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*parse_cmd_fn_t)(ਬਣਤਰ batch_options *, ਅਚਲ ਅੱਖਰ *,
			       ਬਣਤਰ strbuf *, ਬਣਤਰ expand_ਅੰਕੜੇ *);

ਬਣਤਰ queued_cmd {
	parse_cmd_fn_t fn;
	ਅੱਖਰ *ਸਤਰ;
};

ਸਥਿਰ ਖਾਲੀ parse_cmd_contents(ਬਣਤਰ batch_options *opt,
			     ਅਚਲ ਅੱਖਰ *ਸਤਰ,
			     ਬਣਤਰ strbuf *output,
			     ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਗਿਣਤੀ object_type *saved_typep = ਅੰਕੜੇ->info.typep;
	ਅੰਕੜੇ->info.typep = &ਅੰਕੜੇ->type;
	opt->batch_ਬਹੁਲਕ = BATCH_ਬਹੁਲਕ_CONTENTS;
	batch_one_object(ਸਤਰ, output, opt, ਅੰਕੜੇ);
	ਅੰਕੜੇ->info.typep = saved_typep;
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_info(ਬਣਤਰ batch_options *opt,
			   ਅਚਲ ਅੱਖਰ *ਸਤਰ,
			   ਬਣਤਰ strbuf *output,
			   ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	opt->batch_ਬਹੁਲਕ = BATCH_ਬਹੁਲਕ_INFO;
	batch_one_object(ਸਤਰ, output, opt, ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_mailmap(ਬਣਤਰ batch_options *opt ਅਣਵਰਤਿਆ,
			      ਅਚਲ ਅੱਖਰ *ਸਤਰ,
			      ਬਣਤਰ strbuf *output ਅਣਵਰਤਿਆ,
			      ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	use_mailmap = git_parse_maybe_bool(ਸਤਰ);

	ਜੇ (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), ਸਤਰ);

	ਜੇ (use_mailmap)
		ਲੱਦੋ_mailmap();
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_remote_object_info(ਬਣਤਰ batch_options *opt,
					 ਅਚਲ ਅੱਖਰ *ਸਤਰ, ਬਣਤਰ strbuf *output,
					 ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਪੂਰਨ ਗਿਣਤੀ_2;
	ਅਚਲ ਅੱਖਰ **argv;
	ਅੱਖਰ *ਸਤਰ_ਮੰਜ਼ਿਲ_ਵੰਡੋ;
	ਬਣਤਰ fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	ਬਣਤਰ oid_array object_info_oids = OID_ARRAY_INIT;

	ਜੇ (ਲੜੀ_ਲੰਬਾਈ(ਸਤਰ) >= MAX_REMOTE_OBJ_INFO_ਸਤਰ)
		die(_("remote-object-info command too long"));

	ਸਤਰ_ਮੰਜ਼ਿਲ_ਵੰਡੋ = xstrdup(ਸਤਰ);
	ਗਿਣਤੀ_2 = ਵੰਡੋ_cmdline(ਸਤਰ_ਮੰਜ਼ਿਲ_ਵੰਡੋ, &argv);
	ਜੇ (ਗਿਣਤੀ_2 < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    ਵੰਡੋ_cmdline_strerror(ਗਿਣਤੀ_2));
	ਜੇ (ਗਿਣਤੀ_2 - 1 > MAX_ALLOWED_OBJ_ਹੱਦਬੰਦੀ)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_ਹੱਦਬੰਦੀ);

	ਜੇ (ਅੰਕੜੇ->info.sizep)
		results.ਚਾਹੁੰਦਾ_ਮਾਪ = 1;
	ਜੇ (ਅੰਕੜੇ->info.typep)
		results.ਚਾਹੁੰਦਾ_type = 1;

	ਜੇ (get_remote_info(ਗਿਣਤੀ_2, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&ਅੰਕੜੇ->remote_allowed_atoms, 0);
	string_list_append(&ਅੰਕੜੇ->remote_allowed_atoms, "objectname");
	ਜੇ (results.sizes)
		string_list_append(&ਅੰਕੜੇ->remote_allowed_atoms, "objectsize");
	ਜੇ (results.types)
		string_list_append(&ਅੰਕੜੇ->remote_allowed_atoms, "objecttype");

	ਅੰਕੜੇ->skip_object_info = 1;
	ਲਈ (ਮਾਪ_t i = 0; i < results.nr; i++) {
		ਅੰਕੜੇ->oid = object_info_oids.oid[i];

		ਜੇ (results.unrecognized[i]) {
			report_object_ਸਥਿਤੀ(opt, oid_ਮੰਜ਼ਿਲ_hex(&ਅੰਕੜੇ->oid),
					     &ਅੰਕੜੇ->oid, "missing");
			ਜਾਰੀ;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		ਜੇ (results.sizes)
			ਅੰਕੜੇ->ਮਾਪ = results.sizes[i];

		ਜੇ (results.types)
			ਅੰਕੜੇ->type = results.types[i];

		opt->batch_ਬਹੁਲਕ = BATCH_ਬਹੁਲਕ_INFO;
		ਅੰਕੜੇ->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, ਅੰਕੜੇ, ਨਿਰਾ, 0);
		ਅੰਕੜੇ->is_remote = 0;
	}
	ਅੰਕੜੇ->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	ਮੁਕਤ(ਸਤਰ_ਮੰਜ਼ਿਲ_ਵੰਡੋ);
	ਮੁਕਤ(argv);
	oid_array_clear(&object_info_oids);
}

ਸਥਿਰ ਖਾਲੀ dispatch_calls(ਬਣਤਰ batch_options *opt,
		ਬਣਤਰ strbuf *output,
		ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ,
		ਬਣਤਰ queued_cmd *cmd,
		ਮਾਪ_t nr)
{
	ਜੇ (!opt->ਬਫਰ_output)
		die(_("flush is only for --buffer mode"));

	ਲਈ (ਮਾਪ_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].ਸਤਰ, output, ਅੰਕੜੇ);

	fflush(stdout);
}

ਸਥਿਰ ਖਾਲੀ free_cmds(ਬਣਤਰ queued_cmd *cmd, ਮਾਪ_t *nr)
{
	ਲਈ (ਮਾਪ_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].ਸਤਰ);

	*nr = 0;
}


ਸਥਿਰ ਅਚਲ ਬਣਤਰ parse_cmd {
	ਅਚਲ ਅੱਖਰ *name;
	parse_cmd_fn_t fn;
	ਬਿਨਾਨਿਸ਼ਾਨੀ takes_ਦਲੀਲਾਂ;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ਨਿਰਾ, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ਸਥਿਰ ਖਾਲੀ batch_objects_command(ਬਣਤਰ batch_options *opt,
				    ਬਣਤਰ strbuf *output,
				    ਬਣਤਰ expand_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ strbuf ਨਿਵੇਸ਼ = STRBUF_INIT;
	ਬਣਤਰ queued_cmd *queued_cmd = ਨਿਰਾ;
	ਮਾਪ_t alloc = 0, nr = 0;

	ਜਦੋਂ (strbuf_getdelim_strip_crlf(&ਨਿਵੇਸ਼, stdin, opt->ਨਿਵੇਸ਼_delim) != EOF) {
		ਅਚਲ ਬਣਤਰ parse_cmd *cmd = ਨਿਰਾ;
		ਅਚਲ ਅੱਖਰ *p = ਨਿਰਾ, *cmd_ਸਿਰਾ;
		ਬਣਤਰ queued_cmd call = {0};

		ਜੇ (!ਨਿਵੇਸ਼.len)
			die(_("empty command in input"));
		ਜੇ (ਖਾਲੀ_ਥਾਂ_ਹੈ(*ਨਿਵੇਸ਼.buf))
			die(_("whitespace before command: '%s'"), ਨਿਵੇਸ਼.buf);

		ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(commands); i++) {
			ਜੇ (!skip_ਅਗੇਤਰ(ਨਿਵੇਸ਼.buf, commands[i].name, &cmd_ਸਿਰਾ))
				ਜਾਰੀ;

			cmd = &commands[i];
			ਜੇ (cmd->takes_ਦਲੀਲਾਂ) {
				ਜੇ (*cmd_ਸਿਰਾ != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_ਸਿਰਾ + 1;
			} ਨਹੀਂਤਾਂ ਜੇ (*cmd_ਸਿਰਾ) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			ਰੋਕੋ;
		}

		ਜੇ (!cmd)
			die(_("unknown command: '%s'"), ਨਿਵੇਸ਼.buf);

		ਜੇ (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, ਅੰਕੜੇ, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} ਨਹੀਂਤਾਂ ਜੇ (!opt->ਬਫਰ_output) {
			cmd->fn(opt, p, output, ਅੰਕੜੇ);
		} ਨਹੀਂਤਾਂ {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.ਸਤਰ = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	ਜੇ (opt->ਬਫਰ_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, ਅੰਕੜੇ, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	ਮੁਕਤ(queued_cmd);
	strbuf_release(&ਨਿਵੇਸ਼);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*for_each_object_fn)(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ packed_git *pack,
				  off_t ਵਿੱਥ, ਖਾਲੀ *ਅੰਕੜੇ);

ਬਣਤਰ for_each_object_payload {
	for_each_object_fn callback;
	ਖਾਲੀ *payload;
};

ਸਥਿਰ ਪੂਰਨ batch_one_object_oi(ਅਚਲ ਬਣਤਰ object_id *oid,
			       ਬਣਤਰ object_info *oi,
			       ਖਾਲੀ *_payload)
{
	ਬਣਤਰ for_each_object_payload *payload = _payload;
	ਜੇ (oi && oi->ਸਰੋਤ_infop->ਸਰੋਤ->type == ODB_ਸਰੋਤ_PACKED)
		ਵਾਪਸ payload->callback(oid, oi->ਸਰੋਤ_infop->u.packed.pack,
					 oi->ਸਰੋਤ_infop->u.packed.ਵਿੱਥ,
					 payload->payload);
	ਵਾਪਸ payload->callback(oid, ਨਿਰਾ, 0, payload->payload);
}

ਸਥਿਰ ਖਾਲੀ batch_each_object(ਬਣਤਰ batch_options *opt,
			      for_each_object_fn callback,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ flags,
			      ਖਾਲੀ *_payload)
{
	ਬਣਤਰ for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	ਬਣਤਰ odb_ਸਰੋਤ_info ਸਰੋਤ_info;
	ਬਣਤਰ object_info oi = {
		.ਸਰੋਤ_infop = &ਸਰੋਤ_info,
	};
	ਬਣਤਰ odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ਸਥਿਰ ਪੂਰਨ batch_objects(ਬਣਤਰ batch_options *opt)
{
	ਬਣਤਰ strbuf ਨਿਵੇਸ਼ = STRBUF_INIT;
	ਬਣਤਰ strbuf output = STRBUF_INIT;
	ਬਣਤਰ expand_ਅੰਕੜੇ ਅੰਕੜੇ = EXPAND_ਅੰਕੜੇ_INIT;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਪੂਰਨ save_warning;
	ਪੂਰਨ retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	ਅੰਕੜੇ.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &ਅੰਕੜੇ);
	ਅੰਕੜੇ.mark_query = 0;
	strbuf_release(&output);
	ਜੇ (opt->transform_ਬਹੁਲਕ)
		ਅੰਕੜੇ.ਵੰਡੋ_on_whitespace = 1;

	ਜੇ (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ਨਿਰਾ;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	ਜੇ (opt->batch_ਬਹੁਲਕ == BATCH_ਬਹੁਲਕ_CONTENTS)
		ਅੰਕੜੇ.info.typep = &ਅੰਕੜੇ.type;

	ਜੇ (opt->all_objects) {
		ਬਣਤਰ object_cb_ਅੰਕੜੇ cb;
		ਬਣਤਰ object_info ਖਾਲੀ_ਥਾਵਾਂ = OBJECT_INFO_INIT;

		ਜੇ (!memcmp(&ਅੰਕੜੇ.info, &ਖਾਲੀ_ਥਾਵਾਂ, ਆਕਾਰ(ਖਾਲੀ_ਥਾਵਾਂ)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			ਅੰਕੜੇ.skip_object_info = 1;

		ਜੇ (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &ਅੰਕੜੇ;
		cb.scratch = &output;

		ਜੇ (opt->unordered) {
			ਬਣਤਰ oidset ਵੇਖਿਆ = OIDSET_INIT;

			cb.ਵੇਖਿਆ = &ਵੇਖਿਆ;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ਕ੍ਰਮ, &cb);

			oidset_clear(&ਵੇਖਿਆ);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ਵਾਪਸ 0;
	}

	/*
	 * We are going to call get_sha1 on a potentially very large number of
	 * objects. In most large cases, these will be actual object sha1s. The
	 * cost to double-check that each one is not also a ref (just so we can
	 * warn) ends up dwarfing the actual cost of the object lookups
	 * themselves. We can work around it by just turning off the warning.
	 */
	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	ਜੇ (opt->batch_ਬਹੁਲਕ == BATCH_ਬਹੁਲਕ_ਕਤਾਰ_AND_DISPATCH) {
		batch_objects_command(opt, &output, &ਅੰਕੜੇ);
		ਜਾਓ cleanup;
	}

	ਜਦੋਂ (strbuf_getdelim_strip_crlf(&ਨਿਵੇਸ਼, stdin, opt->ਨਿਵੇਸ਼_delim) != EOF) {
		ਜੇ (ਅੰਕੜੇ.ਵੰਡੋ_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			ਅੱਖਰ *p = strpbrk(ਨਿਵੇਸ਼.buf, " \t");
			ਜੇ (p) {
				ਜਦੋਂ (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			ਅੰਕੜੇ.rest = p;
		}

		batch_one_object(ਨਿਵੇਸ਼.buf, &output, opt, &ਅੰਕੜੇ);
	}

 cleanup:
	strbuf_release(&ਨਿਵੇਸ਼);
	strbuf_release(&output);
	string_list_clear(&ਅੰਕੜੇ.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ git_cat_file_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			       ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (userdiff_config(var, ਮੁੱਲ) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ batch_option_callback(ਅਚਲ ਬਣਤਰ option *opt,
				 ਅਚਲ ਅੱਖਰ *arg,
				 ਪੂਰਨ unset)
{
	ਬਣਤਰ batch_options *bo = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (bo->enabled) {
		ਵਾਪਸ error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	ਜੇ (!strcmp(opt->long_name, "batch"))
		bo->batch_ਬਹੁਲਕ = BATCH_ਬਹੁਲਕ_CONTENTS;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(opt->long_name, "batch-check"))
		bo->batch_ਬਹੁਲਕ = BATCH_ਬਹੁਲਕ_INFO;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(opt->long_name, "batch-command"))
		bo->batch_ਬਹੁਲਕ = BATCH_ਬਹੁਲਕ_ਕਤਾਰ_AND_DISPATCH;
	ਨਹੀਂਤਾਂ
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_cat_file(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ opt = 0;
	ਪੂਰਨ opt_cw = 0;
	ਪੂਰਨ opt_epts = 0;
	ਅਚਲ ਅੱਖਰ *exp_type = ਨਿਰਾ, *obj_name = ਨਿਰਾ;
	ਬਣਤਰ batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	ਪੂਰਨ unknown_type = 0;
	ਪੂਰਨ ਨਿਵੇਸ਼_nul_terminated = 0;
	ਪੂਰਨ nul_terminated = 0;
	ਪੂਰਨ ret;

	ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ਨਿਰਾ
	};
	ਅਚਲ ਬਣਤਰ option options[] = {
		/* Simple queries */
		OPT_ਸਮੂਹ(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ਨਿਰਾ, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ਨਿਰਾ, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_ਸਮੂਹ(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ਨਿਰਾ, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ਨਿਰਾ, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_ਸਮੂਹ(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', ਨਿਰਾ, &ਨਿਵੇਸ਼_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ਨਿਰਾ, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_ਸਮੂਹ(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.ਬਫਰ_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_ਸਮੂਹ(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_ਰਸਤਾ, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_cat_file_config, ਨਿਰਾ);

	batch.ਬਫਰ_output = -1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	ਜੇ (use_mailmap)
		ਲੱਦੋ_mailmap();

	ਚੋਣ (batch.objects_filter.choice) {
	ਹਾਲਤ LOFC_DISABLED:
		ਰੋਕੋ;
	ਹਾਲਤ LOFC_BLOB_NONE:
	ਹਾਲਤ LOFC_BLOB_ਹੱਦਬੰਦੀ:
	ਹਾਲਤ LOFC_OBJECT_TYPE:
		ਜੇ (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		ਰੋਕੋ;
	ਮੂਲ:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	ਜੇ (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	ਜੇ (force_ਰਸਤਾ && !opt_cw)
		usage_ਸੁਨੇਹਾ_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	ਜੇ (batch.enabled)
		;
	ਨਹੀਂਤਾਂ ਜੇ (batch.follow_symlinks)
		usage_ਸੁਨੇਹਾ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	ਨਹੀਂਤਾਂ ਜੇ (batch.ਬਫਰ_output >= 0)
		usage_ਸੁਨੇਹਾ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	ਨਹੀਂਤਾਂ ਜੇ (batch.all_objects)
		usage_ਸੁਨੇਹਾ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	ਨਹੀਂਤਾਂ ਜੇ (ਨਿਵੇਸ਼_nul_terminated)
		usage_ਸੁਨੇਹਾ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	ਨਹੀਂਤਾਂ ਜੇ (nul_terminated)
		usage_ਸੁਨੇਹਾ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.ਨਿਵੇਸ਼_delim = batch.output_delim = '\n';
	ਜੇ (ਨਿਵੇਸ਼_nul_terminated)
		batch.ਨਿਵੇਸ਼_delim = '\0';
	ਜੇ (nul_terminated)
		batch.ਨਿਵੇਸ਼_delim = batch.output_delim = '\0';

	/* Batch defaults */
	ਜੇ (batch.ਬਫਰ_output < 0)
		batch.ਬਫਰ_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	/* Return early if we're in batch mode? */
	ਜੇ (batch.enabled) {
		ਜੇ (opt_cw)
			batch.transform_ਬਹੁਲਕ = opt;
		ਨਹੀਂਤਾਂ ਜੇ (opt && opt != 'b')
			usage_ਸੁਨੇਹਾ_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		ਨਹੀਂਤਾਂ ਜੇ (argc)
			usage_ਸੁਨੇਹਾ_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (opt) {
		ਜੇ (!argc && opt == 'c')
			usage_ਸੁਨੇਹਾ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		ਨਹੀਂਤਾਂ ਜੇ (!argc && opt == 'w')
			usage_ਸੁਨੇਹਾ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		ਨਹੀਂਤਾਂ ਜੇ (!argc && opt_epts)
			usage_ਸੁਨੇਹਾ_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		ਨਹੀਂਤਾਂ ਜੇ (argc == 1)
			obj_name = argv[0];
		ਨਹੀਂਤਾਂ
			usage_ਸੁਨੇਹਾ_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} ਨਹੀਂਤਾਂ ਜੇ (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} ਨਹੀਂਤਾਂ ਜੇ (argc != 2) {
		usage_ਸੁਨੇਹਾ_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} ਨਹੀਂਤਾਂ ਜੇ (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

ਬਾਹਰਲਾ:
	list_objects_filter_release(&batch.objects_filter);
	ਵਾਪਸ ret;
}
