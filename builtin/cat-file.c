/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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
#تعریف_کړه MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#تعریف_کړه MAX_ALLOWED_OBJ_حد 10000

/* Maximum input size permitted for the remote-object-info command. */
#تعریف_کړه MAX_REMOTE_OBJ_INFO_کرښه \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_حد * (GIT_MAX_HEXSZ + 1))

تعداد batch_ډېرپېښ {
	BATCH_ډېرپېښ_CONTENTS,
	BATCH_ډېرپېښ_INFO,
	BATCH_ډېرپېښ_کتار_AND_DISPATCH,
};

بنية batch_options {
	بنية list_objects_filter_options objects_filter;
	صحيح enabled;
	صحيح follow_symlinks;
	تعداد batch_ډېرپېښ batch_ډېرپېښ;
	صحيح بفر_output;
	صحيح all_objects;
	صحيح unordered;
	صحيح transform_ډېرپېښ; /* may be 'w' or 'c' for --filters or --textconv */
	حرف ننوت_delim;
	حرف output_delim;
	ثابت حرف *format;
};

ساكن ثابت حرف *force_لار;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;
ساكن صحيح use_mailmap;

ساكن حرف *replace_idents_using_mailmap(حرف *, اندازه_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ساكن فراغ بارول_mailmap(فراغ)
{
	إذا (mailmap.strdup_مزي_ګانې)
		أرجع;

	read_mailmap(the_repository, &mailmap);
}

ساكن حرف *replace_idents_using_mailmap(حرف *object_buf, اندازه_t *اندازه)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *سرلیکونه[] = { "author ", "committer ", "tagger ", عدم };

	strbuf_attach(&sb, object_buf, *اندازه, *اندازه + 1);
	apply_mailmap_منزل_سرلیک(&sb, سرلیکونه, &mailmap);
	*اندازه = sb.len;
	أرجع strbuf_detach(&sb, عدم);
}

ساكن صحيح filter_object(ثابت حرف *لار, بینښه ډېرپېښ,
			 ثابت بنية object_id *oid,
			 حرف **buf, اندازه_t *اندازه)
{
	تعداد object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, اندازه);
	إذا (!*buf)
		أرجع error(_("cannot read object %s '%s'"),
			     oid_منزل_hex(oid), لار);
	إذا ((type == OBJ_BLOB) && S_ISREG(ډېرپېښ)) {
		بنية strbuf strbuf = STRBUF_INIT;
		بنية checkout_metadata meta;

		init_checkout_metadata(&meta, عدم, عدم, oid);
		إذا (convert_منزل_working_ونه(the_repository->شاخص, لار, *buf, *اندازه, &strbuf, &meta)) {
			حرر(*buf);
			*اندازه = strbuf.len;
			*buf = strbuf_detach(&strbuf, عدم);
		}
	}

	أرجع 0;
}

ساكن صحيح بهیر_blob(ثابت بنية object_id *oid)
{
	إذا (odb_بهیر_blob_منزل_fd(the_repository->objects, 1, oid, عدم, 0))
		die("unable to stream %s to stdout", oid_منزل_hex(oid));
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
	بینښه flags = OBJECT_INFO_LOOKUP_REPLACE;
	بینښه get_oid_flags =
		GET_OID_ریکارډ_لار |
		GET_OID_ONLY_منزل_DIE |
		GET_OID_HASH_ANY;
	ثابت حرف *لار = force_لار;
	ثابت صحيح opt_cw = (opt == 'c' || opt == 'w');
	إذا (!لار && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_لار;

	إذا (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	إذا (!لار)
		لار = obj_context.لار;
	إذا (obj_context.ډېرپېښ == S_IFINVALID)
		obj_context.ډېرپېښ = 0100644;

	buf = عدم;
	بدل (opt) {
	حالة 't':
		oi.typep = &type;
		إذا (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		چاپ_کړه("%s\n", type_name(type));
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

		إذا (use_mailmap && (type == OBJ_COMMIT || type == OBJ_نښان))
			buf = replace_idents_using_mailmap(buf, &اندازه);

		چاپ_کړه("%"PRIuMAX"\n", (uintmax_t)اندازه);
		ret = 0;
		اذهب cleanup;

	حالة 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		اذهب cleanup;

	حالة 'w':

		إذا (filter_object(لار, obj_context.ډېرپېښ,
				  &oid, &buf, &اندازه)) {
			ret = -1;
			اذهب cleanup;
		}
		اكسر;

	حالة 'c':
	{
		بینښه اوږد اندازه_ul = 0;
		صحيح textconv_ret = textconv_object(the_repository, لار,
						   obj_context.ډېرپېښ, &oid, 1,
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
		إذا (type == OBJ_ونه) {
			ثابت حرف *ls_دلیلونه[3] = { عدم };
			ls_دلیلونه[0] =  "ls-tree";
			ls_دلیلونه[1] =  obj_name;
			ret = cmd_ls_ونه(2, ls_دلیلونه, عدم, the_repository);
			اذهب cleanup;
		}

		إذا (type == OBJ_BLOB) {
			ret = بهیر_blob(&oid);
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
						 &oid, عدم) == OBJ_نښان) {
				حرف *بفر = odb_read_object(the_repository->objects,
							       &oid, &type, &اندازه);
				ثابت حرف *موخه;

				إذا (!بفر)
					die(_("unable to read %s"), oid_منزل_hex(&oid));

				إذا (!skip_مختاړی(بفر, "object ", &موخه) ||
				    get_oid_hex_algop(موخه, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_منزل_hex(&oid));
				حرر(بفر);
			} وإلا
				oidcpy(&blob_oid, &oid);

			إذا (odb_read_object_info(the_repository->objects,
						 &blob_oid, عدم) == OBJ_BLOB) {
				ret = بهیر_blob(&blob_oid);
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

بنية expand_معلومات {
	بنية object_id oid;
	تعداد object_type type;
	اندازه_t اندازه;
	بینښه لنډ ډېرپېښ;
	off_t disk_اندازه;
	ثابت حرف *rest;
	بنية object_id delta_بنسټ_oid;

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
	صحيح وېشل_on_whitespace;

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
	بینښه skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	بینښه is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	بنية string_list remote_allowed_atoms;
};

#تعریف_کړه EXPAND_معلومات_INIT  {  \
	.ډېرپېښ = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ساكن صحيح is_atom(ثابت حرف *atom, ثابت حرف *s, صحيح slen)
{
	صحيح alen = مزي_اوږدوالی(atom);
	أرجع alen == slen && !memcmp(atom, s, alen);
}

ساكن صحيح expand_atom(بنية strbuf *sb, ثابت حرف *atom, صحيح len,
		       بنية expand_معلومات *معلومات)
{
	إذا (معلومات->is_remote) {
		اندازه_t i;
		لكل (i = 0; i < معلومات->remote_allowed_atoms.nr; i++)
			إذا (is_atom(معلومات->remote_allowed_atoms.توکي[i].string,
				    atom, len))
				اكسر;
		إذا (i == معلومات->remote_allowed_atoms.nr)
			أرجع 1;
	}

	إذا (is_atom("objectname", atom, len)) {
		إذا (!معلومات->mark_query)
			strbuf_جمع_کړه_oid_hex(sb, &معلومات->oid);
	} وإلا إذا (is_atom("objecttype", atom, len)) {
		إذا (معلومات->mark_query) {
			معلومات->info.typep = &معلومات->type;
		} وإلا {
			ثابت حرف *t = type_name(معلومات->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} وإلا إذا (is_atom("objectsize", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->info.sizep = &معلومات->اندازه;
		وإلا
			strbuf_جمع_کړه_uint(sb, معلومات->اندازه);
	} وإلا إذا (is_atom("objectsize:disk", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->info.disk_sizep = &معلومات->disk_اندازه;
		وإلا
			strbuf_جمع_کړه_uint(sb, معلومات->disk_اندازه);
	} وإلا إذا (is_atom("rest", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->وېشل_on_whitespace = 1;
		وإلا إذا (معلومات->rest)
			strbuf_addstr(sb, معلومات->rest);
	} وإلا إذا (is_atom("deltabase", atom, len)) {
		إذا (معلومات->mark_query)
			معلومات->info.delta_بنسټ_oid = &معلومات->delta_بنسټ_oid;
		وإلا
			strbuf_جمع_کړه_oid_hex(sb, &معلومات->delta_بنسټ_oid);
	} وإلا إذا (is_atom("objectmode", atom, len)) {
		إذا (!معلومات->mark_query && !(S_IFINVALID == معلومات->ډېرپېښ))
			strbuf_addf(sb, "%06o", معلومات->ډېرپېښ);
	} وإلا
		أرجع 0;
	أرجع 1;
}

ساكن فراغ expand_format(بنية strbuf *sb, ثابت حرف *پیل,
			  بنية expand_معلومات *معلومات)
{
	طالما (strbuf_expand_ګام(sb, &پیل)) {
		ثابت حرف *پای_ټکی;

		إذا (skip_مختاړی(پیل, "%", &پیل) || *پیل != '(')
			strbuf_addch(sb, '%');
		وإلا إذا ((پای_ټکی = strchr(پیل + 1, ')')) &&
			 expand_atom(sb, پیل + 1, پای_ټکی - پیل - 1, معلومات))
			پیل = پای_ټکی + 1;
		وإلا
			strbuf_expand_bad_format(پیل, "cat-file");
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
		إذا (opt->transform_ډېرپېښ) {
			حرف *contents;
			اندازه_t اندازه;

			إذا (!معلومات->rest)
				die("missing path for '%s'", oid_منزل_hex(oid));

			إذا (opt->transform_ډېرپېښ == 'w') {
				إذا (filter_object(معلومات->rest, 0100644, oid,
						  &contents, &اندازه))
					die("could not convert '%s' %s",
					    oid_منزل_hex(oid), معلومات->rest);
			} وإلا إذا (opt->transform_ډېرپېښ == 'c') {
				تعداد object_type type;
				بینښه اوږد اندازه_ul = 0;
				إذا (textconv_object(the_repository,
						    معلومات->rest, 0100644, oid,
						    1, &contents, &اندازه_ul))
					اندازه = اندازه_ul;
				وإلا
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &اندازه);
				إذا (!contents)
					die("could not convert '%s' %s",
					    oid_منزل_hex(oid), معلومات->rest);
			} وإلا
				BUG("invalid transform_mode: %c", opt->transform_ډېرپېښ);
			batch_write(opt, contents, اندازه);
			حرر(contents);
		} وإلا {
			بهیر_blob(oid);
		}
	}
	وإلا {
		تعداد object_type type;
		اندازه_t اندازه;
		فراغ *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &اندازه);
		إذا (!contents)
			die("object %s disappeared", oid_منزل_hex(oid));

		إذا (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &اندازه);

		إذا (type != معلومات->type)
			die("object %s changed type!?", oid_منزل_hex(oid));
		إذا (معلومات->info.sizep && اندازه != معلومات->اندازه && !use_mailmap)
			die("object %s changed size!?", oid_منزل_hex(oid));

		batch_write(opt, contents, اندازه);
		حرر(contents);
	}
}

ساكن فراغ print_default_format(بنية strbuf *scratch, بنية expand_معلومات *معلومات,
				 بنية batch_options *opt)
{
	strbuf_جمع_کړه_oid_hex(scratch, &معلومات->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(معلومات->type));
	strbuf_addch(scratch, ' ');
	strbuf_جمع_کړه_uint(scratch, معلومات->اندازه);
	strbuf_addch(scratch, opt->output_delim);
}

ساكن فراغ report_object_حالت(بنية batch_options *opt,
				 ثابت حرف *obj_name,
				 ثابت بنية object_id *oid,
				 ثابت حرف *حالت)
{
	چاپ_کړه("%s %s%c", obj_name ? obj_name : oid_منزل_hex(oid),
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
			       off_t دننه_واټن)
{
	إذا (!معلومات->skip_object_info) {
		صحيح ret;

		إذا (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_حد ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			معلومات->info.typep = &معلومات->type;
		إذا (opt->objects_filter.choice == LOFC_BLOB_حد)
			معلومات->info.sizep = &معلومات->اندازه;

		إذا (pack)
			ret = packed_object_info(عدم, pack, دننه_واټن, &معلومات->info);
		وإلا
			ret = odb_read_object_info_extended(the_repository->objects,
							    &معلومات->oid, &معلومات->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		إذا (ret < 0) {
			إذا (معلومات->ډېرپېښ == S_IFGITLINK)
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
		حالة LOFC_BLOB_حد:
			إذا (معلومات->type == OBJ_BLOB &&
			    معلومات->اندازه >= opt->objects_filter.blob_حد_ارزښت) {
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

		إذا (use_mailmap && (معلومات->type == OBJ_COMMIT || معلومات->type == OBJ_نښان)) {
			حرف *buf = عدم;

			buf = odb_read_object(the_repository->objects, &معلومات->oid,
					      &معلومات->type, &معلومات->اندازه);
			إذا (!buf)
				die(_("unable to read %s"), oid_منزل_hex(&معلومات->oid));
			buf = replace_idents_using_mailmap(buf, &معلومات->اندازه);

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

	إذا (opt->batch_ډېرپېښ == BATCH_ډېرپېښ_CONTENTS) {
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
	تعداد get_oid_پایله پایله;

	پایله = get_oid_with_context(the_repository, obj_name,
				      flags, &معلومات->oid, &ctx);
	إذا (پایله != وموندل) {
		بدل (پایله) {
		حالة MISSING_OBJECT:
			report_object_حالت(opt, obj_name, &معلومات->oid, "missing");
			اكسر;
		حالة SHORT_NAME_AMBIGUOUS:
			report_object_حالت(opt, obj_name, &معلومات->oid, "ambiguous");
			اكسر;
		حالة DANGLING_SYMLINK:
			چاپ_کړه("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)مزي_اوږدوالی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة SYMLINK_LOOP:
			چاپ_کړه("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)مزي_اوږدوالی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		حالة NOT_DIR:
			چاپ_کړه("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)مزي_اوږدوالی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			اكسر;
		افتراضي:
			BUG("unknown get_sha1_with_context result %d\n",
			       پایله);
			اكسر;
		}
		fflush(stdout);

		اذهب بهر;
	}

	إذا (ctx.ډېرپېښ == 0) {
		چاپ_کړه("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_لار.len,
		       opt->output_delim, ctx.symlink_لار.buf, opt->output_delim);
		fflush(stdout);
		اذهب بهر;
	}

	معلومات->ډېرپېښ = ctx.ډېرپېښ;
	batch_object_write(obj_name, scratch, opt, معلومات, عدم, 0);

بهر:
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
			اندازه_t len = مزي_اوږدوالی(argv[i]);

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

بنية object_cb_معلومات {
	بنية batch_options *opt;
	بنية expand_معلومات *expand;
	بنية oidset *کتل_شوی;
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
			  بنية packed_git *pack ناکارول_شوی,
			  off_t دننه_واټن ناکارول_شوی,
			  فراغ *معلومات)
{
	oid_array_append(معلومات, oid);
	أرجع 0;
}

ساكن صحيح batch_unordered_object(ثابت بنية object_id *oid,
				  بنية packed_git *pack,
				  off_t دننه_واټن,
				  فراغ *vdata)
{
	بنية object_cb_معلومات *معلومات = vdata;

	إذا (oidset_ورننباسه(معلومات->کتل_شوی, oid))
		أرجع 0;

	oidcpy(&معلومات->expand->oid, oid);
	batch_object_write(عدم, معلومات->scratch, معلومات->opt, معلومات->expand,
			   pack, دننه_واټن);
	أرجع 0;
}

تعريفنوع فراغ (*parse_cmd_fn_t)(بنية batch_options *, ثابت حرف *,
			       بنية strbuf *, بنية expand_معلومات *);

بنية queued_cmd {
	parse_cmd_fn_t fn;
	حرف *کرښه;
};

ساكن فراغ parse_cmd_contents(بنية batch_options *opt,
			     ثابت حرف *کرښه,
			     بنية strbuf *output,
			     بنية expand_معلومات *معلومات)
{
	تعداد object_type *saved_typep = معلومات->info.typep;
	معلومات->info.typep = &معلومات->type;
	opt->batch_ډېرپېښ = BATCH_ډېرپېښ_CONTENTS;
	batch_one_object(کرښه, output, opt, معلومات);
	معلومات->info.typep = saved_typep;
}

ساكن فراغ parse_cmd_info(بنية batch_options *opt,
			   ثابت حرف *کرښه,
			   بنية strbuf *output,
			   بنية expand_معلومات *معلومات)
{
	opt->batch_ډېرپېښ = BATCH_ډېرپېښ_INFO;
	batch_one_object(کرښه, output, opt, معلومات);
}

ساكن فراغ parse_cmd_mailmap(بنية batch_options *opt ناکارول_شوی,
			      ثابت حرف *کرښه,
			      بنية strbuf *output ناکارول_شوی,
			      بنية expand_معلومات *معلومات ناکارول_شوی)
{
	use_mailmap = git_parse_maybe_bool(کرښه);

	إذا (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), کرښه);

	إذا (use_mailmap)
		بارول_mailmap();
}

ساكن فراغ parse_cmd_remote_object_info(بنية batch_options *opt,
					 ثابت حرف *کرښه, بنية strbuf *output,
					 بنية expand_معلومات *معلومات)
{
	صحيح شمېر;
	ثابت حرف **argv;
	حرف *کرښه_منزل_وېشل;
	بنية fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	بنية oid_array object_info_oids = OID_ARRAY_INIT;

	إذا (مزي_اوږدوالی(کرښه) >= MAX_REMOTE_OBJ_INFO_کرښه)
		die(_("remote-object-info command too long"));

	کرښه_منزل_وېشل = xstrdup(کرښه);
	شمېر = وېشل_cmdline(کرښه_منزل_وېشل, &argv);
	إذا (شمېر < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    وېشل_cmdline_strerror(شمېر));
	إذا (شمېر - 1 > MAX_ALLOWED_OBJ_حد)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_حد);

	إذا (معلومات->info.sizep)
		results.غواړي_اندازه = 1;
	إذا (معلومات->info.typep)
		results.غواړي_type = 1;

	إذا (get_remote_info(شمېر, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&معلومات->remote_allowed_atoms, 0);
	string_list_append(&معلومات->remote_allowed_atoms, "objectname");
	إذا (results.sizes)
		string_list_append(&معلومات->remote_allowed_atoms, "objectsize");
	إذا (results.types)
		string_list_append(&معلومات->remote_allowed_atoms, "objecttype");

	معلومات->skip_object_info = 1;
	لكل (اندازه_t i = 0; i < results.nr; i++) {
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
			معلومات->اندازه = results.sizes[i];

		إذا (results.types)
			معلومات->type = results.types[i];

		opt->batch_ډېرپېښ = BATCH_ډېرپېښ_INFO;
		معلومات->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, معلومات, عدم, 0);
		معلومات->is_remote = 0;
	}
	معلومات->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	حرر(کرښه_منزل_وېشل);
	حرر(argv);
	oid_array_clear(&object_info_oids);
}

ساكن فراغ dispatch_calls(بنية batch_options *opt,
		بنية strbuf *output,
		بنية expand_معلومات *معلومات,
		بنية queued_cmd *cmd,
		اندازه_t nr)
{
	إذا (!opt->بفر_output)
		die(_("flush is only for --buffer mode"));

	لكل (اندازه_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].کرښه, output, معلومات);

	fflush(stdout);
}

ساكن فراغ free_cmds(بنية queued_cmd *cmd, اندازه_t *nr)
{
	لكل (اندازه_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].کرښه);

	*nr = 0;
}


ساكن ثابت بنية parse_cmd {
	ثابت حرف *name;
	parse_cmd_fn_t fn;
	بینښه takes_دلیلونه;
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
	بنية strbuf ننوت = STRBUF_INIT;
	بنية queued_cmd *queued_cmd = عدم;
	اندازه_t alloc = 0, nr = 0;

	طالما (strbuf_getdelim_strip_crlf(&ننوت, stdin, opt->ننوت_delim) != EOF) {
		ثابت بنية parse_cmd *cmd = عدم;
		ثابت حرف *p = عدم, *cmd_پای_ټکی;
		بنية queued_cmd call = {0};

		إذا (!ننوت.len)
			die(_("empty command in input"));
		إذا (که_تشه(*ننوت.buf))
			die(_("whitespace before command: '%s'"), ننوت.buf);

		لكل (اندازه_t i = 0; i < ARRAY_اندازه(commands); i++) {
			إذا (!skip_مختاړی(ننوت.buf, commands[i].name, &cmd_پای_ټکی))
				استمر;

			cmd = &commands[i];
			إذا (cmd->takes_دلیلونه) {
				إذا (*cmd_پای_ټکی != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_پای_ټکی + 1;
			} وإلا إذا (*cmd_پای_ټکی) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			اكسر;
		}

		إذا (!cmd)
			die(_("unknown command: '%s'"), ننوت.buf);

		إذا (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, معلومات, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} وإلا إذا (!opt->بفر_output) {
			cmd->fn(opt, p, output, معلومات);
		} وإلا {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.کرښه = xstrdup_or_null(p);
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
	strbuf_release(&ننوت);
}

#تعریف_کړه DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

تعريفنوع صحيح (*for_each_object_fn)(ثابت بنية object_id *oid, بنية packed_git *pack,
				  off_t دننه_واټن, فراغ *معلومات);

بنية for_each_object_payload {
	for_each_object_fn callback;
	فراغ *payload;
};

ساكن صحيح batch_one_object_oi(ثابت بنية object_id *oid,
			       بنية object_info *oi,
			       فراغ *_payload)
{
	بنية for_each_object_payload *payload = _payload;
	إذا (oi && oi->سرچینه_infop->سرچینه->type == ODB_سرچینه_PACKED)
		أرجع payload->callback(oid, oi->سرچینه_infop->u.packed.pack,
					 oi->سرچینه_infop->u.packed.دننه_واټن,
					 payload->payload);
	أرجع payload->callback(oid, عدم, 0, payload->payload);
}

ساكن فراغ batch_each_object(بنية batch_options *opt,
			      for_each_object_fn callback,
			      بینښه flags,
			      فراغ *_payload)
{
	بنية for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	بنية odb_سرچینه_info سرچینه_info;
	بنية object_info oi = {
		.سرچینه_infop = &سرچینه_info,
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
	بنية strbuf ننوت = STRBUF_INIT;
	بنية strbuf output = STRBUF_INIT;
	بنية expand_معلومات معلومات = EXPAND_معلومات_INIT;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);
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
	إذا (opt->transform_ډېرپېښ)
		معلومات.وېشل_on_whitespace = 1;

	إذا (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = عدم;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	إذا (opt->batch_ډېرپېښ == BATCH_ډېرپېښ_CONTENTS)
		معلومات.info.typep = &معلومات.type;

	إذا (opt->all_objects) {
		بنية object_cb_معلومات cb;
		بنية object_info تشه = OBJECT_INFO_INIT;

		إذا (!memcmp(&معلومات.info, &تشه, حجم(تشه)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			معلومات.skip_object_info = 1;

		إذا (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &معلومات;
		cb.scratch = &output;

		إذا (opt->unordered) {
			بنية oidset کتل_شوی = OIDSET_INIT;

			cb.کتل_شوی = &کتل_شوی;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ترتیب, &cb);

			oidset_clear(&کتل_شوی);
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

	إذا (opt->batch_ډېرپېښ == BATCH_ډېرپېښ_کتار_AND_DISPATCH) {
		batch_objects_command(opt, &output, &معلومات);
		اذهب cleanup;
	}

	طالما (strbuf_getdelim_strip_crlf(&ننوت, stdin, opt->ننوت_delim) != EOF) {
		إذا (معلومات.وېشل_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			حرف *p = strpbrk(ننوت.buf, " \t");
			إذا (p) {
				طالما (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			معلومات.rest = p;
		}

		batch_one_object(ننوت.buf, &output, opt, &معلومات);
	}

 cleanup:
	strbuf_release(&ننوت);
	strbuf_release(&output);
	string_list_clear(&معلومات.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	أرجع retval;
}

ساكن صحيح git_cat_file_config(ثابت حرف *var, ثابت حرف *ارزښت,
			       ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (userdiff_config(var, ارزښت) < 0)
		أرجع -1;

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

ساكن صحيح batch_option_callback(ثابت بنية option *opt,
				 ثابت حرف *arg,
				 صحيح unset)
{
	بنية batch_options *bo = opt->ارزښت;

	BUG_ON_OPT_NEG(unset);

	إذا (bo->enabled) {
		أرجع error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	إذا (!strcmp(opt->long_name, "batch"))
		bo->batch_ډېرپېښ = BATCH_ډېرپېښ_CONTENTS;
	وإلا إذا (!strcmp(opt->long_name, "batch-check"))
		bo->batch_ډېرپېښ = BATCH_ډېرپېښ_INFO;
	وإلا إذا (!strcmp(opt->long_name, "batch-command"))
		bo->batch_ډېرپېښ = BATCH_ډېرپېښ_کتار_AND_DISPATCH;
	وإلا
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	أرجع 0;
}

صحيح cmd_cat_file(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *مختاړی,
		 بنية repository *repo ناکارول_شوی)
{
	صحيح opt = 0;
	صحيح opt_cw = 0;
	صحيح opt_epts = 0;
	ثابت حرف *exp_type = عدم, *obj_name = عدم;
	بنية batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	صحيح unknown_type = 0;
	صحيح ننوت_nul_terminated = 0;
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
		OPT_ډله(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', عدم, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', عدم, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_ډله(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', عدم, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', عدم, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_ډله(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', عدم, &ننوت_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', عدم, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_ډله(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.بفر_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_ډله(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_لار, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_پای_ټکی()
	};

	repo_config(the_repository, git_cat_file_config, عدم);

	batch.بفر_output = -1;

	argc = parse_options(argc, argv, مختاړی, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	إذا (use_mailmap)
		بارول_mailmap();

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
	إذا (force_لار && !opt_cw)
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
	وإلا إذا (ننوت_nul_terminated)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	وإلا إذا (nul_terminated)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.ننوت_delim = batch.output_delim = '\n';
	إذا (ننوت_nul_terminated)
		batch.ننوت_delim = '\0';
	إذا (nul_terminated)
		batch.ننوت_delim = batch.output_delim = '\0';

	/* Batch defaults */
	إذا (batch.بفر_output < 0)
		batch.بفر_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	/* Return early if we're in batch mode? */
	إذا (batch.enabled) {
		إذا (opt_cw)
			batch.transform_ډېرپېښ = opt;
		وإلا إذا (opt && opt != 'b')
			usage_پیغام_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		وإلا إذا (argc)
			usage_پیغام_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		اذهب بهر;
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

بهر:
	list_objects_filter_release(&batch.objects_filter);
	أرجع ret;
}
