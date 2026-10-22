/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
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

ساكن ثابت حرف * ثابت ls_tree_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	عدم
};

ساكن فراغ expand_objectsize(بنية strbuf *line, ثابت بنية object_id *oid,
			      ثابت تعداد object_type type, unsigned صحيح padded)
{
	ساكن ثابت حرف padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = line->len;
	size_t len;

	𐤀𐤌 (type == OBJ_BLOB) {
		size_t size;
		𐤀𐤌 (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(line, size);
	} وإلا {
		strbuf_addstr(line, "-");
	}
	len = line->len - orig_len;
	𐤀𐤌 (len < min_len)
		strbuf_insert(line, orig_len, padding, min_len - len);
}

بنية ls_tree_options {
	unsigned null_termination:1;
	صحيح abbrev;
	تعداد ls_tree_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_TREE_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	بنية pathspec pathspec;
	ثابت حرف *prefix;
	ثابت حرف *format;
};

ساكن صحيح show_recursive(بنية ls_tree_options *options, ثابت حرف *base,
			  size_t baselen, ثابت حرف *pathname)
{
	صحيح i;

	𐤀𐤌 (options->ls_options & LS_RECURSIVE)
		أرجع 1;

	𐤀𐤌 (!options->pathspec.nr)
		أرجع 0;

	لكل (i = 0; i < options->pathspec.nr; i++) {
		ثابت حرف *spec = options->pathspec.items[i].match;
		size_t len, speclen;

		𐤀𐤌 (strncmp(base, spec, baselen))
			استمر;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		𐤀𐤌 (speclen <= len)
			استمر;
		𐤀𐤌 (spec[len] && spec[len] != '/')
			استمر;
		𐤀𐤌 (memcmp(pathname, spec, len))
			استمر;
		أرجع 1;
	}
	أرجع 0;
}

ساكن صحيح show_tree_fmt(ثابت بنية object_id *oid, بنية strbuf *base,
			 ثابت حرف *pathname, unsigned mode, فراغ *context)
{
	بنية ls_tree_options *options = context;
	صحيح recurse = 0;
	بنية strbuf sb = STRBUF_INIT;
	تعداد object_type type = object_type(mode);
	ثابت حرف *format = options->format;

	𐤀𐤌 (type == OBJ_TREE && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_TREE_RECURSIVE;
	𐤀𐤌 (type == OBJ_TREE && recurse && !(options->ls_options & LS_SHOW_TREES))
		أرجع recurse;
	𐤀𐤌 (type == OBJ_BLOB && (options->ls_options & LS_TREE_ONLY))
		أرجع 0;

	طالما (strbuf_expand_step(&sb, &format)) {
		size_t len;

		𐤀𐤌 (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		وإلا 𐤀𐤌 ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وإلا 𐤀𐤌 (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		وإلا 𐤀𐤌 (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		وإلا 𐤀𐤌 (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		وإلا 𐤀𐤌 (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		وإلا 𐤀𐤌 (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		وإلا 𐤀𐤌 (skip_prefix(format, "(path)", &format)) {
			ثابت حرف *name;
			ثابت حرف *prefix = options->prefix;
			بنية strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, عدم, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} وإلا
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	أرجع recurse;
}

ساكن صحيح show_tree_common(بنية ls_tree_options *options, صحيح *recurse,
			    بنية strbuf *base, ثابت حرف *pathname,
			    تعداد object_type type)
{
	صحيح ret = -1;
	*recurse = 0;

	𐤀𐤌 (type == OBJ_BLOB) {
		𐤀𐤌 (options->ls_options & LS_TREE_ONLY)
			ret = 0;
	} وإلا 𐤀𐤌 (type == OBJ_TREE &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_TREE_RECURSIVE;
		𐤀𐤌 (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	أرجع ret;
}

ساكن فراغ show_tree_common_default_long(بنية ls_tree_options *options,
					  بنية strbuf *base,
					  ثابت حرف *pathname,
					  ثابت size_t baselen)
{
	ثابت حرف *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	𐤀𐤌 (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

ساكن صحيح show_tree_default(ثابت بنية object_id *oid, بنية strbuf *base,
			     ثابت حرف *pathname, unsigned mode,
			     فراغ *context)
{
	بنية ls_tree_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	𐤀𐤌 (early >= 0)
		أرجع early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_tree_common_default_long(options, base, pathname, base->len);
	أرجع recurse;
}

ساكن صحيح show_tree_long(ثابت بنية object_id *oid, بنية strbuf *base,
			  ثابت حرف *pathname, unsigned mode,
			  فراغ *context)
{
	بنية ls_tree_options *options = context;
	صحيح early;
	صحيح recurse;
	حرف size_text[24];
	تعداد object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	𐤀𐤌 (early >= 0)
		أرجع early;

	𐤀𐤌 (type == OBJ_BLOB) {
		size_t size;
		𐤀𐤌 (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, حجم(size_text), "BAD");
		وإلا
			xsnprintf(size_text, حجم(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} وإلا {
		xsnprintf(size_text, حجم(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_tree_common_default_long(options, base, pathname, base->len);
	أرجع recurse;
}

ساكن صحيح show_tree_name_only(ثابت بنية object_id *oid UNUSED,
			       بنية strbuf *base,
			       ثابت حرف *pathname, unsigned mode,
			       فراغ *context)
{
	بنية ls_tree_options *options = context;
	صحيح early;
	صحيح recurse;
	ثابت size_t baselen = base->len;
	تعداد object_type type = object_type(mode);
	ثابت حرف *prefix;

	early = show_tree_common(options, &recurse, base, pathname, type);
	𐤀𐤌 (early >= 0)
		أرجع early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	𐤀𐤌 (options->null_termination) {
		بنية strbuf sb = STRBUF_INIT;
		ثابت حرف *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} وإلا {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	أرجع recurse;
}

ساكن صحيح show_tree_object(ثابت بنية object_id *oid, بنية strbuf *base,
			    ثابت حرف *pathname, unsigned mode,
			    فراغ *context)
{
	بنية ls_tree_options *options = context;
	صحيح early;
	صحيح recurse;
	تعداد object_type type = object_type(mode);
	ثابت حرف *str;

	early = show_tree_common(options, &recurse, base, pathname, type);
	𐤀𐤌 (early >= 0)
		أرجع early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	𐤀𐤌 (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} وإلا  {
		puts(str);
	}
	أرجع recurse;
}

تعداد ls_tree_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

بنية ls_tree_cmdmode_to_fmt {
	تعداد ls_tree_cmdmode mode;
	ثابت حرف *ثابت fmt;
	read_tree_fn_t fn;
};

ساكن بنية ls_tree_cmdmode_to_fmt ls_tree_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_tree_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_tree_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_tree_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_tree_object
	},
	{
		/* fallback */
		.fn = show_tree_default,
	},
};

صحيح cmd_ls_tree(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	بنية object_id oid;
	بنية tree *tree;
	صحيح i, full_tree = 0;
	صحيح full_name = !prefix || !*prefix;
	read_tree_fn_t fn = عدم;
	تعداد ls_tree_cmdmode cmdmode = MODE_DEFAULT;
	صحيح null_termination = 0;
	بنية ls_tree_options options = { 0 };
	ثابت بنية option ls_tree_options[] = {
		OPT_BIT('d', عدم, &options.ls_options, N_("only show trees"),
			LS_TREE_ONLY),
		OPT_BIT('r', عدم, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', عدم, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', عدم, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_tree,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	بنية ls_tree_cmdmode_to_fmt *m2f = ls_tree_cmdmode_format;
	صحيح ret;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, prefix, ls_tree_options,
			     ls_tree_usage, 0);
	options.null_termination = null_termination;

	𐤀𐤌 (full_tree)
		prefix = عدم;
	options.prefix = full_name ? عدم : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	𐤀𐤌 (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	𐤀𐤌 ( (LS_TREE_ONLY|LS_RECURSIVE) ==
	    ((LS_TREE_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	𐤀𐤌 (options.format && cmdmode)
		usage_msg_opt(
			_("--format can't be combined with other format-altering options"),
			ls_tree_usage, ls_tree_options);
	𐤀𐤌 (argc < 1)
		usage_with_options(ls_tree_usage, ls_tree_options);
	𐤀𐤌 (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	لكل (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.items[i].nowildcard_len = options.pathspec.items[i].len;
	options.pathspec.has_wildcard = 0;
	tree = repo_parse_tree_indirect(the_repository, &oid);
	𐤀𐤌 (!tree)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	طالما (m2f) {
		𐤀𐤌 (!m2f->fmt) {
			fn = options.format ? show_tree_fmt : show_tree_default;
		} وإلا 𐤀𐤌 (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} وإلا 𐤀𐤌 (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} وإلا {
			m2f++;
			استمر;
		}
		اكسر;
	}

	ret = !!read_tree(the_repository, tree, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	أرجع ret;
}
