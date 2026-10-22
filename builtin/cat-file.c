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

enum batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

struct batch_options {
	struct list_objects_filter_options objects_filter;
	số_nguyên enabled;
	số_nguyên follow_symlinks;
	enum batch_mode batch_mode;
	số_nguyên buffer_output;
	số_nguyên all_objects;
	số_nguyên unordered;
	số_nguyên transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	char input_delim;
	char output_delim;
	const char *format;
};

static const char *force_path;

static struct string_list mailmap = STRING_LIST_INIT_NODUP;
static số_nguyên use_mailmap;

static char *replace_idents_using_mailmap(char *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
static void load_mailmap(void)
{
	nếu (mailmap.strdup_strings)
		trả_về;

	read_mailmap(the_repository, &mailmap);
}

static char *replace_idents_using_mailmap(char *object_buf, size_t *size)
{
	struct strbuf sb = STRBUF_INIT;
	const char *headers[] = { "author ", "committer ", "tagger ", NULL };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	trả_về strbuf_detach(&sb, NULL);
}

static số_nguyên filter_object(const char *path, unsigned mode,
			 const struct object_id *oid,
			 char **buf, size_t *size)
{
	enum object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	nếu (!*buf)
		trả_về error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	nếu ((type == OBJ_BLOB) && S_ISREG(mode)) {
		struct strbuf strbuf = STRBUF_INIT;
		struct checkout_metadata meta;

		init_checkout_metadata(&meta, NULL, NULL, oid);
		nếu (convert_to_working_tree(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			free(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, NULL);
		}
	}

	trả_về 0;
}

static số_nguyên stream_blob(const struct object_id *oid)
{
	nếu (odb_stream_blob_to_fd(the_repository->objects, 1, oid, NULL, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	trả_về 0;
}

static số_nguyên cat_one_file(số_nguyên opt, const char *exp_type, const char *obj_name)
{
	số_nguyên ret;
	struct object_id oid;
	enum object_type type;
	char *buf;
	size_t size;
	struct object_context obj_context = {0};
	struct object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	const char *path = force_path;
	const số_nguyên opt_cw = (opt == 'c' || opt == 'w');
	nếu (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	nếu (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	nếu (!path)
		path = obj_context.path;
	nếu (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = NULL;
	switch (opt) {
	case 't':
		oi.typep = &type;
		nếu (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		goto cleanup;

	case 's':
		oi.sizep = &size;

		nếu (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (void**)&buf;
		}

		nếu (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		nếu (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		goto cleanup;

	case 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		goto cleanup;

	case 'w':

		nếu (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			goto cleanup;
		}
		break;

	case 'c':
	{
		unsigned long size_ul = 0;
		số_nguyên textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		nếu (textconv_ret)
			break;
	}
		/* else fallthrough */

	case 'p':
		type = odb_read_object_info(the_repository->objects, &oid, NULL);
		nếu (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		nếu (type == OBJ_TREE) {
			const char *ls_args[3] = { NULL };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_tree(2, ls_args, NULL, the_repository);
			goto cleanup;
		}

		nếu (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			goto cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		nếu (!buf)
			die("Cannot read object %s", obj_name);

		nếu (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		break;

	case 0:
	{
		enum object_type exp_type_id = type_from_string(exp_type);

		nếu (exp_type_id == OBJ_BLOB) {
			struct object_id blob_oid;
			nếu (odb_read_object_info(the_repository->objects,
						 &oid, NULL) == OBJ_TAG) {
				char *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				const char *target;

				nếu (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				nếu (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				free(buffer);
			} không_thì
				oidcpy(&blob_oid, &oid);

			nếu (odb_read_object_info(the_repository->objects,
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

		nếu (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		break;
	}
	default:
		die("git cat-file: unknown option: %s", exp_type);
	}

	nếu (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	free(buf);
	object_context_release(&obj_context);
	trả_về ret;
}

struct expand_data {
	struct object_id oid;
	enum object_type type;
	size_t size;
	unsigned short mode;
	off_t disk_size;
	const char *rest;
	struct object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	số_nguyên mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	số_nguyên split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	struct object_info info;

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
	struct string_list remote_allowed_atoms;
};

#define EXPAND_DATA_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

static số_nguyên is_atom(const char *atom, const char *s, số_nguyên slen)
{
	số_nguyên alen = strlen(atom);
	trả_về alen == slen && !memcmp(atom, s, alen);
}

static số_nguyên expand_atom(struct strbuf *sb, const char *atom, số_nguyên len,
		       struct expand_data *data)
{
	nếu (data->is_remote) {
		size_t i;
		cho (i = 0; i < data->remote_allowed_atoms.nr; i++)
			nếu (is_atom(data->remote_allowed_atoms.items[i].string,
				    atom, len))
				break;
		nếu (i == data->remote_allowed_atoms.nr)
			trả_về 1;
	}

	nếu (is_atom("objectname", atom, len)) {
		nếu (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} không_thì nếu (is_atom("objecttype", atom, len)) {
		nếu (data->mark_query) {
			data->info.typep = &data->type;
		} không_thì {
			const char *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} không_thì nếu (is_atom("objectsize", atom, len)) {
		nếu (data->mark_query)
			data->info.sizep = &data->size;
		không_thì
			strbuf_add_uint(sb, data->size);
	} không_thì nếu (is_atom("objectsize:disk", atom, len)) {
		nếu (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		không_thì
			strbuf_add_uint(sb, data->disk_size);
	} không_thì nếu (is_atom("rest", atom, len)) {
		nếu (data->mark_query)
			data->split_on_whitespace = 1;
		không_thì nếu (data->rest)
			strbuf_addstr(sb, data->rest);
	} không_thì nếu (is_atom("deltabase", atom, len)) {
		nếu (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		không_thì
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} không_thì nếu (is_atom("objectmode", atom, len)) {
		nếu (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} không_thì
		trả_về 0;
	trả_về 1;
}

static void expand_format(struct strbuf *sb, const char *start,
			  struct expand_data *data)
{
	trong_khi (strbuf_expand_step(sb, &start)) {
		const char *end;

		nếu (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		không_thì nếu ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		không_thì
			strbuf_expand_bad_format(start, "cat-file");
	}
}

static void batch_write(struct batch_options *opt, const void *data, số_nguyên len)
{
	nếu (opt->buffer_output) {
		nếu (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} không_thì
		write_or_die(1, data, len);
}

static void print_object_or_die(struct batch_options *opt, struct expand_data *data)
{
	const struct object_id *oid = &data->oid;

	assert(data->info.typep);

	nếu (data->type == OBJ_BLOB) {
		nếu (opt->buffer_output)
			fflush(stdout);
		nếu (opt->transform_mode) {
			char *contents;
			size_t size;

			nếu (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			nếu (opt->transform_mode == 'w') {
				nếu (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} không_thì nếu (opt->transform_mode == 'c') {
				enum object_type type;
				unsigned long size_ul = 0;
				nếu (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				không_thì
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				nếu (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} không_thì
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			free(contents);
		} không_thì {
			stream_blob(oid);
		}
	}
	không_thì {
		enum object_type type;
		size_t size;
		void *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		nếu (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		nếu (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		nếu (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		nếu (data->info.sizep && size != data->size && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, size);
		free(contents);
	}
}

static void print_default_format(struct strbuf *scratch, struct expand_data *data,
				 struct batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &data->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(data->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, data->size);
	strbuf_addch(scratch, opt->output_delim);
}

static void report_object_status(struct batch_options *opt,
				 const char *obj_name,
				 const struct object_id *oid,
				 const char *status)
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
static void batch_object_write(const char *obj_name,
			       struct strbuf *scratch,
			       struct batch_options *opt,
			       struct expand_data *data,
			       struct packed_git *pack,
			       off_t offset)
{
	nếu (!data->skip_object_info) {
		số_nguyên ret;

		nếu (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		nếu (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		nếu (pack)
			ret = packed_object_info(NULL, pack, offset, &data->info);
		không_thì
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		nếu (ret < 0) {
			nếu (data->mode == S_IFGITLINK)
				report_object_status(opt, NULL, &data->oid, "submodule");
			không_thì
				report_object_status(opt, obj_name, &data->oid, "missing");
			trả_về;
		}

		switch (opt->objects_filter.choice) {
		case LOFC_DISABLED:
			break;
		case LOFC_BLOB_NONE:
			nếu (data->type == OBJ_BLOB) {
				nếu (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				trả_về;
			}
			break;
		case LOFC_BLOB_LIMIT:
			nếu (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_value) {
				nếu (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				trả_về;
			}
			break;
		case LOFC_OBJECT_TYPE:
			nếu (data->type != opt->objects_filter.object_type) {
				nếu (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				trả_về;
			}
			break;
		default:
			BUG("unsupported objects filter");
		}

		nếu (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			char *buf = NULL;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			nếu (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			free(buf);
		}
	}

	strbuf_reset(scratch);

	nếu (!opt->format) {
		print_default_format(scratch, data, opt);
	} không_thì {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	nếu (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, data);
		batch_write(opt, &opt->output_delim, 1);
	}
}

static void batch_one_object(const char *obj_name,
			     struct strbuf *scratch,
			     struct batch_options *opt,
			     struct expand_data *data)
{
	struct object_context ctx = {0};
	số_nguyên flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	enum get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	nếu (result != FOUND) {
		switch (result) {
		case MISSING_OBJECT:
			report_object_status(opt, obj_name, &data->oid, "missing");
			break;
		case SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &data->oid, "ambiguous");
			break;
		case DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			break;
		case SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			break;
		case NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			break;
		default:
			BUG("unknown get_sha1_with_context result %d\n",
			       result);
			break;
		}
		fflush(stdout);

		goto out;
	}

	nếu (ctx.mode == 0) {
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

static số_nguyên get_remote_info(số_nguyên argc,
			   const char **argv,
			   struct fetch_object_info_results *results,
			   struct oid_array *object_info_oids)
{
	số_nguyên retval = 0;
	struct remote *remote = NULL;
	struct object_id oid;
	struct transport *gtransport;

	remote = remote_get(argv[0]);
	nếu (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	cho (size_t i = 1; i < argc; i++) {
		nếu (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			nếu (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				cho (j = 0; j < len; j++)
					nếu (!isxdigit(argv[i][j]))
						break;
				nếu (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (số_nguyên)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	nếu (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, NULL);

	nếu (!gtransport->smart_options) {
		retval = -1;
		goto cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	trả_về retval;
}

struct object_cb_data {
	struct batch_options *opt;
	struct expand_data *expand;
	struct oidset *seen;
	struct strbuf *scratch;
};

static số_nguyên batch_object_cb(const struct object_id *oid, void *vdata)
{
	struct object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(NULL, data->scratch, data->opt, data->expand,
			   NULL, 0);
	trả_về 0;
}

static số_nguyên collect_object(const struct object_id *oid,
			  struct packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  void *data)
{
	oid_array_append(data, oid);
	trả_về 0;
}

static số_nguyên batch_unordered_object(const struct object_id *oid,
				  struct packed_git *pack,
				  off_t offset,
				  void *vdata)
{
	struct object_cb_data *data = vdata;

	nếu (oidset_insert(data->seen, oid))
		trả_về 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(NULL, data->scratch, data->opt, data->expand,
			   pack, offset);
	trả_về 0;
}

typedef void (*parse_cmd_fn_t)(struct batch_options *, const char *,
			       struct strbuf *, struct expand_data *);

struct queued_cmd {
	parse_cmd_fn_t fn;
	char *line;
};

static void parse_cmd_contents(struct batch_options *opt,
			     const char *line,
			     struct strbuf *output,
			     struct expand_data *data)
{
	enum object_type *saved_typep = data->info.typep;
	data->info.typep = &data->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(line, output, opt, data);
	data->info.typep = saved_typep;
}

static void parse_cmd_info(struct batch_options *opt,
			   const char *line,
			   struct strbuf *output,
			   struct expand_data *data)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(line, output, opt, data);
}

static void parse_cmd_mailmap(struct batch_options *opt UNUSED,
			      const char *line,
			      struct strbuf *output UNUSED,
			      struct expand_data *data UNUSED)
{
	use_mailmap = git_parse_maybe_bool(line);

	nếu (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), line);

	nếu (use_mailmap)
		load_mailmap();
}

static void parse_cmd_remote_object_info(struct batch_options *opt,
					 const char *line, struct strbuf *output,
					 struct expand_data *data)
{
	số_nguyên count;
	const char **argv;
	char *line_to_split;
	struct fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	struct oid_array object_info_oids = OID_ARRAY_INIT;

	nếu (strlen(line) >= MAX_REMOTE_OBJ_INFO_LINE)
		die(_("remote-object-info command too long"));

	line_to_split = xstrdup(line);
	count = split_cmdline(line_to_split, &argv);
	nếu (count < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(count));
	nếu (count - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	nếu (data->info.sizep)
		results.wants_size = 1;
	nếu (data->info.typep)
		results.wants_type = 1;

	nếu (get_remote_info(count, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	nếu (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	nếu (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	cho (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		nếu (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			continue;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		nếu (results.sizes)
			data->size = results.sizes[i];

		nếu (results.types)
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

static void dispatch_calls(struct batch_options *opt,
		struct strbuf *output,
		struct expand_data *data,
		struct queued_cmd *cmd,
		size_t nr)
{
	nếu (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	cho (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].line, output, data);

	fflush(stdout);
}

static void free_cmds(struct queued_cmd *cmd, size_t *nr)
{
	cho (size_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].line);

	*nr = 0;
}


static const struct parse_cmd {
	const char *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", NULL, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

static void batch_objects_command(struct batch_options *opt,
				    struct strbuf *output,
				    struct expand_data *data)
{
	struct strbuf input = STRBUF_INIT;
	struct queued_cmd *queued_cmd = NULL;
	size_t alloc = 0, nr = 0;

	trong_khi (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		const struct parse_cmd *cmd = NULL;
		const char *p = NULL, *cmd_end;
		struct queued_cmd call = {0};

		nếu (!input.len)
			die(_("empty command in input"));
		nếu (isspace(*input.buf))
			die(_("whitespace before command: '%s'"), input.buf);

		cho (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			nếu (!skip_prefix(input.buf, commands[i].name, &cmd_end))
				continue;

			cmd = &commands[i];
			nếu (cmd->takes_args) {
				nếu (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} không_thì nếu (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			break;
		}

		nếu (!cmd)
			die(_("unknown command: '%s'"), input.buf);

		nếu (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} không_thì nếu (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} không_thì {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.line = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	nếu (opt->buffer_output &&
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

typedef số_nguyên (*for_each_object_fn)(const struct object_id *oid, struct packed_git *pack,
				  off_t offset, void *data);

struct for_each_object_payload {
	for_each_object_fn callback;
	void *payload;
};

static số_nguyên batch_one_object_oi(const struct object_id *oid,
			       struct object_info *oi,
			       void *_payload)
{
	struct for_each_object_payload *payload = _payload;
	nếu (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		trả_về payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	trả_về payload->callback(oid, NULL, 0, payload->payload);
}

static void batch_each_object(struct batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      void *_payload)
{
	struct for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	struct odb_source_info source_info;
	struct object_info oi = {
		.source_infop = &source_info,
	};
	struct odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

static số_nguyên batch_objects(struct batch_options *opt)
{
	struct strbuf input = STRBUF_INIT;
	struct strbuf output = STRBUF_INIT;
	struct expand_data data = EXPAND_DATA_INIT;
	struct repo_config_values *cfg = repo_config_values(the_repository);
	số_nguyên save_warning;
	số_nguyên retval = 0;

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
	nếu (opt->transform_mode)
		data.split_on_whitespace = 1;

	nếu (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = NULL;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	nếu (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	nếu (opt->all_objects) {
		struct object_cb_data cb;
		struct object_info empty = OBJECT_INFO_INIT;

		nếu (!memcmp(&data.info, &empty, sizeof(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		nếu (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		nếu (opt->unordered) {
			struct oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} không_thì {
			struct oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		trả_về 0;
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

	nếu (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		goto cleanup;
	}

	trong_khi (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		nếu (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			char *p = strpbrk(input.buf, " \t");
			nếu (p) {
				trong_khi (*p && strchr(" \t", *p))
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
	trả_về retval;
}

static số_nguyên git_cat_file_config(const char *var, const char *value,
			       const struct config_context *ctx, void *cb)
{
	nếu (userdiff_config(var, value) < 0)
		trả_về -1;

	trả_về git_default_config(var, value, ctx, cb);
}

static số_nguyên batch_option_callback(const struct option *opt,
				 const char *arg,
				 số_nguyên unset)
{
	struct batch_options *bo = opt->value;

	BUG_ON_OPT_NEG(unset);

	nếu (bo->enabled) {
		trả_về error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	nếu (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	không_thì nếu (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	không_thì nếu (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	không_thì
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	trả_về 0;
}

số_nguyên cmd_cat_file(số_nguyên argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo UNUSED)
{
	số_nguyên opt = 0;
	số_nguyên opt_cw = 0;
	số_nguyên opt_epts = 0;
	const char *exp_type = NULL, *obj_name = NULL;
	struct batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	số_nguyên unknown_type = 0;
	số_nguyên input_nul_terminated = 0;
	số_nguyên nul_terminated = 0;
	số_nguyên ret;

	const char * const builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		NULL
	};
	const struct option options[] = {
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

	nếu (use_mailmap)
		load_mailmap();

	switch (batch.objects_filter.choice) {
	case LOFC_DISABLED:
		break;
	case LOFC_BLOB_NONE:
	case LOFC_BLOB_LIMIT:
	case LOFC_OBJECT_TYPE:
		nếu (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		break;
	default:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	nếu (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	nếu (force_path && !opt_cw)
		usage_msg_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	nếu (batch.enabled)
		;
	không_thì nếu (batch.follow_symlinks)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	không_thì nếu (batch.buffer_output >= 0)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	không_thì nếu (batch.all_objects)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	không_thì nếu (input_nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	không_thì nếu (nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.input_delim = batch.output_delim = '\n';
	nếu (input_nul_terminated)
		batch.input_delim = '\0';
	nếu (nul_terminated)
		batch.input_delim = batch.output_delim = '\0';

	/* Batch defaults */
	nếu (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	nếu (batch.enabled) {
		nếu (opt_cw)
			batch.transform_mode = opt;
		không_thì nếu (opt && opt != 'b')
			usage_msg_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		không_thì nếu (argc)
			usage_msg_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		goto out;
	}

	nếu (opt) {
		nếu (!argc && opt == 'c')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		không_thì nếu (!argc && opt == 'w')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		không_thì nếu (!argc && opt_epts)
			usage_msg_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		không_thì nếu (argc == 1)
			obj_name = argv[0];
		không_thì
			usage_msg_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} không_thì nếu (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} không_thì nếu (argc != 2) {
		usage_msg_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} không_thì nếu (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	trả_về ret;
}
