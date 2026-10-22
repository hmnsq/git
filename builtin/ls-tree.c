/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"

#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "tree.h"
#شامل "path.h"
#شامل "quote.h"
#شامل "parse-options.h"
#شامل "pathspec.h"

ساکن ثابت حرف * ثابت ls_درخت_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	تہی
};

ساکن خالی expand_objectsize(ساخت strbuf *سطر, ثابت ساخت object_id *oid,
			      ثابت شمار object_type type, غیرعلامتی صحیح padded)
{
	ساکن ثابت حرف padding[] = "       ";
	سائز_t min_len = padded ? لڑی_لمبائی(padding) : 0;
	سائز_t orig_len = سطر->len;
	سائز_t len;

	اگر (type == OBJ_BLOB) {
		سائز_t سائز;
		اگر (odb_read_object_info(the_repository->objects, oid, &سائز) < 0)
			die(_("could not get object info about '%s'"),
			    oid_مقصد_hex(oid));
		strbuf_جوڑو_uint(سطر, سائز);
	} ورنہ {
		strbuf_addstr(سطر, "-");
	}
	len = سطر->len - orig_len;
	اگر (len < min_len)
		strbuf_داخل_کرو(سطر, orig_len, padding, min_len - len);
}

ساخت ls_درخت_options {
	غیرعلامتی null_termination:1;
	صحیح abbrev;
	شمار ls_درخت_راستہ_options {
		LS_RECURSIVE = 1 << 0,
		LS_درخت_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	ساخت pathspec pathspec;
	ثابت حرف *سابقہ;
	ثابت حرف *format;
};

ساکن صحیح show_recursive(ساخت ls_درخت_options *options, ثابت حرف *بنیاد,
			  سائز_t baselen, ثابت حرف *pathname)
{
	صحیح i;

	اگر (options->ls_options & LS_RECURSIVE)
		واپس 1;

	اگر (!options->pathspec.nr)
		واپس 0;

	برائے (i = 0; i < options->pathspec.nr; i++) {
		ثابت حرف *spec = options->pathspec.اشیاء[i].مطابقت;
		سائز_t len, speclen;

		اگر (لڑی_موازنہ(بنیاد, spec, baselen))
			جاری;
		len = لڑی_لمبائی(pathname);
		spec += baselen;
		speclen = لڑی_لمبائی(spec);
		اگر (speclen <= len)
			جاری;
		اگر (spec[len] && spec[len] != '/')
			جاری;
		اگر (memcmp(pathname, spec, len))
			جاری;
		واپس 1;
	}
	واپس 0;
}

ساکن صحیح show_درخت_fmt(ثابت ساخت object_id *oid, ساخت strbuf *بنیاد,
			 ثابت حرف *pathname, غیرعلامتی منوال, خالی *context)
{
	ساخت ls_درخت_options *options = context;
	صحیح recurse = 0;
	ساخت strbuf sb = STRBUF_INIT;
	شمار object_type type = object_type(منوال);
	ثابت حرف *format = options->format;

	اگر (type == OBJ_درخت && show_recursive(options, بنیاد->buf, بنیاد->len, pathname))
		recurse = READ_درخت_RECURSIVE;
	اگر (type == OBJ_درخت && recurse && !(options->ls_options & LS_SHOW_TREES))
		واپس recurse;
	اگر (type == OBJ_BLOB && (options->ls_options & LS_درخت_ONLY))
		واپس 0;

	جب (strbuf_expand_قدم(&sb, &format)) {
		سائز_t len;

		اگر (skip_سابقہ(format, "%", &format))
			strbuf_addch(&sb, '%');
		ورنہ اگر ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ورنہ اگر (skip_سابقہ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", منوال);
		ورنہ اگر (skip_سابقہ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		ورنہ اگر (skip_سابقہ(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		ورنہ اگر (skip_سابقہ(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		ورنہ اگر (skip_سابقہ(format, "(objectname)", &format))
			strbuf_جوڑو_unique_abbrev(&sb, oid, options->abbrev);
		ورنہ اگر (skip_سابقہ(format, "(path)", &format)) {
			ثابت حرف *name;
			ثابت حرف *سابقہ = options->سابقہ;
			ساخت strbuf sbuf = STRBUF_INIT;
			سائز_t baselen = بنیاد->len;

			strbuf_addstr(بنیاد, pathname);
			name = relative_راستہ(بنیاد->buf, سابقہ, &sbuf);
			quote_c_style(name, &sb, تہی, 0);
			strbuf_setlen(بنیاد, baselen);
			strbuf_release(&sbuf);
		} ورنہ
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	واپس recurse;
}

ساکن صحیح show_درخت_common(ساخت ls_درخت_options *options, صحیح *recurse,
			    ساخت strbuf *بنیاد, ثابت حرف *pathname,
			    شمار object_type type)
{
	صحیح ret = -1;
	*recurse = 0;

	اگر (type == OBJ_BLOB) {
		اگر (options->ls_options & LS_درخت_ONLY)
			ret = 0;
	} ورنہ اگر (type == OBJ_درخت &&
		   show_recursive(options, بنیاد->buf, بنیاد->len, pathname)) {
		*recurse = READ_درخت_RECURSIVE;
		اگر (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	واپس ret;
}

ساکن خالی show_درخت_common_default_long(ساخت ls_درخت_options *options,
					  ساخت strbuf *بنیاد,
					  ثابت حرف *pathname,
					  ثابت سائز_t baselen)
{
	ثابت حرف *سابقہ = options->سابقہ;

	strbuf_addstr(بنیاد, pathname);

	اگر (options->null_termination) {
		ساخت strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_راستہ(بنیاد->buf, سابقہ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ورنہ {
		write_name_quoted_relative(بنیاد->buf, سابقہ, stdout, '\n');
	}

	strbuf_setlen(بنیاد, baselen);
}

ساکن صحیح show_درخت_default(ثابت ساخت object_id *oid, ساخت strbuf *بنیاد,
			     ثابت حرف *pathname, غیرعلامتی منوال,
			     خالی *context)
{
	ساخت ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	شمار object_type type = object_type(منوال);

	early = show_درخت_common(options, &recurse, بنیاد, pathname, type);
	اگر (early >= 0)
		واپس early;

	لکھو("%06o %s %s\t", منوال, type_name(object_type(منوال)),
	       repo_ڈھونڈو_unique_abbrev(the_repository, oid, options->abbrev));
	show_درخت_common_default_long(options, بنیاد, pathname, بنیاد->len);
	واپس recurse;
}

ساکن صحیح show_درخت_long(ثابت ساخت object_id *oid, ساخت strbuf *بنیاد,
			  ثابت حرف *pathname, غیرعلامتی منوال,
			  خالی *context)
{
	ساخت ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	حرف سائز_متن[24];
	شمار object_type type = object_type(منوال);

	early = show_درخت_common(options, &recurse, بنیاد, pathname, type);
	اگر (early >= 0)
		واپس early;

	اگر (type == OBJ_BLOB) {
		سائز_t سائز;
		اگر (odb_read_object_info(the_repository->objects, oid, &سائز) == OBJ_BAD)
			xsnprintf(سائز_متن, حجم(سائز_متن), "BAD");
		ورنہ
			xsnprintf(سائز_متن, حجم(سائز_متن),
				  "%" PRIuMAX, (uintmax_t)سائز);
	} ورنہ {
		xsnprintf(سائز_متن, حجم(سائز_متن), "-");
	}

	لکھو("%06o %s %s %7s\t", منوال, type_name(type),
	       repo_ڈھونڈو_unique_abbrev(the_repository, oid, options->abbrev),
	       سائز_متن);
	show_درخت_common_default_long(options, بنیاد, pathname, بنیاد->len);
	واپس recurse;
}

ساکن صحیح show_درخت_name_only(ثابت ساخت object_id *oid غیر_مستعمل,
			       ساخت strbuf *بنیاد,
			       ثابت حرف *pathname, غیرعلامتی منوال,
			       خالی *context)
{
	ساخت ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	ثابت سائز_t baselen = بنیاد->len;
	شمار object_type type = object_type(منوال);
	ثابت حرف *سابقہ;

	early = show_درخت_common(options, &recurse, بنیاد, pathname, type);
	اگر (early >= 0)
		واپس early;

	سابقہ = options->سابقہ;
	strbuf_addstr(بنیاد, pathname);
	اگر (options->null_termination) {
		ساخت strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_راستہ(بنیاد->buf, سابقہ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ورنہ {
		write_name_quoted_relative(بنیاد->buf, سابقہ, stdout, '\n');
	}
	strbuf_setlen(بنیاد, baselen);
	واپس recurse;
}

ساکن صحیح show_درخت_object(ثابت ساخت object_id *oid, ساخت strbuf *بنیاد,
			    ثابت حرف *pathname, غیرعلامتی منوال,
			    خالی *context)
{
	ساخت ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	شمار object_type type = object_type(منوال);
	ثابت حرف *str;

	early = show_درخت_common(options, &recurse, بنیاد, pathname, type);
	اگر (early >= 0)
		واپس early;

	str = repo_ڈھونڈو_unique_abbrev(the_repository, oid, options->abbrev);
	اگر (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} ورنہ  {
		puts(str);
	}
	واپس recurse;
}

شمار ls_درخت_cmdmode {
	منوال_DEFAULT = 0,
	منوال_LONG,
	منوال_NAME_ONLY,
	منوال_NAME_حالت,
	منوال_OBJECT_ONLY,
};

ساخت ls_درخت_cmdmode_مقصد_fmt {
	شمار ls_درخت_cmdmode منوال;
	ثابت حرف *ثابت fmt;
	read_درخت_fn_t fn;
};

ساکن ساخت ls_درخت_cmdmode_مقصد_fmt ls_درخت_cmdmode_format[] = {
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

صحیح cmd_ls_درخت(صحیح argc,
		ثابت حرف **argv,
		ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	ساخت object_id oid;
	ساخت درخت *درخت;
	صحیح i, بھرا_درخت = 0;
	صحیح بھرا_name = !سابقہ || !*سابقہ;
	read_درخت_fn_t fn = تہی;
	شمار ls_درخت_cmdmode cmdmode = منوال_DEFAULT;
	صحیح null_termination = 0;
	ساخت ls_درخت_options options = { 0 };
	ثابت ساخت option ls_درخت_options[] = {
		OPT_ثنائی_ہندسہ('d', تہی, &options.ls_options, N_("only show trees"),
			LS_درخت_ONLY),
		OPT_ثنائی_ہندسہ('r', تہی, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_ثنائی_ہندسہ('t', تہی, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', تہی, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    منوال_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    منوال_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    منوال_NAME_حالت),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    منوال_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &بھرا_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &بھرا_درخت,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_انتہا()
	};
	ساخت ls_درخت_cmdmode_مقصد_fmt *m2f = ls_درخت_cmdmode_format;
	صحیح ret;

	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, ls_درخت_options,
			     ls_درخت_usage, 0);
	options.null_termination = null_termination;

	اگر (بھرا_درخت)
		سابقہ = تہی;
	options.سابقہ = بھرا_name ? تہی : سابقہ;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	اگر (cmdmode == منوال_NAME_حالت)
		cmdmode = منوال_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	اگر ( (LS_درخت_ONLY|LS_RECURSIVE) ==
	    ((LS_درخت_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	اگر (options.format && cmdmode)
		usage_پیغام_opt(
			_("--format can't be combined with other format-altering options"),
			ls_درخت_usage, ls_درخت_options);
	اگر (argc < 1)
		usage_with_options(ls_درخت_usage, ls_درخت_options);
	اگر (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
		       سابقہ, argv + 1);
	برائے (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.اشیاء[i].nowildcard_len = options.pathspec.اشیاء[i].len;
	options.pathspec.has_wildcard = 0;
	درخت = repo_parse_درخت_بالواسطہ(the_repository, &oid);
	اگر (!درخت)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	جب (m2f) {
		اگر (!m2f->fmt) {
			fn = options.format ? show_درخت_fmt : show_درخت_default;
		} ورنہ اگر (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->منوال;
			fn = m2f->fn;
		} ورنہ اگر (!options.format && cmdmode == m2f->منوال) {
			fn = m2f->fn;
		} ورنہ {
			m2f++;
			جاری;
		}
		توڑو;
	}

	ret = !!read_درخت(the_repository, درخت, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	واپس ret;
}
