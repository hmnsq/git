/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"

#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "tree.h"
#समावेश "path.h"
#समावेश "quote.h"
#समावेश "parse-options.h"
#समावेश "pathspec.h"

अटल स्थिर अक्षर * स्थिर ls_वृक्ष_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	शून्य
};

अटल व्योम expand_objectsize(काष्ठा strbuf *पंक्ति, स्थिर काष्ठा object_id *oid,
			      स्थिर क्रमागत object_type type, अचिन्हित पूर्णांक padded)
{
	अटल स्थिर अक्षर padding[] = "       ";
	माप_प्रकार min_len = padded ? म_माप(padding) : 0;
	माप_प्रकार orig_len = पंक्ति->len;
	माप_प्रकार len;

	अगर (type == OBJ_BLOB) {
		माप_प्रकार आकार;
		अगर (odb_read_object_info(the_repository->objects, oid, &आकार) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(पंक्ति, आकार);
	} अन्यथा {
		strbuf_addstr(पंक्ति, "-");
	}
	len = पंक्ति->len - orig_len;
	अगर (len < min_len)
		strbuf_जोड़ो(पंक्ति, orig_len, padding, min_len - len);
}

काष्ठा ls_वृक्ष_options {
	अचिन्हित null_termination:1;
	पूर्णांक abbrev;
	क्रमागत ls_वृक्ष_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_वृक्ष_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	काष्ठा pathspec pathspec;
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *format;
};

अटल पूर्णांक show_recursive(काष्ठा ls_वृक्ष_options *options, स्थिर अक्षर *base,
			  माप_प्रकार baselen, स्थिर अक्षर *pathname)
{
	पूर्णांक i;

	अगर (options->ls_options & LS_RECURSIVE)
		वापस 1;

	अगर (!options->pathspec.nr)
		वापस 0;

	क्रम (i = 0; i < options->pathspec.nr; i++) {
		स्थिर अक्षर *spec = options->pathspec.वस्तुएँ[i].match;
		माप_प्रकार len, speclen;

		अगर (म_भेदन(base, spec, baselen))
			जारी;
		len = म_माप(pathname);
		spec += baselen;
		speclen = म_माप(spec);
		अगर (speclen <= len)
			जारी;
		अगर (spec[len] && spec[len] != '/')
			जारी;
		अगर (स_भेद(pathname, spec, len))
			जारी;
		वापस 1;
	}
	वापस 0;
}

अटल पूर्णांक show_वृक्ष_fmt(स्थिर काष्ठा object_id *oid, काष्ठा strbuf *base,
			 स्थिर अक्षर *pathname, अचिन्हित mode, व्योम *context)
{
	काष्ठा ls_वृक्ष_options *options = context;
	पूर्णांक recurse = 0;
	काष्ठा strbuf sb = STRBUF_INIT;
	क्रमागत object_type type = object_type(mode);
	स्थिर अक्षर *format = options->format;

	अगर (type == OBJ_वृक्ष && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_वृक्ष_RECURSIVE;
	अगर (type == OBJ_वृक्ष && recurse && !(options->ls_options & LS_SHOW_TREES))
		वापस recurse;
	अगर (type == OBJ_BLOB && (options->ls_options & LS_वृक्ष_ONLY))
		वापस 0;

	जबतक (strbuf_expand_step(&sb, &format)) {
		माप_प्रकार len;

		अगर (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		अन्यथा अगर ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		अन्यथा अगर (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		अन्यथा अगर (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		अन्यथा अगर (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		अन्यथा अगर (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		अन्यथा अगर (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		अन्यथा अगर (skip_prefix(format, "(path)", &format)) {
			स्थिर अक्षर *name;
			स्थिर अक्षर *prefix = options->prefix;
			काष्ठा strbuf sbuf = STRBUF_INIT;
			माप_प्रकार baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, शून्य, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} अन्यथा
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	ख_डालो(sb.buf, sb.len, 1, मानक_निकास);
	strbuf_release(&sb);
	वापस recurse;
}

अटल पूर्णांक show_वृक्ष_common(काष्ठा ls_वृक्ष_options *options, पूर्णांक *recurse,
			    काष्ठा strbuf *base, स्थिर अक्षर *pathname,
			    क्रमागत object_type type)
{
	पूर्णांक ret = -1;
	*recurse = 0;

	अगर (type == OBJ_BLOB) {
		अगर (options->ls_options & LS_वृक्ष_ONLY)
			ret = 0;
	} अन्यथा अगर (type == OBJ_वृक्ष &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_वृक्ष_RECURSIVE;
		अगर (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	वापस ret;
}

अटल व्योम show_वृक्ष_common_default_long(काष्ठा ls_वृक्ष_options *options,
					  काष्ठा strbuf *base,
					  स्थिर अक्षर *pathname,
					  स्थिर माप_प्रकार baselen)
{
	स्थिर अक्षर *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	अगर (options->null_termination) {
		काष्ठा strbuf sb = STRBUF_INIT;
		स्थिर अक्षर *name = relative_path(base->buf, prefix, &sb);

		ख_माला_दो(name, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, मानक_निकास, '\n');
	}

	strbuf_setlen(base, baselen);
}

अटल पूर्णांक show_वृक्ष_default(स्थिर काष्ठा object_id *oid, काष्ठा strbuf *base,
			     स्थिर अक्षर *pathname, अचिन्हित mode,
			     व्योम *context)
{
	काष्ठा ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	क्रमागत object_type type = object_type(mode);

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	अगर (early >= 0)
		वापस early;

	म_लिखो("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_वृक्ष_common_default_long(options, base, pathname, base->len);
	वापस recurse;
}

अटल पूर्णांक show_वृक्ष_long(स्थिर काष्ठा object_id *oid, काष्ठा strbuf *base,
			  स्थिर अक्षर *pathname, अचिन्हित mode,
			  व्योम *context)
{
	काष्ठा ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	अक्षर आकार_text[24];
	क्रमागत object_type type = object_type(mode);

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	अगर (early >= 0)
		वापस early;

	अगर (type == OBJ_BLOB) {
		माप_प्रकार आकार;
		अगर (odb_read_object_info(the_repository->objects, oid, &आकार) == OBJ_BAD)
			xsnprintf(आकार_text, माप(आकार_text), "BAD");
		अन्यथा
			xsnprintf(आकार_text, माप(आकार_text),
				  "%" PRIuMAX, (uintmax_t)आकार);
	} अन्यथा {
		xsnprintf(आकार_text, माप(आकार_text), "-");
	}

	म_लिखो("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       आकार_text);
	show_वृक्ष_common_default_long(options, base, pathname, base->len);
	वापस recurse;
}

अटल पूर्णांक show_वृक्ष_name_only(स्थिर काष्ठा object_id *oid UNUSED,
			       काष्ठा strbuf *base,
			       स्थिर अक्षर *pathname, अचिन्हित mode,
			       व्योम *context)
{
	काष्ठा ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	स्थिर माप_प्रकार baselen = base->len;
	क्रमागत object_type type = object_type(mode);
	स्थिर अक्षर *prefix;

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	अगर (early >= 0)
		वापस early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	अगर (options->null_termination) {
		काष्ठा strbuf sb = STRBUF_INIT;
		स्थिर अक्षर *name = relative_path(base->buf, prefix, &sb);

		ख_माला_दो(name, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, मानक_निकास, '\n');
	}
	strbuf_setlen(base, baselen);
	वापस recurse;
}

अटल पूर्णांक show_वृक्ष_object(स्थिर काष्ठा object_id *oid, काष्ठा strbuf *base,
			    स्थिर अक्षर *pathname, अचिन्हित mode,
			    व्योम *context)
{
	काष्ठा ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	क्रमागत object_type type = object_type(mode);
	स्थिर अक्षर *str;

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	अगर (early >= 0)
		वापस early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	अगर (options->null_termination) {
		ख_माला_दो(str, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);
	} अन्यथा  {
		माला_दो(str);
	}
	वापस recurse;
}

क्रमागत ls_वृक्ष_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

काष्ठा ls_वृक्ष_cmdmode_to_fmt {
	क्रमागत ls_वृक्ष_cmdmode mode;
	स्थिर अक्षर *स्थिर fmt;
	read_वृक्ष_fn_t fn;
};

अटल काष्ठा ls_वृक्ष_cmdmode_to_fmt ls_वृक्ष_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_वृक्ष_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_वृक्ष_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_वृक्ष_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_वृक्ष_object
	},
	{
		/* fallback */
		.fn = show_वृक्ष_default,
	},
};

पूर्णांक cmd_ls_वृक्ष(पूर्णांक argc,
		स्थिर अक्षर **argv,
		स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	काष्ठा object_id oid;
	काष्ठा वृक्ष *वृक्ष;
	पूर्णांक i, full_वृक्ष = 0;
	पूर्णांक full_name = !prefix || !*prefix;
	read_वृक्ष_fn_t fn = शून्य;
	क्रमागत ls_वृक्ष_cmdmode cmdmode = MODE_DEFAULT;
	पूर्णांक null_termination = 0;
	काष्ठा ls_वृक्ष_options options = { 0 };
	स्थिर काष्ठा option ls_वृक्ष_options[] = {
		OPT_BIT('d', शून्य, &options.ls_options, N_("only show trees"),
			LS_वृक्ष_ONLY),
		OPT_BIT('r', शून्य, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', शून्य, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', शून्य, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_वृक्ष,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	काष्ठा ls_वृक्ष_cmdmode_to_fmt *m2f = ls_वृक्ष_cmdmode_format;
	पूर्णांक ret;

	repo_config(the_repository, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, ls_वृक्ष_options,
			     ls_वृक्ष_usage, 0);
	options.null_termination = null_termination;

	अगर (full_वृक्ष)
		prefix = शून्य;
	options.prefix = full_name ? शून्य : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	अगर (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	अगर ( (LS_वृक्ष_ONLY|LS_RECURSIVE) ==
	    ((LS_वृक्ष_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	अगर (options.format && cmdmode)
		usage_संदेश_opt(
			_("--format can't be combined with other format-altering options"),
			ls_वृक्ष_usage, ls_वृक्ष_options);
	अगर (argc < 1)
		usage_with_options(ls_वृक्ष_usage, ls_वृक्ष_options);
	अगर (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	क्रम (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.वस्तुएँ[i].nowildcard_len = options.pathspec.वस्तुएँ[i].len;
	options.pathspec.has_wildcard = 0;
	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &oid);
	अगर (!वृक्ष)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	जबतक (m2f) {
		अगर (!m2f->fmt) {
			fn = options.format ? show_वृक्ष_fmt : show_वृक्ष_default;
		} अन्यथा अगर (options.format && !म_भेद(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} अन्यथा अगर (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} अन्यथा {
			m2f++;
			जारी;
		}
		अवरोध;
	}

	ret = !!read_वृक्ष(the_repository, वृक्ष, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	वापस ret;
}
