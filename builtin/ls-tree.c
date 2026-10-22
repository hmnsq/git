/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"

#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "tree.h"
#ضمن "path.h"
#ضمن "quote.h"
#ضمن "parse-options.h"
#ضمن "pathspec.h"

ساكن ثابت حرف * ثابت ls_درخت_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	عدم
};

ساكن فراغ expand_objectsize(بنية strbuf *سطر, ثابت بنية object_id *oid,
			      ثابت تعداد object_type type, بنانشانی صحيح padded)
{
	ساكن ثابت حرف padding[] = "       ";
	ناپ_t min_len = padded ? لڑی_لمبائی(padding) : 0;
	ناپ_t orig_len = سطر->len;
	ناپ_t len;

	إذا (type == OBJ_BLOB) {
		ناپ_t ناپ;
		إذا (odb_read_object_info(the_repository->objects, oid, &ناپ) < 0)
			die(_("could not get object info about '%s'"),
			    oid_منزل_hex(oid));
		strbuf_جوڑو_uint(سطر, ناپ);
	} وإلا {
		strbuf_addstr(سطر, "-");
	}
	len = سطر->len - orig_len;
	إذا (len < min_len)
		strbuf_داخل_کرو(سطر, orig_len, padding, min_len - len);
}

بنية ls_درخت_options {
	بنانشانی null_termination:1;
	صحيح abbrev;
	تعداد ls_درخت_رستہ_options {
		LS_RECURSIVE = 1 << 0,
		LS_درخت_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	بنية pathspec pathspec;
	ثابت حرف *اگیتر;
	ثابت حرف *format;
};

ساكن صحيح show_recursive(بنية ls_درخت_options *options, ثابت حرف *ادھار,
			  ناپ_t baselen, ثابت حرف *pathname)
{
	صحيح i;

	إذا (options->ls_options & LS_RECURSIVE)
		أرجع 1;

	إذا (!options->pathspec.nr)
		أرجع 0;

	لكل (i = 0; i < options->pathspec.nr; i++) {
		ثابت حرف *spec = options->pathspec.اشیاء[i].میل;
		ناپ_t len, speclen;

		إذا (لڑی_مقابلہ(ادھار, spec, baselen))
			استمر;
		len = لڑی_لمبائی(pathname);
		spec += baselen;
		speclen = لڑی_لمبائی(spec);
		إذا (speclen <= len)
			استمر;
		إذا (spec[len] && spec[len] != '/')
			استمر;
		إذا (memcmp(pathname, spec, len))
			استمر;
		أرجع 1;
	}
	أرجع 0;
}

ساكن صحيح show_درخت_fmt(ثابت بنية object_id *oid, بنية strbuf *ادھار,
			 ثابت حرف *pathname, بنانشانی منوال, فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح recurse = 0;
	بنية strbuf sb = STRBUF_INIT;
	تعداد object_type type = object_type(منوال);
	ثابت حرف *format = options->format;

	إذا (type == OBJ_درخت && show_recursive(options, ادھار->buf, ادھار->len, pathname))
		recurse = READ_درخت_RECURSIVE;
	إذا (type == OBJ_درخت && recurse && !(options->ls_options & LS_SHOW_TREES))
		أرجع recurse;
	إذا (type == OBJ_BLOB && (options->ls_options & LS_درخت_ONLY))
		أرجع 0;

	طالما (strbuf_expand_قدم(&sb, &format)) {
		ناپ_t len;

		إذا (skip_اگیتر(format, "%", &format))
			strbuf_addch(&sb, '%');
		وإلا إذا ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وإلا إذا (skip_اگیتر(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", منوال);
		وإلا إذا (skip_اگیتر(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		وإلا إذا (skip_اگیتر(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		وإلا إذا (skip_اگیتر(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		وإلا إذا (skip_اگیتر(format, "(objectname)", &format))
			strbuf_جوڑو_unique_abbrev(&sb, oid, options->abbrev);
		وإلا إذا (skip_اگیتر(format, "(path)", &format)) {
			ثابت حرف *name;
			ثابت حرف *اگیتر = options->اگیتر;
			بنية strbuf sbuf = STRBUF_INIT;
			ناپ_t baselen = ادھار->len;

			strbuf_addstr(ادھار, pathname);
			name = relative_رستہ(ادھار->buf, اگیتر, &sbuf);
			quote_c_style(name, &sb, عدم, 0);
			strbuf_setlen(ادھار, baselen);
			strbuf_release(&sbuf);
		} وإلا
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	أرجع recurse;
}

ساكن صحيح show_درخت_common(بنية ls_درخت_options *options, صحيح *recurse,
			    بنية strbuf *ادھار, ثابت حرف *pathname,
			    تعداد object_type type)
{
	صحيح ret = -1;
	*recurse = 0;

	إذا (type == OBJ_BLOB) {
		إذا (options->ls_options & LS_درخت_ONLY)
			ret = 0;
	} وإلا إذا (type == OBJ_درخت &&
		   show_recursive(options, ادھار->buf, ادھار->len, pathname)) {
		*recurse = READ_درخت_RECURSIVE;
		إذا (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	أرجع ret;
}

ساكن فراغ show_درخت_common_default_long(بنية ls_درخت_options *options,
					  بنية strbuf *ادھار,
					  ثابت حرف *pathname,
					  ثابت ناپ_t baselen)
{
	ثابت حرف *اگیتر = options->اگیتر;

	strbuf_addstr(ادھار, pathname);

	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_رستہ(ادھار->buf, اگیتر, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(ادھار->buf, اگیتر, stdout, '\n');
	}

	strbuf_setlen(ادھار, baselen);
}

ساكن صحيح show_درخت_default(ثابت بنية object_id *oid, بنية strbuf *ادھار,
			     ثابت حرف *pathname, بنانشانی منوال,
			     فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(منوال);

	early = show_درخت_common(options, &recurse, ادھار, pathname, type);
	إذا (early >= 0)
		أرجع early;

	لکھو("%06o %s %s\t", منوال, type_name(object_type(منوال)),
	       repo_لبھو_unique_abbrev(the_repository, oid, options->abbrev));
	show_درخت_common_default_long(options, ادھار, pathname, ادھار->len);
	أرجع recurse;
}

ساكن صحيح show_درخت_long(ثابت بنية object_id *oid, بنية strbuf *ادھار,
			  ثابت حرف *pathname, بنانشانی منوال,
			  فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	حرف ناپ_متن[24];
	تعداد object_type type = object_type(منوال);

	early = show_درخت_common(options, &recurse, ادھار, pathname, type);
	إذا (early >= 0)
		أرجع early;

	إذا (type == OBJ_BLOB) {
		ناپ_t ناپ;
		إذا (odb_read_object_info(the_repository->objects, oid, &ناپ) == OBJ_BAD)
			xsnprintf(ناپ_متن, حجم(ناپ_متن), "BAD");
		وإلا
			xsnprintf(ناپ_متن, حجم(ناپ_متن),
				  "%" PRIuMAX, (uintmax_t)ناپ);
	} وإلا {
		xsnprintf(ناپ_متن, حجم(ناپ_متن), "-");
	}

	لکھو("%06o %s %s %7s\t", منوال, type_name(type),
	       repo_لبھو_unique_abbrev(the_repository, oid, options->abbrev),
	       ناپ_متن);
	show_درخت_common_default_long(options, ادھار, pathname, ادھار->len);
	أرجع recurse;
}

ساكن صحيح show_درخت_name_only(ثابت بنية object_id *oid بن_ورتیا,
			       بنية strbuf *ادھار,
			       ثابت حرف *pathname, بنانشانی منوال,
			       فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	ثابت ناپ_t baselen = ادھار->len;
	تعداد object_type type = object_type(منوال);
	ثابت حرف *اگیتر;

	early = show_درخت_common(options, &recurse, ادھار, pathname, type);
	إذا (early >= 0)
		أرجع early;

	اگیتر = options->اگیتر;
	strbuf_addstr(ادھار, pathname);
	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_رستہ(ادھار->buf, اگیتر, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(ادھار->buf, اگیتر, stdout, '\n');
	}
	strbuf_setlen(ادھار, baselen);
	أرجع recurse;
}

ساكن صحيح show_درخت_object(ثابت بنية object_id *oid, بنية strbuf *ادھار,
			    ثابت حرف *pathname, بنانشانی منوال,
			    فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(منوال);
	ثابت حرف *str;

	early = show_درخت_common(options, &recurse, ادھار, pathname, type);
	إذا (early >= 0)
		أرجع early;

	str = repo_لبھو_unique_abbrev(the_repository, oid, options->abbrev);
	إذا (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} وإلا  {
		puts(str);
	}
	أرجع recurse;
}

تعداد ls_درخت_cmdmode {
	منوال_DEFAULT = 0,
	منوال_LONG,
	منوال_NAME_ONLY,
	منوال_NAME_حالت,
	منوال_OBJECT_ONLY,
};

بنية ls_درخت_cmdmode_منزل_fmt {
	تعداد ls_درخت_cmdmode منوال;
	ثابت حرف *ثابت fmt;
	read_درخت_fn_t fn;
};

ساكن بنية ls_درخت_cmdmode_منزل_fmt ls_درخت_cmdmode_format[] = {
	{
		.منوال = منوال_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_درخت_default,
	},
	{
		.منوال = منوال_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_درخت_long,
	},
	{
		.منوال = منوال_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_درخت_name_only,
	},
	{
		.منوال = منوال_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_درخت_object
	},
	{
		/* fallback */
		.fn = show_درخت_default,
	},
};

صحيح cmd_ls_درخت(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *اگیتر,
		بنية repository *repo بن_ورتیا)
{
	بنية object_id oid;
	بنية درخت *درخت;
	صحيح i, بھریا_درخت = 0;
	صحيح بھریا_name = !اگیتر || !*اگیتر;
	read_درخت_fn_t fn = عدم;
	تعداد ls_درخت_cmdmode cmdmode = منوال_DEFAULT;
	صحيح null_termination = 0;
	بنية ls_درخت_options options = { 0 };
	ثابت بنية option ls_درخت_options[] = {
		OPT_ثنائی_ہندسہ('d', عدم, &options.ls_options, N_("only show trees"),
			LS_درخت_ONLY),
		OPT_ثنائی_ہندسہ('r', عدم, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_ثنائی_ہندسہ('t', عدم, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', عدم, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    منوال_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    منوال_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    منوال_NAME_حالت),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    منوال_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &بھریا_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &بھریا_درخت,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_سرا()
	};
	بنية ls_درخت_cmdmode_منزل_fmt *m2f = ls_درخت_cmdmode_format;
	صحيح ret;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اگیتر, ls_درخت_options,
			     ls_درخت_usage, 0);
	options.null_termination = null_termination;

	إذا (بھریا_درخت)
		اگیتر = عدم;
	options.اگیتر = بھریا_name ? عدم : اگیتر;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	إذا (cmdmode == منوال_NAME_حالت)
		cmdmode = منوال_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	إذا ( (LS_درخت_ONLY|LS_RECURSIVE) ==
	    ((LS_درخت_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	إذا (options.format && cmdmode)
		usage_پیغام_opt(
			_("--format can't be combined with other format-altering options"),
			ls_درخت_usage, ls_درخت_options);
	إذا (argc < 1)
		usage_with_options(ls_درخت_usage, ls_درخت_options);
	إذا (repo_get_oid_with_flags(the_repository, argv[0], &oid,
				    GET_OID_HASH_ANY))
		die("Not a valid object name %s", argv[0]);

	/*
	 * show_recursive() rolls its own matching code and is
	 * generally ignorant of 'struct pathspec'. The magic mask
	 * cannot be lifted until it is converted to use
	 * match_pathspec() or tree_entry_interesting()
	 */
	parse_pathspec(&options.pathspec, PATHSPEC_ALL_MAGIC &
		       ~(PATHSPEC_FROMTOP | PATHSPEC_LITERAL),
		       PATHSPEC_PREFER_CWD,
		       اگیتر, argv + 1);
	لكل (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.اشیاء[i].nowildcard_len = options.pathspec.اشیاء[i].len;
	options.pathspec.has_wildcard = 0;
	درخت = repo_parse_درخت_اسدھا(the_repository, &oid);
	إذا (!درخت)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	طالما (m2f) {
		إذا (!m2f->fmt) {
			fn = options.format ? show_درخت_fmt : show_درخت_default;
		} وإلا إذا (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->منوال;
			fn = m2f->fn;
		} وإلا إذا (!options.format && cmdmode == m2f->منوال) {
			fn = m2f->fn;
		} وإلا {
			m2f++;
			استمر;
		}
		اكسر;
	}

	ret = !!read_درخت(the_repository, درخت, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	أرجع ret;
}
