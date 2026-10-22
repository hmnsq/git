/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "convert.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "ident.h"
#അന്തര്ഭാവ "list-objects-filter-options.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "userdiff.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "packfile.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/streaming.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "promisor-remote.h"
#അന്തര്ഭാവ "mailmap.h"
#അന്തര്ഭാവ "write-or-die.h"
#അന്തര്ഭാവ "alias.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "transport.h"
#അന്തര്ഭാവ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_വരി \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

എണ്ണൽ batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

ഘടന batch_options {
	ഘടന list_objects_filter_options objects_filter;
	പൂർണ്ണസംഖ്യ enabled;
	പൂർണ്ണസംഖ്യ follow_symlinks;
	എണ്ണൽ batch_mode batch_mode;
	പൂർണ്ണസംഖ്യ buffer_output;
	പൂർണ്ണസംഖ്യ all_objects;
	പൂർണ്ണസംഖ്യ unordered;
	പൂർണ്ണസംഖ്യ transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	അക്ഷരം നിവേശം_delim;
	അക്ഷരം output_delim;
	സ്ഥിരം അക്ഷരം *format;
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *force_path;

സ്ഥിതം ഘടന string_list mailmap = STRING_LIST_INIT_NODUP;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_mailmap;

സ്ഥിതം അക്ഷരം *replace_idents_using_mailmap(അക്ഷരം *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
സ്ഥിതം ശൂന്യം load_mailmap(ശൂന്യം)
{
	എങ്കിൽ (mailmap.strdup_strings)
		തിരികെ;

	read_mailmap(the_repository, &mailmap);
}

സ്ഥിതം അക്ഷരം *replace_idents_using_mailmap(അക്ഷരം *object_buf, size_t *size)
{
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *headers[] = { "author ", "committer ", "tagger ", ശൂന്യ };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	തിരികെ strbuf_detach(&sb, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ filter_object(സ്ഥിരം അക്ഷരം *path, unsigned mode,
			 സ്ഥിരം ഘടന object_id *oid,
			 അക്ഷരം **buf, size_t *size)
{
	എണ്ണൽ object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	എങ്കിൽ (!*buf)
		തിരികെ error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	എങ്കിൽ ((type == OBJ_BLOB) && S_ISREG(mode)) {
		ഘടന strbuf strbuf = STRBUF_INIT;
		ഘടന checkout_metadata meta;

		init_checkout_metadata(&meta, ശൂന്യ, ശൂന്യ, oid);
		എങ്കിൽ (convert_to_working_വൃക്ഷം(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			സ്വതന്ത്രം(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, ശൂന്യ);
		}
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ stream_blob(സ്ഥിരം ഘടന object_id *oid)
{
	എങ്കിൽ (odb_stream_blob_to_fd(the_repository->objects, 1, oid, ശൂന്യ, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cat_one_file(പൂർണ്ണസംഖ്യ opt, സ്ഥിരം അക്ഷരം *exp_type, സ്ഥിരം അക്ഷരം *obj_name)
{
	പൂർണ്ണസംഖ്യ ret;
	ഘടന object_id oid;
	എണ്ണൽ object_type type;
	അക്ഷരം *buf;
	size_t size;
	ഘടന object_context obj_context = {0};
	ഘടന object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	സ്ഥിരം അക്ഷരം *path = force_path;
	സ്ഥിരം പൂർണ്ണസംഖ്യ opt_cw = (opt == 'c' || opt == 'w');
	എങ്കിൽ (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	എങ്കിൽ (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	എങ്കിൽ (!path)
		path = obj_context.path;
	എങ്കിൽ (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = ശൂന്യ;
	തിരഞ്ഞെടുപ്പ് (opt) {
	അവസ്ഥ 't':
		oi.typep = &type;
		എങ്കിൽ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		പോകുക cleanup;

	അവസ്ഥ 's':
		oi.sizep = &size;

		എങ്കിൽ (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (ശൂന്യം**)&buf;
		}

		എങ്കിൽ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		എങ്കിൽ (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		പോകുക cleanup;

	അവസ്ഥ 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		പോകുക cleanup;

	അവസ്ഥ 'w':

		എങ്കിൽ (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			പോകുക cleanup;
		}
		നിർത്തുക;

	അവസ്ഥ 'c':
	{
		unsigned long size_ul = 0;
		പൂർണ്ണസംഖ്യ textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		എങ്കിൽ (textconv_ret)
			നിർത്തുക;
	}
		/* else fallthrough */

	അവസ്ഥ 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ശൂന്യ);
		എങ്കിൽ (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		എങ്കിൽ (type == OBJ_വൃക്ഷം) {
			സ്ഥിരം അക്ഷരം *ls_args[3] = { ശൂന്യ };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_വൃക്ഷം(2, ls_args, ശൂന്യ, the_repository);
			പോകുക cleanup;
		}

		എങ്കിൽ (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			പോകുക cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		എങ്കിൽ (!buf)
			die("Cannot read object %s", obj_name);

		എങ്കിൽ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		നിർത്തുക;

	അവസ്ഥ 0:
	{
		എണ്ണൽ object_type exp_type_id = type_from_string(exp_type);

		എങ്കിൽ (exp_type_id == OBJ_BLOB) {
			ഘടന object_id blob_oid;
			എങ്കിൽ (odb_read_object_info(the_repository->objects,
						 &oid, ശൂന്യ) == OBJ_TAG) {
				അക്ഷരം *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				സ്ഥിരം അക്ഷരം *target;

				എങ്കിൽ (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				എങ്കിൽ (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				സ്വതന്ത്രം(buffer);
			} അല്ലെങ്കിൽ
				oidcpy(&blob_oid, &oid);

			എങ്കിൽ (odb_read_object_info(the_repository->objects,
						 &blob_oid, ശൂന്യ) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				പോകുക cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, ശൂന്യ);

		എങ്കിൽ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		നിർത്തുക;
	}
	സ്ഥിരസ്ഥിതി:
		die("git cat-file: unknown option: %s", exp_type);
	}

	എങ്കിൽ (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	സ്വതന്ത്രം(buf);
	object_context_release(&obj_context);
	തിരികെ ret;
}

ഘടന expand_data {
	ഘടന object_id oid;
	എണ്ണൽ object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	സ്ഥിരം അക്ഷരം *rest;
	ഘടന object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	പൂർണ്ണസംഖ്യ mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	പൂർണ്ണസംഖ്യ split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	ഘടന object_info info;

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
	ഘടന string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_atom(സ്ഥിരം അക്ഷരം *atom, സ്ഥിരം അക്ഷരം *s, പൂർണ്ണസംഖ്യ slen)
{
	പൂർണ്ണസംഖ്യ alen = strlen(atom);
	തിരികെ alen == slen && !memcmp(atom, s, alen);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ expand_atom(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *atom, പൂർണ്ണസംഖ്യ len,
		       ഘടന expand_data *data)
{
	എങ്കിൽ (data->is_remote) {
		size_t i;
		വേണ്ടി (i = 0; i < data->remote_allowed_atoms.nr; i++)
			എങ്കിൽ (is_atom(data->remote_allowed_atoms.ഇനങ്ങൾ[i].string,
				    atom, len))
				നിർത്തുക;
		എങ്കിൽ (i == data->remote_allowed_atoms.nr)
			തിരികെ 1;
	}

	എങ്കിൽ (is_atom("objectname", atom, len)) {
		എങ്കിൽ (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (is_atom("objecttype", atom, len)) {
		എങ്കിൽ (data->mark_query) {
			data->info.typep = &data->type;
		} അല്ലെങ്കിൽ {
			സ്ഥിരം അക്ഷരം *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (is_atom("objectsize", atom, len)) {
		എങ്കിൽ (data->mark_query)
			data->info.sizep = &data->size;
		അല്ലെങ്കിൽ
			strbuf_add_uint(sb, data->size);
	} അല്ലെങ്കിൽ എങ്കിൽ (is_atom("objectsize:disk", atom, len)) {
		എങ്കിൽ (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		അല്ലെങ്കിൽ
			strbuf_add_uint(sb, data->disk_size);
	} അല്ലെങ്കിൽ എങ്കിൽ (is_atom("rest", atom, len)) {
		എങ്കിൽ (data->mark_query)
			data->split_on_whitespace = 1;
		അല്ലെങ്കിൽ എങ്കിൽ (data->rest)
			strbuf_addstr(sb, data->rest);
	} അല്ലെങ്കിൽ എങ്കിൽ (is_atom("deltabase", atom, len)) {
		എങ്കിൽ (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		അല്ലെങ്കിൽ
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} അല്ലെങ്കിൽ എങ്കിൽ (is_atom("objectmode", atom, len)) {
		എങ്കിൽ (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} അല്ലെങ്കിൽ
		തിരികെ 0;
	തിരികെ 1;
}

സ്ഥിതം ശൂന്യം expand_format(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *start,
			  ഘടന expand_data *data)
{
	ആകുമ്പോൾ (strbuf_expand_step(sb, &start)) {
		സ്ഥിരം അക്ഷരം *end;

		എങ്കിൽ (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		അല്ലെങ്കിൽ എങ്കിൽ ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		അല്ലെങ്കിൽ
			strbuf_expand_bad_format(start, "cat-file");
	}
}

സ്ഥിതം ശൂന്യം batch_write(ഘടന batch_options *opt, സ്ഥിരം ശൂന്യം *data, പൂർണ്ണസംഖ്യ len)
{
	എങ്കിൽ (opt->buffer_output) {
		എങ്കിൽ (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} അല്ലെങ്കിൽ
		write_or_die(1, data, len);
}

സ്ഥിതം ശൂന്യം print_object_or_die(ഘടന batch_options *opt, ഘടന expand_data *data)
{
	സ്ഥിരം ഘടന object_id *oid = &data->oid;

	പ്രതിജ്ഞാ(data->info.typep);

	എങ്കിൽ (data->type == OBJ_BLOB) {
		എങ്കിൽ (opt->buffer_output)
			fflush(stdout);
		എങ്കിൽ (opt->transform_mode) {
			അക്ഷരം *contents;
			size_t size;

			എങ്കിൽ (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			എങ്കിൽ (opt->transform_mode == 'w') {
				എങ്കിൽ (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} അല്ലെങ്കിൽ എങ്കിൽ (opt->transform_mode == 'c') {
				എണ്ണൽ object_type type;
				unsigned long size_ul = 0;
				എങ്കിൽ (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				അല്ലെങ്കിൽ
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				എങ്കിൽ (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} അല്ലെങ്കിൽ
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			സ്വതന്ത്രം(contents);
		} അല്ലെങ്കിൽ {
			stream_blob(oid);
		}
	}
	അല്ലെങ്കിൽ {
		എണ്ണൽ object_type type;
		size_t size;
		ശൂന്യം *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		എങ്കിൽ (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		എങ്കിൽ (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		എങ്കിൽ (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		എങ്കിൽ (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		സ്വതന്ത്രം(contents);
	}
}

സ്ഥിതം ശൂന്യം print_default_format(ഘടന strbuf *scratch, ഘടന expand_data *data,
				 ഘടന batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

സ്ഥിതം ശൂന്യം report_object_status(ഘടന batch_options *opt,
				 സ്ഥിരം അക്ഷരം *obj_name,
				 സ്ഥിരം ഘടന object_id *oid,
				 സ്ഥിരം അക്ഷരം *status)
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
സ്ഥിതം ശൂന്യം batch_object_write(സ്ഥിരം അക്ഷരം *obj_name,
			       ഘടന strbuf *scratch,
			       ഘടന batch_options *opt,
			       ഘടന expand_data *data,
			       ഘടന packed_git *pack,
			       off_t offset)
{
	എങ്കിൽ (!data->skip_object_info) {
		പൂർണ്ണസംഖ്യ ret;

		എങ്കിൽ (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		എങ്കിൽ (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		എങ്കിൽ (pack)
			ret = packed_object_info(ശൂന്യ, pack, offset, &data->info);
		അല്ലെങ്കിൽ
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		എങ്കിൽ (ret < 0) {
			എങ്കിൽ (data->mode == S_IFGITLINK)
				report_object_status(opt, ശൂന്യ, &data->oid, "submodule");
			അല്ലെങ്കിൽ
				report_object_status(opt, obj_name, &data->oid, "missing");
			തിരികെ;
		}

		തിരഞ്ഞെടുപ്പ് (opt->objects_filter.choice) {
		അവസ്ഥ LOFC_DISABLED:
			നിർത്തുക;
		അവസ്ഥ LOFC_BLOB_NONE:
			എങ്കിൽ (data->type == OBJ_BLOB) {
				എങ്കിൽ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				തിരികെ;
			}
			നിർത്തുക;
		അവസ്ഥ LOFC_BLOB_LIMIT:
			എങ്കിൽ (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_മൂല്യം) {
				എങ്കിൽ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				തിരികെ;
			}
			നിർത്തുക;
		അവസ്ഥ LOFC_OBJECT_TYPE:
			എങ്കിൽ (data->type != opt->objects_filter.object_type) {
				എങ്കിൽ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				തിരികെ;
			}
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			BUG("unsupported objects filter");
		}

		എങ്കിൽ (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			അക്ഷരം *buf = ശൂന്യ;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			എങ്കിൽ (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			സ്വതന്ത്രം(buf);
		}
	}

	strbuf_reset(scratch);

	എങ്കിൽ (!opt->format) {
		print_default_format(scratch, data, opt);
	} അല്ലെങ്കിൽ {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	എങ്കിൽ (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

സ്ഥിതം ശൂന്യം batch_one_object(സ്ഥിരം അക്ഷരം *obj_name,
			     ഘടന strbuf *scratch,
			     ഘടന batch_options *opt,
			     ഘടന expand_data *data)
{
	ഘടന object_context ctx = {0};
	പൂർണ്ണസംഖ്യ flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	എണ്ണൽ get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	എങ്കിൽ (result != FOUND) {
		തിരഞ്ഞെടുപ്പ് (result) {
		അവസ്ഥ MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			നിർത്തുക;
		അവസ്ഥ SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			നിർത്തുക;
		അവസ്ഥ DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			നിർത്തുക;
		അവസ്ഥ SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			നിർത്തുക;
		അവസ്ഥ NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			നിർത്തുക;
		}
		fflush(stdout);

		പോകുക out;
	}

	എങ്കിൽ (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		പോകുക out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, ശൂന്യ, 0);

out:
	object_context_release(&ctx);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_remote_info(പൂർണ്ണസംഖ്യ argc,
			   സ്ഥിരം അക്ഷരം **argv,
			   ഘടന fetch_object_info_results *results,
			   ഘടന oid_array *object_info_oids)
{
	പൂർണ്ണസംഖ്യ retval = 0;
	ഘടന remote *remote = ശൂന്യ;
	ഘടന object_id oid;
	ഘടന transport *gtransport;

	remote = remote_get(argv[0]);
	എങ്കിൽ (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	വേണ്ടി (size_t i = 1; i < argc; i++) {
		എങ്കിൽ (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			എങ്കിൽ (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				വേണ്ടി (j = 0; j < len; j++)
					എങ്കിൽ (!isxdigit(argv[i][j]))
						നിർത്തുക;
				എങ്കിൽ (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (പൂർണ്ണസംഖ്യ)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	എങ്കിൽ (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ശൂന്യ);

	എങ്കിൽ (!gtransport->smart_options) {
		retval = -1;
		പോകുക cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	തിരികെ retval;
}

ഘടന object_cb_data {
	ഘടന batch_options *opt;
	ഘടന expand_data *expand;
	ഘടന oidset *seen;
	ഘടന strbuf *scratch;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ batch_object_cb(സ്ഥിരം ഘടന object_id *oid, ശൂന്യം *vdata)
{
	ഘടന object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(ശൂന്യ, data->scratch, data->opt, data->expand,
			   ശൂന്യ, 0);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ collect_object(സ്ഥിരം ഘടന object_id *oid,
			  ഘടന packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  ശൂന്യം *data)
{
	oid_array_append(data, oid);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ batch_unordered_object(സ്ഥിരം ഘടന object_id *oid,
				  ഘടന packed_git *pack,
				  off_t offset,
				  ശൂന്യം *vdata)
{
	ഘടന object_cb_data *data = vdata;

	എങ്കിൽ (oidset_തിരുകുക(data->seen, oid))
		തിരികെ 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(ശൂന്യ, data->scratch, data->opt, data->expand,
			   pack, offset);
	തിരികെ 0;
}

തരനിർവചനം ശൂന്യം (*parse_cmd_fn_t)(ഘടന batch_options *, സ്ഥിരം അക്ഷരം *,
			       ഘടന strbuf *, ഘടന expand_data *);

ഘടന queued_cmd {
	parse_cmd_fn_t fn;
	അക്ഷരം *വരി;
};

സ്ഥിതം ശൂന്യം parse_cmd_contents(ഘടന batch_options *opt,
			     സ്ഥിരം അക്ഷരം *വരി,
			     ഘടന strbuf *output,
			     ഘടന expand_data *data)
{
	എണ്ണൽ object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(വരി, output, opt, data);
	data->info.typep = saved_typep;
}

സ്ഥിതം ശൂന്യം parse_cmd_info(ഘടന batch_options *opt,
			   സ്ഥിരം അക്ഷരം *വരി,
			   ഘടന strbuf *output,
			   ഘടന expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(വരി, output, opt, data);
}

സ്ഥിതം ശൂന്യം parse_cmd_mailmap(ഘടന batch_options *opt UNUSED,
			      സ്ഥിരം അക്ഷരം *വരി,
			      ഘടന strbuf *output UNUSED,
			      ഘടന expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(വരി);

	എങ്കിൽ (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), വരി);

	എങ്കിൽ (use_mailmap)
		load_mailmap();
}

സ്ഥിതം ശൂന്യം parse_cmd_remote_object_info(ഘടന batch_options *opt,
					 സ്ഥിരം അക്ഷരം *വരി, ഘടന strbuf *output,
					 ഘടന expand_data *data)
{
	പൂർണ്ണസംഖ്യ എണ്ണം;
	സ്ഥിരം അക്ഷരം **argv;
	അക്ഷരം *വരി_to_split;
	ഘടന fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	ഘടന oid_array object_info_oids = OID_ARRAY_INIT;

	എങ്കിൽ (strlen(വരി) >= MAX_REMOTE_OBJ_INFO_വരി)
		die(_("remote-object-info command too long"));

	വരി_to_split = xstrdup(വരി);
	എണ്ണം = split_cmdline(വരി_to_split, &argv);
	എങ്കിൽ (എണ്ണം < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(എണ്ണം));
	എങ്കിൽ (എണ്ണം - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	എങ്കിൽ (data->info.sizep)
		results.wants_size = 1;
	എങ്കിൽ (data->info.typep)
		results.wants_type = 1;

	എങ്കിൽ (get_remote_info(എണ്ണം, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	എങ്കിൽ (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	എങ്കിൽ (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	വേണ്ടി (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		എങ്കിൽ (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			തുടരുക;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		എങ്കിൽ (results.sizes)
			data->size = results.sizes[i];

		എങ്കിൽ (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, ശൂന്യ, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	സ്വതന്ത്രം(വരി_to_split);
	സ്വതന്ത്രം(argv);
	oid_array_clear(&object_info_oids);
}

സ്ഥിതം ശൂന്യം dispatch_calls(ഘടന batch_options *opt,
		ഘടന strbuf *output,
		ഘടന expand_data *data,
		ഘടന queued_cmd *cmd,
		size_t nr)
{
	എങ്കിൽ (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	വേണ്ടി (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].വരി, output, data);

	fflush(stdout);
}

സ്ഥിതം ശൂന്യം free_cmds(ഘടന queued_cmd *cmd, size_t *nr)
{
	വേണ്ടി (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].വരി);

	*nr = 0;
}


സ്ഥിതം സ്ഥിരം ഘടന parse_cmd {
	സ്ഥിരം അക്ഷരം *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ശൂന്യ, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

സ്ഥിതം ശൂന്യം batch_objects_command(ഘടന batch_options *opt,
				    ഘടന strbuf *output,
				    ഘടന expand_data *data)
{
	ഘടന strbuf നിവേശം = STRBUF_INIT;
	ഘടന queued_cmd *queued_cmd = ശൂന്യ;
	size_t alloc = 0, nr = 0;

	ആകുമ്പോൾ (strbuf_getdelim_strip_crlf(&നിവേശം, stdin, opt->നിവേശം_delim) != EOF) {
		സ്ഥിരം ഘടന parse_cmd *cmd = ശൂന്യ;
		സ്ഥിരം അക്ഷരം *p = ശൂന്യ, *cmd_end;
		ഘടന queued_cmd call = {0};

		എങ്കിൽ (!നിവേശം.len)
			die(_("empty command in input"));
		എങ്കിൽ (isspace(*നിവേശം.buf))
			die(_("whitespace before command: '%s'"), നിവേശം.buf);

		വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			എങ്കിൽ (!skip_prefix(നിവേശം.buf, commands[i].name, &cmd_end))
				തുടരുക;

			cmd = &commands[i];
			എങ്കിൽ (cmd->takes_args) {
				എങ്കിൽ (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			നിർത്തുക;
		}

		എങ്കിൽ (!cmd)
			die(_("unknown command: '%s'"), നിവേശം.buf);

		എങ്കിൽ (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} അല്ലെങ്കിൽ എങ്കിൽ (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} അല്ലെങ്കിൽ {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.വരി = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	എങ്കിൽ (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	സ്വതന്ത്രം(queued_cmd);
	strbuf_release(&നിവേശം);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

തരനിർവചനം പൂർണ്ണസംഖ്യ (*for_each_object_fn)(സ്ഥിരം ഘടന object_id *oid, ഘടന packed_git *pack,
				  off_t offset, ശൂന്യം *data);

ഘടന for_each_object_payload {
	for_each_object_fn callback;
	ശൂന്യം *payload;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ batch_one_object_oi(സ്ഥിരം ഘടന object_id *oid,
			       ഘടന object_info *oi,
			       ശൂന്യം *_payload)
{
	ഘടന for_each_object_payload *payload = _payload;
	എങ്കിൽ (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		തിരികെ payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	തിരികെ payload->callback(oid, ശൂന്യ, 0, payload->payload);
}

സ്ഥിതം ശൂന്യം batch_each_object(ഘടന batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      ശൂന്യം *_payload)
{
	ഘടന for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	ഘടന odb_source_info source_info;
	ഘടന object_info oi = {
		.source_infop = &source_info,
	};
	ഘടന odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ batch_objects(ഘടന batch_options *opt)
{
	ഘടന strbuf നിവേശം = STRBUF_INIT;
	ഘടന strbuf output = STRBUF_INIT;
	ഘടന expand_data data = EXPAND_DATA_INIT;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	പൂർണ്ണസംഖ്യ save_warning;
	പൂർണ്ണസംഖ്യ retval = 0;

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
	എങ്കിൽ (opt->transform_mode)
		data.split_on_whitespace = 1;

	എങ്കിൽ (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ശൂന്യ;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	എങ്കിൽ (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	എങ്കിൽ (opt->all_objects) {
		ഘടന object_cb_data cb;
		ഘടന object_info empty = OBJECT_INFO_INIT;

		എങ്കിൽ (!memcmp(&data.info, &empty, വലുപ്പം(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		എങ്കിൽ (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		എങ്കിൽ (opt->unordered) {
			ഘടന oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} അല്ലെങ്കിൽ {
			ഘടന oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		തിരികെ 0;
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

	എങ്കിൽ (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		പോകുക cleanup;
	}

	ആകുമ്പോൾ (strbuf_getdelim_strip_crlf(&നിവേശം, stdin, opt->നിവേശം_delim) != EOF) {
		എങ്കിൽ (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			അക്ഷരം *p = strpbrk(നിവേശം.buf, " \t");
			എങ്കിൽ (p) {
				ആകുമ്പോൾ (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(നിവേശം.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&നിവേശം);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_cat_file_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			       സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (userdiff_config(var, മൂല്യം) < 0)
		തിരികെ -1;

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ batch_option_callback(സ്ഥിരം ഘടന option *opt,
				 സ്ഥിരം അക്ഷരം *arg,
				 പൂർണ്ണസംഖ്യ unset)
{
	ഘടന batch_options *bo = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (bo->enabled) {
		തിരികെ error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	എങ്കിൽ (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	അല്ലെങ്കിൽ
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_cat_file(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ opt = 0;
	പൂർണ്ണസംഖ്യ opt_cw = 0;
	പൂർണ്ണസംഖ്യ opt_epts = 0;
	സ്ഥിരം അക്ഷരം *exp_type = ശൂന്യ, *obj_name = ശൂന്യ;
	ഘടന batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	പൂർണ്ണസംഖ്യ unknown_type = 0;
	പൂർണ്ണസംഖ്യ നിവേശം_nul_terminated = 0;
	പൂർണ്ണസംഖ്യ nul_terminated = 0;
	പൂർണ്ണസംഖ്യ ret;

	സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ശൂന്യ
	};
	സ്ഥിരം ഘടന option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ശൂന്യ, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ശൂന്യ, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ശൂന്യ, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ശൂന്യ, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', ശൂന്യ, &നിവേശം_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ശൂന്യ, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, ശൂന്യ);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	എങ്കിൽ (use_mailmap)
		load_mailmap();

	തിരഞ്ഞെടുപ്പ് (batch.objects_filter.choice) {
	അവസ്ഥ LOFC_DISABLED:
		നിർത്തുക;
	അവസ്ഥ LOFC_BLOB_NONE:
	അവസ്ഥ LOFC_BLOB_LIMIT:
	അവസ്ഥ LOFC_OBJECT_TYPE:
		എങ്കിൽ (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	എങ്കിൽ (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	എങ്കിൽ (force_path && !opt_cw)
		usage_സന്ദേശം_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	എങ്കിൽ (batch.enabled)
		;
	അല്ലെങ്കിൽ എങ്കിൽ (batch.follow_symlinks)
		usage_സന്ദേശം_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	അല്ലെങ്കിൽ എങ്കിൽ (batch.buffer_output >= 0)
		usage_സന്ദേശം_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	അല്ലെങ്കിൽ എങ്കിൽ (batch.all_objects)
		usage_സന്ദേശം_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	അല്ലെങ്കിൽ എങ്കിൽ (നിവേശം_nul_terminated)
		usage_സന്ദേശം_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	അല്ലെങ്കിൽ എങ്കിൽ (nul_terminated)
		usage_സന്ദേശം_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.നിവേശം_delim = batch.output_delim = '\n';
	എങ്കിൽ (നിവേശം_nul_terminated)
		batch.നിവേശം_delim = '\0';
	എങ്കിൽ (nul_terminated)
		batch.നിവേശം_delim = batch.output_delim = '\0';

	/* Batch defaults */
	എങ്കിൽ (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	എങ്കിൽ (batch.enabled) {
		എങ്കിൽ (opt_cw)
			batch.transform_mode = opt;
		അല്ലെങ്കിൽ എങ്കിൽ (opt && opt != 'b')
			usage_സന്ദേശം_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		അല്ലെങ്കിൽ എങ്കിൽ (argc)
			usage_സന്ദേശം_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		പോകുക out;
	}

	എങ്കിൽ (opt) {
		എങ്കിൽ (!argc && opt == 'c')
			usage_സന്ദേശം_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		അല്ലെങ്കിൽ എങ്കിൽ (!argc && opt == 'w')
			usage_സന്ദേശം_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		അല്ലെങ്കിൽ എങ്കിൽ (!argc && opt_epts)
			usage_സന്ദേശം_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		അല്ലെങ്കിൽ എങ്കിൽ (argc == 1)
			obj_name = argv[0];
		അല്ലെങ്കിൽ
			usage_സന്ദേശം_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} അല്ലെങ്കിൽ എങ്കിൽ (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} അല്ലെങ്കിൽ എങ്കിൽ (argc != 2) {
		usage_സന്ദേശം_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} അല്ലെങ്കിൽ എങ്കിൽ (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	തിരികെ ret;
}
