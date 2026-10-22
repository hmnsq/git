/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "convert.h"
#অন্তর্ভাৱ "diff.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "ident.h"
#অন্তর্ভাৱ "list-objects-filter-options.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "userdiff.h"
#অন্তর্ভাৱ "oid-array.h"
#অন্তর্ভাৱ "packfile.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "odb/streaming.h"
#অন্তর্ভাৱ "replace-object.h"
#অন্তর্ভাৱ "promisor-remote.h"
#অন্তর্ভাৱ "mailmap.h"
#অন্তর্ভাৱ "write-or-die.h"
#অন্তর্ভাৱ "alias.h"
#অন্তর্ভাৱ "remote.h"
#অন্তর্ভাৱ "transport.h"
#অন্তর্ভাৱ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_সাৰি \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

গণনা batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

সংরচনা batch_options {
	সংরচনা list_objects_filter_options objects_filter;
	পূর্ণাঙ্ক enabled;
	পূর্ণাঙ্ক follow_symlinks;
	গণনা batch_mode batch_mode;
	পূর্ণাঙ্ক buffer_output;
	পূর্ণাঙ্ক all_objects;
	পূর্ণাঙ্ক unordered;
	পূর্ণাঙ্ক transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	ৱর্ণ প্ৰবেশ_delim;
	ৱর্ণ output_delim;
	ধ্রুৱ ৱর্ণ *format;
};

স্থির ধ্রুৱ ৱর্ণ *force_path;

স্থির সংরচনা string_list mailmap = STRING_LIST_INIT_NODUP;
স্থির পূর্ণাঙ্ক use_mailmap;

স্থির ৱর্ণ *replace_idents_using_mailmap(ৱর্ণ *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
স্থির শূন্য load_mailmap(শূন্য)
{
	যদি (mailmap.strdup_strings)
		ঘূৰাই;

	read_mailmap(the_repository, &mailmap);
}

স্থির ৱর্ণ *replace_idents_using_mailmap(ৱর্ণ *object_buf, size_t *size)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *headers[] = { "author ", "committer ", "tagger ", রিক্ত };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	ঘূৰাই strbuf_detach(&sb, রিক্ত);
}

স্থির পূর্ণাঙ্ক filter_object(ধ্রুৱ ৱর্ণ *path, unsigned mode,
			 ধ্রুৱ সংরচনা object_id *oid,
			 ৱর্ণ **buf, size_t *size)
{
	গণনা object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	যদি (!*buf)
		ঘূৰাই error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	যদি ((type == OBJ_BLOB) && S_ISREG(mode)) {
		সংরচনা strbuf strbuf = STRBUF_INIT;
		সংরচনা checkout_metadata meta;

		init_checkout_metadata(&meta, রিক্ত, রিক্ত, oid);
		যদি (convert_to_working_বৃক্ষ(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			মুক্ত(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, রিক্ত);
		}
	}

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক stream_blob(ধ্রুৱ সংরচনা object_id *oid)
{
	যদি (odb_stream_blob_to_fd(the_repository->objects, 1, oid, রিক্ত, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক cat_one_file(পূর্ণাঙ্ক opt, ধ্রুৱ ৱর্ণ *exp_type, ধ্রুৱ ৱর্ণ *obj_name)
{
	পূর্ণাঙ্ক ret;
	সংরচনা object_id oid;
	গণনা object_type type;
	ৱর্ণ *buf;
	size_t size;
	সংরচনা object_context obj_context = {0};
	সংরচনা object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	ধ্রুৱ ৱর্ণ *path = force_path;
	ধ্রুৱ পূর্ণাঙ্ক opt_cw = (opt == 'c' || opt == 'w');
	যদি (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	যদি (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	যদি (!path)
		path = obj_context.path;
	যদি (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = রিক্ত;
	ৱিকল্প (opt) {
	পক্ষ 't':
		oi.typep = &type;
		যদি (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		গচ্ছ cleanup;

	পক্ষ 's':
		oi.sizep = &size;

		যদি (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (শূন্য**)&buf;
		}

		যদি (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		যদি (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		গচ্ছ cleanup;

	পক্ষ 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		গচ্ছ cleanup;

	পক্ষ 'w':

		যদি (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			গচ্ছ cleanup;
		}
		ভঙ্গ;

	পক্ষ 'c':
	{
		unsigned long size_ul = 0;
		পূর্ণাঙ্ক textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		যদি (textconv_ret)
			ভঙ্গ;
	}
		/* else fallthrough */

	পক্ষ 'p':
		type = odb_read_object_info(the_repository->objects, &oid, রিক্ত);
		যদি (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		যদি (type == OBJ_বৃক্ষ) {
			ধ্রুৱ ৱর্ণ *ls_args[3] = { রিক্ত };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_বৃক্ষ(2, ls_args, রিক্ত, the_repository);
			গচ্ছ cleanup;
		}

		যদি (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			গচ্ছ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		যদি (!buf)
			die("Cannot read object %s", obj_name);

		যদি (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		ভঙ্গ;

	পক্ষ 0:
	{
		গণনা object_type exp_type_id = type_from_string(exp_type);

		যদি (exp_type_id == OBJ_BLOB) {
			সংরচনা object_id blob_oid;
			যদি (odb_read_object_info(the_repository->objects,
						 &oid, রিক্ত) == OBJ_TAG) {
				ৱর্ণ *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				ধ্রুৱ ৱর্ণ *target;

				যদি (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				যদি (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				মুক্ত(buffer);
			} অন্যথা
				oidcpy(&blob_oid, &oid);

			যদি (odb_read_object_info(the_repository->objects,
						 &blob_oid, রিক্ত) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				গচ্ছ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, রিক্ত);

		যদি (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		ভঙ্গ;
	}
	স্ৱভাৱ:
		die("git cat-file: unknown option: %s", exp_type);
	}

	যদি (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	মুক্ত(buf);
	object_context_release(&obj_context);
	ঘূৰাই ret;
}

সংরচনা expand_data {
	সংরচনা object_id oid;
	গণনা object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	ধ্রুৱ ৱর্ণ *rest;
	সংরচনা object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	পূর্ণাঙ্ক mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	পূর্ণাঙ্ক split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	সংরচনা object_info info;

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
	সংরচনা string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

স্থির পূর্ণাঙ্ক is_atom(ধ্রুৱ ৱর্ণ *atom, ধ্রুৱ ৱর্ণ *s, পূর্ণাঙ্ক slen)
{
	পূর্ণাঙ্ক alen = strlen(atom);
	ঘূৰাই alen == slen && !memcmp(atom, s, alen);
}

স্থির পূর্ণাঙ্ক expand_atom(সংরচনা strbuf *sb, ধ্রুৱ ৱর্ণ *atom, পূর্ণাঙ্ক len,
		       সংরচনা expand_data *data)
{
	যদি (data->is_remote) {
		size_t i;
		বাবে (i = 0; i < data->remote_allowed_atoms.nr; i++)
			যদি (is_atom(data->remote_allowed_atoms.বস্তুসমূহ[i].string,
				    atom, len))
				ভঙ্গ;
		যদি (i == data->remote_allowed_atoms.nr)
			ঘূৰাই 1;
	}

	যদি (is_atom("objectname", atom, len)) {
		যদি (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} অন্যথা যদি (is_atom("objecttype", atom, len)) {
		যদি (data->mark_query) {
			data->info.typep = &data->type;
		} অন্যথা {
			ধ্রুৱ ৱর্ণ *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} অন্যথা যদি (is_atom("objectsize", atom, len)) {
		যদি (data->mark_query)
			data->info.sizep = &data->size;
		অন্যথা
			strbuf_add_uint(sb, data->size);
	} অন্যথা যদি (is_atom("objectsize:disk", atom, len)) {
		যদি (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		অন্যথা
			strbuf_add_uint(sb, data->disk_size);
	} অন্যথা যদি (is_atom("rest", atom, len)) {
		যদি (data->mark_query)
			data->split_on_whitespace = 1;
		অন্যথা যদি (data->rest)
			strbuf_addstr(sb, data->rest);
	} অন্যথা যদি (is_atom("deltabase", atom, len)) {
		যদি (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		অন্যথা
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} অন্যথা যদি (is_atom("objectmode", atom, len)) {
		যদি (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} অন্যথা
		ঘূৰাই 0;
	ঘূৰাই 1;
}

স্থির শূন্য expand_format(সংরচনা strbuf *sb, ধ্রুৱ ৱর্ণ *start,
			  সংরচনা expand_data *data)
{
	যেতিয়ালৈ (strbuf_expand_step(sb, &start)) {
		ধ্রুৱ ৱর্ণ *end;

		যদি (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		অন্যথা যদি ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		অন্যথা
			strbuf_expand_bad_format(start, "cat-file");
	}
}

স্থির শূন্য batch_write(সংরচনা batch_options *opt, ধ্রুৱ শূন্য *data, পূর্ণাঙ্ক len)
{
	যদি (opt->buffer_output) {
		যদি (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} অন্যথা
		write_or_die(1, data, len);
}

স্থির শূন্য print_object_or_die(সংরচনা batch_options *opt, সংরচনা expand_data *data)
{
	ধ্রুৱ সংরচনা object_id *oid = &data->oid;

	প্রতিজ্ঞা(data->info.typep);

	যদি (data->type == OBJ_BLOB) {
		যদি (opt->buffer_output)
			fflush(stdout);
		যদি (opt->transform_mode) {
			ৱর্ণ *contents;
			size_t size;

			যদি (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			যদি (opt->transform_mode == 'w') {
				যদি (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} অন্যথা যদি (opt->transform_mode == 'c') {
				গণনা object_type type;
				unsigned long size_ul = 0;
				যদি (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				অন্যথা
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				যদি (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} অন্যথা
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			মুক্ত(contents);
		} অন্যথা {
			stream_blob(oid);
		}
	}
	অন্যথা {
		গণনা object_type type;
		size_t size;
		শূন্য *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		যদি (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		যদি (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		যদি (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		যদি (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		মুক্ত(contents);
	}
}

স্থির শূন্য print_default_format(সংরচনা strbuf *scratch, সংরচনা expand_data *data,
				 সংরচনা batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

স্থির শূন্য report_object_status(সংরচনা batch_options *opt,
				 ধ্রুৱ ৱর্ণ *obj_name,
				 ধ্রুৱ সংরচনা object_id *oid,
				 ধ্রুৱ ৱর্ণ *status)
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
স্থির শূন্য batch_object_write(ধ্রুৱ ৱর্ণ *obj_name,
			       সংরচনা strbuf *scratch,
			       সংরচনা batch_options *opt,
			       সংরচনা expand_data *data,
			       সংরচনা packed_git *pack,
			       off_t offset)
{
	যদি (!data->skip_object_info) {
		পূর্ণাঙ্ক ret;

		যদি (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		যদি (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		যদি (pack)
			ret = packed_object_info(রিক্ত, pack, offset, &data->info);
		অন্যথা
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		যদি (ret < 0) {
			যদি (data->mode == S_IFGITLINK)
				report_object_status(opt, রিক্ত, &data->oid, "submodule");
			অন্যথা
				report_object_status(opt, obj_name, &data->oid, "missing");
			ঘূৰাই;
		}

		ৱিকল্প (opt->objects_filter.choice) {
		পক্ষ LOFC_DISABLED:
			ভঙ্গ;
		পক্ষ LOFC_BLOB_NONE:
			যদি (data->type == OBJ_BLOB) {
				যদি (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ঘূৰাই;
			}
			ভঙ্গ;
		পক্ষ LOFC_BLOB_LIMIT:
			যদি (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_মান) {
				যদি (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ঘূৰাই;
			}
			ভঙ্গ;
		পক্ষ LOFC_OBJECT_TYPE:
			যদি (data->type != opt->objects_filter.object_type) {
				যদি (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ঘূৰাই;
			}
			ভঙ্গ;
		স্ৱভাৱ:
			BUG("unsupported objects filter");
		}

		যদি (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			ৱর্ণ *buf = রিক্ত;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			যদি (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			মুক্ত(buf);
		}
	}

	strbuf_reset(scratch);

	যদি (!opt->format) {
		print_default_format(scratch, data, opt);
	} অন্যথা {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	যদি (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

স্থির শূন্য batch_one_object(ধ্রুৱ ৱর্ণ *obj_name,
			     সংরচনা strbuf *scratch,
			     সংরচনা batch_options *opt,
			     সংরচনা expand_data *data)
{
	সংরচনা object_context ctx = {0};
	পূর্ণাঙ্ক flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	গণনা get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	যদি (result != FOUND) {
		ৱিকল্প (result) {
		পক্ষ MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			ভঙ্গ;
		পক্ষ SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			ভঙ্গ;
		পক্ষ DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ভঙ্গ;
		পক্ষ SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ভঙ্গ;
		পক্ষ NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ভঙ্গ;
		স্ৱভাৱ:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			ভঙ্গ;
		}
		fflush(stdout);

		গচ্ছ out;
	}

	যদি (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		গচ্ছ out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, রিক্ত, 0);

out:
	object_context_release(&ctx);
}

স্থির পূর্ণাঙ্ক get_remote_info(পূর্ণাঙ্ক argc,
			   ধ্রুৱ ৱর্ণ **argv,
			   সংরচনা fetch_object_info_results *results,
			   সংরচনা oid_array *object_info_oids)
{
	পূর্ণাঙ্ক retval = 0;
	সংরচনা remote *remote = রিক্ত;
	সংরচনা object_id oid;
	সংরচনা transport *gtransport;

	remote = remote_get(argv[0]);
	যদি (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	বাবে (size_t i = 1; i < argc; i++) {
		যদি (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			যদি (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				বাবে (j = 0; j < len; j++)
					যদি (!isxdigit(argv[i][j]))
						ভঙ্গ;
				যদি (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (পূর্ণাঙ্ক)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	যদি (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, রিক্ত);

	যদি (!gtransport->smart_options) {
		retval = -1;
		গচ্ছ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ঘূৰাই retval;
}

সংরচনা object_cb_data {
	সংরচনা batch_options *opt;
	সংরচনা expand_data *expand;
	সংরচনা oidset *seen;
	সংরচনা strbuf *scratch;
};

স্থির পূর্ণাঙ্ক batch_object_cb(ধ্রুৱ সংরচনা object_id *oid, শূন্য *vdata)
{
	সংরচনা object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(রিক্ত, data->scratch, data->opt, data->expand,
			   রিক্ত, 0);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক collect_object(ধ্রুৱ সংরচনা object_id *oid,
			  সংরচনা packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  শূন্য *data)
{
	oid_array_append(data, oid);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক batch_unordered_object(ধ্রুৱ সংরচনা object_id *oid,
				  সংরচনা packed_git *pack,
				  off_t offset,
				  শূন্য *vdata)
{
	সংরচনা object_cb_data *data = vdata;

	যদি (oidset_ঢোকাও(data->seen, oid))
		ঘূৰাই 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(রিক্ত, data->scratch, data->opt, data->expand,
			   pack, offset);
	ঘূৰাই 0;
}

প্রকারনির্দেশ শূন্য (*parse_cmd_fn_t)(সংরচনা batch_options *, ধ্রুৱ ৱর্ণ *,
			       সংরচনা strbuf *, সংরচনা expand_data *);

সংরচনা queued_cmd {
	parse_cmd_fn_t fn;
	ৱর্ণ *সাৰি;
};

স্থির শূন্য parse_cmd_contents(সংরচনা batch_options *opt,
			     ধ্রুৱ ৱর্ণ *সাৰি,
			     সংরচনা strbuf *output,
			     সংরচনা expand_data *data)
{
	গণনা object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(সাৰি, output, opt, data);
	data->info.typep = saved_typep;
}

স্থির শূন্য parse_cmd_info(সংরচনা batch_options *opt,
			   ধ্রুৱ ৱর্ণ *সাৰি,
			   সংরচনা strbuf *output,
			   সংরচনা expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(সাৰি, output, opt, data);
}

স্থির শূন্য parse_cmd_mailmap(সংরচনা batch_options *opt UNUSED,
			      ধ্রুৱ ৱর্ণ *সাৰি,
			      সংরচনা strbuf *output UNUSED,
			      সংরচনা expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(সাৰি);

	যদি (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), সাৰি);

	যদি (use_mailmap)
		load_mailmap();
}

স্থির শূন্য parse_cmd_remote_object_info(সংরচনা batch_options *opt,
					 ধ্রুৱ ৱর্ণ *সাৰি, সংরচনা strbuf *output,
					 সংরচনা expand_data *data)
{
	পূর্ণাঙ্ক গুনতি;
	ধ্রুৱ ৱর্ণ **argv;
	ৱর্ণ *সাৰি_to_split;
	সংরচনা fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	সংরচনা oid_array object_info_oids = OID_ARRAY_INIT;

	যদি (strlen(সাৰি) >= MAX_REMOTE_OBJ_INFO_সাৰি)
		die(_("remote-object-info command too long"));

	সাৰি_to_split = xstrdup(সাৰি);
	গুনতি = split_cmdline(সাৰি_to_split, &argv);
	যদি (গুনতি < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(গুনতি));
	যদি (গুনতি - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	যদি (data->info.sizep)
		results.wants_size = 1;
	যদি (data->info.typep)
		results.wants_type = 1;

	যদি (get_remote_info(গুনতি, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	যদি (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	যদি (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	বাবে (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		যদি (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			অনুৱর্ত;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		যদি (results.sizes)
			data->size = results.sizes[i];

		যদি (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, রিক্ত, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	মুক্ত(সাৰি_to_split);
	মুক্ত(argv);
	oid_array_clear(&object_info_oids);
}

স্থির শূন্য dispatch_calls(সংরচনা batch_options *opt,
		সংরচনা strbuf *output,
		সংরচনা expand_data *data,
		সংরচনা queued_cmd *cmd,
		size_t nr)
{
	যদি (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	বাবে (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].সাৰি, output, data);

	fflush(stdout);
}

স্থির শূন্য free_cmds(সংরচনা queued_cmd *cmd, size_t *nr)
{
	বাবে (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].সাৰি);

	*nr = 0;
}


স্থির ধ্রুৱ সংরচনা parse_cmd {
	ধ্রুৱ ৱর্ণ *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", রিক্ত, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

স্থির শূন্য batch_objects_command(সংরচনা batch_options *opt,
				    সংরচনা strbuf *output,
				    সংরচনা expand_data *data)
{
	সংরচনা strbuf প্ৰবেশ = STRBUF_INIT;
	সংরচনা queued_cmd *queued_cmd = রিক্ত;
	size_t alloc = 0, nr = 0;

	যেতিয়ালৈ (strbuf_getdelim_strip_crlf(&প্ৰবেশ, stdin, opt->প্ৰবেশ_delim) != EOF) {
		ধ্রুৱ সংরচনা parse_cmd *cmd = রিক্ত;
		ধ্রুৱ ৱর্ণ *p = রিক্ত, *cmd_end;
		সংরচনা queued_cmd call = {0};

		যদি (!প্ৰবেশ.len)
			die(_("empty command in input"));
		যদি (isspace(*প্ৰবেশ.buf))
			die(_("whitespace before command: '%s'"), প্ৰবেশ.buf);

		বাবে (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			যদি (!skip_prefix(প্ৰবেশ.buf, commands[i].name, &cmd_end))
				অনুৱর্ত;

			cmd = &commands[i];
			যদি (cmd->takes_args) {
				যদি (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} অন্যথা যদি (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			ভঙ্গ;
		}

		যদি (!cmd)
			die(_("unknown command: '%s'"), প্ৰবেশ.buf);

		যদি (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} অন্যথা যদি (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} অন্যথা {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.সাৰি = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	যদি (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	মুক্ত(queued_cmd);
	strbuf_release(&প্ৰবেশ);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

প্রকারনির্দেশ পূর্ণাঙ্ক (*for_each_object_fn)(ধ্রুৱ সংরচনা object_id *oid, সংরচনা packed_git *pack,
				  off_t offset, শূন্য *data);

সংরচনা for_each_object_payload {
	for_each_object_fn callback;
	শূন্য *payload;
};

স্থির পূর্ণাঙ্ক batch_one_object_oi(ধ্রুৱ সংরচনা object_id *oid,
			       সংরচনা object_info *oi,
			       শূন্য *_payload)
{
	সংরচনা for_each_object_payload *payload = _payload;
	যদি (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		ঘূৰাই payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	ঘূৰাই payload->callback(oid, রিক্ত, 0, payload->payload);
}

স্থির শূন্য batch_each_object(সংরচনা batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      শূন্য *_payload)
{
	সংরচনা for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	সংরচনা odb_source_info source_info;
	সংরচনা object_info oi = {
		.source_infop = &source_info,
	};
	সংরচনা odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

স্থির পূর্ণাঙ্ক batch_objects(সংরচনা batch_options *opt)
{
	সংরচনা strbuf প্ৰবেশ = STRBUF_INIT;
	সংরচনা strbuf output = STRBUF_INIT;
	সংরচনা expand_data data = EXPAND_DATA_INIT;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণাঙ্ক save_warning;
	পূর্ণাঙ্ক retval = 0;

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
	যদি (opt->transform_mode)
		data.split_on_whitespace = 1;

	যদি (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = রিক্ত;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	যদি (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	যদি (opt->all_objects) {
		সংরচনা object_cb_data cb;
		সংরচনা object_info empty = OBJECT_INFO_INIT;

		যদি (!memcmp(&data.info, &empty, পরিমাণ(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		যদি (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		যদি (opt->unordered) {
			সংরচনা oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} অন্যথা {
			সংরচনা oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ঘূৰাই 0;
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

	যদি (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		গচ্ছ cleanup;
	}

	যেতিয়ালৈ (strbuf_getdelim_strip_crlf(&প্ৰবেশ, stdin, opt->প্ৰবেশ_delim) != EOF) {
		যদি (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			ৱর্ণ *p = strpbrk(প্ৰবেশ.buf, " \t");
			যদি (p) {
				যেতিয়ালৈ (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(প্ৰবেশ.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&প্ৰবেশ);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	ঘূৰাই retval;
}

স্থির পূর্ণাঙ্ক git_cat_file_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			       ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (userdiff_config(var, মান) < 0)
		ঘূৰাই -1;

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণাঙ্ক batch_option_callback(ধ্রুৱ সংরচনা option *opt,
				 ধ্রুৱ ৱর্ণ *arg,
				 পূর্ণাঙ্ক unset)
{
	সংরচনা batch_options *bo = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (bo->enabled) {
		ঘূৰাই error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	যদি (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	অন্যথা যদি (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	অন্যথা যদি (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	অন্যথা
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_cat_file(পূর্ণাঙ্ক argc,
		 ধ্রুৱ ৱর্ণ **argv,
		 ধ্রুৱ ৱর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক opt = 0;
	পূর্ণাঙ্ক opt_cw = 0;
	পূর্ণাঙ্ক opt_epts = 0;
	ধ্রুৱ ৱর্ণ *exp_type = রিক্ত, *obj_name = রিক্ত;
	সংরচনা batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	পূর্ণাঙ্ক unknown_type = 0;
	পূর্ণাঙ্ক প্ৰবেশ_nul_terminated = 0;
	পূর্ণাঙ্ক nul_terminated = 0;
	পূর্ণাঙ্ক ret;

	ধ্রুৱ ৱর্ণ * ধ্রুৱ builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		রিক্ত
	};
	ধ্রুৱ সংরচনা option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', রিক্ত, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', রিক্ত, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', রিক্ত, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', রিক্ত, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', রিক্ত, &প্ৰবেশ_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', রিক্ত, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, রিক্ত);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	যদি (use_mailmap)
		load_mailmap();

	ৱিকল্প (batch.objects_filter.choice) {
	পক্ষ LOFC_DISABLED:
		ভঙ্গ;
	পক্ষ LOFC_BLOB_NONE:
	পক্ষ LOFC_BLOB_LIMIT:
	পক্ষ LOFC_OBJECT_TYPE:
		যদি (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		ভঙ্গ;
	স্ৱভাৱ:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	যদি (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	যদি (force_path && !opt_cw)
		usage_বাৰ্তা_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	যদি (batch.enabled)
		;
	অন্যথা যদি (batch.follow_symlinks)
		usage_বাৰ্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	অন্যথা যদি (batch.buffer_output >= 0)
		usage_বাৰ্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	অন্যথা যদি (batch.all_objects)
		usage_বাৰ্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	অন্যথা যদি (প্ৰবেশ_nul_terminated)
		usage_বাৰ্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	অন্যথা যদি (nul_terminated)
		usage_বাৰ্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.প্ৰবেশ_delim = batch.output_delim = '\n';
	যদি (প্ৰবেশ_nul_terminated)
		batch.প্ৰবেশ_delim = '\0';
	যদি (nul_terminated)
		batch.প্ৰবেশ_delim = batch.output_delim = '\0';

	/* Batch defaults */
	যদি (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	যদি (batch.enabled) {
		যদি (opt_cw)
			batch.transform_mode = opt;
		অন্যথা যদি (opt && opt != 'b')
			usage_বাৰ্তা_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		অন্যথা যদি (argc)
			usage_বাৰ্তা_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		গচ্ছ out;
	}

	যদি (opt) {
		যদি (!argc && opt == 'c')
			usage_বাৰ্তা_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		অন্যথা যদি (!argc && opt == 'w')
			usage_বাৰ্তা_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		অন্যথা যদি (!argc && opt_epts)
			usage_বাৰ্তা_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		অন্যথা যদি (argc == 1)
			obj_name = argv[0];
		অন্যথা
			usage_বাৰ্তা_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} অন্যথা যদি (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} অন্যথা যদি (argc != 2) {
		usage_বাৰ্তা_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} অন্যথা যদি (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	ঘূৰাই ret;
}
