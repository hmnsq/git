/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "convert.h"
#ضمن "diff.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "ident.h"
#ضمن "list-objects-filter-options.h"
#ضمن "parse-options.h"
#ضمن "userdiff.h"
#ضمن "oid-array.h"
#ضمن "packfile.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "replace-object.h"
#ضمن "promisor-remote.h"
#ضمن "mailmap.h"
#ضمن "write-or-die.h"
#ضمن "alias.h"
#ضمن "remote.h"
#ضمن "transport.h"
#ضمن "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#تعریف_کن MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#تعریف_کن MAX_ALLOWED_OBJ_حد 10000

/* Maximum input size permitted for the remote-object-info command. */
#تعریف_کن MAX_REMOTE_OBJ_INFO_سطر \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_حد * (GIT_MAX_HEXSZ + 1))

تعداد batch_نما {
	BATCH_نما_CONTENTS,
	BATCH_نما_INFO,
	BATCH_نما_صف_AND_DISPATCH,
};

بنية batch_options {
	بنية list_objects_filter_options objects_filter;
	صحيح enabled;
	صحيح follow_symlinks;
	تعداد batch_نما batch_نما;
	صحيح میانگیر_output;
	صحيح all_objects;
	صحيح unordered;
	صحيح transform_نما; /* may be 'w' or 'c' for --filters or --textconv */
	حرف درون_داد_delim;
	حرف output_delim;
	ثابت حرف *format;
};

ساكن ثابت حرف *force_مسیر;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;
ساكن صحيح use_mailmap;

ساكن حرف *replace_idents_using_mailmap(حرف *, اندازه_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ساكن فراغ بارگیری_mailmap(فراغ)
{
	إذا (mailmap.strdup_رشتهها)
		أرجع;

	read_mailmap(the_repository, &mailmap);
}

ساكن حرف *replace_idents_using_mailmap(حرف *object_buf, اندازه_t *اندازه)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *سرآیندها[] = { "author ", "committer ", "tagger ", عدم };

	strbuf_attach(&sb, object_buf, *اندازه, *اندازه + 1);
	apply_mailmap_مقصد_سرآیند(&sb, سرآیندها, &mailmap);
	*اندازه = sb.len;
	أرجع strbuf_detach(&sb, عدم);
}

ساكن صحيح filter_object(ثابت حرف *مسیر, بیعلامت نما,
			 ثابت بنية object_id *oid,
			 حرف **buf, اندازه_t *اندازه)
{
	تعداد object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, اندازه);
	إذا (!*buf)
		أرجع error(_("cannot read object %s '%s'"),
			     oid_مقصد_hex(oid), مسیر);
	إذا ((type == OBJ_BLOB) && S_ISREG(نما)) {
		بنية strbuf strbuf = STRBUF_INIT;
		بنية checkout_metadata meta;

		init_checkout_metadata(&meta, عدم, عدم, oid);
		إذا (convert_مقصد_working_درخت(the_repository->شاخص, مسیر, *buf, *اندازه, &strbuf, &meta)) {
			حرر(*buf);
			*اندازه = strbuf.len;
			*buf = strbuf_detach(&strbuf, عدم);
		}
	}

	أرجع 0;
}

ساكن صحيح جریان_blob(ثابت بنية object_id *oid)
{
	إذا (odb_جریان_blob_مقصد_fd(the_repository->objects, 1, oid, عدم, 0))
		die("unable to stream %s to stdout", oid_مقصد_hex(oid));
	أرجع 0;
}

ساكن صحيح cat_one_file(صحيح opt, ثابت حرف *exp_type, ثابت حرف *obj_name)
{
	صحيح ret;
	بنية object_id oid;
	تعداد object_type type;
	حرف *buf;
	اندازه_t اندازه;
	بنية object_context obj_context = {0};
	بنية object_info oi = OBJECT_INFO_INIT;
	بیعلامت flags = OBJECT_INFO_LOOKUP_REPLACE;
	بیعلامت get_oid_flags =
		GET_OID_رکورد_مسیر |
		GET_OID_ONLY_مقصد_DIE |
		GET_OID_HASH_ANY;
	ثابت حرف *مسیر = force_مسیر;
	ثابت صحيح opt_cw = (opt == 'c' || opt == 'w');
	إذا (!مسیر && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_مسیر;

	إذا (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	إذا (!مسیر)
		مسیر = obj_context.مسیر;
	إذا (obj_context.نما == S_IFINVALID)
		obj_context.نما = 0100644;

	buf = عدم;
	بدل (opt) {
	حالة 't':
		oi.typep = &type;
		إذا (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		چاپ_کن("%s\n", type_name(type));
		ret = 0;
		اذهب cleanup;

	حالة 's':
		oi.sizep = &اندازه;

		إذا (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (فراغ**)&buf;
		}

		إذا (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		إذا (use_mailmap && (type == OBJ_COMMIT || type == OBJ_برچسب))
			buf = replace_idents_using_mailmap(buf, &اندازه);

		چاپ_کن("%"PRIuMAX"\n", (uintmax_t)اندازه);
		ret = 0;
		اذهب cleanup;

	حالة 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		اذهب cleanup;

	حالة 'w':

		إذا (filter_object(مسیر, obj_context.نما,
				  &oid, &buf, &اندازه)) {
			ret = -1;
			اذهب cleanup;
		}
		اكسر;

	حالة 'c':
	{
		بیعلامت دراز اندازه_ul = 0;
		صحيح textconv_ret = textconv_object(the_repository, مسیر,
						   obj_context.نما, &oid, 1,
						   &buf, &اندازه_ul);
		اندازه = اندازه_ul;
		إذا (textconv_ret)
			اكسر;
	}
		/* else fallthrough */

	حالة 'p':
		type = odb_read_object_info(the_repository->objects, &oid, عدم);
		إذا (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		إذا (type == OBJ_درخت) {
			ثابت حرف *ls_آرگومانها[3] = { عدم };
			ls_آرگومانها[0] =  "ls-tree";
			ls_آرگومانها[1] =  obj_name;
			ret = cmd_ls_درخت(2, ls_آرگومانها, عدم, the_repository);
			اذهب cleanup;
		}

		إذا (type == OBJ_BLOB) {
			ret = جریان_blob(&oid);
			اذهب cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &اندازه);
		إذا (!buf)
			die("Cannot read object %s", obj_name);

		إذا (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &اندازه);

		/* otherwise just spit out the data */
		اكسر;

	حالة 0:
	{
		تعداد object_type exp_type_id = type_from_string(exp_type);

		إذا (exp_type_id == OBJ_BLOB) {
			بنية object_id blob_oid;
			إذا (odb_read_object_info(the_repository->objects,
						 &oid, عدم) == OBJ_برچسب) {
				حرف *میانگیر = odb_read_object(the_repository->objects,
							       &oid, &type, &اندازه);
				ثابت حرف *هدف;

				إذا (!میانگیر)
					die(_("unable to read %s"), oid_مقصد_hex(&oid));

				إذا (!skip_پیشوند(میانگیر, "object ", &هدف) ||
				    get_oid_hex_algop(هدف, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_مقصد_hex(&oid));
				حرر(میانگیر);
			} وإلا
				oidcpy(&blob_oid, &oid);

			إذا (odb_read_object_info(the_repository->objects,
						 &blob_oid, عدم) == OBJ_BLOB) {
				ret = جریان_blob(&blob_oid);
				اذهب cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &اندازه, عدم);

		إذا (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &اندازه);
		اكسر;
	}
	افتراضي:
		die("git cat-file: unknown option: %s", exp_type);
	}

	إذا (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, اندازه);
	ret = 0;
cleanup:
	حرر(buf);
	object_context_release(&obj_context);
	أرجع ret;
}

بنية expand_داده {
	بنية object_id oid;
	تعداد object_type type;
	اندازه_t اندازه;
	بیعلامت کوتاه نما;
	off_t disk_اندازه;
	ثابت حرف *rest;
	بنية object_id delta_پایه_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	صحيح mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	صحيح تقسیم_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	بنية object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	بیعلامت skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	بیعلامت is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	بنية string_list remote_allowed_atoms;
};

#تعریف_کن EXPAND_داده_INIT  {  \
	.نما = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ساكن صحيح is_atom(ثابت حرف *atom, ثابت حرف *s, صحيح slen)
{
	صحيح alen = طول_رشته(atom);
	أرجع alen == slen && !memcmp(atom, s, alen);
}

ساكن صحيح expand_atom(بنية strbuf *sb, ثابت حرف *atom, صحيح len,
		       بنية expand_داده *داده)
{
	إذا (داده->is_remote) {
		اندازه_t i;
		لكل (i = 0; i < داده->remote_allowed_atoms.nr; i++)
			إذا (is_atom(داده->remote_allowed_atoms.اقلام[i].string,
				    atom, len))
				اكسر;
		إذا (i == داده->remote_allowed_atoms.nr)
			أرجع 1;
	}

	إذا (is_atom("objectname", atom, len)) {
		إذا (!داده->mark_query)
			strbuf_جمع_کن_oid_hex(sb, &داده->oid);
	} وإلا إذا (is_atom("objecttype", atom, len)) {
		إذا (داده->mark_query) {
			داده->info.typep = &داده->type;
		} وإلا {
			ثابت حرف *t = type_name(داده->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} وإلا إذا (is_atom("objectsize", atom, len)) {
		إذا (داده->mark_query)
			داده->info.sizep = &داده->اندازه;
		وإلا
			strbuf_جمع_کن_uint(sb, داده->اندازه);
	} وإلا إذا (is_atom("objectsize:disk", atom, len)) {
		إذا (داده->mark_query)
			داده->info.disk_sizep = &داده->disk_اندازه;
		وإلا
			strbuf_جمع_کن_uint(sb, داده->disk_اندازه);
	} وإلا إذا (is_atom("rest", atom, len)) {
		إذا (داده->mark_query)
			داده->تقسیم_on_whitespace = 1;
		وإلا إذا (داده->rest)
			strbuf_addstr(sb, داده->rest);
	} وإلا إذا (is_atom("deltabase", atom, len)) {
		إذا (داده->mark_query)
			داده->info.delta_پایه_oid = &داده->delta_پایه_oid;
		وإلا
			strbuf_جمع_کن_oid_hex(sb, &داده->delta_پایه_oid);
	} وإلا إذا (is_atom("objectmode", atom, len)) {
		إذا (!داده->mark_query && !(S_IFINVALID == داده->نما))
			strbuf_addf(sb, "%06o", داده->نما);
	} وإلا
		أرجع 0;
	أرجع 1;
}

ساكن فراغ expand_format(بنية strbuf *sb, ثابت حرف *شروع,
			  بنية expand_داده *داده)
{
	طالما (strbuf_expand_گام(sb, &شروع)) {
		ثابت حرف *انتها;

		إذا (skip_پیشوند(شروع, "%", &شروع) || *شروع != '(')
			strbuf_addch(sb, '%');
		وإلا إذا ((انتها = strchr(شروع + 1, ')')) &&
			 expand_atom(sb, شروع + 1, انتها - شروع - 1, داده))
			شروع = انتها + 1;
		وإلا
			strbuf_expand_bad_format(شروع, "cat-file");
	}
}

ساكن فراغ batch_write(بنية batch_options *opt, ثابت فراغ *داده, صحيح len)
{
	إذا (opt->میانگیر_output) {
		إذا (fwrite(داده, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} وإلا
		write_or_die(1, داده, len);
}

ساكن فراغ print_object_or_die(بنية batch_options *opt, بنية expand_داده *داده)
{
	ثابت بنية object_id *oid = &داده->oid;

	أكد(داده->info.typep);

	إذا (داده->type == OBJ_BLOB) {
		إذا (opt->میانگیر_output)
			fflush(stdout);
		إذا (opt->transform_نما) {
			حرف *contents;
			اندازه_t اندازه;

			إذا (!داده->rest)
				die("missing path for '%s'", oid_مقصد_hex(oid));

			إذا (opt->transform_نما == 'w') {
				إذا (filter_object(داده->rest, 0100644, oid,
						  &contents, &اندازه))
					die("could not convert '%s' %s",
					    oid_مقصد_hex(oid), داده->rest);
			} وإلا إذا (opt->transform_نما == 'c') {
				تعداد object_type type;
				بیعلامت دراز اندازه_ul = 0;
				إذا (textconv_object(the_repository,
						    داده->rest, 0100644, oid,
						    1, &contents, &اندازه_ul))
					اندازه = اندازه_ul;
				وإلا
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &اندازه);
				إذا (!contents)
					die("could not convert '%s' %s",
					    oid_مقصد_hex(oid), داده->rest);
			} وإلا
				BUG("invalid transform_mode: %c", opt->transform_نما);
			batch_write(opt, contents, اندازه);
			حرر(contents);
		} وإلا {
			جریان_blob(oid);
		}
	}
	وإلا {
		تعداد object_type type;
		اندازه_t اندازه;
		فراغ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &اندازه);
		إذا (!contents)
			die("object %s disappeared", oid_مقصد_hex(oid));

		إذا (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &اندازه);

		إذا (type != داده->type)
			die("object %s changed type!?", oid_مقصد_hex(oid));
		إذا (داده->info.sizep && اندازه != داده->اندازه && !use_mailmap)
			die("object %s changed size!?", oid_مقصد_hex(oid));

		batch_write(opt, contents, اندازه);
		حرر(contents);
	}
}

ساكن فراغ print_default_format(بنية strbuf *scratch, بنية expand_داده *داده,
				 بنية batch_options *opt)
{
	strbuf_جمع_کن_oid_hex(scratch, &داده->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(داده->type));
	strbuf_addch(scratch, ' ');
	strbuf_جمع_کن_uint(scratch, داده->اندازه);
	strbuf_addch(scratch, opt->output_delim);
}

ساكن فراغ report_object_وضعیت(بنية batch_options *opt,
				 ثابت حرف *obj_name,
				 ثابت بنية object_id *oid,
				 ثابت حرف *وضعیت)
{
	چاپ_کن("%s %s%c", obj_name ? obj_name : oid_مقصد_hex(oid),
	       وضعیت, opt->output_delim);
	fflush(stdout);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
ساكن فراغ batch_object_write(ثابت حرف *obj_name,
			       بنية strbuf *scratch,
			       بنية batch_options *opt,
			       بنية expand_داده *داده,
			       بنية packed_git *pack,
			       off_t جابه_جایی)
{
	إذا (!داده->skip_object_info) {
		صحيح ret;

		إذا (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_حد ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			داده->info.typep = &داده->type;
		إذا (opt->objects_filter.choice == LOFC_BLOB_حد)
			داده->info.sizep = &داده->اندازه;

		إذا (pack)
			ret = packed_object_info(عدم, pack, جابه_جایی, &داده->info);
		وإلا
			ret = odb_read_object_info_extended(the_repository->objects,
							    &داده->oid, &داده->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		إذا (ret < 0) {
			إذا (داده->نما == S_IFGITLINK)
				report_object_وضعیت(opt, عدم, &داده->oid, "submodule");
			وإلا
				report_object_وضعیت(opt, obj_name, &داده->oid, "missing");
			أرجع;
		}

		بدل (opt->objects_filter.choice) {
		حالة LOFC_DISABLED:
			اكسر;
		حالة LOFC_BLOB_NONE:
			إذا (داده->type == OBJ_BLOB) {
				إذا (!opt->all_objects)
					report_object_وضعیت(opt, obj_name,
							     &داده->oid, "excluded");
				أرجع;
			}
			اكسر;
		حالة LOFC_BLOB_حد:
			إذا (داده->type == OBJ_BLOB &&
			    داده->اندازه >= opt->objects_filter.blob_حد_مقدار) {
				إذا (!opt->all_objects)
					report_object_وضعیت(opt, obj_name,
							     &داده->oid, "excluded");
				أرجع;
			}
			اكسر;
		حالة LOFC_OBJECT_TYPE:
			إذا (داده->type != opt->objects_filter.object_type) {
				إذا (!opt->all_objects)
					report_object_وضعیت(opt, obj_name,
							     &داده->oid, "excluded");
				أرجع;
			}
			اكسر;
		افتراضي:
			BUG("unsupported objects filter");
		}

		إذا (use_mailmap && (داده->type == OBJ_COMMIT || داده->type == OBJ_برچسب)) {
			حرف *buf = عدم;

			buf = odb_read_object(the_repository->objects, &داده->oid,
					      &داده->type, &داده->اندازه);
			إذا (!buf)
				die(_("unable to read %s"), oid_مقصد_hex(&داده->oid));
			buf = replace_idents_using_mailmap(buf, &داده->اندازه);

			حرر(buf);
		}
	}

	strbuf_reset(scratch);

	إذا (!opt->format) {
		print_default_format(scratch, داده, opt);
	} وإلا {
		expand_format(scratch, opt->format, داده);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	إذا (opt->batch_نما == BATCH_نما_CONTENTS) {
		print_object_or_die(opt, داده);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ساكن فراغ batch_one_object(ثابت حرف *obj_name,
			     بنية strbuf *scratch,
			     بنية batch_options *opt,
			     بنية expand_داده *داده)
{
	بنية object_context ctx = {0};
	صحيح flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	تعداد get_oid_نتیجه نتیجه;

	نتیجه = get_oid_with_context(the_repository, obj_name,
				      flags, &داده->oid, &ctx);
	إذا (نتیجه != یافت) {
		بدل (نتیجه) {
		حالة MISSING_OBJECT:
			report_object_وضعیت(opt, obj_name, &داده->oid, "missing");
			اكسر;
		حالة SHORT_NAME_AMBIGUOUS:
			report_object_وضعیت(opt, obj_name, &داده->oid, "ambiguous");
			اكسر;
		حالة DANGLING_SYMLINK:
			چاپ_کن("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)طول_رشته(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة SYMLINK_LOOP:
			چاپ_کن("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)طول_رشته(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة NOT_DIR:
			چاپ_کن("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)طول_رشته(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		افتراضي:
			BUG("unknown get_sha1_with_context result %d\n",
			       نتیجه);
			اكسر;
		}
		fflush(stdout);

		اذهب بیرون;
	}

	إذا (ctx.نما == 0) {
		چاپ_کن("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_مسیر.len,
		       opt->output_delim, ctx.symlink_مسیر.buf, opt->output_delim);
		fflush(stdout);
		اذهب بیرون;
	}

	داده->نما = ctx.نما;
	batch_object_write(obj_name, scratch, opt, داده, عدم, 0);

بیرون:
	object_context_release(&ctx);
}

ساكن صحيح get_remote_info(صحيح argc,
			   ثابت حرف **argv,
			   بنية fetch_object_info_results *results,
			   بنية oid_array *object_info_oids)
{
	صحيح retval = 0;
	بنية remote *remote = عدم;
	بنية object_id oid;
	بنية transport *gtransport;

	remote = remote_get(argv[0]);
	إذا (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	لكل (اندازه_t i = 1; i < argc; i++) {
		إذا (get_oid_hex(argv[i], &oid)) {
			اندازه_t len = طول_رشته(argv[i]);

			إذا (len < the_hash_algo->hexsz && len >= 4) {
				اندازه_t j;
				لكل (j = 0; j < len; j++)
					إذا (!isxdigit(argv[i][j]))
						اكسر;
				إذا (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (صحيح)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	إذا (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, عدم);

	إذا (!gtransport->smart_options) {
		retval = -1;
		اذهب cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	أرجع retval;
}

بنية object_cb_داده {
	بنية batch_options *opt;
	بنية expand_داده *expand;
	بنية oidset *دیده;
	بنية strbuf *scratch;
};

ساكن صحيح batch_object_cb(ثابت بنية object_id *oid, فراغ *vdata)
{
	بنية object_cb_داده *داده = vdata;
	oidcpy(&داده->expand->oid, oid);
	batch_object_write(عدم, داده->scratch, داده->opt, داده->expand,
			   عدم, 0);
	أرجع 0;
}

ساكن صحيح collect_object(ثابت بنية object_id *oid,
			  بنية packed_git *pack استفادهنشده,
			  off_t جابه_جایی استفادهنشده,
			  فراغ *داده)
{
	oid_array_append(داده, oid);
	أرجع 0;
}

ساكن صحيح batch_unordered_object(ثابت بنية object_id *oid,
				  بنية packed_git *pack,
				  off_t جابه_جایی,
				  فراغ *vdata)
{
	بنية object_cb_داده *داده = vdata;

	إذا (oidset_درج(داده->دیده, oid))
		أرجع 0;

	oidcpy(&داده->expand->oid, oid);
	batch_object_write(عدم, داده->scratch, داده->opt, داده->expand,
			   pack, جابه_جایی);
	أرجع 0;
}

تعريفنوع فراغ (*parse_cmd_fn_t)(بنية batch_options *, ثابت حرف *,
			       بنية strbuf *, بنية expand_داده *);

بنية queued_cmd {
	parse_cmd_fn_t fn;
	حرف *سطر;
};

ساكن فراغ parse_cmd_contents(بنية batch_options *opt,
			     ثابت حرف *سطر,
			     بنية strbuf *output,
			     بنية expand_داده *داده)
{
	تعداد object_type *saved_typep = داده->info.typep;
	داده->info.typep = &داده->type;
	opt->batch_نما = BATCH_نما_CONTENTS;
	batch_one_object(سطر, output, opt, داده);
	داده->info.typep = saved_typep;
}

ساكن فراغ parse_cmd_info(بنية batch_options *opt,
			   ثابت حرف *سطر,
			   بنية strbuf *output,
			   بنية expand_داده *داده)
{
	opt->batch_نما = BATCH_نما_INFO;
	batch_one_object(سطر, output, opt, داده);
}

ساكن فراغ parse_cmd_mailmap(بنية batch_options *opt استفادهنشده,
			      ثابت حرف *سطر,
			      بنية strbuf *output استفادهنشده,
			      بنية expand_داده *داده استفادهنشده)
{
	use_mailmap = git_parse_maybe_bool(سطر);

	إذا (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), سطر);

	إذا (use_mailmap)
		بارگیری_mailmap();
}

ساكن فراغ parse_cmd_remote_object_info(بنية batch_options *opt,
					 ثابت حرف *سطر, بنية strbuf *output,
					 بنية expand_داده *داده)
{
	صحيح شمار;
	ثابت حرف **argv;
	حرف *سطر_مقصد_تقسیم;
	بنية fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	بنية oid_array object_info_oids = OID_ARRAY_INIT;

	إذا (طول_رشته(سطر) >= MAX_REMOTE_OBJ_INFO_سطر)
		die(_("remote-object-info command too long"));

	سطر_مقصد_تقسیم = xstrdup(سطر);
	شمار = تقسیم_cmdline(سطر_مقصد_تقسیم, &argv);
	إذا (شمار < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    تقسیم_cmdline_strerror(شمار));
	إذا (شمار - 1 > MAX_ALLOWED_OBJ_حد)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_حد);

	إذا (داده->info.sizep)
		results.میخواهد_اندازه = 1;
	إذا (داده->info.typep)
		results.میخواهد_type = 1;

	إذا (get_remote_info(شمار, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&داده->remote_allowed_atoms, 0);
	string_list_append(&داده->remote_allowed_atoms, "objectname");
	إذا (results.sizes)
		string_list_append(&داده->remote_allowed_atoms, "objectsize");
	إذا (results.types)
		string_list_append(&داده->remote_allowed_atoms, "objecttype");

	داده->skip_object_info = 1;
	لكل (اندازه_t i = 0; i < results.nr; i++) {
		داده->oid = object_info_oids.oid[i];

		إذا (results.unrecognized[i]) {
			report_object_وضعیت(opt, oid_مقصد_hex(&داده->oid),
					     &داده->oid, "missing");
			استمر;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		إذا (results.sizes)
			داده->اندازه = results.sizes[i];

		إذا (results.types)
			داده->type = results.types[i];

		opt->batch_نما = BATCH_نما_INFO;
		داده->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, داده, عدم, 0);
		داده->is_remote = 0;
	}
	داده->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	حرر(سطر_مقصد_تقسیم);
	حرر(argv);
	oid_array_clear(&object_info_oids);
}

ساكن فراغ dispatch_calls(بنية batch_options *opt,
		بنية strbuf *output,
		بنية expand_داده *داده,
		بنية queued_cmd *cmd,
		اندازه_t nr)
{
	إذا (!opt->میانگیر_output)
		die(_("flush is only for --buffer mode"));

	لكل (اندازه_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].سطر, output, داده);

	fflush(stdout);
}

ساكن فراغ free_cmds(بنية queued_cmd *cmd, اندازه_t *nr)
{
	لكل (اندازه_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].سطر);

	*nr = 0;
}


ساكن ثابت بنية parse_cmd {
	ثابت حرف *name;
	parse_cmd_fn_t fn;
	بیعلامت takes_آرگومانها;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", عدم, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ساكن فراغ batch_objects_command(بنية batch_options *opt,
				    بنية strbuf *output,
				    بنية expand_داده *داده)
{
	بنية strbuf درون_داد = STRBUF_INIT;
	بنية queued_cmd *queued_cmd = عدم;
	اندازه_t alloc = 0, nr = 0;

	طالما (strbuf_getdelim_strip_crlf(&درون_داد, stdin, opt->درون_داد_delim) != EOF) {
		ثابت بنية parse_cmd *cmd = عدم;
		ثابت حرف *p = عدم, *cmd_انتها;
		بنية queued_cmd call = {0};

		إذا (!درون_داد.len)
			die(_("empty command in input"));
		إذا (آیا_فاصله(*درون_داد.buf))
			die(_("whitespace before command: '%s'"), درون_داد.buf);

		لكل (اندازه_t i = 0; i < ARRAY_اندازه(commands); i++) {
			إذا (!skip_پیشوند(درون_داد.buf, commands[i].name, &cmd_انتها))
				استمر;

			cmd = &commands[i];
			إذا (cmd->takes_آرگومانها) {
				إذا (*cmd_انتها != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_انتها + 1;
			} وإلا إذا (*cmd_انتها) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			اكسر;
		}

		إذا (!cmd)
			die(_("unknown command: '%s'"), درون_داد.buf);

		إذا (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, داده, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} وإلا إذا (!opt->میانگیر_output) {
			cmd->fn(opt, p, output, داده);
		} وإلا {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.سطر = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	إذا (opt->میانگیر_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, داده, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	حرر(queued_cmd);
	strbuf_release(&درون_داد);
}

#تعریف_کن DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

تعريفنوع صحيح (*for_each_object_fn)(ثابت بنية object_id *oid, بنية packed_git *pack,
				  off_t جابه_جایی, فراغ *داده);

بنية for_each_object_payload {
	for_each_object_fn callback;
	فراغ *payload;
};

ساكن صحيح batch_one_object_oi(ثابت بنية object_id *oid,
			       بنية object_info *oi,
			       فراغ *_payload)
{
	بنية for_each_object_payload *payload = _payload;
	إذا (oi && oi->منبع_infop->منبع->type == ODB_منبع_PACKED)
		أرجع payload->callback(oid, oi->منبع_infop->u.packed.pack,
					 oi->منبع_infop->u.packed.جابه_جایی,
					 payload->payload);
	أرجع payload->callback(oid, عدم, 0, payload->payload);
}

ساكن فراغ batch_each_object(بنية batch_options *opt,
			      for_each_object_fn callback,
			      بیعلامت flags,
			      فراغ *_payload)
{
	بنية for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	بنية odb_منبع_info منبع_info;
	بنية object_info oi = {
		.منبع_infop = &منبع_info,
	};
	بنية odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ساكن صحيح batch_objects(بنية batch_options *opt)
{
	بنية strbuf درون_داد = STRBUF_INIT;
	بنية strbuf output = STRBUF_INIT;
	بنية expand_داده داده = EXPAND_داده_INIT;
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);
	صحيح save_warning;
	صحيح retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	داده.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &داده);
	داده.mark_query = 0;
	strbuf_release(&output);
	إذا (opt->transform_نما)
		داده.تقسیم_on_whitespace = 1;

	إذا (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = عدم;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	إذا (opt->batch_نما == BATCH_نما_CONTENTS)
		داده.info.typep = &داده.type;

	إذا (opt->all_objects) {
		بنية object_cb_داده cb;
		بنية object_info خالی = OBJECT_INFO_INIT;

		إذا (!memcmp(&داده.info, &خالی, حجم(خالی)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			داده.skip_object_info = 1;

		إذا (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &داده;
		cb.scratch = &output;

		إذا (opt->unordered) {
			بنية oidset دیده = OIDSET_INIT;

			cb.دیده = &دیده;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ترتیب, &cb);

			oidset_clear(&دیده);
		} وإلا {
			بنية oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		أرجع 0;
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

	إذا (opt->batch_نما == BATCH_نما_صف_AND_DISPATCH) {
		batch_objects_command(opt, &output, &داده);
		اذهب cleanup;
	}

	طالما (strbuf_getdelim_strip_crlf(&درون_داد, stdin, opt->درون_داد_delim) != EOF) {
		إذا (داده.تقسیم_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			حرف *p = strpbrk(درون_داد.buf, " \t");
			إذا (p) {
				طالما (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			داده.rest = p;
		}

		batch_one_object(درون_داد.buf, &output, opt, &داده);
	}

 cleanup:
	strbuf_release(&درون_داد);
	strbuf_release(&output);
	string_list_clear(&داده.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	أرجع retval;
}

ساكن صحيح git_cat_file_config(ثابت حرف *var, ثابت حرف *مقدار,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (userdiff_config(var, مقدار) < 0)
		أرجع -1;

	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن صحيح batch_option_callback(ثابت بنية option *opt,
				 ثابت حرف *arg,
				 صحيح unset)
{
	بنية batch_options *bo = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	إذا (bo->enabled) {
		أرجع error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	إذا (!strcmp(opt->long_name, "batch"))
		bo->batch_نما = BATCH_نما_CONTENTS;
	وإلا إذا (!strcmp(opt->long_name, "batch-check"))
		bo->batch_نما = BATCH_نما_INFO;
	وإلا إذا (!strcmp(opt->long_name, "batch-command"))
		bo->batch_نما = BATCH_نما_صف_AND_DISPATCH;
	وإلا
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	أرجع 0;
}

صحيح cmd_cat_file(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *پیشوند,
		 بنية repository *repo استفادهنشده)
{
	صحيح opt = 0;
	صحيح opt_cw = 0;
	صحيح opt_epts = 0;
	ثابت حرف *exp_type = عدم, *obj_name = عدم;
	بنية batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	صحيح unknown_type = 0;
	صحيح درون_داد_nul_terminated = 0;
	صحيح nul_terminated = 0;
	صحيح ret;

	ثابت حرف * ثابت builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		عدم
	};
	ثابت بنية option options[] = {
		/* Simple queries */
		OPT_گروه(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', عدم, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', عدم, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_گروه(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', عدم, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', عدم, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_گروه(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', عدم, &درون_داد_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', عدم, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_گروه(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.میانگیر_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_گروه(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_مسیر, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_انتها()
	};

	repo_config(the_repository, git_cat_file_config, عدم);

	batch.میانگیر_output = -1;

	argc = parse_options(argc, argv, پیشوند, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	إذا (use_mailmap)
		بارگیری_mailmap();

	بدل (batch.objects_filter.choice) {
	حالة LOFC_DISABLED:
		اكسر;
	حالة LOFC_BLOB_NONE:
	حالة LOFC_BLOB_حد:
	حالة LOFC_OBJECT_TYPE:
		إذا (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		اكسر;
	افتراضي:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	إذا (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	إذا (force_مسیر && !opt_cw)
		usage_پیام_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	إذا (batch.enabled)
		;
	وإلا إذا (batch.follow_symlinks)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	وإلا إذا (batch.میانگیر_output >= 0)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	وإلا إذا (batch.all_objects)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	وإلا إذا (درون_داد_nul_terminated)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	وإلا إذا (nul_terminated)
		usage_پیام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.درون_داد_delim = batch.output_delim = '\n';
	إذا (درون_داد_nul_terminated)
		batch.درون_داد_delim = '\0';
	إذا (nul_terminated)
		batch.درون_داد_delim = batch.output_delim = '\0';

	/* Batch defaults */
	إذا (batch.میانگیر_output < 0)
		batch.میانگیر_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_پر_شاخص = 0;

	/* Return early if we're in batch mode? */
	إذا (batch.enabled) {
		إذا (opt_cw)
			batch.transform_نما = opt;
		وإلا إذا (opt && opt != 'b')
			usage_پیام_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		وإلا إذا (argc)
			usage_پیام_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		اذهب بیرون;
	}

	إذا (opt) {
		إذا (!argc && opt == 'c')
			usage_پیام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		وإلا إذا (!argc && opt == 'w')
			usage_پیام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		وإلا إذا (!argc && opt_epts)
			usage_پیام_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		وإلا إذا (argc == 1)
			obj_name = argv[0];
		وإلا
			usage_پیام_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} وإلا إذا (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} وإلا إذا (argc != 2) {
		usage_پیام_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} وإلا إذا (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

بیرون:
	list_objects_filter_release(&batch.objects_filter);
	أرجع ret;
}
