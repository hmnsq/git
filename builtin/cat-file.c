/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "convert.h"
#समावेश "diff.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "ident.h"
#समावेश "list-objects-filter-options.h"
#समावेश "parse-options.h"
#समावेश "userdiff.h"
#समावेश "oid-array.h"
#समावेश "packfile.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "odb/streaming.h"
#समावेश "replace-object.h"
#समावेश "promisor-remote.h"
#समावेश "mailmap.h"
#समावेश "write-or-die.h"
#समावेश "alias.h"
#समावेश "remote.h"
#समावेश "transport.h"
#समावेश "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#घोषणा MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#घोषणा MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#घोषणा MAX_REMOTE_OBJ_INFO_पंक्ति \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

क्रमागत batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

काष्ठा batch_options {
	काष्ठा list_objects_filter_options objects_filter;
	पूर्णांक enabled;
	पूर्णांक follow_symlinks;
	क्रमागत batch_mode batch_mode;
	पूर्णांक बफर_output;
	पूर्णांक all_objects;
	पूर्णांक unordered;
	पूर्णांक transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	अक्षर निवेश_delim;
	अक्षर output_delim;
	स्थिर अक्षर *format;
};

अटल स्थिर अक्षर *force_path;

अटल काष्ठा string_list mailmap = STRING_LIST_INIT_NODUP;
अटल पूर्णांक use_mailmap;

अटल अक्षर *replace_idents_using_mailmap(अक्षर *, माप_प्रकार *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
अटल व्योम load_mailmap(व्योम)
{
	अगर (mailmap.strdup_strings)
		वापस;

	read_mailmap(the_repository, &mailmap);
}

अटल अक्षर *replace_idents_using_mailmap(अक्षर *object_buf, माप_प्रकार *आकार)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *headers[] = { "author ", "committer ", "tagger ", शून्य };

	strbuf_attach(&sb, object_buf, *आकार, *आकार + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*आकार = sb.len;
	वापस strbuf_detach(&sb, शून्य);
}

अटल पूर्णांक filter_object(स्थिर अक्षर *path, अचिन्हित mode,
			 स्थिर काष्ठा object_id *oid,
			 अक्षर **buf, माप_प्रकार *आकार)
{
	क्रमागत object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, आकार);
	अगर (!*buf)
		वापस error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	अगर ((type == OBJ_BLOB) && S_ISREG(mode)) {
		काष्ठा strbuf strbuf = STRBUF_INIT;
		काष्ठा checkout_metadata meta;

		init_checkout_metadata(&meta, शून्य, शून्य, oid);
		अगर (convert_to_working_वृक्ष(the_repository->सूचकांक, path, *buf, *आकार, &strbuf, &meta)) {
			मुक्त(*buf);
			*आकार = strbuf.len;
			*buf = strbuf_detach(&strbuf, शून्य);
		}
	}

	वापस 0;
}

अटल पूर्णांक stream_blob(स्थिर काष्ठा object_id *oid)
{
	अगर (odb_stream_blob_to_fd(the_repository->objects, 1, oid, शून्य, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	वापस 0;
}

अटल पूर्णांक cat_one_file(पूर्णांक opt, स्थिर अक्षर *exp_type, स्थिर अक्षर *obj_name)
{
	पूर्णांक ret;
	काष्ठा object_id oid;
	क्रमागत object_type type;
	अक्षर *buf;
	माप_प्रकार आकार;
	काष्ठा object_context obj_context = {0};
	काष्ठा object_info oi = OBJECT_INFO_INIT;
	अचिन्हित flags = OBJECT_INFO_LOOKUP_REPLACE;
	अचिन्हित get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	स्थिर अक्षर *path = force_path;
	स्थिर पूर्णांक opt_cw = (opt == 'c' || opt == 'w');
	अगर (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	अगर (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	अगर (!path)
		path = obj_context.path;
	अगर (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = शून्य;
	चयन (opt) {
	हाल 't':
		oi.typep = &type;
		अगर (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		म_लिखो("%s\n", type_name(type));
		ret = 0;
		जाओ cleanup;

	हाल 's':
		oi.sizep = &आकार;

		अगर (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (व्योम**)&buf;
		}

		अगर (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		अगर (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &आकार);

		म_लिखो("%"PRIuMAX"\n", (uintmax_t)आकार);
		ret = 0;
		जाओ cleanup;

	हाल 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		जाओ cleanup;

	हाल 'w':

		अगर (filter_object(path, obj_context.mode,
				  &oid, &buf, &आकार)) {
			ret = -1;
			जाओ cleanup;
		}
		अवरोध;

	हाल 'c':
	{
		अचिन्हित दीर्घ आकार_ul = 0;
		पूर्णांक textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &आकार_ul);
		आकार = आकार_ul;
		अगर (textconv_ret)
			अवरोध;
	}
		/* else fallthrough */

	हाल 'p':
		type = odb_read_object_info(the_repository->objects, &oid, शून्य);
		अगर (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		अगर (type == OBJ_वृक्ष) {
			स्थिर अक्षर *ls_args[3] = { शून्य };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_वृक्ष(2, ls_args, शून्य, the_repository);
			जाओ cleanup;
		}

		अगर (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			जाओ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &आकार);
		अगर (!buf)
			die("Cannot read object %s", obj_name);

		अगर (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &आकार);

		/* otherwise just spit out the data */
		अवरोध;

	हाल 0:
	{
		क्रमागत object_type exp_type_id = type_from_string(exp_type);

		अगर (exp_type_id == OBJ_BLOB) {
			काष्ठा object_id blob_oid;
			अगर (odb_read_object_info(the_repository->objects,
						 &oid, शून्य) == OBJ_TAG) {
				अक्षर *बफर = odb_read_object(the_repository->objects,
							       &oid, &type, &आकार);
				स्थिर अक्षर *target;

				अगर (!बफर)
					die(_("unable to read %s"), oid_to_hex(&oid));

				अगर (!skip_prefix(बफर, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				मुक्त(बफर);
			} अन्यथा
				oidcpy(&blob_oid, &oid);

			अगर (odb_read_object_info(the_repository->objects,
						 &blob_oid, शून्य) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				जाओ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &आकार, शून्य);

		अगर (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &आकार);
		अवरोध;
	}
	शेष:
		die("git cat-file: unknown option: %s", exp_type);
	}

	अगर (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, आकार);
	ret = 0;
cleanup:
	मुक्त(buf);
	object_context_release(&obj_context);
	वापस ret;
}

काष्ठा expand_तथ्य {
	काष्ठा object_id oid;
	क्रमागत object_type type;
	माप_प्रकार आकार;
	अचिन्हित लघु mode;
	off_t disk_आकार;
	स्थिर अक्षर *rest;
	काष्ठा object_id delta_base_oid;

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
	काष्ठा object_info info;

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
	काष्ठा string_list remote_allowed_atoms;
};

#घोषणा EXPAND_तथ्य_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

अटल पूर्णांक is_atom(स्थिर अक्षर *atom, स्थिर अक्षर *s, पूर्णांक slen)
{
	पूर्णांक alen = म_माप(atom);
	वापस alen == slen && !स_भेद(atom, s, alen);
}

अटल पूर्णांक expand_atom(काष्ठा strbuf *sb, स्थिर अक्षर *atom, पूर्णांक len,
		       काष्ठा expand_तथ्य *तथ्य)
{
	अगर (तथ्य->is_remote) {
		माप_प्रकार i;
		क्रम (i = 0; i < तथ्य->remote_allowed_atoms.nr; i++)
			अगर (is_atom(तथ्य->remote_allowed_atoms.वस्तुएँ[i].string,
				    atom, len))
				अवरोध;
		अगर (i == तथ्य->remote_allowed_atoms.nr)
			वापस 1;
	}

	अगर (is_atom("objectname", atom, len)) {
		अगर (!तथ्य->mark_query)
			strbuf_add_oid_hex(sb, &तथ्य->oid);
	} अन्यथा अगर (is_atom("objecttype", atom, len)) {
		अगर (तथ्य->mark_query) {
			तथ्य->info.typep = &तथ्य->type;
		} अन्यथा {
			स्थिर अक्षर *t = type_name(तथ्य->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} अन्यथा अगर (is_atom("objectsize", atom, len)) {
		अगर (तथ्य->mark_query)
			तथ्य->info.sizep = &तथ्य->आकार;
		अन्यथा
			strbuf_add_uint(sb, तथ्य->आकार);
	} अन्यथा अगर (is_atom("objectsize:disk", atom, len)) {
		अगर (तथ्य->mark_query)
			तथ्य->info.disk_sizep = &तथ्य->disk_आकार;
		अन्यथा
			strbuf_add_uint(sb, तथ्य->disk_आकार);
	} अन्यथा अगर (is_atom("rest", atom, len)) {
		अगर (तथ्य->mark_query)
			तथ्य->split_on_whitespace = 1;
		अन्यथा अगर (तथ्य->rest)
			strbuf_addstr(sb, तथ्य->rest);
	} अन्यथा अगर (is_atom("deltabase", atom, len)) {
		अगर (तथ्य->mark_query)
			तथ्य->info.delta_base_oid = &तथ्य->delta_base_oid;
		अन्यथा
			strbuf_add_oid_hex(sb, &तथ्य->delta_base_oid);
	} अन्यथा अगर (is_atom("objectmode", atom, len)) {
		अगर (!तथ्य->mark_query && !(S_IFINVALID == तथ्य->mode))
			strbuf_addf(sb, "%06o", तथ्य->mode);
	} अन्यथा
		वापस 0;
	वापस 1;
}

अटल व्योम expand_format(काष्ठा strbuf *sb, स्थिर अक्षर *start,
			  काष्ठा expand_तथ्य *तथ्य)
{
	जबतक (strbuf_expand_step(sb, &start)) {
		स्थिर अक्षर *end;

		अगर (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		अन्यथा अगर ((end = म_अक्षर(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, तथ्य))
			start = end + 1;
		अन्यथा
			strbuf_expand_bad_format(start, "cat-file");
	}
}

अटल व्योम batch_write(काष्ठा batch_options *opt, स्थिर व्योम *तथ्य, पूर्णांक len)
{
	अगर (opt->बफर_output) {
		अगर (ख_डालो(तथ्य, 1, len, मानक_निकास) != len)
			die_errno("unable to write to stdout");
	} अन्यथा
		write_or_die(1, तथ्य, len);
}

अटल व्योम print_object_or_die(काष्ठा batch_options *opt, काष्ठा expand_तथ्य *तथ्य)
{
	स्थिर काष्ठा object_id *oid = &तथ्य->oid;

	निश्चित(तथ्य->info.typep);

	अगर (तथ्य->type == OBJ_BLOB) {
		अगर (opt->बफर_output)
			ख_साफ(मानक_निकास);
		अगर (opt->transform_mode) {
			अक्षर *contents;
			माप_प्रकार आकार;

			अगर (!तथ्य->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			अगर (opt->transform_mode == 'w') {
				अगर (filter_object(तथ्य->rest, 0100644, oid,
						  &contents, &आकार))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), तथ्य->rest);
			} अन्यथा अगर (opt->transform_mode == 'c') {
				क्रमागत object_type type;
				अचिन्हित दीर्घ आकार_ul = 0;
				अगर (textconv_object(the_repository,
						    तथ्य->rest, 0100644, oid,
						    1, &contents, &आकार_ul))
					आकार = आकार_ul;
				अन्यथा
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &आकार);
				अगर (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), तथ्य->rest);
			} अन्यथा
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, आकार);
			मुक्त(contents);
		} अन्यथा {
			stream_blob(oid);
		}
	}
	अन्यथा {
		क्रमागत object_type type;
		माप_प्रकार आकार;
		व्योम *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &आकार);
		अगर (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		अगर (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &आकार);

		अगर (type != तथ्य->type)
			die("object %s changed type!?", oid_to_hex(oid));
		अगर (तथ्य->info.sizep && आकार != तथ्य->आकार && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, आकार);
		मुक्त(contents);
	}
}

अटल व्योम print_default_format(काष्ठा strbuf *scratch, काष्ठा expand_तथ्य *तथ्य,
				 काष्ठा batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &तथ्य->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(तथ्य->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, तथ्य->आकार);
	strbuf_addch(scratch, opt->output_delim);
}

अटल व्योम report_object_status(काष्ठा batch_options *opt,
				 स्थिर अक्षर *obj_name,
				 स्थिर काष्ठा object_id *oid,
				 स्थिर अक्षर *status)
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
अटल व्योम batch_object_write(स्थिर अक्षर *obj_name,
			       काष्ठा strbuf *scratch,
			       काष्ठा batch_options *opt,
			       काष्ठा expand_तथ्य *तथ्य,
			       काष्ठा packed_git *pack,
			       off_t offset)
{
	अगर (!तथ्य->skip_object_info) {
		पूर्णांक ret;

		अगर (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			तथ्य->info.typep = &तथ्य->type;
		अगर (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			तथ्य->info.sizep = &तथ्य->आकार;

		अगर (pack)
			ret = packed_object_info(शून्य, pack, offset, &तथ्य->info);
		अन्यथा
			ret = odb_read_object_info_extended(the_repository->objects,
							    &तथ्य->oid, &तथ्य->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		अगर (ret < 0) {
			अगर (तथ्य->mode == S_IFGITLINK)
				report_object_status(opt, शून्य, &तथ्य->oid, "submodule");
			अन्यथा
				report_object_status(opt, obj_name, &तथ्य->oid, "missing");
			वापस;
		}

		चयन (opt->objects_filter.choice) {
		हाल LOFC_DISABLED:
			अवरोध;
		हाल LOFC_BLOB_NONE:
			अगर (तथ्य->type == OBJ_BLOB) {
				अगर (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &तथ्य->oid, "excluded");
				वापस;
			}
			अवरोध;
		हाल LOFC_BLOB_LIMIT:
			अगर (तथ्य->type == OBJ_BLOB &&
			    तथ्य->आकार >= opt->objects_filter.blob_limit_मूल्य) {
				अगर (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &तथ्य->oid, "excluded");
				वापस;
			}
			अवरोध;
		हाल LOFC_OBJECT_TYPE:
			अगर (तथ्य->type != opt->objects_filter.object_type) {
				अगर (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &तथ्य->oid, "excluded");
				वापस;
			}
			अवरोध;
		शेष:
			BUG("unsupported objects filter");
		}

		अगर (use_mailmap && (तथ्य->type == OBJ_COMMIT || तथ्य->type == OBJ_TAG)) {
			अक्षर *buf = शून्य;

			buf = odb_read_object(the_repository->objects, &तथ्य->oid,
					      &तथ्य->type, &तथ्य->आकार);
			अगर (!buf)
				die(_("unable to read %s"), oid_to_hex(&तथ्य->oid));
			buf = replace_idents_using_mailmap(buf, &तथ्य->आकार);

			मुक्त(buf);
		}
	}

	strbuf_reset(scratch);

	अगर (!opt->format) {
		print_default_format(scratch, तथ्य, opt);
	} अन्यथा {
		expand_format(scratch, opt->format, तथ्य);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	अगर (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, तथ्य);
		batch_write(opt, &opt->output_delim, 1);
	}
}

अटल व्योम batch_one_object(स्थिर अक्षर *obj_name,
			     काष्ठा strbuf *scratch,
			     काष्ठा batch_options *opt,
			     काष्ठा expand_तथ्य *तथ्य)
{
	काष्ठा object_context ctx = {0};
	पूर्णांक flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	क्रमागत get_oid_परिणाम परिणाम;

	परिणाम = get_oid_with_context(the_repository, obj_name,
				      flags, &तथ्य->oid, &ctx);
	अगर (परिणाम != FOUND) {
		चयन (परिणाम) {
		हाल MISSING_OBJECT:
			report_object_status(opt, obj_name, &तथ्य->oid, "missing");
			अवरोध;
		हाल SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &तथ्य->oid, "ambiguous");
			अवरोध;
		हाल DANGLING_SYMLINK:
			म_लिखो("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)म_माप(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			अवरोध;
		हाल SYMLINK_LOOP:
			म_लिखो("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)म_माप(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			अवरोध;
		हाल NOT_DIR:
			म_लिखो("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)म_माप(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			अवरोध;
		शेष:
			BUG("unknown get_sha1_with_context result %d\n",
			       परिणाम);
			अवरोध;
		}
		ख_साफ(मानक_निकास);

		जाओ out;
	}

	अगर (ctx.mode == 0) {
		म_लिखो("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		ख_साफ(मानक_निकास);
		जाओ out;
	}

	तथ्य->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, तथ्य, शून्य, 0);

out:
	object_context_release(&ctx);
}

अटल पूर्णांक get_remote_info(पूर्णांक argc,
			   स्थिर अक्षर **argv,
			   काष्ठा fetch_object_info_results *results,
			   काष्ठा oid_array *object_info_oids)
{
	पूर्णांक retval = 0;
	काष्ठा remote *remote = शून्य;
	काष्ठा object_id oid;
	काष्ठा transport *gtransport;

	remote = remote_get(argv[0]);
	अगर (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	क्रम (माप_प्रकार i = 1; i < argc; i++) {
		अगर (get_oid_hex(argv[i], &oid)) {
			माप_प्रकार len = म_माप(argv[i]);

			अगर (len < the_hash_algo->hexsz && len >= 4) {
				माप_प्रकार j;
				क्रम (j = 0; j < len; j++)
					अगर (!है_षष्ठादशक(argv[i][j]))
						अवरोध;
				अगर (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (पूर्णांक)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	अगर (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, शून्य);

	अगर (!gtransport->smart_options) {
		retval = -1;
		जाओ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	वापस retval;
}

काष्ठा object_cb_तथ्य {
	काष्ठा batch_options *opt;
	काष्ठा expand_तथ्य *expand;
	काष्ठा oidset *seen;
	काष्ठा strbuf *scratch;
};

अटल पूर्णांक batch_object_cb(स्थिर काष्ठा object_id *oid, व्योम *vdata)
{
	काष्ठा object_cb_तथ्य *तथ्य = vdata;
	oidcpy(&तथ्य->expand->oid, oid);
	batch_object_write(शून्य, तथ्य->scratch, तथ्य->opt, तथ्य->expand,
			   शून्य, 0);
	वापस 0;
}

अटल पूर्णांक collect_object(स्थिर काष्ठा object_id *oid,
			  काष्ठा packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  व्योम *तथ्य)
{
	oid_array_append(तथ्य, oid);
	वापस 0;
}

अटल पूर्णांक batch_unordered_object(स्थिर काष्ठा object_id *oid,
				  काष्ठा packed_git *pack,
				  off_t offset,
				  व्योम *vdata)
{
	काष्ठा object_cb_तथ्य *तथ्य = vdata;

	अगर (oidset_जोड़ो(तथ्य->seen, oid))
		वापस 0;

	oidcpy(&तथ्य->expand->oid, oid);
	batch_object_write(शून्य, तथ्य->scratch, तथ्य->opt, तथ्य->expand,
			   pack, offset);
	वापस 0;
}

प्रकार व्योम (*parse_cmd_fn_t)(काष्ठा batch_options *, स्थिर अक्षर *,
			       काष्ठा strbuf *, काष्ठा expand_तथ्य *);

काष्ठा queued_cmd {
	parse_cmd_fn_t fn;
	अक्षर *पंक्ति;
};

अटल व्योम parse_cmd_contents(काष्ठा batch_options *opt,
			     स्थिर अक्षर *पंक्ति,
			     काष्ठा strbuf *output,
			     काष्ठा expand_तथ्य *तथ्य)
{
	क्रमागत object_type *saved_typep = तथ्य->info.typep;
	तथ्य->info.typep = &तथ्य->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(पंक्ति, output, opt, तथ्य);
	तथ्य->info.typep = saved_typep;
}

अटल व्योम parse_cmd_info(काष्ठा batch_options *opt,
			   स्थिर अक्षर *पंक्ति,
			   काष्ठा strbuf *output,
			   काष्ठा expand_तथ्य *तथ्य)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(पंक्ति, output, opt, तथ्य);
}

अटल व्योम parse_cmd_mailmap(काष्ठा batch_options *opt UNUSED,
			      स्थिर अक्षर *पंक्ति,
			      काष्ठा strbuf *output UNUSED,
			      काष्ठा expand_तथ्य *तथ्य UNUSED)
{
	use_mailmap = git_parse_maybe_bool(पंक्ति);

	अगर (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), पंक्ति);

	अगर (use_mailmap)
		load_mailmap();
}

अटल व्योम parse_cmd_remote_object_info(काष्ठा batch_options *opt,
					 स्थिर अक्षर *पंक्ति, काष्ठा strbuf *output,
					 काष्ठा expand_तथ्य *तथ्य)
{
	पूर्णांक गिनती;
	स्थिर अक्षर **argv;
	अक्षर *पंक्ति_to_split;
	काष्ठा fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	काष्ठा oid_array object_info_oids = OID_ARRAY_INIT;

	अगर (म_माप(पंक्ति) >= MAX_REMOTE_OBJ_INFO_पंक्ति)
		die(_("remote-object-info command too long"));

	पंक्ति_to_split = xstrdup(पंक्ति);
	गिनती = split_cmdline(पंक्ति_to_split, &argv);
	अगर (गिनती < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(गिनती));
	अगर (गिनती - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	अगर (तथ्य->info.sizep)
		results.wants_आकार = 1;
	अगर (तथ्य->info.typep)
		results.wants_type = 1;

	अगर (get_remote_info(गिनती, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&तथ्य->remote_allowed_atoms, 0);
	string_list_append(&तथ्य->remote_allowed_atoms, "objectname");
	अगर (results.sizes)
		string_list_append(&तथ्य->remote_allowed_atoms, "objectsize");
	अगर (results.types)
		string_list_append(&तथ्य->remote_allowed_atoms, "objecttype");

	तथ्य->skip_object_info = 1;
	क्रम (माप_प्रकार i = 0; i < results.nr; i++) {
		तथ्य->oid = object_info_oids.oid[i];

		अगर (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&तथ्य->oid),
					     &तथ्य->oid, "missing");
			जारी;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		अगर (results.sizes)
			तथ्य->आकार = results.sizes[i];

		अगर (results.types)
			तथ्य->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		तथ्य->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, तथ्य, शून्य, 0);
		तथ्य->is_remote = 0;
	}
	तथ्य->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	मुक्त(पंक्ति_to_split);
	मुक्त(argv);
	oid_array_clear(&object_info_oids);
}

अटल व्योम dispatch_calls(काष्ठा batch_options *opt,
		काष्ठा strbuf *output,
		काष्ठा expand_तथ्य *तथ्य,
		काष्ठा queued_cmd *cmd,
		माप_प्रकार nr)
{
	अगर (!opt->बफर_output)
		die(_("flush is only for --buffer mode"));

	क्रम (माप_प्रकार i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].पंक्ति, output, तथ्य);

	ख_साफ(मानक_निकास);
}

अटल व्योम free_cmds(काष्ठा queued_cmd *cmd, माप_प्रकार *nr)
{
	क्रम (माप_प्रकार i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].पंक्ति);

	*nr = 0;
}


अटल स्थिर काष्ठा parse_cmd {
	स्थिर अक्षर *name;
	parse_cmd_fn_t fn;
	अचिन्हित takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", शून्य, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

अटल व्योम batch_objects_command(काष्ठा batch_options *opt,
				    काष्ठा strbuf *output,
				    काष्ठा expand_तथ्य *तथ्य)
{
	काष्ठा strbuf निवेश = STRBUF_INIT;
	काष्ठा queued_cmd *queued_cmd = शून्य;
	माप_प्रकार alloc = 0, nr = 0;

	जबतक (strbuf_getdelim_strip_crlf(&निवेश, मानक_निवेश, opt->निवेश_delim) != खातापूर्ण) {
		स्थिर काष्ठा parse_cmd *cmd = शून्य;
		स्थिर अक्षर *p = शून्य, *cmd_end;
		काष्ठा queued_cmd call = {0};

		अगर (!निवेश.len)
			die(_("empty command in input"));
		अगर (है_खाली(*निवेश.buf))
			die(_("whitespace before command: '%s'"), निवेश.buf);

		क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(commands); i++) {
			अगर (!skip_prefix(निवेश.buf, commands[i].name, &cmd_end))
				जारी;

			cmd = &commands[i];
			अगर (cmd->takes_args) {
				अगर (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} अन्यथा अगर (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			अवरोध;
		}

		अगर (!cmd)
			die(_("unknown command: '%s'"), निवेश.buf);

		अगर (!म_भेद(cmd->name, "flush")) {
			dispatch_calls(opt, output, तथ्य, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} अन्यथा अगर (!opt->बफर_output) {
			cmd->fn(opt, p, output, तथ्य);
		} अन्यथा {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.पंक्ति = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	अगर (opt->बफर_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, तथ्य, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	मुक्त(queued_cmd);
	strbuf_release(&निवेश);
}

#घोषणा DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

प्रकार पूर्णांक (*for_each_object_fn)(स्थिर काष्ठा object_id *oid, काष्ठा packed_git *pack,
				  off_t offset, व्योम *तथ्य);

काष्ठा for_each_object_payload {
	for_each_object_fn callback;
	व्योम *payload;
};

अटल पूर्णांक batch_one_object_oi(स्थिर काष्ठा object_id *oid,
			       काष्ठा object_info *oi,
			       व्योम *_payload)
{
	काष्ठा for_each_object_payload *payload = _payload;
	अगर (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		वापस payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	वापस payload->callback(oid, शून्य, 0, payload->payload);
}

अटल व्योम batch_each_object(काष्ठा batch_options *opt,
			      for_each_object_fn callback,
			      अचिन्हित flags,
			      व्योम *_payload)
{
	काष्ठा for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	काष्ठा odb_source_info source_info;
	काष्ठा object_info oi = {
		.source_infop = &source_info,
	};
	काष्ठा odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

अटल पूर्णांक batch_objects(काष्ठा batch_options *opt)
{
	काष्ठा strbuf निवेश = STRBUF_INIT;
	काष्ठा strbuf output = STRBUF_INIT;
	काष्ठा expand_तथ्य तथ्य = EXPAND_तथ्य_INIT;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णांक save_warning;
	पूर्णांक retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	तथ्य.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &तथ्य);
	तथ्य.mark_query = 0;
	strbuf_release(&output);
	अगर (opt->transform_mode)
		तथ्य.split_on_whitespace = 1;

	अगर (opt->format && !म_भेद(opt->format, DEFAULT_FORMAT))
		opt->format = शून्य;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	अगर (opt->batch_mode == BATCH_MODE_CONTENTS)
		तथ्य.info.typep = &तथ्य.type;

	अगर (opt->all_objects) {
		काष्ठा object_cb_तथ्य cb;
		काष्ठा object_info empty = OBJECT_INFO_INIT;

		अगर (!स_भेद(&तथ्य.info, &empty, माप(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			तथ्य.skip_object_info = 1;

		अगर (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &तथ्य;
		cb.scratch = &output;

		अगर (opt->unordered) {
			काष्ठा oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} अन्यथा {
			काष्ठा oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		वापस 0;
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

	अगर (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &तथ्य);
		जाओ cleanup;
	}

	जबतक (strbuf_getdelim_strip_crlf(&निवेश, मानक_निवेश, opt->निवेश_delim) != खातापूर्ण) {
		अगर (तथ्य.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			अक्षर *p = strpbrk(निवेश.buf, " \t");
			अगर (p) {
				जबतक (*p && म_अक्षर(" \t", *p))
					*p++ = '\0';
			}
			तथ्य.rest = p;
		}

		batch_one_object(निवेश.buf, &output, opt, &तथ्य);
	}

 cleanup:
	strbuf_release(&निवेश);
	strbuf_release(&output);
	string_list_clear(&तथ्य.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	वापस retval;
}

अटल पूर्णांक git_cat_file_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			       स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (userdiff_config(var, मूल्य) < 0)
		वापस -1;

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल पूर्णांक batch_option_callback(स्थिर काष्ठा option *opt,
				 स्थिर अक्षर *arg,
				 पूर्णांक unset)
{
	काष्ठा batch_options *bo = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (bo->enabled) {
		वापस error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	अगर (!म_भेद(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	अन्यथा अगर (!म_भेद(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	अन्यथा अगर (!म_भेद(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	अन्यथा
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	वापस 0;
}

पूर्णांक cmd_cat_file(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	पूर्णांक opt = 0;
	पूर्णांक opt_cw = 0;
	पूर्णांक opt_epts = 0;
	स्थिर अक्षर *exp_type = शून्य, *obj_name = शून्य;
	काष्ठा batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	पूर्णांक unknown_type = 0;
	पूर्णांक निवेश_nul_terminated = 0;
	पूर्णांक nul_terminated = 0;
	पूर्णांक ret;

	स्थिर अक्षर * स्थिर builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		शून्य
	};
	स्थिर काष्ठा option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', शून्य, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', शून्य, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', शून्य, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', शून्य, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', शून्य, &निवेश_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', शून्य, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_GROUP(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.बफर_output, N_("buffer --batch output")),
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

	repo_config(the_repository, git_cat_file_config, शून्य);

	batch.बफर_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	अगर (use_mailmap)
		load_mailmap();

	चयन (batch.objects_filter.choice) {
	हाल LOFC_DISABLED:
		अवरोध;
	हाल LOFC_BLOB_NONE:
	हाल LOFC_BLOB_LIMIT:
	हाल LOFC_OBJECT_TYPE:
		अगर (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		अवरोध;
	शेष:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	अगर (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	अगर (force_path && !opt_cw)
		usage_संदेश_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	अगर (batch.enabled)
		;
	अन्यथा अगर (batch.follow_symlinks)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	अन्यथा अगर (batch.बफर_output >= 0)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	अन्यथा अगर (batch.all_objects)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	अन्यथा अगर (निवेश_nul_terminated)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	अन्यथा अगर (nul_terminated)
		usage_संदेश_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.निवेश_delim = batch.output_delim = '\n';
	अगर (निवेश_nul_terminated)
		batch.निवेश_delim = '\0';
	अगर (nul_terminated)
		batch.निवेश_delim = batch.output_delim = '\0';

	/* Batch defaults */
	अगर (batch.बफर_output < 0)
		batch.बफर_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	/* Return early if we're in batch mode? */
	अगर (batch.enabled) {
		अगर (opt_cw)
			batch.transform_mode = opt;
		अन्यथा अगर (opt && opt != 'b')
			usage_संदेश_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		अन्यथा अगर (argc)
			usage_संदेश_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		जाओ out;
	}

	अगर (opt) {
		अगर (!argc && opt == 'c')
			usage_संदेश_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		अन्यथा अगर (!argc && opt == 'w')
			usage_संदेश_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		अन्यथा अगर (!argc && opt_epts)
			usage_संदेश_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		अन्यथा अगर (argc == 1)
			obj_name = argv[0];
		अन्यथा
			usage_संदेश_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} अन्यथा अगर (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} अन्यथा अगर (argc != 2) {
		usage_संदेश_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} अन्यथा अगर (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	वापस ret;
}
