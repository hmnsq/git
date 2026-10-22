/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাৱ "builtin.h"

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "tree.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "pathspec.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ ls_বৃক্ষ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	রিক্ত
};

স্থির শূন্য expand_objectsize(সংরচনা strbuf *সাৰি, ধ্রুৱ সংরচনা object_id *oid,
			      ধ্রুৱ গণনা object_type type, unsigned পূর্ণাঙ্ক padded)
{
	স্থির ধ্রুৱ ৱর্ণ padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = সাৰি->len;
	size_t len;

	যদি (type == OBJ_BLOB) {
		size_t size;
		যদি (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(সাৰি, size);
	} অন্যথা {
		strbuf_addstr(সাৰি, "-");
	}
	len = সাৰি->len - orig_len;
	যদি (len < min_len)
		strbuf_ঢোকাও(সাৰি, orig_len, padding, min_len - len);
}

সংরচনা ls_বৃক্ষ_options {
	unsigned null_termination:1;
	পূর্ণাঙ্ক abbrev;
	গণনা ls_বৃক্ষ_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_বৃক্ষ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	সংরচনা pathspec pathspec;
	ধ্রুৱ ৱর্ণ *prefix;
	ধ্রুৱ ৱর্ণ *format;
};

স্থির পূর্ণাঙ্ক show_recursive(সংরচনা ls_বৃক্ষ_options *options, ধ্রুৱ ৱর্ণ *base,
			  size_t baselen, ধ্রুৱ ৱর্ণ *pathname)
{
	পূর্ণাঙ্ক i;

	যদি (options->ls_options & LS_RECURSIVE)
		ঘূৰাই 1;

	যদি (!options->pathspec.nr)
		ঘূৰাই 0;

	বাবে (i = 0; i < options->pathspec.nr; i++) {
		ধ্রুৱ ৱর্ণ *spec = options->pathspec.বস্তুসমূহ[i].match;
		size_t len, speclen;

		যদি (strncmp(base, spec, baselen))
			অনুৱর্ত;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		যদি (speclen <= len)
			অনুৱর্ত;
		যদি (spec[len] && spec[len] != '/')
			অনুৱর্ত;
		যদি (memcmp(pathname, spec, len))
			অনুৱর্ত;
		ঘূৰাই 1;
	}
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক show_বৃক্ষ_fmt(ধ্রুৱ সংরচনা object_id *oid, সংরচনা strbuf *base,
			 ধ্রুৱ ৱর্ণ *pathname, unsigned mode, শূন্য *context)
{
	সংরচনা ls_বৃক্ষ_options *options = context;
	পূর্ণাঙ্ক recurse = 0;
	সংরচনা strbuf sb = STRBUF_INIT;
	গণনা object_type type = object_type(mode);
	ধ্রুৱ ৱর্ণ *format = options->format;

	যদি (type == OBJ_বৃক্ষ && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_বৃক্ষ_RECURSIVE;
	যদি (type == OBJ_বৃক্ষ && recurse && !(options->ls_options & LS_SHOW_TREES))
		ঘূৰাই recurse;
	যদি (type == OBJ_BLOB && (options->ls_options & LS_বৃক্ষ_ONLY))
		ঘূৰাই 0;

	যেতিয়ালৈ (strbuf_expand_step(&sb, &format)) {
		size_t len;

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
			ধ্রুৱ ৱর্ণ *name;
			ধ্রুৱ ৱর্ণ *prefix = options->prefix;
			সংরচনা strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, রিক্ত, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} অন্যথা
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ঘূৰাই recurse;
}

স্থির পূর্ণাঙ্ক show_বৃক্ষ_common(সংরচনা ls_বৃক্ষ_options *options, পূর্ণাঙ্ক *recurse,
			    সংরচনা strbuf *base, ধ্রুৱ ৱর্ণ *pathname,
			    গণনা object_type type)
{
	পূর্ণাঙ্ক ret = -1;
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

	ঘূৰাই ret;
}

স্থির শূন্য show_বৃক্ষ_common_default_long(সংরচনা ls_বৃক্ষ_options *options,
					  সংরচনা strbuf *base,
					  ধ্রুৱ ৱর্ণ *pathname,
					  ধ্রুৱ size_t baselen)
{
	ধ্রুৱ ৱর্ণ *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	যদি (options->null_termination) {
		সংরচনা strbuf sb = STRBUF_INIT;
		ধ্রুৱ ৱর্ণ *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} অন্যথা {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

স্থির পূর্ণাঙ্ক show_বৃক্ষ_default(ধ্রুৱ সংরচনা object_id *oid, সংরচনা strbuf *base,
			     ধ্রুৱ ৱর্ণ *pathname, unsigned mode,
			     শূন্য *context)
{
	সংরচনা ls_বৃক্ষ_options *options = context;
	পূর্ণাঙ্ক early;
	পূর্ণাঙ্ক recurse;
	গণনা object_type type = object_type(mode);

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ঘূৰাই early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_বৃক্ষ_common_default_long(options, base, pathname, base->len);
	ঘূৰাই recurse;
}

স্থির পূর্ণাঙ্ক show_বৃক্ষ_long(ধ্রুৱ সংরচনা object_id *oid, সংরচনা strbuf *base,
			  ধ্রুৱ ৱর্ণ *pathname, unsigned mode,
			  শূন্য *context)
{
	সংরচনা ls_বৃক্ষ_options *options = context;
	পূর্ণাঙ্ক early;
	পূর্ণাঙ্ক recurse;
	ৱর্ণ size_text[24];
	গণনা object_type type = object_type(mode);

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ঘূৰাই early;

	যদি (type == OBJ_BLOB) {
		size_t size;
		যদি (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, পরিমাণ(size_text), "BAD");
		অন্যথা
			xsnprintf(size_text, পরিমাণ(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} অন্যথা {
		xsnprintf(size_text, পরিমাণ(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_বৃক্ষ_common_default_long(options, base, pathname, base->len);
	ঘূৰাই recurse;
}

স্থির পূর্ণাঙ্ক show_বৃক্ষ_name_only(ধ্রুৱ সংরচনা object_id *oid UNUSED,
			       সংরচনা strbuf *base,
			       ধ্রুৱ ৱর্ণ *pathname, unsigned mode,
			       শূন্য *context)
{
	সংরচনা ls_বৃক্ষ_options *options = context;
	পূর্ণাঙ্ক early;
	পূর্ণাঙ্ক recurse;
	ধ্রুৱ size_t baselen = base->len;
	গণনা object_type type = object_type(mode);
	ধ্রুৱ ৱর্ণ *prefix;

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ঘূৰাই early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	যদি (options->null_termination) {
		সংরচনা strbuf sb = STRBUF_INIT;
		ধ্রুৱ ৱর্ণ *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} অন্যথা {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	ঘূৰাই recurse;
}

স্থির পূর্ণাঙ্ক show_বৃক্ষ_object(ধ্রুৱ সংরচনা object_id *oid, সংরচনা strbuf *base,
			    ধ্রুৱ ৱর্ণ *pathname, unsigned mode,
			    শূন্য *context)
{
	সংরচনা ls_বৃক্ষ_options *options = context;
	পূর্ণাঙ্ক early;
	পূর্ণাঙ্ক recurse;
	গণনা object_type type = object_type(mode);
	ধ্রুৱ ৱর্ণ *str;

	early = show_বৃক্ষ_common(options, &recurse, base, pathname, type);
	যদি (early >= 0)
		ঘূৰাই early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	যদি (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} অন্যথা  {
		puts(str);
	}
	ঘূৰাই recurse;
}

গণনা ls_বৃক্ষ_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

সংরচনা ls_বৃক্ষ_cmdmode_to_fmt {
	গণনা ls_বৃক্ষ_cmdmode mode;
	ধ্রুৱ ৱর্ণ *ধ্রুৱ fmt;
	read_বৃক্ষ_fn_t fn;
};

স্থির সংরচনা ls_বৃক্ষ_cmdmode_to_fmt ls_বৃক্ষ_cmdmode_format[] = {
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

পূর্ণাঙ্ক cmd_ls_বৃক্ষ(পূর্ণাঙ্ক argc,
		ধ্রুৱ ৱর্ণ **argv,
		ধ্রুৱ ৱর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	সংরচনা object_id oid;
	সংরচনা বৃক্ষ *বৃক্ষ;
	পূর্ণাঙ্ক i, full_বৃক্ষ = 0;
	পূর্ণাঙ্ক full_name = !prefix || !*prefix;
	read_বৃক্ষ_fn_t fn = রিক্ত;
	গণনা ls_বৃক্ষ_cmdmode cmdmode = MODE_DEFAULT;
	পূর্ণাঙ্ক null_termination = 0;
	সংরচনা ls_বৃক্ষ_options options = { 0 };
	ধ্রুৱ সংরচনা option ls_বৃক্ষ_options[] = {
		OPT_BIT('d', রিক্ত, &options.ls_options, N_("only show trees"),
			LS_বৃক্ষ_ONLY),
		OPT_BIT('r', রিক্ত, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', রিক্ত, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', রিক্ত, &null_termination,
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
	সংরচনা ls_বৃক্ষ_cmdmode_to_fmt *m2f = ls_বৃক্ষ_cmdmode_format;
	পূর্ণাঙ্ক ret;

	repo_config(the_repository, git_default_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, ls_বৃক্ষ_options,
			     ls_বৃক্ষ_usage, 0);
	options.null_termination = null_termination;

	যদি (full_বৃক্ষ)
		prefix = রিক্ত;
	options.prefix = full_name ? রিক্ত : prefix;

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
		usage_বাৰ্তা_opt(
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
	বাবে (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.বস্তুসমূহ[i].nowildcard_len = options.pathspec.বস্তুসমূহ[i].len;
	options.pathspec.has_wildcard = 0;
	বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, &oid);
	যদি (!বৃক্ষ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	যেতিয়ালৈ (m2f) {
		যদি (!m2f->fmt) {
			fn = options.format ? show_বৃক্ষ_fmt : show_বৃক্ষ_default;
		} অন্যথা যদি (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} অন্যথা যদি (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} অন্যথা {
			m2f++;
			অনুৱর্ত;
		}
		ভঙ্গ;
	}

	ret = !!read_বৃক্ষ(the_repository, বৃক্ষ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ঘূৰাই ret;
}
