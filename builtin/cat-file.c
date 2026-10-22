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
#define MAX_REMOTE_OBJ_INFO_पङ्क्तिः \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

गणना batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

संरचना batch_options {
	संरचना list_objects_filter_options objects_filter;
	पूर्णाङ्क enabled;
	पूर्णाङ्क follow_symlinks;
	गणना batch_mode batch_mode;
	पूर्णाङ्क buffer_output;
	पूर्णाङ्क all_objects;
	पूर्णाङ्क unordered;
	पूर्णाङ्क transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	वर्ण निवेशः_delim;
	वर्ण output_delim;
	ध्रुव वर्ण *format;
};

स्थिर ध्रुव वर्ण *force_path;

स्थिर संरचना string_list mailmap = STRING_LIST_INIT_NODUP;
स्थिर पूर्णाङ्क use_mailmap;

स्थिर वर्ण *replace_idents_using_mailmap(वर्ण *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
स्थिर शून्य load_mailmap(शून्य)
{
	सचे (mailmap.strdup_strings)
		पटिनिवत्त;

	read_mailmap(the_repository, &mailmap);
}

स्थिर वर्ण *replace_idents_using_mailmap(वर्ण *object_buf, size_t *size)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *headers[] = { "author ", "committer ", "tagger ", रिक्त };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	पटिनिवत्त strbuf_detach(&sb, रिक्त);
}

स्थिर पूर्णाङ्क filter_object(ध्रुव वर्ण *path, unsigned mode,
			 ध्रुव संरचना object_id *oid,
			 वर्ण **buf, size_t *size)
{
	गणना object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	सचे (!*buf)
		पटिनिवत्त error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	सचे ((type == OBJ_BLOB) && S_ISREG(mode)) {
		संरचना strbuf strbuf = STRBUF_INIT;
		संरचना checkout_metadata meta;

		init_checkout_metadata(&meta, रिक्त, रिक्त, oid);
		सचे (convert_to_working_वृक्षः(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			मुक्त(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, रिक्त);
		}
	}

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क stream_blob(ध्रुव संरचना object_id *oid)
{
	सचे (odb_stream_blob_to_fd(the_repository->objects, 1, oid, रिक्त, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क cat_one_file(पूर्णाङ्क opt, ध्रुव वर्ण *exp_type, ध्रुव वर्ण *obj_name)
{
	पूर्णाङ्क ret;
	संरचना object_id oid;
	गणना object_type type;
	वर्ण *buf;
	size_t size;
	संरचना object_context obj_context = {0};
	संरचना object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	ध्रुव वर्ण *path = force_path;
	ध्रुव पूर्णाङ्क opt_cw = (opt == 'c' || opt == 'w');
	सचे (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	सचे (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	सचे (!path)
		path = obj_context.path;
	सचे (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = रिक्त;
	विकल्प (opt) {
	पक्ष 't':
		oi.typep = &type;
		सचे (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		गच्छ cleanup;

	पक्ष 's':
		oi.sizep = &size;

		सचे (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (शून्य**)&buf;
		}

		सचे (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		सचे (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		गच्छ cleanup;

	पक्ष 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		गच्छ cleanup;

	पक्ष 'w':

		सचे (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			गच्छ cleanup;
		}
		भङ्ग;

	पक्ष 'c':
	{
		unsigned long size_ul = 0;
		पूर्णाङ्क textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		सचे (textconv_ret)
			भङ्ग;
	}
		/* else fallthrough */

	पक्ष 'p':
		type = odb_read_object_info(the_repository->objects, &oid, रिक्त);
		सचे (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		सचे (type == OBJ_वृक्षः) {
			ध्रुव वर्ण *ls_args[3] = { रिक्त };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_वृक्षः(2, ls_args, रिक्त, the_repository);
			गच्छ cleanup;
		}

		सचे (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			गच्छ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		सचे (!buf)
			die("Cannot read object %s", obj_name);

		सचे (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		भङ्ग;

	पक्ष 0:
	{
		गणना object_type exp_type_id = type_from_string(exp_type);

		सचे (exp_type_id == OBJ_BLOB) {
			संरचना object_id blob_oid;
			सचे (odb_read_object_info(the_repository->objects,
						 &oid, रिक्त) == OBJ_TAG) {
				वर्ण *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				ध्रुव वर्ण *target;

				सचे (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				सचे (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				मुक्त(buffer);
			} अञ्ञथा
				oidcpy(&blob_oid, &oid);

			सचे (odb_read_object_info(the_repository->objects,
						 &blob_oid, रिक्त) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				गच्छ cleanup;
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

		सचे (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		भङ्ग;
	}
	स्वभाव:
		die("git cat-file: unknown option: %s", exp_type);
	}

	सचे (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	मुक्त(buf);
	object_context_release(&obj_context);
	पटिनिवत्त ret;
}

संरचना expand_data {
	संरचना object_id oid;
	गणना object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	ध्रुव वर्ण *rest;
	संरचना object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	पूर्णाङ्क mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	पूर्णाङ्क split_on_whitespace;

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
	unsigned skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	unsigned is_remote:1;

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

स्थिर पूर्णाङ्क is_atom(ध्रुव वर्ण *atom, ध्रुव वर्ण *s, पूर्णाङ्क slen)
{
	पूर्णाङ्क alen = strlen(atom);
	पटिनिवत्त alen == slen && !memcmp(atom, s, alen);
}

स्थिर पूर्णाङ्क expand_atom(संरचना strbuf *sb, ध्रुव वर्ण *atom, पूर्णाङ्क len,
		       संरचना expand_data *data)
{
	सचे (data->is_remote) {
		size_t i;
		अत्थाय (i = 0; i < data->remote_allowed_atoms.nr; i++)
			सचे (is_atom(data->remote_allowed_atoms.वस्तूनि[i].string,
				    atom, len))
				भङ्ग;
		सचे (i == data->remote_allowed_atoms.nr)
			पटिनिवत्त 1;
	}

	सचे (is_atom("objectname", atom, len)) {
		सचे (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} अञ्ञथा सचे (is_atom("objecttype", atom, len)) {
		सचे (data->mark_query) {
			data->info.typep = &data->type;
		} अञ्ञथा {
			ध्रुव वर्ण *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} अञ्ञथा सचे (is_atom("objectsize", atom, len)) {
		सचे (data->mark_query)
			data->info.sizep = &data->size;
		अञ्ञथा
			strbuf_add_uint(sb, data->size);
	} अञ्ञथा सचे (is_atom("objectsize:disk", atom, len)) {
		सचे (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		अञ्ञथा
			strbuf_add_uint(sb, data->disk_size);
	} अञ्ञथा सचे (is_atom("rest", atom, len)) {
		सचे (data->mark_query)
			data->split_on_whitespace = 1;
		अञ्ञथा सचे (data->rest)
			strbuf_addstr(sb, data->rest);
	} अञ्ञथा सचे (is_atom("deltabase", atom, len)) {
		सचे (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		अञ्ञथा
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} अञ्ञथा सचे (is_atom("objectmode", atom, len)) {
		सचे (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} अञ्ञथा
		पटिनिवत्त 0;
	पटिनिवत्त 1;
}

स्थिर शून्य expand_format(संरचना strbuf *sb, ध्रुव वर्ण *start,
			  संरचना expand_data *data)
{
	याव (strbuf_expand_step(sb, &start)) {
		ध्रुव वर्ण *end;

		सचे (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		अञ्ञथा सचे ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		अञ्ञथा
			strbuf_expand_bad_format(start, "cat-file");
	}
}

स्थिर शून्य batch_write(संरचना batch_options *opt, ध्रुव शून्य *data, पूर्णाङ्क len)
{
	सचे (opt->buffer_output) {
		सचे (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} अञ्ञथा
		write_or_die(1, data, len);
}

स्थिर शून्य print_object_or_die(संरचना batch_options *opt, संरचना expand_data *data)
{
	ध्रुव संरचना object_id *oid = &data->oid;

	प्रतिज्ञा(data->info.typep);

	सचे (data->type == OBJ_BLOB) {
		सचे (opt->buffer_output)
			fflush(stdout);
		सचे (opt->transform_mode) {
			वर्ण *contents;
			size_t size;

			सचे (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			सचे (opt->transform_mode == 'w') {
				सचे (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} अञ्ञथा सचे (opt->transform_mode == 'c') {
				गणना object_type type;
				unsigned long size_ul = 0;
				सचे (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				अञ्ञथा
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				सचे (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} अञ्ञथा
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			मुक्त(contents);
		} अञ्ञथा {
			stream_blob(oid);
		}
	}
	अञ्ञथा {
		गणना object_type type;
		size_t size;
		शून्य *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		सचे (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		सचे (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		सचे (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		सचे (data->info.sizep && size != data->size && !use_mailmap)
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
				 ध्रुव वर्ण *obj_name,
				 ध्रुव संरचना object_id *oid,
				 ध्रुव वर्ण *status)
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
स्थिर शून्य batch_object_write(ध्रुव वर्ण *obj_name,
			       संरचना strbuf *scratch,
			       संरचना batch_options *opt,
			       संरचना expand_data *data,
			       संरचना packed_git *pack,
			       off_t offset)
{
	सचे (!data->skip_object_info) {
		पूर्णाङ्क ret;

		सचे (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		सचे (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		सचे (pack)
			ret = packed_object_info(रिक्त, pack, offset, &data->info);
		अञ्ञथा
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		सचे (ret < 0) {
			सचे (data->mode == S_IFGITLINK)
				report_object_status(opt, रिक्त, &data->oid, "submodule");
			अञ्ञथा
				report_object_status(opt, obj_name, &data->oid, "missing");
			पटिनिवत्त;
		}

		विकल्प (opt->objects_filter.choice) {
		पक्ष LOFC_DISABLED:
			भङ्ग;
		पक्ष LOFC_BLOB_NONE:
			सचे (data->type == OBJ_BLOB) {
				सचे (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				पटिनिवत्त;
			}
			भङ्ग;
		पक्ष LOFC_BLOB_LIMIT:
			सचे (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_मानम्) {
				सचे (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				पटिनिवत्त;
			}
			भङ्ग;
		पक्ष LOFC_OBJECT_TYPE:
			सचे (data->type != opt->objects_filter.object_type) {
				सचे (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				पटिनिवत्त;
			}
			भङ्ग;
		स्वभाव:
			BUG("unsupported objects filter");
		}

		सचे (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			वर्ण *buf = रिक्त;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			सचे (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			मुक्त(buf);
		}
	}

	strbuf_reset(scratch);

	सचे (!opt->format) {
		print_default_format(scratch, data, opt);
	} अञ्ञथा {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	सचे (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

स्थिर शून्य batch_one_object(ध्रुव वर्ण *obj_name,
			     संरचना strbuf *scratch,
			     संरचना batch_options *opt,
			     संरचना expand_data *data)
{
	संरचना object_context ctx = {0};
	पूर्णाङ्क flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	गणना get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	सचे (result != FOUND) {
		विकल्प (result) {
		पक्ष MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			भङ्ग;
		पक्ष SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			भङ्ग;
		पक्ष DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			भङ्ग;
		पक्ष SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			भङ्ग;
		पक्ष NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			भङ्ग;
		स्वभाव:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			भङ्ग;
		}
		fflush(stdout);

		गच्छ out;
	}

	सचे (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		गच्छ out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, रिक्त, 0);

out:
	object_context_release(&ctx);
}

स्थिर पूर्णाङ्क get_remote_info(पूर्णाङ्क argc,
			   ध्रुव वर्ण **argv,
			   संरचना fetch_object_info_results *results,
			   संरचना oid_array *object_info_oids)
{
	पूर्णाङ्क retval = 0;
	संरचना remote *remote = रिक्त;
	संरचना object_id oid;
	संरचना transport *gtransport;

	remote = remote_get(argv[0]);
	सचे (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	अत्थाय (size_t i = 1; i < argc; i++) {
		सचे (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			सचे (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				अत्थाय (j = 0; j < len; j++)
					सचे (!isxdigit(argv[i][j]))
						भङ्ग;
				सचे (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (पूर्णाङ्क)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	सचे (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, रिक्त);

	सचे (!gtransport->smart_options) {
		retval = -1;
		गच्छ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	पटिनिवत्त retval;
}

संरचना object_cb_data {
	संरचना batch_options *opt;
	संरचना expand_data *expand;
	संरचना oidset *seen;
	संरचना strbuf *scratch;
};

स्थिर पूर्णाङ्क batch_object_cb(ध्रुव संरचना object_id *oid, शून्य *vdata)
{
	संरचना object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(रिक्त, data->scratch, data->opt, data->expand,
			   रिक्त, 0);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क collect_object(ध्रुव संरचना object_id *oid,
			  संरचना packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  शून्य *data)
{
	oid_array_append(data, oid);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क batch_unordered_object(ध्रुव संरचना object_id *oid,
				  संरचना packed_git *pack,
				  off_t offset,
				  शून्य *vdata)
{
	संरचना object_cb_data *data = vdata;

	सचे (oidset_निवेशय(data->seen, oid))
		पटिनिवत्त 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(रिक्त, data->scratch, data->opt, data->expand,
			   pack, offset);
	पटिनिवत्त 0;
}

प्रकारनिर्देश शून्य (*parse_cmd_fn_t)(संरचना batch_options *, ध्रुव वर्ण *,
			       संरचना strbuf *, संरचना expand_data *);

संरचना queued_cmd {
	parse_cmd_fn_t fn;
	वर्ण *पङ्क्तिः;
};

स्थिर शून्य parse_cmd_contents(संरचना batch_options *opt,
			     ध्रुव वर्ण *पङ्क्तिः,
			     संरचना strbuf *output,
			     संरचना expand_data *data)
{
	गणना object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(पङ्क्तिः, output, opt, data);
	data->info.typep = saved_typep;
}

स्थिर शून्य parse_cmd_info(संरचना batch_options *opt,
			   ध्रुव वर्ण *पङ्क्तिः,
			   संरचना strbuf *output,
			   संरचना expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(पङ्क्तिः, output, opt, data);
}

स्थिर शून्य parse_cmd_mailmap(संरचना batch_options *opt UNUSED,
			      ध्रुव वर्ण *पङ्क्तिः,
			      संरचना strbuf *output UNUSED,
			      संरचना expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(पङ्क्तिः);

	सचे (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), पङ्क्तिः);

	सचे (use_mailmap)
		load_mailmap();
}

स्थिर शून्य parse_cmd_remote_object_info(संरचना batch_options *opt,
					 ध्रुव वर्ण *पङ्क्तिः, संरचना strbuf *output,
					 संरचना expand_data *data)
{
	पूर्णाङ्क गणनम्;
	ध्रुव वर्ण **argv;
	वर्ण *पङ्क्तिः_to_split;
	संरचना fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	संरचना oid_array object_info_oids = OID_ARRAY_INIT;

	सचे (strlen(पङ्क्तिः) >= MAX_REMOTE_OBJ_INFO_पङ्क्तिः)
		die(_("remote-object-info command too long"));

	पङ्क्तिः_to_split = xstrdup(पङ्क्तिः);
	गणनम् = split_cmdline(पङ्क्तिः_to_split, &argv);
	सचे (गणनम् < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(गणनम्));
	सचे (गणनम् - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	सचे (data->info.sizep)
		results.wants_size = 1;
	सचे (data->info.typep)
		results.wants_type = 1;

	सचे (get_remote_info(गणनम्, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	सचे (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	सचे (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	अत्थाय (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		सचे (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			अनुवर्त;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		सचे (results.sizes)
			data->size = results.sizes[i];

		सचे (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, रिक्त, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	मुक्त(पङ्क्तिः_to_split);
	मुक्त(argv);
	oid_array_clear(&object_info_oids);
}

स्थिर शून्य dispatch_calls(संरचना batch_options *opt,
		संरचना strbuf *output,
		संरचना expand_data *data,
		संरचना queued_cmd *cmd,
		size_t nr)
{
	सचे (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	अत्थाय (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].पङ्क्तिः, output, data);

	fflush(stdout);
}

स्थिर शून्य free_cmds(संरचना queued_cmd *cmd, size_t *nr)
{
	अत्थाय (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].पङ्क्तिः);

	*nr = 0;
}


स्थिर ध्रुव संरचना parse_cmd {
	ध्रुव वर्ण *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
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
	संरचना strbuf निवेशः = STRBUF_INIT;
	संरचना queued_cmd *queued_cmd = रिक्त;
	size_t alloc = 0, nr = 0;

	याव (strbuf_getdelim_strip_crlf(&निवेशः, stdin, opt->निवेशः_delim) != EOF) {
		ध्रुव संरचना parse_cmd *cmd = रिक्त;
		ध्रुव वर्ण *p = रिक्त, *cmd_end;
		संरचना queued_cmd call = {0};

		सचे (!निवेशः.len)
			die(_("empty command in input"));
		सचे (isspace(*निवेशः.buf))
			die(_("whitespace before command: '%s'"), निवेशः.buf);

		अत्थाय (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			सचे (!skip_prefix(निवेशः.buf, commands[i].name, &cmd_end))
				अनुवर्त;

			cmd = &commands[i];
			सचे (cmd->takes_args) {
				सचे (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} अञ्ञथा सचे (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			भङ्ग;
		}

		सचे (!cmd)
			die(_("unknown command: '%s'"), निवेशः.buf);

		सचे (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} अञ्ञथा सचे (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} अञ्ञथा {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.पङ्क्तिः = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	सचे (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	मुक्त(queued_cmd);
	strbuf_release(&निवेशः);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

प्रकारनिर्देश पूर्णाङ्क (*for_each_object_fn)(ध्रुव संरचना object_id *oid, संरचना packed_git *pack,
				  off_t offset, शून्य *data);

संरचना for_each_object_payload {
	for_each_object_fn callback;
	शून्य *payload;
};

स्थिर पूर्णाङ्क batch_one_object_oi(ध्रुव संरचना object_id *oid,
			       संरचना object_info *oi,
			       शून्य *_payload)
{
	संरचना for_each_object_payload *payload = _payload;
	सचे (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		पटिनिवत्त payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	पटिनिवत्त payload->callback(oid, रिक्त, 0, payload->payload);
}

स्थिर शून्य batch_each_object(संरचना batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
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

स्थिर पूर्णाङ्क batch_objects(संरचना batch_options *opt)
{
	संरचना strbuf निवेशः = STRBUF_INIT;
	संरचना strbuf output = STRBUF_INIT;
	संरचना expand_data data = EXPAND_DATA_INIT;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);
	पूर्णाङ्क save_warning;
	पूर्णाङ्क retval = 0;

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
	सचे (opt->transform_mode)
		data.split_on_whitespace = 1;

	सचे (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = रिक्त;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	सचे (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	सचे (opt->all_objects) {
		संरचना object_cb_data cb;
		संरचना object_info empty = OBJECT_INFO_INIT;

		सचे (!memcmp(&data.info, &empty, परिमाण(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		सचे (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		सचे (opt->unordered) {
			संरचना oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} अञ्ञथा {
			संरचना oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		पटिनिवत्त 0;
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

	सचे (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		गच्छ cleanup;
	}

	याव (strbuf_getdelim_strip_crlf(&निवेशः, stdin, opt->निवेशः_delim) != EOF) {
		सचे (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			वर्ण *p = strpbrk(निवेशः.buf, " \t");
			सचे (p) {
				याव (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(निवेशः.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&निवेशः);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	पटिनिवत्त retval;
}

स्थिर पूर्णाङ्क git_cat_file_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			       ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	सचे (userdiff_config(var, मानम्) < 0)
		पटिनिवत्त -1;

	पटिनिवत्त git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क batch_option_callback(ध्रुव संरचना option *opt,
				 ध्रुव वर्ण *arg,
				 पूर्णाङ्क unset)
{
	संरचना batch_options *bo = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	सचे (bo->enabled) {
		पटिनिवत्त error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	सचे (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	अञ्ञथा सचे (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	अञ्ञथा सचे (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	अञ्ञथा
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	पटिनिवत्त 0;
}

पूर्णाङ्क cmd_cat_file(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	पूर्णाङ्क opt = 0;
	पूर्णाङ्क opt_cw = 0;
	पूर्णाङ्क opt_epts = 0;
	ध्रुव वर्ण *exp_type = रिक्त, *obj_name = रिक्त;
	संरचना batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	पूर्णाङ्क unknown_type = 0;
	पूर्णाङ्क निवेशः_nul_terminated = 0;
	पूर्णाङ्क nul_terminated = 0;
	पूर्णाङ्क ret;

	ध्रुव वर्ण * ध्रुव builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		रिक्त
	};
	ध्रुव संरचना option options[] = {
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
		OPT_BOOL_F('z', रिक्त, &निवेशः_nul_terminated, N_("stdin is NUL-terminated"),
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

	सचे (use_mailmap)
		load_mailmap();

	विकल्प (batch.objects_filter.choice) {
	पक्ष LOFC_DISABLED:
		भङ्ग;
	पक्ष LOFC_BLOB_NONE:
	पक्ष LOFC_BLOB_LIMIT:
	पक्ष LOFC_OBJECT_TYPE:
		सचे (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		भङ्ग;
	स्वभाव:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	सचे (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	सचे (force_path && !opt_cw)
		usage_सन्देशः_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	सचे (batch.enabled)
		;
	अञ्ञथा सचे (batch.follow_symlinks)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	अञ्ञथा सचे (batch.buffer_output >= 0)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	अञ्ञथा सचे (batch.all_objects)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	अञ्ञथा सचे (निवेशः_nul_terminated)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	अञ्ञथा सचे (nul_terminated)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.निवेशः_delim = batch.output_delim = '\n';
	सचे (निवेशः_nul_terminated)
		batch.निवेशः_delim = '\0';
	सचे (nul_terminated)
		batch.निवेशः_delim = batch.output_delim = '\0';

	/* Batch defaults */
	सचे (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	सचे (batch.enabled) {
		सचे (opt_cw)
			batch.transform_mode = opt;
		अञ्ञथा सचे (opt && opt != 'b')
			usage_सन्देशः_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		अञ्ञथा सचे (argc)
			usage_सन्देशः_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		गच्छ out;
	}

	सचे (opt) {
		सचे (!argc && opt == 'c')
			usage_सन्देशः_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		अञ्ञथा सचे (!argc && opt == 'w')
			usage_सन्देशः_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		अञ्ञथा सचे (!argc && opt_epts)
			usage_सन्देशः_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		अञ्ञथा सचे (argc == 1)
			obj_name = argv[0];
		अञ्ञथा
			usage_सन्देशः_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} अञ्ञथा सचे (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} अञ्ञथा सचे (argc != 2) {
		usage_सन्देशः_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} अञ्ञथा सचे (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	पटिनिवत्त ret;
}
