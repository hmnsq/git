/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ ls_ವೃಕ್ಷ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಶೂನ್ಯ expand_objectsize(ಸಂರಚನಾ strbuf *ಸಾಲು, ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			      ಧ್ರುವ ಗಣನಾ object_type type, unsigned ಪೂರ್ಣಾಙ್ಕ padded)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = ಸಾಲು->len;
	size_t len;

	ಯದಿ (type == OBJ_BLOB) {
		size_t size;
		ಯದಿ (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(ಸಾಲು, size);
	} ಅನ್ಯಥಾ {
		strbuf_addstr(ಸಾಲು, "-");
	}
	len = ಸಾಲು->len - orig_len;
	ಯದಿ (len < min_len)
		strbuf_ಸೇರಿಸು(ಸಾಲು, orig_len, padding, min_len - len);
}

ಸಂರಚನಾ ls_ವೃಕ್ಷ_options {
	unsigned null_termination:1;
	ಪೂರ್ಣಾಙ್ಕ abbrev;
	ಗಣನಾ ls_ವೃಕ್ಷ_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_ವೃಕ್ಷ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	ಸಂರಚನಾ pathspec pathspec;
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *format;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_recursive(ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options, ಧ್ರುವ ವರ್ಣ *base,
			  size_t baselen, ಧ್ರುವ ವರ್ಣ *pathname)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (options->ls_options & LS_RECURSIVE)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (!options->pathspec.nr)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತಿ (i = 0; i < options->pathspec.nr; i++) {
		ಧ್ರುವ ವರ್ಣ *spec = options->pathspec.ವಸ್ತುಗಳು[i].match;
		size_t len, speclen;

		ಯದಿ (strncmp(base, spec, baselen))
			ಅನುವರ್ತ;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		ಯದಿ (speclen <= len)
			ಅನುವರ್ತ;
		ಯದಿ (spec[len] && spec[len] != '/')
			ಅನುವರ್ತ;
		ಯದಿ (memcmp(pathname, spec, len))
			ಅನುವರ್ತ;
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ವೃಕ್ಷ_fmt(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ strbuf *base,
			 ಧ್ರುವ ವರ್ಣ *pathname, unsigned mode, ಶೂನ್ಯ *context)
{
	ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಙ್ಕ recurse = 0;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಗಣನಾ object_type type = object_type(mode);
	ಧ್ರುವ ವರ್ಣ *format = options->format;

	ಯದಿ (type == OBJ_ವೃಕ್ಷ && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_ವೃಕ್ಷ_RECURSIVE;
	ಯದಿ (type == OBJ_ವೃಕ್ಷ && recurse && !(options->ls_options & LS_SHOW_TREES))
		ಪ್ರತ್ಯಾಗಮ recurse;
	ಯದಿ (type == OBJ_BLOB && (options->ls_options & LS_ವೃಕ್ಷ_ONLY))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯಾವತ್ (strbuf_expand_step(&sb, &format)) {
		size_t len;

		ಯದಿ (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		ಅನ್ಯಥಾ ಯದಿ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(path)", &format)) {
			ಧ್ರುವ ವರ್ಣ *name;
			ಧ್ರುವ ವರ್ಣ *prefix = options->prefix;
			ಸಂರಚನಾ strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, ರಿಕ್ತ, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} ಅನ್ಯಥಾ
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ವೃಕ್ಷ_common(ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options, ಪೂರ್ಣಾಙ್ಕ *recurse,
			    ಸಂರಚನಾ strbuf *base, ಧ್ರುವ ವರ್ಣ *pathname,
			    ಗಣನಾ object_type type)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	*recurse = 0;

	ಯದಿ (type == OBJ_BLOB) {
		ಯದಿ (options->ls_options & LS_ವೃಕ್ಷ_ONLY)
			ret = 0;
	} ಅನ್ಯಥಾ ಯದಿ (type == OBJ_ವೃಕ್ಷ &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_ವೃಕ್ಷ_RECURSIVE;
		ಯದಿ (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ವೃಕ್ಷ_common_default_long(ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options,
					  ಸಂರಚನಾ strbuf *base,
					  ಧ್ರುವ ವರ್ಣ *pathname,
					  ಧ್ರುವ size_t baselen)
{
	ಧ್ರುವ ವರ್ಣ *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	ಯದಿ (options->null_termination) {
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ಅನ್ಯಥಾ {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ವೃಕ್ಷ_default(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ strbuf *base,
			     ಧ್ರುವ ವರ್ಣ *pathname, unsigned mode,
			     ಶೂನ್ಯ *context)
{
	ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಙ್ಕ early;
	ಪೂರ್ಣಾಙ್ಕ recurse;
	ಗಣನಾ object_type type = object_type(mode);

	early = show_ವೃಕ್ಷ_common(options, &recurse, base, pathname, type);
	ಯದಿ (early >= 0)
		ಪ್ರತ್ಯಾಗಮ early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_ವೃಕ್ಷ_common_default_long(options, base, pathname, base->len);
	ಪ್ರತ್ಯಾಗಮ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ವೃಕ್ಷ_long(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ strbuf *base,
			  ಧ್ರುವ ವರ್ಣ *pathname, unsigned mode,
			  ಶೂನ್ಯ *context)
{
	ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಙ್ಕ early;
	ಪೂರ್ಣಾಙ್ಕ recurse;
	ವರ್ಣ size_text[24];
	ಗಣನಾ object_type type = object_type(mode);

	early = show_ವೃಕ್ಷ_common(options, &recurse, base, pathname, type);
	ಯದಿ (early >= 0)
		ಪ್ರತ್ಯಾಗಮ early;

	ಯದಿ (type == OBJ_BLOB) {
		size_t size;
		ಯದಿ (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, ಪರಿಮಾಣ(size_text), "BAD");
		ಅನ್ಯಥಾ
			xsnprintf(size_text, ಪರಿಮಾಣ(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} ಅನ್ಯಥಾ {
		xsnprintf(size_text, ಪರಿಮಾಣ(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_ವೃಕ್ಷ_common_default_long(options, base, pathname, base->len);
	ಪ್ರತ್ಯಾಗಮ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ವೃಕ್ಷ_name_only(ಧ್ರುವ ಸಂರಚನಾ object_id *oid UNUSED,
			       ಸಂರಚನಾ strbuf *base,
			       ಧ್ರುವ ವರ್ಣ *pathname, unsigned mode,
			       ಶೂನ್ಯ *context)
{
	ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಙ್ಕ early;
	ಪೂರ್ಣಾಙ್ಕ recurse;
	ಧ್ರುವ size_t baselen = base->len;
	ಗಣನಾ object_type type = object_type(mode);
	ಧ್ರುವ ವರ್ಣ *prefix;

	early = show_ವೃಕ್ಷ_common(options, &recurse, base, pathname, type);
	ಯದಿ (early >= 0)
		ಪ್ರತ್ಯಾಗಮ early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	ಯದಿ (options->null_termination) {
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ಅನ್ಯಥಾ {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	ಪ್ರತ್ಯಾಗಮ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ವೃಕ್ಷ_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ strbuf *base,
			    ಧ್ರುವ ವರ್ಣ *pathname, unsigned mode,
			    ಶೂನ್ಯ *context)
{
	ಸಂರಚನಾ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಙ್ಕ early;
	ಪೂರ್ಣಾಙ್ಕ recurse;
	ಗಣನಾ object_type type = object_type(mode);
	ಧ್ರುವ ವರ್ಣ *str;

	early = show_ವೃಕ್ಷ_common(options, &recurse, base, pathname, type);
	ಯದಿ (early >= 0)
		ಪ್ರತ್ಯಾಗಮ early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	ಯದಿ (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} ಅನ್ಯಥಾ  {
		puts(str);
	}
	ಪ್ರತ್ಯಾಗಮ recurse;
}

ಗಣನಾ ls_ವೃಕ್ಷ_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

ಸಂರಚನಾ ls_ವೃಕ್ಷ_cmdmode_to_fmt {
	ಗಣನಾ ls_ವೃಕ್ಷ_cmdmode mode;
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ fmt;
	read_ವೃಕ್ಷ_fn_t fn;
};

ಸ್ಥಿರ ಸಂರಚನಾ ls_ವೃಕ್ಷ_cmdmode_to_fmt ls_ವೃಕ್ಷ_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_ವೃಕ್ಷ_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_ವೃಕ್ಷ_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_ವೃಕ್ಷ_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_ವೃಕ್ಷ_object
	},
	{
		/* fallback */
		.fn = show_ವೃಕ್ಷ_default,
	},
};

ಪೂರ್ಣಾಙ್ಕ cmd_ls_ವೃಕ್ಷ(ಪೂರ್ಣಾಙ್ಕ argc,
		ಧ್ರುವ ವರ್ಣ **argv,
		ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಪೂರ್ಣಾಙ್ಕ i, full_ವೃಕ್ಷ = 0;
	ಪೂರ್ಣಾಙ್ಕ full_name = !prefix || !*prefix;
	read_ವೃಕ್ಷ_fn_t fn = ರಿಕ್ತ;
	ಗಣನಾ ls_ವೃಕ್ಷ_cmdmode cmdmode = MODE_DEFAULT;
	ಪೂರ್ಣಾಙ್ಕ null_termination = 0;
	ಸಂರಚನಾ ls_ವೃಕ್ಷ_options options = { 0 };
	ಧ್ರುವ ಸಂರಚನಾ option ls_ವೃಕ್ಷ_options[] = {
		OPT_BIT('d', ರಿಕ್ತ, &options.ls_options, N_("only show trees"),
			LS_ವೃಕ್ಷ_ONLY),
		OPT_BIT('r', ರಿಕ್ತ, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', ರಿಕ್ತ, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ರಿಕ್ತ, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_ವೃಕ್ಷ,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	ಸಂರಚನಾ ls_ವೃಕ್ಷ_cmdmode_to_fmt *m2f = ls_ವೃಕ್ಷ_cmdmode_format;
	ಪೂರ್ಣಾಙ್ಕ ret;

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, ls_ವೃಕ್ಷ_options,
			     ls_ವೃಕ್ಷ_usage, 0);
	options.null_termination = null_termination;

	ಯದಿ (full_ವೃಕ್ಷ)
		prefix = ರಿಕ್ತ;
	options.prefix = full_name ? ರಿಕ್ತ : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	ಯದಿ (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	ಯದಿ ( (LS_ವೃಕ್ಷ_ONLY|LS_RECURSIVE) ==
	    ((LS_ವೃಕ್ಷ_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	ಯದಿ (options.format && cmdmode)
		usage_ಸಂದೇಶ_opt(
			_("--format can't be combined with other format-altering options"),
			ls_ವೃಕ್ಷ_usage, ls_ವೃಕ್ಷ_options);
	ಯದಿ (argc < 1)
		usage_with_options(ls_ವೃಕ್ಷ_usage, ls_ವೃಕ್ಷ_options);
	ಯದಿ (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	ಪ್ರತಿ (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.ವಸ್ತುಗಳು[i].nowildcard_len = options.pathspec.ವಸ್ತುಗಳು[i].len;
	options.pathspec.has_wildcard = 0;
	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, &oid);
	ಯದಿ (!ವೃಕ್ಷ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	ಯಾವತ್ (m2f) {
		ಯದಿ (!m2f->fmt) {
			fn = options.format ? show_ವೃಕ್ಷ_fmt : show_ವೃಕ್ಷ_default;
		} ಅನ್ಯಥಾ ಯದಿ (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} ಅನ್ಯಥಾ ಯದಿ (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} ಅನ್ಯಥಾ {
			m2f++;
			ಅನುವರ್ತ;
		}
		ಭಙ್ಗ;
	}

	ret = !!read_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ಪ್ರತ್ಯಾಗಮ ret;
}
