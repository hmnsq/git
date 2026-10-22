/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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
#تعریف_کرو MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#تعریف_کرو MAX_ALLOWED_OBJ_حد_بندی 10000

/* Maximum input size permitted for the remote-object-info command. */
#تعریف_کرو MAX_REMOTE_OBJ_INFO_سطر \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_حد_بندی * (GIT_MAX_HEXSZ + 1))

تعداد batch_منوال {
	BATCH_منوال_CONTENTS,
	BATCH_منوال_INFO,
	BATCH_منوال_قطار_AND_DISPATCH,
};

بنية batch_options {
	بنية list_objects_filter_options objects_filter;
	صحيح enabled;
	صحيح follow_symlinks;
	تعداد batch_منوال batch_منوال;
	صحيح بفر_output;
	صحيح all_objects;
	صحيح unordered;
	صحيح transform_منوال; /* may be 'w' or 'c' for --filters or --textconv */
	حرف داخلہ_delim;
	حرف output_delim;
	ثابت حرف *format;
};

ساكن ثابت حرف *force_رستہ;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;
ساكن صحيح use_mailmap;

ساكن حرف *replace_idents_using_mailmap(حرف *, ناپ_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ساكن فراغ لدو_mailmap(فراغ)
{
	إذا (mailmap.strdup_لڑیاں)
		أرجع;

	read_mailmap(the_repository, &mailmap);
}

ساكن حرف *replace_idents_using_mailmap(حرف *object_buf, ناپ_t *ناپ)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *سرخیاں[] = { "author ", "committer ", "tagger ", عدم };

	strbuf_attach(&sb, object_buf, *ناپ, *ناپ + 1);
	apply_mailmap_منزل_سرخی(&sb, سرخیاں, &mailmap);
	*ناپ = sb.len;
	أرجع strbuf_detach(&sb, عدم);
}

ساكن صحيح filter_object(ثابت حرف *رستہ, بنانشانی منوال,
			 ثابت بنية object_id *oid,
			 حرف **buf, ناپ_t *ناپ)
{
	تعداد object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, ناپ);
	إذا (!*buf)
		أرجع error(_("cannot read object %s '%s'"),
			     oid_منزل_hex(oid), رستہ);
	إذا ((type == OBJ_BLOB) && S_ISREG(منوال)) {
		بنية strbuf strbuf = STRBUF_INIT;
		بنية checkout_metadata meta;

		init_checkout_metadata(&meta, عدم, عدم, oid);
		إذا (convert_منزل_working_درخت(the_repository->اشاریہ, رستہ, *buf, *ناپ, &strbuf, &meta)) {
			حرر(*buf);
			*ناپ = strbuf.len;
			*buf = strbuf_detach(&strbuf, عدم);
		}
	}

	أرجع 0;
}

ساكن صحيح دھارا_blob(ثابت بنية object_id *oid)
{
	إذا (odb_دھارا_blob_منزل_fd(the_repository->objects, 1, oid, عدم, 0))
		die("unable to stream %s to stdout", oid_منزل_hex(oid));
	أرجع 0;
}

ساكن صحيح cat_one_file(صحيح opt, ثابت حرف *exp_type, ثابت حرف *obj_name)
{
	صحيح ret;
	بنية object_id oid;
	تعداد object_type type;
	حرف *buf;
	ناپ_t ناپ;
	بنية object_context obj_context = {0};
	بنية object_info oi = OBJECT_INFO_INIT;
	بنانشانی flags = OBJECT_INFO_LOOKUP_REPLACE;
	بنانشانی get_oid_flags =
		GET_OID_ریکارڈ_رستہ |
		GET_OID_ONLY_منزل_DIE |
		GET_OID_HASH_ANY;
	ثابت حرف *رستہ = force_رستہ;
	ثابت صحيح opt_cw = (opt == 'c' || opt == 'w');
	إذا (!رستہ && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_رستہ;

	إذا (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	إذا (!رستہ)
		رستہ = obj_context.رستہ;
	إذا (obj_context.منوال == S_IFINVALID)
		obj_context.منوال = 0100644;

	buf = عدم;
	بدل (opt) {
	حالة 't':
		oi.typep = &type;
		إذا (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		لکھو("%s\n", type_name(type));
		ret = 0;
		اذهب cleanup;

	حالة 's':
		oi.sizep = &ناپ;

		إذا (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (فراغ**)&buf;
		}

		إذا (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		إذا (use_mailmap && (type == OBJ_COMMIT || type == OBJ_ٹیگ))
			buf = replace_idents_using_mailmap(buf, &ناپ);

		لکھو("%"PRIuMAX"\n", (uintmax_t)ناپ);
		ret = 0;
		اذهب cleanup;

	حالة 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		اذهب cleanup;

	حالة 'w':

		إذا (filter_object(رستہ, obj_context.منوال,
				  &oid, &buf, &ناپ)) {
			ret = -1;
			اذهب cleanup;
		}
		اكسر;

	حالة 'c':
	{
		بنانشانی لما ناپ_ul = 0;
		صحيح textconv_ret = textconv_object(the_repository, رستہ,
						   obj_context.منوال, &oid, 1,
						   &buf, &ناپ_ul);
		ناپ = ناپ_ul;
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
			ثابت حرف *ls_دلیلاں[3] = { عدم };
			ls_دلیلاں[0] =  "ls-tree";
			ls_دلیلاں[1] =  obj_name;
			ret = cmd_ls_درخت(2, ls_دلیلاں, عدم, the_repository);
			اذهب cleanup;
		}

		إذا (type == OBJ_BLOB) {
			ret = دھارا_blob(&oid);
			اذهب cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &ناپ);
		إذا (!buf)
			die("Cannot read object %s", obj_name);

		إذا (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &ناپ);

		/* otherwise just spit out the data */
		اكسر;

	حالة 0:
	{
		تعداد object_type exp_type_id = type_from_string(exp_type);

		إذا (exp_type_id == OBJ_BLOB) {
			بنية object_id blob_oid;
			إذا (odb_read_object_info(the_repository->objects,
						 &oid, عدم) == OBJ_ٹیگ) {
				حرف *بفر = odb_read_object(the_repository->objects,
							       &oid, &type, &ناپ);
				ثابت حرف *نشانہ;

				إذا (!بفر)
					die(_("unable to read %s"), oid_منزل_hex(&oid));

				إذا (!skip_اگیتر(بفر, "object ", &نشانہ) ||
				    get_oid_hex_algop(نشانہ, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_منزل_hex(&oid));
				حرر(بفر);
			} وإلا
				oidcpy(&blob_oid, &oid);

			إذا (odb_read_object_info(the_repository->objects,
						 &blob_oid, عدم) == OBJ_BLOB) {
				ret = دھارا_blob(&blob_oid);
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
					     exp_type_id, &ناپ, عدم);

		إذا (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &ناپ);
		اكسر;
	}
	افتراضي:
		die("git cat-file: unknown option: %s", exp_type);
	}

	إذا (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, ناپ);
	ret = 0;
cleanup:
	حرر(buf);
	object_context_release(&obj_context);
	أرجع ret;
}

بنية expand_انکڑے {
	بنية object_id oid;
	تعداد object_type type;
	ناپ_t ناپ;
	بنانشانی چھوٹا منوال;
	off_t disk_ناپ;
	ثابت حرف *rest;
	بنية object_id delta_ادھار_oid;

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
	صحيح ونڈو_on_whitespace;

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
	بنانشانی skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	بنانشانی is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	بنية string_list remote_allowed_atoms;
};

#تعریف_کرو EXPAND_انکڑے_INIT  {  \
	.منوال = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ساكن صحيح is_atom(ثابت حرف *atom, ثابت حرف *s, صحيح slen)
{
	صحيح alen = لڑی_لمبائی(atom);
	أرجع alen == slen && !memcmp(atom, s, alen);
}

ساكن صحيح expand_atom(بنية strbuf *sb, ثابت حرف *atom, صحيح len,
		       بنية expand_انکڑے *انکڑے)
{
	إذا (انکڑے->is_remote) {
		ناپ_t i;
		لكل (i = 0; i < انکڑے->remote_allowed_atoms.nr; i++)
			إذا (is_atom(انکڑے->remote_allowed_atoms.اشیاء[i].string,
				    atom, len))
				اكسر;
		إذا (i == انکڑے->remote_allowed_atoms.nr)
			أرجع 1;
	}

	إذا (is_atom("objectname", atom, len)) {
		إذا (!انکڑے->mark_query)
			strbuf_جوڑو_oid_hex(sb, &انکڑے->oid);
	} وإلا إذا (is_atom("objecttype", atom, len)) {
		إذا (انکڑے->mark_query) {
			انکڑے->info.typep = &انکڑے->type;
		} وإلا {
			ثابت حرف *t = type_name(انکڑے->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} وإلا إذا (is_atom("objectsize", atom, len)) {
		إذا (انکڑے->mark_query)
			انکڑے->info.sizep = &انکڑے->ناپ;
		وإلا
			strbuf_جوڑو_uint(sb, انکڑے->ناپ);
	} وإلا إذا (is_atom("objectsize:disk", atom, len)) {
		إذا (انکڑے->mark_query)
			انکڑے->info.disk_sizep = &انکڑے->disk_ناپ;
		وإلا
			strbuf_جوڑو_uint(sb, انکڑے->disk_ناپ);
	} وإلا إذا (is_atom("rest", atom, len)) {
		إذا (انکڑے->mark_query)
			انکڑے->ونڈو_on_whitespace = 1;
		وإلا إذا (انکڑے->rest)
			strbuf_addstr(sb, انکڑے->rest);
	} وإلا إذا (is_atom("deltabase", atom, len)) {
		إذا (انکڑے->mark_query)
			انکڑے->info.delta_ادھار_oid = &انکڑے->delta_ادھار_oid;
		وإلا
			strbuf_جوڑو_oid_hex(sb, &انکڑے->delta_ادھار_oid);
	} وإلا إذا (is_atom("objectmode", atom, len)) {
		إذا (!انکڑے->mark_query && !(S_IFINVALID == انکڑے->منوال))
			strbuf_addf(sb, "%06o", انکڑے->منوال);
	} وإلا
		أرجع 0;
	أرجع 1;
}

ساكن فراغ expand_format(بنية strbuf *sb, ثابت حرف *شروع,
			  بنية expand_انکڑے *انکڑے)
{
	طالما (strbuf_expand_قدم(sb, &شروع)) {
		ثابت حرف *سرا;

		إذا (skip_اگیتر(شروع, "%", &شروع) || *شروع != '(')
			strbuf_addch(sb, '%');
		وإلا إذا ((سرا = strchr(شروع + 1, ')')) &&
			 expand_atom(sb, شروع + 1, سرا - شروع - 1, انکڑے))
			شروع = سرا + 1;
		وإلا
			strbuf_expand_bad_format(شروع, "cat-file");
	}
}

ساكن فراغ batch_write(بنية batch_options *opt, ثابت فراغ *انکڑے, صحيح len)
{
	إذا (opt->بفر_output) {
		إذا (fwrite(انکڑے, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} وإلا
		write_or_die(1, انکڑے, len);
}

ساكن فراغ print_object_or_die(بنية batch_options *opt, بنية expand_انکڑے *انکڑے)
{
	ثابت بنية object_id *oid = &انکڑے->oid;

	أكد(انکڑے->info.typep);

	إذا (انکڑے->type == OBJ_BLOB) {
		إذا (opt->بفر_output)
			fflush(stdout);
		إذا (opt->transform_منوال) {
			حرف *contents;
			ناپ_t ناپ;

			إذا (!انکڑے->rest)
				die("missing path for '%s'", oid_منزل_hex(oid));

			إذا (opt->transform_منوال == 'w') {
				إذا (filter_object(انکڑے->rest, 0100644, oid,
						  &contents, &ناپ))
					die("could not convert '%s' %s",
					    oid_منزل_hex(oid), انکڑے->rest);
			} وإلا إذا (opt->transform_منوال == 'c') {
				تعداد object_type type;
				بنانشانی لما ناپ_ul = 0;
				إذا (textconv_object(the_repository,
						    انکڑے->rest, 0100644, oid,
						    1, &contents, &ناپ_ul))
					ناپ = ناپ_ul;
				وإلا
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &ناپ);
				إذا (!contents)
					die("could not convert '%s' %s",
					    oid_منزل_hex(oid), انکڑے->rest);
			} وإلا
				BUG("invalid transform_mode: %c", opt->transform_منوال);
			batch_write(opt, contents, ناپ);
			حرر(contents);
		} وإلا {
			دھارا_blob(oid);
		}
	}
	وإلا {
		تعداد object_type type;
		ناپ_t ناپ;
		فراغ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &ناپ);
		إذا (!contents)
			die("object %s disappeared", oid_منزل_hex(oid));

		إذا (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &ناپ);

		إذا (type != انکڑے->type)
			die("object %s changed type!?", oid_منزل_hex(oid));
		إذا (انکڑے->info.sizep && ناپ != انکڑے->ناپ && !use_mailmap)
			die("object %s changed size!?", oid_منزل_hex(oid));

		batch_write(opt, contents, ناپ);
		حرر(contents);
	}
}

ساكن فراغ print_default_format(بنية strbuf *scratch, بنية expand_انکڑے *انکڑے,
				 بنية batch_options *opt)
{
	strbuf_جوڑو_oid_hex(scratch, &انکڑے->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(انکڑے->type));
	strbuf_addch(scratch, ' ');
	strbuf_جوڑو_uint(scratch, انکڑے->ناپ);
	strbuf_addch(scratch, opt->output_delim);
}

ساكن فراغ report_object_حالت(بنية batch_options *opt,
				 ثابت حرف *obj_name,
				 ثابت بنية object_id *oid,
				 ثابت حرف *حالت)
{
	لکھو("%s %s%c", obj_name ? obj_name : oid_منزل_hex(oid),
	       حالت, opt->output_delim);
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
			       بنية expand_انکڑے *انکڑے,
			       بنية packed_git *pack,
			       off_t وتھ)
{
	إذا (!انکڑے->skip_object_info) {
		صحيح ret;

		إذا (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_حد_بندی ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			انکڑے->info.typep = &انکڑے->type;
		إذا (opt->objects_filter.choice == LOFC_BLOB_حد_بندی)
			انکڑے->info.sizep = &انکڑے->ناپ;

		إذا (pack)
			ret = packed_object_info(عدم, pack, وتھ, &انکڑے->info);
		وإلا
			ret = odb_read_object_info_extended(the_repository->objects,
							    &انکڑے->oid, &انکڑے->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		إذا (ret < 0) {
			إذا (انکڑے->منوال == S_IFGITLINK)
				report_object_حالت(opt, عدم, &انکڑے->oid, "submodule");
			وإلا
				report_object_حالت(opt, obj_name, &انکڑے->oid, "missing");
			أرجع;
		}

		بدل (opt->objects_filter.choice) {
		حالة LOFC_DISABLED:
			اكسر;
		حالة LOFC_BLOB_NONE:
			إذا (انکڑے->type == OBJ_BLOB) {
				إذا (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &انکڑے->oid, "excluded");
				أرجع;
			}
			اكسر;
		حالة LOFC_BLOB_حد_بندی:
			إذا (انکڑے->type == OBJ_BLOB &&
			    انکڑے->ناپ >= opt->objects_filter.blob_حد_بندی_قدر) {
				إذا (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &انکڑے->oid, "excluded");
				أرجع;
			}
			اكسر;
		حالة LOFC_OBJECT_TYPE:
			إذا (انکڑے->type != opt->objects_filter.object_type) {
				إذا (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &انکڑے->oid, "excluded");
				أرجع;
			}
			اكسر;
		افتراضي:
			BUG("unsupported objects filter");
		}

		إذا (use_mailmap && (انکڑے->type == OBJ_COMMIT || انکڑے->type == OBJ_ٹیگ)) {
			حرف *buf = عدم;

			buf = odb_read_object(the_repository->objects, &انکڑے->oid,
					      &انکڑے->type, &انکڑے->ناپ);
			إذا (!buf)
				die(_("unable to read %s"), oid_منزل_hex(&انکڑے->oid));
			buf = replace_idents_using_mailmap(buf, &انکڑے->ناپ);

			حرر(buf);
		}
	}

	strbuf_reset(scratch);

	إذا (!opt->format) {
		print_default_format(scratch, انکڑے, opt);
	} وإلا {
		expand_format(scratch, opt->format, انکڑے);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	إذا (opt->batch_منوال == BATCH_منوال_CONTENTS) {
		print_object_or_die(opt, انکڑے);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ساكن فراغ batch_one_object(ثابت حرف *obj_name,
			     بنية strbuf *scratch,
			     بنية batch_options *opt,
			     بنية expand_انکڑے *انکڑے)
{
	بنية object_context ctx = {0};
	صحيح flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	تعداد get_oid_نتیجہ نتیجہ;

	نتیجہ = get_oid_with_context(the_repository, obj_name,
				      flags, &انکڑے->oid, &ctx);
	إذا (نتیجہ != لبھیا) {
		بدل (نتیجہ) {
		حالة MISSING_OBJECT:
			report_object_حالت(opt, obj_name, &انکڑے->oid, "missing");
			اكسر;
		حالة SHORT_NAME_AMBIGUOUS:
			report_object_حالت(opt, obj_name, &انکڑے->oid, "ambiguous");
			اكسر;
		حالة DANGLING_SYMLINK:
			لکھو("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڑی_لمبائی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة SYMLINK_LOOP:
			لکھو("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڑی_لمبائی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة NOT_DIR:
			لکھو("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڑی_لمبائی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		افتراضي:
			BUG("unknown get_sha1_with_context result %d\n",
			       نتیجہ);
			اكسر;
		}
		fflush(stdout);

		اذهب باہرلا;
	}

	إذا (ctx.منوال == 0) {
		لکھو("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_رستہ.len,
		       opt->output_delim, ctx.symlink_رستہ.buf, opt->output_delim);
		fflush(stdout);
		اذهب باہرلا;
	}

	انکڑے->منوال = ctx.منوال;
	batch_object_write(obj_name, scratch, opt, انکڑے, عدم, 0);

باہرلا:
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
	لكل (ناپ_t i = 1; i < argc; i++) {
		إذا (get_oid_hex(argv[i], &oid)) {
			ناپ_t len = لڑی_لمبائی(argv[i]);

			إذا (len < the_hash_algo->hexsz && len >= 4) {
				ناپ_t j;
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

بنية object_cb_انکڑے {
	بنية batch_options *opt;
	بنية expand_انکڑے *expand;
	بنية oidset *تکیا;
	بنية strbuf *scratch;
};

ساكن صحيح batch_object_cb(ثابت بنية object_id *oid, فراغ *vdata)
{
	بنية object_cb_انکڑے *انکڑے = vdata;
	oidcpy(&انکڑے->expand->oid, oid);
	batch_object_write(عدم, انکڑے->scratch, انکڑے->opt, انکڑے->expand,
			   عدم, 0);
	أرجع 0;
}

ساكن صحيح collect_object(ثابت بنية object_id *oid,
			  بنية packed_git *pack بن_ورتیا,
			  off_t وتھ بن_ورتیا,
			  فراغ *انکڑے)
{
	oid_array_append(انکڑے, oid);
	أرجع 0;
}

ساكن صحيح batch_unordered_object(ثابت بنية object_id *oid,
				  بنية packed_git *pack,
				  off_t وتھ,
				  فراغ *vdata)
{
	بنية object_cb_انکڑے *انکڑے = vdata;

	إذا (oidset_داخل_کرو(انکڑے->تکیا, oid))
		أرجع 0;

	oidcpy(&انکڑے->expand->oid, oid);
	batch_object_write(عدم, انکڑے->scratch, انکڑے->opt, انکڑے->expand,
			   pack, وتھ);
	أرجع 0;
}

تعريفنوع فراغ (*parse_cmd_fn_t)(بنية batch_options *, ثابت حرف *,
			       بنية strbuf *, بنية expand_انکڑے *);

بنية queued_cmd {
	parse_cmd_fn_t fn;
	حرف *سطر;
};

ساكن فراغ parse_cmd_contents(بنية batch_options *opt,
			     ثابت حرف *سطر,
			     بنية strbuf *output,
			     بنية expand_انکڑے *انکڑے)
{
	تعداد object_type *saved_typep = انکڑے->info.typep;
	انکڑے->info.typep = &انکڑے->type;
	opt->batch_منوال = BATCH_منوال_CONTENTS;
	batch_one_object(سطر, output, opt, انکڑے);
	انکڑے->info.typep = saved_typep;
}

ساكن فراغ parse_cmd_info(بنية batch_options *opt,
			   ثابت حرف *سطر,
			   بنية strbuf *output,
			   بنية expand_انکڑے *انکڑے)
{
	opt->batch_منوال = BATCH_منوال_INFO;
	batch_one_object(سطر, output, opt, انکڑے);
}

ساكن فراغ parse_cmd_mailmap(بنية batch_options *opt بن_ورتیا,
			      ثابت حرف *سطر,
			      بنية strbuf *output بن_ورتیا,
			      بنية expand_انکڑے *انکڑے بن_ورتیا)
{
	use_mailmap = git_parse_maybe_bool(سطر);

	إذا (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), سطر);

	إذا (use_mailmap)
		لدو_mailmap();
}

ساكن فراغ parse_cmd_remote_object_info(بنية batch_options *opt,
					 ثابت حرف *سطر, بنية strbuf *output,
					 بنية expand_انکڑے *انکڑے)
{
	صحيح گنتی;
	ثابت حرف **argv;
	حرف *سطر_منزل_ونڈو;
	بنية fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	بنية oid_array object_info_oids = OID_ARRAY_INIT;

	إذا (لڑی_لمبائی(سطر) >= MAX_REMOTE_OBJ_INFO_سطر)
		die(_("remote-object-info command too long"));

	سطر_منزل_ونڈو = xstrdup(سطر);
	گنتی = ونڈو_cmdline(سطر_منزل_ونڈو, &argv);
	إذا (گنتی < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    ونڈو_cmdline_strerror(گنتی));
	إذا (گنتی - 1 > MAX_ALLOWED_OBJ_حد_بندی)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_حد_بندی);

	إذا (انکڑے->info.sizep)
		results.چاہندا_ناپ = 1;
	إذا (انکڑے->info.typep)
		results.چاہندا_type = 1;

	إذا (get_remote_info(گنتی, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&انکڑے->remote_allowed_atoms, 0);
	string_list_append(&انکڑے->remote_allowed_atoms, "objectname");
	إذا (results.sizes)
		string_list_append(&انکڑے->remote_allowed_atoms, "objectsize");
	إذا (results.types)
		string_list_append(&انکڑے->remote_allowed_atoms, "objecttype");

	انکڑے->skip_object_info = 1;
	لكل (ناپ_t i = 0; i < results.nr; i++) {
		انکڑے->oid = object_info_oids.oid[i];

		إذا (results.unrecognized[i]) {
			report_object_حالت(opt, oid_منزل_hex(&انکڑے->oid),
					     &انکڑے->oid, "missing");
			استمر;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		إذا (results.sizes)
			انکڑے->ناپ = results.sizes[i];

		إذا (results.types)
			انکڑے->type = results.types[i];

		opt->batch_منوال = BATCH_منوال_INFO;
		انکڑے->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, انکڑے, عدم, 0);
		انکڑے->is_remote = 0;
	}
	انکڑے->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	حرر(سطر_منزل_ونڈو);
	حرر(argv);
	oid_array_clear(&object_info_oids);
}

ساكن فراغ dispatch_calls(بنية batch_options *opt,
		بنية strbuf *output,
		بنية expand_انکڑے *انکڑے,
		بنية queued_cmd *cmd,
		ناپ_t nr)
{
	إذا (!opt->بفر_output)
		die(_("flush is only for --buffer mode"));

	لكل (ناپ_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].سطر, output, انکڑے);

	fflush(stdout);
}

ساكن فراغ free_cmds(بنية queued_cmd *cmd, ناپ_t *nr)
{
	لكل (ناپ_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].سطر);

	*nr = 0;
}


ساكن ثابت بنية parse_cmd {
	ثابت حرف *name;
	parse_cmd_fn_t fn;
	بنانشانی takes_دلیلاں;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", عدم, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ساكن فراغ batch_objects_command(بنية batch_options *opt,
				    بنية strbuf *output,
				    بنية expand_انکڑے *انکڑے)
{
	بنية strbuf داخلہ = STRBUF_INIT;
	بنية queued_cmd *queued_cmd = عدم;
	ناپ_t alloc = 0, nr = 0;

	طالما (strbuf_getdelim_strip_crlf(&داخلہ, stdin, opt->داخلہ_delim) != EOF) {
		ثابت بنية parse_cmd *cmd = عدم;
		ثابت حرف *p = عدم, *cmd_سرا;
		بنية queued_cmd call = {0};

		إذا (!داخلہ.len)
			die(_("empty command in input"));
		إذا (خلا_اے(*داخلہ.buf))
			die(_("whitespace before command: '%s'"), داخلہ.buf);

		لكل (ناپ_t i = 0; i < ARRAY_ناپ(commands); i++) {
			إذا (!skip_اگیتر(داخلہ.buf, commands[i].name, &cmd_سرا))
				استمر;

			cmd = &commands[i];
			إذا (cmd->takes_دلیلاں) {
				إذا (*cmd_سرا != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_سرا + 1;
			} وإلا إذا (*cmd_سرا) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			اكسر;
		}

		إذا (!cmd)
			die(_("unknown command: '%s'"), داخلہ.buf);

		إذا (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, انکڑے, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} وإلا إذا (!opt->بفر_output) {
			cmd->fn(opt, p, output, انکڑے);
		} وإلا {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.سطر = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	إذا (opt->بفر_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, انکڑے, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	حرر(queued_cmd);
	strbuf_release(&داخلہ);
}

#تعریف_کرو DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

تعريفنوع صحيح (*for_each_object_fn)(ثابت بنية object_id *oid, بنية packed_git *pack,
				  off_t وتھ, فراغ *انکڑے);

بنية for_each_object_payload {
	for_each_object_fn callback;
	فراغ *payload;
};

ساكن صحيح batch_one_object_oi(ثابت بنية object_id *oid,
			       بنية object_info *oi,
			       فراغ *_payload)
{
	بنية for_each_object_payload *payload = _payload;
	إذا (oi && oi->سروت_infop->سروت->type == ODB_سروت_PACKED)
		أرجع payload->callback(oid, oi->سروت_infop->u.packed.pack,
					 oi->سروت_infop->u.packed.وتھ,
					 payload->payload);
	أرجع payload->callback(oid, عدم, 0, payload->payload);
}

ساكن فراغ batch_each_object(بنية batch_options *opt,
			      for_each_object_fn callback,
			      بنانشانی flags,
			      فراغ *_payload)
{
	بنية for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	بنية odb_سروت_info سروت_info;
	بنية object_info oi = {
		.سروت_infop = &سروت_info,
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
	بنية strbuf داخلہ = STRBUF_INIT;
	بنية strbuf output = STRBUF_INIT;
	بنية expand_انکڑے انکڑے = EXPAND_انکڑے_INIT;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحيح save_warning;
	صحيح retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	انکڑے.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &انکڑے);
	انکڑے.mark_query = 0;
	strbuf_release(&output);
	إذا (opt->transform_منوال)
		انکڑے.ونڈو_on_whitespace = 1;

	إذا (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = عدم;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	إذا (opt->batch_منوال == BATCH_منوال_CONTENTS)
		انکڑے.info.typep = &انکڑے.type;

	إذا (opt->all_objects) {
		بنية object_cb_انکڑے cb;
		بنية object_info خالی_تھاواں = OBJECT_INFO_INIT;

		إذا (!memcmp(&انکڑے.info, &خالی_تھاواں, حجم(خالی_تھاواں)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			انکڑے.skip_object_info = 1;

		إذا (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &انکڑے;
		cb.scratch = &output;

		إذا (opt->unordered) {
			بنية oidset تکیا = OIDSET_INIT;

			cb.تکیا = &تکیا;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ترتیب, &cb);

			oidset_clear(&تکیا);
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

	إذا (opt->batch_منوال == BATCH_منوال_قطار_AND_DISPATCH) {
		batch_objects_command(opt, &output, &انکڑے);
		اذهب cleanup;
	}

	طالما (strbuf_getdelim_strip_crlf(&داخلہ, stdin, opt->داخلہ_delim) != EOF) {
		إذا (انکڑے.ونڈو_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			حرف *p = strpbrk(داخلہ.buf, " \t");
			إذا (p) {
				طالما (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			انکڑے.rest = p;
		}

		batch_one_object(داخلہ.buf, &output, opt, &انکڑے);
	}

 cleanup:
	strbuf_release(&داخلہ);
	strbuf_release(&output);
	string_list_clear(&انکڑے.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	أرجع retval;
}

ساكن صحيح git_cat_file_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (userdiff_config(var, قدر) < 0)
		أرجع -1;

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن صحيح batch_option_callback(ثابت بنية option *opt,
				 ثابت حرف *arg,
				 صحيح unset)
{
	بنية batch_options *bo = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (bo->enabled) {
		أرجع error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	إذا (!strcmp(opt->long_name, "batch"))
		bo->batch_منوال = BATCH_منوال_CONTENTS;
	وإلا إذا (!strcmp(opt->long_name, "batch-check"))
		bo->batch_منوال = BATCH_منوال_INFO;
	وإلا إذا (!strcmp(opt->long_name, "batch-command"))
		bo->batch_منوال = BATCH_منوال_قطار_AND_DISPATCH;
	وإلا
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	أرجع 0;
}

صحيح cmd_cat_file(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اگیتر,
		 بنية repository *repo بن_ورتیا)
{
	صحيح opt = 0;
	صحيح opt_cw = 0;
	صحيح opt_epts = 0;
	ثابت حرف *exp_type = عدم, *obj_name = عدم;
	بنية batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	صحيح unknown_type = 0;
	صحيح داخلہ_nul_terminated = 0;
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
		OPT_گروہ(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', عدم, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', عدم, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_گروہ(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', عدم, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', عدم, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_گروہ(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', عدم, &داخلہ_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', عدم, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_گروہ(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.بفر_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_گروہ(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_رستہ, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_سرا()
	};

	repo_config(the_repository, git_cat_file_config, عدم);

	batch.بفر_output = -1;

	argc = parse_options(argc, argv, اگیتر, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	إذا (use_mailmap)
		لدو_mailmap();

	بدل (batch.objects_filter.choice) {
	حالة LOFC_DISABLED:
		اكسر;
	حالة LOFC_BLOB_NONE:
	حالة LOFC_BLOB_حد_بندی:
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
	إذا (force_رستہ && !opt_cw)
		usage_پیغام_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	إذا (batch.enabled)
		;
	وإلا إذا (batch.follow_symlinks)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	وإلا إذا (batch.بفر_output >= 0)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	وإلا إذا (batch.all_objects)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	وإلا إذا (داخلہ_nul_terminated)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	وإلا إذا (nul_terminated)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.داخلہ_delim = batch.output_delim = '\n';
	إذا (داخلہ_nul_terminated)
		batch.داخلہ_delim = '\0';
	إذا (nul_terminated)
		batch.داخلہ_delim = batch.output_delim = '\0';

	/* Batch defaults */
	إذا (batch.بفر_output < 0)
		batch.بفر_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	/* Return early if we're in batch mode? */
	إذا (batch.enabled) {
		إذا (opt_cw)
			batch.transform_منوال = opt;
		وإلا إذا (opt && opt != 'b')
			usage_پیغام_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		وإلا إذا (argc)
			usage_پیغام_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		اذهب باہرلا;
	}

	إذا (opt) {
		إذا (!argc && opt == 'c')
			usage_پیغام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		وإلا إذا (!argc && opt == 'w')
			usage_پیغام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		وإلا إذا (!argc && opt_epts)
			usage_پیغام_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		وإلا إذا (argc == 1)
			obj_name = argv[0];
		وإلا
			usage_پیغام_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} وإلا إذا (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} وإلا إذا (argc != 2) {
		usage_پیغام_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} وإلا إذا (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

باہرلا:
	list_objects_filter_release(&batch.objects_filter);
	أرجع ret;
}
