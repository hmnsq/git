/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "convert.h"
#شامل "diff.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "ident.h"
#شامل "list-objects-filter-options.h"
#شامل "parse-options.h"
#شامل "userdiff.h"
#شامل "oid-array.h"
#شامل "packfile.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "replace-object.h"
#شامل "promisor-remote.h"
#شامل "mailmap.h"
#شامل "write-or-die.h"
#شامل "alias.h"
#شامل "remote.h"
#شامل "transport.h"
#شامل "fetch-object-info.h"

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

شمار batch_منوال {
	BATCH_منوال_CONTENTS,
	BATCH_منوال_INFO,
	BATCH_منوال_قطار_AND_DISPATCH,
};

ساخت batch_options {
	ساخت list_objects_filter_options objects_filter;
	صحیح enabled;
	صحیح follow_symlinks;
	شمار batch_منوال batch_منوال;
	صحیح بفر_output;
	صحیح all_objects;
	صحیح unordered;
	صحیح transform_منوال; /* may be 'w' or 'c' for --filters or --textconv */
	حرف داخلہ_delim;
	حرف output_delim;
	ثابت حرف *format;
};

ساکن ثابت حرف *force_راستہ;

ساکن ساخت string_list mailmap = STRING_LIST_INIT_NODUP;
ساکن صحیح use_mailmap;

ساکن حرف *replace_idents_using_mailmap(حرف *, سائز_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
ساکن خالی بھرو_mailmap(خالی)
{
	اگر (mailmap.strdup_لڑیاں)
		واپس;

	read_mailmap(the_repository, &mailmap);
}

ساکن حرف *replace_idents_using_mailmap(حرف *object_buf, سائز_t *سائز)
{
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *سرخیاں[] = { "author ", "committer ", "tagger ", تہی };

	strbuf_attach(&sb, object_buf, *سائز, *سائز + 1);
	apply_mailmap_مقصد_سرخی(&sb, سرخیاں, &mailmap);
	*سائز = sb.len;
	واپس strbuf_detach(&sb, تہی);
}

ساکن صحیح filter_object(ثابت حرف *راستہ, غیرعلامتی منوال,
			 ثابت ساخت object_id *oid,
			 حرف **buf, سائز_t *سائز)
{
	شمار object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, سائز);
	اگر (!*buf)
		واپس error(_("cannot read object %s '%s'"),
			     oid_مقصد_hex(oid), راستہ);
	اگر ((type == OBJ_BLOB) && S_ISREG(منوال)) {
		ساخت strbuf strbuf = STRBUF_INIT;
		ساخت checkout_metadata meta;

		init_checkout_metadata(&meta, تہی, تہی, oid);
		اگر (convert_مقصد_working_درخت(the_repository->اشاریہ, راستہ, *buf, *سائز, &strbuf, &meta)) {
			آزاد(*buf);
			*سائز = strbuf.len;
			*buf = strbuf_detach(&strbuf, تہی);
		}
	}

	واپس 0;
}

ساکن صحیح دھارا_blob(ثابت ساخت object_id *oid)
{
	اگر (odb_دھارا_blob_مقصد_fd(the_repository->objects, 1, oid, تہی, 0))
		die("unable to stream %s to stdout", oid_مقصد_hex(oid));
	واپس 0;
}

ساکن صحیح cat_one_file(صحیح opt, ثابت حرف *exp_type, ثابت حرف *obj_name)
{
	صحیح ret;
	ساخت object_id oid;
	شمار object_type type;
	حرف *buf;
	سائز_t سائز;
	ساخت object_context obj_context = {0};
	ساخت object_info oi = OBJECT_INFO_INIT;
	غیرعلامتی flags = OBJECT_INFO_LOOKUP_REPLACE;
	غیرعلامتی get_oid_flags =
		GET_OID_ریکارڈ_راستہ |
		GET_OID_ONLY_مقصد_DIE |
		GET_OID_HASH_ANY;
	ثابت حرف *راستہ = force_راستہ;
	ثابت صحیح opt_cw = (opt == 'c' || opt == 'w');
	اگر (!راستہ && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_راستہ;

	اگر (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	اگر (!راستہ)
		راستہ = obj_context.راستہ;
	اگر (obj_context.منوال == S_IFINVALID)
		obj_context.منوال = 0100644;

	buf = تہی;
	انتخاب (opt) {
	حالت 't':
		oi.typep = &type;
		اگر (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		لکھو("%s\n", type_name(type));
		ret = 0;
		جاؤ cleanup;

	حالت 's':
		oi.sizep = &سائز;

		اگر (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (خالی**)&buf;
		}

		اگر (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		اگر (use_mailmap && (type == OBJ_COMMIT || type == OBJ_لیبل))
			buf = replace_idents_using_mailmap(buf, &سائز);

		لکھو("%"PRIuMAX"\n", (uintmax_t)سائز);
		ret = 0;
		جاؤ cleanup;

	حالت 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		جاؤ cleanup;

	حالت 'w':

		اگر (filter_object(راستہ, obj_context.منوال,
				  &oid, &buf, &سائز)) {
			ret = -1;
			جاؤ cleanup;
		}
		توڑو;

	حالت 'c':
	{
		غیرعلامتی طویل سائز_ul = 0;
		صحیح textconv_ret = textconv_object(the_repository, راستہ,
						   obj_context.منوال, &oid, 1,
						   &buf, &سائز_ul);
		سائز = سائز_ul;
		اگر (textconv_ret)
			توڑو;
	}
		/* else fallthrough */

	حالت 'p':
		type = odb_read_object_info(the_repository->objects, &oid, تہی);
		اگر (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		اگر (type == OBJ_درخت) {
			ثابت حرف *ls_دلائل[3] = { تہی };
			ls_دلائل[0] =  "ls-tree";
			ls_دلائل[1] =  obj_name;
			ret = cmd_ls_درخت(2, ls_دلائل, تہی, the_repository);
			جاؤ cleanup;
		}

		اگر (type == OBJ_BLOB) {
			ret = دھارا_blob(&oid);
			جاؤ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &سائز);
		اگر (!buf)
			die("Cannot read object %s", obj_name);

		اگر (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &سائز);

		/* otherwise just spit out the data */
		توڑو;

	حالت 0:
	{
		شمار object_type exp_type_id = type_from_string(exp_type);

		اگر (exp_type_id == OBJ_BLOB) {
			ساخت object_id blob_oid;
			اگر (odb_read_object_info(the_repository->objects,
						 &oid, تہی) == OBJ_لیبل) {
				حرف *بفر = odb_read_object(the_repository->objects,
							       &oid, &type, &سائز);
				ثابت حرف *ہدف;

				اگر (!بفر)
					die(_("unable to read %s"), oid_مقصد_hex(&oid));

				اگر (!skip_سابقہ(بفر, "object ", &ہدف) ||
				    get_oid_hex_algop(ہدف, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_مقصد_hex(&oid));
				آزاد(بفر);
			} ورنہ
				oidcpy(&blob_oid, &oid);

			اگر (odb_read_object_info(the_repository->objects,
						 &blob_oid, تہی) == OBJ_BLOB) {
				ret = دھارا_blob(&blob_oid);
				جاؤ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &سائز, تہی);

		اگر (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &سائز);
		توڑو;
	}
	طےشدہ:
		die("git cat-file: unknown option: %s", exp_type);
	}

	اگر (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, سائز);
	ret = 0;
cleanup:
	آزاد(buf);
	object_context_release(&obj_context);
	واپس ret;
}

ساخت expand_ڈیٹا {
	ساخت object_id oid;
	شمار object_type type;
	سائز_t سائز;
	غیرعلامتی مختصر منوال;
	off_t disk_سائز;
	ثابت حرف *rest;
	ساخت object_id delta_بنیاد_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	صحیح mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	صحیح بانٹو_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	ساخت object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	غیرعلامتی skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	غیرعلامتی is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	ساخت string_list remote_allowed_atoms;
};

#تعریف_کرو EXPAND_ڈیٹا_INIT  {  \
	.منوال = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

ساکن صحیح is_atom(ثابت حرف *atom, ثابت حرف *s, صحیح slen)
{
	صحیح alen = لڑی_لمبائی(atom);
	واپس alen == slen && !memcmp(atom, s, alen);
}

ساکن صحیح expand_atom(ساخت strbuf *sb, ثابت حرف *atom, صحیح len,
		       ساخت expand_ڈیٹا *ڈیٹا)
{
	اگر (ڈیٹا->is_remote) {
		سائز_t i;
		برائے (i = 0; i < ڈیٹا->remote_allowed_atoms.nr; i++)
			اگر (is_atom(ڈیٹا->remote_allowed_atoms.اشیاء[i].string,
				    atom, len))
				توڑو;
		اگر (i == ڈیٹا->remote_allowed_atoms.nr)
			واپس 1;
	}

	اگر (is_atom("objectname", atom, len)) {
		اگر (!ڈیٹا->mark_query)
			strbuf_جوڑو_oid_hex(sb, &ڈیٹا->oid);
	} ورنہ اگر (is_atom("objecttype", atom, len)) {
		اگر (ڈیٹا->mark_query) {
			ڈیٹا->info.typep = &ڈیٹا->type;
		} ورنہ {
			ثابت حرف *t = type_name(ڈیٹا->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} ورنہ اگر (is_atom("objectsize", atom, len)) {
		اگر (ڈیٹا->mark_query)
			ڈیٹا->info.sizep = &ڈیٹا->سائز;
		ورنہ
			strbuf_جوڑو_uint(sb, ڈیٹا->سائز);
	} ورنہ اگر (is_atom("objectsize:disk", atom, len)) {
		اگر (ڈیٹا->mark_query)
			ڈیٹا->info.disk_sizep = &ڈیٹا->disk_سائز;
		ورنہ
			strbuf_جوڑو_uint(sb, ڈیٹا->disk_سائز);
	} ورنہ اگر (is_atom("rest", atom, len)) {
		اگر (ڈیٹا->mark_query)
			ڈیٹا->بانٹو_on_whitespace = 1;
		ورنہ اگر (ڈیٹا->rest)
			strbuf_addstr(sb, ڈیٹا->rest);
	} ورنہ اگر (is_atom("deltabase", atom, len)) {
		اگر (ڈیٹا->mark_query)
			ڈیٹا->info.delta_بنیاد_oid = &ڈیٹا->delta_بنیاد_oid;
		ورنہ
			strbuf_جوڑو_oid_hex(sb, &ڈیٹا->delta_بنیاد_oid);
	} ورنہ اگر (is_atom("objectmode", atom, len)) {
		اگر (!ڈیٹا->mark_query && !(S_IFINVALID == ڈیٹا->منوال))
			strbuf_addf(sb, "%06o", ڈیٹا->منوال);
	} ورنہ
		واپس 0;
	واپس 1;
}

ساکن خالی expand_format(ساخت strbuf *sb, ثابت حرف *آغاز,
			  ساخت expand_ڈیٹا *ڈیٹا)
{
	جب (strbuf_expand_قدم(sb, &آغاز)) {
		ثابت حرف *انتہا;

		اگر (skip_سابقہ(آغاز, "%", &آغاز) || *آغاز != '(')
			strbuf_addch(sb, '%');
		ورنہ اگر ((انتہا = strchr(آغاز + 1, ')')) &&
			 expand_atom(sb, آغاز + 1, انتہا - آغاز - 1, ڈیٹا))
			آغاز = انتہا + 1;
		ورنہ
			strbuf_expand_bad_format(آغاز, "cat-file");
	}
}

ساکن خالی batch_write(ساخت batch_options *opt, ثابت خالی *ڈیٹا, صحیح len)
{
	اگر (opt->بفر_output) {
		اگر (fwrite(ڈیٹا, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} ورنہ
		write_or_die(1, ڈیٹا, len);
}

ساکن خالی print_object_or_die(ساخت batch_options *opt, ساخت expand_ڈیٹا *ڈیٹا)
{
	ثابت ساخت object_id *oid = &ڈیٹا->oid;

	أكد(ڈیٹا->info.typep);

	اگر (ڈیٹا->type == OBJ_BLOB) {
		اگر (opt->بفر_output)
			fflush(stdout);
		اگر (opt->transform_منوال) {
			حرف *contents;
			سائز_t سائز;

			اگر (!ڈیٹا->rest)
				die("missing path for '%s'", oid_مقصد_hex(oid));

			اگر (opt->transform_منوال == 'w') {
				اگر (filter_object(ڈیٹا->rest, 0100644, oid,
						  &contents, &سائز))
					die("could not convert '%s' %s",
					    oid_مقصد_hex(oid), ڈیٹا->rest);
			} ورنہ اگر (opt->transform_منوال == 'c') {
				شمار object_type type;
				غیرعلامتی طویل سائز_ul = 0;
				اگر (textconv_object(the_repository,
						    ڈیٹا->rest, 0100644, oid,
						    1, &contents, &سائز_ul))
					سائز = سائز_ul;
				ورنہ
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &سائز);
				اگر (!contents)
					die("could not convert '%s' %s",
					    oid_مقصد_hex(oid), ڈیٹا->rest);
			} ورنہ
				BUG("invalid transform_mode: %c", opt->transform_منوال);
			batch_write(opt, contents, سائز);
			آزاد(contents);
		} ورنہ {
			دھارا_blob(oid);
		}
	}
	ورنہ {
		شمار object_type type;
		سائز_t سائز;
		خالی *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &سائز);
		اگر (!contents)
			die("object %s disappeared", oid_مقصد_hex(oid));

		اگر (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &سائز);

		اگر (type != ڈیٹا->type)
			die("object %s changed type!?", oid_مقصد_hex(oid));
		اگر (ڈیٹا->info.sizep && سائز != ڈیٹا->سائز && !use_mailmap)
			die("object %s changed size!?", oid_مقصد_hex(oid));

		batch_write(opt, contents, سائز);
		آزاد(contents);
	}
}

ساکن خالی print_default_format(ساخت strbuf *scratch, ساخت expand_ڈیٹا *ڈیٹا,
				 ساخت batch_options *opt)
{
	strbuf_جوڑو_oid_hex(scratch, &ڈیٹا->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(ڈیٹا->type));
	strbuf_addch(scratch, ' ');
	strbuf_جوڑو_uint(scratch, ڈیٹا->سائز);
	strbuf_addch(scratch, opt->output_delim);
}

ساکن خالی report_object_حالت(ساخت batch_options *opt,
				 ثابت حرف *obj_name,
				 ثابت ساخت object_id *oid,
				 ثابت حرف *حالت_2)
{
	لکھو("%s %s%c", obj_name ? obj_name : oid_مقصد_hex(oid),
	       حالت_2, opt->output_delim);
	fflush(stdout);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
ساکن خالی batch_object_write(ثابت حرف *obj_name,
			       ساخت strbuf *scratch,
			       ساخت batch_options *opt,
			       ساخت expand_ڈیٹا *ڈیٹا,
			       ساخت packed_git *pack,
			       off_t انحراف)
{
	اگر (!ڈیٹا->skip_object_info) {
		صحیح ret;

		اگر (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_حد_بندی ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			ڈیٹا->info.typep = &ڈیٹا->type;
		اگر (opt->objects_filter.choice == LOFC_BLOB_حد_بندی)
			ڈیٹا->info.sizep = &ڈیٹا->سائز;

		اگر (pack)
			ret = packed_object_info(تہی, pack, انحراف, &ڈیٹا->info);
		ورنہ
			ret = odb_read_object_info_extended(the_repository->objects,
							    &ڈیٹا->oid, &ڈیٹا->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		اگر (ret < 0) {
			اگر (ڈیٹا->منوال == S_IFGITLINK)
				report_object_حالت(opt, تہی, &ڈیٹا->oid, "submodule");
			ورنہ
				report_object_حالت(opt, obj_name, &ڈیٹا->oid, "missing");
			واپس;
		}

		انتخاب (opt->objects_filter.choice) {
		حالت LOFC_DISABLED:
			توڑو;
		حالت LOFC_BLOB_NONE:
			اگر (ڈیٹا->type == OBJ_BLOB) {
				اگر (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &ڈیٹا->oid, "excluded");
				واپس;
			}
			توڑو;
		حالت LOFC_BLOB_حد_بندی:
			اگر (ڈیٹا->type == OBJ_BLOB &&
			    ڈیٹا->سائز >= opt->objects_filter.blob_حد_بندی_قدر) {
				اگر (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &ڈیٹا->oid, "excluded");
				واپس;
			}
			توڑو;
		حالت LOFC_OBJECT_TYPE:
			اگر (ڈیٹا->type != opt->objects_filter.object_type) {
				اگر (!opt->all_objects)
					report_object_حالت(opt, obj_name,
							     &ڈیٹا->oid, "excluded");
				واپس;
			}
			توڑو;
		طےشدہ:
			BUG("unsupported objects filter");
		}

		اگر (use_mailmap && (ڈیٹا->type == OBJ_COMMIT || ڈیٹا->type == OBJ_لیبل)) {
			حرف *buf = تہی;

			buf = odb_read_object(the_repository->objects, &ڈیٹا->oid,
					      &ڈیٹا->type, &ڈیٹا->سائز);
			اگر (!buf)
				die(_("unable to read %s"), oid_مقصد_hex(&ڈیٹا->oid));
			buf = replace_idents_using_mailmap(buf, &ڈیٹا->سائز);

			آزاد(buf);
		}
	}

	strbuf_reset(scratch);

	اگر (!opt->format) {
		print_default_format(scratch, ڈیٹا, opt);
	} ورنہ {
		expand_format(scratch, opt->format, ڈیٹا);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	اگر (opt->batch_منوال == BATCH_منوال_CONTENTS) {
		print_object_or_die(opt, ڈیٹا);
		batch_write(opt, &opt->output_delim, 1);
	}
}

ساکن خالی batch_one_object(ثابت حرف *obj_name,
			     ساخت strbuf *scratch,
			     ساخت batch_options *opt,
			     ساخت expand_ڈیٹا *ڈیٹا)
{
	ساخت object_context ctx = {0};
	صحیح flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	شمار get_oid_نتیجہ نتیجہ;

	نتیجہ = get_oid_with_context(the_repository, obj_name,
				      flags, &ڈیٹا->oid, &ctx);
	اگر (نتیجہ != ملا) {
		انتخاب (نتیجہ) {
		حالت MISSING_OBJECT:
			report_object_حالت(opt, obj_name, &ڈیٹا->oid, "missing");
			توڑو;
		حالت SHORT_NAME_AMBIGUOUS:
			report_object_حالت(opt, obj_name, &ڈیٹا->oid, "ambiguous");
			توڑو;
		حالت DANGLING_SYMLINK:
			لکھو("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڑی_لمبائی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			توڑو;
		حالت SYMLINK_LOOP:
			لکھو("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڑی_لمبائی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			توڑو;
		حالت NOT_DIR:
			لکھو("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)لڑی_لمبائی(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			توڑو;
		طےشدہ:
			BUG("unknown get_sha1_with_context result %d\n",
			       نتیجہ);
			توڑو;
		}
		fflush(stdout);

		جاؤ برآمد;
	}

	اگر (ctx.منوال == 0) {
		لکھو("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_راستہ.len,
		       opt->output_delim, ctx.symlink_راستہ.buf, opt->output_delim);
		fflush(stdout);
		جاؤ برآمد;
	}

	ڈیٹا->منوال = ctx.منوال;
	batch_object_write(obj_name, scratch, opt, ڈیٹا, تہی, 0);

برآمد:
	object_context_release(&ctx);
}

ساکن صحیح get_remote_info(صحیح argc,
			   ثابت حرف **argv,
			   ساخت fetch_object_info_results *results,
			   ساخت oid_array *object_info_oids)
{
	صحیح retval = 0;
	ساخت remote *remote = تہی;
	ساخت object_id oid;
	ساخت transport *gtransport;

	remote = remote_get(argv[0]);
	اگر (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	برائے (سائز_t i = 1; i < argc; i++) {
		اگر (get_oid_hex(argv[i], &oid)) {
			سائز_t len = لڑی_لمبائی(argv[i]);

			اگر (len < the_hash_algo->hexsz && len >= 4) {
				سائز_t j;
				برائے (j = 0; j < len; j++)
					اگر (!isxdigit(argv[i][j]))
						توڑو;
				اگر (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (صحیح)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	اگر (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, تہی);

	اگر (!gtransport->smart_options) {
		retval = -1;
		جاؤ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	واپس retval;
}

ساخت object_cb_ڈیٹا {
	ساخت batch_options *opt;
	ساخت expand_ڈیٹا *expand;
	ساخت oidset *نظر_آیا;
	ساخت strbuf *scratch;
};

ساکن صحیح batch_object_cb(ثابت ساخت object_id *oid, خالی *vdata)
{
	ساخت object_cb_ڈیٹا *ڈیٹا = vdata;
	oidcpy(&ڈیٹا->expand->oid, oid);
	batch_object_write(تہی, ڈیٹا->scratch, ڈیٹا->opt, ڈیٹا->expand,
			   تہی, 0);
	واپس 0;
}

ساکن صحیح collect_object(ثابت ساخت object_id *oid,
			  ساخت packed_git *pack غیر_مستعمل,
			  off_t انحراف غیر_مستعمل,
			  خالی *ڈیٹا)
{
	oid_array_append(ڈیٹا, oid);
	واپس 0;
}

ساکن صحیح batch_unordered_object(ثابت ساخت object_id *oid,
				  ساخت packed_git *pack,
				  off_t انحراف,
				  خالی *vdata)
{
	ساخت object_cb_ڈیٹا *ڈیٹا = vdata;

	اگر (oidset_داخل_کرو(ڈیٹا->نظر_آیا, oid))
		واپس 0;

	oidcpy(&ڈیٹا->expand->oid, oid);
	batch_object_write(تہی, ڈیٹا->scratch, ڈیٹا->opt, ڈیٹا->expand,
			   pack, انحراف);
	واپس 0;
}

قسمتعریف خالی (*parse_cmd_fn_t)(ساخت batch_options *, ثابت حرف *,
			       ساخت strbuf *, ساخت expand_ڈیٹا *);

ساخت queued_cmd {
	parse_cmd_fn_t fn;
	حرف *سطر;
};

ساکن خالی parse_cmd_contents(ساخت batch_options *opt,
			     ثابت حرف *سطر,
			     ساخت strbuf *output,
			     ساخت expand_ڈیٹا *ڈیٹا)
{
	شمار object_type *saved_typep = ڈیٹا->info.typep;
	ڈیٹا->info.typep = &ڈیٹا->type;
	opt->batch_منوال = BATCH_منوال_CONTENTS;
	batch_one_object(سطر, output, opt, ڈیٹا);
	ڈیٹا->info.typep = saved_typep;
}

ساکن خالی parse_cmd_info(ساخت batch_options *opt,
			   ثابت حرف *سطر,
			   ساخت strbuf *output,
			   ساخت expand_ڈیٹا *ڈیٹا)
{
	opt->batch_منوال = BATCH_منوال_INFO;
	batch_one_object(سطر, output, opt, ڈیٹا);
}

ساکن خالی parse_cmd_mailmap(ساخت batch_options *opt غیر_مستعمل,
			      ثابت حرف *سطر,
			      ساخت strbuf *output غیر_مستعمل,
			      ساخت expand_ڈیٹا *ڈیٹا غیر_مستعمل)
{
	use_mailmap = git_parse_maybe_bool(سطر);

	اگر (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), سطر);

	اگر (use_mailmap)
		بھرو_mailmap();
}

ساکن خالی parse_cmd_remote_object_info(ساخت batch_options *opt,
					 ثابت حرف *سطر, ساخت strbuf *output,
					 ساخت expand_ڈیٹا *ڈیٹا)
{
	صحیح گنتی;
	ثابت حرف **argv;
	حرف *سطر_مقصد_بانٹو;
	ساخت fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	ساخت oid_array object_info_oids = OID_ARRAY_INIT;

	اگر (لڑی_لمبائی(سطر) >= MAX_REMOTE_OBJ_INFO_سطر)
		die(_("remote-object-info command too long"));

	سطر_مقصد_بانٹو = xstrdup(سطر);
	گنتی = بانٹو_cmdline(سطر_مقصد_بانٹو, &argv);
	اگر (گنتی < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    بانٹو_cmdline_strerror(گنتی));
	اگر (گنتی - 1 > MAX_ALLOWED_OBJ_حد_بندی)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_حد_بندی);

	اگر (ڈیٹا->info.sizep)
		results.چاہتا_سائز = 1;
	اگر (ڈیٹا->info.typep)
		results.چاہتا_type = 1;

	اگر (get_remote_info(گنتی, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&ڈیٹا->remote_allowed_atoms, 0);
	string_list_append(&ڈیٹا->remote_allowed_atoms, "objectname");
	اگر (results.sizes)
		string_list_append(&ڈیٹا->remote_allowed_atoms, "objectsize");
	اگر (results.types)
		string_list_append(&ڈیٹا->remote_allowed_atoms, "objecttype");

	ڈیٹا->skip_object_info = 1;
	برائے (سائز_t i = 0; i < results.nr; i++) {
		ڈیٹا->oid = object_info_oids.oid[i];

		اگر (results.unrecognized[i]) {
			report_object_حالت(opt, oid_مقصد_hex(&ڈیٹا->oid),
					     &ڈیٹا->oid, "missing");
			جاری;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		اگر (results.sizes)
			ڈیٹا->سائز = results.sizes[i];

		اگر (results.types)
			ڈیٹا->type = results.types[i];

		opt->batch_منوال = BATCH_منوال_INFO;
		ڈیٹا->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, ڈیٹا, تہی, 0);
		ڈیٹا->is_remote = 0;
	}
	ڈیٹا->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	آزاد(سطر_مقصد_بانٹو);
	آزاد(argv);
	oid_array_clear(&object_info_oids);
}

ساکن خالی dispatch_calls(ساخت batch_options *opt,
		ساخت strbuf *output,
		ساخت expand_ڈیٹا *ڈیٹا,
		ساخت queued_cmd *cmd,
		سائز_t nr)
{
	اگر (!opt->بفر_output)
		die(_("flush is only for --buffer mode"));

	برائے (سائز_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].سطر, output, ڈیٹا);

	fflush(stdout);
}

ساکن خالی free_cmds(ساخت queued_cmd *cmd, سائز_t *nr)
{
	برائے (سائز_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].سطر);

	*nr = 0;
}


ساکن ثابت ساخت parse_cmd {
	ثابت حرف *name;
	parse_cmd_fn_t fn;
	غیرعلامتی takes_دلائل;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", تہی, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

ساکن خالی batch_objects_command(ساخت batch_options *opt,
				    ساخت strbuf *output,
				    ساخت expand_ڈیٹا *ڈیٹا)
{
	ساخت strbuf داخلہ = STRBUF_INIT;
	ساخت queued_cmd *queued_cmd = تہی;
	سائز_t alloc = 0, nr = 0;

	جب (strbuf_getdelim_strip_crlf(&داخلہ, stdin, opt->داخلہ_delim) != EOF) {
		ثابت ساخت parse_cmd *cmd = تہی;
		ثابت حرف *p = تہی, *cmd_انتہا;
		ساخت queued_cmd call = {0};

		اگر (!داخلہ.len)
			die(_("empty command in input"));
		اگر (خلا_ہے(*داخلہ.buf))
			die(_("whitespace before command: '%s'"), داخلہ.buf);

		برائے (سائز_t i = 0; i < ARRAY_سائز(commands); i++) {
			اگر (!skip_سابقہ(داخلہ.buf, commands[i].name, &cmd_انتہا))
				جاری;

			cmd = &commands[i];
			اگر (cmd->takes_دلائل) {
				اگر (*cmd_انتہا != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_انتہا + 1;
			} ورنہ اگر (*cmd_انتہا) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			توڑو;
		}

		اگر (!cmd)
			die(_("unknown command: '%s'"), داخلہ.buf);

		اگر (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, ڈیٹا, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} ورنہ اگر (!opt->بفر_output) {
			cmd->fn(opt, p, output, ڈیٹا);
		} ورنہ {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.سطر = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	اگر (opt->بفر_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, ڈیٹا, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	آزاد(queued_cmd);
	strbuf_release(&داخلہ);
}

#تعریف_کرو DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

قسمتعریف صحیح (*for_each_object_fn)(ثابت ساخت object_id *oid, ساخت packed_git *pack,
				  off_t انحراف, خالی *ڈیٹا);

ساخت for_each_object_payload {
	for_each_object_fn callback;
	خالی *payload;
};

ساکن صحیح batch_one_object_oi(ثابت ساخت object_id *oid,
			       ساخت object_info *oi,
			       خالی *_payload)
{
	ساخت for_each_object_payload *payload = _payload;
	اگر (oi && oi->ماخذ_infop->ماخذ->type == ODB_ماخذ_PACKED)
		واپس payload->callback(oid, oi->ماخذ_infop->u.packed.pack,
					 oi->ماخذ_infop->u.packed.انحراف,
					 payload->payload);
	واپس payload->callback(oid, تہی, 0, payload->payload);
}

ساکن خالی batch_each_object(ساخت batch_options *opt,
			      for_each_object_fn callback,
			      غیرعلامتی flags,
			      خالی *_payload)
{
	ساخت for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	ساخت odb_ماخذ_info ماخذ_info;
	ساخت object_info oi = {
		.ماخذ_infop = &ماخذ_info,
	};
	ساخت odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

ساکن صحیح batch_objects(ساخت batch_options *opt)
{
	ساخت strbuf داخلہ = STRBUF_INIT;
	ساخت strbuf output = STRBUF_INIT;
	ساخت expand_ڈیٹا ڈیٹا = EXPAND_ڈیٹا_INIT;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحیح save_warning;
	صحیح retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	ڈیٹا.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &ڈیٹا);
	ڈیٹا.mark_query = 0;
	strbuf_release(&output);
	اگر (opt->transform_منوال)
		ڈیٹا.بانٹو_on_whitespace = 1;

	اگر (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = تہی;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	اگر (opt->batch_منوال == BATCH_منوال_CONTENTS)
		ڈیٹا.info.typep = &ڈیٹا.type;

	اگر (opt->all_objects) {
		ساخت object_cb_ڈیٹا cb;
		ساخت object_info خالی_خانے = OBJECT_INFO_INIT;

		اگر (!memcmp(&ڈیٹا.info, &خالی_خانے, حجم(خالی_خانے)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			ڈیٹا.skip_object_info = 1;

		اگر (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &ڈیٹا;
		cb.scratch = &output;

		اگر (opt->unordered) {
			ساخت oidset نظر_آیا = OIDSET_INIT;

			cb.نظر_آیا = &نظر_آیا;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_ترتیب, &cb);

			oidset_clear(&نظر_آیا);
		} ورنہ {
			ساخت oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		واپس 0;
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

	اگر (opt->batch_منوال == BATCH_منوال_قطار_AND_DISPATCH) {
		batch_objects_command(opt, &output, &ڈیٹا);
		جاؤ cleanup;
	}

	جب (strbuf_getdelim_strip_crlf(&داخلہ, stdin, opt->داخلہ_delim) != EOF) {
		اگر (ڈیٹا.بانٹو_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			حرف *p = strpbrk(داخلہ.buf, " \t");
			اگر (p) {
				جب (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			ڈیٹا.rest = p;
		}

		batch_one_object(داخلہ.buf, &output, opt, &ڈیٹا);
	}

 cleanup:
	strbuf_release(&داخلہ);
	strbuf_release(&output);
	string_list_clear(&ڈیٹا.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	واپس retval;
}

ساکن صحیح git_cat_file_config(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (userdiff_config(var, قدر) < 0)
		واپس -1;

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن صحیح batch_option_callback(ثابت ساخت option *opt,
				 ثابت حرف *arg,
				 صحیح unset)
{
	ساخت batch_options *bo = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (bo->enabled) {
		واپس error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	اگر (!strcmp(opt->long_name, "batch"))
		bo->batch_منوال = BATCH_منوال_CONTENTS;
	ورنہ اگر (!strcmp(opt->long_name, "batch-check"))
		bo->batch_منوال = BATCH_منوال_INFO;
	ورنہ اگر (!strcmp(opt->long_name, "batch-command"))
		bo->batch_منوال = BATCH_منوال_قطار_AND_DISPATCH;
	ورنہ
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	واپس 0;
}

صحیح cmd_cat_file(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	صحیح opt = 0;
	صحیح opt_cw = 0;
	صحیح opt_epts = 0;
	ثابت حرف *exp_type = تہی, *obj_name = تہی;
	ساخت batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	صحیح unknown_type = 0;
	صحیح داخلہ_nul_terminated = 0;
	صحیح nul_terminated = 0;
	صحیح ret;

	ثابت حرف * ثابت builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		تہی
	};
	ثابت ساخت option options[] = {
		/* Simple queries */
		OPT_گروہ(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', تہی, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', تہی, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_گروہ(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', تہی, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', تہی, &opt, N_("show object size"), 's'),
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
		OPT_BOOL_F('z', تہی, &داخلہ_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', تہی, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
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
		OPT_STRING(0, "path", &force_راستہ, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_انتہا()
	};

	repo_config(the_repository, git_cat_file_config, تہی);

	batch.بفر_output = -1;

	argc = parse_options(argc, argv, سابقہ, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	اگر (use_mailmap)
		بھرو_mailmap();

	انتخاب (batch.objects_filter.choice) {
	حالت LOFC_DISABLED:
		توڑو;
	حالت LOFC_BLOB_NONE:
	حالت LOFC_BLOB_حد_بندی:
	حالت LOFC_OBJECT_TYPE:
		اگر (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		توڑو;
	طےشدہ:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	اگر (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	اگر (force_راستہ && !opt_cw)
		usage_پیغام_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	اگر (batch.enabled)
		;
	ورنہ اگر (batch.follow_symlinks)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	ورنہ اگر (batch.بفر_output >= 0)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	ورنہ اگر (batch.all_objects)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	ورنہ اگر (داخلہ_nul_terminated)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	ورنہ اگر (nul_terminated)
		usage_پیغام_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.داخلہ_delim = batch.output_delim = '\n';
	اگر (داخلہ_nul_terminated)
		batch.داخلہ_delim = '\0';
	اگر (nul_terminated)
		batch.داخلہ_delim = batch.output_delim = '\0';

	/* Batch defaults */
	اگر (batch.بفر_output < 0)
		batch.بفر_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	/* Return early if we're in batch mode? */
	اگر (batch.enabled) {
		اگر (opt_cw)
			batch.transform_منوال = opt;
		ورنہ اگر (opt && opt != 'b')
			usage_پیغام_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		ورنہ اگر (argc)
			usage_پیغام_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		جاؤ برآمد;
	}

	اگر (opt) {
		اگر (!argc && opt == 'c')
			usage_پیغام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		ورنہ اگر (!argc && opt == 'w')
			usage_پیغام_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		ورنہ اگر (!argc && opt_epts)
			usage_پیغام_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		ورنہ اگر (argc == 1)
			obj_name = argv[0];
		ورنہ
			usage_پیغام_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} ورنہ اگر (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} ورنہ اگر (argc != 2) {
		usage_پیغام_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} ورنہ اگر (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

برآمد:
	list_objects_filter_release(&batch.objects_filter);
	واپس ret;
}
