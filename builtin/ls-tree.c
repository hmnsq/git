/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
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

ساكن ثابت حرف * ثابت ls_وڻ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	عدم
};

ساكن فراغ expand_objectsize(بنية strbuf *سٽ, ثابت بنية object_id *oid,
			      ثابت تعداد object_type type, بنانشان صحيح padded)
{
	ساكن ثابت حرف padding[] = "       ";
	سائيز_t min_len = padded ? لڙي_ڊيگهه(padding) : 0;
	سائيز_t orig_len = سٽ->len;
	سائيز_t len;

	إذا (type == OBJ_BLOB) {
		سائيز_t سائيز;
		إذا (odb_read_object_info(the_repository->objects, oid, &سائيز) < 0)
			die(_("could not get object info about '%s'"),
			    oid_منزل_hex(oid));
		strbuf_جوڙيو_uint(سٽ, سائيز);
	} وإلا {
		strbuf_addstr(سٽ, "-");
	}
	len = سٽ->len - orig_len;
	إذا (len < min_len)
		strbuf_وجهو(سٽ, orig_len, padding, min_len - len);
}

بنية ls_وڻ_options {
	بنانشان null_termination:1;
	صحيح abbrev;
	تعداد ls_وڻ_رستو_options {
		LS_RECURSIVE = 1 << 0,
		LS_وڻ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	بنية pathspec pathspec;
	ثابت حرف *اڳياڙي;
	ثابت حرف *format;
};

ساكن صحيح show_recursive(بنية ls_وڻ_options *options, ثابت حرف *بنياد,
			  سائيز_t baselen, ثابت حرف *pathname)
{
	صحيح i;

	إذا (options->ls_options & LS_RECURSIVE)
		أرجع 1;

	إذا (!options->pathspec.nr)
		أرجع 0;

	لكل (i = 0; i < options->pathspec.nr; i++) {
		ثابت حرف *spec = options->pathspec.شيون[i].ميلاپ;
		سائيز_t len, speclen;

		إذا (لڙي_ڀيٽ(بنياد, spec, baselen))
			استمر;
		len = لڙي_ڊيگهه(pathname);
		spec += baselen;
		speclen = لڙي_ڊيگهه(spec);
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

ساكن صحيح show_وڻ_fmt(ثابت بنية object_id *oid, بنية strbuf *بنياد,
			 ثابت حرف *pathname, بنانشان گهڻائي, فراغ *context)
{
	بنية ls_وڻ_options *options = context;
	صحيح recurse = 0;
	بنية strbuf sb = STRBUF_INIT;
	تعداد object_type type = object_type(گهڻائي);
	ثابت حرف *format = options->format;

	إذا (type == OBJ_وڻ && show_recursive(options, بنياد->buf, بنياد->len, pathname))
		recurse = READ_وڻ_RECURSIVE;
	إذا (type == OBJ_وڻ && recurse && !(options->ls_options & LS_SHOW_TREES))
		أرجع recurse;
	إذا (type == OBJ_BLOB && (options->ls_options & LS_وڻ_ONLY))
		أرجع 0;

	طالما (strbuf_expand_قدم(&sb, &format)) {
		سائيز_t len;

		إذا (skip_اڳياڙي(format, "%", &format))
			strbuf_addch(&sb, '%');
		وإلا إذا ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وإلا إذا (skip_اڳياڙي(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", گهڻائي);
		وإلا إذا (skip_اڳياڙي(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		وإلا إذا (skip_اڳياڙي(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		وإلا إذا (skip_اڳياڙي(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		وإلا إذا (skip_اڳياڙي(format, "(objectname)", &format))
			strbuf_جوڙيو_unique_abbrev(&sb, oid, options->abbrev);
		وإلا إذا (skip_اڳياڙي(format, "(path)", &format)) {
			ثابت حرف *name;
			ثابت حرف *اڳياڙي = options->اڳياڙي;
			بنية strbuf sbuf = STRBUF_INIT;
			سائيز_t baselen = بنياد->len;

			strbuf_addstr(بنياد, pathname);
			name = relative_رستو(بنياد->buf, اڳياڙي, &sbuf);
			quote_c_style(name, &sb, عدم, 0);
			strbuf_setlen(بنياد, baselen);
			strbuf_release(&sbuf);
		} وإلا
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	أرجع recurse;
}

ساكن صحيح show_وڻ_common(بنية ls_وڻ_options *options, صحيح *recurse,
			    بنية strbuf *بنياد, ثابت حرف *pathname,
			    تعداد object_type type)
{
	صحيح ret = -1;
	*recurse = 0;

	إذا (type == OBJ_BLOB) {
		إذا (options->ls_options & LS_وڻ_ONLY)
			ret = 0;
	} وإلا إذا (type == OBJ_وڻ &&
		   show_recursive(options, بنياد->buf, بنياد->len, pathname)) {
		*recurse = READ_وڻ_RECURSIVE;
		إذا (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	أرجع ret;
}

ساكن فراغ show_وڻ_common_default_long(بنية ls_وڻ_options *options,
					  بنية strbuf *بنياد,
					  ثابت حرف *pathname,
					  ثابت سائيز_t baselen)
{
	ثابت حرف *اڳياڙي = options->اڳياڙي;

	strbuf_addstr(بنياد, pathname);

	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_رستو(بنياد->buf, اڳياڙي, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(بنياد->buf, اڳياڙي, stdout, '\n');
	}

	strbuf_setlen(بنياد, baselen);
}

ساكن صحيح show_وڻ_default(ثابت بنية object_id *oid, بنية strbuf *بنياد,
			     ثابت حرف *pathname, بنانشان گهڻائي,
			     فراغ *context)
{
	بنية ls_وڻ_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(گهڻائي);

	early = show_وڻ_common(options, &recurse, بنياد, pathname, type);
	إذا (early >= 0)
		أرجع early;

	لکو("%06o %s %s\t", گهڻائي, type_name(object_type(گهڻائي)),
	       repo_ڳوليو_unique_abbrev(the_repository, oid, options->abbrev));
	show_وڻ_common_default_long(options, بنياد, pathname, بنياد->len);
	أرجع recurse;
}

ساكن صحيح show_وڻ_long(ثابت بنية object_id *oid, بنية strbuf *بنياد,
			  ثابت حرف *pathname, بنانشان گهڻائي,
			  فراغ *context)
{
	بنية ls_وڻ_options *options = context;
	صحيح early;
	صحيح recurse;
	حرف سائيز_متن[24];
	تعداد object_type type = object_type(گهڻائي);

	early = show_وڻ_common(options, &recurse, بنياد, pathname, type);
	إذا (early >= 0)
		أرجع early;

	إذا (type == OBJ_BLOB) {
		سائيز_t سائيز;
		إذا (odb_read_object_info(the_repository->objects, oid, &سائيز) == OBJ_BAD)
			xsnprintf(سائيز_متن, حجم(سائيز_متن), "BAD");
		وإلا
			xsnprintf(سائيز_متن, حجم(سائيز_متن),
				  "%" PRIuMAX, (uintmax_t)سائيز);
	} وإلا {
		xsnprintf(سائيز_متن, حجم(سائيز_متن), "-");
	}

	لکو("%06o %s %s %7s\t", گهڻائي, type_name(type),
	       repo_ڳوليو_unique_abbrev(the_repository, oid, options->abbrev),
	       سائيز_متن);
	show_وڻ_common_default_long(options, بنياد, pathname, بنياد->len);
	أرجع recurse;
}

ساكن صحيح show_وڻ_name_only(ثابت بنية object_id *oid اڻ_استعمال_ٿيل,
			       بنية strbuf *بنياد,
			       ثابت حرف *pathname, بنانشان گهڻائي,
			       فراغ *context)
{
	بنية ls_وڻ_options *options = context;
	صحيح early;
	صحيح recurse;
	ثابت سائيز_t baselen = بنياد->len;
	تعداد object_type type = object_type(گهڻائي);
	ثابت حرف *اڳياڙي;

	early = show_وڻ_common(options, &recurse, بنياد, pathname, type);
	إذا (early >= 0)
		أرجع early;

	اڳياڙي = options->اڳياڙي;
	strbuf_addstr(بنياد, pathname);
	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_رستو(بنياد->buf, اڳياڙي, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(بنياد->buf, اڳياڙي, stdout, '\n');
	}
	strbuf_setlen(بنياد, baselen);
	أرجع recurse;
}

ساكن صحيح show_وڻ_object(ثابت بنية object_id *oid, بنية strbuf *بنياد,
			    ثابت حرف *pathname, بنانشان گهڻائي,
			    فراغ *context)
{
	بنية ls_وڻ_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(گهڻائي);
	ثابت حرف *str;

	early = show_وڻ_common(options, &recurse, بنياد, pathname, type);
	إذا (early >= 0)
		أرجع early;

	str = repo_ڳوليو_unique_abbrev(the_repository, oid, options->abbrev);
	إذا (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} وإلا  {
		puts(str);
	}
	أرجع recurse;
}

تعداد ls_وڻ_cmdmode {
	گهڻائي_DEFAULT = 0,
	گهڻائي_LONG,
	گهڻائي_NAME_ONLY,
	گهڻائي_NAME_حالت,
	گهڻائي_OBJECT_ONLY,
};

بنية ls_وڻ_cmdmode_منزل_fmt {
	تعداد ls_وڻ_cmdmode گهڻائي;
	ثابت حرف *ثابت fmt;
	read_وڻ_fn_t fn;
};

ساكن بنية ls_وڻ_cmdmode_منزل_fmt ls_وڻ_cmdmode_format[] = {
	{
		.گهڻائي = گهڻائي_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_وڻ_default,
	},
	{
		.گهڻائي = گهڻائي_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_وڻ_long,
	},
	{
		.گهڻائي = گهڻائي_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_وڻ_name_only,
	},
	{
		.گهڻائي = گهڻائي_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_وڻ_object
	},
	{
		/* fallback */
		.fn = show_وڻ_default,
	},
};

صحيح cmd_ls_وڻ(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *اڳياڙي,
		بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية object_id oid;
	بنية وڻ *وڻ;
	صحيح i, ڀريل_وڻ = 0;
	صحيح ڀريل_name = !اڳياڙي || !*اڳياڙي;
	read_وڻ_fn_t fn = عدم;
	تعداد ls_وڻ_cmdmode cmdmode = گهڻائي_DEFAULT;
	صحيح null_termination = 0;
	بنية ls_وڻ_options options = { 0 };
	ثابت بنية option ls_وڻ_options[] = {
		OPT_ٻٽو_انگ('d', عدم, &options.ls_options, N_("only show trees"),
			LS_وڻ_ONLY),
		OPT_ٻٽو_انگ('r', عدم, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_ٻٽو_انگ('t', عدم, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', عدم, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    گهڻائي_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    گهڻائي_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    گهڻائي_NAME_حالت),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    گهڻائي_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &ڀريل_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &ڀريل_وڻ,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_پڇاڙي()
	};
	بنية ls_وڻ_cmdmode_منزل_fmt *m2f = ls_وڻ_cmdmode_format;
	صحيح ret;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, ls_وڻ_options,
			     ls_وڻ_usage, 0);
	options.null_termination = null_termination;

	إذا (ڀريل_وڻ)
		اڳياڙي = عدم;
	options.اڳياڙي = ڀريل_name ? عدم : اڳياڙي;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	إذا (cmdmode == گهڻائي_NAME_حالت)
		cmdmode = گهڻائي_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	إذا ( (LS_وڻ_ONLY|LS_RECURSIVE) ==
	    ((LS_وڻ_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	إذا (options.format && cmdmode)
		usage_نياپو_opt(
			_("--format can't be combined with other format-altering options"),
			ls_وڻ_usage, ls_وڻ_options);
	إذا (argc < 1)
		usage_with_options(ls_وڻ_usage, ls_وڻ_options);
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
		       اڳياڙي, argv + 1);
	لكل (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.شيون[i].nowildcard_len = options.pathspec.شيون[i].len;
	options.pathspec.has_wildcard = 0;
	وڻ = repo_parse_وڻ_اڻسڌو(the_repository, &oid);
	إذا (!وڻ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	طالما (m2f) {
		إذا (!m2f->fmt) {
			fn = options.format ? show_وڻ_fmt : show_وڻ_default;
		} وإلا إذا (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->گهڻائي;
			fn = m2f->fn;
		} وإلا إذا (!options.format && cmdmode == m2f->گهڻائي) {
			fn = m2f->fn;
		} وإلا {
			m2f++;
			استمر;
		}
		اكسر;
	}

	ret = !!read_وڻ(the_repository, وڻ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	أرجع ret;
}
