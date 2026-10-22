/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_ಸಾಲು \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

ಗಣನಾ batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

ಸಂರಚನಾ batch_options {
	ಸಂರಚನಾ list_objects_filter_options objects_filter;
	ಪೂರ್ಣಾಙ್ಕ enabled;
	ಪೂರ್ಣಾಙ್ಕ follow_symlinks;
	ಗಣನಾ batch_mode batch_mode;
	ಪೂರ್ಣಾಙ್ಕ buffer_output;
	ಪೂರ್ಣಾಙ್ಕ all_objects;
	ಪೂರ್ಣಾಙ್ಕ unordered;
	ಪೂರ್ಣಾಙ್ಕ transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	ವರ್ಣ ಒಳಹರಿವು_delim;
	ವರ್ಣ output_delim;
	ಧ್ರುವ ವರ್ಣ *format;
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *force_path;

ಸ್ಥಿರ ಸಂರಚನಾ string_list mailmap = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_mailmap;

ಸ್ಥಿರ ವರ್ಣ *replace_idents_using_mailmap(ವರ್ಣ *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ಸ್ಥಿರ ಶೂನ್ಯ load_mailmap(ಶೂನ್ಯ)
{
	ಯದಿ (mailmap.strdup_strings)
		ಪ್ರತ್ಯಾಗಮ;

	read_mailmap(the_repository, &mailmap);
}

ಸ್ಥಿರ ವರ್ಣ *replace_idents_using_mailmap(ವರ್ಣ *object_buf, size_t *size)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *headers[] = { "author ", "committer ", "tagger ", ರಿಕ್ತ };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&sb, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ filter_object(ಧ್ರುವ ವರ್ಣ *path, unsigned mode,
			 ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			 ವರ್ಣ **buf, size_t *size)
{
	ಗಣನಾ object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	ಯದಿ (!*buf)
		ಪ್ರತ್ಯಾಗಮ error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	ಯದಿ ((type == OBJ_BLOB) && S_ISREG(mode)) {
		ಸಂರಚನಾ strbuf strbuf = STRBUF_INIT;
		ಸಂರಚನಾ checkout_metadata meta;

		init_checkout_metadata(&meta, ರಿಕ್ತ, ರಿಕ್ತ, oid);
		ಯದಿ (convert_to_working_ವೃಕ್ಷ(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			ಮುಕ್ತ(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, ರಿಕ್ತ);
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stream_blob(ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಯದಿ (odb_stream_blob_to_fd(the_repository->objects, 1, oid, ರಿಕ್ತ, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cat_one_file(ಪೂರ್ಣಾಙ್ಕ opt, ಧ್ರುವ ವರ್ಣ *exp_type, ಧ್ರುವ ವರ್ಣ *obj_name)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ object_id oid;
	ಗಣನಾ object_type type;
	ವರ್ಣ *buf;
	size_t size;
	ಸಂರಚನಾ object_context obj_context = {0};
	ಸಂರಚನಾ object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	ಧ್ರುವ ವರ್ಣ *path = force_path;
	ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ opt_cw = (opt == 'c' || opt == 'w');
	ಯದಿ (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	ಯದಿ (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	ಯದಿ (!path)
		path = obj_context.path;
	ಯದಿ (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = ರಿಕ್ತ;
	ವಿಕಲ್ಪ (opt) {
	ಪಕ್ಷ 't':
		oi.typep = &type;
		ಯದಿ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		ಗಚ್ಛ cleanup;

	ಪಕ್ಷ 's':
		oi.sizep = &size;

		ಯದಿ (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (ಶೂನ್ಯ**)&buf;
		}

		ಯದಿ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		ಯದಿ (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		ಗಚ್ಛ cleanup;

	ಪಕ್ಷ 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		ಗಚ್ಛ cleanup;

	ಪಕ್ಷ 'w':

		ಯದಿ (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			ಗಚ್ಛ cleanup;
		}
		ಭಙ್ಗ;

	ಪಕ್ಷ 'c':
	{
		unsigned long size_ul = 0;
		ಪೂರ್ಣಾಙ್ಕ textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		ಯದಿ (textconv_ret)
			ಭಙ್ಗ;
	}
		/* else fallthrough */

	ಪಕ್ಷ 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ರಿಕ್ತ);
		ಯದಿ (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		ಯದಿ (type == OBJ_ವೃಕ್ಷ) {
			ಧ್ರುವ ವರ್ಣ *ls_args[3] = { ರಿಕ್ತ };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_ವೃಕ್ಷ(2, ls_args, ರಿಕ್ತ, the_repository);
			ಗಚ್ಛ cleanup;
		}

		ಯದಿ (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			ಗಚ್ಛ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		ಯದಿ (!buf)
			die("Cannot read object %s", obj_name);

		ಯದಿ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		ಭಙ್ಗ;

	ಪಕ್ಷ 0:
	{
		ಗಣನಾ object_type exp_type_id = type_from_string(exp_type);

		ಯದಿ (exp_type_id == OBJ_BLOB) {
			ಸಂರಚನಾ object_id blob_oid;
			ಯದಿ (odb_read_object_info(the_repository->objects,
						 &oid, ರಿಕ್ತ) == OBJ_TAG) {
				ವರ್ಣ *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				ಧ್ರುವ ವರ್ಣ *target;

				ಯದಿ (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				ಯದಿ (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				ಮುಕ್ತ(buffer);
			} ಅನ್ಯಥಾ
				oidcpy(&blob_oid, &oid);

			ಯದಿ (odb_read_object_info(the_repository->objects,
						 &blob_oid, ರಿಕ್ತ) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				ಗಚ್ಛ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, ರಿಕ್ತ);

		ಯದಿ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		ಭಙ್ಗ;
	}
	ಸ್ವಭಾವ:
		die("git cat-file: unknown option: %s", exp_type);
	}

	ಯದಿ (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	ಮುಕ್ತ(buf);
	object_context_release(&obj_context);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ expand_data {
	ಸಂರಚನಾ object_id oid;
	ಗಣನಾ object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	ಧ್ರುವ ವರ್ಣ *rest;
	ಸಂರಚನಾ object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	ಪೂರ್ಣಾಙ್ಕ mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	ಪೂರ್ಣಾಙ್ಕ split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	ಸಂರಚನಾ object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	unsigned skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	unsigned is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	ಸಂರಚನಾ string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_atom(ಧ್ರುವ ವರ್ಣ *atom, ಧ್ರುವ ವರ್ಣ *s, ಪೂರ್ಣಾಙ್ಕ slen)
{
	ಪೂರ್ಣಾಙ್ಕ alen = strlen(atom);
	ಪ್ರತ್ಯಾಗಮ alen == slen && !memcmp(atom, s, alen);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ expand_atom(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *atom, ಪೂರ್ಣಾಙ್ಕ len,
		       ಸಂರಚನಾ expand_data *data)
{
	ಯದಿ (data->is_remote) {
		size_t i;
		ಪ್ರತಿ (i = 0; i < data->remote_allowed_atoms.nr; i++)
			ಯದಿ (is_atom(data->remote_allowed_atoms.ವಸ್ತುಗಳು[i].string,
				    atom, len))
				ಭಙ್ಗ;
		ಯದಿ (i == data->remote_allowed_atoms.nr)
			ಪ್ರತ್ಯಾಗಮ 1;
	}

	ಯದಿ (is_atom("objectname", atom, len)) {
		ಯದಿ (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} ಅನ್ಯಥಾ ಯದಿ (is_atom("objecttype", atom, len)) {
		ಯದಿ (data->mark_query) {
			data->info.typep = &data->type;
		} ಅನ್ಯಥಾ {
			ಧ್ರುವ ವರ್ಣ *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} ಅನ್ಯಥಾ ಯದಿ (is_atom("objectsize", atom, len)) {
		ಯದಿ (data->mark_query)
			data->info.sizep = &data->size;
		ಅನ್ಯಥಾ
			strbuf_add_uint(sb, data->size);
	} ಅನ್ಯಥಾ ಯದಿ (is_atom("objectsize:disk", atom, len)) {
		ಯದಿ (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		ಅನ್ಯಥಾ
			strbuf_add_uint(sb, data->disk_size);
	} ಅನ್ಯಥಾ ಯದಿ (is_atom("rest", atom, len)) {
		ಯದಿ (data->mark_query)
			data->split_on_whitespace = 1;
		ಅನ್ಯಥಾ ಯದಿ (data->rest)
			strbuf_addstr(sb, data->rest);
	} ಅನ್ಯಥಾ ಯದಿ (is_atom("deltabase", atom, len)) {
		ಯದಿ (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		ಅನ್ಯಥಾ
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} ಅನ್ಯಥಾ ಯದಿ (is_atom("objectmode", atom, len)) {
		ಯದಿ (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ expand_format(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *start,
			  ಸಂರಚನಾ expand_data *data)
{
	ಯಾವತ್ (strbuf_expand_step(sb, &start)) {
		ಧ್ರುವ ವರ್ಣ *end;

		ಯದಿ (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		ಅನ್ಯಥಾ ಯದಿ ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		ಅನ್ಯಥಾ
			strbuf_expand_bad_format(start, "cat-file");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ batch_write(ಸಂರಚನಾ batch_options *opt, ಧ್ರುವ ಶೂನ್ಯ *data, ಪೂರ್ಣಾಙ್ಕ len)
{
	ಯದಿ (opt->buffer_output) {
		ಯದಿ (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} ಅನ್ಯಥಾ
		write_or_die(1, data, len);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_object_or_die(ಸಂರಚನಾ batch_options *opt, ಸಂರಚನಾ expand_data *data)
{
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid = &data->oid;

	ಪ್ರತಿಜ್ಞಾ(data->info.typep);

	ಯದಿ (data->type == OBJ_BLOB) {
		ಯದಿ (opt->buffer_output)
			fflush(stdout);
		ಯದಿ (opt->transform_mode) {
			ವರ್ಣ *contents;
			size_t size;

			ಯದಿ (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			ಯದಿ (opt->transform_mode == 'w') {
				ಯದಿ (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} ಅನ್ಯಥಾ ಯದಿ (opt->transform_mode == 'c') {
				ಗಣನಾ object_type type;
				unsigned long size_ul = 0;
				ಯದಿ (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				ಅನ್ಯಥಾ
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				ಯದಿ (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} ಅನ್ಯಥಾ
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			ಮುಕ್ತ(contents);
		} ಅನ್ಯಥಾ {
			stream_blob(oid);
		}
	}
	ಅನ್ಯಥಾ {
		ಗಣನಾ object_type type;
		size_t size;
		ಶೂನ್ಯ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		ಯದಿ (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		ಯದಿ (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		ಯದಿ (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		ಯದಿ (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		ಮುಕ್ತ(contents);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ print_default_format(ಸಂರಚನಾ strbuf *scratch, ಸಂರಚನಾ expand_data *data,
				 ಸಂರಚನಾ batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

ಸ್ಥಿರ ಶೂನ್ಯ report_object_status(ಸಂರಚನಾ batch_options *opt,
				 ಧ್ರುವ ವರ್ಣ *obj_name,
				 ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				 ಧ್ರುವ ವರ್ಣ *status)
{
	printf("%s %s%c", obj_name ? obj_name : oid_to_hex(oid),
	       status, opt->output_delim);
	fflush(stdout);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
ಸ್ಥಿರ ಶೂನ್ಯ batch_object_write(ಧ್ರುವ ವರ್ಣ *obj_name,
			       ಸಂರಚನಾ strbuf *scratch,
			       ಸಂರಚನಾ batch_options *opt,
			       ಸಂರಚನಾ expand_data *data,
			       ಸಂರಚನಾ packed_git *pack,
			       off_t offset)
{
	ಯದಿ (!data->skip_object_info) {
		ಪೂರ್ಣಾಙ್ಕ ret;

		ಯದಿ (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		ಯದಿ (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		ಯದಿ (pack)
			ret = packed_object_info(ರಿಕ್ತ, pack, offset, &data->info);
		ಅನ್ಯಥಾ
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		ಯದಿ (ret < 0) {
			ಯದಿ (data->mode == S_IFGITLINK)
				report_object_status(opt, ರಿಕ್ತ, &data->oid, "submodule");
			ಅನ್ಯಥಾ
				report_object_status(opt, obj_name, &data->oid, "missing");
			ಪ್ರತ್ಯಾಗಮ;
		}

		ವಿಕಲ್ಪ (opt->objects_filter.choice) {
		ಪಕ್ಷ LOFC_DISABLED:
			ಭಙ್ಗ;
		ಪಕ್ಷ LOFC_BLOB_NONE:
			ಯದಿ (data->type == OBJ_BLOB) {
				ಯದಿ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ಪ್ರತ್ಯಾಗಮ;
			}
			ಭಙ್ಗ;
		ಪಕ್ಷ LOFC_BLOB_LIMIT:
			ಯದಿ (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_ಮೌಲ್ಯ) {
				ಯದಿ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ಪ್ರತ್ಯಾಗಮ;
			}
			ಭಙ್ಗ;
		ಪಕ್ಷ LOFC_OBJECT_TYPE:
			ಯದಿ (data->type != opt->objects_filter.object_type) {
				ಯದಿ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ಪ್ರತ್ಯಾಗಮ;
			}
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			BUG("unsupported objects filter");
		}

		ಯದಿ (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			ವರ್ಣ *buf = ರಿಕ್ತ;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			ಯದಿ (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			ಮುಕ್ತ(buf);
		}
	}

	strbuf_reset(scratch);

	ಯದಿ (!opt->format) {
		print_default_format(scratch, data, opt);
	} ಅನ್ಯಥಾ {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	ಯದಿ (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ batch_one_object(ಧ್ರುವ ವರ್ಣ *obj_name,
			     ಸಂರಚನಾ strbuf *scratch,
			     ಸಂರಚನಾ batch_options *opt,
			     ಸಂರಚನಾ expand_data *data)
{
	ಸಂರಚನಾ object_context ctx = {0};
	ಪೂರ್ಣಾಙ್ಕ flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	ಗಣನಾ get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	ಯದಿ (result != FOUND) {
		ವಿಕಲ್ಪ (result) {
		ಪಕ್ಷ MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			ಭಙ್ಗ;
		ಪಕ್ಷ SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			ಭಙ್ಗ;
		ಪಕ್ಷ DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ಭಙ್ಗ;
		ಪಕ್ಷ SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ಭಙ್ಗ;
		ಪಕ್ಷ NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			ಭಙ್ಗ;
		}
		fflush(stdout);

		ಗಚ್ಛ out;
	}

	ಯದಿ (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		ಗಚ್ಛ out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, ರಿಕ್ತ, 0);

out:
	object_context_release(&ctx);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_remote_info(ಪೂರ್ಣಾಙ್ಕ argc,
			   ಧ್ರುವ ವರ್ಣ **argv,
			   ಸಂರಚನಾ fetch_object_info_results *results,
			   ಸಂರಚನಾ oid_array *object_info_oids)
{
	ಪೂರ್ಣಾಙ್ಕ retval = 0;
	ಸಂರಚನಾ remote *remote = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ transport *gtransport;

	remote = remote_get(argv[0]);
	ಯದಿ (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	ಪ್ರತಿ (size_t i = 1; i < argc; i++) {
		ಯದಿ (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			ಯದಿ (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				ಪ್ರತಿ (j = 0; j < len; j++)
					ಯದಿ (!isxdigit(argv[i][j]))
						ಭಙ್ಗ;
				ಯದಿ (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (ಪೂರ್ಣಾಙ್ಕ)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	ಯದಿ (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ರಿಕ್ತ);

	ಯದಿ (!gtransport->smart_options) {
		retval = -1;
		ಗಚ್ಛ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸಂರಚನಾ object_cb_data {
	ಸಂರಚನಾ batch_options *opt;
	ಸಂರಚನಾ expand_data *expand;
	ಸಂರಚನಾ oidset *seen;
	ಸಂರಚನಾ strbuf *scratch;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ batch_object_cb(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಶೂನ್ಯ *vdata)
{
	ಸಂರಚನಾ object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(ರಿಕ್ತ, data->scratch, data->opt, data->expand,
			   ರಿಕ್ತ, 0);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collect_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			  ಸಂರಚನಾ packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  ಶೂನ್ಯ *data)
{
	oid_array_append(data, oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ batch_unordered_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
				  ಸಂರಚನಾ packed_git *pack,
				  off_t offset,
				  ಶೂನ್ಯ *vdata)
{
	ಸಂರಚನಾ object_cb_data *data = vdata;

	ಯದಿ (oidset_ಸೇರಿಸು(data->seen, oid))
		ಪ್ರತ್ಯಾಗಮ 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(ರಿಕ್ತ, data->scratch, data->opt, data->expand,
			   pack, offset);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*parse_cmd_fn_t)(ಸಂರಚನಾ batch_options *, ಧ್ರುವ ವರ್ಣ *,
			       ಸಂರಚನಾ strbuf *, ಸಂರಚನಾ expand_data *);

ಸಂರಚನಾ queued_cmd {
	parse_cmd_fn_t fn;
	ವರ್ಣ *ಸಾಲು;
};

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_contents(ಸಂರಚನಾ batch_options *opt,
			     ಧ್ರುವ ವರ್ಣ *ಸಾಲು,
			     ಸಂರಚನಾ strbuf *output,
			     ಸಂರಚನಾ expand_data *data)
{
	ಗಣನಾ object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(ಸಾಲು, output, opt, data);
	data->info.typep = saved_typep;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_info(ಸಂರಚನಾ batch_options *opt,
			   ಧ್ರುವ ವರ್ಣ *ಸಾಲು,
			   ಸಂರಚನಾ strbuf *output,
			   ಸಂರಚನಾ expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(ಸಾಲು, output, opt, data);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_mailmap(ಸಂರಚನಾ batch_options *opt UNUSED,
			      ಧ್ರುವ ವರ್ಣ *ಸಾಲು,
			      ಸಂರಚನಾ strbuf *output UNUSED,
			      ಸಂರಚನಾ expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(ಸಾಲು);

	ಯದಿ (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), ಸಾಲು);

	ಯದಿ (use_mailmap)
		load_mailmap();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_remote_object_info(ಸಂರಚನಾ batch_options *opt,
					 ಧ್ರುವ ವರ್ಣ *ಸಾಲು, ಸಂರಚನಾ strbuf *output,
					 ಸಂರಚನಾ expand_data *data)
{
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;
	ಧ್ರುವ ವರ್ಣ **argv;
	ವರ್ಣ *ಸಾಲು_to_split;
	ಸಂರಚನಾ fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	ಸಂರಚನಾ oid_array object_info_oids = OID_ARRAY_INIT;

	ಯದಿ (strlen(ಸಾಲು) >= MAX_REMOTE_OBJ_INFO_ಸಾಲು)
		die(_("remote-object-info command too long"));

	ಸಾಲು_to_split = xstrdup(ಸಾಲು);
	ಎಣಿಕೆ = split_cmdline(ಸಾಲು_to_split, &argv);
	ಯದಿ (ಎಣಿಕೆ < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(ಎಣಿಕೆ));
	ಯದಿ (ಎಣಿಕೆ - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	ಯದಿ (data->info.sizep)
		results.wants_size = 1;
	ಯದಿ (data->info.typep)
		results.wants_type = 1;

	ಯದಿ (get_remote_info(ಎಣಿಕೆ, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	ಯದಿ (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	ಯದಿ (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	ಪ್ರತಿ (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		ಯದಿ (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			ಅನುವರ್ತ;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		ಯದಿ (results.sizes)
			data->size = results.sizes[i];

		ಯದಿ (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, ರಿಕ್ತ, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	ಮುಕ್ತ(ಸಾಲು_to_split);
	ಮುಕ್ತ(argv);
	oid_array_clear(&object_info_oids);
}

ಸ್ಥಿರ ಶೂನ್ಯ dispatch_calls(ಸಂರಚನಾ batch_options *opt,
		ಸಂರಚನಾ strbuf *output,
		ಸಂರಚನಾ expand_data *data,
		ಸಂರಚನಾ queued_cmd *cmd,
		size_t nr)
{
	ಯದಿ (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	ಪ್ರತಿ (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].ಸಾಲು, output, data);

	fflush(stdout);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_cmds(ಸಂರಚನಾ queued_cmd *cmd, size_t *nr)
{
	ಪ್ರತಿ (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].ಸಾಲು);

	*nr = 0;
}


ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ parse_cmd {
	ಧ್ರುವ ವರ್ಣ *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ರಿಕ್ತ, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ಸ್ಥಿರ ಶೂನ್ಯ batch_objects_command(ಸಂರಚನಾ batch_options *opt,
				    ಸಂರಚನಾ strbuf *output,
				    ಸಂರಚನಾ expand_data *data)
{
	ಸಂರಚನಾ strbuf ಒಳಹರಿವು = STRBUF_INIT;
	ಸಂರಚನಾ queued_cmd *queued_cmd = ರಿಕ್ತ;
	size_t alloc = 0, nr = 0;

	ಯಾವತ್ (strbuf_getdelim_strip_crlf(&ಒಳಹರಿವು, stdin, opt->ಒಳಹರಿವು_delim) != EOF) {
		ಧ್ರುವ ಸಂರಚನಾ parse_cmd *cmd = ರಿಕ್ತ;
		ಧ್ರುವ ವರ್ಣ *p = ರಿಕ್ತ, *cmd_end;
		ಸಂರಚನಾ queued_cmd call = {0};

		ಯದಿ (!ಒಳಹರಿವು.len)
			die(_("empty command in input"));
		ಯದಿ (isspace(*ಒಳಹರಿವು.buf))
			die(_("whitespace before command: '%s'"), ಒಳಹರಿವು.buf);

		ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			ಯದಿ (!skip_prefix(ಒಳಹರಿವು.buf, commands[i].name, &cmd_end))
				ಅನುವರ್ತ;

			cmd = &commands[i];
			ಯದಿ (cmd->takes_args) {
				ಯದಿ (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} ಅನ್ಯಥಾ ಯದಿ (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			ಭಙ್ಗ;
		}

		ಯದಿ (!cmd)
			die(_("unknown command: '%s'"), ಒಳಹರಿವು.buf);

		ಯದಿ (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} ಅನ್ಯಥಾ ಯದಿ (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} ಅನ್ಯಥಾ {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.ಸಾಲು = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	ಯದಿ (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	ಮುಕ್ತ(queued_cmd);
	strbuf_release(&ಒಳಹರಿವು);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*for_each_object_fn)(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ packed_git *pack,
				  off_t offset, ಶೂನ್ಯ *data);

ಸಂರಚನಾ for_each_object_payload {
	for_each_object_fn callback;
	ಶೂನ್ಯ *payload;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ batch_one_object_oi(ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			       ಸಂರಚನಾ object_info *oi,
			       ಶೂನ್ಯ *_payload)
{
	ಸಂರಚನಾ for_each_object_payload *payload = _payload;
	ಯದಿ (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		ಪ್ರತ್ಯಾಗಮ payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	ಪ್ರತ್ಯಾಗಮ payload->callback(oid, ರಿಕ್ತ, 0, payload->payload);
}

ಸ್ಥಿರ ಶೂನ್ಯ batch_each_object(ಸಂರಚನಾ batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      ಶೂನ್ಯ *_payload)
{
	ಸಂರಚನಾ for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	ಸಂರಚನಾ odb_source_info source_info;
	ಸಂರಚನಾ object_info oi = {
		.source_infop = &source_info,
	};
	ಸಂರಚನಾ odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ batch_objects(ಸಂರಚನಾ batch_options *opt)
{
	ಸಂರಚನಾ strbuf ಒಳಹರಿವು = STRBUF_INIT;
	ಸಂರಚನಾ strbuf output = STRBUF_INIT;
	ಸಂರಚನಾ expand_data data = EXPAND_DATA_INIT;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಙ್ಕ save_warning;
	ಪೂರ್ಣಾಙ್ಕ retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	data.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &data);
	data.mark_query = 0;
	strbuf_release(&output);
	ಯದಿ (opt->transform_mode)
		data.split_on_whitespace = 1;

	ಯದಿ (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ರಿಕ್ತ;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	ಯದಿ (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	ಯದಿ (opt->all_objects) {
		ಸಂರಚನಾ object_cb_data cb;
		ಸಂರಚನಾ object_info empty = OBJECT_INFO_INIT;

		ಯದಿ (!memcmp(&data.info, &empty, ಪರಿಮಾಣ(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		ಯದಿ (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		ಯದಿ (opt->unordered) {
			ಸಂರಚನಾ oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ಪ್ರತ್ಯಾಗಮ 0;
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

	ಯದಿ (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		ಗಚ್ಛ cleanup;
	}

	ಯಾವತ್ (strbuf_getdelim_strip_crlf(&ಒಳಹರಿವು, stdin, opt->ಒಳಹರಿವು_delim) != EOF) {
		ಯದಿ (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			ವರ್ಣ *p = strpbrk(ಒಳಹರಿವು.buf, " \t");
			ಯದಿ (p) {
				ಯಾವತ್ (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(ಒಳಹರಿವು.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&ಒಳಹರಿವು);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_cat_file_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			       ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (userdiff_config(var, ಮೌಲ್ಯ) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ batch_option_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				 ಧ್ರುವ ವರ್ಣ *arg,
				 ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ batch_options *bo = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (bo->enabled) {
		ಪ್ರತ್ಯಾಗಮ error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	ಯದಿ (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	ಅನ್ಯಥಾ
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_cat_file(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ opt = 0;
	ಪೂರ್ಣಾಙ್ಕ opt_cw = 0;
	ಪೂರ್ಣಾಙ್ಕ opt_epts = 0;
	ಧ್ರುವ ವರ್ಣ *exp_type = ರಿಕ್ತ, *obj_name = ರಿಕ್ತ;
	ಸಂರಚನಾ batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	ಪೂರ್ಣಾಙ್ಕ unknown_type = 0;
	ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_nul_terminated = 0;
	ಪೂರ್ಣಾಙ್ಕ nul_terminated = 0;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ರಿಕ್ತ
	};
	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ರಿಕ್ತ, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ರಿಕ್ತ, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ರಿಕ್ತ, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ರಿಕ್ತ, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_GROUP(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', ರಿಕ್ತ, &ಒಳಹರಿವು_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ರಿಕ್ತ, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_GROUP(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.buffer_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_GROUP(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_path, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_END()
	};

	repo_config(the_repository, git_cat_file_config, ರಿಕ್ತ);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	ಯದಿ (use_mailmap)
		load_mailmap();

	ವಿಕಲ್ಪ (batch.objects_filter.choice) {
	ಪಕ್ಷ LOFC_DISABLED:
		ಭಙ್ಗ;
	ಪಕ್ಷ LOFC_BLOB_NONE:
	ಪಕ್ಷ LOFC_BLOB_LIMIT:
	ಪಕ್ಷ LOFC_OBJECT_TYPE:
		ಯದಿ (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	ಯದಿ (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	ಯದಿ (force_path && !opt_cw)
		usage_ಸಂದೇಶ_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	ಯದಿ (batch.enabled)
		;
	ಅನ್ಯಥಾ ಯದಿ (batch.follow_symlinks)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	ಅನ್ಯಥಾ ಯದಿ (batch.buffer_output >= 0)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	ಅನ್ಯಥಾ ಯದಿ (batch.all_objects)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	ಅನ್ಯಥಾ ಯದಿ (ಒಳಹರಿವು_nul_terminated)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	ಅನ್ಯಥಾ ಯದಿ (nul_terminated)
		usage_ಸಂದೇಶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.ಒಳಹರಿವು_delim = batch.output_delim = '\n';
	ಯದಿ (ಒಳಹರಿವು_nul_terminated)
		batch.ಒಳಹರಿವು_delim = '\0';
	ಯದಿ (nul_terminated)
		batch.ಒಳಹರಿವು_delim = batch.output_delim = '\0';

	/* Batch defaults */
	ಯದಿ (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	ಯದಿ (batch.enabled) {
		ಯದಿ (opt_cw)
			batch.transform_mode = opt;
		ಅನ್ಯಥಾ ಯದಿ (opt && opt != 'b')
			usage_ಸಂದೇಶ_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		ಅನ್ಯಥಾ ಯದಿ (argc)
			usage_ಸಂದೇಶ_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		ಗಚ್ಛ out;
	}

	ಯದಿ (opt) {
		ಯದಿ (!argc && opt == 'c')
			usage_ಸಂದೇಶ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		ಅನ್ಯಥಾ ಯದಿ (!argc && opt == 'w')
			usage_ಸಂದೇಶ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		ಅನ್ಯಥಾ ಯದಿ (!argc && opt_epts)
			usage_ಸಂದೇಶ_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		ಅನ್ಯಥಾ ಯದಿ (argc == 1)
			obj_name = argv[0];
		ಅನ್ಯಥಾ
			usage_ಸಂದೇಶ_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} ಅನ್ಯಥಾ ಯದಿ (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} ಅನ್ಯಥಾ ಯದಿ (argc != 2) {
		usage_ಸಂದೇಶ_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} ಅನ್ಯಥಾ ಯದಿ (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	ಪ್ರತ್ಯಾಗಮ ret;
}
