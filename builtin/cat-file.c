/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "convert.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "ident.h"
#அந்தர்பாவ "list-objects-filter-options.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "userdiff.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "packfile.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/streaming.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "promisor-remote.h"
#அந்தர்பாவ "mailmap.h"
#அந்தர்பாவ "write-or-die.h"
#அந்தர்பாவ "alias.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "transport.h"
#அந்தர்பாவ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_வரி \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

எண்ணிக்கை batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

கட்டமைப்பு batch_options {
	கட்டமைப்பு list_objects_filter_options objects_filter;
	முழு enabled;
	முழு follow_symlinks;
	எண்ணிக்கை batch_mode batch_mode;
	முழு buffer_output;
	முழு all_objects;
	முழு unordered;
	முழு transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	எழுத்து உள்ளீடு_delim;
	எழுத்து output_delim;
	மாறிலி எழுத்து *format;
};

நிலையான மாறிலி எழுத்து *force_path;

நிலையான கட்டமைப்பு string_list mailmap = STRING_LIST_INIT_NODUP;
நிலையான முழு use_mailmap;

நிலையான எழுத்து *replace_idents_using_mailmap(எழுத்து *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
நிலையான வெறுமை load_mailmap(வெறுமை)
{
	எனில் (mailmap.strdup_strings)
		திரும்பு;

	read_mailmap(the_repository, &mailmap);
}

நிலையான எழுத்து *replace_idents_using_mailmap(எழுத்து *object_buf, size_t *size)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *headers[] = { "author ", "committer ", "tagger ", வெற்று };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	திரும்பு strbuf_detach(&sb, வெற்று);
}

நிலையான முழு filter_object(மாறிலி எழுத்து *path, unsigned mode,
			 மாறிலி கட்டமைப்பு object_id *oid,
			 எழுத்து **buf, size_t *size)
{
	எண்ணிக்கை object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	எனில் (!*buf)
		திரும்பு error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	எனில் ((type == OBJ_BLOB) && S_ISREG(mode)) {
		கட்டமைப்பு strbuf strbuf = STRBUF_INIT;
		கட்டமைப்பு checkout_metadata meta;

		init_checkout_metadata(&meta, வெற்று, வெற்று, oid);
		எனில் (convert_to_working_மரம்(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			விடுவி(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, வெற்று);
		}
	}

	திரும்பு 0;
}

நிலையான முழு stream_blob(மாறிலி கட்டமைப்பு object_id *oid)
{
	எனில் (odb_stream_blob_to_fd(the_repository->objects, 1, oid, வெற்று, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	திரும்பு 0;
}

நிலையான முழு cat_one_file(முழு opt, மாறிலி எழுத்து *exp_type, மாறிலி எழுத்து *obj_name)
{
	முழு ret;
	கட்டமைப்பு object_id oid;
	எண்ணிக்கை object_type type;
	எழுத்து *buf;
	size_t size;
	கட்டமைப்பு object_context obj_context = {0};
	கட்டமைப்பு object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	மாறிலி எழுத்து *path = force_path;
	மாறிலி முழு opt_cw = (opt == 'c' || opt == 'w');
	எனில் (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	எனில் (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	எனில் (!path)
		path = obj_context.path;
	எனில் (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = வெற்று;
	தேர்வு (opt) {
	நிலை 't':
		oi.typep = &type;
		எனில் (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		செல் cleanup;

	நிலை 's':
		oi.sizep = &size;

		எனில் (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (வெறுமை**)&buf;
		}

		எனில் (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		எனில் (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		செல் cleanup;

	நிலை 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		செல் cleanup;

	நிலை 'w':

		எனில் (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			செல் cleanup;
		}
		நிறுத்து;

	நிலை 'c':
	{
		unsigned long size_ul = 0;
		முழு textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		எனில் (textconv_ret)
			நிறுத்து;
	}
		/* else fallthrough */

	நிலை 'p':
		type = odb_read_object_info(the_repository->objects, &oid, வெற்று);
		எனில் (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		எனில் (type == OBJ_மரம்) {
			மாறிலி எழுத்து *ls_args[3] = { வெற்று };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_மரம்(2, ls_args, வெற்று, the_repository);
			செல் cleanup;
		}

		எனில் (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			செல் cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		எனில் (!buf)
			die("Cannot read object %s", obj_name);

		எனில் (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		நிறுத்து;

	நிலை 0:
	{
		எண்ணிக்கை object_type exp_type_id = type_from_string(exp_type);

		எனில் (exp_type_id == OBJ_BLOB) {
			கட்டமைப்பு object_id blob_oid;
			எனில் (odb_read_object_info(the_repository->objects,
						 &oid, வெற்று) == OBJ_TAG) {
				எழுத்து *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				மாறிலி எழுத்து *target;

				எனில் (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				எனில் (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				விடுவி(buffer);
			} இல்லையெனில்
				oidcpy(&blob_oid, &oid);

			எனில் (odb_read_object_info(the_repository->objects,
						 &blob_oid, வெற்று) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				செல் cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, வெற்று);

		எனில் (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		நிறுத்து;
	}
	இயல்பு:
		die("git cat-file: unknown option: %s", exp_type);
	}

	எனில் (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	விடுவி(buf);
	object_context_release(&obj_context);
	திரும்பு ret;
}

கட்டமைப்பு expand_data {
	கட்டமைப்பு object_id oid;
	எண்ணிக்கை object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	மாறிலி எழுத்து *rest;
	கட்டமைப்பு object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	முழு mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	முழு split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	கட்டமைப்பு object_info info;

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
	கட்டமைப்பு string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

நிலையான முழு is_atom(மாறிலி எழுத்து *atom, மாறிலி எழுத்து *s, முழு slen)
{
	முழு alen = strlen(atom);
	திரும்பு alen == slen && !memcmp(atom, s, alen);
}

நிலையான முழு expand_atom(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *atom, முழு len,
		       கட்டமைப்பு expand_data *data)
{
	எனில் (data->is_remote) {
		size_t i;
		க்கு (i = 0; i < data->remote_allowed_atoms.nr; i++)
			எனில் (is_atom(data->remote_allowed_atoms.பொருட்கள்[i].string,
				    atom, len))
				நிறுத்து;
		எனில் (i == data->remote_allowed_atoms.nr)
			திரும்பு 1;
	}

	எனில் (is_atom("objectname", atom, len)) {
		எனில் (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} இல்லையெனில் எனில் (is_atom("objecttype", atom, len)) {
		எனில் (data->mark_query) {
			data->info.typep = &data->type;
		} இல்லையெனில் {
			மாறிலி எழுத்து *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} இல்லையெனில் எனில் (is_atom("objectsize", atom, len)) {
		எனில் (data->mark_query)
			data->info.sizep = &data->size;
		இல்லையெனில்
			strbuf_add_uint(sb, data->size);
	} இல்லையெனில் எனில் (is_atom("objectsize:disk", atom, len)) {
		எனில் (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		இல்லையெனில்
			strbuf_add_uint(sb, data->disk_size);
	} இல்லையெனில் எனில் (is_atom("rest", atom, len)) {
		எனில் (data->mark_query)
			data->split_on_whitespace = 1;
		இல்லையெனில் எனில் (data->rest)
			strbuf_addstr(sb, data->rest);
	} இல்லையெனில் எனில் (is_atom("deltabase", atom, len)) {
		எனில் (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		இல்லையெனில்
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} இல்லையெனில் எனில் (is_atom("objectmode", atom, len)) {
		எனில் (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} இல்லையெனில்
		திரும்பு 0;
	திரும்பு 1;
}

நிலையான வெறுமை expand_format(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *start,
			  கட்டமைப்பு expand_data *data)
{
	போது (strbuf_expand_step(sb, &start)) {
		மாறிலி எழுத்து *end;

		எனில் (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		இல்லையெனில் எனில் ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		இல்லையெனில்
			strbuf_expand_bad_format(start, "cat-file");
	}
}

நிலையான வெறுமை batch_write(கட்டமைப்பு batch_options *opt, மாறிலி வெறுமை *data, முழு len)
{
	எனில் (opt->buffer_output) {
		எனில் (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} இல்லையெனில்
		write_or_die(1, data, len);
}

நிலையான வெறுமை print_object_or_die(கட்டமைப்பு batch_options *opt, கட்டமைப்பு expand_data *data)
{
	மாறிலி கட்டமைப்பு object_id *oid = &data->oid;

	ப்ரதிஜ்ஞா(data->info.typep);

	எனில் (data->type == OBJ_BLOB) {
		எனில் (opt->buffer_output)
			fflush(stdout);
		எனில் (opt->transform_mode) {
			எழுத்து *contents;
			size_t size;

			எனில் (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			எனில் (opt->transform_mode == 'w') {
				எனில் (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} இல்லையெனில் எனில் (opt->transform_mode == 'c') {
				எண்ணிக்கை object_type type;
				unsigned long size_ul = 0;
				எனில் (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				இல்லையெனில்
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				எனில் (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} இல்லையெனில்
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			விடுவி(contents);
		} இல்லையெனில் {
			stream_blob(oid);
		}
	}
	இல்லையெனில் {
		எண்ணிக்கை object_type type;
		size_t size;
		வெறுமை *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		எனில் (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		எனில் (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		எனில் (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		எனில் (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		விடுவி(contents);
	}
}

நிலையான வெறுமை print_default_format(கட்டமைப்பு strbuf *scratch, கட்டமைப்பு expand_data *data,
				 கட்டமைப்பு batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

நிலையான வெறுமை report_object_status(கட்டமைப்பு batch_options *opt,
				 மாறிலி எழுத்து *obj_name,
				 மாறிலி கட்டமைப்பு object_id *oid,
				 மாறிலி எழுத்து *status)
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
நிலையான வெறுமை batch_object_write(மாறிலி எழுத்து *obj_name,
			       கட்டமைப்பு strbuf *scratch,
			       கட்டமைப்பு batch_options *opt,
			       கட்டமைப்பு expand_data *data,
			       கட்டமைப்பு packed_git *pack,
			       off_t offset)
{
	எனில் (!data->skip_object_info) {
		முழு ret;

		எனில் (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		எனில் (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		எனில் (pack)
			ret = packed_object_info(வெற்று, pack, offset, &data->info);
		இல்லையெனில்
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		எனில் (ret < 0) {
			எனில் (data->mode == S_IFGITLINK)
				report_object_status(opt, வெற்று, &data->oid, "submodule");
			இல்லையெனில்
				report_object_status(opt, obj_name, &data->oid, "missing");
			திரும்பு;
		}

		தேர்வு (opt->objects_filter.choice) {
		நிலை LOFC_DISABLED:
			நிறுத்து;
		நிலை LOFC_BLOB_NONE:
			எனில் (data->type == OBJ_BLOB) {
				எனில் (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				திரும்பு;
			}
			நிறுத்து;
		நிலை LOFC_BLOB_LIMIT:
			எனில் (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_மதிப்பு) {
				எனில் (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				திரும்பு;
			}
			நிறுத்து;
		நிலை LOFC_OBJECT_TYPE:
			எனில் (data->type != opt->objects_filter.object_type) {
				எனில் (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				திரும்பு;
			}
			நிறுத்து;
		இயல்பு:
			BUG("unsupported objects filter");
		}

		எனில் (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			எழுத்து *buf = வெற்று;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			எனில் (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			விடுவி(buf);
		}
	}

	strbuf_reset(scratch);

	எனில் (!opt->format) {
		print_default_format(scratch, data, opt);
	} இல்லையெனில் {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	எனில் (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

நிலையான வெறுமை batch_one_object(மாறிலி எழுத்து *obj_name,
			     கட்டமைப்பு strbuf *scratch,
			     கட்டமைப்பு batch_options *opt,
			     கட்டமைப்பு expand_data *data)
{
	கட்டமைப்பு object_context ctx = {0};
	முழு flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	எண்ணிக்கை get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	எனில் (result != FOUND) {
		தேர்வு (result) {
		நிலை MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			நிறுத்து;
		நிலை SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			நிறுத்து;
		நிலை DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			நிறுத்து;
		நிலை SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			நிறுத்து;
		நிலை NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			நிறுத்து;
		இயல்பு:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			நிறுத்து;
		}
		fflush(stdout);

		செல் out;
	}

	எனில் (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		செல் out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, வெற்று, 0);

out:
	object_context_release(&ctx);
}

நிலையான முழு get_remote_info(முழு argc,
			   மாறிலி எழுத்து **argv,
			   கட்டமைப்பு fetch_object_info_results *results,
			   கட்டமைப்பு oid_array *object_info_oids)
{
	முழு retval = 0;
	கட்டமைப்பு remote *remote = வெற்று;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு transport *gtransport;

	remote = remote_get(argv[0]);
	எனில் (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	க்கு (size_t i = 1; i < argc; i++) {
		எனில் (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			எனில் (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				க்கு (j = 0; j < len; j++)
					எனில் (!isxdigit(argv[i][j]))
						நிறுத்து;
				எனில் (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (முழு)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	எனில் (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, வெற்று);

	எனில் (!gtransport->smart_options) {
		retval = -1;
		செல் cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	திரும்பு retval;
}

கட்டமைப்பு object_cb_data {
	கட்டமைப்பு batch_options *opt;
	கட்டமைப்பு expand_data *expand;
	கட்டமைப்பு oidset *seen;
	கட்டமைப்பு strbuf *scratch;
};

நிலையான முழு batch_object_cb(மாறிலி கட்டமைப்பு object_id *oid, வெறுமை *vdata)
{
	கட்டமைப்பு object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(வெற்று, data->scratch, data->opt, data->expand,
			   வெற்று, 0);
	திரும்பு 0;
}

நிலையான முழு collect_object(மாறிலி கட்டமைப்பு object_id *oid,
			  கட்டமைப்பு packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  வெறுமை *data)
{
	oid_array_append(data, oid);
	திரும்பு 0;
}

நிலையான முழு batch_unordered_object(மாறிலி கட்டமைப்பு object_id *oid,
				  கட்டமைப்பு packed_git *pack,
				  off_t offset,
				  வெறுமை *vdata)
{
	கட்டமைப்பு object_cb_data *data = vdata;

	எனில் (oidset_செருகு(data->seen, oid))
		திரும்பு 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(வெற்று, data->scratch, data->opt, data->expand,
			   pack, offset);
	திரும்பு 0;
}

வகைவரை வெறுமை (*parse_cmd_fn_t)(கட்டமைப்பு batch_options *, மாறிலி எழுத்து *,
			       கட்டமைப்பு strbuf *, கட்டமைப்பு expand_data *);

கட்டமைப்பு queued_cmd {
	parse_cmd_fn_t fn;
	எழுத்து *வரி;
};

நிலையான வெறுமை parse_cmd_contents(கட்டமைப்பு batch_options *opt,
			     மாறிலி எழுத்து *வரி,
			     கட்டமைப்பு strbuf *output,
			     கட்டமைப்பு expand_data *data)
{
	எண்ணிக்கை object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(வரி, output, opt, data);
	data->info.typep = saved_typep;
}

நிலையான வெறுமை parse_cmd_info(கட்டமைப்பு batch_options *opt,
			   மாறிலி எழுத்து *வரி,
			   கட்டமைப்பு strbuf *output,
			   கட்டமைப்பு expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(வரி, output, opt, data);
}

நிலையான வெறுமை parse_cmd_mailmap(கட்டமைப்பு batch_options *opt UNUSED,
			      மாறிலி எழுத்து *வரி,
			      கட்டமைப்பு strbuf *output UNUSED,
			      கட்டமைப்பு expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(வரி);

	எனில் (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), வரி);

	எனில் (use_mailmap)
		load_mailmap();
}

நிலையான வெறுமை parse_cmd_remote_object_info(கட்டமைப்பு batch_options *opt,
					 மாறிலி எழுத்து *வரி, கட்டமைப்பு strbuf *output,
					 கட்டமைப்பு expand_data *data)
{
	முழு மொத்தம்;
	மாறிலி எழுத்து **argv;
	எழுத்து *வரி_to_split;
	கட்டமைப்பு fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	கட்டமைப்பு oid_array object_info_oids = OID_ARRAY_INIT;

	எனில் (strlen(வரி) >= MAX_REMOTE_OBJ_INFO_வரி)
		die(_("remote-object-info command too long"));

	வரி_to_split = xstrdup(வரி);
	மொத்தம் = split_cmdline(வரி_to_split, &argv);
	எனில் (மொத்தம் < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(மொத்தம்));
	எனில் (மொத்தம் - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	எனில் (data->info.sizep)
		results.wants_size = 1;
	எனில் (data->info.typep)
		results.wants_type = 1;

	எனில் (get_remote_info(மொத்தம், argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	எனில் (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	எனில் (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	க்கு (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		எனில் (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			தொடர்;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		எனில் (results.sizes)
			data->size = results.sizes[i];

		எனில் (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, வெற்று, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	விடுவி(வரி_to_split);
	விடுவி(argv);
	oid_array_clear(&object_info_oids);
}

நிலையான வெறுமை dispatch_calls(கட்டமைப்பு batch_options *opt,
		கட்டமைப்பு strbuf *output,
		கட்டமைப்பு expand_data *data,
		கட்டமைப்பு queued_cmd *cmd,
		size_t nr)
{
	எனில் (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	க்கு (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].வரி, output, data);

	fflush(stdout);
}

நிலையான வெறுமை free_cmds(கட்டமைப்பு queued_cmd *cmd, size_t *nr)
{
	க்கு (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].வரி);

	*nr = 0;
}


நிலையான மாறிலி கட்டமைப்பு parse_cmd {
	மாறிலி எழுத்து *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", வெற்று, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

நிலையான வெறுமை batch_objects_command(கட்டமைப்பு batch_options *opt,
				    கட்டமைப்பு strbuf *output,
				    கட்டமைப்பு expand_data *data)
{
	கட்டமைப்பு strbuf உள்ளீடு = STRBUF_INIT;
	கட்டமைப்பு queued_cmd *queued_cmd = வெற்று;
	size_t alloc = 0, nr = 0;

	போது (strbuf_getdelim_strip_crlf(&உள்ளீடு, stdin, opt->உள்ளீடு_delim) != EOF) {
		மாறிலி கட்டமைப்பு parse_cmd *cmd = வெற்று;
		மாறிலி எழுத்து *p = வெற்று, *cmd_end;
		கட்டமைப்பு queued_cmd call = {0};

		எனில் (!உள்ளீடு.len)
			die(_("empty command in input"));
		எனில் (isspace(*உள்ளீடு.buf))
			die(_("whitespace before command: '%s'"), உள்ளீடு.buf);

		க்கு (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			எனில் (!skip_prefix(உள்ளீடு.buf, commands[i].name, &cmd_end))
				தொடர்;

			cmd = &commands[i];
			எனில் (cmd->takes_args) {
				எனில் (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} இல்லையெனில் எனில் (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			நிறுத்து;
		}

		எனில் (!cmd)
			die(_("unknown command: '%s'"), உள்ளீடு.buf);

		எனில் (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} இல்லையெனில் எனில் (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} இல்லையெனில் {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.வரி = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	எனில் (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	விடுவி(queued_cmd);
	strbuf_release(&உள்ளீடு);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

வகைவரை முழு (*for_each_object_fn)(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு packed_git *pack,
				  off_t offset, வெறுமை *data);

கட்டமைப்பு for_each_object_payload {
	for_each_object_fn callback;
	வெறுமை *payload;
};

நிலையான முழு batch_one_object_oi(மாறிலி கட்டமைப்பு object_id *oid,
			       கட்டமைப்பு object_info *oi,
			       வெறுமை *_payload)
{
	கட்டமைப்பு for_each_object_payload *payload = _payload;
	எனில் (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		திரும்பு payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	திரும்பு payload->callback(oid, வெற்று, 0, payload->payload);
}

நிலையான வெறுமை batch_each_object(கட்டமைப்பு batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      வெறுமை *_payload)
{
	கட்டமைப்பு for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	கட்டமைப்பு odb_source_info source_info;
	கட்டமைப்பு object_info oi = {
		.source_infop = &source_info,
	};
	கட்டமைப்பு odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

நிலையான முழு batch_objects(கட்டமைப்பு batch_options *opt)
{
	கட்டமைப்பு strbuf உள்ளீடு = STRBUF_INIT;
	கட்டமைப்பு strbuf output = STRBUF_INIT;
	கட்டமைப்பு expand_data data = EXPAND_DATA_INIT;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	முழு save_warning;
	முழு retval = 0;

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
	எனில் (opt->transform_mode)
		data.split_on_whitespace = 1;

	எனில் (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = வெற்று;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	எனில் (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	எனில் (opt->all_objects) {
		கட்டமைப்பு object_cb_data cb;
		கட்டமைப்பு object_info empty = OBJECT_INFO_INIT;

		எனில் (!memcmp(&data.info, &empty, அளவு(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		எனில் (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		எனில் (opt->unordered) {
			கட்டமைப்பு oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} இல்லையெனில் {
			கட்டமைப்பு oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		திரும்பு 0;
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

	எனில் (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		செல் cleanup;
	}

	போது (strbuf_getdelim_strip_crlf(&உள்ளீடு, stdin, opt->உள்ளீடு_delim) != EOF) {
		எனில் (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			எழுத்து *p = strpbrk(உள்ளீடு.buf, " \t");
			எனில் (p) {
				போது (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(உள்ளீடு.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&உள்ளீடு);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	திரும்பு retval;
}

நிலையான முழு git_cat_file_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			       மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (userdiff_config(var, மதிப்பு) < 0)
		திரும்பு -1;

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான முழு batch_option_callback(மாறிலி கட்டமைப்பு option *opt,
				 மாறிலி எழுத்து *arg,
				 முழு unset)
{
	கட்டமைப்பு batch_options *bo = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (bo->enabled) {
		திரும்பு error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	எனில் (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	இல்லையெனில் எனில் (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	இல்லையெனில் எனில் (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	இல்லையெனில்
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	திரும்பு 0;
}

முழு cmd_cat_file(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	முழு opt = 0;
	முழு opt_cw = 0;
	முழு opt_epts = 0;
	மாறிலி எழுத்து *exp_type = வெற்று, *obj_name = வெற்று;
	கட்டமைப்பு batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	முழு unknown_type = 0;
	முழு உள்ளீடு_nul_terminated = 0;
	முழு nul_terminated = 0;
	முழு ret;

	மாறிலி எழுத்து * மாறிலி builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		வெற்று
	};
	மாறிலி கட்டமைப்பு option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', வெற்று, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', வெற்று, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', வெற்று, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', வெற்று, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', வெற்று, &உள்ளீடு_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', வெற்று, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, வெற்று);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	எனில் (use_mailmap)
		load_mailmap();

	தேர்வு (batch.objects_filter.choice) {
	நிலை LOFC_DISABLED:
		நிறுத்து;
	நிலை LOFC_BLOB_NONE:
	நிலை LOFC_BLOB_LIMIT:
	நிலை LOFC_OBJECT_TYPE:
		எனில் (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		நிறுத்து;
	இயல்பு:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	எனில் (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	எனில் (force_path && !opt_cw)
		usage_செய்தி_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	எனில் (batch.enabled)
		;
	இல்லையெனில் எனில் (batch.follow_symlinks)
		usage_செய்தி_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	இல்லையெனில் எனில் (batch.buffer_output >= 0)
		usage_செய்தி_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	இல்லையெனில் எனில் (batch.all_objects)
		usage_செய்தி_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	இல்லையெனில் எனில் (உள்ளீடு_nul_terminated)
		usage_செய்தி_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	இல்லையெனில் எனில் (nul_terminated)
		usage_செய்தி_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.உள்ளீடு_delim = batch.output_delim = '\n';
	எனில் (உள்ளீடு_nul_terminated)
		batch.உள்ளீடு_delim = '\0';
	எனில் (nul_terminated)
		batch.உள்ளீடு_delim = batch.output_delim = '\0';

	/* Batch defaults */
	எனில் (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	எனில் (batch.enabled) {
		எனில் (opt_cw)
			batch.transform_mode = opt;
		இல்லையெனில் எனில் (opt && opt != 'b')
			usage_செய்தி_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		இல்லையெனில் எனில் (argc)
			usage_செய்தி_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		செல் out;
	}

	எனில் (opt) {
		எனில் (!argc && opt == 'c')
			usage_செய்தி_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		இல்லையெனில் எனில் (!argc && opt == 'w')
			usage_செய்தி_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		இல்லையெனில் எனில் (!argc && opt_epts)
			usage_செய்தி_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		இல்லையெனில் எனில் (argc == 1)
			obj_name = argv[0];
		இல்லையெனில்
			usage_செய்தி_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} இல்லையெனில் எனில் (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} இல்லையெனில் எனில் (argc != 2) {
		usage_செய்தி_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} இல்லையெனில் எனில் (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	திரும்பு ret;
}
