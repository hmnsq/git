/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#परिभाषय USE_THE_REPOSITORY_VARIABLE
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

स्थिर ध्रुव वर्ण * ध्रुव ls_वृक्षः_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	रिक्त
};

स्थिर शून्य expand_objectsize(संरचना strbuf *पङ्क्तिः, ध्रुव संरचना object_id *oid,
			      ध्रुव गणना object_type type, अचिह्नित पूर्णाङ्क padded)
{
	स्थिर ध्रुव वर्ण padding[] = "       ";
	आकार_t min_len = padded ? वाक्यदैर्घ्यम्(padding) : 0;
	आकार_t orig_len = पङ्क्तिः->len;
	आकार_t len;

	यदि (type == OBJ_BLOB) {
		आकार_t आकार;
		यदि (odb_read_object_info(the_repository->objects, oid, &आकार) < 0)
			die(_("could not get object info about '%s'"),
			    oid_गन्तव्यम्_hex(oid));
		strbuf_सङ्कलय_uint(पङ्क्तिः, आकार);
	} अन्यथा {
		strbuf_addstr(पङ्क्तिः, "-");
	}
	len = पङ्क्तिः->len - orig_len;
	यदि (len < min_len)
		strbuf_निवेशय(पङ्क्तिः, orig_len, padding, min_len - len);
}

संरचना ls_वृक्षः_options {
	अचिह्नित null_termination:1;
	पूर्णाङ्क abbrev;
	गणना ls_वृक्षः_मार्गः_options {
		LS_RECURSIVE = 1 << 0,
		LS_वृक्षः_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	संरचना pathspec pathspec;
	ध्रुव वर्ण *उपसर्गः;
	ध्रुव वर्ण *format;
};

स्थिर पूर्णाङ्क show_recursive(संरचना ls_वृक्षः_options *options, ध्रुव वर्ण *आधारः,
			  आकार_t baselen, ध्रुव वर्ण *pathname)
{
	पूर्णाङ्क i;

	यदि (options->ls_options & LS_RECURSIVE)
		प्रत्यागम 1;

	यदि (!options->pathspec.nr)
		प्रत्यागम 0;

	प्रति (i = 0; i < options->pathspec.nr; i++) {
		ध्रुव वर्ण *spec = options->pathspec.वस्तूनि[i].मेलनम्;
		आकार_t len, speclen;

		यदि (वाक्ये_तुलय(आधारः, spec, baselen))
			अनुवर्त;
		len = वाक्यदैर्घ्यम्(pathname);
		spec += baselen;
		speclen = वाक्यदैर्घ्यम्(spec);
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

स्थिर पूर्णाङ्क show_वृक्षः_fmt(ध्रुव संरचना object_id *oid, संरचना strbuf *आधारः,
			 ध्रुव वर्ण *pathname, अचिह्नित बहुलकम्, शून्य *context)
{
	संरचना ls_वृक्षः_options *options = context;
	पूर्णाङ्क recurse = 0;
	संरचना strbuf sb = STRBUF_INIT;
	गणना object_type type = object_type(बहुलकम्);
	ध्रुव वर्ण *format = options->format;

	यदि (type == OBJ_वृक्षः && show_recursive(options, आधारः->buf, आधारः->len, pathname))
		recurse = READ_वृक्षः_RECURSIVE;
	यदि (type == OBJ_वृक्षः && recurse && !(options->ls_options & LS_SHOW_TREES))
		प्रत्यागम recurse;
	यदि (type == OBJ_BLOB && (options->ls_options & LS_वृक्षः_ONLY))
		प्रत्यागम 0;

	यावत् (strbuf_expand_चरणः(&sb, &format)) {
		आकार_t len;

		यदि (skip_उपसर्गः(format, "%", &format))
			strbuf_addch(&sb, '%');
		अन्यथा यदि ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		अन्यथा यदि (skip_उपसर्गः(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", बहुलकम्);
		अन्यथा यदि (skip_उपसर्गः(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		अन्यथा यदि (skip_उपसर्गः(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		अन्यथा यदि (skip_उपसर्गः(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		अन्यथा यदि (skip_उपसर्गः(format, "(objectname)", &format))
			strbuf_सङ्कलय_unique_abbrev(&sb, oid, options->abbrev);
		अन्यथा यदि (skip_उपसर्गः(format, "(path)", &format)) {
			ध्रुव वर्ण *name;
			ध्रुव वर्ण *उपसर्गः = options->उपसर्गः;
			संरचना strbuf sbuf = STRBUF_INIT;
			आकार_t baselen = आधारः->len;

			strbuf_addstr(आधारः, pathname);
			name = relative_मार्गः(आधारः->buf, उपसर्गः, &sbuf);
			quote_c_style(name, &sb, रिक्त, 0);
			strbuf_setlen(आधारः, baselen);
			strbuf_release(&sbuf);
		} अन्यथा
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्षः_common(संरचना ls_वृक्षः_options *options, पूर्णाङ्क *recurse,
			    संरचना strbuf *आधारः, ध्रुव वर्ण *pathname,
			    गणना object_type type)
{
	पूर्णाङ्क ret = -1;
	*recurse = 0;

	यदि (type == OBJ_BLOB) {
		यदि (options->ls_options & LS_वृक्षः_ONLY)
			ret = 0;
	} अन्यथा यदि (type == OBJ_वृक्षः &&
		   show_recursive(options, आधारः->buf, आधारः->len, pathname)) {
		*recurse = READ_वृक्षः_RECURSIVE;
		यदि (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	प्रत्यागम ret;
}

स्थिर शून्य show_वृक्षः_common_default_long(संरचना ls_वृक्षः_options *options,
					  संरचना strbuf *आधारः,
					  ध्रुव वर्ण *pathname,
					  ध्रुव आकार_t baselen)
{
	ध्रुव वर्ण *उपसर्गः = options->उपसर्गः;

	strbuf_addstr(आधारः, pathname);

	यदि (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		ध्रुव वर्ण *name = relative_मार्गः(आधारः->buf, उपसर्गः, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(आधारः->buf, उपसर्गः, stdout, '\n');
	}

	strbuf_setlen(आधारः, baselen);
}

स्थिर पूर्णाङ्क show_वृक्षः_default(ध्रुव संरचना object_id *oid, संरचना strbuf *आधारः,
			     ध्रुव वर्ण *pathname, अचिह्नित बहुलकम्,
			     शून्य *context)
{
	संरचना ls_वृक्षः_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	गणना object_type type = object_type(बहुलकम्);

	early = show_वृक्षः_common(options, &recurse, आधारः, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	प्रारूप_लिख("%06o %s %s\t", बहुलकम्, type_name(object_type(बहुलकम्)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_वृक्षः_common_default_long(options, आधारः, pathname, आधारः->len);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्षः_long(ध्रुव संरचना object_id *oid, संरचना strbuf *आधारः,
			  ध्रुव वर्ण *pathname, अचिह्नित बहुलकम्,
			  शून्य *context)
{
	संरचना ls_वृक्षः_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	वर्ण आकार_text[24];
	गणना object_type type = object_type(बहुलकम्);

	early = show_वृक्षः_common(options, &recurse, आधारः, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	यदि (type == OBJ_BLOB) {
		आकार_t आकार;
		यदि (odb_read_object_info(the_repository->objects, oid, &आकार) == OBJ_BAD)
			xsnprintf(आकार_text, परिमाण(आकार_text), "BAD");
		अन्यथा
			xsnprintf(आकार_text, परिमाण(आकार_text),
				  "%" PRIuMAX, (uintmax_t)आकार);
	} अन्यथा {
		xsnprintf(आकार_text, परिमाण(आकार_text), "-");
	}

	प्रारूप_लिख("%06o %s %s %7s\t", बहुलकम्, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       आकार_text);
	show_वृक्षः_common_default_long(options, आधारः, pathname, आधारः->len);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्षः_name_only(ध्रुव संरचना object_id *oid अप्रयुक्तम्,
			       संरचना strbuf *आधारः,
			       ध्रुव वर्ण *pathname, अचिह्नित बहुलकम्,
			       शून्य *context)
{
	संरचना ls_वृक्षः_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	ध्रुव आकार_t baselen = आधारः->len;
	गणना object_type type = object_type(बहुलकम्);
	ध्रुव वर्ण *उपसर्गः;

	early = show_वृक्षः_common(options, &recurse, आधारः, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	उपसर्गः = options->उपसर्गः;
	strbuf_addstr(आधारः, pathname);
	यदि (options->null_termination) {
		संरचना strbuf sb = STRBUF_INIT;
		ध्रुव वर्ण *name = relative_मार्गः(आधारः->buf, उपसर्गः, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} अन्यथा {
		write_name_quoted_relative(आधारः->buf, उपसर्गः, stdout, '\n');
	}
	strbuf_setlen(आधारः, baselen);
	प्रत्यागम recurse;
}

स्थिर पूर्णाङ्क show_वृक्षः_object(ध्रुव संरचना object_id *oid, संरचना strbuf *आधारः,
			    ध्रुव वर्ण *pathname, अचिह्नित बहुलकम्,
			    शून्य *context)
{
	संरचना ls_वृक्षः_options *options = context;
	पूर्णाङ्क early;
	पूर्णाङ्क recurse;
	गणना object_type type = object_type(बहुलकम्);
	ध्रुव वर्ण *str;

	early = show_वृक्षः_common(options, &recurse, आधारः, pathname, type);
	यदि (early >= 0)
		प्रत्यागम early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	यदि (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} अन्यथा  {
		पङ्क्तिं_लिख(str);
	}
	प्रत्यागम recurse;
}

गणना ls_वृक्षः_cmdmode {
	बहुलकम्_DEFAULT = 0,
	बहुलकम्_LONG,
	बहुलकम्_NAME_ONLY,
	बहुलकम्_NAME_स्थितिः,
	बहुलकम्_OBJECT_ONLY,
};

संरचना ls_वृक्षः_cmdmode_गन्तव्यम्_fmt {
	गणना ls_वृक्षः_cmdmode बहुलकम्;
	ध्रुव वर्ण *ध्रुव fmt;
	read_वृक्षः_fn_t fn;
};

स्थिर संरचना ls_वृक्षः_cmdmode_गन्तव्यम्_fmt ls_वृक्षः_cmdmode_format[] = {
	{
		.बहुलकम् = बहुलकम्_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_वृक्षः_default,
	},
	{
		.बहुलकम् = बहुलकम्_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_वृक्षः_long,
	},
	{
		.बहुलकम् = बहुलकम्_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_वृक्षः_name_only,
	},
	{
		.बहुलकम् = बहुलकम्_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_वृक्षः_object
	},
	{
		/* fallback */
		.fn = show_वृक्षः_default,
	},
};

पूर्णाङ्क cmd_ls_वृक्षः(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo अप्रयुक्तम्)
{
	संरचना object_id oid;
	संरचना वृक्षः *वृक्षः;
	पूर्णाङ्क i, पूर्णम्_वृक्षः = 0;
	पूर्णाङ्क पूर्णम्_name = !उपसर्गः || !*उपसर्गः;
	read_वृक्षः_fn_t fn = रिक्त;
	गणना ls_वृक्षः_cmdmode cmdmode = बहुलकम्_DEFAULT;
	पूर्णाङ्क null_termination = 0;
	संरचना ls_वृक्षः_options options = { 0 };
	ध्रुव संरचना option ls_वृक्षः_options[] = {
		OPT_BIT('d', रिक्त, &options.ls_options, N_("only show trees"),
			LS_वृक्षः_ONLY),
		OPT_BIT('r', रिक्त, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', रिक्त, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', रिक्त, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    बहुलकम्_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    बहुलकम्_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    बहुलकम्_NAME_स्थितिः),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    बहुलकम्_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &पूर्णम्_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &पूर्णम्_वृक्षः,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_अन्तः()
	};
	संरचना ls_वृक्षः_cmdmode_गन्तव्यम्_fmt *m2f = ls_वृक्षः_cmdmode_format;
	पूर्णाङ्क ret;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, उपसर्गः, ls_वृक्षः_options,
			     ls_वृक्षः_usage, 0);
	options.null_termination = null_termination;

	यदि (पूर्णम्_वृक्षः)
		उपसर्गः = रिक्त;
	options.उपसर्गः = पूर्णम्_name ? रिक्त : उपसर्गः;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	यदि (cmdmode == बहुलकम्_NAME_स्थितिः)
		cmdmode = बहुलकम्_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	यदि ( (LS_वृक्षः_ONLY|LS_RECURSIVE) ==
	    ((LS_वृक्षः_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	यदि (options.format && cmdmode)
		usage_सन्देशः_opt(
			_("--format can't be combined with other format-altering options"),
			ls_वृक्षः_usage, ls_वृक्षः_options);
	यदि (argc < 1)
		usage_with_options(ls_वृक्षः_usage, ls_वृक्षः_options);
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
		       उपसर्गः, argv + 1);
	प्रति (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.वस्तूनि[i].nowildcard_len = options.pathspec.वस्तूनि[i].len;
	options.pathspec.has_wildcard = 0;
	वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, &oid);
	यदि (!वृक्षः)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	यावत् (m2f) {
		यदि (!m2f->fmt) {
			fn = options.format ? show_वृक्षः_fmt : show_वृक्षः_default;
		} अन्यथा यदि (options.format && !वाक्यं_तुलय(options.format, m2f->fmt)) {
			cmdmode = m2f->बहुलकम्;
			fn = m2f->fn;
		} अन्यथा यदि (!options.format && cmdmode == m2f->बहुलकम्) {
			fn = m2f->fn;
		} अन्यथा {
			m2f++;
			अनुवर्त;
		}
		भङ्ग;
	}

	ret = !!read_वृक्षः(the_repository, वृक्षः, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	प्रत्यागम ret;
}
