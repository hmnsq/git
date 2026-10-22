/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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

ساكن ثابت حرف * ثابت ls_ونه_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	عدم
};

ساكن فراغ expand_objectsize(بنية strbuf *کرښه, ثابت بنية object_id *oid,
			      ثابت تعداد object_type type, بینښه صحيح padded)
{
	ساكن ثابت حرف padding[] = "       ";
	اندازه_t min_len = padded ? مزي_اوږدوالی(padding) : 0;
	اندازه_t orig_len = کرښه->len;
	اندازه_t len;

	إذا (type == OBJ_BLOB) {
		اندازه_t اندازه;
		إذا (odb_read_object_info(the_repository->objects, oid, &اندازه) < 0)
			die(_("could not get object info about '%s'"),
			    oid_منزل_hex(oid));
		strbuf_جمع_کړه_uint(کرښه, اندازه);
	} وإلا {
		strbuf_addstr(کرښه, "-");
	}
	len = کرښه->len - orig_len;
	إذا (len < min_len)
		strbuf_ورننباسه(کرښه, orig_len, padding, min_len - len);
}

بنية ls_ونه_options {
	بینښه null_termination:1;
	صحيح abbrev;
	تعداد ls_ونه_لار_options {
		LS_RECURSIVE = 1 << 0,
		LS_ونه_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	بنية pathspec pathspec;
	ثابت حرف *مختاړی;
	ثابت حرف *format;
};

ساكن صحيح show_recursive(بنية ls_ونه_options *options, ثابت حرف *بنسټ,
			  اندازه_t baselen, ثابت حرف *pathname)
{
	صحيح i;

	إذا (options->ls_options & LS_RECURSIVE)
		أرجع 1;

	إذا (!options->pathspec.nr)
		أرجع 0;

	لكل (i = 0; i < options->pathspec.nr; i++) {
		ثابت حرف *spec = options->pathspec.توکي[i].سمون;
		اندازه_t len, speclen;

		إذا (مزي_پرتله(بنسټ, spec, baselen))
			استمر;
		len = مزي_اوږدوالی(pathname);
		spec += baselen;
		speclen = مزي_اوږدوالی(spec);
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

ساكن صحيح show_ونه_fmt(ثابت بنية object_id *oid, بنية strbuf *بنسټ,
			 ثابت حرف *pathname, بینښه ډېرپېښ, فراغ *context)
{
	بنية ls_ونه_options *options = context;
	صحيح recurse = 0;
	بنية strbuf sb = STRBUF_INIT;
	تعداد object_type type = object_type(ډېرپېښ);
	ثابت حرف *format = options->format;

	إذا (type == OBJ_ونه && show_recursive(options, بنسټ->buf, بنسټ->len, pathname))
		recurse = READ_ونه_RECURSIVE;
	إذا (type == OBJ_ونه && recurse && !(options->ls_options & LS_SHOW_TREES))
		أرجع recurse;
	إذا (type == OBJ_BLOB && (options->ls_options & LS_ونه_ONLY))
		أرجع 0;

	طالما (strbuf_expand_ګام(&sb, &format)) {
		اندازه_t len;

		إذا (skip_مختاړی(format, "%", &format))
			strbuf_addch(&sb, '%');
		وإلا إذا ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وإلا إذا (skip_مختاړی(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ډېرپېښ);
		وإلا إذا (skip_مختاړی(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		وإلا إذا (skip_مختاړی(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		وإلا إذا (skip_مختاړی(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		وإلا إذا (skip_مختاړی(format, "(objectname)", &format))
			strbuf_جمع_کړه_unique_abbrev(&sb, oid, options->abbrev);
		وإلا إذا (skip_مختاړی(format, "(path)", &format)) {
			ثابت حرف *name;
			ثابت حرف *مختاړی = options->مختاړی;
			بنية strbuf sbuf = STRBUF_INIT;
			اندازه_t baselen = بنسټ->len;

			strbuf_addstr(بنسټ, pathname);
			name = relative_لار(بنسټ->buf, مختاړی, &sbuf);
			quote_c_style(name, &sb, عدم, 0);
			strbuf_setlen(بنسټ, baselen);
			strbuf_release(&sbuf);
		} وإلا
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	أرجع recurse;
}

ساكن صحيح show_ونه_common(بنية ls_ونه_options *options, صحيح *recurse,
			    بنية strbuf *بنسټ, ثابت حرف *pathname,
			    تعداد object_type type)
{
	صحيح ret = -1;
	*recurse = 0;

	إذا (type == OBJ_BLOB) {
		إذا (options->ls_options & LS_ونه_ONLY)
			ret = 0;
	} وإلا إذا (type == OBJ_ونه &&
		   show_recursive(options, بنسټ->buf, بنسټ->len, pathname)) {
		*recurse = READ_ونه_RECURSIVE;
		إذا (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	أرجع ret;
}

ساكن فراغ show_ونه_common_default_long(بنية ls_ونه_options *options,
					  بنية strbuf *بنسټ,
					  ثابت حرف *pathname,
					  ثابت اندازه_t baselen)
{
	ثابت حرف *مختاړی = options->مختاړی;

	strbuf_addstr(بنسټ, pathname);

	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_لار(بنسټ->buf, مختاړی, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(بنسټ->buf, مختاړی, stdout, '\n');
	}

	strbuf_setlen(بنسټ, baselen);
}

ساكن صحيح show_ونه_default(ثابت بنية object_id *oid, بنية strbuf *بنسټ,
			     ثابت حرف *pathname, بینښه ډېرپېښ,
			     فراغ *context)
{
	بنية ls_ونه_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(ډېرپېښ);

	early = show_ونه_common(options, &recurse, بنسټ, pathname, type);
	إذا (early >= 0)
		أرجع early;

	چاپ_کړه("%06o %s %s\t", ډېرپېښ, type_name(object_type(ډېرپېښ)),
	       repo_پیدا_کړه_unique_abbrev(the_repository, oid, options->abbrev));
	show_ونه_common_default_long(options, بنسټ, pathname, بنسټ->len);
	أرجع recurse;
}

ساكن صحيح show_ونه_long(ثابت بنية object_id *oid, بنية strbuf *بنسټ,
			  ثابت حرف *pathname, بینښه ډېرپېښ,
			  فراغ *context)
{
	بنية ls_ونه_options *options = context;
	صحيح early;
	صحيح recurse;
	حرف اندازه_متن[24];
	تعداد object_type type = object_type(ډېرپېښ);

	early = show_ونه_common(options, &recurse, بنسټ, pathname, type);
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

	چاپ_کړه("%06o %s %s %7s\t", ډېرپېښ, type_name(type),
	       repo_پیدا_کړه_unique_abbrev(the_repository, oid, options->abbrev),
	       اندازه_متن);
	show_ونه_common_default_long(options, بنسټ, pathname, بنسټ->len);
	أرجع recurse;
}

ساكن صحيح show_ونه_name_only(ثابت بنية object_id *oid ناکارول_شوی,
			       بنية strbuf *بنسټ,
			       ثابت حرف *pathname, بینښه ډېرپېښ,
			       فراغ *context)
{
	بنية ls_ونه_options *options = context;
	صحيح early;
	صحيح recurse;
	ثابت اندازه_t baselen = بنسټ->len;
	تعداد object_type type = object_type(ډېرپېښ);
	ثابت حرف *مختاړی;

	early = show_ونه_common(options, &recurse, بنسټ, pathname, type);
	إذا (early >= 0)
		أرجع early;

	مختاړی = options->مختاړی;
	strbuf_addstr(بنسټ, pathname);
	إذا (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_لار(بنسټ->buf, مختاړی, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(بنسټ->buf, مختاړی, stdout, '\n');
	}
	strbuf_setlen(بنسټ, baselen);
	أرجع recurse;
}

ساكن صحيح show_ونه_object(ثابت بنية object_id *oid, بنية strbuf *بنسټ,
			    ثابت حرف *pathname, بینښه ډېرپېښ,
			    فراغ *context)
{
	بنية ls_ونه_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(ډېرپېښ);
	ثابت حرف *str;

	early = show_ونه_common(options, &recurse, بنسټ, pathname, type);
	إذا (early >= 0)
		أرجع early;

	str = repo_پیدا_کړه_unique_abbrev(the_repository, oid, options->abbrev);
	إذا (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} وإلا  {
		puts(str);
	}
	أرجع recurse;
}

تعداد ls_ونه_cmdmode {
	ډېرپېښ_DEFAULT = 0,
	ډېرپېښ_LONG,
	ډېرپېښ_NAME_ONLY,
	ډېرپېښ_NAME_حالت,
	ډېرپېښ_OBJECT_ONLY,
};

بنية ls_ونه_cmdmode_منزل_fmt {
	تعداد ls_ونه_cmdmode ډېرپېښ;
	ثابت حرف *ثابت fmt;
	read_ونه_fn_t fn;
};

ساكن بنية ls_ونه_cmdmode_منزل_fmt ls_ونه_cmdmode_format[] = {
	{
		.ډېرپېښ = ډېرپېښ_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_ونه_default,
	},
	{
		.ډېرپېښ = ډېرپېښ_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_ونه_long,
	},
	{
		.ډېرپېښ = ډېرپېښ_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_ونه_name_only,
	},
	{
		.ډېرپېښ = ډېرپېښ_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_ونه_object
	},
	{
		/* fallback */
		.fn = show_ونه_default,
	},
};

صحيح cmd_ls_ونه(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *مختاړی,
		بنية repository *repo ناکارول_شوی)
{
	بنية object_id oid;
	بنية ونه *ونه;
	صحيح i, ډک_ونه = 0;
	صحيح ډک_name = !مختاړی || !*مختاړی;
	read_ونه_fn_t fn = عدم;
	تعداد ls_ونه_cmdmode cmdmode = ډېرپېښ_DEFAULT;
	صحيح null_termination = 0;
	بنية ls_ونه_options options = { 0 };
	ثابت بنية option ls_ونه_options[] = {
		OPT_بټ('d', عدم, &options.ls_options, N_("only show trees"),
			LS_ونه_ONLY),
		OPT_بټ('r', عدم, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_بټ('t', عدم, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', عدم, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    ډېرپېښ_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    ډېرپېښ_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    ډېرپېښ_NAME_حالت),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    ډېرپېښ_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &ډک_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &ډک_ونه,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_پای_ټکی()
	};
	بنية ls_ونه_cmdmode_منزل_fmt *m2f = ls_ونه_cmdmode_format;
	صحيح ret;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, مختاړی, ls_ونه_options,
			     ls_ونه_usage, 0);
	options.null_termination = null_termination;

	إذا (ډک_ونه)
		مختاړی = عدم;
	options.مختاړی = ډک_name ? عدم : مختاړی;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	إذا (cmdmode == ډېرپېښ_NAME_حالت)
		cmdmode = ډېرپېښ_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	إذا ( (LS_ونه_ONLY|LS_RECURSIVE) ==
	    ((LS_ونه_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	إذا (options.format && cmdmode)
		usage_پیغام_opt(
			_("--format can't be combined with other format-altering options"),
			ls_ونه_usage, ls_ونه_options);
	إذا (argc < 1)
		usage_with_options(ls_ونه_usage, ls_ونه_options);
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
		       مختاړی, argv + 1);
	لكل (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.توکي[i].nowildcard_len = options.pathspec.توکي[i].len;
	options.pathspec.has_wildcard = 0;
	ونه = repo_parse_ونه_غیرمستقیم(the_repository, &oid);
	إذا (!ونه)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	طالما (m2f) {
		إذا (!m2f->fmt) {
			fn = options.format ? show_ونه_fmt : show_ونه_default;
		} وإلا إذا (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->ډېرپېښ;
			fn = m2f->fn;
		} وإلا إذا (!options.format && cmdmode == m2f->ډېرپېښ) {
			fn = m2f->fn;
		} وإلا {
			m2f++;
			استمر;
		}
		اكسر;
	}

	ret = !!read_ونه(the_repository, ونه, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	أرجع ret;
}
