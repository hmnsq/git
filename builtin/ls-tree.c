/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"

#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "pathspec.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক ls_বৃক্ষ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ফাঁকা
};

স্থির শূন্য expand_objectsize(কাঠামো strbuf *সারি, ধ্রুবক কাঠামো object_id *oid,
			      ধ্রুবক গণনা object_type type, unsigned পূর্ণসংখ্যা padded)
{
	স্থির ধ্রুবক অক্ষর padding[] = "       ";
	আকার_t min_len = padded ? strlen(padding) : 0;
	আকার_t orig_len = সারি->len;
	আকার_t len;

	যদি (type == OBJ_BLOB) {
		আকার_t আকার_2;
		যদি (odb_read_object_info(the_repository->objects, oid, &আকার_2) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(সারি, আকার_2);
	} অন্যথা {
		strbuf_addstr(সারি, "-");
	}
	len = সারি->len - orig_len;
	যদি (len < min_len)
		strbuf_ঢোকাও(সারি, orig_len, padding, min_len - len);
}

কাঠামো ls_বৃক্ষ_options {
	unsigned null_termination:1;
	পূর্ণসংখ্যা abbrev;
	গণনা ls_বৃক্ষ_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_বৃক্ষ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	কাঠামো pathspec pathspec;
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *format;
};

স্থির পূর্ণসংখ্যা show_recursive(কাঠামো ls_বৃক্ষ_options *options, ধ্রুবক অক্ষর *base,
			  আকার_t baselen, ধ্রুবক অক্ষর *pathname)
{
	পূর্ণসংখ্যা i;

	যদি (options->ls_options & LS_RECURSIVE)
		ফেরত 1;

	যদি (!options->pathspec.nr)
		ফেরত 0;

	জন্য (i = 0; i < options->pathspec.nr; i++) {
		ধ্রুবক অক্ষর *spec = options->pathspec.বস্তুসমূহ[i].match;
		আকার_t len, speclen;

		যদি (strncmp(base, spec, baselen))
			চালু;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		যদি (speclen <= len)
			চালু;
		যদি (spec[len] && spec[len] != '/')
			চালু;
		যদি (memcmp(pathname, spec, len))
			চালু;
		ফেরত 1;
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা show_বৃক্ষ_fmt(ধ্রুবক কাঠামো object_id *oid, কাঠামো strbuf *base,
			 ধ্রুবক অক্ষর *pathname, unsigned mode, শূন্য *context)
{
	কাঠামো ls_বৃক্ষ_options *options = context;
	পূর্ণসংখ্যা recurse = 0;
	কাঠামো strbuf sb = STRBUF_INIT;
	গণনা object_type type = object_type(mode);
	ধ্রুবক অক্ষর *format = options->format;

	যদি (type == OBJ_বৃক্ষ && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_বৃক্ষ_RECURSIVE;
	যদি (type == OBJ_বৃক্ষ && recurse && !(options->ls_options & LS_SHOW_TREES))
		ফেরত recurse;
	যদি (type == OBJ_BLOB && (options->ls_options & LS_বৃক্ষ_ONLY))
		ফেরত 0;

	যখন (strbuf_expand_step(&sb, &format)) {
		আকার_t len;

		যদি (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		অন্যথা যদি ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		অন্যথা যদি (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		অন্যথা যদি (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		অন্যথা যদি (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		অন্যথা যদি (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		অন্যথা যদি (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		অন্যথা যদি (skip_prefix(format, "(path)", &format)) {
			ধ্রুবক অক্ষর *name;
			ধ্রুবক অক্ষর *prefix = options->prefix;
			কাঠামো strbuf sbuf = STRBUF_INIT;
			আকার_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, ফাঁকা, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} অন্যথা
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ফেরত recurse;
}

স্থির পূর্ণসংখ্যা show_বৃক্ষ_common(কাঠামো ls_বৃক্ষ_options *options, পূর্ণসংখ্যা *recurse,
			    কাঠামো strbuf *base, ধ্রুবক অক্ষর *pathname,
			    গণনা object_type type)
{
	পূর্ণসংখ্যা ret = -1;
	*recurse = 0;

	যদি (type == OBJ_BLOB) {
		যদি (options->ls_options & LS_বৃক্ষ_ONLY)
			ret = 0;
	} অন্যথা যদি (type == OBJ_বৃক্ষ &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_বৃক্ষ_RECURSIVE;
		যদি (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	ফেরত ret;
}

স্থির শূন্য show_বৃক্ষ_common_default_long(কাঠামো ls_বৃক্ষ_options *options,
					  কাঠামো strbuf *base,
					  ধ্রুবক অক্ষর *pathname,
					  ধ্রুবক আকার_t baselen)
{
	ধ্রুবক অক্ষর *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	যদি (options->null_termination) {
		কাঠামো strbuf sb = STRBUF_INIT;
		ধ্রুবক অক্ষর *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} অন্যথা {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

স্থির পূর্ণসংখ্যা show_বৃক্ষ_default(ধ্রুবক কাঠামো object_id *oid, কাঠামো strbuf *base,
			     ধ্রুবক অক্ষর *pathname, unsigned mode,
			     শূন্য *context)
{
	কাঠামো ls_বৃক্ষ_options *options = context;
	পূর্ণসংখ্যা early;
	পূর্ণসংখ্যা recurse;
	গণনা object_type type = object_type(mode);

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ফেরত early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_বৃক্ষ_common_default_long(options, base, pathname, base->len);
	ফেরত recurse;
}

স্থির পূর্ণসংখ্যা show_বৃক্ষ_long(ধ্রুবক কাঠামো object_id *oid, কাঠামো strbuf *base,
			  ধ্রুবক অক্ষর *pathname, unsigned mode,
			  শূন্য *context)
{
	কাঠামো ls_বৃক্ষ_options *options = context;
	পূর্ণসংখ্যা early;
	পূর্ণসংখ্যা recurse;
	অক্ষর আকার_text[24];
	গণনা object_type type = object_type(mode);

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ফেরত early;

	যদি (type == OBJ_BLOB) {
		আকার_t আকার_2;
		যদি (odb_read_object_info(the_repository->objects, oid, &আকার_2) == OBJ_BAD)
			xsnprintf(আকার_text, আকার(আকার_text), "BAD");
		অন্যথা
			xsnprintf(আকার_text, আকার(আকার_text),
				  "%" PRIuMAX, (uintmax_t)আকার_2);
	} অন্যথা {
		xsnprintf(আকার_text, আকার(আকার_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       আকার_text);
	show_বৃক্ষ_common_default_long(options, base, pathname, base->len);
	ফেরত recurse;
}

স্থির পূর্ণসংখ্যা show_বৃক্ষ_name_only(ধ্রুবক কাঠামো object_id *oid UNUSED,
			       কাঠামো strbuf *base,
			       ধ্রুবক অক্ষর *pathname, unsigned mode,
			       শূন্য *context)
{
	কাঠামো ls_বৃক্ষ_options *options = context;
	পূর্ণসংখ্যা early;
	পূর্ণসংখ্যা recurse;
	ধ্রুবক আকার_t baselen = base->len;
	গণনা object_type type = object_type(mode);
	ধ্রুবক অক্ষর *prefix;

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ফেরত early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	যদি (options->null_termination) {
		কাঠামো strbuf sb = STRBUF_INIT;
		ধ্রুবক অক্ষর *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} অন্যথা {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	ফেরত recurse;
}

স্থির পূর্ণসংখ্যা show_বৃক্ষ_object(ধ্রুবক কাঠামো object_id *oid, কাঠামো strbuf *base,
			    ধ্রুবক অক্ষর *pathname, unsigned mode,
			    শূন্য *context)
{
	কাঠামো ls_বৃক্ষ_options *options = context;
	পূর্ণসংখ্যা early;
	পূর্ণসংখ্যা recurse;
	গণনা object_type type = object_type(mode);
	ধ্রুবক অক্ষর *str;

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ফেরত early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	যদি (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} অন্যথা  {
		puts(str);
	}
	ফেরত recurse;
}

গণনা ls_বৃক্ষ_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

কাঠামো ls_বৃক্ষ_cmdmode_to_fmt {
	গণনা ls_বৃক্ষ_cmdmode mode;
	ধ্রুবক অক্ষর *ধ্রুবক fmt;
	read_বৃক্ষ_fn_t fn;
};

স্থির কাঠামো ls_বৃক্ষ_cmdmode_to_fmt ls_বৃক্ষ_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_বৃক্ষ_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_বৃক্ষ_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_বৃক্ষ_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_বৃক্ষ_object
	},
	{
		/* fallback */
		.fn = show_বৃক্ষ_default,
	},
};

পূর্ণসংখ্যা cmd_ls_বৃক্ষ(পূর্ণসংখ্যা argc,
		ধ্রুবক অক্ষর **argv,
		ধ্রুবক অক্ষর *prefix,
		কাঠামো repository *repo UNUSED)
{
	কাঠামো object_id oid;
	কাঠামো বৃক্ষ *বৃক্ষ;
	পূর্ণসংখ্যা i, full_বৃক্ষ = 0;
	পূর্ণসংখ্যা full_name = !prefix || !*prefix;
	read_বৃক্ষ_fn_t fn = ফাঁকা;
	গণনা ls_বৃক্ষ_cmdmode cmdmode = MODE_DEFAULT;
	পূর্ণসংখ্যা null_termination = 0;
	কাঠামো ls_বৃক্ষ_options options = { 0 };
	ধ্রুবক কাঠামো option ls_বৃক্ষ_options[] = {
		OPT_BIT('d', ফাঁকা, &options.ls_options, N_("only show trees"),
			LS_বৃক্ষ_ONLY),
		OPT_BIT('r', ফাঁকা, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', ফাঁকা, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ফাঁকা, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_বৃক্ষ,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	কাঠামো ls_বৃক্ষ_cmdmode_to_fmt *m2f = ls_বৃক্ষ_cmdmode_format;
	পূর্ণসংখ্যা ret;

	repo_config(the_repository, git_default_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, ls_বৃক্ষ_options,
			     ls_বৃক্ষ_usage, 0);
	options.null_termination = null_termination;

	যদি (full_বৃক্ষ)
		prefix = ফাঁকা;
	options.prefix = full_name ? ফাঁকা : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	যদি (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	যদি ( (LS_বৃক্ষ_ONLY|LS_RECURSIVE) ==
	    ((LS_বৃক্ষ_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	যদি (options.format && cmdmode)
		usage_বার্তা_opt(
			_("--format can't be combined with other format-altering options"),
			ls_বৃক্ষ_usage, ls_বৃক্ষ_options);
	যদি (argc < 1)
		usage_with_options(ls_বৃক্ষ_usage, ls_বৃক্ষ_options);
	যদি (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	জন্য (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.বস্তুসমূহ[i].nowildcard_len = options.pathspec.বস্তুসমূহ[i].len;
	options.pathspec.has_wildcard = 0;
	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, &oid);
	যদি (!বৃক্ষ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	যখন (m2f) {
		যদি (!m2f->fmt) {
			fn = options.format ? show_বৃক্ষ_fmt : show_বৃক্ষ_default;
		} অন্যথা যদি (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} অন্যথা যদি (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} অন্যথা {
			m2f++;
			চালু;
		}
		বিরতি;
	}

	ret = !!read_বৃক্ষ(the_repository, বৃক্ষ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ফেরত ret;
}
