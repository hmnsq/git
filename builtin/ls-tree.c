/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کن USE_THE_REPOSITORY_VARIABLE
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
			      ثابت تعداد object_type type, بیعلامت صحيح padded)
{
	ساكن ثابت حرف padding[] = "       ";
	اندازه_t min_len = padded ? طول_رشته(padding) : 0;
	اندازه_t orig_len = سطر->len;
	اندازه_t len;

	إذا (type == OBJ_BLOB) {
		اندازه_t اندازه;
		إذا (odb_read_object_info(the_repository->objects, oid, &اندازه) < 0)
			die(_("could not get object info about '%s'"),
			    oid_مقصد_hex(oid));
		strbuf_جمع_کن_uint(سطر, اندازه);
	} وإلا {
		strbuf_addstr(سطر, "-");
	}
	len = سطر->len - orig_len;
	إذا (len < min_len)
		strbuf_درج(سطر, orig_len, padding, min_len - len);
}

بنية ls_درخت_options {
	بیعلامت null_termination:1;
	صحيح abbrev;
	تعداد ls_درخت_مسیر_options {
		LS_RECURSIVE = 1 << 0,
		LS_درخت_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	بنية pathspec pathspec;
	ثابت حرف *پیشوند;
	ثابت حرف *format;
};

ساكن صحيح show_recursive(بنية ls_درخت_options *options, ثابت حرف *پایه,
			  اندازه_t baselen, ثابت حرف *pathname)
{
	صحيح i;

	إذا (options->ls_options & LS_RECURSIVE)
		أرجع 1;

	إذا (!options->pathspec.nr)
		أرجع 0;

	لكل (i = 0; i < options->pathspec.nr; i++) {
		ثابت حرف *spec = options->pathspec.اقلام[i].تطابق;
		اندازه_t len, speclen;

		إذا (مقایسه_رشته(پایه, spec, baselen))
			استمر;
		len = طول_رشته(pathname);
		spec += baselen;
		speclen = طول_رشته(spec);
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

ساكن صحيح show_درخت_fmt(ثابت بنية object_id *oid, بنية strbuf *پایه,
			 ثابت حرف *pathname, بیعلامت نما, فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح recurse = 0;
	بنية strbuf sb = STRBUF_INIT;
	تعداد object_type type = object_type(نما);
	ثابت حرف *format = options->format;

	إذا (type == OBJ_درخت && show_recursive(options, پایه->buf, پایه->len, pathname))
		recurse = READ_درخت_RECURSIVE;
	إذا (type == OBJ_درخت && recurse && !(options->ls_options & LS_SHOW_TREES))
		أرجع recurse;
	إذا (type == OBJ_BLOB && (options->ls_options & LS_درخت_ONLY))
		أرجع 0;

	طالما (strbuf_expand_گام(&sb, &format)) {
		اندازه_t len;

		إذا (skip_پیشوند(format, "%", &format))
			strbuf_addch(&sb, '%');
		وإلا إذا ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وإلا إذا (skip_پیشوند(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", نما);
		وإلا إذا (skip_پیشوند(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		وإلا إذا (skip_پیشوند(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		وإلا إذا (skip_پیشوند(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		وإلا إذا (skip_پیشوند(format, "(objectname)", &format))
			strbuf_جمع_کن_unique_abbrev(&sb, oid, options->abbrev);
		وإلا إذا (skip_پیشوند(format, "(path)", &format)) {
			ثابت حرف *name;
			ثابت حرف *پیشوند = options->پیشوند;
			بنية strbuf sbuf = STRBUF_INIT;
			اندازه_t baselen = پایه->len;

			strbuf_addstr(پایه, pathname);
			name = relative_مسیر(پایه->buf, پیشوند, &sbuf);
			quote_c_style(name, &sb, عدم, 0);
			strbuf_setlen(پایه, baselen);
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
			    بنية strbuf *پایه, ثابت حرف *pathname,
			    تعداد object_type type)
{
	صحيح ret = -1;
	*recurse = 0;

	إذا (type == OBJ_BLOB) {
		إذا (options->ls_options & LS_درخت_ONLY)
			ret = 0;
	} وإلا إذا (type == OBJ_درخت &&
		   show_recursive(options, پایه->buf, پایه->len, pathname)) {
		*recurse = READ_درخت_RECURSIVE;
		إذا (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	أرجع ret;
}

ساكن فراغ show_درخت_common_default_long(بنية ls_درخت_options *options,
					  بنية strbuf *پایه,
					  ثابت حرف *pathname,
					  ثابت اندازه_t baselen)
{
	ثابت حرف *پیشوند = options->پیشوند;

	strbuf_addstr(پایه, pathname);

	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_مسیر(پایه->buf, پیشوند, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(پایه->buf, پیشوند, stdout, '\n');
	}

	strbuf_setlen(پایه, baselen);
}

ساكن صحيح show_درخت_default(ثابت بنية object_id *oid, بنية strbuf *پایه,
			     ثابت حرف *pathname, بیعلامت نما,
			     فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(نما);

	early = show_درخت_common(options, &recurse, پایه, pathname, type);
	إذا (early >= 0)
		أرجع early;

	چاپ_کن("%06o %s %s\t", نما, type_name(object_type(نما)),
	       repo_پیدا_کن_unique_abbrev(the_repository, oid, options->abbrev));
	show_درخت_common_default_long(options, پایه, pathname, پایه->len);
	أرجع recurse;
}

ساكن صحيح show_درخت_long(ثابت بنية object_id *oid, بنية strbuf *پایه,
			  ثابت حرف *pathname, بیعلامت نما,
			  فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	حرف اندازه_متن[24];
	تعداد object_type type = object_type(نما);

	early = show_درخت_common(options, &recurse, پایه, pathname, type);
	إذا (early >= 0)
		أرجع early;

	إذا (type == OBJ_BLOB) {
		اندازه_t اندازه;
		إذا (odb_read_object_info(the_repository->objects, oid, &اندازه) == OBJ_BAD)
			xsnprintf(اندازه_متن, حجم(اندازه_متن), "BAD");
		وإلا
			xsnprintf(اندازه_متن, حجم(اندازه_متن),
				  "%" PRIuMAX, (uintmax_t)اندازه);
	} وإلا {
		xsnprintf(اندازه_متن, حجم(اندازه_متن), "-");
	}

	چاپ_کن("%06o %s %s %7s\t", نما, type_name(type),
	       repo_پیدا_کن_unique_abbrev(the_repository, oid, options->abbrev),
	       اندازه_متن);
	show_درخت_common_default_long(options, پایه, pathname, پایه->len);
	أرجع recurse;
}

ساكن صحيح show_درخت_name_only(ثابت بنية object_id *oid استفادهنشده,
			       بنية strbuf *پایه,
			       ثابت حرف *pathname, بیعلامت نما,
			       فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	ثابت اندازه_t baselen = پایه->len;
	تعداد object_type type = object_type(نما);
	ثابت حرف *پیشوند;

	early = show_درخت_common(options, &recurse, پایه, pathname, type);
	إذا (early >= 0)
		أرجع early;

	پیشوند = options->پیشوند;
	strbuf_addstr(پایه, pathname);
	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_مسیر(پایه->buf, پیشوند, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(پایه->buf, پیشوند, stdout, '\n');
	}
	strbuf_setlen(پایه, baselen);
	أرجع recurse;
}

ساكن صحيح show_درخت_object(ثابت بنية object_id *oid, بنية strbuf *پایه,
			    ثابت حرف *pathname, بیعلامت نما,
			    فراغ *context)
{
	بنية ls_درخت_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(نما);
	ثابت حرف *str;

	early = show_درخت_common(options, &recurse, پایه, pathname, type);
	إذا (early >= 0)
		أرجع early;

	str = repo_پیدا_کن_unique_abbrev(the_repository, oid, options->abbrev);
	إذا (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} وإلا  {
		puts(str);
	}
	أرجع recurse;
}

تعداد ls_درخت_cmdmode {
	نما_DEFAULT = 0,
	نما_LONG,
	نما_NAME_ONLY,
	نما_NAME_وضعیت,
	نما_OBJECT_ONLY,
};

بنية ls_درخت_cmdmode_مقصد_fmt {
	تعداد ls_درخت_cmdmode نما;
	ثابت حرف *ثابت fmt;
	read_درخت_fn_t fn;
};

ساكن بنية ls_درخت_cmdmode_مقصد_fmt ls_درخت_cmdmode_format[] = {
	{
		.نما = نما_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_درخت_default,
	},
	{
		.نما = نما_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_درخت_long,
	},
	{
		.نما = نما_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_درخت_name_only,
	},
	{
		.نما = نما_OBJECT_ONLY,
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
		ثابت حرف *پیشوند,
		بنية repository *repo استفادهنشده)
{
	بنية object_id oid;
	بنية درخت *درخت;
	صحيح i, پر_درخت = 0;
	صحيح پر_name = !پیشوند || !*پیشوند;
	read_درخت_fn_t fn = عدم;
	تعداد ls_درخت_cmdmode cmdmode = نما_DEFAULT;
	صحيح null_termination = 0;
	بنية ls_درخت_options options = { 0 };
	ثابت بنية option ls_درخت_options[] = {
		OPT_بیت('d', عدم, &options.ls_options, N_("only show trees"),
			LS_درخت_ONLY),
		OPT_بیت('r', عدم, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_بیت('t', عدم, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', عدم, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    نما_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    نما_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    نما_NAME_وضعیت),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    نما_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &پر_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &پر_درخت,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_انتها()
	};
	بنية ls_درخت_cmdmode_مقصد_fmt *m2f = ls_درخت_cmdmode_format;
	صحيح ret;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, پیشوند, ls_درخت_options,
			     ls_درخت_usage, 0);
	options.null_termination = null_termination;

	إذا (پر_درخت)
		پیشوند = عدم;
	options.پیشوند = پر_name ? عدم : پیشوند;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	إذا (cmdmode == نما_NAME_وضعیت)
		cmdmode = نما_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	إذا ( (LS_درخت_ONLY|LS_RECURSIVE) ==
	    ((LS_درخت_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	إذا (options.format && cmdmode)
		usage_پیام_opt(
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
		       پیشوند, argv + 1);
	لكل (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.اقلام[i].nowildcard_len = options.pathspec.اقلام[i].len;
	options.pathspec.has_wildcard = 0;
	درخت = repo_parse_درخت_غیرمستقیم(the_repository, &oid);
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
			cmdmode = m2f->نما;
			fn = m2f->fn;
		} وإلا إذا (!options.format && cmdmode == m2f->نما) {
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
