/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"

#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pathspec.h"

स्थिर अचर वर्ण * अचर ls_रूख_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	रिक्त
};

स्थिर शून्य expand_objectsize(संरचना strbuf *पङ्क्ति, अचर संरचना object_id *oid,
			      अचर गणना object_type type, अचिन्हित पूर्णाङ्क padded)
{
	स्थिर अचर वर्ण padding[] = "       ";
	माप_प्रकार min_len = padded ? म_माप(padding) : 0;
	माप_प्रकार orig_len = पङ्क्ति->len;
	माप_प्रकार len;

	यदि (type == OBJ_BLOB) {
		माप_प्रकार size;
		यदि (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(पङ्क्ति, size);
	} अन्यथा {
		strbuf_addstr(पङ्क्ति, "-");
	}
	len = पङ्क्ति->len - orig_len;
	यदि (len < min_len)
		strbuf_घुसाउ(पङ्क्ति, orig_len, padding, min_len - len);
}

संरचना ls_रूख_options {
	अचिन्हित null_termination:1;
	पूर्णाङ्क abbrev;
	गणना ls_रूख_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_रूख_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	संरचना pathspec pathspec;
	अचर वर्ण *prefix;
	अचर वर्ण *format;
};

स्थिर पूर्णाङ्क show_recursive(संरचना ls_रूख_options *options, अचर वर्ण *base,
			  माप_प्रकार baselen, अचर वर्ण *pathname)
{
	पूर्णाङ्क i;

	यदि (options->ls_options & LS_RECURSIVE)
		फर्काऊ 1;

	यदि (!options->pathspec.nr)
		फर्काऊ 0;

	लागि (i = 0; i < options->pathspec.nr; i++) {
		अचर वर्ण *spec = options->pathspec.वस्तुहरू[i].match;
		माप_प्रकार len, speclen;

		यदि (म_भेदन(base, spec, baselen))
			जारी;
		len = म_माप(pathname);
		spec += baselen;
		speclen = म_माप(spec);
		यदि (speclen <= len)
			जारी;
		यदि (spec[len] && spec[len] != '/')
			जारी;
		यदि (स_भेद(pathname, spec, len))
			जारी;
		फर्काऊ 1;
	}
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क show_रूख_fmt(अचर संरचना object_id *oid, संरचना strbuf *base,
			 अचर वर्ण *pathname, अचिन्हित mode, शून्य *context)
{
	संरचना ls_रूख_options *options = context;
	पूर्णाङ्क recurse = 0;
	संरचना strbuf sb = STRBUF_INIT;
	गणना object_type type = object_type(mode);
	अचर वर्ण *format = options->format;

	यदि (type == OBJ_रूख && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_रूख_RECURSIVE;
	यदि (type == OBJ_रूख && recurse && !(options->ls_options & LS_SHOW_TREES))
		फर्काऊ recurse;
	यदि (type == OBJ_BLOB && (options->ls_options & LS_रूख_ONLY))
		फर्काऊ 0;

	जबसम्म (strbuf_expand_step(&sb, &format)) {
		माप_प्रकार len;

		यदि (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		अन्यथा यदि ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		अन्यथा यदि (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		अन्यथा यदि (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		अन्यथा यदि (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		अन्यथा यदि (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		अन्यथा यदि (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		अन्यथा यदि (skip_prefix(format, "(path)", &format)) {
			अचर वर्ण *name;
			अचर वर्ण *prefix = options->prefix;
			संरचना strbuf sbuf = STRBUF_INIT;
			माप_प्रकार baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, रिक्त, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} अन्यथा
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	ख_डालो(sb.buf, sb.len, 1, मानक_निकास);
	strbuf_release(&sb);
	फर्काऊ recurse;
}

स्थिर पूर्णाङ्क show_रूख_common(संरचना ls_रूख_options *options, पूर्णाङ्क *recurse,
			    संरचना strbuf *base, अचर वर्ण *pathname,
			    गणना object_type type)
{
	पूर्णाङ्क ret = -1;
	*recurse = 0;

	यदि (type == OBJ_BLOB) {
		यदि (options->ls_options & LS_रूख_ONLY)
			ret = 0;
	} अन्यथा यदि (type == OBJ_रूख &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_रूख_RECURSIVE;
		यदि (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	फर्काऊ ret;
}

स्थिर शून्य show_रूख_common_default_long(संरचना ls_रूख_options *options,
					  संरचना strbuf *base,
					  अचर वर्ण *pathname,
					  अचर माप_प्रकार baselen)
{
	अचर वर्ण *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	यदि (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		अचर वर्ण *name = relative_path(base->buf, prefix, &sb);

		ख_माला_दो(name, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, मानक_निकास, '\n');
	}

	strbuf_setlen(base, baselen);
}

स्थिर पूर्णाङ्क show_रूख_default(अचर संरचना object_id *oid, संरचना strbuf *base,
			     अचर वर्ण *pathname, अचिन्हित mode,
			     शून्य *context)
{
	संरचना ls_रूख_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	गणना object_type type = object_type(mode);

	early = show_रूख_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		फर्काऊ early;

	म_लिखो("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_रूख_common_default_long(options, base, pathname, base->len);
	फर्काऊ recurse;
}

स्थिर पूर्णाङ्क show_रूख_long(अचर संरचना object_id *oid, संरचना strbuf *base,
			  अचर वर्ण *pathname, अचिन्हित mode,
			  शून्य *context)
{
	संरचना ls_रूख_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	वर्ण size_text[24];
	गणना object_type type = object_type(mode);

	early = show_रूख_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		फर्काऊ early;

	यदि (type == OBJ_BLOB) {
		माप_प्रकार size;
		यदि (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, परिमाण(size_text), "BAD");
		अन्यथा
			xsnprintf(size_text, परिमाण(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} अन्यथा {
		xsnprintf(size_text, परिमाण(size_text), "-");
	}

	म_लिखो("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_रूख_common_default_long(options, base, pathname, base->len);
	फर्काऊ recurse;
}

स्थिर पूर्णाङ्क show_रूख_name_only(अचर संरचना object_id *oid UNUSED,
			       संरचना strbuf *base,
			       अचर वर्ण *pathname, अचिन्हित mode,
			       शून्य *context)
{
	संरचना ls_रूख_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	अचर माप_प्रकार baselen = base->len;
	गणना object_type type = object_type(mode);
	अचर वर्ण *prefix;

	early = show_रूख_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		फर्काऊ early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	यदि (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		अचर वर्ण *name = relative_path(base->buf, prefix, &sb);

		ख_माला_दो(name, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, मानक_निकास, '\n');
	}
	strbuf_setlen(base, baselen);
	फर्काऊ recurse;
}

स्थिर पूर्णाङ्क show_रूख_object(अचर संरचना object_id *oid, संरचना strbuf *base,
			    अचर वर्ण *pathname, अचिन्हित mode,
			    शून्य *context)
{
	संरचना ls_रूख_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	गणना object_type type = object_type(mode);
	अचर वर्ण *str;

	early = show_रूख_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		फर्काऊ early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	यदि (options->null_termination) {
		ख_माला_दो(str, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);
	} अन्यथा  {
		माला_दो(str);
	}
	फर्काऊ recurse;
}

गणना ls_रूख_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

संरचना ls_रूख_cmdmode_to_fmt {
	गणना ls_रूख_cmdmode mode;
	अचर वर्ण *अचर fmt;
	read_रूख_fn_t fn;
};

स्थिर संरचना ls_रूख_cmdmode_to_fmt ls_रूख_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_रूख_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_रूख_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_रूख_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_रूख_object
	},
	{
		/* fallback */
		.fn = show_रूख_default,
	},
};

पूर्णाङ्क cmd_ls_रूख(पूर्णाङ्क argc,
		अचर वर्ण **argv,
		अचर वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना object_id oid;
	संरचना रूख *रूख;
	पूर्णाङ्क i, full_रूख = 0;
	पूर्णाङ्क full_name = !prefix || !*prefix;
	read_रूख_fn_t fn = रिक्त;
	गणना ls_रूख_cmdmode cmdmode = MODE_DEFAULT;
	पूर्णाङ्क null_termination = 0;
	संरचना ls_रूख_options options = { 0 };
	अचर संरचना option ls_रूख_options[] = {
		OPT_BIT('d', रिक्त, &options.ls_options, N_("only show trees"),
			LS_रूख_ONLY),
		OPT_BIT('r', रिक्त, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', रिक्त, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', रिक्त, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_रूख,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	संरचना ls_रूख_cmdmode_to_fmt *m2f = ls_रूख_cmdmode_format;
	पूर्णाङ्क ret;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, ls_रूख_options,
			     ls_रूख_usage, 0);
	options.null_termination = null_termination;

	यदि (full_रूख)
		prefix = रिक्त;
	options.prefix = full_name ? रिक्त : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	यदि (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	यदि ( (LS_रूख_ONLY|LS_RECURSIVE) ==
	    ((LS_रूख_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	यदि (options.format && cmdmode)
		usage_सन्देश_opt(
			_("--format can't be combined with other format-altering options"),
			ls_रूख_usage, ls_रूख_options);
	यदि (argc < 1)
		usage_with_options(ls_रूख_usage, ls_रूख_options);
	यदि (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	लागि (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.वस्तुहरू[i].nowildcard_len = options.pathspec.वस्तुहरू[i].len;
	options.pathspec.has_wildcard = 0;
	रूख = repo_parse_रूख_indirect(the_repository, &oid);
	यदि (!रूख)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	जबसम्म (m2f) {
		यदि (!m2f->fmt) {
			fn = options.format ? show_रूख_fmt : show_रूख_default;
		} अन्यथा यदि (options.format && !म_भेद(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} अन्यथा यदि (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} अन्यथा {
			m2f++;
			जारी;
		}
		रोक;
	}

	ret = !!read_रूख(the_repository, रूख, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	फर्काऊ ret;
}
