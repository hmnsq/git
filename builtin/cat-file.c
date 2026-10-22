/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "convert.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "ident.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "userdiff.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "write-or-die.h"
#अन्तर्भाव "alias.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "transport.h"
#अन्तर्भाव "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_ओळ \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

गणना batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

संरचना batch_options {
	संरचना list_objects_filter_options objects_filter;
	पूर्णांक enabled;
	पूर्णांक follow_symlinks;
	गणना batch_mode batch_mode;
	पूर्णांक buffer_output;
	पूर्णांक all_objects;
	पूर्णांक unordered;
	पूर्णांक transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	अक्षर आदान_delim;
	अक्षर output_delim;
	स्थिरांक अक्षर *format;
};

स्थिर स्थिरांक अक्षर *force_path;

स्थिर संरचना string_list mailmap = STRING_LIST_INIT_NODUP;
स्थिर पूर्णांक use_mailmap;

स्थिर अक्षर *replace_idents_using_mailmap(अक्षर *, माप_प्रकार *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
स्थिर शून्य load_mailmap(शून्य)
{
	जर (mailmap.strdup_strings)
		परत;

	read_mailmap(the_repository, &mailmap);
}

स्थिर अक्षर *replace_idents_using_mailmap(अक्षर *object_buf, माप_प्रकार *size)
{
	संरचना strbuf sb = STRBUF_INIT;
	स्थिरांक अक्षर *headers[] = { "author ", "committer ", "tagger ", रिक्त };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	परत strbuf_detach(&sb, रिक्त);
}

स्थिर पूर्णांक filter_object(स्थिरांक अक्षर *path, अचिन्हित mode,
			 स्थिरांक संरचना object_id *oid,
			 अक्षर **buf, माप_प्रकार *size)
{
	गणना object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	जर (!*buf)
		परत error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	जर ((type == OBJ_BLOB) && S_ISREG(mode)) {
		संरचना strbuf strbuf = STRBUF_INIT;
		संरचना checkout_metadata meta;

		init_checkout_metadata(&meta, रिक्त, रिक्त, oid);
		जर (convert_to_working_वृक्ष(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			मुक्त(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, रिक्त);
		}
	}

	परत 0;
}

स्थिर पूर्णांक stream_blob(स्थिरांक संरचना object_id *oid)
{
	जर (odb_stream_blob_to_fd(the_repository->objects, 1, oid, रिक्त, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	परत 0;
}

स्थिर पूर्णांक cat_one_file(पूर्णांक opt, स्थिरांक अक्षर *exp_type, स्थिरांक अक्षर *obj_name)
{
	पूर्णांक ret;
	संरचना object_id oid;
	गणना object_type type;
	अक्षर *buf;
	माप_प्रकार size;
	संरचना object_context obj_context = {0};
	संरचना object_info oi = OBJECT_INFO_INIT;
	अचिन्हित flags = OBJECT_INFO_LOOKUP_REPLACE;
	अचिन्हित get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	स्थिरांक अक्षर *path = force_path;
	स्थिरांक पूर्णांक opt_cw = (opt == 'c' || opt == 'w');
	जर (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	जर (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	जर (!path)
		path = obj_context.path;
	जर (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = रिक्त;
	निवड (opt) {
	स्थिती 't':
		oi.typep = &type;
		जर (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		म_लिखो("%s\n", type_name(type));
		ret = 0;
		जा cleanup;

	स्थिती 's':
		oi.sizep = &size;

		जर (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (शून्य**)&buf;
		}

		जर (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		जर (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		म_लिखो("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		जा cleanup;

	स्थिती 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		जा cleanup;

	स्थिती 'w':

		जर (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			जा cleanup;
		}
		थांबा;

	स्थिती 'c':
	{
		अचिन्हित दीर्घ size_ul = 0;
		पूर्णांक textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		जर (textconv_ret)
			थांबा;
	}
		/* else fallthrough */

	स्थिती 'p':
		type = odb_read_object_info(the_repository->objects, &oid, रिक्त);
		जर (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		जर (type == OBJ_वृक्ष) {
			स्थिरांक अक्षर *ls_args[3] = { रिक्त };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_वृक्ष(2, ls_args, रिक्त, the_repository);
			जा cleanup;
		}

		जर (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			जा cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		जर (!buf)
			die("Cannot read object %s", obj_name);

		जर (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		थांबा;

	स्थिती 0:
	{
		गणना object_type exp_type_id = type_from_string(exp_type);

		जर (exp_type_id == OBJ_BLOB) {
			संरचना object_id blob_oid;
			जर (odb_read_object_info(the_repository->objects,
						 &oid, रिक्त) == OBJ_TAG) {
				अक्षर *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				स्थिरांक अक्षर *target;

				जर (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				जर (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				मुक्त(buffer);
			} अन्यथा
				oidcpy(&blob_oid, &oid);

			जर (odb_read_object_info(the_repository->objects,
						 &blob_oid, रिक्त) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				जा cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, रिक्त);

		जर (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		थांबा;
	}
	पूर्वनिर्धारित:
		die("git cat-file: unknown option: %s", exp_type);
	}

	जर (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	मुक्त(buf);
	object_context_release(&obj_context);
	परत ret;
}

संरचना expand_data {
	संरचना object_id oid;
	गणना object_type type;
	माप_प्रकार size;
	अचिन्हित लघु mode;
	off_t disk_size;
	स्थिरांक अक्षर *rest;
	संरचना object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	पूर्णांक mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	पूर्णांक split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	संरचना object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	अचिन्हित skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	अचिन्हित is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	संरचना string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

स्थिर पूर्णांक is_atom(स्थिरांक अक्षर *atom, स्थिरांक अक्षर *s, पूर्णांक slen)
{
	पूर्णांक alen = म_माप(atom);
	परत alen == slen && !स_भेद(atom, s, alen);
}

स्थिर पूर्णांक expand_atom(संरचना strbuf *sb, स्थिरांक अक्षर *atom, पूर्णांक len,
		       संरचना expand_data *data)
{
	जर (data->is_remote) {
		माप_प्रकार i;
		साठी (i = 0; i < data->remote_allowed_atoms.nr; i++)
			जर (is_atom(data->remote_allowed_atoms.वस्तुसंच[i].string,
				    atom, len))
				थांबा;
		जर (i == data->remote_allowed_atoms.nr)
			परत 1;
	}

	जर (is_atom("objectname", atom, len)) {
		जर (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} अन्यथा जर (is_atom("objecttype", atom, len)) {
		जर (data->mark_query) {
			data->info.typep = &data->type;
		} अन्यथा {
			स्थिरांक अक्षर *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} अन्यथा जर (is_atom("objectsize", atom, len)) {
		जर (data->mark_query)
			data->info.sizep = &data->size;
		अन्यथा
			strbuf_add_uint(sb, data->size);
	} अन्यथा जर (is_atom("objectsize:disk", atom, len)) {
		जर (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		अन्यथा
			strbuf_add_uint(sb, data->disk_size);
	} अन्यथा जर (is_atom("rest", atom, len)) {
		जर (data->mark_query)
			data->split_on_whitespace = 1;
		अन्यथा जर (data->rest)
			strbuf_addstr(sb, data->rest);
	} अन्यथा जर (is_atom("deltabase", atom, len)) {
		जर (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		अन्यथा
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} अन्यथा जर (is_atom("objectmode", atom, len)) {
		जर (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} अन्यथा
		परत 0;
	परत 1;
}

स्थिर शून्य expand_format(संरचना strbuf *sb, स्थिरांक अक्षर *start,
			  संरचना expand_data *data)
{
	पर्यंत (strbuf_expand_step(sb, &start)) {
		स्थिरांक अक्षर *end;

		जर (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		अन्यथा जर ((end = म_अक्षर(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		अन्यथा
			strbuf_expand_bad_format(start, "cat-file");
	}
}

स्थिर शून्य batch_write(संरचना batch_options *opt, स्थिरांक शून्य *data, पूर्णांक len)
{
	जर (opt->buffer_output) {
		जर (ख_डालो(data, 1, len, मानक_निकास) != len)
			die_errno("unable to write to stdout");
	} अन्यथा
		write_or_die(1, data, len);
}

स्थिर शून्य print_object_or_die(संरचना batch_options *opt, संरचना expand_data *data)
{
	स्थिरांक संरचना object_id *oid = &data->oid;

	प्रतिज्ञा(data->info.typep);

	जर (data->type == OBJ_BLOB) {
		जर (opt->buffer_output)
			ख_साफ(मानक_निकास);
		जर (opt->transform_mode) {
			अक्षर *contents;
			माप_प्रकार size;

			जर (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			जर (opt->transform_mode == 'w') {
				जर (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} अन्यथा जर (opt->transform_mode == 'c') {
				गणना object_type type;
				अचिन्हित दीर्घ size_ul = 0;
				जर (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				अन्यथा
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				जर (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} अन्यथा
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			मुक्त(contents);
		} अन्यथा {
			stream_blob(oid);
		}
	}
	अन्यथा {
		गणना object_type type;
		माप_प्रकार size;
		शून्य *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		जर (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		जर (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		जर (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		जर (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		मुक्त(contents);
	}
}

स्थिर शून्य print_default_format(संरचना strbuf *scratch, संरचना expand_data *data,
				 संरचना batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

स्थिर शून्य report_object_status(संरचना batch_options *opt,
				 स्थिरांक अक्षर *obj_name,
				 स्थिरांक संरचना object_id *oid,
				 स्थिरांक अक्षर *status)
{
	म_लिखो("%s %s%c", obj_name ? obj_name : oid_to_hex(oid),
	       status, opt->output_delim);
	ख_साफ(मानक_निकास);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
स्थिर शून्य batch_object_write(स्थिरांक अक्षर *obj_name,
			       संरचना strbuf *scratch,
			       संरचना batch_options *opt,
			       संरचना expand_data *data,
			       संरचना packed_git *pack,
			       off_t offset)
{
	जर (!data->skip_object_info) {
		पूर्णांक ret;

		जर (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		जर (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		जर (pack)
			ret = packed_object_info(रिक्त, pack, offset, &data->info);
		अन्यथा
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		जर (ret < 0) {
			जर (data->mode == S_IFGITLINK)
				report_object_status(opt, रिक्त, &data->oid, "submodule");
			अन्यथा
				report_object_status(opt, obj_name, &data->oid, "missing");
			परत;
		}

		निवड (opt->objects_filter.choice) {
		स्थिती LOFC_DISABLED:
			थांबा;
		स्थिती LOFC_BLOB_NONE:
			जर (data->type == OBJ_BLOB) {
				जर (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				परत;
			}
			थांबा;
		स्थिती LOFC_BLOB_LIMIT:
			जर (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_मूल्य) {
				जर (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				परत;
			}
			थांबा;
		स्थिती LOFC_OBJECT_TYPE:
			जर (data->type != opt->objects_filter.object_type) {
				जर (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				परत;
			}
			थांबा;
		पूर्वनिर्धारित:
			BUG("unsupported objects filter");
		}

		जर (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			अक्षर *buf = रिक्त;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			जर (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			मुक्त(buf);
		}
	}

	strbuf_reset(scratch);

	जर (!opt->format) {
		print_default_format(scratch, data, opt);
	} अन्यथा {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	जर (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

स्थिर शून्य batch_one_object(स्थिरांक अक्षर *obj_name,
			     संरचना strbuf *scratch,
			     संरचना batch_options *opt,
			     संरचना expand_data *data)
{
	संरचना object_context ctx = {0};
	पूर्णांक flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	गणना get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	जर (result != FOUND) {
		निवड (result) {
		स्थिती MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			थांबा;
		स्थिती SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			थांबा;
		स्थिती DANGLING_SYMLINK:
			म_लिखो("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)म_माप(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			थांबा;
		स्थिती SYMLINK_LOOP:
			म_लिखो("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)म_माप(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			थांबा;
		स्थिती NOT_DIR:
			म_लिखो("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)म_माप(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			थांबा;
		पूर्वनिर्धारित:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			थांबा;
		}
		ख_साफ(मानक_निकास);

		जा out;
	}

	जर (ctx.mode == 0) {
		म_लिखो("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		ख_साफ(मानक_निकास);
		जा out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, रिक्त, 0);

out:
	object_context_release(&ctx);
}

स्थिर पूर्णांक get_remote_info(पूर्णांक argc,
			   स्थिरांक अक्षर **argv,
			   संरचना fetch_object_info_results *results,
			   संरचना oid_array *object_info_oids)
{
	पूर्णांक retval = 0;
	संरचना remote *remote = रिक्त;
	संरचना object_id oid;
	संरचना transport *gtransport;

	remote = remote_get(argv[0]);
	जर (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	साठी (माप_प्रकार i = 1; i < argc; i++) {
		जर (get_oid_hex(argv[i], &oid)) {
			माप_प्रकार len = म_माप(argv[i]);

			जर (len < the_hash_algo->hexsz && len >= 4) {
				माप_प्रकार j;
				साठी (j = 0; j < len; j++)
					जर (!है_षष्ठादशक(argv[i][j]))
						थांबा;
				जर (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (पूर्णांक)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	जर (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, रिक्त);

	जर (!gtransport->smart_options) {
		retval = -1;
		जा cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	परत retval;
}

संरचना object_cb_data {
	संरचना batch_options *opt;
	संरचना expand_data *expand;
	संरचना oidset *seen;
	संरचना strbuf *scratch;
};

स्थिर पूर्णांक batch_object_cb(स्थिरांक संरचना object_id *oid, शून्य *vdata)
{
	संरचना object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(रिक्त, data->scratch, data->opt, data->expand,
			   रिक्त, 0);
	परत 0;
}

स्थिर पूर्णांक collect_object(स्थिरांक संरचना object_id *oid,
			  संरचना packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  शून्य *data)
{
	oid_array_append(data, oid);
	परत 0;
}

स्थिर पूर्णांक batch_unordered_object(स्थिरांक संरचना object_id *oid,
				  संरचना packed_git *pack,
				  off_t offset,
				  शून्य *vdata)
{
	संरचना object_cb_data *data = vdata;

	जर (oidset_घाला(data->seen, oid))
		परत 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(रिक्त, data->scratch, data->opt, data->expand,
			   pack, offset);
	परत 0;
}

प्रकारव्याख्या शून्य (*parse_cmd_fn_t)(संरचना batch_options *, स्थिरांक अक्षर *,
			       संरचना strbuf *, संरचना expand_data *);

संरचना queued_cmd {
	parse_cmd_fn_t fn;
	अक्षर *ओळ;
};

स्थिर शून्य parse_cmd_contents(संरचना batch_options *opt,
			     स्थिरांक अक्षर *ओळ,
			     संरचना strbuf *output,
			     संरचना expand_data *data)
{
	गणना object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(ओळ, output, opt, data);
	data->info.typep = saved_typep;
}

स्थिर शून्य parse_cmd_info(संरचना batch_options *opt,
			   स्थिरांक अक्षर *ओळ,
			   संरचना strbuf *output,
			   संरचना expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(ओळ, output, opt, data);
}

स्थिर शून्य parse_cmd_mailmap(संरचना batch_options *opt UNUSED,
			      स्थिरांक अक्षर *ओळ,
			      संरचना strbuf *output UNUSED,
			      संरचना expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(ओळ);

	जर (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), ओळ);

	जर (use_mailmap)
		load_mailmap();
}

स्थिर शून्य parse_cmd_remote_object_info(संरचना batch_options *opt,
					 स्थिरांक अक्षर *ओळ, संरचना strbuf *output,
					 संरचना expand_data *data)
{
	पूर्णांक मोजणी;
	स्थिरांक अक्षर **argv;
	अक्षर *ओळ_to_split;
	संरचना fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	संरचना oid_array object_info_oids = OID_ARRAY_INIT;

	जर (म_माप(ओळ) >= MAX_REMOTE_OBJ_INFO_ओळ)
		die(_("remote-object-info command too long"));

	ओळ_to_split = xstrdup(ओळ);
	मोजणी = split_cmdline(ओळ_to_split, &argv);
	जर (मोजणी < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(मोजणी));
	जर (मोजणी - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	जर (data->info.sizep)
		results.wants_size = 1;
	जर (data->info.typep)
		results.wants_type = 1;

	जर (get_remote_info(मोजणी, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	जर (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	जर (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	साठी (माप_प्रकार i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		जर (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			सुरूठेवा;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		जर (results.sizes)
			data->size = results.sizes[i];

		जर (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, रिक्त, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	मुक्त(ओळ_to_split);
	मुक्त(argv);
	oid_array_clear(&object_info_oids);
}

स्थिर शून्य dispatch_calls(संरचना batch_options *opt,
		संरचना strbuf *output,
		संरचना expand_data *data,
		संरचना queued_cmd *cmd,
		माप_प्रकार nr)
{
	जर (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	साठी (माप_प्रकार i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].ओळ, output, data);

	ख_साफ(मानक_निकास);
}

स्थिर शून्य free_cmds(संरचना queued_cmd *cmd, माप_प्रकार *nr)
{
	साठी (माप_प्रकार i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].ओळ);

	*nr = 0;
}


स्थिर स्थिरांक संरचना parse_cmd {
	स्थिरांक अक्षर *name;
	parse_cmd_fn_t fn;
	अचिन्हित takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", रिक्त, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

स्थिर शून्य batch_objects_command(संरचना batch_options *opt,
				    संरचना strbuf *output,
				    संरचना expand_data *data)
{
	संरचना strbuf आदान = STRBUF_INIT;
	संरचना queued_cmd *queued_cmd = रिक्त;
	माप_प्रकार alloc = 0, nr = 0;

	पर्यंत (strbuf_getdelim_strip_crlf(&आदान, मानक_निवेश, opt->आदान_delim) != खातापूर्ण) {
		स्थिरांक संरचना parse_cmd *cmd = रिक्त;
		स्थिरांक अक्षर *p = रिक्त, *cmd_end;
		संरचना queued_cmd call = {0};

		जर (!आदान.len)
			die(_("empty command in input"));
		जर (है_खाली(*आदान.buf))
			die(_("whitespace before command: '%s'"), आदान.buf);

		साठी (माप_प्रकार i = 0; i < ARRAY_SIZE(commands); i++) {
			जर (!skip_prefix(आदान.buf, commands[i].name, &cmd_end))
				सुरूठेवा;

			cmd = &commands[i];
			जर (cmd->takes_args) {
				जर (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} अन्यथा जर (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			थांबा;
		}

		जर (!cmd)
			die(_("unknown command: '%s'"), आदान.buf);

		जर (!म_भेद(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} अन्यथा जर (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} अन्यथा {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.ओळ = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	जर (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	मुक्त(queued_cmd);
	strbuf_release(&आदान);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

प्रकारव्याख्या पूर्णांक (*for_each_object_fn)(स्थिरांक संरचना object_id *oid, संरचना packed_git *pack,
				  off_t offset, शून्य *data);

संरचना for_each_object_payload {
	for_each_object_fn callback;
	शून्य *payload;
};

स्थिर पूर्णांक batch_one_object_oi(स्थिरांक संरचना object_id *oid,
			       संरचना object_info *oi,
			       शून्य *_payload)
{
	संरचना for_each_object_payload *payload = _payload;
	जर (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		परत payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	परत payload->callback(oid, रिक्त, 0, payload->payload);
}

स्थिर शून्य batch_each_object(संरचना batch_options *opt,
			      for_each_object_fn callback,
			      अचिन्हित flags,
			      शून्य *_payload)
{
	संरचना for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	संरचना odb_source_info source_info;
	संरचना object_info oi = {
		.source_infop = &source_info,
	};
	संरचना odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

स्थिर पूर्णांक batch_objects(संरचना batch_options *opt)
{
	संरचना strbuf आदान = STRBUF_INIT;
	संरचना strbuf output = STRBUF_INIT;
	संरचना expand_data data = EXPAND_DATA_INIT;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);
	पूर्णांक save_warning;
	पूर्णांक retval = 0;

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
	जर (opt->transform_mode)
		data.split_on_whitespace = 1;

	जर (opt->format && !म_भेद(opt->format, DEFAULT_FORMAT))
		opt->format = रिक्त;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	जर (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	जर (opt->all_objects) {
		संरचना object_cb_data cb;
		संरचना object_info empty = OBJECT_INFO_INIT;

		जर (!स_भेद(&data.info, &empty, आकार(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		जर (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		जर (opt->unordered) {
			संरचना oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} अन्यथा {
			संरचना oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		परत 0;
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

	जर (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		जा cleanup;
	}

	पर्यंत (strbuf_getdelim_strip_crlf(&आदान, मानक_निवेश, opt->आदान_delim) != खातापूर्ण) {
		जर (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			अक्षर *p = strpbrk(आदान.buf, " \t");
			जर (p) {
				पर्यंत (*p && म_अक्षर(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(आदान.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&आदान);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	परत retval;
}

स्थिर पूर्णांक git_cat_file_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			       स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	जर (userdiff_config(var, मूल्य) < 0)
		परत -1;

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर पूर्णांक batch_option_callback(स्थिरांक संरचना option *opt,
				 स्थिरांक अक्षर *arg,
				 पूर्णांक unset)
{
	संरचना batch_options *bo = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	जर (bo->enabled) {
		परत error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	जर (!म_भेद(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	अन्यथा जर (!म_भेद(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	अन्यथा जर (!म_भेद(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	अन्यथा
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	परत 0;
}

पूर्णांक cmd_cat_file(पूर्णांक argc,
		 स्थिरांक अक्षर **argv,
		 स्थिरांक अक्षर *prefix,
		 संरचना repository *repo UNUSED)
{
	पूर्णांक opt = 0;
	पूर्णांक opt_cw = 0;
	पूर्णांक opt_epts = 0;
	स्थिरांक अक्षर *exp_type = रिक्त, *obj_name = रिक्त;
	संरचना batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	पूर्णांक unknown_type = 0;
	पूर्णांक आदान_nul_terminated = 0;
	पूर्णांक nul_terminated = 0;
	पूर्णांक ret;

	स्थिरांक अक्षर * स्थिरांक builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		रिक्त
	};
	स्थिरांक संरचना option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', रिक्त, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', रिक्त, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', रिक्त, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', रिक्त, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', रिक्त, &आदान_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', रिक्त, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, रिक्त);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	जर (use_mailmap)
		load_mailmap();

	निवड (batch.objects_filter.choice) {
	स्थिती LOFC_DISABLED:
		थांबा;
	स्थिती LOFC_BLOB_NONE:
	स्थिती LOFC_BLOB_LIMIT:
	स्थिती LOFC_OBJECT_TYPE:
		जर (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		थांबा;
	पूर्वनिर्धारित:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	जर (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	जर (force_path && !opt_cw)
		usage_संदेश_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	जर (batch.enabled)
		;
	अन्यथा जर (batch.follow_symlinks)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	अन्यथा जर (batch.buffer_output >= 0)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	अन्यथा जर (batch.all_objects)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	अन्यथा जर (आदान_nul_terminated)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	अन्यथा जर (nul_terminated)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.आदान_delim = batch.output_delim = '\n';
	जर (आदान_nul_terminated)
		batch.आदान_delim = '\0';
	जर (nul_terminated)
		batch.आदान_delim = batch.output_delim = '\0';

	/* Batch defaults */
	जर (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	जर (batch.enabled) {
		जर (opt_cw)
			batch.transform_mode = opt;
		अन्यथा जर (opt && opt != 'b')
			usage_संदेश_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		अन्यथा जर (argc)
			usage_संदेश_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		जा out;
	}

	जर (opt) {
		जर (!argc && opt == 'c')
			usage_संदेश_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		अन्यथा जर (!argc && opt == 'w')
			usage_संदेश_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		अन्यथा जर (!argc && opt_epts)
			usage_संदेश_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		अन्यथा जर (argc == 1)
			obj_name = argv[0];
		अन्यथा
			usage_संदेश_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} अन्यथा जर (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} अन्यथा जर (argc != 2) {
		usage_संदेश_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} अन्यथा जर (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	परत ret;
}
