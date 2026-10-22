/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "convert.h"
#ضمن "diff.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "ident.h"
#ضمن "list-objects-filter-options.h"
#ضمن "parse-options.h"
#ضمن "userdiff.h"
#ضمن "oid-array.h"
#ضمن "packfile.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "replace-object.h"
#ضمن "promisor-remote.h"
#ضمن "mailmap.h"
#ضمن "write-or-die.h"
#ضمن "alias.h"
#ضمن "remote.h"
#ضمن "transport.h"
#ضمن "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_LINE \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

تعداد batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

بنية batch_options {
	بنية list_objects_filter_options objects_filter;
	صحيح enabled;
	صحيح follow_symlinks;
	تعداد batch_mode batch_mode;
	صحيح buffer_output;
	صحيح all_objects;
	صحيح unordered;
	صحيح transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	حرف input_delim;
	حرف output_delim;
	ثابت حرف *format;
};

ساكن ثابت حرف *force_path;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;
ساكن صحيح use_mailmap;

ساكن حرف *replace_idents_using_mailmap(حرف *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ساكن فراغ load_mailmap(فراغ)
{
	ܐܢ (mailmap.strdup_strings)
		ܦܢܐ;

	read_mailmap(the_repository, &mailmap);
}

ساكن حرف *replace_idents_using_mailmap(حرف *object_buf, size_t *size)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *headers[] = { "author ", "committer ", "tagger ", عدم };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	ܦܢܐ strbuf_detach(&sb, عدم);
}

ساكن صحيح filter_object(ثابت حرف *path, unsigned mode,
			 ثابت بنية object_id *oid,
			 حرف **buf, size_t *size)
{
	تعداد object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	ܐܢ (!*buf)
		ܦܢܐ error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	ܐܢ ((type == OBJ_BLOB) && S_ISREG(mode)) {
		بنية strbuf strbuf = STRBUF_INIT;
		بنية checkout_metadata meta;

		init_checkout_metadata(&meta, عدم, عدم, oid);
		ܐܢ (convert_to_working_tree(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			حرر(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, عدم);
		}
	}

	ܦܢܐ 0;
}

ساكن صحيح stream_blob(ثابت بنية object_id *oid)
{
	ܐܢ (odb_stream_blob_to_fd(the_repository->objects, 1, oid, عدم, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	ܦܢܐ 0;
}

ساكن صحيح cat_one_file(صحيح opt, ثابت حرف *exp_type, ثابت حرف *obj_name)
{
	صحيح ret;
	بنية object_id oid;
	تعداد object_type type;
	حرف *buf;
	size_t size;
	بنية object_context obj_context = {0};
	بنية object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	ثابت حرف *path = force_path;
	ثابت صحيح opt_cw = (opt == 'c' || opt == 'w');
	ܐܢ (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	ܐܢ (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	ܐܢ (!path)
		path = obj_context.path;
	ܐܢ (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = عدم;
	بدل (opt) {
	حالة 't':
		oi.typep = &type;
		ܐܢ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		اذهب cleanup;

	حالة 's':
		oi.sizep = &size;

		ܐܢ (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (فراغ**)&buf;
		}

		ܐܢ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		ܐܢ (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		اذهب cleanup;

	حالة 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		اذهب cleanup;

	حالة 'w':

		ܐܢ (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			اذهب cleanup;
		}
		اكسر;

	حالة 'c':
	{
		unsigned long size_ul = 0;
		صحيح textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		ܐܢ (textconv_ret)
			اكسر;
	}
		/* else fallthrough */

	حالة 'p':
		type = odb_read_object_info(the_repository->objects, &oid, عدم);
		ܐܢ (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		ܐܢ (type == OBJ_TREE) {
			ثابت حرف *ls_args[3] = { عدم };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_tree(2, ls_args, عدم, the_repository);
			اذهب cleanup;
		}

		ܐܢ (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			اذهب cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		ܐܢ (!buf)
			die("Cannot read object %s", obj_name);

		ܐܢ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		اكسر;

	حالة 0:
	{
		تعداد object_type exp_type_id = type_from_string(exp_type);

		ܐܢ (exp_type_id == OBJ_BLOB) {
			بنية object_id blob_oid;
			ܐܢ (odb_read_object_info(the_repository->objects,
						 &oid, عدم) == OBJ_TAG) {
				حرف *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				ثابت حرف *target;

				ܐܢ (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				ܐܢ (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				حرر(buffer);
			} ܐܚܪܢܐ
				oidcpy(&blob_oid, &oid);

			ܐܢ (odb_read_object_info(the_repository->objects,
						 &blob_oid, عدم) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				اذهب cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, عدم);

		ܐܢ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		اكسر;
	}
	افتراضي:
		die("git cat-file: unknown option: %s", exp_type);
	}

	ܐܢ (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	حرر(buf);
	object_context_release(&obj_context);
	ܦܢܐ ret;
}

بنية expand_data {
	بنية object_id oid;
	تعداد object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	ثابت حرف *rest;
	بنية object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	صحيح mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	صحيح split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	بنية object_info info;

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
	بنية string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ساكن صحيح is_atom(ثابت حرف *atom, ثابت حرف *s, صحيح slen)
{
	صحيح alen = strlen(atom);
	ܦܢܐ alen == slen && !memcmp(atom, s, alen);
}

ساكن صحيح expand_atom(بنية strbuf *sb, ثابت حرف *atom, صحيح len,
		       بنية expand_data *data)
{
	ܐܢ (data->is_remote) {
		size_t i;
		لكل (i = 0; i < data->remote_allowed_atoms.nr; i++)
			ܐܢ (is_atom(data->remote_allowed_atoms.items[i].string,
				    atom, len))
				اكسر;
		ܐܢ (i == data->remote_allowed_atoms.nr)
			ܦܢܐ 1;
	}

	ܐܢ (is_atom("objectname", atom, len)) {
		ܐܢ (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} ܐܚܪܢܐ ܐܢ (is_atom("objecttype", atom, len)) {
		ܐܢ (data->mark_query) {
			data->info.typep = &data->type;
		} ܐܚܪܢܐ {
			ثابت حرف *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} ܐܚܪܢܐ ܐܢ (is_atom("objectsize", atom, len)) {
		ܐܢ (data->mark_query)
			data->info.sizep = &data->size;
		ܐܚܪܢܐ
			strbuf_add_uint(sb, data->size);
	} ܐܚܪܢܐ ܐܢ (is_atom("objectsize:disk", atom, len)) {
		ܐܢ (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		ܐܚܪܢܐ
			strbuf_add_uint(sb, data->disk_size);
	} ܐܚܪܢܐ ܐܢ (is_atom("rest", atom, len)) {
		ܐܢ (data->mark_query)
			data->split_on_whitespace = 1;
		ܐܚܪܢܐ ܐܢ (data->rest)
			strbuf_addstr(sb, data->rest);
	} ܐܚܪܢܐ ܐܢ (is_atom("deltabase", atom, len)) {
		ܐܢ (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		ܐܚܪܢܐ
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} ܐܚܪܢܐ ܐܢ (is_atom("objectmode", atom, len)) {
		ܐܢ (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} ܐܚܪܢܐ
		ܦܢܐ 0;
	ܦܢܐ 1;
}

ساكن فراغ expand_format(بنية strbuf *sb, ثابت حرف *start,
			  بنية expand_data *data)
{
	ܟܡܐ (strbuf_expand_step(sb, &start)) {
		ثابت حرف *end;

		ܐܢ (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		ܐܚܪܢܐ ܐܢ ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		ܐܚܪܢܐ
			strbuf_expand_bad_format(start, "cat-file");
	}
}

ساكن فراغ batch_write(بنية batch_options *opt, ثابت فراغ *data, صحيح len)
{
	ܐܢ (opt->buffer_output) {
		ܐܢ (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} ܐܚܪܢܐ
		write_or_die(1, data, len);
}

ساكن فراغ print_object_or_die(بنية batch_options *opt, بنية expand_data *data)
{
	ثابت بنية object_id *oid = &data->oid;

	أكد(data->info.typep);

	ܐܢ (data->type == OBJ_BLOB) {
		ܐܢ (opt->buffer_output)
			fflush(stdout);
		ܐܢ (opt->transform_mode) {
			حرف *contents;
			size_t size;

			ܐܢ (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			ܐܢ (opt->transform_mode == 'w') {
				ܐܢ (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} ܐܚܪܢܐ ܐܢ (opt->transform_mode == 'c') {
				تعداد object_type type;
				unsigned long size_ul = 0;
				ܐܢ (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				ܐܚܪܢܐ
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				ܐܢ (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} ܐܚܪܢܐ
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			حرر(contents);
		} ܐܚܪܢܐ {
			stream_blob(oid);
		}
	}
	ܐܚܪܢܐ {
		تعداد object_type type;
		size_t size;
		فراغ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		ܐܢ (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		ܐܢ (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		ܐܢ (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		ܐܢ (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		حرر(contents);
	}
}

ساكن فراغ print_default_format(بنية strbuf *scratch, بنية expand_data *data,
				 بنية batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

ساكن فراغ report_object_status(بنية batch_options *opt,
				 ثابت حرف *obj_name,
				 ثابت بنية object_id *oid,
				 ثابت حرف *status)
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
ساكن فراغ batch_object_write(ثابت حرف *obj_name,
			       بنية strbuf *scratch,
			       بنية batch_options *opt,
			       بنية expand_data *data,
			       بنية packed_git *pack,
			       off_t offset)
{
	ܐܢ (!data->skip_object_info) {
		صحيح ret;

		ܐܢ (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		ܐܢ (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		ܐܢ (pack)
			ret = packed_object_info(عدم, pack, offset, &data->info);
		ܐܚܪܢܐ
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		ܐܢ (ret < 0) {
			ܐܢ (data->mode == S_IFGITLINK)
				report_object_status(opt, عدم, &data->oid, "submodule");
			ܐܚܪܢܐ
				report_object_status(opt, obj_name, &data->oid, "missing");
			ܦܢܐ;
		}

		بدل (opt->objects_filter.choice) {
		حالة LOFC_DISABLED:
			اكسر;
		حالة LOFC_BLOB_NONE:
			ܐܢ (data->type == OBJ_BLOB) {
				ܐܢ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ܦܢܐ;
			}
			اكسر;
		حالة LOFC_BLOB_LIMIT:
			ܐܢ (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_value) {
				ܐܢ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ܦܢܐ;
			}
			اكسر;
		حالة LOFC_OBJECT_TYPE:
			ܐܢ (data->type != opt->objects_filter.object_type) {
				ܐܢ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ܦܢܐ;
			}
			اكسر;
		افتراضي:
			BUG("unsupported objects filter");
		}

		ܐܢ (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			حرف *buf = عدم;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			ܐܢ (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			حرر(buf);
		}
	}

	strbuf_reset(scratch);

	ܐܢ (!opt->format) {
		print_default_format(scratch, data, opt);
	} ܐܚܪܢܐ {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	ܐܢ (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ساكن فراغ batch_one_object(ثابت حرف *obj_name,
			     بنية strbuf *scratch,
			     بنية batch_options *opt,
			     بنية expand_data *data)
{
	بنية object_context ctx = {0};
	صحيح flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	تعداد get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	ܐܢ (result != FOUND) {
		بدل (result) {
		حالة MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			اكسر;
		حالة SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			اكسر;
		حالة DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		افتراضي:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			اكسر;
		}
		fflush(stdout);

		اذهب out;
	}

	ܐܢ (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		اذهب out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, عدم, 0);

out:
	object_context_release(&ctx);
}

ساكن صحيح get_remote_info(صحيح argc,
			   ثابت حرف **argv,
			   بنية fetch_object_info_results *results,
			   بنية oid_array *object_info_oids)
{
	صحيح retval = 0;
	بنية remote *remote = عدم;
	بنية object_id oid;
	بنية transport *gtransport;

	remote = remote_get(argv[0]);
	ܐܢ (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	لكل (size_t i = 1; i < argc; i++) {
		ܐܢ (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			ܐܢ (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				لكل (j = 0; j < len; j++)
					ܐܢ (!isxdigit(argv[i][j]))
						اكسر;
				ܐܢ (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (صحيح)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	ܐܢ (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, عدم);

	ܐܢ (!gtransport->smart_options) {
		retval = -1;
		اذهب cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ܦܢܐ retval;
}

بنية object_cb_data {
	بنية batch_options *opt;
	بنية expand_data *expand;
	بنية oidset *seen;
	بنية strbuf *scratch;
};

ساكن صحيح batch_object_cb(ثابت بنية object_id *oid, فراغ *vdata)
{
	بنية object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(عدم, data->scratch, data->opt, data->expand,
			   عدم, 0);
	ܦܢܐ 0;
}

ساكن صحيح collect_object(ثابت بنية object_id *oid,
			  بنية packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  فراغ *data)
{
	oid_array_append(data, oid);
	ܦܢܐ 0;
}

ساكن صحيح batch_unordered_object(ثابت بنية object_id *oid,
				  بنية packed_git *pack,
				  off_t offset,
				  فراغ *vdata)
{
	بنية object_cb_data *data = vdata;

	ܐܢ (oidset_insert(data->seen, oid))
		ܦܢܐ 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(عدم, data->scratch, data->opt, data->expand,
			   pack, offset);
	ܦܢܐ 0;
}

تعريفنوع فراغ (*parse_cmd_fn_t)(بنية batch_options *, ثابت حرف *,
			       بنية strbuf *, بنية expand_data *);

بنية queued_cmd {
	parse_cmd_fn_t fn;
	حرف *line;
};

ساكن فراغ parse_cmd_contents(بنية batch_options *opt,
			     ثابت حرف *line,
			     بنية strbuf *output,
			     بنية expand_data *data)
{
	تعداد object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(line, output, opt, data);
	data->info.typep = saved_typep;
}

ساكن فراغ parse_cmd_info(بنية batch_options *opt,
			   ثابت حرف *line,
			   بنية strbuf *output,
			   بنية expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(line, output, opt, data);
}

ساكن فراغ parse_cmd_mailmap(بنية batch_options *opt UNUSED,
			      ثابت حرف *line,
			      بنية strbuf *output UNUSED,
			      بنية expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(line);

	ܐܢ (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), line);

	ܐܢ (use_mailmap)
		load_mailmap();
}

ساكن فراغ parse_cmd_remote_object_info(بنية batch_options *opt,
					 ثابت حرف *line, بنية strbuf *output,
					 بنية expand_data *data)
{
	صحيح count;
	ثابت حرف **argv;
	حرف *line_to_split;
	بنية fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	بنية oid_array object_info_oids = OID_ARRAY_INIT;

	ܐܢ (strlen(line) >= MAX_REMOTE_OBJ_INFO_LINE)
		die(_("remote-object-info command too long"));

	line_to_split = xstrdup(line);
	count = split_cmdline(line_to_split, &argv);
	ܐܢ (count < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(count));
	ܐܢ (count - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	ܐܢ (data->info.sizep)
		results.wants_size = 1;
	ܐܢ (data->info.typep)
		results.wants_type = 1;

	ܐܢ (get_remote_info(count, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	ܐܢ (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	ܐܢ (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	لكل (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		ܐܢ (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			استمر;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		ܐܢ (results.sizes)
			data->size = results.sizes[i];

		ܐܢ (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, عدم, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	حرر(line_to_split);
	حرر(argv);
	oid_array_clear(&object_info_oids);
}

ساكن فراغ dispatch_calls(بنية batch_options *opt,
		بنية strbuf *output,
		بنية expand_data *data,
		بنية queued_cmd *cmd,
		size_t nr)
{
	ܐܢ (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	لكل (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].line, output, data);

	fflush(stdout);
}

ساكن فراغ free_cmds(بنية queued_cmd *cmd, size_t *nr)
{
	لكل (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].line);

	*nr = 0;
}


ساكن ثابت بنية parse_cmd {
	ثابت حرف *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", عدم, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ساكن فراغ batch_objects_command(بنية batch_options *opt,
				    بنية strbuf *output,
				    بنية expand_data *data)
{
	بنية strbuf input = STRBUF_INIT;
	بنية queued_cmd *queued_cmd = عدم;
	size_t alloc = 0, nr = 0;

	ܟܡܐ (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		ثابت بنية parse_cmd *cmd = عدم;
		ثابت حرف *p = عدم, *cmd_end;
		بنية queued_cmd call = {0};

		ܐܢ (!input.len)
			die(_("empty command in input"));
		ܐܢ (isspace(*input.buf))
			die(_("whitespace before command: '%s'"), input.buf);

		لكل (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			ܐܢ (!skip_prefix(input.buf, commands[i].name, &cmd_end))
				استمر;

			cmd = &commands[i];
			ܐܢ (cmd->takes_args) {
				ܐܢ (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} ܐܚܪܢܐ ܐܢ (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			اكسر;
		}

		ܐܢ (!cmd)
			die(_("unknown command: '%s'"), input.buf);

		ܐܢ (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} ܐܚܪܢܐ ܐܢ (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} ܐܚܪܢܐ {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.line = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	ܐܢ (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	حرر(queued_cmd);
	strbuf_release(&input);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

تعريفنوع صحيح (*for_each_object_fn)(ثابت بنية object_id *oid, بنية packed_git *pack,
				  off_t offset, فراغ *data);

بنية for_each_object_payload {
	for_each_object_fn callback;
	فراغ *payload;
};

ساكن صحيح batch_one_object_oi(ثابت بنية object_id *oid,
			       بنية object_info *oi,
			       فراغ *_payload)
{
	بنية for_each_object_payload *payload = _payload;
	ܐܢ (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		ܦܢܐ payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	ܦܢܐ payload->callback(oid, عدم, 0, payload->payload);
}

ساكن فراغ batch_each_object(بنية batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      فراغ *_payload)
{
	بنية for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	بنية odb_source_info source_info;
	بنية object_info oi = {
		.source_infop = &source_info,
	};
	بنية odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ساكن صحيح batch_objects(بنية batch_options *opt)
{
	بنية strbuf input = STRBUF_INIT;
	بنية strbuf output = STRBUF_INIT;
	بنية expand_data data = EXPAND_DATA_INIT;
	بنية repo_config_values *cfg = repo_config_values(the_repository);
	صحيح save_warning;
	صحيح retval = 0;

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
	ܐܢ (opt->transform_mode)
		data.split_on_whitespace = 1;

	ܐܢ (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = عدم;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	ܐܢ (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	ܐܢ (opt->all_objects) {
		بنية object_cb_data cb;
		بنية object_info empty = OBJECT_INFO_INIT;

		ܐܢ (!memcmp(&data.info, &empty, حجم(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		ܐܢ (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		ܐܢ (opt->unordered) {
			بنية oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} ܐܚܪܢܐ {
			بنية oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ܦܢܐ 0;
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

	ܐܢ (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		اذهب cleanup;
	}

	ܟܡܐ (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		ܐܢ (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			حرف *p = strpbrk(input.buf, " \t");
			ܐܢ (p) {
				ܟܡܐ (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(input.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&input);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	ܦܢܐ retval;
}

ساكن صحيح git_cat_file_config(ثابت حرف *var, ثابت حرف *value,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	ܐܢ (userdiff_config(var, value) < 0)
		ܦܢܐ -1;

	ܦܢܐ git_default_config(var, value, ctx, cb);
}

ساكن صحيح batch_option_callback(ثابت بنية option *opt,
				 ثابت حرف *arg,
				 صحيح unset)
{
	بنية batch_options *bo = opt->value;

	BUG_ON_OPT_NEG(unset);

	ܐܢ (bo->enabled) {
		ܦܢܐ error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	ܐܢ (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	ܐܚܪܢܐ ܐܢ (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	ܐܚܪܢܐ ܐܢ (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	ܐܚܪܢܐ
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	ܦܢܐ 0;
}

صحيح cmd_cat_file(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	صحيح opt = 0;
	صحيح opt_cw = 0;
	صحيح opt_epts = 0;
	ثابت حرف *exp_type = عدم, *obj_name = عدم;
	بنية batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	صحيح unknown_type = 0;
	صحيح input_nul_terminated = 0;
	صحيح nul_terminated = 0;
	صحيح ret;

	ثابت حرف * ثابت builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		عدم
	};
	ثابت بنية option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', عدم, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', عدم, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', عدم, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', عدم, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', عدم, &input_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', عدم, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, عدم);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	ܐܢ (use_mailmap)
		load_mailmap();

	بدل (batch.objects_filter.choice) {
	حالة LOFC_DISABLED:
		اكسر;
	حالة LOFC_BLOB_NONE:
	حالة LOFC_BLOB_LIMIT:
	حالة LOFC_OBJECT_TYPE:
		ܐܢ (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		اكسر;
	افتراضي:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	ܐܢ (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	ܐܢ (force_path && !opt_cw)
		usage_msg_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	ܐܢ (batch.enabled)
		;
	ܐܚܪܢܐ ܐܢ (batch.follow_symlinks)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	ܐܚܪܢܐ ܐܢ (batch.buffer_output >= 0)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	ܐܚܪܢܐ ܐܢ (batch.all_objects)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	ܐܚܪܢܐ ܐܢ (input_nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	ܐܚܪܢܐ ܐܢ (nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.input_delim = batch.output_delim = '\n';
	ܐܢ (input_nul_terminated)
		batch.input_delim = '\0';
	ܐܢ (nul_terminated)
		batch.input_delim = batch.output_delim = '\0';

	/* Batch defaults */
	ܐܢ (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	ܐܢ (batch.enabled) {
		ܐܢ (opt_cw)
			batch.transform_mode = opt;
		ܐܚܪܢܐ ܐܢ (opt && opt != 'b')
			usage_msg_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		ܐܚܪܢܐ ܐܢ (argc)
			usage_msg_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		اذهب out;
	}

	ܐܢ (opt) {
		ܐܢ (!argc && opt == 'c')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		ܐܚܪܢܐ ܐܢ (!argc && opt == 'w')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		ܐܚܪܢܐ ܐܢ (!argc && opt_epts)
			usage_msg_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		ܐܚܪܢܐ ܐܢ (argc == 1)
			obj_name = argv[0];
		ܐܚܪܢܐ
			usage_msg_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} ܐܚܪܢܐ ܐܢ (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} ܐܚܪܢܐ ܐܢ (argc != 2) {
		usage_msg_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} ܐܚܪܢܐ ܐܢ (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	ܦܢܐ ret;
}
