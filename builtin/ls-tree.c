/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
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

ایستا ثابت نویسه * ثابت ls_درخت_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	پوچ
};

ایستا تهی expand_objectsize(ساختار strbuf *سطر, ثابت ساختار object_id *oid,
			      ثابت شمارش object_type type, unsigned صحیح padded)
{
	ایستا ثابت نویسه padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = سطر->len;
	size_t len;

	اگر (type == OBJ_BLOB) {
		size_t size;
		اگر (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(سطر, size);
	} وگرنه {
		strbuf_addstr(سطر, "-");
	}
	len = سطر->len - orig_len;
	اگر (len < min_len)
		strbuf_درج(سطر, orig_len, padding, min_len - len);
}

ساختار ls_درخت_options {
	unsigned null_termination:1;
	صحیح abbrev;
	شمارش ls_درخت_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_درخت_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	ساختار pathspec pathspec;
	ثابت نویسه *prefix;
	ثابت نویسه *format;
};

ایستا صحیح show_recursive(ساختار ls_درخت_options *options, ثابت نویسه *base,
			  size_t baselen, ثابت نویسه *pathname)
{
	صحیح i;

	اگر (options->ls_options & LS_RECURSIVE)
		بازگشت 1;

	اگر (!options->pathspec.nr)
		بازگشت 0;

	برای (i = 0; i < options->pathspec.nr; i++) {
		ثابت نویسه *spec = options->pathspec.اقلام[i].match;
		size_t len, speclen;

		اگر (strncmp(base, spec, baselen))
			ادامه;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		اگر (speclen <= len)
			ادامه;
		اگر (spec[len] && spec[len] != '/')
			ادامه;
		اگر (memcmp(pathname, spec, len))
			ادامه;
		بازگشت 1;
	}
	بازگشت 0;
}

ایستا صحیح show_درخت_fmt(ثابت ساختار object_id *oid, ساختار strbuf *base,
			 ثابت نویسه *pathname, unsigned mode, تهی *context)
{
	ساختار ls_درخت_options *options = context;
	صحیح recurse = 0;
	ساختار strbuf sb = STRBUF_INIT;
	شمارش object_type type = object_type(mode);
	ثابت نویسه *format = options->format;

	اگر (type == OBJ_درخت && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_درخت_RECURSIVE;
	اگر (type == OBJ_درخت && recurse && !(options->ls_options & LS_SHOW_TREES))
		بازگشت recurse;
	اگر (type == OBJ_BLOB && (options->ls_options & LS_درخت_ONLY))
		بازگشت 0;

	وقتی (strbuf_expand_step(&sb, &format)) {
		size_t len;

		اگر (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		وگرنه اگر ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وگرنه اگر (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		وگرنه اگر (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		وگرنه اگر (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		وگرنه اگر (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		وگرنه اگر (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		وگرنه اگر (skip_prefix(format, "(path)", &format)) {
			ثابت نویسه *name;
			ثابت نویسه *prefix = options->prefix;
			ساختار strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, پوچ, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} وگرنه
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	بازگشت recurse;
}

ایستا صحیح show_درخت_common(ساختار ls_درخت_options *options, صحیح *recurse,
			    ساختار strbuf *base, ثابت نویسه *pathname,
			    شمارش object_type type)
{
	صحیح ret = -1;
	*recurse = 0;

	اگر (type == OBJ_BLOB) {
		اگر (options->ls_options & LS_درخت_ONLY)
			ret = 0;
	} وگرنه اگر (type == OBJ_درخت &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_درخت_RECURSIVE;
		اگر (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	بازگشت ret;
}

ایستا تهی show_درخت_common_default_long(ساختار ls_درخت_options *options,
					  ساختار strbuf *base,
					  ثابت نویسه *pathname,
					  ثابت size_t baselen)
{
	ثابت نویسه *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	اگر (options->null_termination) {
		ساختار strbuf sb = STRBUF_INIT;
		ثابت نویسه *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وگرنه {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

ایستا صحیح show_درخت_default(ثابت ساختار object_id *oid, ساختار strbuf *base,
			     ثابت نویسه *pathname, unsigned mode,
			     تهی *context)
{
	ساختار ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	شمارش object_type type = object_type(mode);

	early = show_درخت_common(options, &recurse, base, pathname, type);
	اگر (early >= 0)
		بازگشت early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_درخت_common_default_long(options, base, pathname, base->len);
	بازگشت recurse;
}

ایستا صحیح show_درخت_long(ثابت ساختار object_id *oid, ساختار strbuf *base,
			  ثابت نویسه *pathname, unsigned mode,
			  تهی *context)
{
	ساختار ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	نویسه size_text[24];
	شمارش object_type type = object_type(mode);

	early = show_درخت_common(options, &recurse, base, pathname, type);
	اگر (early >= 0)
		بازگشت early;

	اگر (type == OBJ_BLOB) {
		size_t size;
		اگر (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, اندازه(size_text), "BAD");
		وگرنه
			xsnprintf(size_text, اندازه(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} وگرنه {
		xsnprintf(size_text, اندازه(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_درخت_common_default_long(options, base, pathname, base->len);
	بازگشت recurse;
}

ایستا صحیح show_درخت_name_only(ثابت ساختار object_id *oid UNUSED,
			       ساختار strbuf *base,
			       ثابت نویسه *pathname, unsigned mode,
			       تهی *context)
{
	ساختار ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	ثابت size_t baselen = base->len;
	شمارش object_type type = object_type(mode);
	ثابت نویسه *prefix;

	early = show_درخت_common(options, &recurse, base, pathname, type);
	اگر (early >= 0)
		بازگشت early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	اگر (options->null_termination) {
		ساختار strbuf sb = STRBUF_INIT;
		ثابت نویسه *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وگرنه {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	بازگشت recurse;
}

ایستا صحیح show_درخت_object(ثابت ساختار object_id *oid, ساختار strbuf *base,
			    ثابت نویسه *pathname, unsigned mode,
			    تهی *context)
{
	ساختار ls_درخت_options *options = context;
	صحیح early;
	صحیح recurse;
	شمارش object_type type = object_type(mode);
	ثابت نویسه *str;

	early = show_درخت_common(options, &recurse, base, pathname, type);
	اگر (early >= 0)
		بازگشت early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	اگر (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} وگرنه  {
		puts(str);
	}
	بازگشت recurse;
}

شمارش ls_درخت_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

ساختار ls_درخت_cmdmode_to_fmt {
	شمارش ls_درخت_cmdmode mode;
	ثابت نویسه *ثابت fmt;
	read_درخت_fn_t fn;
};

ایستا ساختار ls_درخت_cmdmode_to_fmt ls_درخت_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_درخت_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_درخت_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_درخت_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_درخت_object
	},
	{
		/* fallback */
		.fn = show_درخت_default,
	},
};

صحیح cmd_ls_درخت(صحیح argc,
		ثابت نویسه **argv,
		ثابت نویسه *prefix,
		ساختار repository *repo UNUSED)
{
	ساختار object_id oid;
	ساختار درخت *درخت;
	صحیح i, full_درخت = 0;
	صحیح full_name = !prefix || !*prefix;
	read_درخت_fn_t fn = پوچ;
	شمارش ls_درخت_cmdmode cmdmode = MODE_DEFAULT;
	صحیح null_termination = 0;
	ساختار ls_درخت_options options = { 0 };
	ثابت ساختار option ls_درخت_options[] = {
		OPT_BIT('d', پوچ, &options.ls_options, N_("only show trees"),
			LS_درخت_ONLY),
		OPT_BIT('r', پوچ, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', پوچ, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', پوچ, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    MODE_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    MODE_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    MODE_NAME_STATUS),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    MODE_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &full_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &full_درخت,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	ساختار ls_درخت_cmdmode_to_fmt *m2f = ls_درخت_cmdmode_format;
	صحیح ret;

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, ls_درخت_options,
			     ls_درخت_usage, 0);
	options.null_termination = null_termination;

	اگر (full_درخت)
		prefix = پوچ;
	options.prefix = full_name ? پوچ : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	اگر (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	اگر ( (LS_درخت_ONLY|LS_RECURSIVE) ==
	    ((LS_درخت_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	اگر (options.format && cmdmode)
		usage_پیام_opt(
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
		       prefix, argv + 1);
	برای (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.اقلام[i].nowildcard_len = options.pathspec.اقلام[i].len;
	options.pathspec.has_wildcard = 0;
	درخت = repo_parse_درخت_indirect(the_repository, &oid);
	اگر (!درخت)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	وقتی (m2f) {
		اگر (!m2f->fmt) {
			fn = options.format ? show_درخت_fmt : show_درخت_default;
		} وگرنه اگر (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} وگرنه اگر (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} وگرنه {
			m2f++;
			ادامه;
		}
		بشکن;
	}

	ret = !!read_درخت(the_repository, درخت, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	بازگشت ret;
}
