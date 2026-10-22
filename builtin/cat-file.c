/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "convert.h"
#شامل "diff.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "ident.h"
#شامل "list-objects-filter-options.h"
#شامل "parse-options.h"
#شامل "userdiff.h"
#شامل "oid-array.h"
#شامل "packfile.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "replace-object.h"
#شامل "promisor-remote.h"
#شامل "mailmap.h"
#شامل "write-or-die.h"
#شامل "alias.h"
#شامل "remote.h"
#شامل "transport.h"
#شامل "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_سطر \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

شمارش batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

ساختار batch_options {
	ساختار list_objects_filter_options objects_filter;
	صحیح enabled;
	صحیح follow_symlinks;
	شمارش batch_mode batch_mode;
	صحیح buffer_output;
	صحیح all_objects;
	صحیح unordered;
	صحیح transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	نویسه درون_داد_delim;
	نویسه output_delim;
	ثابت نویسه *format;
};

ایستا ثابت نویسه *force_path;

ایستا ساختار string_list mailmap = STRING_LIST_INIT_NODUP;
ایستا صحیح use_mailmap;

ایستا نویسه *replace_idents_using_mailmap(نویسه *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ایستا تهی load_mailmap(تهی)
{
	اگر (mailmap.strdup_strings)
		بازگشت;

	read_mailmap(the_repository, &mailmap);
}

ایستا نویسه *replace_idents_using_mailmap(نویسه *object_buf, size_t *size)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *headers[] = { "author ", "committer ", "tagger ", پوچ };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	بازگشت strbuf_detach(&sb, پوچ);
}

ایستا صحیح filter_object(ثابت نویسه *path, unsigned mode,
			 ثابت ساختار object_id *oid,
			 نویسه **buf, size_t *size)
{
	شمارش object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	اگر (!*buf)
		بازگشت error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	اگر ((type == OBJ_BLOB) && S_ISREG(mode)) {
		ساختار strbuf strbuf = STRBUF_INIT;
		ساختار checkout_metadata meta;

		init_checkout_metadata(&meta, پوچ, پوچ, oid);
		اگر (convert_to_working_درخت(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			آزاد(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, پوچ);
		}
	}

	بازگشت 0;
}

ایستا صحیح stream_blob(ثابت ساختار object_id *oid)
{
	اگر (odb_stream_blob_to_fd(the_repository->objects, 1, oid, پوچ, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	بازگشت 0;
}

ایستا صحیح cat_one_file(صحیح opt, ثابت نویسه *exp_type, ثابت نویسه *obj_name)
{
	صحیح ret;
	ساختار object_id oid;
	شمارش object_type type;
	نویسه *buf;
	size_t size;
	ساختار object_context obj_context = {0};
	ساختار object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	ثابت نویسه *path = force_path;
	ثابت صحیح opt_cw = (opt == 'c' || opt == 'w');
	اگر (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	اگر (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	اگر (!path)
		path = obj_context.path;
	اگر (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = پوچ;
	انتخاب (opt) {
	حالت 't':
		oi.typep = &type;
		اگر (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		برو cleanup;

	حالت 's':
		oi.sizep = &size;

		اگر (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (تهی**)&buf;
		}

		اگر (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		اگر (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		برو cleanup;

	حالت 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		برو cleanup;

	حالت 'w':

		اگر (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			برو cleanup;
		}
		بشکن;

	حالت 'c':
	{
		unsigned long size_ul = 0;
		صحیح textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		اگر (textconv_ret)
			بشکن;
	}
		/* else fallthrough */

	حالت 'p':
		type = odb_read_object_info(the_repository->objects, &oid, پوچ);
		اگر (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		اگر (type == OBJ_درخت) {
			ثابت نویسه *ls_args[3] = { پوچ };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_درخت(2, ls_args, پوچ, the_repository);
			برو cleanup;
		}

		اگر (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			برو cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		اگر (!buf)
			die("Cannot read object %s", obj_name);

		اگر (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		بشکن;

	حالت 0:
	{
		شمارش object_type exp_type_id = type_from_string(exp_type);

		اگر (exp_type_id == OBJ_BLOB) {
			ساختار object_id blob_oid;
			اگر (odb_read_object_info(the_repository->objects,
						 &oid, پوچ) == OBJ_TAG) {
				نویسه *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				ثابت نویسه *target;

				اگر (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				اگر (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				آزاد(buffer);
			} وگرنه
				oidcpy(&blob_oid, &oid);

			اگر (odb_read_object_info(the_repository->objects,
						 &blob_oid, پوچ) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				برو cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, پوچ);

		اگر (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		بشکن;
	}
	پیشفرض:
		die("git cat-file: unknown option: %s", exp_type);
	}

	اگر (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	آزاد(buf);
	object_context_release(&obj_context);
	بازگشت ret;
}

ساختار expand_data {
	ساختار object_id oid;
	شمارش object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	ثابت نویسه *rest;
	ساختار object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	صحیح mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	صحیح split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	ساختار object_info info;

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
	ساختار string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ایستا صحیح is_atom(ثابت نویسه *atom, ثابت نویسه *s, صحیح slen)
{
	صحیح alen = strlen(atom);
	بازگشت alen == slen && !memcmp(atom, s, alen);
}

ایستا صحیح expand_atom(ساختار strbuf *sb, ثابت نویسه *atom, صحیح len,
		       ساختار expand_data *data)
{
	اگر (data->is_remote) {
		size_t i;
		برای (i = 0; i < data->remote_allowed_atoms.nr; i++)
			اگر (is_atom(data->remote_allowed_atoms.اقلام[i].string,
				    atom, len))
				بشکن;
		اگر (i == data->remote_allowed_atoms.nr)
			بازگشت 1;
	}

	اگر (is_atom("objectname", atom, len)) {
		اگر (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} وگرنه اگر (is_atom("objecttype", atom, len)) {
		اگر (data->mark_query) {
			data->info.typep = &data->type;
		} وگرنه {
			ثابت نویسه *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} وگرنه اگر (is_atom("objectsize", atom, len)) {
		اگر (data->mark_query)
			data->info.sizep = &data->size;
		وگرنه
			strbuf_add_uint(sb, data->size);
	} وگرنه اگر (is_atom("objectsize:disk", atom, len)) {
		اگر (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		وگرنه
			strbuf_add_uint(sb, data->disk_size);
	} وگرنه اگر (is_atom("rest", atom, len)) {
		اگر (data->mark_query)
			data->split_on_whitespace = 1;
		وگرنه اگر (data->rest)
			strbuf_addstr(sb, data->rest);
	} وگرنه اگر (is_atom("deltabase", atom, len)) {
		اگر (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		وگرنه
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} وگرنه اگر (is_atom("objectmode", atom, len)) {
		اگر (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} وگرنه
		بازگشت 0;
	بازگشت 1;
}

ایستا تهی expand_format(ساختار strbuf *sb, ثابت نویسه *start,
			  ساختار expand_data *data)
{
	وقتی (strbuf_expand_step(sb, &start)) {
		ثابت نویسه *end;

		اگر (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		وگرنه اگر ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		وگرنه
			strbuf_expand_bad_format(start, "cat-file");
	}
}

ایستا تهی batch_write(ساختار batch_options *opt, ثابت تهی *data, صحیح len)
{
	اگر (opt->buffer_output) {
		اگر (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} وگرنه
		write_or_die(1, data, len);
}

ایستا تهی print_object_or_die(ساختار batch_options *opt, ساختار expand_data *data)
{
	ثابت ساختار object_id *oid = &data->oid;

	أكد(data->info.typep);

	اگر (data->type == OBJ_BLOB) {
		اگر (opt->buffer_output)
			fflush(stdout);
		اگر (opt->transform_mode) {
			نویسه *contents;
			size_t size;

			اگر (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			اگر (opt->transform_mode == 'w') {
				اگر (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} وگرنه اگر (opt->transform_mode == 'c') {
				شمارش object_type type;
				unsigned long size_ul = 0;
				اگر (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				وگرنه
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				اگر (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} وگرنه
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			آزاد(contents);
		} وگرنه {
			stream_blob(oid);
		}
	}
	وگرنه {
		شمارش object_type type;
		size_t size;
		تهی *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		اگر (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		اگر (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		اگر (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		اگر (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		آزاد(contents);
	}
}

ایستا تهی print_default_format(ساختار strbuf *scratch, ساختار expand_data *data,
				 ساختار batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

ایستا تهی report_object_status(ساختار batch_options *opt,
				 ثابت نویسه *obj_name,
				 ثابت ساختار object_id *oid,
				 ثابت نویسه *status)
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
ایستا تهی batch_object_write(ثابت نویسه *obj_name,
			       ساختار strbuf *scratch,
			       ساختار batch_options *opt,
			       ساختار expand_data *data,
			       ساختار packed_git *pack,
			       off_t offset)
{
	اگر (!data->skip_object_info) {
		صحیح ret;

		اگر (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		اگر (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		اگر (pack)
			ret = packed_object_info(پوچ, pack, offset, &data->info);
		وگرنه
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		اگر (ret < 0) {
			اگر (data->mode == S_IFGITLINK)
				report_object_status(opt, پوچ, &data->oid, "submodule");
			وگرنه
				report_object_status(opt, obj_name, &data->oid, "missing");
			بازگشت;
		}

		انتخاب (opt->objects_filter.choice) {
		حالت LOFC_DISABLED:
			بشکن;
		حالت LOFC_BLOB_NONE:
			اگر (data->type == OBJ_BLOB) {
				اگر (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				بازگشت;
			}
			بشکن;
		حالت LOFC_BLOB_LIMIT:
			اگر (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_مقدار) {
				اگر (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				بازگشت;
			}
			بشکن;
		حالت LOFC_OBJECT_TYPE:
			اگر (data->type != opt->objects_filter.object_type) {
				اگر (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				بازگشت;
			}
			بشکن;
		پیشفرض:
			BUG("unsupported objects filter");
		}

		اگر (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			نویسه *buf = پوچ;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			اگر (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			آزاد(buf);
		}
	}

	strbuf_reset(scratch);

	اگر (!opt->format) {
		print_default_format(scratch, data, opt);
	} وگرنه {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	اگر (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ایستا تهی batch_one_object(ثابت نویسه *obj_name,
			     ساختار strbuf *scratch,
			     ساختار batch_options *opt,
			     ساختار expand_data *data)
{
	ساختار object_context ctx = {0};
	صحیح flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	شمارش get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	اگر (result != FOUND) {
		انتخاب (result) {
		حالت MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			بشکن;
		حالت SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			بشکن;
		حالت DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			بشکن;
		حالت SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			بشکن;
		حالت NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			بشکن;
		پیشفرض:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			بشکن;
		}
		fflush(stdout);

		برو out;
	}

	اگر (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		برو out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, پوچ, 0);

out:
	object_context_release(&ctx);
}

ایستا صحیح get_remote_info(صحیح argc,
			   ثابت نویسه **argv,
			   ساختار fetch_object_info_results *results,
			   ساختار oid_array *object_info_oids)
{
	صحیح retval = 0;
	ساختار remote *remote = پوچ;
	ساختار object_id oid;
	ساختار transport *gtransport;

	remote = remote_get(argv[0]);
	اگر (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	برای (size_t i = 1; i < argc; i++) {
		اگر (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			اگر (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				برای (j = 0; j < len; j++)
					اگر (!isxdigit(argv[i][j]))
						بشکن;
				اگر (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (صحیح)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	اگر (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, پوچ);

	اگر (!gtransport->smart_options) {
		retval = -1;
		برو cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	بازگشت retval;
}

ساختار object_cb_data {
	ساختار batch_options *opt;
	ساختار expand_data *expand;
	ساختار oidset *seen;
	ساختار strbuf *scratch;
};

ایستا صحیح batch_object_cb(ثابت ساختار object_id *oid, تهی *vdata)
{
	ساختار object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(پوچ, data->scratch, data->opt, data->expand,
			   پوچ, 0);
	بازگشت 0;
}

ایستا صحیح collect_object(ثابت ساختار object_id *oid,
			  ساختار packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  تهی *data)
{
	oid_array_append(data, oid);
	بازگشت 0;
}

ایستا صحیح batch_unordered_object(ثابت ساختار object_id *oid,
				  ساختار packed_git *pack,
				  off_t offset,
				  تهی *vdata)
{
	ساختار object_cb_data *data = vdata;

	اگر (oidset_درج(data->seen, oid))
		بازگشت 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(پوچ, data->scratch, data->opt, data->expand,
			   pack, offset);
	بازگشت 0;
}

تعریفنوع تهی (*parse_cmd_fn_t)(ساختار batch_options *, ثابت نویسه *,
			       ساختار strbuf *, ساختار expand_data *);

ساختار queued_cmd {
	parse_cmd_fn_t fn;
	نویسه *سطر;
};

ایستا تهی parse_cmd_contents(ساختار batch_options *opt,
			     ثابت نویسه *سطر,
			     ساختار strbuf *output,
			     ساختار expand_data *data)
{
	شمارش object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(سطر, output, opt, data);
	data->info.typep = saved_typep;
}

ایستا تهی parse_cmd_info(ساختار batch_options *opt,
			   ثابت نویسه *سطر,
			   ساختار strbuf *output,
			   ساختار expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(سطر, output, opt, data);
}

ایستا تهی parse_cmd_mailmap(ساختار batch_options *opt UNUSED,
			      ثابت نویسه *سطر,
			      ساختار strbuf *output UNUSED,
			      ساختار expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(سطر);

	اگر (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), سطر);

	اگر (use_mailmap)
		load_mailmap();
}

ایستا تهی parse_cmd_remote_object_info(ساختار batch_options *opt,
					 ثابت نویسه *سطر, ساختار strbuf *output,
					 ساختار expand_data *data)
{
	صحیح شمار;
	ثابت نویسه **argv;
	نویسه *سطر_to_split;
	ساختار fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	ساختار oid_array object_info_oids = OID_ARRAY_INIT;

	اگر (strlen(سطر) >= MAX_REMOTE_OBJ_INFO_سطر)
		die(_("remote-object-info command too long"));

	سطر_to_split = xstrdup(سطر);
	شمار = split_cmdline(سطر_to_split, &argv);
	اگر (شمار < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(شمار));
	اگر (شمار - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	اگر (data->info.sizep)
		results.wants_size = 1;
	اگر (data->info.typep)
		results.wants_type = 1;

	اگر (get_remote_info(شمار, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	اگر (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	اگر (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	برای (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		اگر (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			ادامه;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		اگر (results.sizes)
			data->size = results.sizes[i];

		اگر (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, پوچ, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	آزاد(سطر_to_split);
	آزاد(argv);
	oid_array_clear(&object_info_oids);
}

ایستا تهی dispatch_calls(ساختار batch_options *opt,
		ساختار strbuf *output,
		ساختار expand_data *data,
		ساختار queued_cmd *cmd,
		size_t nr)
{
	اگر (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	برای (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].سطر, output, data);

	fflush(stdout);
}

ایستا تهی free_cmds(ساختار queued_cmd *cmd, size_t *nr)
{
	برای (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].سطر);

	*nr = 0;
}


ایستا ثابت ساختار parse_cmd {
	ثابت نویسه *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", پوچ, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ایستا تهی batch_objects_command(ساختار batch_options *opt,
				    ساختار strbuf *output,
				    ساختار expand_data *data)
{
	ساختار strbuf درون_داد = STRBUF_INIT;
	ساختار queued_cmd *queued_cmd = پوچ;
	size_t alloc = 0, nr = 0;

	وقتی (strbuf_getdelim_strip_crlf(&درون_داد, stdin, opt->درون_داد_delim) != EOF) {
		ثابت ساختار parse_cmd *cmd = پوچ;
		ثابت نویسه *p = پوچ, *cmd_end;
		ساختار queued_cmd call = {0};

		اگر (!درون_داد.len)
			die(_("empty command in input"));
		اگر (isspace(*درون_داد.buf))
			die(_("whitespace before command: '%s'"), درون_داد.buf);

		برای (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			اگر (!skip_prefix(درون_داد.buf, commands[i].name, &cmd_end))
				ادامه;

			cmd = &commands[i];
			اگر (cmd->takes_args) {
				اگر (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} وگرنه اگر (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			بشکن;
		}

		اگر (!cmd)
			die(_("unknown command: '%s'"), درون_داد.buf);

		اگر (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} وگرنه اگر (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} وگرنه {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.سطر = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	اگر (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	آزاد(queued_cmd);
	strbuf_release(&درون_داد);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

تعریفنوع صحیح (*for_each_object_fn)(ثابت ساختار object_id *oid, ساختار packed_git *pack,
				  off_t offset, تهی *data);

ساختار for_each_object_payload {
	for_each_object_fn callback;
	تهی *payload;
};

ایستا صحیح batch_one_object_oi(ثابت ساختار object_id *oid,
			       ساختار object_info *oi,
			       تهی *_payload)
{
	ساختار for_each_object_payload *payload = _payload;
	اگر (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		بازگشت payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	بازگشت payload->callback(oid, پوچ, 0, payload->payload);
}

ایستا تهی batch_each_object(ساختار batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      تهی *_payload)
{
	ساختار for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	ساختار odb_source_info source_info;
	ساختار object_info oi = {
		.source_infop = &source_info,
	};
	ساختار odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ایستا صحیح batch_objects(ساختار batch_options *opt)
{
	ساختار strbuf درون_داد = STRBUF_INIT;
	ساختار strbuf output = STRBUF_INIT;
	ساختار expand_data data = EXPAND_DATA_INIT;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	صحیح save_warning;
	صحیح retval = 0;

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
	اگر (opt->transform_mode)
		data.split_on_whitespace = 1;

	اگر (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = پوچ;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	اگر (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	اگر (opt->all_objects) {
		ساختار object_cb_data cb;
		ساختار object_info empty = OBJECT_INFO_INIT;

		اگر (!memcmp(&data.info, &empty, اندازه(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		اگر (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		اگر (opt->unordered) {
			ساختار oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} وگرنه {
			ساختار oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		بازگشت 0;
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

	اگر (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		برو cleanup;
	}

	وقتی (strbuf_getdelim_strip_crlf(&درون_داد, stdin, opt->درون_داد_delim) != EOF) {
		اگر (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			نویسه *p = strpbrk(درون_داد.buf, " \t");
			اگر (p) {
				وقتی (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(درون_داد.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&درون_داد);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	بازگشت retval;
}

ایستا صحیح git_cat_file_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			       ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (userdiff_config(var, مقدار) < 0)
		بازگشت -1;

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا صحیح batch_option_callback(ثابت ساختار option *opt,
				 ثابت نویسه *arg,
				 صحیح unset)
{
	ساختار batch_options *bo = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (bo->enabled) {
		بازگشت error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	اگر (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	وگرنه اگر (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	وگرنه اگر (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	وگرنه
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	بازگشت 0;
}

صحیح cmd_cat_file(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo UNUSED)
{
	صحیح opt = 0;
	صحیح opt_cw = 0;
	صحیح opt_epts = 0;
	ثابت نویسه *exp_type = پوچ, *obj_name = پوچ;
	ساختار batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	صحیح unknown_type = 0;
	صحیح درون_داد_nul_terminated = 0;
	صحیح nul_terminated = 0;
	صحیح ret;

	ثابت نویسه * ثابت builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		پوچ
	};
	ثابت ساختار option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', پوچ, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', پوچ, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', پوچ, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', پوچ, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', پوچ, &درون_داد_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', پوچ, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, پوچ);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	اگر (use_mailmap)
		load_mailmap();

	انتخاب (batch.objects_filter.choice) {
	حالت LOFC_DISABLED:
		بشکن;
	حالت LOFC_BLOB_NONE:
	حالت LOFC_BLOB_LIMIT:
	حالت LOFC_OBJECT_TYPE:
		اگر (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		بشکن;
	پیشفرض:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	اگر (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	اگر (force_path && !opt_cw)
		usage_پیام_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	اگر (batch.enabled)
		;
	وگرنه اگر (batch.follow_symlinks)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	وگرنه اگر (batch.buffer_output >= 0)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	وگرنه اگر (batch.all_objects)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	وگرنه اگر (درون_داد_nul_terminated)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	وگرنه اگر (nul_terminated)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.درون_داد_delim = batch.output_delim = '\n';
	اگر (درون_داد_nul_terminated)
		batch.درون_داد_delim = '\0';
	اگر (nul_terminated)
		batch.درون_داد_delim = batch.output_delim = '\0';

	/* Batch defaults */
	اگر (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	اگر (batch.enabled) {
		اگر (opt_cw)
			batch.transform_mode = opt;
		وگرنه اگر (opt && opt != 'b')
			usage_پیام_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		وگرنه اگر (argc)
			usage_پیام_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		برو out;
	}

	اگر (opt) {
		اگر (!argc && opt == 'c')
			usage_پیام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		وگرنه اگر (!argc && opt == 'w')
			usage_پیام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		وگرنه اگر (!argc && opt_epts)
			usage_پیام_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		وگرنه اگر (argc == 1)
			obj_name = argv[0];
		وگرنه
			usage_پیام_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} وگرنه اگر (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} وگرنه اگر (argc != 2) {
		usage_پیام_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} وگرنه اگر (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	بازگشت ret;
}
