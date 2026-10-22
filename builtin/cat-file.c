/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "convert.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "ident.h"
#અન્તર્ભાવ "list-objects-filter-options.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "userdiff.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "packfile.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/streaming.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "promisor-remote.h"
#અન્તર્ભાવ "mailmap.h"
#અન્તર્ભાવ "write-or-die.h"
#અન્તર્ભાવ "alias.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_લીટી \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

ગણના batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

સંરચના batch_options {
	સંરચના list_objects_filter_options objects_filter;
	પૂર્ણાંક enabled;
	પૂર્ણાંક follow_symlinks;
	ગણના batch_mode batch_mode;
	પૂર્ણાંક buffer_output;
	પૂર્ણાંક all_objects;
	પૂર્ણાંક unordered;
	પૂર્ણાંક transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	અક્ષર આગત_delim;
	અક્ષર output_delim;
	અચલ અક્ષર *format;
};

સ્થિર અચલ અક્ષર *force_path;

સ્થિર સંરચના string_list mailmap = STRING_LIST_INIT_NODUP;
સ્થિર પૂર્ણાંક use_mailmap;

સ્થિર અક્ષર *replace_idents_using_mailmap(અક્ષર *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
સ્થિર શૂન્ય load_mailmap(શૂન્ય)
{
	જો (mailmap.strdup_strings)
		પાછા;

	read_mailmap(the_repository, &mailmap);
}

સ્થિર અક્ષર *replace_idents_using_mailmap(અક્ષર *object_buf, size_t *size)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *headers[] = { "author ", "committer ", "tagger ", ખાલી };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	પાછા strbuf_detach(&sb, ખાલી);
}

સ્થિર પૂર્ણાંક filter_object(અચલ અક્ષર *path, unsigned mode,
			 અચલ સંરચના object_id *oid,
			 અક્ષર **buf, size_t *size)
{
	ગણના object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	જો (!*buf)
		પાછા error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	જો ((type == OBJ_BLOB) && S_ISREG(mode)) {
		સંરચના strbuf strbuf = STRBUF_INIT;
		સંરચના checkout_metadata meta;

		init_checkout_metadata(&meta, ખાલી, ખાલી, oid);
		જો (convert_to_working_વૃક્ષ(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			મુક્ત(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, ખાલી);
		}
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક stream_blob(અચલ સંરચના object_id *oid)
{
	જો (odb_stream_blob_to_fd(the_repository->objects, 1, oid, ખાલી, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	પાછા 0;
}

સ્થિર પૂર્ણાંક cat_one_file(પૂર્ણાંક opt, અચલ અક્ષર *exp_type, અચલ અક્ષર *obj_name)
{
	પૂર્ણાંક ret;
	સંરચના object_id oid;
	ગણના object_type type;
	અક્ષર *buf;
	size_t size;
	સંરચના object_context obj_context = {0};
	સંરચના object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	અચલ અક્ષર *path = force_path;
	અચલ પૂર્ણાંક opt_cw = (opt == 'c' || opt == 'w');
	જો (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	જો (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	જો (!path)
		path = obj_context.path;
	જો (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = ખાલી;
	પસંદગી (opt) {
	સ્થિતિ 't':
		oi.typep = &type;
		જો (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		જાઓ cleanup;

	સ્થિતિ 's':
		oi.sizep = &size;

		જો (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (શૂન્ય**)&buf;
		}

		જો (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		જો (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		જાઓ cleanup;

	સ્થિતિ 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		જાઓ cleanup;

	સ્થિતિ 'w':

		જો (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			જાઓ cleanup;
		}
		તોડો;

	સ્થિતિ 'c':
	{
		unsigned long size_ul = 0;
		પૂર્ણાંક textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		જો (textconv_ret)
			તોડો;
	}
		/* else fallthrough */

	સ્થિતિ 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ખાલી);
		જો (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		જો (type == OBJ_વૃક્ષ) {
			અચલ અક્ષર *ls_args[3] = { ખાલી };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_વૃક્ષ(2, ls_args, ખાલી, the_repository);
			જાઓ cleanup;
		}

		જો (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			જાઓ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		જો (!buf)
			die("Cannot read object %s", obj_name);

		જો (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		તોડો;

	સ્થિતિ 0:
	{
		ગણના object_type exp_type_id = type_from_string(exp_type);

		જો (exp_type_id == OBJ_BLOB) {
			સંરચના object_id blob_oid;
			જો (odb_read_object_info(the_repository->objects,
						 &oid, ખાલી) == OBJ_TAG) {
				અક્ષર *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				અચલ અક્ષર *target;

				જો (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				જો (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				મુક્ત(buffer);
			} નહીંતર
				oidcpy(&blob_oid, &oid);

			જો (odb_read_object_info(the_repository->objects,
						 &blob_oid, ખાલી) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				જાઓ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, ખાલી);

		જો (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		તોડો;
	}
	મૂળભૂત:
		die("git cat-file: unknown option: %s", exp_type);
	}

	જો (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	મુક્ત(buf);
	object_context_release(&obj_context);
	પાછા ret;
}

સંરચના expand_data {
	સંરચના object_id oid;
	ગણના object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	અચલ અક્ષર *rest;
	સંરચના object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	પૂર્ણાંક mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	પૂર્ણાંક split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	સંરચના object_info info;

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
	સંરચના string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

સ્થિર પૂર્ણાંક is_atom(અચલ અક્ષર *atom, અચલ અક્ષર *s, પૂર્ણાંક slen)
{
	પૂર્ણાંક alen = strlen(atom);
	પાછા alen == slen && !memcmp(atom, s, alen);
}

સ્થિર પૂર્ણાંક expand_atom(સંરચના strbuf *sb, અચલ અક્ષર *atom, પૂર્ણાંક len,
		       સંરચના expand_data *data)
{
	જો (data->is_remote) {
		size_t i;
		માટે (i = 0; i < data->remote_allowed_atoms.nr; i++)
			જો (is_atom(data->remote_allowed_atoms.વસ્તુઓ[i].string,
				    atom, len))
				તોડો;
		જો (i == data->remote_allowed_atoms.nr)
			પાછા 1;
	}

	જો (is_atom("objectname", atom, len)) {
		જો (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} નહીંતર જો (is_atom("objecttype", atom, len)) {
		જો (data->mark_query) {
			data->info.typep = &data->type;
		} નહીંતર {
			અચલ અક્ષર *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} નહીંતર જો (is_atom("objectsize", atom, len)) {
		જો (data->mark_query)
			data->info.sizep = &data->size;
		નહીંતર
			strbuf_add_uint(sb, data->size);
	} નહીંતર જો (is_atom("objectsize:disk", atom, len)) {
		જો (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		નહીંતર
			strbuf_add_uint(sb, data->disk_size);
	} નહીંતર જો (is_atom("rest", atom, len)) {
		જો (data->mark_query)
			data->split_on_whitespace = 1;
		નહીંતર જો (data->rest)
			strbuf_addstr(sb, data->rest);
	} નહીંતર જો (is_atom("deltabase", atom, len)) {
		જો (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		નહીંતર
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} નહીંતર જો (is_atom("objectmode", atom, len)) {
		જો (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} નહીંતર
		પાછા 0;
	પાછા 1;
}

સ્થિર શૂન્ય expand_format(સંરચના strbuf *sb, અચલ અક્ષર *start,
			  સંરચના expand_data *data)
{
	જ્યારે (strbuf_expand_step(sb, &start)) {
		અચલ અક્ષર *end;

		જો (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		નહીંતર જો ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		નહીંતર
			strbuf_expand_bad_format(start, "cat-file");
	}
}

સ્થિર શૂન્ય batch_write(સંરચના batch_options *opt, અચલ શૂન્ય *data, પૂર્ણાંક len)
{
	જો (opt->buffer_output) {
		જો (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} નહીંતર
		write_or_die(1, data, len);
}

સ્થિર શૂન્ય print_object_or_die(સંરચના batch_options *opt, સંરચના expand_data *data)
{
	અચલ સંરચના object_id *oid = &data->oid;

	પ્રતિજ્ઞા(data->info.typep);

	જો (data->type == OBJ_BLOB) {
		જો (opt->buffer_output)
			fflush(stdout);
		જો (opt->transform_mode) {
			અક્ષર *contents;
			size_t size;

			જો (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			જો (opt->transform_mode == 'w') {
				જો (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} નહીંતર જો (opt->transform_mode == 'c') {
				ગણના object_type type;
				unsigned long size_ul = 0;
				જો (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				નહીંતર
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				જો (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} નહીંતર
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			મુક્ત(contents);
		} નહીંતર {
			stream_blob(oid);
		}
	}
	નહીંતર {
		ગણના object_type type;
		size_t size;
		શૂન્ય *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		જો (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		જો (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		જો (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		જો (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		મુક્ત(contents);
	}
}

સ્થિર શૂન્ય print_default_format(સંરચના strbuf *scratch, સંરચના expand_data *data,
				 સંરચના batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

સ્થિર શૂન્ય report_object_status(સંરચના batch_options *opt,
				 અચલ અક્ષર *obj_name,
				 અચલ સંરચના object_id *oid,
				 અચલ અક્ષર *status)
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
સ્થિર શૂન્ય batch_object_write(અચલ અક્ષર *obj_name,
			       સંરચના strbuf *scratch,
			       સંરચના batch_options *opt,
			       સંરચના expand_data *data,
			       સંરચના packed_git *pack,
			       off_t offset)
{
	જો (!data->skip_object_info) {
		પૂર્ણાંક ret;

		જો (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		જો (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		જો (pack)
			ret = packed_object_info(ખાલી, pack, offset, &data->info);
		નહીંતર
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		જો (ret < 0) {
			જો (data->mode == S_IFGITLINK)
				report_object_status(opt, ખાલી, &data->oid, "submodule");
			નહીંતર
				report_object_status(opt, obj_name, &data->oid, "missing");
			પાછા;
		}

		પસંદગી (opt->objects_filter.choice) {
		સ્થિતિ LOFC_DISABLED:
			તોડો;
		સ્થિતિ LOFC_BLOB_NONE:
			જો (data->type == OBJ_BLOB) {
				જો (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				પાછા;
			}
			તોડો;
		સ્થિતિ LOFC_BLOB_LIMIT:
			જો (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_કિંમત) {
				જો (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				પાછા;
			}
			તોડો;
		સ્થિતિ LOFC_OBJECT_TYPE:
			જો (data->type != opt->objects_filter.object_type) {
				જો (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				પાછા;
			}
			તોડો;
		મૂળભૂત:
			BUG("unsupported objects filter");
		}

		જો (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			અક્ષર *buf = ખાલી;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			જો (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			મુક્ત(buf);
		}
	}

	strbuf_reset(scratch);

	જો (!opt->format) {
		print_default_format(scratch, data, opt);
	} નહીંતર {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	જો (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

સ્થિર શૂન્ય batch_one_object(અચલ અક્ષર *obj_name,
			     સંરચના strbuf *scratch,
			     સંરચના batch_options *opt,
			     સંરચના expand_data *data)
{
	સંરચના object_context ctx = {0};
	પૂર્ણાંક flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	ગણના get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	જો (result != FOUND) {
		પસંદગી (result) {
		સ્થિતિ MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			તોડો;
		સ્થિતિ SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			તોડો;
		સ્થિતિ DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			તોડો;
		સ્થિતિ SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			તોડો;
		સ્થિતિ NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			તોડો;
		મૂળભૂત:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			તોડો;
		}
		fflush(stdout);

		જાઓ out;
	}

	જો (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		જાઓ out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, ખાલી, 0);

out:
	object_context_release(&ctx);
}

સ્થિર પૂર્ણાંક get_remote_info(પૂર્ણાંક argc,
			   અચલ અક્ષર **argv,
			   સંરચના fetch_object_info_results *results,
			   સંરચના oid_array *object_info_oids)
{
	પૂર્ણાંક retval = 0;
	સંરચના remote *remote = ખાલી;
	સંરચના object_id oid;
	સંરચના transport *gtransport;

	remote = remote_get(argv[0]);
	જો (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	માટે (size_t i = 1; i < argc; i++) {
		જો (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			જો (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				માટે (j = 0; j < len; j++)
					જો (!isxdigit(argv[i][j]))
						તોડો;
				જો (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (પૂર્ણાંક)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	જો (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ખાલી);

	જો (!gtransport->smart_options) {
		retval = -1;
		જાઓ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	પાછા retval;
}

સંરચના object_cb_data {
	સંરચના batch_options *opt;
	સંરચના expand_data *expand;
	સંરચના oidset *seen;
	સંરચના strbuf *scratch;
};

સ્થિર પૂર્ણાંક batch_object_cb(અચલ સંરચના object_id *oid, શૂન્ય *vdata)
{
	સંરચના object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(ખાલી, data->scratch, data->opt, data->expand,
			   ખાલી, 0);
	પાછા 0;
}

સ્થિર પૂર્ણાંક collect_object(અચલ સંરચના object_id *oid,
			  સંરચના packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  શૂન્ય *data)
{
	oid_array_append(data, oid);
	પાછા 0;
}

સ્થિર પૂર્ણાંક batch_unordered_object(અચલ સંરચના object_id *oid,
				  સંરચના packed_git *pack,
				  off_t offset,
				  શૂન્ય *vdata)
{
	સંરચના object_cb_data *data = vdata;

	જો (oidset_ઉમેરો(data->seen, oid))
		પાછા 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(ખાલી, data->scratch, data->opt, data->expand,
			   pack, offset);
	પાછા 0;
}

પ્રકારવ્યાખ્યા શૂન્ય (*parse_cmd_fn_t)(સંરચના batch_options *, અચલ અક્ષર *,
			       સંરચના strbuf *, સંરચના expand_data *);

સંરચના queued_cmd {
	parse_cmd_fn_t fn;
	અક્ષર *લીટી;
};

સ્થિર શૂન્ય parse_cmd_contents(સંરચના batch_options *opt,
			     અચલ અક્ષર *લીટી,
			     સંરચના strbuf *output,
			     સંરચના expand_data *data)
{
	ગણના object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(લીટી, output, opt, data);
	data->info.typep = saved_typep;
}

સ્થિર શૂન્ય parse_cmd_info(સંરચના batch_options *opt,
			   અચલ અક્ષર *લીટી,
			   સંરચના strbuf *output,
			   સંરચના expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(લીટી, output, opt, data);
}

સ્થિર શૂન્ય parse_cmd_mailmap(સંરચના batch_options *opt UNUSED,
			      અચલ અક્ષર *લીટી,
			      સંરચના strbuf *output UNUSED,
			      સંરચના expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(લીટી);

	જો (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), લીટી);

	જો (use_mailmap)
		load_mailmap();
}

સ્થિર શૂન્ય parse_cmd_remote_object_info(સંરચના batch_options *opt,
					 અચલ અક્ષર *લીટી, સંરચના strbuf *output,
					 સંરચના expand_data *data)
{
	પૂર્ણાંક ગણતરી;
	અચલ અક્ષર **argv;
	અક્ષર *લીટી_to_split;
	સંરચના fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	સંરચના oid_array object_info_oids = OID_ARRAY_INIT;

	જો (strlen(લીટી) >= MAX_REMOTE_OBJ_INFO_લીટી)
		die(_("remote-object-info command too long"));

	લીટી_to_split = xstrdup(લીટી);
	ગણતરી = split_cmdline(લીટી_to_split, &argv);
	જો (ગણતરી < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(ગણતરી));
	જો (ગણતરી - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	જો (data->info.sizep)
		results.wants_size = 1;
	જો (data->info.typep)
		results.wants_type = 1;

	જો (get_remote_info(ગણતરી, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	જો (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	જો (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	માટે (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		જો (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			ચાલુ;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		જો (results.sizes)
			data->size = results.sizes[i];

		જો (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, ખાલી, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	મુક્ત(લીટી_to_split);
	મુક્ત(argv);
	oid_array_clear(&object_info_oids);
}

સ્થિર શૂન્ય dispatch_calls(સંરચના batch_options *opt,
		સંરચના strbuf *output,
		સંરચના expand_data *data,
		સંરચના queued_cmd *cmd,
		size_t nr)
{
	જો (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	માટે (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].લીટી, output, data);

	fflush(stdout);
}

સ્થિર શૂન્ય free_cmds(સંરચના queued_cmd *cmd, size_t *nr)
{
	માટે (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].લીટી);

	*nr = 0;
}


સ્થિર અચલ સંરચના parse_cmd {
	અચલ અક્ષર *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ખાલી, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

સ્થિર શૂન્ય batch_objects_command(સંરચના batch_options *opt,
				    સંરચના strbuf *output,
				    સંરચના expand_data *data)
{
	સંરચના strbuf આગત = STRBUF_INIT;
	સંરચના queued_cmd *queued_cmd = ખાલી;
	size_t alloc = 0, nr = 0;

	જ્યારે (strbuf_getdelim_strip_crlf(&આગત, stdin, opt->આગત_delim) != EOF) {
		અચલ સંરચના parse_cmd *cmd = ખાલી;
		અચલ અક્ષર *p = ખાલી, *cmd_end;
		સંરચના queued_cmd call = {0};

		જો (!આગત.len)
			die(_("empty command in input"));
		જો (isspace(*આગત.buf))
			die(_("whitespace before command: '%s'"), આગત.buf);

		માટે (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			જો (!skip_prefix(આગત.buf, commands[i].name, &cmd_end))
				ચાલુ;

			cmd = &commands[i];
			જો (cmd->takes_args) {
				જો (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} નહીંતર જો (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			તોડો;
		}

		જો (!cmd)
			die(_("unknown command: '%s'"), આગત.buf);

		જો (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} નહીંતર જો (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} નહીંતર {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.લીટી = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	જો (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	મુક્ત(queued_cmd);
	strbuf_release(&આગત);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

પ્રકારવ્યાખ્યા પૂર્ણાંક (*for_each_object_fn)(અચલ સંરચના object_id *oid, સંરચના packed_git *pack,
				  off_t offset, શૂન્ય *data);

સંરચના for_each_object_payload {
	for_each_object_fn callback;
	શૂન્ય *payload;
};

સ્થિર પૂર્ણાંક batch_one_object_oi(અચલ સંરચના object_id *oid,
			       સંરચના object_info *oi,
			       શૂન્ય *_payload)
{
	સંરચના for_each_object_payload *payload = _payload;
	જો (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		પાછા payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	પાછા payload->callback(oid, ખાલી, 0, payload->payload);
}

સ્થિર શૂન્ય batch_each_object(સંરચના batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      શૂન્ય *_payload)
{
	સંરચના for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	સંરચના odb_source_info source_info;
	સંરચના object_info oi = {
		.source_infop = &source_info,
	};
	સંરચના odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

સ્થિર પૂર્ણાંક batch_objects(સંરચના batch_options *opt)
{
	સંરચના strbuf આગત = STRBUF_INIT;
	સંરચના strbuf output = STRBUF_INIT;
	સંરચના expand_data data = EXPAND_DATA_INIT;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	પૂર્ણાંક save_warning;
	પૂર્ણાંક retval = 0;

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
	જો (opt->transform_mode)
		data.split_on_whitespace = 1;

	જો (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ખાલી;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	જો (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	જો (opt->all_objects) {
		સંરચના object_cb_data cb;
		સંરચના object_info empty = OBJECT_INFO_INIT;

		જો (!memcmp(&data.info, &empty, કદ(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		જો (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		જો (opt->unordered) {
			સંરચના oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} નહીંતર {
			સંરચના oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		પાછા 0;
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

	જો (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		જાઓ cleanup;
	}

	જ્યારે (strbuf_getdelim_strip_crlf(&આગત, stdin, opt->આગત_delim) != EOF) {
		જો (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			અક્ષર *p = strpbrk(આગત.buf, " \t");
			જો (p) {
				જ્યારે (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(આગત.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&આગત);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	પાછા retval;
}

સ્થિર પૂર્ણાંક git_cat_file_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			       અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (userdiff_config(var, કિંમત) < 0)
		પાછા -1;

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર પૂર્ણાંક batch_option_callback(અચલ સંરચના option *opt,
				 અચલ અક્ષર *arg,
				 પૂર્ણાંક unset)
{
	સંરચના batch_options *bo = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (bo->enabled) {
		પાછા error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	જો (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	નહીંતર જો (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	નહીંતર જો (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	નહીંતર
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	પાછા 0;
}

પૂર્ણાંક cmd_cat_file(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક opt = 0;
	પૂર્ણાંક opt_cw = 0;
	પૂર્ણાંક opt_epts = 0;
	અચલ અક્ષર *exp_type = ખાલી, *obj_name = ખાલી;
	સંરચના batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	પૂર્ણાંક unknown_type = 0;
	પૂર્ણાંક આગત_nul_terminated = 0;
	પૂર્ણાંક nul_terminated = 0;
	પૂર્ણાંક ret;

	અચલ અક્ષર * અચલ builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ખાલી
	};
	અચલ સંરચના option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ખાલી, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ખાલી, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ખાલી, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ખાલી, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', ખાલી, &આગત_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ખાલી, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, ખાલી);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	જો (use_mailmap)
		load_mailmap();

	પસંદગી (batch.objects_filter.choice) {
	સ્થિતિ LOFC_DISABLED:
		તોડો;
	સ્થિતિ LOFC_BLOB_NONE:
	સ્થિતિ LOFC_BLOB_LIMIT:
	સ્થિતિ LOFC_OBJECT_TYPE:
		જો (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		તોડો;
	મૂળભૂત:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	જો (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	જો (force_path && !opt_cw)
		usage_સંદેશ_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	જો (batch.enabled)
		;
	નહીંતર જો (batch.follow_symlinks)
		usage_સંદેશ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	નહીંતર જો (batch.buffer_output >= 0)
		usage_સંદેશ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	નહીંતર જો (batch.all_objects)
		usage_સંદેશ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	નહીંતર જો (આગત_nul_terminated)
		usage_સંદેશ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	નહીંતર જો (nul_terminated)
		usage_સંદેશ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.આગત_delim = batch.output_delim = '\n';
	જો (આગત_nul_terminated)
		batch.આગત_delim = '\0';
	જો (nul_terminated)
		batch.આગત_delim = batch.output_delim = '\0';

	/* Batch defaults */
	જો (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	જો (batch.enabled) {
		જો (opt_cw)
			batch.transform_mode = opt;
		નહીંતર જો (opt && opt != 'b')
			usage_સંદેશ_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		નહીંતર જો (argc)
			usage_સંદેશ_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		જાઓ out;
	}

	જો (opt) {
		જો (!argc && opt == 'c')
			usage_સંદેશ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		નહીંતર જો (!argc && opt == 'w')
			usage_સંદેશ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		નહીંતર જો (!argc && opt_epts)
			usage_સંદેશ_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		નહીંતર જો (argc == 1)
			obj_name = argv[0];
		નહીંતર
			usage_સંદેશ_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} નહીંતર જો (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} નહીંતર જો (argc != 2) {
		usage_સંદેશ_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} નહીંતર જો (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	પાછા ret;
}
