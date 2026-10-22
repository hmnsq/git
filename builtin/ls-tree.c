/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "pathspec.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം ls_വൃക്ഷം_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ശൂന്യ
};

സ്ഥിതം ശൂന്യം expand_objectsize(ഘടന strbuf *വരി, സ്ഥിരം ഘടന object_id *oid,
			      സ്ഥിരം എണ്ണൽ object_type type, unsigned പൂർണ്ണസംഖ്യ padded)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = വരി->len;
	size_t len;

	എങ്കിൽ (type == OBJ_BLOB) {
		size_t size;
		എങ്കിൽ (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(വരി, size);
	} അല്ലെങ്കിൽ {
		strbuf_addstr(വരി, "-");
	}
	len = വരി->len - orig_len;
	എങ്കിൽ (len < min_len)
		strbuf_തിരുകുക(വരി, orig_len, padding, min_len - len);
}

ഘടന ls_വൃക്ഷം_options {
	unsigned null_termination:1;
	പൂർണ്ണസംഖ്യ abbrev;
	എണ്ണൽ ls_വൃക്ഷം_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_വൃക്ഷം_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	ഘടന pathspec pathspec;
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *format;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ show_recursive(ഘടന ls_വൃക്ഷം_options *options, സ്ഥിരം അക്ഷരം *base,
			  size_t baselen, സ്ഥിരം അക്ഷരം *pathname)
{
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (options->ls_options & LS_RECURSIVE)
		തിരികെ 1;

	എങ്കിൽ (!options->pathspec.nr)
		തിരികെ 0;

	വേണ്ടി (i = 0; i < options->pathspec.nr; i++) {
		സ്ഥിരം അക്ഷരം *spec = options->pathspec.ഇനങ്ങൾ[i].match;
		size_t len, speclen;

		എങ്കിൽ (strncmp(base, spec, baselen))
			തുടരുക;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		എങ്കിൽ (speclen <= len)
			തുടരുക;
		എങ്കിൽ (spec[len] && spec[len] != '/')
			തുടരുക;
		എങ്കിൽ (memcmp(pathname, spec, len))
			തുടരുക;
		തിരികെ 1;
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വൃക്ഷം_fmt(സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *base,
			 സ്ഥിരം അക്ഷരം *pathname, unsigned mode, ശൂന്യം *context)
{
	ഘടന ls_വൃക്ഷം_options *options = context;
	പൂർണ്ണസംഖ്യ recurse = 0;
	ഘടന strbuf sb = STRBUF_INIT;
	എണ്ണൽ object_type type = object_type(mode);
	സ്ഥിരം അക്ഷരം *format = options->format;

	എങ്കിൽ (type == OBJ_വൃക്ഷം && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_വൃക്ഷം_RECURSIVE;
	എങ്കിൽ (type == OBJ_വൃക്ഷം && recurse && !(options->ls_options & LS_SHOW_TREES))
		തിരികെ recurse;
	എങ്കിൽ (type == OBJ_BLOB && (options->ls_options & LS_വൃക്ഷം_ONLY))
		തിരികെ 0;

	ആകുമ്പോൾ (strbuf_expand_step(&sb, &format)) {
		size_t len;

		എങ്കിൽ (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		അല്ലെങ്കിൽ എങ്കിൽ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(path)", &format)) {
			സ്ഥിരം അക്ഷരം *name;
			സ്ഥിരം അക്ഷരം *prefix = options->prefix;
			ഘടന strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, ശൂന്യ, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} അല്ലെങ്കിൽ
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	തിരികെ recurse;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വൃക്ഷം_common(ഘടന ls_വൃക്ഷം_options *options, പൂർണ്ണസംഖ്യ *recurse,
			    ഘടന strbuf *base, സ്ഥിരം അക്ഷരം *pathname,
			    എണ്ണൽ object_type type)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	*recurse = 0;

	എങ്കിൽ (type == OBJ_BLOB) {
		എങ്കിൽ (options->ls_options & LS_വൃക്ഷം_ONLY)
			ret = 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (type == OBJ_വൃക്ഷം &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_വൃക്ഷം_RECURSIVE;
		എങ്കിൽ (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം show_വൃക്ഷം_common_default_long(ഘടന ls_വൃക്ഷം_options *options,
					  ഘടന strbuf *base,
					  സ്ഥിരം അക്ഷരം *pathname,
					  സ്ഥിരം size_t baselen)
{
	സ്ഥിരം അക്ഷരം *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	എങ്കിൽ (options->null_termination) {
		ഘടന strbuf sb = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} അല്ലെങ്കിൽ {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വൃക്ഷം_default(സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *base,
			     സ്ഥിരം അക്ഷരം *pathname, unsigned mode,
			     ശൂന്യം *context)
{
	ഘടന ls_വൃക്ഷം_options *options = context;
	പൂർണ്ണസംഖ്യ early;
	പൂർണ്ണസംഖ്യ recurse;
	എണ്ണൽ object_type type = object_type(mode);

	early = show_വൃക്ഷം_common(options, &recurse, base, pathname, type);
	എങ്കിൽ (early >= 0)
		തിരികെ early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_വൃക്ഷം_common_default_long(options, base, pathname, base->len);
	തിരികെ recurse;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വൃക്ഷം_long(സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *base,
			  സ്ഥിരം അക്ഷരം *pathname, unsigned mode,
			  ശൂന്യം *context)
{
	ഘടന ls_വൃക്ഷം_options *options = context;
	പൂർണ്ണസംഖ്യ early;
	പൂർണ്ണസംഖ്യ recurse;
	അക്ഷരം size_text[24];
	എണ്ണൽ object_type type = object_type(mode);

	early = show_വൃക്ഷം_common(options, &recurse, base, pathname, type);
	എങ്കിൽ (early >= 0)
		തിരികെ early;

	എങ്കിൽ (type == OBJ_BLOB) {
		size_t size;
		എങ്കിൽ (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, വലുപ്പം(size_text), "BAD");
		അല്ലെങ്കിൽ
			xsnprintf(size_text, വലുപ്പം(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} അല്ലെങ്കിൽ {
		xsnprintf(size_text, വലുപ്പം(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_വൃക്ഷം_common_default_long(options, base, pathname, base->len);
	തിരികെ recurse;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വൃക്ഷം_name_only(സ്ഥിരം ഘടന object_id *oid UNUSED,
			       ഘടന strbuf *base,
			       സ്ഥിരം അക്ഷരം *pathname, unsigned mode,
			       ശൂന്യം *context)
{
	ഘടന ls_വൃക്ഷം_options *options = context;
	പൂർണ്ണസംഖ്യ early;
	പൂർണ്ണസംഖ്യ recurse;
	സ്ഥിരം size_t baselen = base->len;
	എണ്ണൽ object_type type = object_type(mode);
	സ്ഥിരം അക്ഷരം *prefix;

	early = show_വൃക്ഷം_common(options, &recurse, base, pathname, type);
	എങ്കിൽ (early >= 0)
		തിരികെ early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	എങ്കിൽ (options->null_termination) {
		ഘടന strbuf sb = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} അല്ലെങ്കിൽ {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	തിരികെ recurse;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വൃക്ഷം_object(സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *base,
			    സ്ഥിരം അക്ഷരം *pathname, unsigned mode,
			    ശൂന്യം *context)
{
	ഘടന ls_വൃക്ഷം_options *options = context;
	പൂർണ്ണസംഖ്യ early;
	പൂർണ്ണസംഖ്യ recurse;
	എണ്ണൽ object_type type = object_type(mode);
	സ്ഥിരം അക്ഷരം *str;

	early = show_വൃക്ഷം_common(options, &recurse, base, pathname, type);
	എങ്കിൽ (early >= 0)
		തിരികെ early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	എങ്കിൽ (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} അല്ലെങ്കിൽ  {
		puts(str);
	}
	തിരികെ recurse;
}

എണ്ണൽ ls_വൃക്ഷം_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

ഘടന ls_വൃക്ഷം_cmdmode_to_fmt {
	എണ്ണൽ ls_വൃക്ഷം_cmdmode mode;
	സ്ഥിരം അക്ഷരം *സ്ഥിരം fmt;
	read_വൃക്ഷം_fn_t fn;
};

സ്ഥിതം ഘടന ls_വൃക്ഷം_cmdmode_to_fmt ls_വൃക്ഷം_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_വൃക്ഷം_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_വൃക്ഷം_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_വൃക്ഷം_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_വൃക്ഷം_object
	},
	{
		/* fallback */
		.fn = show_വൃക്ഷം_default,
	},
};

പൂർണ്ണസംഖ്യ cmd_ls_വൃക്ഷം(പൂർണ്ണസംഖ്യ argc,
		സ്ഥിരം അക്ഷരം **argv,
		സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	ഘടന object_id oid;
	ഘടന വൃക്ഷം *വൃക്ഷം;
	പൂർണ്ണസംഖ്യ i, full_വൃക്ഷം = 0;
	പൂർണ്ണസംഖ്യ full_name = !prefix || !*prefix;
	read_വൃക്ഷം_fn_t fn = ശൂന്യ;
	എണ്ണൽ ls_വൃക്ഷം_cmdmode cmdmode = MODE_DEFAULT;
	പൂർണ്ണസംഖ്യ null_termination = 0;
	ഘടന ls_വൃക്ഷം_options options = { 0 };
	സ്ഥിരം ഘടന option ls_വൃക്ഷം_options[] = {
		OPT_BIT('d', ശൂന്യ, &options.ls_options, N_("only show trees"),
			LS_വൃക്ഷം_ONLY),
		OPT_BIT('r', ശൂന്യ, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', ശൂന്യ, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ശൂന്യ, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_വൃക്ഷം,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	ഘടന ls_വൃക്ഷം_cmdmode_to_fmt *m2f = ls_വൃക്ഷം_cmdmode_format;
	പൂർണ്ണസംഖ്യ ret;

	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, ls_വൃക്ഷം_options,
			     ls_വൃക്ഷം_usage, 0);
	options.null_termination = null_termination;

	എങ്കിൽ (full_വൃക്ഷം)
		prefix = ശൂന്യ;
	options.prefix = full_name ? ശൂന്യ : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	എങ്കിൽ (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	എങ്കിൽ ( (LS_വൃക്ഷം_ONLY|LS_RECURSIVE) ==
	    ((LS_വൃക്ഷം_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	എങ്കിൽ (options.format && cmdmode)
		usage_സന്ദേശം_opt(
			_("--format can't be combined with other format-altering options"),
			ls_വൃക്ഷം_usage, ls_വൃക്ഷം_options);
	എങ്കിൽ (argc < 1)
		usage_with_options(ls_വൃക്ഷം_usage, ls_വൃക്ഷം_options);
	എങ്കിൽ (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	വേണ്ടി (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.ഇനങ്ങൾ[i].nowildcard_len = options.pathspec.ഇനങ്ങൾ[i].len;
	options.pathspec.has_wildcard = 0;
	വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, &oid);
	എങ്കിൽ (!വൃക്ഷം)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	ആകുമ്പോൾ (m2f) {
		എങ്കിൽ (!m2f->fmt) {
			fn = options.format ? show_വൃക്ഷം_fmt : show_വൃക്ഷം_default;
		} അല്ലെങ്കിൽ എങ്കിൽ (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} അല്ലെങ്കിൽ എങ്കിൽ (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} അല്ലെങ്കിൽ {
			m2f++;
			തുടരുക;
		}
		നിർത്തുക;
	}

	ret = !!read_വൃക്ഷം(the_repository, വൃക്ഷം, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	തിരികെ ret;
}
