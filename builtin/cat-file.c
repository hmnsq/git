/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "convert.h"
#include "diff.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "ident.h"
#include "list-objects-filter-options.h"
#include "parse-options.h"
#include "userdiff.h"
#include "oid-array.h"
#include "packfile.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "odb/streaming.h"
#include "replace-object.h"
#include "promisor-remote.h"
#include "mailmap.h"
#include "write-or-die.h"
#include "alias.h"
#include "remote.h"
#include "transport.h"
#include "fetch-object-info.h"

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

ga_nanaa batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

sa_nracanaa batch_options {
	sa_nracanaa list_objects_filter_options objects_filter;
	ကိန်းပြည့် enabled;
	ကိန်းပြည့် follow_symlinks;
	ga_nanaa batch_mode batch_mode;
	ကိန်းပြည့် buffer_output;
	ကိန်းပြည့် all_objects;
	ကိန်းပြည့် unordered;
	ကိန်းပြည့် transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	var_na input_delim;
	var_na output_delim;
	const var_na *format;
};

static const var_na *force_path;

static sa_nracanaa string_list mailmap = STRING_LIST_INIT_NODUP;
static ကိန်းပြည့် use_mailmap;

static var_na *replace_idents_using_mailmap(var_na *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
static void load_mailmap(void)
{
	အကယ်၍ (mailmap.strdup_strings)
		ပြန်ပေး;

	read_mailmap(the_repository, &mailmap);
}

static var_na *replace_idents_using_mailmap(var_na *object_buf, size_t *size)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *headers[] = { "author ", "committer ", "tagger ", NULL };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	ပြန်ပေး strbuf_detach(&sb, NULL);
}

static ကိန်းပြည့် filter_object(const var_na *path, unsigned mode,
			 const sa_nracanaa object_id *oid,
			 var_na **buf, size_t *size)
{
	ga_nanaa object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	အကယ်၍ (!*buf)
		ပြန်ပေး error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	အကယ်၍ ((type == OBJ_BLOB) && S_ISREG(mode)) {
		sa_nracanaa strbuf strbuf = STRBUF_INIT;
		sa_nracanaa checkout_metadata meta;

		init_checkout_metadata(&meta, NULL, NULL, oid);
		အကယ်၍ (convert_to_working_tree(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			free(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, NULL);
		}
	}

	ပြန်ပေး 0;
}

static ကိန်းပြည့် stream_blob(const sa_nracanaa object_id *oid)
{
	အကယ်၍ (odb_stream_blob_to_fd(the_repository->objects, 1, oid, NULL, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	ပြန်ပေး 0;
}

static ကိန်းပြည့် cat_one_file(ကိန်းပြည့် opt, const var_na *exp_type, const var_na *obj_name)
{
	ကိန်းပြည့် ret;
	sa_nracanaa object_id oid;
	ga_nanaa object_type type;
	var_na *buf;
	size_t size;
	sa_nracanaa object_context obj_context = {0};
	sa_nracanaa object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	const var_na *path = force_path;
	const ကိန်းပြည့် opt_cw = (opt == 'c' || opt == 'w');
	အကယ်၍ (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	အကယ်၍ (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	အကယ်၍ (!path)
		path = obj_context.path;
	အကယ်၍ (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = NULL;
	switch (opt) {
	pak_sa 't':
		oi.typep = &type;
		အကယ်၍ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		goto cleanup;

	pak_sa 's':
		oi.sizep = &size;

		အကယ်၍ (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (void**)&buf;
		}

		အကယ်၍ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		အကယ်၍ (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		goto cleanup;

	pak_sa 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		goto cleanup;

	pak_sa 'w':

		အကယ်၍ (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			goto cleanup;
		}
		bha_nga;

	pak_sa 'c':
	{
		unsigned long size_ul = 0;
		ကိန်းပြည့် textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		အကယ်၍ (textconv_ret)
			bha_nga;
	}
		/* else fallthrough */

	pak_sa 'p':
		type = odb_read_object_info(the_repository->objects, &oid, NULL);
		အကယ်၍ (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		အကယ်၍ (type == OBJ_TREE) {
			const var_na *ls_args[3] = { NULL };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_tree(2, ls_args, NULL, the_repository);
			goto cleanup;
		}

		အကယ်၍ (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			goto cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		အကယ်၍ (!buf)
			die("Cannot read object %s", obj_name);

		အကယ်၍ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		bha_nga;

	pak_sa 0:
	{
		ga_nanaa object_type exp_type_id = type_from_string(exp_type);

		အကယ်၍ (exp_type_id == OBJ_BLOB) {
			sa_nracanaa object_id blob_oid;
			အကယ်၍ (odb_read_object_info(the_repository->objects,
						 &oid, NULL) == OBJ_TAG) {
				var_na *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				const var_na *target;

				အကယ်၍ (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				အကယ်၍ (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				free(buffer);
			} မဟုတ်ပါက
				oidcpy(&blob_oid, &oid);

			အကယ်၍ (odb_read_object_info(the_repository->objects,
						 &blob_oid, NULL) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				goto cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &size, NULL);

		အကယ်၍ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		bha_nga;
	}
	default:
		die("git cat-file: unknown option: %s", exp_type);
	}

	အကယ်၍ (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	free(buf);
	object_context_release(&obj_context);
	ပြန်ပေး ret;
}

sa_nracanaa expand_data {
	sa_nracanaa object_id oid;
	ga_nanaa object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	const var_na *rest;
	sa_nracanaa object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	ကိန်းပြည့် mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	ကိန်းပြည့် split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	sa_nracanaa object_info info;

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
	sa_nracanaa string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

static ကိန်းပြည့် is_atom(const var_na *atom, const var_na *s, ကိန်းပြည့် slen)
{
	ကိန်းပြည့် alen = strlen(atom);
	ပြန်ပေး alen == slen && !memcmp(atom, s, alen);
}

static ကိန်းပြည့် expand_atom(sa_nracanaa strbuf *sb, const var_na *atom, ကိန်းပြည့် len,
		       sa_nracanaa expand_data *data)
{
	အကယ်၍ (data->is_remote) {
		size_t i;
		အတွက် (i = 0; i < data->remote_allowed_atoms.nr; i++)
			အကယ်၍ (is_atom(data->remote_allowed_atoms.items[i].string,
				    atom, len))
				bha_nga;
		အကယ်၍ (i == data->remote_allowed_atoms.nr)
			ပြန်ပေး 1;
	}

	အကယ်၍ (is_atom("objectname", atom, len)) {
		အကယ်၍ (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} မဟုတ်ပါက အကယ်၍ (is_atom("objecttype", atom, len)) {
		အကယ်၍ (data->mark_query) {
			data->info.typep = &data->type;
		} မဟုတ်ပါက {
			const var_na *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} မဟုတ်ပါက အကယ်၍ (is_atom("objectsize", atom, len)) {
		အကယ်၍ (data->mark_query)
			data->info.sizep = &data->size;
		မဟုတ်ပါက
			strbuf_add_uint(sb, data->size);
	} မဟုတ်ပါက အကယ်၍ (is_atom("objectsize:disk", atom, len)) {
		အကယ်၍ (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		မဟုတ်ပါက
			strbuf_add_uint(sb, data->disk_size);
	} မဟုတ်ပါက အကယ်၍ (is_atom("rest", atom, len)) {
		အကယ်၍ (data->mark_query)
			data->split_on_whitespace = 1;
		မဟုတ်ပါက အကယ်၍ (data->rest)
			strbuf_addstr(sb, data->rest);
	} မဟုတ်ပါက အကယ်၍ (is_atom("deltabase", atom, len)) {
		အကယ်၍ (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		မဟုတ်ပါက
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} မဟုတ်ပါက အကယ်၍ (is_atom("objectmode", atom, len)) {
		အကယ်၍ (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} မဟုတ်ပါက
		ပြန်ပေး 0;
	ပြန်ပေး 1;
}

static void expand_format(sa_nracanaa strbuf *sb, const var_na *start,
			  sa_nracanaa expand_data *data)
{
	နေစဉ် (strbuf_expand_step(sb, &start)) {
		const var_na *end;

		အကယ်၍ (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		မဟုတ်ပါက အကယ်၍ ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		မဟုတ်ပါက
			strbuf_expand_bad_format(start, "cat-file");
	}
}

static void batch_write(sa_nracanaa batch_options *opt, const void *data, ကိန်းပြည့် len)
{
	အကယ်၍ (opt->buffer_output) {
		အကယ်၍ (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} မဟုတ်ပါက
		write_or_die(1, data, len);
}

static void print_object_or_die(sa_nracanaa batch_options *opt, sa_nracanaa expand_data *data)
{
	const sa_nracanaa object_id *oid = &data->oid;

	assert(data->info.typep);

	အကယ်၍ (data->type == OBJ_BLOB) {
		အကယ်၍ (opt->buffer_output)
			fflush(stdout);
		အကယ်၍ (opt->transform_mode) {
			var_na *contents;
			size_t size;

			အကယ်၍ (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			အကယ်၍ (opt->transform_mode == 'w') {
				အကယ်၍ (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} မဟုတ်ပါက အကယ်၍ (opt->transform_mode == 'c') {
				ga_nanaa object_type type;
				unsigned long size_ul = 0;
				အကယ်၍ (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				မဟုတ်ပါက
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				အကယ်၍ (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} မဟုတ်ပါက
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			free(contents);
		} မဟုတ်ပါက {
			stream_blob(oid);
		}
	}
	မဟုတ်ပါက {
		ga_nanaa object_type type;
		size_t size;
		void *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		အကယ်၍ (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		အကယ်၍ (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		အကယ်၍ (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		အကယ်၍ (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		free(contents);
	}
}

static void print_default_format(sa_nracanaa strbuf *scratch, sa_nracanaa expand_data *data,
				 sa_nracanaa batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

static void report_object_status(sa_nracanaa batch_options *opt,
				 const var_na *obj_name,
				 const sa_nracanaa object_id *oid,
				 const var_na *status)
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
static void batch_object_write(const var_na *obj_name,
			       sa_nracanaa strbuf *scratch,
			       sa_nracanaa batch_options *opt,
			       sa_nracanaa expand_data *data,
			       sa_nracanaa packed_git *pack,
			       off_t offset)
{
	အကယ်၍ (!data->skip_object_info) {
		ကိန်းပြည့် ret;

		အကယ်၍ (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		အကယ်၍ (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		အကယ်၍ (pack)
			ret = packed_object_info(NULL, pack, offset, &data->info);
		မဟုတ်ပါက
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		အကယ်၍ (ret < 0) {
			အကယ်၍ (data->mode == S_IFGITLINK)
				report_object_status(opt, NULL, &data->oid, "submodule");
			မဟုတ်ပါက
				report_object_status(opt, obj_name, &data->oid, "missing");
			ပြန်ပေး;
		}

		switch (opt->objects_filter.choice) {
		pak_sa LOFC_DISABLED:
			bha_nga;
		pak_sa LOFC_BLOB_NONE:
			အကယ်၍ (data->type == OBJ_BLOB) {
				အကယ်၍ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ပြန်ပေး;
			}
			bha_nga;
		pak_sa LOFC_BLOB_LIMIT:
			အကယ်၍ (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_value) {
				အကယ်၍ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ပြန်ပေး;
			}
			bha_nga;
		pak_sa LOFC_OBJECT_TYPE:
			အကယ်၍ (data->type != opt->objects_filter.object_type) {
				အကယ်၍ (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				ပြန်ပေး;
			}
			bha_nga;
		default:
			BUG("unsupported objects filter");
		}

		အကယ်၍ (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			var_na *buf = NULL;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			အကယ်၍ (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			free(buf);
		}
	}

	strbuf_reset(scratch);

	အကယ်၍ (!opt->format) {
		print_default_format(scratch, data, opt);
	} မဟုတ်ပါက {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	အကယ်၍ (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

static void batch_one_object(const var_na *obj_name,
			     sa_nracanaa strbuf *scratch,
			     sa_nracanaa batch_options *opt,
			     sa_nracanaa expand_data *data)
{
	sa_nracanaa object_context ctx = {0};
	ကိန်းပြည့် flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	ga_nanaa get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	အကယ်၍ (result != FOUND) {
		switch (result) {
		pak_sa MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			bha_nga;
		pak_sa SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			bha_nga;
		pak_sa DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			bha_nga;
		pak_sa SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			bha_nga;
		pak_sa NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			bha_nga;
		default:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			bha_nga;
		}
		fflush(stdout);

		goto out;
	}

	အကယ်၍ (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		goto out;
	}

	data->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, data, NULL, 0);

out:
	object_context_release(&ctx);
}

static ကိန်းပြည့် get_remote_info(ကိန်းပြည့် argc,
			   const var_na **argv,
			   sa_nracanaa fetch_object_info_results *results,
			   sa_nracanaa oid_array *object_info_oids)
{
	ကိန်းပြည့် retval = 0;
	sa_nracanaa remote *remote = NULL;
	sa_nracanaa object_id oid;
	sa_nracanaa transport *gtransport;

	remote = remote_get(argv[0]);
	အကယ်၍ (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	အတွက် (size_t i = 1; i < argc; i++) {
		အကယ်၍ (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			အကယ်၍ (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				အတွက် (j = 0; j < len; j++)
					အကယ်၍ (!isxdigit(argv[i][j]))
						bha_nga;
				အကယ်၍ (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (ကိန်းပြည့်)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	အကယ်၍ (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, NULL);

	အကယ်၍ (!gtransport->smart_options) {
		retval = -1;
		goto cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ပြန်ပေး retval;
}

sa_nracanaa object_cb_data {
	sa_nracanaa batch_options *opt;
	sa_nracanaa expand_data *expand;
	sa_nracanaa oidset *seen;
	sa_nracanaa strbuf *scratch;
};

static ကိန်းပြည့် batch_object_cb(const sa_nracanaa object_id *oid, void *vdata)
{
	sa_nracanaa object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(NULL, data->scratch, data->opt, data->expand,
			   NULL, 0);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် collect_object(const sa_nracanaa object_id *oid,
			  sa_nracanaa packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  void *data)
{
	oid_array_append(data, oid);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် batch_unordered_object(const sa_nracanaa object_id *oid,
				  sa_nracanaa packed_git *pack,
				  off_t offset,
				  void *vdata)
{
	sa_nracanaa object_cb_data *data = vdata;

	အကယ်၍ (oidset_insert(data->seen, oid))
		ပြန်ပေး 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(NULL, data->scratch, data->opt, data->expand,
			   pack, offset);
	ပြန်ပေး 0;
}

typedef void (*parse_cmd_fn_t)(sa_nracanaa batch_options *, const var_na *,
			       sa_nracanaa strbuf *, sa_nracanaa expand_data *);

sa_nracanaa queued_cmd {
	parse_cmd_fn_t fn;
	var_na *line;
};

static void parse_cmd_contents(sa_nracanaa batch_options *opt,
			     const var_na *line,
			     sa_nracanaa strbuf *output,
			     sa_nracanaa expand_data *data)
{
	ga_nanaa object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(line, output, opt, data);
	data->info.typep = saved_typep;
}

static void parse_cmd_info(sa_nracanaa batch_options *opt,
			   const var_na *line,
			   sa_nracanaa strbuf *output,
			   sa_nracanaa expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(line, output, opt, data);
}

static void parse_cmd_mailmap(sa_nracanaa batch_options *opt UNUSED,
			      const var_na *line,
			      sa_nracanaa strbuf *output UNUSED,
			      sa_nracanaa expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(line);

	အကယ်၍ (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), line);

	အကယ်၍ (use_mailmap)
		load_mailmap();
}

static void parse_cmd_remote_object_info(sa_nracanaa batch_options *opt,
					 const var_na *line, sa_nracanaa strbuf *output,
					 sa_nracanaa expand_data *data)
{
	ကိန်းပြည့် count;
	const var_na **argv;
	var_na *line_to_split;
	sa_nracanaa fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	sa_nracanaa oid_array object_info_oids = OID_ARRAY_INIT;

	အကယ်၍ (strlen(line) >= MAX_REMOTE_OBJ_INFO_LINE)
		die(_("remote-object-info command too long"));

	line_to_split = xstrdup(line);
	count = split_cmdline(line_to_split, &argv);
	အကယ်၍ (count < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(count));
	အကယ်၍ (count - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	အကယ်၍ (data->info.sizep)
		results.wants_size = 1;
	အကယ်၍ (data->info.typep)
		results.wants_type = 1;

	အကယ်၍ (get_remote_info(count, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	အကယ်၍ (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	အကယ်၍ (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	အတွက် (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		အကယ်၍ (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			continue;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		အကယ်၍ (results.sizes)
			data->size = results.sizes[i];

		အကယ်၍ (results.types)
			data->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		data->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, data, NULL, 0);
		data->is_remote = 0;
	}
	data->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	free(line_to_split);
	free(argv);
	oid_array_clear(&object_info_oids);
}

static void dispatch_calls(sa_nracanaa batch_options *opt,
		sa_nracanaa strbuf *output,
		sa_nracanaa expand_data *data,
		sa_nracanaa queued_cmd *cmd,
		size_t nr)
{
	အကယ်၍ (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	အတွက် (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].line, output, data);

	fflush(stdout);
}

static void free_cmds(sa_nracanaa queued_cmd *cmd, size_t *nr)
{
	အတွက် (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].line);

	*nr = 0;
}


static const sa_nracanaa parse_cmd {
	const var_na *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", NULL, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

static void batch_objects_command(sa_nracanaa batch_options *opt,
				    sa_nracanaa strbuf *output,
				    sa_nracanaa expand_data *data)
{
	sa_nracanaa strbuf input = STRBUF_INIT;
	sa_nracanaa queued_cmd *queued_cmd = NULL;
	size_t alloc = 0, nr = 0;

	နေစဉ် (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		const sa_nracanaa parse_cmd *cmd = NULL;
		const var_na *p = NULL, *cmd_end;
		sa_nracanaa queued_cmd call = {0};

		အကယ်၍ (!input.len)
			die(_("empty command in input"));
		အကယ်၍ (isspace(*input.buf))
			die(_("whitespace before command: '%s'"), input.buf);

		အတွက် (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			အကယ်၍ (!skip_prefix(input.buf, commands[i].name, &cmd_end))
				continue;

			cmd = &commands[i];
			အကယ်၍ (cmd->takes_args) {
				အကယ်၍ (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} မဟုတ်ပါက အကယ်၍ (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			bha_nga;
		}

		အကယ်၍ (!cmd)
			die(_("unknown command: '%s'"), input.buf);

		အကယ်၍ (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} မဟုတ်ပါက အကယ်၍ (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} မဟုတ်ပါက {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.line = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	အကယ်၍ (opt->buffer_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, data, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	free(queued_cmd);
	strbuf_release(&input);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

typedef ကိန်းပြည့် (*for_each_object_fn)(const sa_nracanaa object_id *oid, sa_nracanaa packed_git *pack,
				  off_t offset, void *data);

sa_nracanaa for_each_object_payload {
	for_each_object_fn callback;
	void *payload;
};

static ကိန်းပြည့် batch_one_object_oi(const sa_nracanaa object_id *oid,
			       sa_nracanaa object_info *oi,
			       void *_payload)
{
	sa_nracanaa for_each_object_payload *payload = _payload;
	အကယ်၍ (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		ပြန်ပေး payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	ပြန်ပေး payload->callback(oid, NULL, 0, payload->payload);
}

static void batch_each_object(sa_nracanaa batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      void *_payload)
{
	sa_nracanaa for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	sa_nracanaa odb_source_info source_info;
	sa_nracanaa object_info oi = {
		.source_infop = &source_info,
	};
	sa_nracanaa odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

static ကိန်းပြည့် batch_objects(sa_nracanaa batch_options *opt)
{
	sa_nracanaa strbuf input = STRBUF_INIT;
	sa_nracanaa strbuf output = STRBUF_INIT;
	sa_nracanaa expand_data data = EXPAND_DATA_INIT;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);
	ကိန်းပြည့် save_warning;
	ကိန်းပြည့် retval = 0;

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
	အကယ်၍ (opt->transform_mode)
		data.split_on_whitespace = 1;

	အကယ်၍ (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = NULL;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	အကယ်၍ (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	အကယ်၍ (opt->all_objects) {
		sa_nracanaa object_cb_data cb;
		sa_nracanaa object_info empty = OBJECT_INFO_INIT;

		အကယ်၍ (!memcmp(&data.info, &empty, parimaa_na(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		အကယ်၍ (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		အကယ်၍ (opt->unordered) {
			sa_nracanaa oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} မဟုတ်ပါက {
			sa_nracanaa oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ပြန်ပေး 0;
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

	အကယ်၍ (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		goto cleanup;
	}

	နေစဉ် (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		အကယ်၍ (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			var_na *p = strpbrk(input.buf, " \t");
			အကယ်၍ (p) {
				နေစဉ် (*p && strchr(" \t", *p))
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
	ပြန်ပေး retval;
}

static ကိန်းပြည့် git_cat_file_config(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx, void *cb)
{
	အကယ်၍ (userdiff_config(var, value) < 0)
		ပြန်ပေး -1;

	ပြန်ပေး git_default_config(var, value, ctx, cb);
}

static ကိန်းပြည့် batch_option_callback(const sa_nracanaa option *opt,
				 const var_na *arg,
				 ကိန်းပြည့် unset)
{
	sa_nracanaa batch_options *bo = opt->value;

	BUG_ON_OPT_NEG(unset);

	အကယ်၍ (bo->enabled) {
		ပြန်ပေး error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	အကယ်၍ (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	မဟုတ်ပါက အကယ်၍ (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	မဟုတ်ပါက အကယ်၍ (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	မဟုတ်ပါက
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	ပြန်ပေး 0;
}

ကိန်းပြည့် cmd_cat_file(ကိန်းပြည့် argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် opt = 0;
	ကိန်းပြည့် opt_cw = 0;
	ကိန်းပြည့် opt_epts = 0;
	const var_na *exp_type = NULL, *obj_name = NULL;
	sa_nracanaa batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	ကိန်းပြည့် unknown_type = 0;
	ကိန်းပြည့် input_nul_terminated = 0;
	ကိန်းပြည့် nul_terminated = 0;
	ကိန်းပြည့် ret;

	const var_na * const builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		NULL
	};
	const sa_nracanaa option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', NULL, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', NULL, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', NULL, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', NULL, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', NULL, &input_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', NULL, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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

	repo_config(the_repository, git_cat_file_config, NULL);

	batch.buffer_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	အကယ်၍ (use_mailmap)
		load_mailmap();

	switch (batch.objects_filter.choice) {
	pak_sa LOFC_DISABLED:
		bha_nga;
	pak_sa LOFC_BLOB_NONE:
	pak_sa LOFC_BLOB_LIMIT:
	pak_sa LOFC_OBJECT_TYPE:
		အကယ်၍ (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		bha_nga;
	default:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	အကယ်၍ (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	အကယ်၍ (force_path && !opt_cw)
		usage_msg_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	အကယ်၍ (batch.enabled)
		;
	မဟုတ်ပါက အကယ်၍ (batch.follow_symlinks)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	မဟုတ်ပါက အကယ်၍ (batch.buffer_output >= 0)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	မဟုတ်ပါက အကယ်၍ (batch.all_objects)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	မဟုတ်ပါက အကယ်၍ (input_nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	မဟုတ်ပါက အကယ်၍ (nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.input_delim = batch.output_delim = '\n';
	အကယ်၍ (input_nul_terminated)
		batch.input_delim = '\0';
	အကယ်၍ (nul_terminated)
		batch.input_delim = batch.output_delim = '\0';

	/* Batch defaults */
	အကယ်၍ (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	အကယ်၍ (batch.enabled) {
		အကယ်၍ (opt_cw)
			batch.transform_mode = opt;
		မဟုတ်ပါက အကယ်၍ (opt && opt != 'b')
			usage_msg_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		မဟုတ်ပါက အကယ်၍ (argc)
			usage_msg_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		goto out;
	}

	အကယ်၍ (opt) {
		အကယ်၍ (!argc && opt == 'c')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		မဟုတ်ပါက အကယ်၍ (!argc && opt == 'w')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		မဟုတ်ပါက အကယ်၍ (!argc && opt_epts)
			usage_msg_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		မဟုတ်ပါက အကယ်၍ (argc == 1)
			obj_name = argv[0];
		မဟုတ်ပါက
			usage_msg_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} မဟုတ်ပါက အကယ်၍ (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} မဟုတ်ပါက အကယ်၍ (argc != 2) {
		usage_msg_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} မဟုတ်ပါက အကယ်၍ (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	ပြန်ပေး ret;
}
