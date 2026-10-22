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

स्थिर स्थिरांक अक्षर * स्थिरांक ls_वृक्ष_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	रिक्त
};

स्थिर शून्य expand_objectsize(संरचना strbuf *ओळ, स्थिरांक संरचना object_id *oid,
			      स्थिरांक गणना object_type type, अचिन्हित पूर्णांक padded)
{
	स्थिर स्थिरांक अक्षर padding[] = "       ";
	माप_प्रकार min_len = padded ? म_माप(padding) : 0;
	माप_प्रकार orig_len = ओळ->len;
	माप_प्रकार len;

	जर (type == OBJ_BLOB) {
		माप_प्रकार size;
		जर (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(ओळ, size);
	} अन्यथा {
		strbuf_addstr(ओळ, "-");
	}
	len = ओळ->len - orig_len;
	जर (len < min_len)
		strbuf_घाला(ओळ, orig_len, padding, min_len - len);
}

संरचना ls_वृक्ष_options {
	अचिन्हित null_termination:1;
	पूर्णांक abbrev;
	गणना ls_वृक्ष_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_वृक्ष_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	संरचना pathspec pathspec;
	स्थिरांक अक्षर *prefix;
	स्थिरांक अक्षर *format;
};

स्थिर पूर्णांक show_recursive(संरचना ls_वृक्ष_options *options, स्थिरांक अक्षर *base,
			  माप_प्रकार baselen, स्थिरांक अक्षर *pathname)
{
	पूर्णांक i;

	जर (options->ls_options & LS_RECURSIVE)
		परत 1;

	जर (!options->pathspec.nr)
		परत 0;

	साठी (i = 0; i < options->pathspec.nr; i++) {
		स्थिरांक अक्षर *spec = options->pathspec.वस्तुसंच[i].match;
		माप_प्रकार len, speclen;

		जर (म_भेदन(base, spec, baselen))
			सुरूठेवा;
		len = म_माप(pathname);
		spec += baselen;
		speclen = म_माप(spec);
		जर (speclen <= len)
			सुरूठेवा;
		जर (spec[len] && spec[len] != '/')
			सुरूठेवा;
		जर (स_भेद(pathname, spec, len))
			सुरूठेवा;
		परत 1;
	}
	परत 0;
}

स्थिर पूर्णांक show_वृक्ष_fmt(स्थिरांक संरचना object_id *oid, संरचना strbuf *base,
			 स्थिरांक अक्षर *pathname, अचिन्हित mode, शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णांक recurse = 0;
	संरचना strbuf sb = STRBUF_INIT;
	गणना object_type type = object_type(mode);
	स्थिरांक अक्षर *format = options->format;

	जर (type == OBJ_वृक्ष && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_वृक्ष_RECURSIVE;
	जर (type == OBJ_वृक्ष && recurse && !(options->ls_options & LS_SHOW_TREES))
		परत recurse;
	जर (type == OBJ_BLOB && (options->ls_options & LS_वृक्ष_ONLY))
		परत 0;

	पर्यंत (strbuf_expand_step(&sb, &format)) {
		माप_प्रकार len;

		जर (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		अन्यथा जर ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		अन्यथा जर (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		अन्यथा जर (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		अन्यथा जर (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		अन्यथा जर (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		अन्यथा जर (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		अन्यथा जर (skip_prefix(format, "(path)", &format)) {
			स्थिरांक अक्षर *name;
			स्थिरांक अक्षर *prefix = options->prefix;
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
	परत recurse;
}

स्थिर पूर्णांक show_वृक्ष_common(संरचना ls_वृक्ष_options *options, पूर्णांक *recurse,
			    संरचना strbuf *base, स्थिरांक अक्षर *pathname,
			    गणना object_type type)
{
	पूर्णांक ret = -1;
	*recurse = 0;

	जर (type == OBJ_BLOB) {
		जर (options->ls_options & LS_वृक्ष_ONLY)
			ret = 0;
	} अन्यथा जर (type == OBJ_वृक्ष &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_वृक्ष_RECURSIVE;
		जर (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	परत ret;
}

स्थिर शून्य show_वृक्ष_common_default_long(संरचना ls_वृक्ष_options *options,
					  संरचना strbuf *base,
					  स्थिरांक अक्षर *pathname,
					  स्थिरांक माप_प्रकार baselen)
{
	स्थिरांक अक्षर *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	जर (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		स्थिरांक अक्षर *name = relative_path(base->buf, prefix, &sb);

		ख_माला_दो(name, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, मानक_निकास, '\n');
	}

	strbuf_setlen(base, baselen);
}

स्थिर पूर्णांक show_वृक्ष_default(स्थिरांक संरचना object_id *oid, संरचना strbuf *base,
			     स्थिरांक अक्षर *pathname, अचिन्हित mode,
			     शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	गणना object_type type = object_type(mode);

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	जर (early >= 0)
		परत early;

	म_लिखो("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_वृक्ष_common_default_long(options, base, pathname, base->len);
	परत recurse;
}

स्थिर पूर्णांक show_वृक्ष_long(स्थिरांक संरचना object_id *oid, संरचना strbuf *base,
			  स्थिरांक अक्षर *pathname, अचिन्हित mode,
			  शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	अक्षर size_text[24];
	गणना object_type type = object_type(mode);

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	जर (early >= 0)
		परत early;

	जर (type == OBJ_BLOB) {
		माप_प्रकार size;
		जर (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, आकार(size_text), "BAD");
		अन्यथा
			xsnprintf(size_text, आकार(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} अन्यथा {
		xsnprintf(size_text, आकार(size_text), "-");
	}

	म_लिखो("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_वृक्ष_common_default_long(options, base, pathname, base->len);
	परत recurse;
}

स्थिर पूर्णांक show_वृक्ष_name_only(स्थिरांक संरचना object_id *oid UNUSED,
			       संरचना strbuf *base,
			       स्थिरांक अक्षर *pathname, अचिन्हित mode,
			       शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	स्थिरांक माप_प्रकार baselen = base->len;
	गणना object_type type = object_type(mode);
	स्थिरांक अक्षर *prefix;

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	जर (early >= 0)
		परत early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	जर (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		स्थिरांक अक्षर *name = relative_path(base->buf, prefix, &sb);

		ख_माला_दो(name, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, मानक_निकास, '\n');
	}
	strbuf_setlen(base, baselen);
	परत recurse;
}

स्थिर पूर्णांक show_वृक्ष_object(स्थिरांक संरचना object_id *oid, संरचना strbuf *base,
			    स्थिरांक अक्षर *pathname, अचिन्हित mode,
			    शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णांक early;
	पूर्णांक recurse;
	गणना object_type type = object_type(mode);
	स्थिरांक अक्षर *str;

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	जर (early >= 0)
		परत early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	जर (options->null_termination) {
		ख_माला_दो(str, मानक_निकास);
		ख_अक्षर_दो('\0', मानक_निकास);
	} अन्यथा  {
		माला_दो(str);
	}
	परत recurse;
}

गणना ls_वृक्ष_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

संरचना ls_वृक्ष_cmdmode_to_fmt {
	गणना ls_वृक्ष_cmdmode mode;
	स्थिरांक अक्षर *स्थिरांक fmt;
	read_वृक्ष_fn_t fn;
};

स्थिर संरचना ls_वृक्ष_cmdmode_to_fmt ls_वृक्ष_cmdmode_format[] = {
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
		स्थिरांक अक्षर **argv,
		स्थिरांक अक्षर *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना object_id oid;
	संरचना वृक्ष *वृक्ष;
	पूर्णांक i, full_वृक्ष = 0;
	पूर्णांक full_name = !prefix || !*prefix;
	read_वृक्ष_fn_t fn = रिक्त;
	गणना ls_वृक्ष_cmdmode cmdmode = MODE_DEFAULT;
	पूर्णांक null_termination = 0;
	संरचना ls_वृक्ष_options options = { 0 };
	स्थिरांक संरचना option ls_वृक्ष_options[] = {
		OPT_BIT('d', रिक्त, &options.ls_options, N_("only show trees"),
			LS_वृक्ष_ONLY),
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
		OPT_BOOL(0, "full-tree", &full_वृक्ष,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	संरचना ls_वृक्ष_cmdmode_to_fmt *m2f = ls_वृक्ष_cmdmode_format;
	पूर्णांक ret;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, ls_वृक्ष_options,
			     ls_वृक्ष_usage, 0);
	options.null_termination = null_termination;

	जर (full_वृक्ष)
		prefix = रिक्त;
	options.prefix = full_name ? रिक्त : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	जर (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	जर ( (LS_वृक्ष_ONLY|LS_RECURSIVE) ==
	    ((LS_वृक्ष_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	जर (options.format && cmdmode)
		usage_संदेश_opt(
			_("--format can't be combined with other format-altering options"),
			ls_वृक्ष_usage, ls_वृक्ष_options);
	जर (argc < 1)
		usage_with_options(ls_वृक्ष_usage, ls_वृक्ष_options);
	जर (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	साठी (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.वस्तुसंच[i].nowildcard_len = options.pathspec.वस्तुसंच[i].len;
	options.pathspec.has_wildcard = 0;
	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &oid);
	जर (!वृक्ष)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	पर्यंत (m2f) {
		जर (!m2f->fmt) {
			fn = options.format ? show_वृक्ष_fmt : show_वृक्ष_default;
		} अन्यथा जर (options.format && !म_भेद(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} अन्यथा जर (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} अन्यथा {
			m2f++;
			सुरूठेवा;
		}
		थांबा;
	}

	ret = !!read_वृक्ष(the_repository, वृक्ष, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	परत ret;
}
