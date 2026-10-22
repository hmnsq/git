/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "convert.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "ident.h"
#অন্তর্ভাব "list-objects-filter-options.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "userdiff.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "packfile.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/streaming.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "mailmap.h"
#অন্তর্ভাব "write-or-die.h"
#অন্তর্ভাব "alias.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "transport.h"
#অন্তর্ভাব "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#define MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#define MAX_ALLOWED_OBJ_LIMIT 10000

/* Maximum input size permitted for the remote-object-info command. */
#define MAX_REMOTE_OBJ_INFO_সারি \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_LIMIT * (GIT_MAX_HEXSZ + 1))

গণনা batch_mode {
	BATCH_MODE_CONTENTS,
	BATCH_MODE_INFO,
	BATCH_MODE_QUEUE_AND_DISPATCH,
};

কাঠামো batch_options {
	কাঠামো list_objects_filter_options objects_filter;
	পূর্ণসংখ্যা enabled;
	পূর্ণসংখ্যা follow_symlinks;
	গণনা batch_mode batch_mode;
	পূর্ণসংখ্যা বাফার_output;
	পূর্ণসংখ্যা all_objects;
	পূর্ণসংখ্যা unordered;
	পূর্ণসংখ্যা transform_mode; /* may be 'w' or 'c' for --filters or --textconv */
	অক্ষর প্রবেশ_delim;
	অক্ষর output_delim;
	ধ্রুবক অক্ষর *format;
};

স্থির ধ্রুবক অক্ষর *force_path;

স্থির কাঠামো string_list mailmap = STRING_LIST_INIT_NODUP;
স্থির পূর্ণসংখ্যা use_mailmap;

স্থির অক্ষর *replace_idents_using_mailmap(অক্ষর *, আকার_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
স্থির শূন্য load_mailmap(শূন্য)
{
	যদি (mailmap.strdup_strings)
		ফেরত;

	read_mailmap(the_repository, &mailmap);
}

স্থির অক্ষর *replace_idents_using_mailmap(অক্ষর *object_buf, আকার_t *আকার_2)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *headers[] = { "author ", "committer ", "tagger ", ফাঁকা };

	strbuf_attach(&sb, object_buf, *আকার_2, *আকার_2 + 1);
	apply_mailmap_to_header(&sb, headers, &mailmap);
	*আকার_2 = sb.len;
	ফেরত strbuf_detach(&sb, ফাঁকা);
}

স্থির পূর্ণসংখ্যা filter_object(ধ্রুবক অক্ষর *path, unsigned mode,
			 ধ্রুবক কাঠামো object_id *oid,
			 অক্ষর **buf, আকার_t *আকার_2)
{
	গণনা object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, আকার_2);
	যদি (!*buf)
		ফেরত error(_("cannot read object %s '%s'"),
			     oid_to_hex(oid), path);
	যদি ((type == OBJ_BLOB) && S_ISREG(mode)) {
		কাঠামো strbuf strbuf = STRBUF_INIT;
		কাঠামো checkout_metadata meta;

		init_checkout_metadata(&meta, ফাঁকা, ফাঁকা, oid);
		যদি (convert_to_working_বৃক্ষ(the_repository->সূচক, path, *buf, *আকার_2, &strbuf, &meta)) {
			মুক্ত(*buf);
			*আকার_2 = strbuf.len;
			*buf = strbuf_detach(&strbuf, ফাঁকা);
		}
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা stream_blob(ধ্রুবক কাঠামো object_id *oid)
{
	যদি (odb_stream_blob_to_fd(the_repository->objects, 1, oid, ফাঁকা, 0))
		die("unable to stream %s to stdout", oid_to_hex(oid));
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cat_one_file(পূর্ণসংখ্যা opt, ধ্রুবক অক্ষর *exp_type, ধ্রুবক অক্ষর *obj_name)
{
	পূর্ণসংখ্যা ret;
	কাঠামো object_id oid;
	গণনা object_type type;
	অক্ষর *buf;
	আকার_t আকার_2;
	কাঠামো object_context obj_context = {0};
	কাঠামো object_info oi = OBJECT_INFO_INIT;
	unsigned flags = OBJECT_INFO_LOOKUP_REPLACE;
	unsigned get_oid_flags =
		GET_OID_RECORD_PATH |
		GET_OID_ONLY_TO_DIE |
		GET_OID_HASH_ANY;
	ধ্রুবক অক্ষর *path = force_path;
	ধ্রুবক পূর্ণসংখ্যা opt_cw = (opt == 'c' || opt == 'w');
	যদি (!path && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_PATH;

	যদি (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	যদি (!path)
		path = obj_context.path;
	যদি (obj_context.mode == S_IFINVALID)
		obj_context.mode = 0100644;

	buf = ফাঁকা;
	নির্বাচন (opt) {
	ক্ষেত্র 't':
		oi.typep = &type;
		যদি (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		printf("%s\n", type_name(type));
		ret = 0;
		যাও cleanup;

	ক্ষেত্র 's':
		oi.sizep = &আকার_2;

		যদি (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (শূন্য**)&buf;
		}

		যদি (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		যদি (use_mailmap && (type == OBJ_COMMIT || type == OBJ_TAG))
			buf = replace_idents_using_mailmap(buf, &আকার_2);

		printf("%"PRIuMAX"\n", (uintmax_t)আকার_2);
		ret = 0;
		যাও cleanup;

	ক্ষেত্র 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		যাও cleanup;

	ক্ষেত্র 'w':

		যদি (filter_object(path, obj_context.mode,
				  &oid, &buf, &আকার_2)) {
			ret = -1;
			যাও cleanup;
		}
		বিরতি;

	ক্ষেত্র 'c':
	{
		unsigned long আকার_ul = 0;
		পূর্ণসংখ্যা textconv_ret = textconv_object(the_repository, path,
						   obj_context.mode, &oid, 1,
						   &buf, &আকার_ul);
		আকার_2 = আকার_ul;
		যদি (textconv_ret)
			বিরতি;
	}
		/* else fallthrough */

	ক্ষেত্র 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ফাঁকা);
		যদি (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		যদি (type == OBJ_বৃক্ষ) {
			ধ্রুবক অক্ষর *ls_args[3] = { ফাঁকা };
			ls_args[0] =  "ls-tree";
			ls_args[1] =  obj_name;
			ret = cmd_ls_বৃক্ষ(2, ls_args, ফাঁকা, the_repository);
			যাও cleanup;
		}

		যদি (type == OBJ_BLOB) {
			ret = stream_blob(&oid);
			যাও cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &আকার_2);
		যদি (!buf)
			die("Cannot read object %s", obj_name);

		যদি (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &আকার_2);

		/* otherwise just spit out the data */
		বিরতি;

	ক্ষেত্র 0:
	{
		গণনা object_type exp_type_id = type_from_string(exp_type);

		যদি (exp_type_id == OBJ_BLOB) {
			কাঠামো object_id blob_oid;
			যদি (odb_read_object_info(the_repository->objects,
						 &oid, ফাঁকা) == OBJ_TAG) {
				অক্ষর *বাফার = odb_read_object(the_repository->objects,
							       &oid, &type, &আকার_2);
				ধ্রুবক অক্ষর *target;

				যদি (!বাফার)
					die(_("unable to read %s"), oid_to_hex(&oid));

				যদি (!skip_prefix(বাফার, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_to_hex(&oid));
				মুক্ত(বাফার);
			} অন্যথা
				oidcpy(&blob_oid, &oid);

			যদি (odb_read_object_info(the_repository->objects,
						 &blob_oid, ফাঁকা) == OBJ_BLOB) {
				ret = stream_blob(&blob_oid);
				যাও cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &আকার_2, ফাঁকা);

		যদি (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &আকার_2);
		বিরতি;
	}
	পূর্বনির্ধারিত:
		die("git cat-file: unknown option: %s", exp_type);
	}

	যদি (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, আকার_2);
	ret = 0;
cleanup:
	মুক্ত(buf);
	object_context_release(&obj_context);
	ফেরত ret;
}

কাঠামো expand_তথ্য {
	কাঠামো object_id oid;
	গণনা object_type type;
	আকার_t আকার_2;
	unsigned short mode;
	off_t disk_আকার;
	ধ্রুবক অক্ষর *rest;
	কাঠামো object_id delta_base_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	পূর্ণসংখ্যা mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	পূর্ণসংখ্যা split_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	কাঠামো object_info info;

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
	কাঠামো string_list remote_allowed_atoms;
};

#define EXPAND_তথ্য_INIT  {  \
	.mode = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

স্থির পূর্ণসংখ্যা is_atom(ধ্রুবক অক্ষর *atom, ধ্রুবক অক্ষর *s, পূর্ণসংখ্যা slen)
{
	পূর্ণসংখ্যা alen = strlen(atom);
	ফেরত alen == slen && !memcmp(atom, s, alen);
}

স্থির পূর্ণসংখ্যা expand_atom(কাঠামো strbuf *sb, ধ্রুবক অক্ষর *atom, পূর্ণসংখ্যা len,
		       কাঠামো expand_তথ্য *তথ্য)
{
	যদি (তথ্য->is_remote) {
		আকার_t i;
		জন্য (i = 0; i < তথ্য->remote_allowed_atoms.nr; i++)
			যদি (is_atom(তথ্য->remote_allowed_atoms.বস্তুসমূহ[i].string,
				    atom, len))
				বিরতি;
		যদি (i == তথ্য->remote_allowed_atoms.nr)
			ফেরত 1;
	}

	যদি (is_atom("objectname", atom, len)) {
		যদি (!তথ্য->mark_query)
			strbuf_add_oid_hex(sb, &তথ্য->oid);
	} অন্যথা যদি (is_atom("objecttype", atom, len)) {
		যদি (তথ্য->mark_query) {
			তথ্য->info.typep = &তথ্য->type;
		} অন্যথা {
			ধ্রুবক অক্ষর *t = type_name(তথ্য->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} অন্যথা যদি (is_atom("objectsize", atom, len)) {
		যদি (তথ্য->mark_query)
			তথ্য->info.sizep = &তথ্য->আকার_2;
		অন্যথা
			strbuf_add_uint(sb, তথ্য->আকার_2);
	} অন্যথা যদি (is_atom("objectsize:disk", atom, len)) {
		যদি (তথ্য->mark_query)
			তথ্য->info.disk_sizep = &তথ্য->disk_আকার;
		অন্যথা
			strbuf_add_uint(sb, তথ্য->disk_আকার);
	} অন্যথা যদি (is_atom("rest", atom, len)) {
		যদি (তথ্য->mark_query)
			তথ্য->split_on_whitespace = 1;
		অন্যথা যদি (তথ্য->rest)
			strbuf_addstr(sb, তথ্য->rest);
	} অন্যথা যদি (is_atom("deltabase", atom, len)) {
		যদি (তথ্য->mark_query)
			তথ্য->info.delta_base_oid = &তথ্য->delta_base_oid;
		অন্যথা
			strbuf_add_oid_hex(sb, &তথ্য->delta_base_oid);
	} অন্যথা যদি (is_atom("objectmode", atom, len)) {
		যদি (!তথ্য->mark_query && !(S_IFINVALID == তথ্য->mode))
			strbuf_addf(sb, "%06o", তথ্য->mode);
	} অন্যথা
		ফেরত 0;
	ফেরত 1;
}

স্থির শূন্য expand_format(কাঠামো strbuf *sb, ধ্রুবক অক্ষর *start,
			  কাঠামো expand_তথ্য *তথ্য)
{
	যখন (strbuf_expand_step(sb, &start)) {
		ধ্রুবক অক্ষর *end;

		যদি (skip_prefix(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		অন্যথা যদি ((end = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, end - start - 1, তথ্য))
			start = end + 1;
		অন্যথা
			strbuf_expand_bad_format(start, "cat-file");
	}
}

স্থির শূন্য batch_write(কাঠামো batch_options *opt, ধ্রুবক শূন্য *তথ্য, পূর্ণসংখ্যা len)
{
	যদি (opt->বাফার_output) {
		যদি (fwrite(তথ্য, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} অন্যথা
		write_or_die(1, তথ্য, len);
}

স্থির শূন্য print_object_or_die(কাঠামো batch_options *opt, কাঠামো expand_তথ্য *তথ্য)
{
	ধ্রুবক কাঠামো object_id *oid = &তথ্য->oid;

	প্রতিজ্ঞা(তথ্য->info.typep);

	যদি (তথ্য->type == OBJ_BLOB) {
		যদি (opt->বাফার_output)
			fflush(stdout);
		যদি (opt->transform_mode) {
			অক্ষর *contents;
			আকার_t আকার_2;

			যদি (!তথ্য->rest)
				die("missing path for '%s'", oid_to_hex(oid));

			যদি (opt->transform_mode == 'w') {
				যদি (filter_object(তথ্য->rest, 0100644, oid,
						  &contents, &আকার_2))
					die("could not convert '%s' %s",
					    oid_to_hex(oid), তথ্য->rest);
			} অন্যথা যদি (opt->transform_mode == 'c') {
				গণনা object_type type;
				unsigned long আকার_ul = 0;
				যদি (textconv_object(the_repository,
						    তথ্য->rest, 0100644, oid,
						    1, &contents, &আকার_ul))
					আকার_2 = আকার_ul;
				অন্যথা
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &আকার_2);
				যদি (!contents)
					die("could not convert '%s' %s",
					    oid_to_hex(oid), তথ্য->rest);
			} অন্যথা
				BUG("invalid transform_mode: %c", opt->transform_mode);
			batch_write(opt, contents, আকার_2);
			মুক্ত(contents);
		} অন্যথা {
			stream_blob(oid);
		}
	}
	অন্যথা {
		গণনা object_type type;
		আকার_t আকার_2;
		শূন্য *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &আকার_2);
		যদি (!contents)
			die("object %s disappeared", oid_to_hex(oid));

		যদি (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &আকার_2);

		যদি (type != তথ্য->type)
			die("object %s changed type!?", oid_to_hex(oid));
		যদি (তথ্য->info.sizep && আকার_2 != তথ্য->আকার_2 && !use_mailmap)
			die("object %s changed size!?", oid_to_hex(oid));

		batch_write(opt, contents, আকার_2);
		মুক্ত(contents);
	}
}

স্থির শূন্য print_default_format(কাঠামো strbuf *scratch, কাঠামো expand_তথ্য *তথ্য,
				 কাঠামো batch_options *opt)
{
	strbuf_add_oid_hex(scratch, &তথ্য->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(তথ্য->type));
	strbuf_addch(scratch, ' ');
	strbuf_add_uint(scratch, তথ্য->আকার_2);
	strbuf_addch(scratch, opt->output_delim);
}

স্থির শূন্য report_object_status(কাঠামো batch_options *opt,
				 ধ্রুবক অক্ষর *obj_name,
				 ধ্রুবক কাঠামো object_id *oid,
				 ধ্রুবক অক্ষর *status)
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
স্থির শূন্য batch_object_write(ধ্রুবক অক্ষর *obj_name,
			       কাঠামো strbuf *scratch,
			       কাঠামো batch_options *opt,
			       কাঠামো expand_তথ্য *তথ্য,
			       কাঠামো packed_git *pack,
			       off_t offset)
{
	যদি (!তথ্য->skip_object_info) {
		পূর্ণসংখ্যা ret;

		যদি (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_LIMIT ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			তথ্য->info.typep = &তথ্য->type;
		যদি (opt->objects_filter.choice == LOFC_BLOB_LIMIT)
			তথ্য->info.sizep = &তথ্য->আকার_2;

		যদি (pack)
			ret = packed_object_info(ফাঁকা, pack, offset, &তথ্য->info);
		অন্যথা
			ret = odb_read_object_info_extended(the_repository->objects,
							    &তথ্য->oid, &তথ্য->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		যদি (ret < 0) {
			যদি (তথ্য->mode == S_IFGITLINK)
				report_object_status(opt, ফাঁকা, &তথ্য->oid, "submodule");
			অন্যথা
				report_object_status(opt, obj_name, &তথ্য->oid, "missing");
			ফেরত;
		}

		নির্বাচন (opt->objects_filter.choice) {
		ক্ষেত্র LOFC_DISABLED:
			বিরতি;
		ক্ষেত্র LOFC_BLOB_NONE:
			যদি (তথ্য->type == OBJ_BLOB) {
				যদি (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &তথ্য->oid, "excluded");
				ফেরত;
			}
			বিরতি;
		ক্ষেত্র LOFC_BLOB_LIMIT:
			যদি (তথ্য->type == OBJ_BLOB &&
			    তথ্য->আকার_2 >= opt->objects_filter.blob_limit_মান) {
				যদি (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &তথ্য->oid, "excluded");
				ফেরত;
			}
			বিরতি;
		ক্ষেত্র LOFC_OBJECT_TYPE:
			যদি (তথ্য->type != opt->objects_filter.object_type) {
				যদি (!opt->all_objects)
					report_object_status(opt, obj_name,
							     &তথ্য->oid, "excluded");
				ফেরত;
			}
			বিরতি;
		পূর্বনির্ধারিত:
			BUG("unsupported objects filter");
		}

		যদি (use_mailmap && (তথ্য->type == OBJ_COMMIT || তথ্য->type == OBJ_TAG)) {
			অক্ষর *buf = ফাঁকা;

			buf = odb_read_object(the_repository->objects, &তথ্য->oid,
					      &তথ্য->type, &তথ্য->আকার_2);
			যদি (!buf)
				die(_("unable to read %s"), oid_to_hex(&তথ্য->oid));
			buf = replace_idents_using_mailmap(buf, &তথ্য->আকার_2);

			মুক্ত(buf);
		}
	}

	strbuf_reset(scratch);

	যদি (!opt->format) {
		print_default_format(scratch, তথ্য, opt);
	} অন্যথা {
		expand_format(scratch, opt->format, তথ্য);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	যদি (opt->batch_mode == BATCH_MODE_CONTENTS) {
		print_object_or_die(opt, তথ্য);
		batch_write(opt, &opt->output_delim, 1);
	}
}

স্থির শূন্য batch_one_object(ধ্রুবক অক্ষর *obj_name,
			     কাঠামো strbuf *scratch,
			     কাঠামো batch_options *opt,
			     কাঠামো expand_তথ্য *তথ্য)
{
	কাঠামো object_context ctx = {0};
	পূর্ণসংখ্যা flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	গণনা get_oid_ফলাফল ফলাফল;

	ফলাফল = get_oid_with_context(the_repository, obj_name,
				      flags, &তথ্য->oid, &ctx);
	যদি (ফলাফল != FOUND) {
		নির্বাচন (ফলাফল) {
		ক্ষেত্র MISSING_OBJECT:
			report_object_status(opt, obj_name, &তথ্য->oid, "missing");
			বিরতি;
		ক্ষেত্র SHORT_NAME_AMBIGUOUS:
			report_object_status(opt, obj_name, &তথ্য->oid, "ambiguous");
			বিরতি;
		ক্ষেত্র DANGLING_SYMLINK:
			printf("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			বিরতি;
		ক্ষেত্র SYMLINK_LOOP:
			printf("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			বিরতি;
		ক্ষেত্র NOT_DIR:
			printf("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)strlen(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			বিরতি;
		পূর্বনির্ধারিত:
			BUG("unknown get_sha1_with_context result %d\n",
			       ফলাফল);
			বিরতি;
		}
		fflush(stdout);

		যাও out;
	}

	যদি (ctx.mode == 0) {
		printf("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_path.len,
		       opt->output_delim, ctx.symlink_path.buf, opt->output_delim);
		fflush(stdout);
		যাও out;
	}

	তথ্য->mode = ctx.mode;
	batch_object_write(obj_name, scratch, opt, তথ্য, ফাঁকা, 0);

out:
	object_context_release(&ctx);
}

স্থির পূর্ণসংখ্যা get_remote_info(পূর্ণসংখ্যা argc,
			   ধ্রুবক অক্ষর **argv,
			   কাঠামো fetch_object_info_results *results,
			   কাঠামো oid_array *object_info_oids)
{
	পূর্ণসংখ্যা retval = 0;
	কাঠামো remote *remote = ফাঁকা;
	কাঠামো object_id oid;
	কাঠামো transport *gtransport;

	remote = remote_get(argv[0]);
	যদি (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	জন্য (আকার_t i = 1; i < argc; i++) {
		যদি (get_oid_hex(argv[i], &oid)) {
			আকার_t len = strlen(argv[i]);

			যদি (len < the_hash_algo->hexsz && len >= 4) {
				আকার_t j;
				জন্য (j = 0; j < len; j++)
					যদি (!isxdigit(argv[i][j]))
						বিরতি;
				যদি (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (পূর্ণসংখ্যা)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	যদি (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ফাঁকা);

	যদি (!gtransport->smart_options) {
		retval = -1;
		যাও cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	ফেরত retval;
}

কাঠামো object_cb_তথ্য {
	কাঠামো batch_options *opt;
	কাঠামো expand_তথ্য *expand;
	কাঠামো oidset *seen;
	কাঠামো strbuf *scratch;
};

স্থির পূর্ণসংখ্যা batch_object_cb(ধ্রুবক কাঠামো object_id *oid, শূন্য *vdata)
{
	কাঠামো object_cb_তথ্য *তথ্য = vdata;
	oidcpy(&তথ্য->expand->oid, oid);
	batch_object_write(ফাঁকা, তথ্য->scratch, তথ্য->opt, তথ্য->expand,
			   ফাঁকা, 0);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা collect_object(ধ্রুবক কাঠামো object_id *oid,
			  কাঠামো packed_git *pack UNUSED,
			  off_t offset UNUSED,
			  শূন্য *তথ্য)
{
	oid_array_append(তথ্য, oid);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা batch_unordered_object(ধ্রুবক কাঠামো object_id *oid,
				  কাঠামো packed_git *pack,
				  off_t offset,
				  শূন্য *vdata)
{
	কাঠামো object_cb_তথ্য *তথ্য = vdata;

	যদি (oidset_ঢোকাও(তথ্য->seen, oid))
		ফেরত 0;

	oidcpy(&তথ্য->expand->oid, oid);
	batch_object_write(ফাঁকা, তথ্য->scratch, তথ্য->opt, তথ্য->expand,
			   pack, offset);
	ফেরত 0;
}

প্রকারলেখ শূন্য (*parse_cmd_fn_t)(কাঠামো batch_options *, ধ্রুবক অক্ষর *,
			       কাঠামো strbuf *, কাঠামো expand_তথ্য *);

কাঠামো queued_cmd {
	parse_cmd_fn_t fn;
	অক্ষর *সারি;
};

স্থির শূন্য parse_cmd_contents(কাঠামো batch_options *opt,
			     ধ্রুবক অক্ষর *সারি,
			     কাঠামো strbuf *output,
			     কাঠামো expand_তথ্য *তথ্য)
{
	গণনা object_type *saved_typep = তথ্য->info.typep;
	তথ্য->info.typep = &তথ্য->type;
	opt->batch_mode = BATCH_MODE_CONTENTS;
	batch_one_object(সারি, output, opt, তথ্য);
	তথ্য->info.typep = saved_typep;
}

স্থির শূন্য parse_cmd_info(কাঠামো batch_options *opt,
			   ধ্রুবক অক্ষর *সারি,
			   কাঠামো strbuf *output,
			   কাঠামো expand_তথ্য *তথ্য)
{
	opt->batch_mode = BATCH_MODE_INFO;
	batch_one_object(সারি, output, opt, তথ্য);
}

স্থির শূন্য parse_cmd_mailmap(কাঠামো batch_options *opt UNUSED,
			      ধ্রুবক অক্ষর *সারি,
			      কাঠামো strbuf *output UNUSED,
			      কাঠামো expand_তথ্য *তথ্য UNUSED)
{
	use_mailmap = git_parse_maybe_bool(সারি);

	যদি (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), সারি);

	যদি (use_mailmap)
		load_mailmap();
}

স্থির শূন্য parse_cmd_remote_object_info(কাঠামো batch_options *opt,
					 ধ্রুবক অক্ষর *সারি, কাঠামো strbuf *output,
					 কাঠামো expand_তথ্য *তথ্য)
{
	পূর্ণসংখ্যা গুনতি;
	ধ্রুবক অক্ষর **argv;
	অক্ষর *সারি_to_split;
	কাঠামো fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	কাঠামো oid_array object_info_oids = OID_ARRAY_INIT;

	যদি (strlen(সারি) >= MAX_REMOTE_OBJ_INFO_সারি)
		die(_("remote-object-info command too long"));

	সারি_to_split = xstrdup(সারি);
	গুনতি = split_cmdline(সারি_to_split, &argv);
	যদি (গুনতি < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    split_cmdline_strerror(গুনতি));
	যদি (গুনতি - 1 > MAX_ALLOWED_OBJ_LIMIT)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_LIMIT);

	যদি (তথ্য->info.sizep)
		results.wants_আকার = 1;
	যদি (তথ্য->info.typep)
		results.wants_type = 1;

	যদি (get_remote_info(গুনতি, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&তথ্য->remote_allowed_atoms, 0);
	string_list_append(&তথ্য->remote_allowed_atoms, "objectname");
	যদি (results.sizes)
		string_list_append(&তথ্য->remote_allowed_atoms, "objectsize");
	যদি (results.types)
		string_list_append(&তথ্য->remote_allowed_atoms, "objecttype");

	তথ্য->skip_object_info = 1;
	জন্য (আকার_t i = 0; i < results.nr; i++) {
		তথ্য->oid = object_info_oids.oid[i];

		যদি (results.unrecognized[i]) {
			report_object_status(opt, oid_to_hex(&তথ্য->oid),
					     &তথ্য->oid, "missing");
			চালু;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		যদি (results.sizes)
			তথ্য->আকার_2 = results.sizes[i];

		যদি (results.types)
			তথ্য->type = results.types[i];

		opt->batch_mode = BATCH_MODE_INFO;
		তথ্য->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, তথ্য, ফাঁকা, 0);
		তথ্য->is_remote = 0;
	}
	তথ্য->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	মুক্ত(সারি_to_split);
	মুক্ত(argv);
	oid_array_clear(&object_info_oids);
}

স্থির শূন্য dispatch_calls(কাঠামো batch_options *opt,
		কাঠামো strbuf *output,
		কাঠামো expand_তথ্য *তথ্য,
		কাঠামো queued_cmd *cmd,
		আকার_t nr)
{
	যদি (!opt->বাফার_output)
		die(_("flush is only for --buffer mode"));

	জন্য (আকার_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].সারি, output, তথ্য);

	fflush(stdout);
}

স্থির শূন্য free_cmds(কাঠামো queued_cmd *cmd, আকার_t *nr)
{
	জন্য (আকার_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].সারি);

	*nr = 0;
}


স্থির ধ্রুবক কাঠামো parse_cmd {
	ধ্রুবক অক্ষর *name;
	parse_cmd_fn_t fn;
	unsigned takes_args;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ফাঁকা, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

স্থির শূন্য batch_objects_command(কাঠামো batch_options *opt,
				    কাঠামো strbuf *output,
				    কাঠামো expand_তথ্য *তথ্য)
{
	কাঠামো strbuf প্রবেশ = STRBUF_INIT;
	কাঠামো queued_cmd *queued_cmd = ফাঁকা;
	আকার_t alloc = 0, nr = 0;

	যখন (strbuf_getdelim_strip_crlf(&প্রবেশ, stdin, opt->প্রবেশ_delim) != EOF) {
		ধ্রুবক কাঠামো parse_cmd *cmd = ফাঁকা;
		ধ্রুবক অক্ষর *p = ফাঁকা, *cmd_end;
		কাঠামো queued_cmd call = {0};

		যদি (!প্রবেশ.len)
			die(_("empty command in input"));
		যদি (isspace(*প্রবেশ.buf))
			die(_("whitespace before command: '%s'"), প্রবেশ.buf);

		জন্য (আকার_t i = 0; i < ARRAY_আকার(commands); i++) {
			যদি (!skip_prefix(প্রবেশ.buf, commands[i].name, &cmd_end))
				চালু;

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

			বিরতি;
		}

		যদি (!cmd)
			die(_("unknown command: '%s'"), প্রবেশ.buf);

		যদি (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, তথ্য, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} অন্যথা যদি (!opt->বাফার_output) {
			cmd->fn(opt, p, output, তথ্য);
		} অন্যথা {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.সারি = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	যদি (opt->বাফার_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, তথ্য, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	মুক্ত(queued_cmd);
	strbuf_release(&প্রবেশ);
}

#define DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

প্রকারলেখ পূর্ণসংখ্যা (*for_each_object_fn)(ধ্রুবক কাঠামো object_id *oid, কাঠামো packed_git *pack,
				  off_t offset, শূন্য *তথ্য);

কাঠামো for_each_object_payload {
	for_each_object_fn callback;
	শূন্য *payload;
};

স্থির পূর্ণসংখ্যা batch_one_object_oi(ধ্রুবক কাঠামো object_id *oid,
			       কাঠামো object_info *oi,
			       শূন্য *_payload)
{
	কাঠামো for_each_object_payload *payload = _payload;
	যদি (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		ফেরত payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.offset,
					 payload->payload);
	ফেরত payload->callback(oid, ফাঁকা, 0, payload->payload);
}

স্থির শূন্য batch_each_object(কাঠামো batch_options *opt,
			      for_each_object_fn callback,
			      unsigned flags,
			      শূন্য *_payload)
{
	কাঠামো for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	কাঠামো odb_source_info source_info;
	কাঠামো object_info oi = {
		.source_infop = &source_info,
	};
	কাঠামো odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

স্থির পূর্ণসংখ্যা batch_objects(কাঠামো batch_options *opt)
{
	কাঠামো strbuf প্রবেশ = STRBUF_INIT;
	কাঠামো strbuf output = STRBUF_INIT;
	কাঠামো expand_তথ্য তথ্য = EXPAND_তথ্য_INIT;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণসংখ্যা save_warning;
	পূর্ণসংখ্যা retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	তথ্য.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &তথ্য);
	তথ্য.mark_query = 0;
	strbuf_release(&output);
	যদি (opt->transform_mode)
		তথ্য.split_on_whitespace = 1;

	যদি (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ফাঁকা;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	যদি (opt->batch_mode == BATCH_MODE_CONTENTS)
		তথ্য.info.typep = &তথ্য.type;

	যদি (opt->all_objects) {
		কাঠামো object_cb_তথ্য cb;
		কাঠামো object_info empty = OBJECT_INFO_INIT;

		যদি (!memcmp(&তথ্য.info, &empty, আকার(empty)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			তথ্য.skip_object_info = 1;

		যদি (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &তথ্য;
		cb.scratch = &output;

		যদি (opt->unordered) {
			কাঠামো oidset seen = OIDSET_INIT;

			cb.seen = &seen;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ORDER, &cb);

			oidset_clear(&seen);
		} অন্যথা {
			কাঠামো oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		ফেরত 0;
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
		batch_objects_command(opt, &output, &তথ্য);
		যাও cleanup;
	}

	যখন (strbuf_getdelim_strip_crlf(&প্রবেশ, stdin, opt->প্রবেশ_delim) != EOF) {
		যদি (তথ্য.split_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			অক্ষর *p = strpbrk(প্রবেশ.buf, " \t");
			যদি (p) {
				যখন (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			তথ্য.rest = p;
		}

		batch_one_object(প্রবেশ.buf, &output, opt, &তথ্য);
	}

 cleanup:
	strbuf_release(&প্রবেশ);
	strbuf_release(&output);
	string_list_clear(&তথ্য.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	ফেরত retval;
}

স্থির পূর্ণসংখ্যা git_cat_file_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			       ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (userdiff_config(var, মান) < 0)
		ফেরত -1;

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণসংখ্যা batch_option_callback(ধ্রুবক কাঠামো option *opt,
				 ধ্রুবক অক্ষর *arg,
				 পূর্ণসংখ্যা unset)
{
	কাঠামো batch_options *bo = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (bo->enabled) {
		ফেরত error(_("only one batch option may be specified"));
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

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_cat_file(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা opt = 0;
	পূর্ণসংখ্যা opt_cw = 0;
	পূর্ণসংখ্যা opt_epts = 0;
	ধ্রুবক অক্ষর *exp_type = ফাঁকা, *obj_name = ফাঁকা;
	কাঠামো batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	পূর্ণসংখ্যা unknown_type = 0;
	পূর্ণসংখ্যা প্রবেশ_nul_terminated = 0;
	পূর্ণসংখ্যা nul_terminated = 0;
	পূর্ণসংখ্যা ret;

	ধ্রুবক অক্ষর * ধ্রুবক builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ফাঁকা
	};
	ধ্রুবক কাঠামো option options[] = {
		/* Simple queries */
		OPT_GROUP(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ফাঁকা, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ফাঁকা, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_GROUP(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ফাঁকা, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ফাঁকা, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', ফাঁকা, &প্রবেশ_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ফাঁকা, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_GROUP(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.বাফার_output, N_("buffer --batch output")),
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

	repo_config(the_repository, git_cat_file_config, ফাঁকা);

	batch.বাফার_output = -1;

	argc = parse_options(argc, argv, prefix, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	যদি (use_mailmap)
		load_mailmap();

	নির্বাচন (batch.objects_filter.choice) {
	ক্ষেত্র LOFC_DISABLED:
		বিরতি;
	ক্ষেত্র LOFC_BLOB_NONE:
	ক্ষেত্র LOFC_BLOB_LIMIT:
	ক্ষেত্র LOFC_OBJECT_TYPE:
		যদি (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		বিরতি;
	পূর্বনির্ধারিত:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	যদি (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	যদি (force_path && !opt_cw)
		usage_বার্তা_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	যদি (batch.enabled)
		;
	অন্যথা যদি (batch.follow_symlinks)
		usage_বার্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	অন্যথা যদি (batch.বাফার_output >= 0)
		usage_বার্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	অন্যথা যদি (batch.all_objects)
		usage_বার্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	অন্যথা যদি (প্রবেশ_nul_terminated)
		usage_বার্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	অন্যথা যদি (nul_terminated)
		usage_বার্তা_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.প্রবেশ_delim = batch.output_delim = '\n';
	যদি (প্রবেশ_nul_terminated)
		batch.প্রবেশ_delim = '\0';
	যদি (nul_terminated)
		batch.প্রবেশ_delim = batch.output_delim = '\0';

	/* Batch defaults */
	যদি (batch.বাফার_output < 0)
		batch.বাফার_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_সূচক = 0;

	/* Return early if we're in batch mode? */
	যদি (batch.enabled) {
		যদি (opt_cw)
			batch.transform_mode = opt;
		অন্যথা যদি (opt && opt != 'b')
			usage_বার্তা_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		অন্যথা যদি (argc)
			usage_বার্তা_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		যাও out;
	}

	যদি (opt) {
		যদি (!argc && opt == 'c')
			usage_বার্তা_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		অন্যথা যদি (!argc && opt == 'w')
			usage_বার্তা_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		অন্যথা যদি (!argc && opt_epts)
			usage_বার্তা_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		অন্যথা যদি (argc == 1)
			obj_name = argv[0];
		অন্যথা
			usage_বার্তা_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} অন্যথা যদি (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} অন্যথা যদি (argc != 2) {
		usage_বার্তা_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} অন্যথা যদি (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

out:
	list_objects_filter_release(&batch.objects_filter);
	ফেরত ret;
}
