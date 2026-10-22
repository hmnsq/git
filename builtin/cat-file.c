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
	정수 enabled;
	정수 follow_symlinks;
	enum batch_mode batch_mode;
	정수 buffer_output;
	정수 all_objects;
	정수 unordered;
	정수 transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	char input_delim;
	char output_delim;
	const char *format;
};

static const char *force_path;

static struct string_list mailmap = STRING_LIST_INIT_NODUP;
static 정수 use_mailmap;

static char *replace_idents_using_mailmap(char *, size_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
static void load_mailmap(void)
{
	만약 (mailmap.strdup_strings)
		반환;

	read_mailmap(the_repository, &mailmap);
}

static char *replace_idents_using_mailmap(char *object_buf, size_t *size)
{
	struct strbuf sb = STRBUF_INIT;
	const char *headers[] = { "author ", "committer ", "tagger ", NULL };

	strbuf_attach(&sb, object_buf, *size, *size + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*size = sb.len;
	반환 strbuf_detach(&sb, NULL);
}

static 정수 filter_object(const char *path, unsigned mode,
			 const struct object_id *oid,
			 char **buf, size_t *size)
{
	enum object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, size);
	만약 (!*buf)
		반환 error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	만약 ((type == OBJ_BLOB) && S_ISREG(mode)) {
		struct strbuf strbuf = STRBUF_INIT;
		struct checkout_metadata meta;

		init_checkout_metadata(&meta, NULL, NULL, oid);
		만약 (convert_to_working_tree(the_repository->index, path, *buf, *size, &strbuf, &meta)) {
			free(*buf);
			*size = strbuf.len;
			*buf = strbuf_detach(&strbuf, NULL);
		}
	}

	반환 0;
}

static 정수 stream_blob(const struct object_id *oid)
{
	만약 (odb_stream_blob_to_fd(the_repository->objects, 1, oid, NULL, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	반환 0;
}

static 정수 cat_one_file(정수 opt, const char *exp_type, const char *obj_name)
{
	정수 ret;
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
	const 정수 opt_cw = (opt == 'c' || opt == 'w');
	만약 (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	만약 (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	만약 (!path)
		path = obj_context.path;
	만약 (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = NULL;
	switch (opt) {
	case 't':
		oi.typep = &type;
		만약 (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		goto cleanup;

	case 's':
		oi.sizep = &size;

		만약 (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (void**)&buf;
		}

		만약 (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		만약 (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &size);

		printf("%"PRIuMAX"\n", (uintmax_t)size);
		ret = 0;
		goto cleanup;

	case 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		goto cleanup;

	case 'w':

		만약 (filter_object(path, obj_context.mode,
				  &oid, &buf, &size)) {
			ret = -1;
			goto cleanup;
		}
		break;

	case 'c':
	{
		unsigned long size_ul = 0;
		정수 textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &size_ul);
		size = size_ul;
		만약 (textconv_ret)
			break;
	}
		/* else fallthrough */

	case 'p':
		type = odb_read_object_info(the_repository->objects, &oid, NULL);
		만약 (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		만약 (type == OBJ_TREE) {
			const char *ls_args[3] = { NULL };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_tree(2, ls_args, NULL, the_repository);
			goto cleanup;
		}

		만약 (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			goto cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &size);
		만약 (!buf)
			die("Cannot read object %s", obj_name);

		만약 (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);

		/* otherwise just spit out the data */
		break;

	case 0:
	{
		enum object_type exp_type_id = type_from_string(exp_type);

		만약 (exp_type_id == OBJ_BLOB) {
			struct object_id blob_oid;
			만약 (odb_read_object_info(the_repository->objects,
						 &oid, NULL) == OBJ_TAG) {
				char *buffer = odb_read_object(the_repository->objects,
							       &oid, &type, &size);
				const char *target;

				만약 (!buffer)
					die(_("unable to read %s"), oid_to_hex(&oid));

				만약 (!skip_prefix(buffer, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				free(buffer);
			} 아니면
				oidcpy(&blob_oid, &oid);

			만약 (odb_read_object_info(the_repository->objects,
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

		만약 (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &size);
		break;
	}
	default:
		die("git cat-file: unknown option: %s", exp_type);
	}

	만약 (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, size);
	ret = 0;
cleanup:
	free(buf);
	object_context_release(&obj_context);
	반환 ret;
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
	정수 mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	정수 split_on_whitespace;

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

static 정수 is_atom(const char *atom, const char *s, 정수 slen)
{
	정수 alen = strlen(atom);
	반환 alen == slen && !memcmp(atom, s, alen);
}

static 정수 expand_atom(struct strbuf *sb, const char *atom, 정수 len,
		       struct expand_data *data)
{
	만약 (data->is_remote) {
		size_t i;
		위해 (i = 0; i < data->remote_allowed_atoms.nr; i++)
			만약 (is_atom(data->remote_allowed_atoms.items[i].string,
				    atom, len))
				break;
		만약 (i == data->remote_allowed_atoms.nr)
			반환 1;
	}

	만약 (is_atom("objectname", atom, len)) {
		만약 (!data->mark_query)
			strbuf_add_oid_hex(sb, &data->oid);
	} 아니면 만약 (is_atom("objecttype", atom, len)) {
		만약 (data->mark_query) {
			data->info.typep = &data->type;
		} 아니면 {
			const char *t = type_name(data->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} 아니면 만약 (is_atom("objectsize", atom, len)) {
		만약 (data->mark_query)
			data->info.sizep = &data->size;
		아니면
			strbuf_add_uint(sb, data->size);
	} 아니면 만약 (is_atom("objectsize:disk", atom, len)) {
		만약 (data->mark_query)
			data->info.disk_sizep = &data->disk_size;
		아니면
			strbuf_add_uint(sb, data->disk_size);
	} 아니면 만약 (is_atom("rest", atom, len)) {
		만약 (data->mark_query)
			data->split_on_whitespace = 1;
		아니면 만약 (data->rest)
			strbuf_addstr(sb, data->rest);
	} 아니면 만약 (is_atom("deltabase", atom, len)) {
		만약 (data->mark_query)
			data->info.delta_base_oid = &data->delta_base_oid;
		아니면
			strbuf_add_oid_hex(sb, &data->delta_base_oid);
	} 아니면 만약 (is_atom("objectmode", atom, len)) {
		만약 (!data->mark_query && !(S_IFINVALID == data->mode))
			strbuf_addf(sb, "%06o", data->mode);
	} 아니면
		반환 0;
	반환 1;
}

static void expand_format(struct strbuf *sb, const char *start,
			  struct expand_data *data)
{
	동안 (strbuf_expand_step(sb, &start)) {
		const char *end;

		만약 (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		아니면 만약 ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, data))
			start = end + 1;
		아니면
			strbuf_expand_bad_format(start, "cat-file");
	}
}

static void batch_write(struct batch_options *opt, const void *data, 정수 len)
{
	만약 (opt->buffer_output) {
		만약 (fwrite(data, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} 아니면
		write_or_die(1, data, len);
}

static void print_object_or_die(struct batch_options *opt, struct expand_data *data)
{
	const struct object_id *oid = &data->oid;

	assert(data->info.typep);

	만약 (data->type == OBJ_BLOB) {
		만약 (opt->buffer_output)
			fflush(stdout);
		만약 (opt->transform_mode) {
			char *contents;
			size_t size;

			만약 (!data->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			만약 (opt->transform_mode == 'w') {
				만약 (filter_object(data->rest, 0100644, oid,
						  &contents, &size))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} 아니면 만약 (opt->transform_mode == 'c') {
				enum object_type type;
				unsigned long size_ul = 0;
				만약 (textconv_object(the_repository,
						    data->rest, 0100644, oid,
						    1, &contents, &size_ul))
					size = size_ul;
				아니면
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &size);
				만약 (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), data->rest);
			} 아니면
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, size);
			free(contents);
		} 아니면 {
			stream_blob(oid);
		}
	}
	아니면 {
		enum object_type type;
		size_t size;
		void *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &size);
		만약 (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		만약 (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &size);

		만약 (type != data->type)
			die("object %s changed type!?", oid_to_hex(oid));
		만약 (data->info.sizep && size != data->size && !use_mailmap)
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
	만약 (!data->skip_object_info) {
		정수 ret;

		만약 (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			data->info.typep = &data->type;
		만약 (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			data->info.sizep = &data->size;

		만약 (pack)
			ret = packed_object_info(NULL, pack, offset, &data->info);
		아니면
			ret = odb_read_object_info_extended(the_repository->objects,
							    &data->oid, &data->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		만약 (ret < 0) {
			만약 (data->mode == S_IFGITLINK)
				report_object_status(opt, NULL, &data->oid, "submodule");
			아니면
				report_object_status(opt, obj_name, &data->oid, "missing");
			반환;
		}

		switch (opt->objects_filter.choice) {
		case LOFC_DISABLED:
			break;
		case LOFC_BLOB_NONE:
			만약 (data->type == OBJ_BLOB) {
				만약 (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				반환;
			}
			break;
		case LOFC_BLOB_LIMIT:
			만약 (data->type == OBJ_BLOB &&
			    data->size >= opt->objects_filter.blob_limit_value) {
				만약 (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				반환;
			}
			break;
		case LOFC_OBJECT_TYPE:
			만약 (data->type != opt->objects_filter.object_type) {
				만약 (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &data->oid, "excluded");
				반환;
			}
			break;
		default:
			BUG("unsupported objects filter");
		}

		만약 (use_mailmap && (data->type == OBJ_COMMIT || data->type == OBJ_TAG)) {
			char *buf = NULL;

			buf = odb_read_object(the_repository->objects, &data->oid,
					      &data->type, &data->size);
			만약 (!buf)
				die(_("unable to read %s"), oid_to_hex(&data->oid));
			buf = replace_idents_using_mailmap(buf, &data->size);

			free(buf);
		}
	}

	strbuf_reset(scratch);

	만약 (!opt->format) {
		print_default_format(scratch, data, opt);
	} 아니면 {
		expand_format(scratch, opt->format, data);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	만약 (opt->batch_mode == BATCH_MODE_CONTENTS) {
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
	정수 flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	enum get_oid_result result;

	result = get_oid_with_context(the_repository, obj_name,
				      flags, &data->oid, &ctx);
	만약 (result != FOUND) {
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

	만약 (ctx.mode == 0) {
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

static 정수 get_remote_info(정수 argc,
			   const char **argv,
			   struct fetch_object_info_results *results,
			   struct oid_array *object_info_oids)
{
	정수 retval = 0;
	struct remote *remote = NULL;
	struct object_id oid;
	struct transport *gtransport;

	remote = remote_get(argv[0]);
	만약 (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	위해 (size_t i = 1; i < argc; i++) {
		만약 (get_oid_hex(argv[i], &oid)) {
			size_t len = strlen(argv[i]);

			만약 (len < the_hash_algo->hexsz && len >= 4) {
				size_t j;
				위해 (j = 0; j < len; j++)
					만약 (!isxdigit(argv[i][j]))
						break;
				만약 (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (정수)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	만약 (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, NULL);

	만약 (!gtransport->smart_options) {
		retval = -1;
		goto cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	반환 retval;
}

struct object_cb_data {
	struct batch_options *opt;
	struct expand_data *expand;
	struct oidset *seen;
	struct strbuf *scratch;
};

static 정수 batch_object_cb(const struct object_id *oid, void *vdata)
{
	struct object_cb_data *data = vdata;
	oidcpy(&data->expand->oid, oid);
	batch_object_write(NULL, data->scratch, data->opt, data->expand,
			   NULL, 0);
	반환 0;
}

static 정수 collect_object(const struct object_id *oid,
			  struct packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  void *data)
{
	oid_array_append(data, oid);
	반환 0;
}

static 정수 batch_unordered_object(const struct object_id *oid,
				  struct packed_git *pack,
				  off_t offset,
				  void *vdata)
{
	struct object_cb_data *data = vdata;

	만약 (oidset_insert(data->seen, oid))
		반환 0;

	oidcpy(&data->expand->oid, oid);
	batch_object_write(NULL, data->scratch, data->opt, data->expand,
			   pack, offset);
	반환 0;
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

	만약 (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), line);

	만약 (use_mailmap)
		load_mailmap();
}

static void parse_cmd_remote_object_info(struct batch_options *opt,
					 const char *line, struct strbuf *output,
					 struct expand_data *data)
{
	정수 count;
	const char **argv;
	char *line_to_split;
	struct fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	struct oid_array object_info_oids = OID_ARRAY_INIT;

	만약 (strlen(line) >= MAX_REMOTE_OBJ_INFO_LINE)
		die(_("remote-object-info command too long"));

	line_to_split = xstrdup(line);
	count = split_cmdline(line_to_split, &argv);
	만약 (count < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(count));
	만약 (count - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	만약 (data->info.sizep)
		results.wants_size = 1;
	만약 (data->info.typep)
		results.wants_type = 1;

	만약 (get_remote_info(count, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&data->remote_allowed_atoms, 0);
	string_list_append(&data->remote_allowed_atoms, "objectname");
	만약 (results.sizes)
		string_list_append(&data->remote_allowed_atoms, "objectsize");
	만약 (results.types)
		string_list_append(&data->remote_allowed_atoms, "objecttype");

	data->skip_object_info = 1;
	위해 (size_t i = 0; i < results.nr; i++) {
		data->oid = object_info_oids.oid[i];

		만약 (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&data->oid),
					     &data->oid, "missing");
			continue;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		만약 (results.sizes)
			data->size = results.sizes[i];

		만약 (results.types)
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
	만약 (!opt->buffer_output)
		die(_("flush is only for --buffer mode"));

	위해 (size_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].line, output, data);

	fflush(stdout);
}

static void free_cmds(struct queued_cmd *cmd, size_t *nr)
{
	위해 (size_t i = 0; i < *nr; i++)
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

	동안 (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		const struct parse_cmd *cmd = NULL;
		const char *p = NULL, *cmd_end;
		struct queued_cmd call = {0};

		만약 (!input.len)
			die(_("empty command in input"));
		만약 (isspace(*input.buf))
			die(_("whitespace before command: '%s'"), input.buf);

		위해 (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
			만약 (!skip_prefix(input.buf, commands[i].name, &cmd_end))
				continue;

			cmd = &commands[i];
			만약 (cmd->takes_args) {
				만약 (*cmd_end != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_end + 1;
			} 아니면 만약 (*cmd_end) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			break;
		}

		만약 (!cmd)
			die(_("unknown command: '%s'"), input.buf);

		만약 (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, data, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} 아니면 만약 (!opt->buffer_output) {
			cmd->fn(opt, p, output, data);
		} 아니면 {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.line = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	만약 (opt->buffer_output &&
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

typedef 정수 (*for_each_object_fn)(const struct object_id *oid, struct packed_git *pack,
				  off_t offset, void *data);

struct for_each_object_payload {
	for_each_object_fn callback;
	void *payload;
};

static 정수 batch_one_object_oi(const struct object_id *oid,
			       struct object_info *oi,
			       void *_payload)
{
	struct for_each_object_payload *payload = _payload;
	만약 (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		반환 payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	반환 payload->callback(oid, NULL, 0, payload->payload);
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

static 정수 batch_objects(struct batch_options *opt)
{
	struct strbuf input = STRBUF_INIT;
	struct strbuf output = STRBUF_INIT;
	struct expand_data data = EXPAND_DATA_INIT;
	struct repo_config_values *cfg = repo_config_values(the_repository);
	정수 save_warning;
	정수 retval = 0;

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
	만약 (opt->transform_mode)
		data.split_on_whitespace = 1;

	만약 (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = NULL;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	만약 (opt->batch_mode == BATCH_MODE_CONTENTS)
		data.info.typep = &data.type;

	만약 (opt->all_objects) {
		struct object_cb_data cb;
		struct object_info empty = OBJECT_INFO_INIT;

		만약 (!memcmp(&data.info, &empty, sizeof(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			data.skip_object_info = 1;

		만약 (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &data;
		cb.scratch = &output;

		만약 (opt->unordered) {
			struct oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} 아니면 {
			struct oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		반환 0;
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

	만약 (opt->batch_mode == BATCH_MODE_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &data);
		goto cleanup;
	}

	동안 (strbuf_getdelim_strip_crlf(&input, stdin, opt->input_delim) != EOF) {
		만약 (data.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			char *p = strpbrk(input.buf, " \t");
			만약 (p) {
				동안 (*p && strchr(" \t", *p))
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
	반환 retval;
}

static 정수 git_cat_file_config(const char *var, const char *value,
			       const struct config_context *ctx, void *cb)
{
	만약 (userdiff_config(var, value) < 0)
		반환 -1;

	반환 git_default_config(var, value, ctx, cb);
}

static 정수 batch_option_callback(const struct option *opt,
				 const char *arg,
				 정수 unset)
{
	struct batch_options *bo = opt->value;

	BUG_ON_OPT_NEG(unset);

	만약 (bo->enabled) {
		반환 error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	만약 (!strcmp(opt->long_name, "batch"))
		bo->batch_mode = BATCH_MODE_CONTENTS;
	아니면 만약 (!strcmp(opt->long_name, "batch-check"))
		bo->batch_mode = BATCH_MODE_INFO;
	아니면 만약 (!strcmp(opt->long_name, "batch-command"))
		bo->batch_mode = BATCH_MODE_QUEUE_AND_DISPATCH;
	아니면
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	반환 0;
}

정수 cmd_cat_file(정수 argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo UNUSED)
{
	정수 opt = 0;
	정수 opt_cw = 0;
	정수 opt_epts = 0;
	const char *exp_type = NULL, *obj_name = NULL;
	struct batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	정수 unknown_type = 0;
	정수 input_nul_terminated = 0;
	정수 nul_terminated = 0;
	정수 ret;

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

	만약 (use_mailmap)
		load_mailmap();

	switch (batch.objects_filter.choice) {
	case LOFC_DISABLED:
		break;
	case LOFC_BLOB_NONE:
	case LOFC_BLOB_LIMIT:
	case LOFC_OBJECT_TYPE:
		만약 (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		break;
	default:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	만약 (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	만약 (force_path && !opt_cw)
		usage_msg_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	만약 (batch.enabled)
		;
	아니면 만약 (batch.follow_symlinks)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	아니면 만약 (batch.buffer_output >= 0)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	아니면 만약 (batch.all_objects)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	아니면 만약 (input_nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	아니면 만약 (nul_terminated)
		usage_msg_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.input_delim = batch.output_delim = '\n';
	만약 (input_nul_terminated)
		batch.input_delim = '\0';
	만약 (nul_terminated)
		batch.input_delim = batch.output_delim = '\0';

	/* Batch defaults */
	만약 (batch.buffer_output < 0)
		batch.buffer_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	/* Return early if we're in batch mode? */
	만약 (batch.enabled) {
		만약 (opt_cw)
			batch.transform_mode = opt;
		아니면 만약 (opt && opt != 'b')
			usage_msg_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		아니면 만약 (argc)
			usage_msg_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		goto out;
	}

	만약 (opt) {
		만약 (!argc && opt == 'c')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		아니면 만약 (!argc && opt == 'w')
			usage_msg_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		아니면 만약 (!argc && opt_epts)
			usage_msg_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		아니면 만약 (argc == 1)
			obj_name = argv[0];
		아니면
			usage_msg_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} 아니면 만약 (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} 아니면 만약 (argc != 2) {
		usage_msg_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} 아니면 만약 (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	반환 ret;
}
