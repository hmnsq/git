/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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
#وصف_ڪريو MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#وصف_ڪريو MAX_ALLOWED_OBJ_حد_بندي 10000

/* Maximum input size permitted for the remote-object-info command. */
#وصف_ڪريو MAX_REMOTE_OBJ_INFO_سٽ \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_حد_بندي * (GIT_MAX_HEXSZ + 1))

تعداد batch_گهڻائي {
	BATCH_گهڻائي_CONTENTS,
	BATCH_گهڻائي_INFO,
	BATCH_گهڻائي_قطار_AND_DISPATCH,
};

بنية batch_options {
	بنية list_objects_filter_options objects_filter;
	صحيح enabled;
	صحيح follow_symlinks;
	تعداد batch_گهڻائي batch_گهڻائي;
	صحيح بفر_output;
	صحيح all_objects;
	صحيح unordered;
	صحيح transform_گهڻائي; /* may be 'w' or 'c' for --filters or --textconv */
	حرف داخلا_delim;
	حرف output_delim;
	ثابت حرف *format;
};

ساكن ثابت حرف *force_رستو;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;
ساكن صحيح use_mailmap;

ساكن حرف *replace_idents_using_mailmap(حرف *, سائيز_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ساكن فراغ ڀريو_mailmap(فراغ)
{
	إذا (mailmap.strdup_لڙيون)
		أرجع;

	read_mailmap(the_repository, &mailmap);
}

ساكن حرف *replace_idents_using_mailmap(حرف *object_buf, سائيز_t *سائيز)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *سرا[] = { "author ", "committer ", "tagger ", عدم };

	strbuf_attach(&sb, object_buf, *سائيز, *سائيز + 1);
	apply_mailmap_منزل_سرو(&sb, سرا, &mailmap);
	*سائيز = sb.len;
	أرجع strbuf_detach(&sb, عدم);
}

ساكن صحيح filter_object(ثابت حرف *رستو, بنانشان گهڻائي,
			 ثابت بنية object_id *oid,
			 حرف **buf, سائيز_t *سائيز)
{
	تعداد object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, سائيز);
	إذا (!*buf)
		أرجع error(_("cannot read object %s '%s'"),
			     oid_منزل_hex(oid), رستو);
	إذا ((type == OBJ_BLOB) && S_ISREG(گهڻائي)) {
		بنية strbuf strbuf = STRBUF_INIT;
		بنية checkout_metadata meta;

		init_checkout_metadata(&meta, عدم, عدم, oid);
		إذا (convert_منزل_working_وڻ(the_repository->اشاريو, رستو, *buf, *سائيز, &strbuf, &meta)) {
			حرر(*buf);
			*سائيز = strbuf.len;
			*buf = strbuf_detach(&strbuf, عدم);
		}
	}

	أرجع 0;
}

ساكن صحيح وهڪرو_blob(ثابت بنية object_id *oid)
{
	إذا (odb_وهڪرو_blob_منزل_fd(the_repository->objects, 1, oid, عدم, 0))
		die("unable to stream %s to stdout", oid_منزل_hex(oid));
	أرجع 0;
}

ساكن صحيح cat_one_file(صحيح opt, ثابت حرف *exp_type, ثابت حرف *obj_name)
{
	صحيح ret;
	بنية object_id oid;
	تعداد object_type type;
	حرف *buf;
	سائيز_t سائيز;
	بنية object_context obj_context = {0};
	بنية object_info oi = OBJECT_INFO_INIT;
	بنانشان flags = OBJECT_INFO_LOOKUP_REPLACE;
	بنانشان get_oid_flags =
		GET_OID_رڪارڊ_رستو |
		GET_OID_ONLY_منزل_DIE |
		GET_OID_HASH_ANY;
	ثابت حرف *رستو = force_رستو;
	ثابت صحيح opt_cw = (opt == 'c' || opt == 'w');
	إذا (!رستو && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_رستو;

	إذا (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	إذا (!رستو)
		رستو = obj_context.رستو;
	إذا (obj_context.گهڻائي == S_IFINVALID)
		obj_context.گهڻائي = 0100644;

	buf = عدم;
	بدل (opt) {
	حالة 't':
		oi.typep = &type;
		إذا (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		لکو("%s\n", type_name(type));
		ret = 0;
		اذهب cleanup;

	حالة 's':
		oi.sizep = &سائيز;

		إذا (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (فراغ**)&buf;
		}

		إذا (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		إذا (use_mailmap && (type == OBJ_COMMIT || type == OBJ_ٽيگ))
			buf = replace_idents_using_mailmap(buf, &سائيز);

		لکو("%"PRIuMAX"\n", (uintmax_t)سائيز);
		ret = 0;
		اذهب cleanup;

	حالة 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		اذهب cleanup;

	حالة 'w':

		إذا (filter_object(رستو, obj_context.گهڻائي,
				  &oid, &buf, &سائيز)) {
			ret = -1;
			اذهب cleanup;
		}
		اكسر;

	حالة 'c':
	{
		بنانشان ڊگهو سائيز_ul = 0;
		صحيح textconv_ret = textconv_object(the_repository, رستو,
						   obj_context.گهڻائي, &oid, 1,
						   &buf, &سائيز_ul);
		سائيز = سائيز_ul;
		إذا (textconv_ret)
			اكسر;
	}
		/* else fallthrough */

	حالة 'p':
		type = odb_read_object_info(the_repository->objects, &oid, عدم);
		إذا (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		إذا (type == OBJ_وڻ) {
			ثابت حرف *ls_دليلون[3] = { عدم };
			ls_دليلون[0] =  "ls-tree";
			ls_دليلون[1] =  obj_name;
			ret = cmd_ls_وڻ(2, ls_دليلون, عدم, the_repository);
			اذهب cleanup;
		}

		إذا (type == OBJ_BLOB) {
			ret = وهڪرو_blob(&oid);
			اذهب cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &سائيز);
		إذا (!buf)
			die("Cannot read object %s", obj_name);

		إذا (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &سائيز);

		/* otherwise just spit out the data */
		اكسر;

	حالة 0:
	{
		تعداد object_type exp_type_id = type_from_string(exp_type);

		إذا (exp_type_id == OBJ_BLOB) {
			بنية object_id blob_oid;
			إذا (odb_read_object_info(the_repository->objects,
						 &oid, عدم) == OBJ_ٽيگ) {
				حرف *بفر = odb_read_object(the_repository->objects,
							       &oid, &type, &سائيز);
				ثابت حرف *هدف;

				إذا (!بفر)
					die(_("unable to read %s"), oid_منزل_hex(&oid));

				إذا (!skip_اڳياڙي(بفر, "object ", &هدف) ||
				    get_oid_hex_algop(هدف, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_منزل_hex(&oid));
				حرر(بفر);
			} وإلا
				oidcpy(&blob_oid, &oid);

			إذا (odb_read_object_info(the_repository->objects,
						 &blob_oid, عدم) == OBJ_BLOB) {
				ret = وهڪرو_blob(&blob_oid);
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
					     exp_type_id, &سائيز, عدم);

		إذا (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &سائيز);
		اكسر;
	}
	افتراضي:
		die("git cat-file: unknown option: %s", exp_type);
	}

	إذا (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, سائيز);
	ret = 0;
cleanup:
	حرر(buf);
	object_context_release(&obj_context);
	أرجع ret;
}

بنية expand_معلومات {
	بنية object_id oid;
	تعداد object_type type;
	سائيز_t سائيز;
	بنانشان ننڍو گهڻائي;
	off_t disk_سائيز;
	ثابت حرف *rest;
	بنية object_id delta_بنياد_oid;

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
	صحيح ورهايو_on_whitespace;

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
	بنانشان skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	بنانشان is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	بنية string_list remote_allowed_atoms;
};

#وصف_ڪريو EXPAND_معلومات_INIT  {  \
	.گهڻائي = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ساكن صحيح is_atom(ثابت حرف *atom, ثابت حرف *s, صحيح slen)
{
	صحيح alen = لڙي_ڊيگهه(atom);
	أرجع alen == slen && !memcmp(atom, s, alen);
}

ساكن صحيح expand_atom(بنية strbuf *sb, ثابت حرف *atom, صحيح len,
		       بنية expand_معلومات *معلومات)
{
	إذا (معلومات->is_remote) {
		سائيز_t i;
		لكل (i = 0; i < معلومات->remote_allowed_atoms.nr; i++)
			إذا (is_atom(معلومات->remote_allowed_atoms.شيون[i].string,
				    atom, len))
				اكسر;
		إذا (i == معلومات->remote_allowed_atoms.nr)
			أرجع 1;
	}

	إذا (is_atom("objectname", atom, len)) {
		إذا (!معلومات->mark_query)
			strbuf_جوڙيو_oid_hex(sb, &معلومات->oid);
	} وإلا إذا (is_atom("objecttype", atom, len)) {
		إذا (معلومات->mark_query) {
			معلومات->info.typep = &معلومات->type;
		} وإلا {
			ثابت حرف *t = type_name(معلومات->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} وإلا إذا (is_atom("objectsize", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->info.sizep = &معلومات->سائيز;
		وإلا
			strbuf_جوڙيو_uint(sb, معلومات->سائيز);
	} وإلا إذا (is_atom("objectsize:disk", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->info.disk_sizep = &معلومات->disk_سائيز;
		وإلا
			strbuf_جوڙيو_uint(sb, معلومات->disk_سائيز);
	} وإلا إذا (is_atom("rest", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->ورهايو_on_whitespace = 1;
		وإلا إذا (معلومات->rest)
			strbuf_addstr(sb, معلومات->rest);
	} وإلا إذا (is_atom("deltabase", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->info.delta_بنياد_oid = &معلومات->delta_بنياد_oid;
		وإلا
			strbuf_جوڙيو_oid_hex(sb, &معلومات->delta_بنياد_oid);
	} وإلا إذا (is_atom("objectmode", atom, len)) {
		إذا (!معلومات->mark_query && !(S_IFINVALID == معلومات->گهڻائي))
			strbuf_addf(sb, "%06o", معلومات->گهڻائي);
	} وإلا
		أرجع 0;
	أرجع 1;
}

ساكن فراغ expand_format(بنية strbuf *sb, ثابت حرف *شروعات,
			  بنية expand_معلومات *معلومات)
{
	طالما (strbuf_expand_قدم(sb, &شروعات)) {
		ثابت حرف *پڇاڙي;

		إذا (skip_اڳياڙي(شروعات, "%", &شروعات) || *شروعات != '(')
			strbuf_addch(sb, '%');
		وإلا إذا ((پڇاڙي = strchr(شروعات + 1, ')')) &&
			 expand_atom(sb, شروعات + 1, پڇاڙي - شروعات - 1, معلومات))
			شروعات = پڇاڙي + 1;
		وإلا
			strbuf_expand_bad_format(شروعات, "cat-file");
	}
}

ساكن فراغ batch_write(بنية batch_options *opt, ثابت فراغ *معلومات, صحيح len)
{
	إذا (opt->بفر_output) {
		إذا (fwrite(معلومات, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} وإلا
		write_or_die(1, معلومات, len);
}

ساكن فراغ print_object_or_die(بنية batch_options *opt, بنية expand_معلومات *معلومات)
{
	ثابت بنية object_id *oid = &معلومات->oid;

	أكد(معلومات->info.typep);

	إذا (معلومات->type == OBJ_BLOB) {
		إذا (opt->بفر_output)
			fflush(stdout);
		إذا (opt->transform_گهڻائي) {
			حرف *contents;
			سائيز_t سائيز;

			إذا (!معلومات->rest)
				die("missing path for '%s'", oid_منزل_hex(oid));

			إذا (opt->transform_گهڻائي == 'w') {
				إذا (filter_object(معلومات->rest, 0100644, oid,
						  &contents, &سائيز))
					die("could not convert '%s' %s",
					    oid_منزل_hex(oid), معلومات->rest);
			} وإلا إذا (opt->transform_گهڻائي == 'c') {
				تعداد object_type type;
				بنانشان ڊگهو سائيز_ul = 0;
				إذا (textconv_object(the_repository,
						    معلومات->rest, 0100644, oid,
						    1, &contents, &سائيز_ul))
					سائيز = سائيز_ul;
				وإلا
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &سائيز);
				إذا (!contents)
					die("could not convert '%s' %s",
					    oid_منزل_hex(oid), معلومات->rest);
			} وإلا
				BUG("invalid transform_mode: %c", opt->transform_گهڻائي);
			batch_write(opt, contents, سائيز);
			حرر(contents);
		} وإلا {
			وهڪرو_blob(oid);
		}
	}
	وإلا {
		تعداد object_type type;
		سائيز_t سائيز;
		فراغ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &سائيز);
		إذا (!contents)
			die("object %s disappeared", oid_منزل_hex(oid));

		إذا (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &سائيز);

		إذا (type != معلومات->type)
			die("object %s changed type!?", oid_منزل_hex(oid));
		إذا (معلومات->info.sizep && سائيز != معلومات->سائيز && !use_mailmap)
			die("object %s changed size!?", oid_منزل_hex(oid));

		batch_write(opt, contents, سائيز);
		حرر(contents);
	}
}

ساكن فراغ print_default_format(بنية strbuf *scratch, بنية expand_معلومات *معلومات,
				 بنية batch_options *opt)
{
	strbuf_جوڙيو_oid_hex(scratch, &معلومات->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(معلومات->type));
	strbuf_addch(scratch, ' ');
	strbuf_جوڙيو_uint(scratch, معلومات->سائيز);
	strbuf_addch(scratch, opt->output_delim);
}

ساكن فراغ report_object_حالت(بنية batch_options *opt,
				 ثابت حرف *obj_name,
				 ثابت بنية object_id *oid,
				 ثابت حرف *حالت)
{
	لکو("%s %s%c", obj_name ? obj_name : oid_منزل_hex(oid),
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
			       بنية expand_معلومات *معلومات,
			       بنية packed_git *pack,
			       off_t وٿي)
{
	إذا (!معلومات->skip_object_info) {
		صحيح ret;

		إذا (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_حد_بندي ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			معلومات->info.typep = &معلومات->type;
		إذا (opt->objects_filter.choice == LOFC_BLOB_حد_بندي)
			معلومات->info.sizep = &معلومات->سائيز;

		إذا (pack)
			ret = packed_object_info(عدم, pack, وٿي, &معلومات->info);
		وإلا
			ret = odb_read_object_info_extended(the_repository->objects,
							    &معلومات->oid, &معلومات->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		إذا (ret < 0) {
			إذا (معلومات->گهڻائي == S_IFGITLINK)
				report_object_حالت(opt, عدم, &معلومات->oid, "submodule");
			وإلا
				report_object_حالت(opt, obj_name, &معلومات->oid, "missing");
			أرجع;
		}

		بدل (opt->objects_filter.choice) {
		حالة LOFC_DISABLED:
			اكسر;
		حالة LOFC_BLOB_NONE:
			إذا (معلومات->type == OBJ_BLOB) {
				إذا (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &معلومات->oid, "excluded");
				أرجع;
			}
			اكسر;
		حالة LOFC_BLOB_حد_بندي:
			إذا (معلومات->type == OBJ_BLOB &&
			    معلومات->سائيز >= opt->objects_filter.blob_حد_بندي_قدر) {
				إذا (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &معلومات->oid, "excluded");
				أرجع;
			}
			اكسر;
		حالة LOFC_OBJECT_TYPE:
			إذا (معلومات->type != opt->objects_filter.object_type) {
				إذا (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &معلومات->oid, "excluded");
				أرجع;
			}
			اكسر;
		افتراضي:
			BUG("unsupported objects filter");
		}

		إذا (use_mailmap && (معلومات->type == OBJ_COMMIT || معلومات->type == OBJ_ٽيگ)) {
			حرف *buf = عدم;

			buf = odb_read_object(the_repository->objects, &معلومات->oid,
					      &معلومات->type, &معلومات->سائيز);
			إذا (!buf)
				die(_("unable to read %s"), oid_منزل_hex(&معلومات->oid));
			buf = replace_idents_using_mailmap(buf, &معلومات->سائيز);

			حرر(buf);
		}
	}

	strbuf_reset(scratch);

	إذا (!opt->format) {
		print_default_format(scratch, معلومات, opt);
	} وإلا {
		expand_format(scratch, opt->format, معلومات);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	إذا (opt->batch_گهڻائي == BATCH_گهڻائي_CONTENTS) {
		print_object_or_die(opt, معلومات);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ساكن فراغ batch_one_object(ثابت حرف *obj_name,
			     بنية strbuf *scratch,
			     بنية batch_options *opt,
			     بنية expand_معلومات *معلومات)
{
	بنية object_context ctx = {0};
	صحيح flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	تعداد get_oid_نتيجو نتيجو;

	نتيجو = get_oid_with_context(the_repository, obj_name,
				      flags, &معلومات->oid, &ctx);
	إذا (نتيجو != مليو) {
		بدل (نتيجو) {
		حالة MISSING_OBJECT:
			report_object_حالت(opt, obj_name, &معلومات->oid, "missing");
			اكسر;
		حالة SHORT_NAME_AMBIGUOUS:
			report_object_حالت(opt, obj_name, &معلومات->oid, "ambiguous");
			اكسر;
		حالة DANGLING_SYMLINK:
			لکو("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڙي_ڊيگهه(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة SYMLINK_LOOP:
			لکو("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڙي_ڊيگهه(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة NOT_DIR:
			لکو("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڙي_ڊيگهه(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		افتراضي:
			BUG("unknown get_sha1_with_context result %d\n",
			       نتيجو);
			اكسر;
		}
		fflush(stdout);

		اذهب ٻاهريون;
	}

	إذا (ctx.گهڻائي == 0) {
		لکو("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_رستو.len,
		       opt->output_delim, ctx.symlink_رستو.buf, opt->output_delim);
		fflush(stdout);
		اذهب ٻاهريون;
	}

	معلومات->گهڻائي = ctx.گهڻائي;
	batch_object_write(obj_name, scratch, opt, معلومات, عدم, 0);

ٻاهريون:
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
	لكل (سائيز_t i = 1; i < argc; i++) {
		إذا (get_oid_hex(argv[i], &oid)) {
			سائيز_t len = لڙي_ڊيگهه(argv[i]);

			إذا (len < the_hash_algo->hexsz && len >= 4) {
				سائيز_t j;
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

بنية object_cb_معلومات {
	بنية batch_options *opt;
	بنية expand_معلومات *expand;
	بنية oidset *ڏٺل;
	بنية strbuf *scratch;
};

ساكن صحيح batch_object_cb(ثابت بنية object_id *oid, فراغ *vdata)
{
	بنية object_cb_معلومات *معلومات = vdata;
	oidcpy(&معلومات->expand->oid, oid);
	batch_object_write(عدم, معلومات->scratch, معلومات->opt, معلومات->expand,
			   عدم, 0);
	أرجع 0;
}

ساكن صحيح collect_object(ثابت بنية object_id *oid,
			  بنية packed_git *pack اڻ_استعمال_ٿيل,
			  off_t وٿي اڻ_استعمال_ٿيل,
			  فراغ *معلومات)
{
	oid_array_append(معلومات, oid);
	أرجع 0;
}

ساكن صحيح batch_unordered_object(ثابت بنية object_id *oid,
				  بنية packed_git *pack,
				  off_t وٿي,
				  فراغ *vdata)
{
	بنية object_cb_معلومات *معلومات = vdata;

	إذا (oidset_وجهو(معلومات->ڏٺل, oid))
		أرجع 0;

	oidcpy(&معلومات->expand->oid, oid);
	batch_object_write(عدم, معلومات->scratch, معلومات->opt, معلومات->expand,
			   pack, وٿي);
	أرجع 0;
}

تعريفنوع فراغ (*parse_cmd_fn_t)(بنية batch_options *, ثابت حرف *,
			       بنية strbuf *, بنية expand_معلومات *);

بنية queued_cmd {
	parse_cmd_fn_t fn;
	حرف *سٽ;
};

ساكن فراغ parse_cmd_contents(بنية batch_options *opt,
			     ثابت حرف *سٽ,
			     بنية strbuf *output,
			     بنية expand_معلومات *معلومات)
{
	تعداد object_type *saved_typep = معلومات->info.typep;
	معلومات->info.typep = &معلومات->type;
	opt->batch_گهڻائي = BATCH_گهڻائي_CONTENTS;
	batch_one_object(سٽ, output, opt, معلومات);
	معلومات->info.typep = saved_typep;
}

ساكن فراغ parse_cmd_info(بنية batch_options *opt,
			   ثابت حرف *سٽ,
			   بنية strbuf *output,
			   بنية expand_معلومات *معلومات)
{
	opt->batch_گهڻائي = BATCH_گهڻائي_INFO;
	batch_one_object(سٽ, output, opt, معلومات);
}

ساكن فراغ parse_cmd_mailmap(بنية batch_options *opt اڻ_استعمال_ٿيل,
			      ثابت حرف *سٽ,
			      بنية strbuf *output اڻ_استعمال_ٿيل,
			      بنية expand_معلومات *معلومات اڻ_استعمال_ٿيل)
{
	use_mailmap = git_parse_maybe_bool(سٽ);

	إذا (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), سٽ);

	إذا (use_mailmap)
		ڀريو_mailmap();
}

ساكن فراغ parse_cmd_remote_object_info(بنية batch_options *opt,
					 ثابت حرف *سٽ, بنية strbuf *output,
					 بنية expand_معلومات *معلومات)
{
	صحيح ڳڻپ;
	ثابت حرف **argv;
	حرف *سٽ_منزل_ورهايو;
	بنية fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	بنية oid_array object_info_oids = OID_ARRAY_INIT;

	إذا (لڙي_ڊيگهه(سٽ) >= MAX_REMOTE_OBJ_INFO_سٽ)
		die(_("remote-object-info command too long"));

	سٽ_منزل_ورهايو = xstrdup(سٽ);
	ڳڻپ = ورهايو_cmdline(سٽ_منزل_ورهايو, &argv);
	إذا (ڳڻپ < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    ورهايو_cmdline_strerror(ڳڻپ));
	إذا (ڳڻپ - 1 > MAX_ALLOWED_OBJ_حد_بندي)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_حد_بندي);

	إذا (معلومات->info.sizep)
		results.چاهي_سائيز = 1;
	إذا (معلومات->info.typep)
		results.چاهي_type = 1;

	إذا (get_remote_info(ڳڻپ, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&معلومات->remote_allowed_atoms, 0);
	string_list_append(&معلومات->remote_allowed_atoms, "objectname");
	إذا (results.sizes)
		string_list_append(&معلومات->remote_allowed_atoms, "objectsize");
	إذا (results.types)
		string_list_append(&معلومات->remote_allowed_atoms, "objecttype");

	معلومات->skip_object_info = 1;
	لكل (سائيز_t i = 0; i < results.nr; i++) {
		معلومات->oid = object_info_oids.oid[i];

		إذا (results.unrecognized[i]) {
			report_object_حالت(opt, oid_منزل_hex(&معلومات->oid),
					     &معلومات->oid, "missing");
			استمر;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		إذا (results.sizes)
			معلومات->سائيز = results.sizes[i];

		إذا (results.types)
			معلومات->type = results.types[i];

		opt->batch_گهڻائي = BATCH_گهڻائي_INFO;
		معلومات->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, معلومات, عدم, 0);
		معلومات->is_remote = 0;
	}
	معلومات->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	حرر(سٽ_منزل_ورهايو);
	حرر(argv);
	oid_array_clear(&object_info_oids);
}

ساكن فراغ dispatch_calls(بنية batch_options *opt,
		بنية strbuf *output,
		بنية expand_معلومات *معلومات,
		بنية queued_cmd *cmd,
		سائيز_t nr)
{
	إذا (!opt->بفر_output)
		die(_("flush is only for --buffer mode"));

	لكل (سائيز_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].سٽ, output, معلومات);

	fflush(stdout);
}

ساكن فراغ free_cmds(بنية queued_cmd *cmd, سائيز_t *nr)
{
	لكل (سائيز_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].سٽ);

	*nr = 0;
}


ساكن ثابت بنية parse_cmd {
	ثابت حرف *name;
	parse_cmd_fn_t fn;
	بنانشان takes_دليلون;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", عدم, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ساكن فراغ batch_objects_command(بنية batch_options *opt,
				    بنية strbuf *output,
				    بنية expand_معلومات *معلومات)
{
	بنية strbuf داخلا_2 = STRBUF_INIT;
	بنية queued_cmd *queued_cmd = عدم;
	سائيز_t alloc = 0, nr = 0;

	طالما (strbuf_getdelim_strip_crlf(&داخلا_2, stdin, opt->داخلا_delim) != EOF) {
		ثابت بنية parse_cmd *cmd = عدم;
		ثابت حرف *p = عدم, *cmd_پڇاڙي;
		بنية queued_cmd call = {0};

		إذا (!داخلا_2.len)
			die(_("empty command in input"));
		إذا (خال_آهي(*داخلا_2.buf))
			die(_("whitespace before command: '%s'"), داخلا_2.buf);

		لكل (سائيز_t i = 0; i < ARRAY_سائيز(commands); i++) {
			إذا (!skip_اڳياڙي(داخلا_2.buf, commands[i].name, &cmd_پڇاڙي))
				استمر;

			cmd = &commands[i];
			إذا (cmd->takes_دليلون) {
				إذا (*cmd_پڇاڙي != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_پڇاڙي + 1;
			} وإلا إذا (*cmd_پڇاڙي) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			اكسر;
		}

		إذا (!cmd)
			die(_("unknown command: '%s'"), داخلا_2.buf);

		إذا (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, معلومات, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} وإلا إذا (!opt->بفر_output) {
			cmd->fn(opt, p, output, معلومات);
		} وإلا {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.سٽ = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	إذا (opt->بفر_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, معلومات, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	حرر(queued_cmd);
	strbuf_release(&داخلا_2);
}

#وصف_ڪريو DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

تعريفنوع صحيح (*for_each_object_fn)(ثابت بنية object_id *oid, بنية packed_git *pack,
				  off_t وٿي, فراغ *معلومات);

بنية for_each_object_payload {
	for_each_object_fn callback;
	فراغ *payload;
};

ساكن صحيح batch_one_object_oi(ثابت بنية object_id *oid,
			       بنية object_info *oi,
			       فراغ *_payload)
{
	بنية for_each_object_payload *payload = _payload;
	إذا (oi && oi->ذريعو_infop->ذريعو->type == ODB_ذريعو_PACKED)
		أرجع payload->callback(oid, oi->ذريعو_infop->u.packed.pack,
					 oi->ذريعو_infop->u.packed.وٿي,
					 payload->payload);
	أرجع payload->callback(oid, عدم, 0, payload->payload);
}

ساكن فراغ batch_each_object(بنية batch_options *opt,
			      for_each_object_fn callback,
			      بنانشان flags,
			      فراغ *_payload)
{
	بنية for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	بنية odb_ذريعو_info ذريعو_info;
	بنية object_info oi = {
		.ذريعو_infop = &ذريعو_info,
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
	بنية strbuf داخلا_2 = STRBUF_INIT;
	بنية strbuf output = STRBUF_INIT;
	بنية expand_معلومات معلومات = EXPAND_معلومات_INIT;
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);
	صحيح save_warning;
	صحيح retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	معلومات.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &معلومات);
	معلومات.mark_query = 0;
	strbuf_release(&output);
	إذا (opt->transform_گهڻائي)
		معلومات.ورهايو_on_whitespace = 1;

	إذا (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = عدم;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	إذا (opt->batch_گهڻائي == BATCH_گهڻائي_CONTENTS)
		معلومات.info.typep = &معلومات.type;

	إذا (opt->all_objects) {
		بنية object_cb_معلومات cb;
		بنية object_info خالي_جايون = OBJECT_INFO_INIT;

		إذا (!memcmp(&معلومات.info, &خالي_جايون, حجم(خالي_جايون)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			معلومات.skip_object_info = 1;

		إذا (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &معلومات;
		cb.scratch = &output;

		إذا (opt->unordered) {
			بنية oidset ڏٺل = OIDSET_INIT;

			cb.ڏٺل = &ڏٺل;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ترتيب, &cb);

			oidset_clear(&ڏٺل);
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

	إذا (opt->batch_گهڻائي == BATCH_گهڻائي_قطار_AND_DISPATCH) {
		batch_objects_command(opt, &output, &معلومات);
		اذهب cleanup;
	}

	طالما (strbuf_getdelim_strip_crlf(&داخلا_2, stdin, opt->داخلا_delim) != EOF) {
		إذا (معلومات.ورهايو_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			حرف *p = strpbrk(داخلا_2.buf, " \t");
			إذا (p) {
				طالما (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			معلومات.rest = p;
		}

		batch_one_object(داخلا_2.buf, &output, opt, &معلومات);
	}

 cleanup:
	strbuf_release(&داخلا_2);
	strbuf_release(&output);
	string_list_clear(&معلومات.remote_allowed_atoms, 0);
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
		bo->batch_گهڻائي = BATCH_گهڻائي_CONTENTS;
	وإلا إذا (!strcmp(opt->long_name, "batch-check"))
		bo->batch_گهڻائي = BATCH_گهڻائي_INFO;
	وإلا إذا (!strcmp(opt->long_name, "batch-command"))
		bo->batch_گهڻائي = BATCH_گهڻائي_قطار_AND_DISPATCH;
	وإلا
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	أرجع 0;
}

صحيح cmd_cat_file(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اڳياڙي,
		 بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح opt = 0;
	صحيح opt_cw = 0;
	صحيح opt_epts = 0;
	ثابت حرف *exp_type = عدم, *obj_name = عدم;
	بنية batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	صحيح unknown_type = 0;
	صحيح داخلا_nul_terminated = 0;
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
		OPT_گروهه(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', عدم, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', عدم, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_گروهه(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', عدم, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', عدم, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_گروهه(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', عدم, &داخلا_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', عدم, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_گروهه(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.بفر_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_گروهه(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_رستو, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_پڇاڙي()
	};

	repo_config(the_repository, git_cat_file_config, عدم);

	batch.بفر_output = -1;

	argc = parse_options(argc, argv, اڳياڙي, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	إذا (use_mailmap)
		ڀريو_mailmap();

	بدل (batch.objects_filter.choice) {
	حالة LOFC_DISABLED:
		اكسر;
	حالة LOFC_BLOB_NONE:
	حالة LOFC_BLOB_حد_بندي:
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
	إذا (force_رستو && !opt_cw)
		usage_نياپو_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	إذا (batch.enabled)
		;
	وإلا إذا (batch.follow_symlinks)
		usage_نياپو_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	وإلا إذا (batch.بفر_output >= 0)
		usage_نياپو_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	وإلا إذا (batch.all_objects)
		usage_نياپو_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	وإلا إذا (داخلا_nul_terminated)
		usage_نياپو_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	وإلا إذا (nul_terminated)
		usage_نياپو_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.داخلا_delim = batch.output_delim = '\n';
	إذا (داخلا_nul_terminated)
		batch.داخلا_delim = '\0';
	إذا (nul_terminated)
		batch.داخلا_delim = batch.output_delim = '\0';

	/* Batch defaults */
	إذا (batch.بفر_output < 0)
		batch.بفر_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ڀريل_اشاريو = 0;

	/* Return early if we're in batch mode? */
	إذا (batch.enabled) {
		إذا (opt_cw)
			batch.transform_گهڻائي = opt;
		وإلا إذا (opt && opt != 'b')
			usage_نياپو_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		وإلا إذا (argc)
			usage_نياپو_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		اذهب ٻاهريون;
	}

	إذا (opt) {
		إذا (!argc && opt == 'c')
			usage_نياپو_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		وإلا إذا (!argc && opt == 'w')
			usage_نياپو_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		وإلا إذا (!argc && opt_epts)
			usage_نياپو_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		وإلا إذا (argc == 1)
			obj_name = argv[0];
		وإلا
			usage_نياپو_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} وإلا إذا (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} وإلا إذا (argc != 2) {
		usage_نياپو_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} وإلا إذا (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

ٻاهريون:
	list_objects_filter_release(&batch.objects_filter);
	أرجع ret;
}
