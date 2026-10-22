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

स्थिर ध्रुव वर्ण * ध्रुव ls_वृक्ष_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	रिक्त
};

स्थिर शून्य expand_objectsize(संरचना strbuf *पंक्ति, ध्रुव संरचना object_id *oid,
			      ध्रुव गणना object_type type, unsigned पूर्णाङ्क padded)
{
	स्थिर ध्रुव वर्ण padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = पंक्ति->len;
	size_t len;

	यदि (type == OBJ_BLOB) {
		size_t size;
		यदि (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(पंक्ति, size);
	} अन्यथा {
		strbuf_addstr(पंक्ति, "-");
	}
	len = पंक्ति->len - orig_len;
	यदि (len < min_len)
		strbuf_जोड़ो(पंक्ति, orig_len, padding, min_len - len);
}

संरचना ls_वृक्ष_options {
	unsigned null_termination:1;
	पूर्णाङ्क abbrev;
	गणना ls_वृक्ष_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_वृक्ष_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	संरचना pathspec pathspec;
	ध्रुव वर्ण *prefix;
	ध्रुव वर्ण *format;
};

स्थिर पूर्णाङ्क show_recursive(संरचना ls_वृक्ष_options *options, ध्रुव वर्ण *base,
			  size_t baselen, ध्रुव वर्ण *pathname)
{
	पूर्णाङ्क i;

	यदि (options->ls_options & LS_RECURSIVE)
		प्रत्यागम 1;

	यदि (!options->pathspec.nr)
		प्रत्यागम 0;

	प्रति (i = 0; i < options->pathspec.nr; i++) {
		ध्रुव वर्ण *spec = options->pathspec.वस्तुएँ[i].match;
		size_t len, speclen;

		यदि (strncmp(base, spec, baselen))
			अनुवर्त;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		यदि (speclen <= len)
			अनुवर्त;
		यदि (spec[len] && spec[len] != '/')
			अनुवर्त;
		यदि (memcmp(pathname, spec, len))
			अनुवर्त;
		प्रत्यागम 1;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_वृक्ष_fmt(ध्रुव संरचना object_id *oid, संरचना strbuf *base,
			 ध्रुव वर्ण *pathname, unsigned mode, शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णाङ्क recurse = 0;
	संरचना strbuf sb = STRBUF_INIT;
	गणना object_type type = object_type(mode);
	ध्रुव वर्ण *format = options->format;

	यदि (type == OBJ_वृक्ष && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_वृक्ष_RECURSIVE;
	यदि (type == OBJ_वृक्ष && recurse && !(options->ls_options & LS_SHOW_TREES))
		प्रत्यागम recurse;
	यदि (type == OBJ_BLOB && (options->ls_options & LS_वृक्ष_ONLY))
		प्रत्यागम 0;

	यावत् (strbuf_expand_step(&sb, &format)) {
		size_t len;

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
			ध्रुव वर्ण *name;
			ध्रुव वर्ण *prefix = options->prefix;
			संरचना strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, रिक्त, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} अन्यथा
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्ष_common(संरचना ls_वृक्ष_options *options, पूर्णाङ्क *recurse,
			    संरचना strbuf *base, ध्रुव वर्ण *pathname,
			    गणना object_type type)
{
	पूर्णाङ्क ret = -1;
	*recurse = 0;

	यदि (type == OBJ_BLOB) {
		यदि (options->ls_options & LS_वृक्ष_ONLY)
			ret = 0;
	} अन्यथा यदि (type == OBJ_वृक्ष &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_वृक्ष_RECURSIVE;
		यदि (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	प्रत्यागम ret;
}

स्थिर शून्य show_वृक्ष_common_default_long(संरचना ls_वृक्ष_options *options,
					  संरचना strbuf *base,
					  ध्रुव वर्ण *pathname,
					  ध्रुव size_t baselen)
{
	ध्रुव वर्ण *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	यदि (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		ध्रुव वर्ण *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

स्थिर पूर्णाङ्क show_वृक्ष_default(ध्रुव संरचना object_id *oid, संरचना strbuf *base,
			     ध्रुव वर्ण *pathname, unsigned mode,
			     शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	गणना object_type type = object_type(mode);

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_वृक्ष_common_default_long(options, base, pathname, base->len);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्ष_long(ध्रुव संरचना object_id *oid, संरचना strbuf *base,
			  ध्रुव वर्ण *pathname, unsigned mode,
			  शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	वर्ण size_text[24];
	गणना object_type type = object_type(mode);

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	यदि (type == OBJ_BLOB) {
		size_t size;
		यदि (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, परिमाण(size_text), "BAD");
		अन्यथा
			xsnprintf(size_text, परिमाण(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} अन्यथा {
		xsnprintf(size_text, परिमाण(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_वृक्ष_common_default_long(options, base, pathname, base->len);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्ष_name_only(ध्रुव संरचना object_id *oid UNUSED,
			       संरचना strbuf *base,
			       ध्रुव वर्ण *pathname, unsigned mode,
			       शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	ध्रुव size_t baselen = base->len;
	गणना object_type type = object_type(mode);
	ध्रुव वर्ण *prefix;

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	यदि (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		ध्रुव वर्ण *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्ष_object(ध्रुव संरचना object_id *oid, संरचना strbuf *base,
			    ध्रुव वर्ण *pathname, unsigned mode,
			    शून्य *context)
{
	संरचना ls_वृक्ष_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	गणना object_type type = object_type(mode);
	ध्रुव वर्ण *str;

	early = show_वृक्ष_common(options, &recurse, base, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	यदि (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} अन्यथा  {
		puts(str);
	}
	प्रत्यागम recurse;
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
	ध्रुव वर्ण *ध्रुव fmt;
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

पूर्णाङ्क cmd_ls_वृक्ष(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना object_id oid;
	संरचना वृक्ष *वृक्ष;
	पूर्णाङ्क i, full_वृक्ष = 0;
	पूर्णाङ्क full_name = !prefix || !*prefix;
	read_वृक्ष_fn_t fn = रिक्त;
	गणना ls_वृक्ष_cmdmode cmdmode = MODE_DEFAULT;
	पूर्णाङ्क null_termination = 0;
	संरचना ls_वृक्ष_options options = { 0 };
	ध्रुव संरचना option ls_वृक्ष_options[] = {
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
	पूर्णाङ्क ret;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, ls_वृक्ष_options,
			     ls_वृक्ष_usage, 0);
	options.null_termination = null_termination;

	यदि (full_वृक्ष)
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
	यदि ( (LS_वृक्ष_ONLY|LS_RECURSIVE) ==
	    ((LS_वृक्ष_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	यदि (options.format && cmdmode)
		usage_संदेश_opt(
			_("--format can't be combined with other format-altering options"),
			ls_वृक्ष_usage, ls_वृक्ष_options);
	यदि (argc < 1)
		usage_with_options(ls_वृक्ष_usage, ls_वृक्ष_options);
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
	प्रति (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.वस्तुएँ[i].nowildcard_len = options.pathspec.वस्तुएँ[i].len;
	options.pathspec.has_wildcard = 0;
	वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &oid);
	यदि (!वृक्ष)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	यावत् (m2f) {
		यदि (!m2f->fmt) {
			fn = options.format ? show_वृक्ष_fmt : show_वृक्ष_default;
		} अन्यथा यदि (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} अन्यथा यदि (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} अन्यथा {
			m2f++;
			अनुवर्त;
		}
		भङ्ग;
	}

	ret = !!read_वृक्ष(the_repository, वृक्ष, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	प्रत्यागम ret;
}
