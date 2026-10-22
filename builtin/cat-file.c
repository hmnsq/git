/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "convert.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "ident.h"
#ଅନ୍ତର୍ଭାଵ "list-objects-filter-options.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "userdiff.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "packfile.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/streaming.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "promisor-remote.h"
#ଅନ୍ତର୍ଭାଵ "mailmap.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"
#ଅନ୍ତର୍ଭାଵ "alias.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "transport.h"
#ଅନ୍ତର୍ଭାଵ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_ଧାଡ଼ି \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

ଗଣନା batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

ସଂରଚନା batch_options {
	ସଂରଚନା list_objects_filter_options objects_filter;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା enabled;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା follow_symlinks;
	ଗଣନା batch_mode batch_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା buffer_output;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_objects;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା unordered;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	ଅକ୍ଷର ନିବେଶ_delim;
	ଅକ୍ଷର output_delim;
	ଅଚଳ ଅକ୍ଷର *format;
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *force_path;

ସ୍ଥିର ସଂରଚନା string_list mailmap = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_mailmap;

ସ୍ଥିର ଅକ୍ଷର *replace_idents_using_mailmap(ଅକ୍ଷର *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ସ୍ଥିର ଶୂନ୍ୟ load_mailmap(ଶୂନ୍ୟ)
{
	ଯଦି (mailmap.strdup_strings)
		ଫେରନ୍ତୁ;

	read_mailmap(the_repository, &mailmap);
}

ସ୍ଥିର ଅକ୍ଷର *replace_idents_using_mailmap(ଅକ୍ଷର *object_buf, size_t *size)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *headers[] = { "author ", "committer ", "tagger ", ଖାଲି };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	ଫେରନ୍ତୁ strbuf_detach(&sb, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_object(ଅଚଳ ଅକ୍ଷର *path, unsigned mode,
			 ଅଚଳ ସଂରଚନା object_id *oid,
			 ଅକ୍ଷର **buf, size_t *size)
{
	ଗଣନା object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	ଯଦି (!*buf)
		ଫେରନ୍ତୁ error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	ଯଦି ((type == OBJ_BLOB) && S_ISREG(mode)) {
		ସଂରଚନା strbuf strbuf = STRBUF_INIT;
		ସଂରଚନା checkout_metadata meta;

		init_checkout_metadata(&meta, ଖାଲି, ଖାଲି, oid);
		ଯଦି (convert_to_working_ବୃକ୍ଷ(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			ମୁକ୍ତ(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, ଖାଲି);
		}
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stream_blob(ଅଚଳ ସଂରଚନା object_id *oid)
{
	ଯଦି (odb_stream_blob_to_fd(the_repository->objects, 1, oid, ଖାଲି, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cat_one_file(ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt, ଅଚଳ ଅକ୍ଷର *exp_type, ଅଚଳ ଅକ୍ଷର *obj_name)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା object_id oid;
	ଗଣନା object_type type;
	ଅକ୍ଷର *buf;
	size_t size;
	ସଂରଚନା object_context obj_context = {0};
	ସଂରଚନା object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	ଅଚଳ ଅକ୍ଷର *path = force_path;
	ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_cw = (opt == 'c' || opt == 'w');
	ଯଦି (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	ଯଦି (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	ଯଦି (!path)
		path = obj_context.path;
	ଯଦି (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = ଖାଲି;
	ବାଛ (opt) {
	ସ୍ଥିତି 't':
		oi.typep = &type;
		ଯଦି (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		ଯାଅ cleanup;

	ସ୍ଥିତି 's':
		oi.sizep = &size;

		ଯଦି (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (ଶୂନ୍ୟ**)&buf;
		}

		ଯଦି (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		ଯଦି (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		ଯାଅ cleanup;

	ସ୍ଥିତି 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		ଯାଅ cleanup;

	ସ୍ଥିତି 'w':

		ଯଦି (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			ଯାଅ cleanup;
		}
		ଭାଙ୍ଗ;

	ସ୍ଥିତି 'c':
	{
		unsigned long size_ul = 0;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		ଯଦି (textconv_ret)
			ଭାଙ୍ଗ;
	}
		/* else fallthrough */

	ସ୍ଥିତି 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ଖାଲି);
		ଯଦି (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		ଯଦି (type == OBJ_ବୃକ୍ଷ) {
			ଅଚଳ ଅକ୍ଷର *ls_args[3] = { ଖାଲି };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_ବୃକ୍ଷ(2, ls_args, ଖାଲି, the_repository);
			ଯାଅ cleanup;
		}

		ଯଦି (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			ଯାଅ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		ଯଦି (!buf)
			die("Cannot read object %s", obj_name);

		ଯଦି (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		ଭାଙ୍ଗ;

	ସ୍ଥିତି 0:
	{
		ଗଣନା object_type exp_type_id = type_from_string(exp_type);

		ଯଦି (exp_type_id == OBJ_BLOB) {
			ସଂରଚନା object_id blob_oid;
			ଯଦି (odb_read_object_info(the_repository->objects,
						 &oid, ଖାଲି) == OBJ_TAG) {
				ଅକ୍ଷର *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				ଅଚଳ ଅକ୍ଷର *target;

				ଯଦି (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				ଯଦି (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				ମୁକ୍ତ(buffer);
			} ଅନ୍ୟଥା
				oidcpy(&blob_oid, &oid);

			ଯଦି (odb_read_object_info(the_repository->objects,
						 &blob_oid, ଖାଲି) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				ଯାଅ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, ଖାଲି);

		ଯଦି (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		ଭାଙ୍ଗ;
	}
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		die("git cat-file: unknown option: %s", exp_type);
	}

	ଯଦି (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	ମୁକ୍ତ(buf);
	object_context_release(&obj_context);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା expand_data {
	ସଂରଚନା object_id oid;
	ଗଣନା object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	ଅଚଳ ଅକ୍ଷର *rest;
	ସଂରଚନା object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	ସଂରଚନା object_info info;

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
	ସଂରଚନା string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_atom(ଅଚଳ ଅକ୍ଷର *atom, ଅଚଳ ଅକ୍ଷର *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା slen)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା alen = strlen(atom);
	ଫେରନ୍ତୁ alen == slen && !memcmp(atom, s, alen);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା expand_atom(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *atom, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
		       ସଂରଚନା expand_data *data)
{
	ଯଦି (data->is_remote) {
		size_t i;
		ପାଇଁ (i = 0; i < data->remote_allowed_atoms.nr; i++)
			ଯଦି (is_atom(data->remote_allowed_atoms.ବସ୍ତୁଗୁଡ଼ିକ[i].string,
				    atom, len))
				ଭାଙ୍ଗ;
		ଯଦି (i == data->remote_allowed_atoms.nr)
			ଫେରନ୍ତୁ 1;
	}

	ଯଦି (is_atom("objectname", atom, len)) {
		ଯଦି (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} ଅନ୍ୟଥା ଯଦି (is_atom("objecttype", atom, len)) {
		ଯଦି (data->mark_query) {
			data->info.typep = &data->type;
		} ଅନ୍ୟଥା {
			ଅଚଳ ଅକ୍ଷର *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} ଅନ୍ୟଥା ଯଦି (is_atom("objectsize", atom, len)) {
		ଯଦି (data->mark_query)
			data->info.sizep = &data->size;
		ଅନ୍ୟଥା
			strbuf_add_uint(sb, data->size);
	} ଅନ୍ୟଥା ଯଦି (is_atom("objectsize:disk", atom, len)) {
		ଯଦି (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		ଅନ୍ୟଥା
			strbuf_add_uint(sb, data->disk_size);
	} ଅନ୍ୟଥା ଯଦି (is_atom("rest", atom, len)) {
		ଯଦି (data->mark_query)
			data->split_on_whitespace = 1;
		ଅନ୍ୟଥା ଯଦି (data->rest)
			strbuf_addstr(sb, data->rest);
	} ଅନ୍ୟଥା ଯଦି (is_atom("deltabase", atom, len)) {
		ଯଦି (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		ଅନ୍ୟଥା
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} ଅନ୍ୟଥା ଯଦି (is_atom("objectmode", atom, len)) {
		ଯଦି (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଶୂନ୍ୟ expand_format(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *start,
			  ସଂରଚନା expand_data *data)
{
	ଯେତେବେଳେ (strbuf_expand_step(sb, &start)) {
		ଅଚଳ ଅକ୍ଷର *end;

		ଯଦି (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		ଅନ୍ୟଥା ଯଦି ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		ଅନ୍ୟଥା
			strbuf_expand_bad_format(start, "cat-file");
	}
}

ସ୍ଥିର ଶୂନ୍ୟ batch_write(ସଂରଚନା batch_options *opt, ଅଚଳ ଶୂନ୍ୟ *data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ଯଦି (opt->buffer_output) {
		ଯଦି (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} ଅନ୍ୟଥା
		write_or_die(1, data, len);
}

ସ୍ଥିର ଶୂନ୍ୟ print_object_or_die(ସଂରଚନା batch_options *opt, ସଂରଚନା expand_data *data)
{
	ଅଚଳ ସଂରଚନା object_id *oid = &data->oid;

	ପ୍ରତିଜ୍ଞା(data->info.typep);

	ଯଦି (data->type == OBJ_BLOB) {
		ଯଦି (opt->buffer_output)
			fflush(stdout);
		ଯଦି (opt->transform_mode) {
			ଅକ୍ଷର *contents;
			size_t size;

			ଯଦି (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			ଯଦି (opt->transform_mode == 'w') {
				ଯଦି (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} ଅନ୍ୟଥା ଯଦି (opt->transform_mode == 'c') {
				ଗଣନା object_type type;
				unsigned long size_ul = 0;
				ଯଦି (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				ଅନ୍ୟଥା
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				ଯଦି (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} ଅନ୍ୟଥା
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			ମୁକ୍ତ(contents);
		} ଅନ୍ୟଥା {
			stream_blob(oid);
		}
	}
	ଅନ୍ୟଥା {
		ଗଣନା object_type type;
		size_t size;
		ଶୂନ୍ୟ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		ଯଦି (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		ଯଦି (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		ଯଦି (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		ଯଦି (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		ମୁକ୍ତ(contents);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ print_default_format(ସଂରଚନା strbuf *scratch, ସଂରଚନା expand_data *data,
				 ସଂରଚନା batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

ସ୍ଥିର ଶୂନ୍ୟ report_object_status(ସଂରଚନା batch_options *opt,
				 ଅଚଳ ଅକ୍ଷର *obj_name,
				 ଅଚଳ ସଂରଚନା object_id *oid,
				 ଅଚଳ ଅକ୍ଷର *status)
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
ସ୍ଥିର ଶୂନ୍ୟ batch_object_write(ଅଚଳ ଅକ୍ଷର *obj_name,
			       ସଂରଚନା strbuf *scratch,
			       ସଂରଚନା batch_options *opt,
			       ସଂରଚନା expand_data *data,
			       ସଂରଚନା packed_git *pack,
			       off_t offset)
{
	ଯଦି (!data->skip_object_info) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

		ଯଦି (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		ଯଦି (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		ଯଦି (pack)
			ret = packed_object_info(ଖାଲି, pack, offset, &data->info);
		ଅନ୍ୟଥା
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		ଯଦି (ret < 0) {
			ଯଦି (data->mode == S_IFGITLINK)
				report_object_status(opt, ଖାଲି, &data->oid, "submodule");
			ଅନ୍ୟଥା
				report_object_status(opt, obj_name, &data->oid, "missing");
			ଫେରନ୍ତୁ;
		}

		ବାଛ (opt->objects_filter.choice) {
		ସ୍ଥିତି LOFC_DISABLED:
			ଭାଙ୍ଗ;
		ସ୍ଥିତି LOFC_BLOB_NONE:
			ଯଦି (data->type == OBJ_BLOB) {
				ଯଦି (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ଫେରନ୍ତୁ;
			}
			ଭାଙ୍ଗ;
		ସ୍ଥିତି LOFC_BLOB_LIMIT:
			ଯଦି (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_ମୂଲ୍ୟ) {
				ଯଦି (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ଫେରନ୍ତୁ;
			}
			ଭାଙ୍ଗ;
		ସ୍ଥିତି LOFC_OBJECT_TYPE:
			ଯଦି (data->type != opt->objects_filter.object_type) {
				ଯଦି (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ଫେରନ୍ତୁ;
			}
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("unsupported objects filter");
		}

		ଯଦି (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			ଅକ୍ଷର *buf = ଖାଲି;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			ଯଦି (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			ମୁକ୍ତ(buf);
		}
	}

	strbuf_reset(scratch);

	ଯଦି (!opt->format) {
		print_default_format(scratch, data, opt);
	} ଅନ୍ୟଥା {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	ଯଦି (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ batch_one_object(ଅଚଳ ଅକ୍ଷର *obj_name,
			     ସଂରଚନା strbuf *scratch,
			     ସଂରଚନା batch_options *opt,
			     ସଂରଚନା expand_data *data)
{
	ସଂରଚନା object_context ctx = {0};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	ଗଣନା get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	ଯଦି (result != FOUND) {
		ବାଛ (result) {
		ସ୍ଥିତି MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			ଭାଙ୍ଗ;
		ସ୍ଥିତି SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			ଭାଙ୍ଗ;
		ସ୍ଥିତି DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			ଭାଙ୍ଗ;
		}
		fflush(stdout);

		ଯାଅ out;
	}

	ଯଦି (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		ଯାଅ out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, ଖାଲି, 0);

out:
	object_context_release(&ctx);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_remote_info(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			   ଅଚଳ ଅକ୍ଷର **argv,
			   ସଂରଚନା fetch_object_info_results *results,
			   ସଂରଚନା oid_array *object_info_oids)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 0;
	ସଂରଚନା remote *remote = ଖାଲି;
	ସଂରଚନା object_id oid;
	ସଂରଚନା transport *gtransport;

	remote = remote_get(argv[0]);
	ଯଦି (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	ପାଇଁ (size_t i = 1; i < argc; i++) {
		ଯଦି (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			ଯଦି (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				ପାଇଁ (j = 0; j < len; j++)
					ଯଦି (!isxdigit(argv[i][j]))
						ଭାଙ୍ଗ;
				ଯଦି (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	ଯଦି (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ଖାଲି);

	ଯଦି (!gtransport->smart_options) {
		retval = -1;
		ଯାଅ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ଫେରନ୍ତୁ retval;
}

ସଂରଚନା object_cb_data {
	ସଂରଚନା batch_options *opt;
	ସଂରଚନା expand_data *expand;
	ସଂରଚନା oidset *seen;
	ସଂରଚନା strbuf *scratch;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_object_cb(ଅଚଳ ସଂରଚନା object_id *oid, ଶୂନ୍ୟ *vdata)
{
	ସଂରଚନା object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(ଖାଲି, data->scratch, data->opt, data->expand,
			   ଖାଲି, 0);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_object(ଅଚଳ ସଂରଚନା object_id *oid,
			  ସଂରଚନା packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  ଶୂନ୍ୟ *data)
{
	oid_array_append(data, oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_unordered_object(ଅଚଳ ସଂରଚନା object_id *oid,
				  ସଂରଚନା packed_git *pack,
				  off_t offset,
				  ଶୂନ୍ୟ *vdata)
{
	ସଂରଚନା object_cb_data *data = vdata;

	ଯଦି (oidset_ଭର୍ତ୍ତି(data->seen, oid))
		ଫେରନ୍ତୁ 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(ଖାଲି, data->scratch, data->opt, data->expand,
			   pack, offset);
	ଫେରନ୍ତୁ 0;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*parse_cmd_fn_t)(ସଂରଚନା batch_options *, ଅଚଳ ଅକ୍ଷର *,
			       ସଂରଚନା strbuf *, ସଂରଚନା expand_data *);

ସଂରଚନା queued_cmd {
	parse_cmd_fn_t fn;
	ଅକ୍ଷର *ଧାଡ଼ି;
};

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_contents(ସଂରଚନା batch_options *opt,
			     ଅଚଳ ଅକ୍ଷର *ଧାଡ଼ି,
			     ସଂରଚନା strbuf *output,
			     ସଂରଚନା expand_data *data)
{
	ଗଣନା object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(ଧାଡ଼ି, output, opt, data);
	data->info.typep = saved_typep;
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_info(ସଂରଚନା batch_options *opt,
			   ଅଚଳ ଅକ୍ଷର *ଧାଡ଼ି,
			   ସଂରଚନା strbuf *output,
			   ସଂରଚନା expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(ଧାଡ଼ି, output, opt, data);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_mailmap(ସଂରଚନା batch_options *opt UNUSED,
			      ଅଚଳ ଅକ୍ଷର *ଧାଡ଼ି,
			      ସଂରଚନା strbuf *output UNUSED,
			      ସଂରଚନା expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(ଧାଡ଼ି);

	ଯଦି (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), ଧାଡ଼ି);

	ଯଦି (use_mailmap)
		load_mailmap();
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_remote_object_info(ସଂରଚନା batch_options *opt,
					 ଅଚଳ ଅକ୍ଷର *ଧାଡ଼ି, ସଂରଚନା strbuf *output,
					 ସଂରଚନା expand_data *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;
	ଅଚଳ ଅକ୍ଷର **argv;
	ଅକ୍ଷର *ଧାଡ଼ି_to_split;
	ସଂରଚନା fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	ସଂରଚନା oid_array object_info_oids = OID_ARRAY_INIT;

	ଯଦି (strlen(ଧାଡ଼ି) >= MAX_REMOTE_OBJ_INFO_ଧାଡ଼ି)
		die(_("remote-object-info command too long"));

	ଧାଡ଼ି_to_split = xstrdup(ଧାଡ଼ି);
	ଗଣନା_2 = split_cmdline(ଧାଡ଼ି_to_split, &argv);
	ଯଦି (ଗଣନା_2 < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(ଗଣନା_2));
	ଯଦି (ଗଣନା_2 - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	ଯଦି (data->info.sizep)
		results.wants_size = 1;
	ଯଦି (data->info.typep)
		results.wants_type = 1;

	ଯଦି (get_remote_info(ଗଣନା_2, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	ଯଦି (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	ଯଦି (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	ପାଇଁ (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		ଯଦି (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			ଜାରି;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		ଯଦି (results.sizes)
			data->size = results.sizes[i];

		ଯଦି (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, ଖାଲି, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	ମୁକ୍ତ(ଧାଡ଼ି_to_split);
	ମୁକ୍ତ(argv);
	oid_array_clear(&object_info_oids);
}

ସ୍ଥିର ଶୂନ୍ୟ dispatch_calls(ସଂରଚନା batch_options *opt,
		ସଂରଚନା strbuf *output,
		ସଂରଚନା expand_data *data,
		ସଂରଚନା queued_cmd *cmd,
		size_t nr)
{
	ଯଦି (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	ପାଇଁ (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].ଧାଡ଼ି, output, data);

	fflush(stdout);
}

ସ୍ଥିର ଶୂନ୍ୟ free_cmds(ସଂରଚନା queued_cmd *cmd, size_t *nr)
{
	ପାଇଁ (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].ଧାଡ଼ି);

	*nr = 0;
}


ସ୍ଥିର ଅଚଳ ସଂରଚନା parse_cmd {
	ଅଚଳ ଅକ୍ଷର *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ଖାଲି, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ସ୍ଥିର ଶୂନ୍ୟ batch_objects_command(ସଂରଚନା batch_options *opt,
				    ସଂରଚନା strbuf *output,
				    ସଂରଚନା expand_data *data)
{
	ସଂରଚନା strbuf ନିବେଶ = STRBUF_INIT;
	ସଂରଚନା queued_cmd *queued_cmd = ଖାଲି;
	size_t alloc = 0, nr = 0;

	ଯେତେବେଳେ (strbuf_getdelim_strip_crlf(&ନିବେଶ, stdin, opt->ନିବେଶ_delim) != EOF) {
		ଅଚଳ ସଂରଚନା parse_cmd *cmd = ଖାଲି;
		ଅଚଳ ଅକ୍ଷର *p = ଖାଲି, *cmd_end;
		ସଂରଚନା queued_cmd call = {0};

		ଯଦି (!ନିବେଶ.len)
			die(_("empty command in input"));
		ଯଦି (isspace(*ନିବେଶ.buf))
			die(_("whitespace before command: '%s'"), ନିବେଶ.buf);

		ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			ଯଦି (!skip_prefix(ନିବେଶ.buf, commands[i].name, &cmd_end))
				ଜାରି;

			cmd = &commands[i];
			ଯଦି (cmd->takes_args) {
				ଯଦି (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} ଅନ୍ୟଥା ଯଦି (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			ଭାଙ୍ଗ;
		}

		ଯଦି (!cmd)
			die(_("unknown command: '%s'"), ନିବେଶ.buf);

		ଯଦି (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} ଅନ୍ୟଥା ଯଦି (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} ଅନ୍ୟଥା {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.ଧାଡ଼ି = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	ଯଦି (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	ମୁକ୍ତ(queued_cmd);
	strbuf_release(&ନିବେଶ);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*for_each_object_fn)(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା packed_git *pack,
				  off_t offset, ଶୂନ୍ୟ *data);

ସଂରଚନା for_each_object_payload {
	for_each_object_fn callback;
	ଶୂନ୍ୟ *payload;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_one_object_oi(ଅଚଳ ସଂରଚନା object_id *oid,
			       ସଂରଚନା object_info *oi,
			       ଶୂନ୍ୟ *_payload)
{
	ସଂରଚନା for_each_object_payload *payload = _payload;
	ଯଦି (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		ଫେରନ୍ତୁ payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	ଫେରନ୍ତୁ payload->callback(oid, ଖାଲି, 0, payload->payload);
}

ସ୍ଥିର ଶୂନ୍ୟ batch_each_object(ସଂରଚନା batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      ଶୂନ୍ୟ *_payload)
{
	ସଂରଚନା for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	ସଂରଚନା odb_source_info source_info;
	ସଂରଚନା object_info oi = {
		.source_infop = &source_info,
	};
	ସଂରଚନା odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_objects(ସଂରଚନା batch_options *opt)
{
	ସଂରଚନା strbuf ନିବେଶ = STRBUF_INIT;
	ସଂରଚନା strbuf output = STRBUF_INIT;
	ସଂରଚନା expand_data data = EXPAND_DATA_INIT;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_warning;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 0;

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
	ଯଦି (opt->transform_mode)
		data.split_on_whitespace = 1;

	ଯଦି (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ଖାଲି;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	ଯଦି (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	ଯଦି (opt->all_objects) {
		ସଂରଚନା object_cb_data cb;
		ସଂରଚନା object_info empty = OBJECT_INFO_INIT;

		ଯଦି (!memcmp(&data.info, &empty, ଆକାର(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		ଯଦି (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		ଯଦି (opt->unordered) {
			ସଂରଚନା oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} ଅନ୍ୟଥା {
			ସଂରଚନା oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ଫେରନ୍ତୁ 0;
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

	ଯଦି (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		ଯାଅ cleanup;
	}

	ଯେତେବେଳେ (strbuf_getdelim_strip_crlf(&ନିବେଶ, stdin, opt->ନିବେଶ_delim) != EOF) {
		ଯଦି (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			ଅକ୍ଷର *p = strpbrk(ନିବେଶ.buf, " \t");
			ଯଦି (p) {
				ଯେତେବେଳେ (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			data.rest = p;
		}

		batch_one_object(ନିବେଶ.buf, &output, opt, &data);
	}

 cleanup:
	strbuf_release(&ନିବେଶ);
	strbuf_release(&output);
	string_list_clear(&data.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_cat_file_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			       ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (userdiff_config(var, ମୂଲ୍ୟ) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_option_callback(ଅଚଳ ସଂରଚନା option *opt,
				 ଅଚଳ ଅକ୍ଷର *arg,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା batch_options *bo = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (bo->enabled) {
		ଫେରନ୍ତୁ error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	ଯଦି (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	ଅନ୍ୟଥା ଯଦି (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	ଅନ୍ୟଥା ଯଦି (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	ଅନ୍ୟଥା
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_cat_file(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_cw = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_epts = 0;
	ଅଚଳ ଅକ୍ଷର *exp_type = ଖାଲି, *obj_name = ଖାଲି;
	ସଂରଚନା batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା unknown_type = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_nul_terminated = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nul_terminated = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ଖାଲି
	};
	ଅଚଳ ସଂରଚନା option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ଖାଲି, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ଖାଲି, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ଖାଲି, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ଖାଲି, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', ଖାଲି, &ନିବେଶ_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ଖାଲି, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, ଖାଲି);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	ଯଦି (use_mailmap)
		load_mailmap();

	ବାଛ (batch.objects_filter.choice) {
	ସ୍ଥିତି LOFC_DISABLED:
		ଭାଙ୍ଗ;
	ସ୍ଥିତି LOFC_BLOB_NONE:
	ସ୍ଥିତି LOFC_BLOB_LIMIT:
	ସ୍ଥିତି LOFC_OBJECT_TYPE:
		ଯଦି (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	ଯଦି (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	ଯଦି (force_path && !opt_cw)
		usage_ସନ୍ଦେଶ_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	ଯଦି (batch.enabled)
		;
	ଅନ୍ୟଥା ଯଦି (batch.follow_symlinks)
		usage_ସନ୍ଦେଶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	ଅନ୍ୟଥା ଯଦି (batch.buffer_output >= 0)
		usage_ସନ୍ଦେଶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	ଅନ୍ୟଥା ଯଦି (batch.all_objects)
		usage_ସନ୍ଦେଶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	ଅନ୍ୟଥା ଯଦି (ନିବେଶ_nul_terminated)
		usage_ସନ୍ଦେଶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	ଅନ୍ୟଥା ଯଦି (nul_terminated)
		usage_ସନ୍ଦେଶ_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.ନିବେଶ_delim = batch.output_delim = '\n';
	ଯଦି (ନିବେଶ_nul_terminated)
		batch.ନିବେଶ_delim = '\0';
	ଯଦି (nul_terminated)
		batch.ନିବେଶ_delim = batch.output_delim = '\0';

	/* Batch defaults */
	ଯଦି (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	ଯଦି (batch.enabled) {
		ଯଦି (opt_cw)
			batch.transform_mode = opt;
		ଅନ୍ୟଥା ଯଦି (opt && opt != 'b')
			usage_ସନ୍ଦେଶ_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		ଅନ୍ୟଥା ଯଦି (argc)
			usage_ସନ୍ଦେଶ_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		ଯାଅ out;
	}

	ଯଦି (opt) {
		ଯଦି (!argc && opt == 'c')
			usage_ସନ୍ଦେଶ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		ଅନ୍ୟଥା ଯଦି (!argc && opt == 'w')
			usage_ସନ୍ଦେଶ_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		ଅନ୍ୟଥା ଯଦି (!argc && opt_epts)
			usage_ସନ୍ଦେଶ_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		ଅନ୍ୟଥା ଯଦି (argc == 1)
			obj_name = argv[0];
		ଅନ୍ୟଥା
			usage_ସନ୍ଦେଶ_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} ଅନ୍ୟଥା ଯଦି (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} ଅନ୍ୟଥା ଯଦି (argc != 2) {
		usage_ସନ୍ଦେଶ_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} ଅନ୍ୟଥା ଯଦି (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	ଫେରନ୍ତୁ ret;
}
