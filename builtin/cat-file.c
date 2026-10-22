/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "convert.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "ident.h"
#ಅನ್ತರ್ಭಾವ "list-objects-filter-options.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "userdiff.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "packfile.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/streaming.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"
#ಅನ್ತರ್ಭಾವ "alias.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "transport.h"
#ಅನ್ತರ್ಭಾವ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ALLOWED_OBJ_ಎಲ್ಲೆ 10000

/* Maximum input size permitted for the remote-object-info command. */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_REMOTE_OBJ_INFO_ಸಾಲು \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_ಎಲ್ಲೆ * (GIT_MAX_HEXSZ + 1))

ಎಣಿಕೆ batch_ಬಹುಲಕ {
	BATCH_ಬಹುಲಕ_CONTENTS,
	BATCH_ಬಹುಲಕ_INFO,
	BATCH_ಬಹುಲಕ_ಸರತಿ_AND_DISPATCH,
};

ರಚನೆ batch_options {
	ರಚನೆ list_objects_filter_options objects_filter;
	ಪೂರ್ಣಾಂಕ enabled;
	ಪೂರ್ಣಾಂಕ follow_symlinks;
	ಎಣಿಕೆ batch_ಬಹುಲಕ batch_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ ಬಫರ್_output;
	ಪೂರ್ಣಾಂಕ all_objects;
	ಪೂರ್ಣಾಂಕ unordered;
	ಪೂರ್ಣಾಂಕ transform_ಬಹುಲಕ; /* may be 'w' or 'c' for --filters or --textconv */
	ಅಕ್ಷರ ಒಳಹರಿವು_delim;
	ಅಕ್ಷರ output_delim;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format;
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *force_ಮಾರ್ಗ;

ಸ್ಥಿರ ರಚನೆ string_list mailmap = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_mailmap;

ಸ್ಥಿರ ಅಕ್ಷರ *replace_idents_using_mailmap(ಅಕ್ಷರ *, ಗಾತ್ರ_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬು_mailmap(ಶೂನ್ಯ)
{
	ಆದರೆ (mailmap.strdup_ಅಕ್ಷರಸರಣಿಗಳು)
		ಹಿಂತಿರುಗಿ;

	read_mailmap(the_repository, &mailmap);
}

ಸ್ಥಿರ ಅಕ್ಷರ *replace_idents_using_mailmap(ಅಕ್ಷರ *object_buf, ಗಾತ್ರ_t *ಗಾತ್ರ_2)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆಗಳು[] = { "author ", "committer ", "tagger ", ಖಾಲಿ };

	strbuf_attach(&sb, object_buf, *ಗಾತ್ರ_2, *ಗಾತ್ರ_2 + 1);
	apply_mailmap_ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ(&sb, ಶೀರ್ಷಿಕೆಗಳು, &mailmap);
	*ಗಾತ್ರ_2 = sb.len;
	ಹಿಂತಿರುಗಿ strbuf_detach(&sb, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ filter_object(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			 ಅಕ್ಷರ **buf, ಗಾತ್ರ_t *ಗಾತ್ರ_2)
{
	ಎಣಿಕೆ object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, ಗಾತ್ರ_2);
	ಆದರೆ (!*buf)
		ಹಿಂತಿರುಗಿ error(_("cannot read object %s '%s'"),
			     oid_ಗಮ್ಯ_hex(oid), ಮಾರ್ಗ);
	ಆದರೆ ((type == OBJ_BLOB) && S_ISREG(ಬಹುಲಕ)) {
		ರಚನೆ strbuf strbuf = STRBUF_INIT;
		ರಚನೆ checkout_metadata meta;

		init_checkout_metadata(&meta, ಖಾಲಿ, ಖಾಲಿ, oid);
		ಆದರೆ (convert_ಗಮ್ಯ_working_ವೃಕ್ಷ(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ, *buf, *ಗಾತ್ರ_2, &strbuf, &meta)) {
			ಬಿಡುಗಡೆ(*buf);
			*ಗಾತ್ರ_2 = strbuf.len;
			*buf = strbuf_detach(&strbuf, ಖಾಲಿ);
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಹರಿವು_blob(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಆದರೆ (odb_ಹರಿವು_blob_ಗಮ್ಯ_fd(the_repository->objects, 1, oid, ಖಾಲಿ, 0))
		die("unable to stream %s to stdout", oid_ಗಮ್ಯ_hex(oid));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cat_one_file(ಪೂರ್ಣಾಂಕ opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exp_type, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *obj_name)
{
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ object_id oid;
	ಎಣಿಕೆ object_type type;
	ಅಕ್ಷರ *buf;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ರಚನೆ object_context obj_context = {0};
	ರಚನೆ object_info oi = OBJECT_INFO_INIT;
	ಚಿಹ್ನೆರಹಿತ flags = OBJECT_INFO_LOOKUP_REPLACE;
	ಚಿಹ್ನೆರಹಿತ get_oid_flags =
		GET_OID_ದಾಖಲೆ_ಮಾರ್ಗ |
		GET_OID_ONLY_ಗಮ್ಯ_DIE |
		GET_OID_HASH_ANY;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = force_ಮಾರ್ಗ;
	ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ opt_cw = (opt == 'c' || opt == 'w');
	ಆದರೆ (!ಮಾರ್ಗ && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_ಮಾರ್ಗ;

	ಆದರೆ (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	ಆದರೆ (!ಮಾರ್ಗ)
		ಮಾರ್ಗ = obj_context.ಮಾರ್ಗ;
	ಆದರೆ (obj_context.ಬಹುಲಕ == S_IFINVALID)
		obj_context.ಬಹುಲಕ = 0100644;

	buf = ಖಾಲಿ;
	ಆಯ್ಕೆ (opt) {
	ಸ್ಥಿತಿ 't':
		oi.typep = &type;
		ಆದರೆ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		ಬರೆ("%s\n", type_name(type));
		ret = 0;
		ಹೋಗು cleanup;

	ಸ್ಥಿತಿ 's':
		oi.sizep = &ಗಾತ್ರ_2;

		ಆದರೆ (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (ಶೂನ್ಯ**)&buf;
		}

		ಆದರೆ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		ಆದರೆ (use_mailmap && (type == OBJ_COMMIT || type == OBJ_ಟ್ಯಾಗ್))
			buf = replace_idents_using_mailmap(buf, &ಗಾತ್ರ_2);

		ಬರೆ("%"PRIuMAX"\n", (uintmax_t)ಗಾತ್ರ_2);
		ret = 0;
		ಹೋಗು cleanup;

	ಸ್ಥಿತಿ 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		ಹೋಗು cleanup;

	ಸ್ಥಿತಿ 'w':

		ಆದರೆ (filter_object(ಮಾರ್ಗ, obj_context.ಬಹುಲಕ,
				  &oid, &buf, &ಗಾತ್ರ_2)) {
			ret = -1;
			ಹೋಗು cleanup;
		}
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ 'c':
	{
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_ul = 0;
		ಪೂರ್ಣಾಂಕ textconv_ret = textconv_object(the_repository, ಮಾರ್ಗ,
						   obj_context.ಬಹುಲಕ, &oid, 1,
						   &buf, &ಗಾತ್ರ_ul);
		ಗಾತ್ರ_2 = ಗಾತ್ರ_ul;
		ಆದರೆ (textconv_ret)
			ನಿಲ್ಲಿಸು;
	}
		/* else fallthrough */

	ಸ್ಥಿತಿ 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ಖಾಲಿ);
		ಆದರೆ (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		ಆದರೆ (type == OBJ_ವೃಕ್ಷ) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ls_ವಾದಗಳು[3] = { ಖಾಲಿ };
			ls_ವಾದಗಳು[0] =  "ls-tree";
			ls_ವಾದಗಳು[1] =  obj_name;
			ret = cmd_ls_ವೃಕ್ಷ(2, ls_ವಾದಗಳು, ಖಾಲಿ, the_repository);
			ಹೋಗು cleanup;
		}

		ಆದರೆ (type == OBJ_BLOB) {
			ret = ಹರಿವು_blob(&oid);
			ಹೋಗು cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &ಗಾತ್ರ_2);
		ಆದರೆ (!buf)
			die("Cannot read object %s", obj_name);

		ಆದರೆ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &ಗಾತ್ರ_2);

		/* otherwise just spit out the data */
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ 0:
	{
		ಎಣಿಕೆ object_type exp_type_id = type_from_string(exp_type);

		ಆದರೆ (exp_type_id == OBJ_BLOB) {
			ರಚನೆ object_id blob_oid;
			ಆದರೆ (odb_read_object_info(the_repository->objects,
						 &oid, ಖಾಲಿ) == OBJ_ಟ್ಯಾಗ್) {
				ಅಕ್ಷರ *ಬಫರ್ = odb_read_object(the_repository->objects,
							       &oid, &type, &ಗಾತ್ರ_2);
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ;

				ಆದರೆ (!ಬಫರ್)
					die(_("unable to read %s"), oid_ಗಮ್ಯ_hex(&oid));

				ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ಬಫರ್, "object ", &ಗುರಿ) ||
				    get_oid_hex_algop(ಗುರಿ, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_ಗಮ್ಯ_hex(&oid));
				ಬಿಡುಗಡೆ(ಬಫರ್);
			} ಇಲ್ಲದಿದ್ದರೆ
				oidcpy(&blob_oid, &oid);

			ಆದರೆ (odb_read_object_info(the_repository->objects,
						 &blob_oid, ಖಾಲಿ) == OBJ_BLOB) {
				ret = ಹರಿವು_blob(&blob_oid);
				ಹೋಗು cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &ಗಾತ್ರ_2, ಖಾಲಿ);

		ಆದರೆ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &ಗಾತ್ರ_2);
		ನಿಲ್ಲಿಸು;
	}
	ಪೂರ್ವನಿಯೋಜಿತ:
		die("git cat-file: unknown option: %s", exp_type);
	}

	ಆದರೆ (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, ಗಾತ್ರ_2);
	ret = 0;
cleanup:
	ಬಿಡುಗಡೆ(buf);
	object_context_release(&obj_context);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ expand_ದತ್ತಾಂಶ {
	ರಚನೆ object_id oid;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಬಹುಲಕ;
	off_t disk_ಗಾತ್ರ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rest;
	ರಚನೆ object_id delta_ತಳ_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	ಪೂರ್ಣಾಂಕ mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	ರಚನೆ object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	ಚಿಹ್ನೆರಹಿತ skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	ಚಿಹ್ನೆರಹಿತ is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	ರಚನೆ string_list remote_allowed_atoms;
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXPAND_ದತ್ತಾಂಶ_INIT  {  \
	.ಬಹುಲಕ = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_atom(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *atom, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಪೂರ್ಣಾಂಕ slen)
{
	ಪೂರ್ಣಾಂಕ alen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(atom);
	ಹಿಂತಿರುಗಿ alen == slen && !memcmp(atom, s, alen);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ expand_atom(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *atom, ಪೂರ್ಣಾಂಕ len,
		       ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಆದರೆ (ದತ್ತಾಂಶ->is_remote) {
		ಗಾತ್ರ_t i;
		ಗಾಗಿ (i = 0; i < ದತ್ತಾಂಶ->remote_allowed_atoms.nr; i++)
			ಆದರೆ (is_atom(ದತ್ತಾಂಶ->remote_allowed_atoms.ವಸ್ತುಗಳು[i].string,
				    atom, len))
				ನಿಲ್ಲಿಸು;
		ಆದರೆ (i == ದತ್ತಾಂಶ->remote_allowed_atoms.nr)
			ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (is_atom("objectname", atom, len)) {
		ಆದರೆ (!ದತ್ತಾಂಶ->mark_query)
			strbuf_ಕೂಡಿಸು_oid_hex(sb, &ದತ್ತಾಂಶ->oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_atom("objecttype", atom, len)) {
		ಆದರೆ (ದತ್ತಾಂಶ->mark_query) {
			ದತ್ತಾಂಶ->info.typep = &ದತ್ತಾಂಶ->type;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *t = type_name(ದತ್ತಾಂಶ->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_atom("objectsize", atom, len)) {
		ಆದರೆ (ದತ್ತಾಂಶ->mark_query)
			ದತ್ತಾಂಶ->info.sizep = &ದತ್ತಾಂಶ->ಗಾತ್ರ_2;
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_ಕೂಡಿಸು_uint(sb, ದತ್ತಾಂಶ->ಗಾತ್ರ_2);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_atom("objectsize:disk", atom, len)) {
		ಆದರೆ (ದತ್ತಾಂಶ->mark_query)
			ದತ್ತಾಂಶ->info.disk_sizep = &ದತ್ತಾಂಶ->disk_ಗಾತ್ರ;
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_ಕೂಡಿಸು_uint(sb, ದತ್ತಾಂಶ->disk_ಗಾತ್ರ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_atom("rest", atom, len)) {
		ಆದರೆ (ದತ್ತಾಂಶ->mark_query)
			ದತ್ತಾಂಶ->ವಿಭಜಿಸು_on_whitespace = 1;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದತ್ತಾಂಶ->rest)
			strbuf_addstr(sb, ದತ್ತಾಂಶ->rest);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_atom("deltabase", atom, len)) {
		ಆದರೆ (ದತ್ತಾಂಶ->mark_query)
			ದತ್ತಾಂಶ->info.delta_ತಳ_oid = &ದತ್ತಾಂಶ->delta_ತಳ_oid;
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_ಕೂಡಿಸು_oid_hex(sb, &ದತ್ತಾಂಶ->delta_ತಳ_oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_atom("objectmode", atom, len)) {
		ಆದರೆ (!ದತ್ತಾಂಶ->mark_query && !(S_IFINVALID == ದತ್ತಾಂಶ->ಬಹುಲಕ))
			strbuf_addf(sb, "%06o", ದತ್ತಾಂಶ->ಬಹುಲಕ);
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ expand_format(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆರಂಭ,
			  ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಆಗ (strbuf_expand_ಹೆಜ್ಜೆ(sb, &ಆರಂಭ)) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ;

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ಆರಂಭ, "%", &ಆರಂಭ) || *ಆರಂಭ != '(')
			strbuf_addch(sb, '%');
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಕೊನೆ = strchr(ಆರಂಭ + 1, ')')) &&
			 expand_atom(sb, ಆರಂಭ + 1, ಕೊನೆ - ಆರಂಭ - 1, ದತ್ತಾಂಶ))
			ಆರಂಭ = ಕೊನೆ + 1;
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_expand_bad_format(ಆರಂಭ, "cat-file");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ batch_write(ರಚನೆ batch_options *opt, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len)
{
	ಆದರೆ (opt->ಬಫರ್_output) {
		ಆದರೆ (fwrite(ದತ್ತಾಂಶ, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} ಇಲ್ಲದಿದ್ದರೆ
		write_or_die(1, ದತ್ತಾಂಶ, len);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_object_or_die(ರಚನೆ batch_options *opt, ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid = &ದತ್ತಾಂಶ->oid;

	ಪ್ರತಿಜ್ಞಾ(ದತ್ತಾಂಶ->info.typep);

	ಆದರೆ (ದತ್ತಾಂಶ->type == OBJ_BLOB) {
		ಆದರೆ (opt->ಬಫರ್_output)
			fflush(stdout);
		ಆದರೆ (opt->transform_ಬಹುಲಕ) {
			ಅಕ್ಷರ *contents;
			ಗಾತ್ರ_t ಗಾತ್ರ_2;

			ಆದರೆ (!ದತ್ತಾಂಶ->rest)
				die("missing path for '%s'", oid_ಗಮ್ಯ_hex(oid));

			ಆದರೆ (opt->transform_ಬಹುಲಕ == 'w') {
				ಆದರೆ (filter_object(ದತ್ತಾಂಶ->rest, 0100644, oid,
						  &contents, &ಗಾತ್ರ_2))
					die("could not convert '%s' %s",
					    oid_ಗಮ್ಯ_hex(oid), ದತ್ತಾಂಶ->rest);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt->transform_ಬಹುಲಕ == 'c') {
				ಎಣಿಕೆ object_type type;
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_ul = 0;
				ಆದರೆ (textconv_object(the_repository,
						    ದತ್ತಾಂಶ->rest, 0100644, oid,
						    1, &contents, &ಗಾತ್ರ_ul))
					ಗಾತ್ರ_2 = ಗಾತ್ರ_ul;
				ಇಲ್ಲದಿದ್ದರೆ
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &ಗಾತ್ರ_2);
				ಆದರೆ (!contents)
					die("could not convert '%s' %s",
					    oid_ಗಮ್ಯ_hex(oid), ದತ್ತಾಂಶ->rest);
			} ಇಲ್ಲದಿದ್ದರೆ
				BUG("invalid transform_mode: %c", opt->transform_ಬಹುಲಕ);
			batch_write(opt, contents, ಗಾತ್ರ_2);
			ಬಿಡುಗಡೆ(contents);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಹರಿವು_blob(oid);
		}
	}
	ಇಲ್ಲದಿದ್ದರೆ {
		ಎಣಿಕೆ object_type type;
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಶೂನ್ಯ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &ಗಾತ್ರ_2);
		ಆದರೆ (!contents)
			die("object %s disappeared", oid_ಗಮ್ಯ_hex(oid));

		ಆದರೆ (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &ಗಾತ್ರ_2);

		ಆದರೆ (type != ದತ್ತಾಂಶ->type)
			die("object %s changed type!?", oid_ಗಮ್ಯ_hex(oid));
		ಆದರೆ (ದತ್ತಾಂಶ->info.sizep && ಗಾತ್ರ_2 != ದತ್ತಾಂಶ->ಗಾತ್ರ_2 && !use_mailmap)
			die("object %s changed size!?", oid_ಗಮ್ಯ_hex(oid));

		batch_write(opt, contents, ಗಾತ್ರ_2);
		ಬಿಡುಗಡೆ(contents);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ print_default_format(ರಚನೆ strbuf *scratch, ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
				 ರಚನೆ batch_options *opt)
{
	strbuf_ಕೂಡಿಸು_oid_hex(scratch, &ದತ್ತಾಂಶ->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(ದತ್ತಾಂಶ->type));
	strbuf_addch(scratch, ' ');
	strbuf_ಕೂಡಿಸು_uint(scratch, ದತ್ತಾಂಶ->ಗಾತ್ರ_2);
	strbuf_addch(scratch, opt->output_delim);
}

ಸ್ಥಿರ ಶೂನ್ಯ report_object_ಸ್ಥಿತಿ(ರಚನೆ batch_options *opt,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *obj_name,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿತಿ_2)
{
	ಬರೆ("%s %s%c", obj_name ? obj_name : oid_ಗಮ್ಯ_hex(oid),
	       ಸ್ಥಿತಿ_2, opt->output_delim);
	fflush(stdout);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
ಸ್ಥಿರ ಶೂನ್ಯ batch_object_write(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *obj_name,
			       ರಚನೆ strbuf *scratch,
			       ರಚನೆ batch_options *opt,
			       ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			       ರಚನೆ packed_git *pack,
			       off_t ಸ್ಥಾನಾಂತರ)
{
	ಆದರೆ (!ದತ್ತಾಂಶ->skip_object_info) {
		ಪೂರ್ಣಾಂಕ ret;

		ಆದರೆ (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_ಎಲ್ಲೆ ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			ದತ್ತಾಂಶ->info.typep = &ದತ್ತಾಂಶ->type;
		ಆದರೆ (opt->objects_filter.choice == LOFC_BLOB_ಎಲ್ಲೆ)
			ದತ್ತಾಂಶ->info.sizep = &ದತ್ತಾಂಶ->ಗಾತ್ರ_2;

		ಆದರೆ (pack)
			ret = packed_object_info(ಖಾಲಿ, pack, ಸ್ಥಾನಾಂತರ, &ದತ್ತಾಂಶ->info);
		ಇಲ್ಲದಿದ್ದರೆ
			ret = odb_read_object_info_extended(the_repository->objects,
							    &ದತ್ತಾಂಶ->oid, &ದತ್ತಾಂಶ->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		ಆದರೆ (ret < 0) {
			ಆದರೆ (ದತ್ತಾಂಶ->ಬಹುಲಕ == S_IFGITLINK)
				report_object_ಸ್ಥಿತಿ(opt, ಖಾಲಿ, &ದತ್ತಾಂಶ->oid, "submodule");
			ಇಲ್ಲದಿದ್ದರೆ
				report_object_ಸ್ಥಿತಿ(opt, obj_name, &ದತ್ತಾಂಶ->oid, "missing");
			ಹಿಂತಿರುಗಿ;
		}

		ಆಯ್ಕೆ (opt->objects_filter.choice) {
		ಸ್ಥಿತಿ LOFC_DISABLED:
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ LOFC_BLOB_NONE:
			ಆದರೆ (ದತ್ತಾಂಶ->type == OBJ_BLOB) {
				ಆದರೆ (!opt->all_objects)
					report_object_ಸ್ಥಿತಿ(opt, obj_name,
							     &ದತ್ತಾಂಶ->oid, "excluded");
				ಹಿಂತಿರುಗಿ;
			}
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ LOFC_BLOB_ಎಲ್ಲೆ:
			ಆದರೆ (ದತ್ತಾಂಶ->type == OBJ_BLOB &&
			    ದತ್ತಾಂಶ->ಗಾತ್ರ_2 >= opt->objects_filter.blob_ಎಲ್ಲೆ_ಮೌಲ್ಯ) {
				ಆದರೆ (!opt->all_objects)
					report_object_ಸ್ಥಿತಿ(opt, obj_name,
							     &ದತ್ತಾಂಶ->oid, "excluded");
				ಹಿಂತಿರುಗಿ;
			}
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ LOFC_OBJECT_TYPE:
			ಆದರೆ (ದತ್ತಾಂಶ->type != opt->objects_filter.object_type) {
				ಆದರೆ (!opt->all_objects)
					report_object_ಸ್ಥಿತಿ(opt, obj_name,
							     &ದತ್ತಾಂಶ->oid, "excluded");
				ಹಿಂತಿರುಗಿ;
			}
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("unsupported objects filter");
		}

		ಆದರೆ (use_mailmap && (ದತ್ತಾಂಶ->type == OBJ_COMMIT || ದತ್ತಾಂಶ->type == OBJ_ಟ್ಯಾಗ್)) {
			ಅಕ್ಷರ *buf = ಖಾಲಿ;

			buf = odb_read_object(the_repository->objects, &ದತ್ತಾಂಶ->oid,
					      &ದತ್ತಾಂಶ->type, &ದತ್ತಾಂಶ->ಗಾತ್ರ_2);
			ಆದರೆ (!buf)
				die(_("unable to read %s"), oid_ಗಮ್ಯ_hex(&ದತ್ತಾಂಶ->oid));
			buf = replace_idents_using_mailmap(buf, &ದತ್ತಾಂಶ->ಗಾತ್ರ_2);

			ಬಿಡುಗಡೆ(buf);
		}
	}

	strbuf_reset(scratch);

	ಆದರೆ (!opt->format) {
		print_default_format(scratch, ದತ್ತಾಂಶ, opt);
	} ಇಲ್ಲದಿದ್ದರೆ {
		expand_format(scratch, opt->format, ದತ್ತಾಂಶ);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	ಆದರೆ (opt->batch_ಬಹುಲಕ == BATCH_ಬಹುಲಕ_CONTENTS) {
		print_object_or_die(opt, ದತ್ತಾಂಶ);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ batch_one_object(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *obj_name,
			     ರಚನೆ strbuf *scratch,
			     ರಚನೆ batch_options *opt,
			     ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ object_context ctx = {0};
	ಪೂರ್ಣಾಂಕ flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	ಎಣಿಕೆ get_oid_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ;

	ಫಲಿತಾಂಶ = get_oid_with_context(the_repository, obj_name,
				      flags, &ದತ್ತಾಂಶ->oid, &ctx);
	ಆದರೆ (ಫಲಿತಾಂಶ != ಸಿಕ್ಕಿತು) {
		ಆಯ್ಕೆ (ಫಲಿತಾಂಶ) {
		ಸ್ಥಿತಿ MISSING_OBJECT:
			report_object_ಸ್ಥಿತಿ(opt, obj_name, &ದತ್ತಾಂಶ->oid, "missing");
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SHORT_NAME_AMBIGUOUS:
			report_object_ಸ್ಥಿತಿ(opt, obj_name, &ದತ್ತಾಂಶ->oid, "ambiguous");
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DANGLING_SYMLINK:
			ಬರೆ("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SYMLINK_LOOP:
			ಬರೆ("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ NOT_DIR:
			ಬರೆ("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("unknown get_sha1_with_context result %d\n",
			       ಫಲಿತಾಂಶ);
			ನಿಲ್ಲಿಸು;
		}
		fflush(stdout);

		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (ctx.ಬಹುಲಕ == 0) {
		ಬರೆ("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_ಮಾರ್ಗ.len,
		       opt->output_delim, ctx.symlink_ಮಾರ್ಗ.buf, opt->output_delim);
		fflush(stdout);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ದತ್ತಾಂಶ->ಬಹುಲಕ = ctx.ಬಹುಲಕ;
	batch_object_write(obj_name, scratch, opt, ದತ್ತಾಂಶ, ಖಾಲಿ, 0);

ಹೊರಹೋಗು:
	object_context_release(&ctx);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_remote_info(ಪೂರ್ಣಾಂಕ argc,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			   ರಚನೆ fetch_object_info_results *results,
			   ರಚನೆ oid_array *object_info_oids)
{
	ಪೂರ್ಣಾಂಕ retval = 0;
	ರಚನೆ remote *remote = ಖಾಲಿ;
	ರಚನೆ object_id oid;
	ರಚನೆ transport *gtransport;

	remote = remote_get(argv[0]);
	ಆದರೆ (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	ಗಾಗಿ (ಗಾತ್ರ_t i = 1; i < argc; i++) {
		ಆದರೆ (get_oid_hex(argv[i], &oid)) {
			ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(argv[i]);

			ಆದರೆ (len < the_hash_algo->hexsz && len >= 4) {
				ಗಾತ್ರ_t j;
				ಗಾಗಿ (j = 0; j < len; j++)
					ಆದರೆ (!isxdigit(argv[i][j]))
						ನಿಲ್ಲಿಸು;
				ಆದರೆ (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (ಪೂರ್ಣಾಂಕ)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	ಆದರೆ (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ಖಾಲಿ);

	ಆದರೆ (!gtransport->smart_options) {
		retval = -1;
		ಹೋಗು cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ಹಿಂತಿರುಗಿ retval;
}

ರಚನೆ object_cb_ದತ್ತಾಂಶ {
	ರಚನೆ batch_options *opt;
	ರಚನೆ expand_ದತ್ತಾಂಶ *expand;
	ರಚನೆ oidset *ನೋಡಿದ;
	ರಚನೆ strbuf *scratch;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ batch_object_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಶೂನ್ಯ *vdata)
{
	ರಚನೆ object_cb_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = vdata;
	oidcpy(&ದತ್ತಾಂಶ->expand->oid, oid);
	batch_object_write(ಖಾಲಿ, ದತ್ತಾಂಶ->scratch, ದತ್ತಾಂಶ->opt, ದತ್ತಾಂಶ->expand,
			   ಖಾಲಿ, 0);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collect_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			  ರಚನೆ packed_git *pack ಬಳಸದ,
			  off_t ಸ್ಥಾನಾಂತರ ಬಳಸದ,
			  ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	oid_array_append(ದತ್ತಾಂಶ, oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ batch_unordered_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
				  ರಚನೆ packed_git *pack,
				  off_t ಸ್ಥಾನಾಂತರ,
				  ಶೂನ್ಯ *vdata)
{
	ರಚನೆ object_cb_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = vdata;

	ಆದರೆ (oidset_ಸೇರಿಸು(ದತ್ತಾಂಶ->ನೋಡಿದ, oid))
		ಹಿಂತಿರುಗಿ 0;

	oidcpy(&ದತ್ತಾಂಶ->expand->oid, oid);
	batch_object_write(ಖಾಲಿ, ದತ್ತಾಂಶ->scratch, ದತ್ತಾಂಶ->opt, ದತ್ತಾಂಶ->expand,
			   pack, ಸ್ಥಾನಾಂತರ);
	ಹಿಂತಿರುಗಿ 0;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*parse_cmd_fn_t)(ರಚನೆ batch_options *, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *,
			       ರಚನೆ strbuf *, ರಚನೆ expand_ದತ್ತಾಂಶ *);

ರಚನೆ queued_cmd {
	parse_cmd_fn_t fn;
	ಅಕ್ಷರ *ಸಾಲು;
};

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_contents(ರಚನೆ batch_options *opt,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಾಲು,
			     ರಚನೆ strbuf *output,
			     ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಎಣಿಕೆ object_type *saved_typep = ದತ್ತಾಂಶ->info.typep;
	ದತ್ತಾಂಶ->info.typep = &ದತ್ತಾಂಶ->type;
	opt->batch_ಬಹುಲಕ = BATCH_ಬಹುಲಕ_CONTENTS;
	batch_one_object(ಸಾಲು, output, opt, ದತ್ತಾಂಶ);
	ದತ್ತಾಂಶ->info.typep = saved_typep;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_info(ರಚನೆ batch_options *opt,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಾಲು,
			   ರಚನೆ strbuf *output,
			   ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	opt->batch_ಬಹುಲಕ = BATCH_ಬಹುಲಕ_INFO;
	batch_one_object(ಸಾಲು, output, opt, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_mailmap(ರಚನೆ batch_options *opt ಬಳಸದ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಾಲು,
			      ರಚನೆ strbuf *output ಬಳಸದ,
			      ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	use_mailmap = git_parse_maybe_bool(ಸಾಲು);

	ಆದರೆ (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), ಸಾಲು);

	ಆದರೆ (use_mailmap)
		ತುಂಬು_mailmap();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_remote_object_info(ರಚನೆ batch_options *opt,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಾಲು, ರಚನೆ strbuf *output,
					 ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
	ಅಕ್ಷರ *ಸಾಲು_ಗಮ್ಯ_ವಿಭಜಿಸು;
	ರಚನೆ fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	ರಚನೆ oid_array object_info_oids = OID_ARRAY_INIT;

	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಾಲು) >= MAX_REMOTE_OBJ_INFO_ಸಾಲು)
		die(_("remote-object-info command too long"));

	ಸಾಲು_ಗಮ್ಯ_ವಿಭಜಿಸು = xstrdup(ಸಾಲು);
	ಎಣಿಕೆ_2 = ವಿಭಜಿಸು_cmdline(ಸಾಲು_ಗಮ್ಯ_ವಿಭಜಿಸು, &argv);
	ಆದರೆ (ಎಣಿಕೆ_2 < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    ವಿಭಜಿಸು_cmdline_strerror(ಎಣಿಕೆ_2));
	ಆದರೆ (ಎಣಿಕೆ_2 - 1 > MAX_ALLOWED_OBJ_ಎಲ್ಲೆ)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_ಎಲ್ಲೆ);

	ಆದರೆ (ದತ್ತಾಂಶ->info.sizep)
		results.ಬಯಸುತ್ತದೆ_ಗಾತ್ರ = 1;
	ಆದರೆ (ದತ್ತಾಂಶ->info.typep)
		results.ಬಯಸುತ್ತದೆ_type = 1;

	ಆದರೆ (get_remote_info(ಎಣಿಕೆ_2, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&ದತ್ತಾಂಶ->remote_allowed_atoms, 0);
	string_list_append(&ದತ್ತಾಂಶ->remote_allowed_atoms, "objectname");
	ಆದರೆ (results.sizes)
		string_list_append(&ದತ್ತಾಂಶ->remote_allowed_atoms, "objectsize");
	ಆದರೆ (results.types)
		string_list_append(&ದತ್ತಾಂಶ->remote_allowed_atoms, "objecttype");

	ದತ್ತಾಂಶ->skip_object_info = 1;
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < results.nr; i++) {
		ದತ್ತಾಂಶ->oid = object_info_oids.oid[i];

		ಆದರೆ (results.unrecognized[i]) {
			report_object_ಸ್ಥಿತಿ(opt, oid_ಗಮ್ಯ_hex(&ದತ್ತಾಂಶ->oid),
					     &ದತ್ತಾಂಶ->oid, "missing");
			ಮುಂದುವರಿ;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		ಆದರೆ (results.sizes)
			ದತ್ತಾಂಶ->ಗಾತ್ರ_2 = results.sizes[i];

		ಆದರೆ (results.types)
			ದತ್ತಾಂಶ->type = results.types[i];

		opt->batch_ಬಹುಲಕ = BATCH_ಬಹುಲಕ_INFO;
		ದತ್ತಾಂಶ->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, ದತ್ತಾಂಶ, ಖಾಲಿ, 0);
		ದತ್ತಾಂಶ->is_remote = 0;
	}
	ದತ್ತಾಂಶ->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	ಬಿಡುಗಡೆ(ಸಾಲು_ಗಮ್ಯ_ವಿಭಜಿಸು);
	ಬಿಡುಗಡೆ(argv);
	oid_array_clear(&object_info_oids);
}

ಸ್ಥಿರ ಶೂನ್ಯ dispatch_calls(ರಚನೆ batch_options *opt,
		ರಚನೆ strbuf *output,
		ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
		ರಚನೆ queued_cmd *cmd,
		ಗಾತ್ರ_t nr)
{
	ಆದರೆ (!opt->ಬಫರ್_output)
		die(_("flush is only for --buffer mode"));

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].ಸಾಲು, output, ದತ್ತಾಂಶ);

	fflush(stdout);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_cmds(ರಚನೆ queued_cmd *cmd, ಗಾತ್ರ_t *nr)
{
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].ಸಾಲು);

	*nr = 0;
}


ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ parse_cmd {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	parse_cmd_fn_t fn;
	ಚಿಹ್ನೆರಹಿತ takes_ವಾದಗಳು;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ಖಾಲಿ, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ಸ್ಥಿರ ಶೂನ್ಯ batch_objects_command(ರಚನೆ batch_options *opt,
				    ರಚನೆ strbuf *output,
				    ರಚನೆ expand_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ strbuf ಒಳಹರಿವು = STRBUF_INIT;
	ರಚನೆ queued_cmd *queued_cmd = ಖಾಲಿ;
	ಗಾತ್ರ_t alloc = 0, nr = 0;

	ಆಗ (strbuf_getdelim_strip_crlf(&ಒಳಹರಿವು, stdin, opt->ಒಳಹರಿವು_delim) != EOF) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ parse_cmd *cmd = ಖಾಲಿ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = ಖಾಲಿ, *cmd_ಕೊನೆ;
		ರಚನೆ queued_cmd call = {0};

		ಆದರೆ (!ಒಳಹರಿವು.len)
			die(_("empty command in input"));
		ಆದರೆ (ಖಾಲಿಯೇ(*ಒಳಹರಿವು.buf))
			die(_("whitespace before command: '%s'"), ಒಳಹರಿವು.buf);

		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(commands); i++) {
			ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ಒಳಹರಿವು.buf, commands[i].name, &cmd_ಕೊನೆ))
				ಮುಂದುವರಿ;

			cmd = &commands[i];
			ಆದರೆ (cmd->takes_ವಾದಗಳು) {
				ಆದರೆ (*cmd_ಕೊನೆ != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_ಕೊನೆ + 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*cmd_ಕೊನೆ) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (!cmd)
			die(_("unknown command: '%s'"), ಒಳಹರಿವು.buf);

		ಆದರೆ (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, ದತ್ತಾಂಶ, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!opt->ಬಫರ್_output) {
			cmd->fn(opt, p, output, ದತ್ತಾಂಶ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.ಸಾಲು = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	ಆದರೆ (opt->ಬಫರ್_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, ದತ್ತಾಂಶ, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	ಬಿಡುಗಡೆ(queued_cmd);
	strbuf_release(&ಒಳಹರಿವು);
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*for_each_object_fn)(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ packed_git *pack,
				  off_t ಸ್ಥಾನಾಂತರ, ಶೂನ್ಯ *ದತ್ತಾಂಶ);

ರಚನೆ for_each_object_payload {
	for_each_object_fn callback;
	ಶೂನ್ಯ *payload;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ batch_one_object_oi(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			       ರಚನೆ object_info *oi,
			       ಶೂನ್ಯ *_payload)
{
	ರಚನೆ for_each_object_payload *payload = _payload;
	ಆದರೆ (oi && oi->ಮೂಲ_infop->ಮೂಲ->type == ODB_ಮೂಲ_PACKED)
		ಹಿಂತಿರುಗಿ payload->callback(oid, oi->ಮೂಲ_infop->u.packed.pack,
					 oi->ಮೂಲ_infop->u.packed.ಸ್ಥಾನಾಂತರ,
					 payload->payload);
	ಹಿಂತಿರುಗಿ payload->callback(oid, ಖಾಲಿ, 0, payload->payload);
}

ಸ್ಥಿರ ಶೂನ್ಯ batch_each_object(ರಚನೆ batch_options *opt,
			      for_each_object_fn callback,
			      ಚಿಹ್ನೆರಹಿತ flags,
			      ಶೂನ್ಯ *_payload)
{
	ರಚನೆ for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	ರಚನೆ odb_ಮೂಲ_info ಮೂಲ_info;
	ರಚನೆ object_info oi = {
		.ಮೂಲ_infop = &ಮೂಲ_info,
	};
	ರಚನೆ odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ batch_objects(ರಚನೆ batch_options *opt)
{
	ರಚನೆ strbuf ಒಳಹರಿವು = STRBUF_INIT;
	ರಚನೆ strbuf output = STRBUF_INIT;
	ರಚನೆ expand_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = EXPAND_ದತ್ತಾಂಶ_INIT;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಂಕ save_warning;
	ಪೂರ್ಣಾಂಕ retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	ದತ್ತಾಂಶ.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &ದತ್ತಾಂಶ);
	ದತ್ತಾಂಶ.mark_query = 0;
	strbuf_release(&output);
	ಆದರೆ (opt->transform_ಬಹುಲಕ)
		ದತ್ತಾಂಶ.ವಿಭಜಿಸು_on_whitespace = 1;

	ಆದರೆ (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ಖಾಲಿ;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	ಆದರೆ (opt->batch_ಬಹುಲಕ == BATCH_ಬಹುಲಕ_CONTENTS)
		ದತ್ತಾಂಶ.info.typep = &ದತ್ತಾಂಶ.type;

	ಆದರೆ (opt->all_objects) {
		ರಚನೆ object_cb_ದತ್ತಾಂಶ cb;
		ರಚನೆ object_info ಖಾಲಿಗಳು = OBJECT_INFO_INIT;

		ಆದರೆ (!memcmp(&ದತ್ತಾಂಶ.info, &ಖಾಲಿಗಳು, ಗಾತ್ರ(ಖಾಲಿಗಳು)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			ದತ್ತಾಂಶ.skip_object_info = 1;

		ಆದರೆ (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &ದತ್ತಾಂಶ;
		cb.scratch = &output;

		ಆದರೆ (opt->unordered) {
			ರಚನೆ oidset ನೋಡಿದ = OIDSET_INIT;

			cb.ನೋಡಿದ = &ನೋಡಿದ;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ಕ್ರಮ, &cb);

			oidset_clear(&ನೋಡಿದ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ಹಿಂತಿರುಗಿ 0;
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

	ಆದರೆ (opt->batch_ಬಹುಲಕ == BATCH_ಬಹುಲಕ_ಸರತಿ_AND_DISPATCH) {
		batch_objects_command(opt, &output, &ದತ್ತಾಂಶ);
		ಹೋಗು cleanup;
	}

	ಆಗ (strbuf_getdelim_strip_crlf(&ಒಳಹರಿವು, stdin, opt->ಒಳಹರಿವು_delim) != EOF) {
		ಆದರೆ (ದತ್ತಾಂಶ.ವಿಭಜಿಸು_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			ಅಕ್ಷರ *p = strpbrk(ಒಳಹರಿವು.buf, " \t");
			ಆದರೆ (p) {
				ಆಗ (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			ದತ್ತಾಂಶ.rest = p;
		}

		batch_one_object(ಒಳಹರಿವು.buf, &output, opt, &ದತ್ತಾಂಶ);
	}

 cleanup:
	strbuf_release(&ಒಳಹರಿವು);
	strbuf_release(&output);
	string_list_clear(&ದತ್ತಾಂಶ.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_cat_file_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (userdiff_config(var, ಮೌಲ್ಯ) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ batch_option_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				 ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ batch_options *bo = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (bo->enabled) {
		ಹಿಂತಿರುಗಿ error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	ಆದರೆ (!strcmp(opt->long_name, "batch"))
		bo->batch_ಬಹುಲಕ = BATCH_ಬಹುಲಕ_CONTENTS;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(opt->long_name, "batch-check"))
		bo->batch_ಬಹುಲಕ = BATCH_ಬಹುಲಕ_INFO;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(opt->long_name, "batch-command"))
		bo->batch_ಬಹುಲಕ = BATCH_ಬಹುಲಕ_ಸರತಿ_AND_DISPATCH;
	ಇಲ್ಲದಿದ್ದರೆ
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_cat_file(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ opt = 0;
	ಪೂರ್ಣಾಂಕ opt_cw = 0;
	ಪೂರ್ಣಾಂಕ opt_epts = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exp_type = ಖಾಲಿ, *obj_name = ಖಾಲಿ;
	ರಚನೆ batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	ಪೂರ್ಣಾಂಕ unknown_type = 0;
	ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_nul_terminated = 0;
	ಪೂರ್ಣಾಂಕ nul_terminated = 0;
	ಪೂರ್ಣಾಂಕ ret;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		/* Simple queries */
		OPT_ಗುಂಪು(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ಖಾಲಿ, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ಖಾಲಿ, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_ಗುಂಪು(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ಖಾಲಿ, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ಖಾಲಿ, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_ಗುಂಪು(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', ಖಾಲಿ, &ಒಳಹರಿವು_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ಖಾಲಿ, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_ಗುಂಪು(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.ಬಫರ್_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_ಗುಂಪು(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_ಮಾರ್ಗ, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_cat_file_config, ಖಾಲಿ);

	batch.ಬಫರ್_output = -1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	ಆದರೆ (use_mailmap)
		ತುಂಬು_mailmap();

	ಆಯ್ಕೆ (batch.objects_filter.choice) {
	ಸ್ಥಿತಿ LOFC_DISABLED:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ LOFC_BLOB_NONE:
	ಸ್ಥಿತಿ LOFC_BLOB_ಎಲ್ಲೆ:
	ಸ್ಥಿತಿ LOFC_OBJECT_TYPE:
		ಆದರೆ (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	ಆದರೆ (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	ಆದರೆ (force_ಮಾರ್ಗ && !opt_cw)
		usage_ಸಂದೇಶ_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	ಆದರೆ (batch.enabled)
		;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (batch.follow_symlinks)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (batch.ಬಫರ್_output >= 0)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (batch.all_objects)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಳಹರಿವು_nul_terminated)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nul_terminated)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.ಒಳಹರಿವು_delim = batch.output_delim = '\n';
	ಆದರೆ (ಒಳಹರಿವು_nul_terminated)
		batch.ಒಳಹರಿವು_delim = '\0';
	ಆದರೆ (nul_terminated)
		batch.ಒಳಹರಿವು_delim = batch.output_delim = '\0';

	/* Batch defaults */
	ಆದರೆ (batch.ಬಫರ್_output < 0)
		batch.ಬಫರ್_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	/* Return early if we're in batch mode? */
	ಆದರೆ (batch.enabled) {
		ಆದರೆ (opt_cw)
			batch.transform_ಬಹುಲಕ = opt;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt && opt != 'b')
			usage_ಸಂದೇಶ_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc)
			usage_ಸಂದೇಶ_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (opt) {
		ಆದರೆ (!argc && opt == 'c')
			usage_ಸಂದೇಶ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!argc && opt == 'w')
			usage_ಸಂದೇಶ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!argc && opt_epts)
			usage_ಸಂದೇಶ_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 1)
			obj_name = argv[0];
		ಇಲ್ಲದಿದ್ದರೆ
			usage_ಸಂದೇಶ_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc != 2) {
		usage_ಸಂದೇಶ_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

ಹೊರಹೋಗು:
	list_objects_filter_release(&batch.objects_filter);
	ಹಿಂತಿರುಗಿ ret;
}
