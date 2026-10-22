/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"

#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "pathspec.h"

நிலையான மாறிலி எழுத்து * மாறிலி ls_மரம்_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	வெற்று
};

நிலையான வெறுமை expand_objectsize(கட்டமைப்பு strbuf *வரி, மாறிலி கட்டமைப்பு object_id *oid,
			      மாறிலி எண்ணிக்கை object_type type, unsigned முழு padded)
{
	நிலையான மாறிலி எழுத்து padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = வரி->len;
	size_t len;

	எனில் (type == OBJ_BLOB) {
		size_t size;
		எனில் (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(வரி, size);
	} இல்லையெனில் {
		strbuf_addstr(வரி, "-");
	}
	len = வரி->len - orig_len;
	எனில் (len < min_len)
		strbuf_செருகு(வரி, orig_len, padding, min_len - len);
}

கட்டமைப்பு ls_மரம்_options {
	unsigned null_termination:1;
	முழு abbrev;
	எண்ணிக்கை ls_மரம்_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_மரம்_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	கட்டமைப்பு pathspec pathspec;
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *format;
};

நிலையான முழு show_recursive(கட்டமைப்பு ls_மரம்_options *options, மாறிலி எழுத்து *base,
			  size_t baselen, மாறிலி எழுத்து *pathname)
{
	முழு i;

	எனில் (options->ls_options & LS_RECURSIVE)
		திரும்பு 1;

	எனில் (!options->pathspec.nr)
		திரும்பு 0;

	க்கு (i = 0; i < options->pathspec.nr; i++) {
		மாறிலி எழுத்து *spec = options->pathspec.பொருட்கள்[i].match;
		size_t len, speclen;

		எனில் (strncmp(base, spec, baselen))
			தொடர்;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		எனில் (speclen <= len)
			தொடர்;
		எனில் (spec[len] && spec[len] != '/')
			தொடர்;
		எனில் (memcmp(pathname, spec, len))
			தொடர்;
		திரும்பு 1;
	}
	திரும்பு 0;
}

நிலையான முழு show_மரம்_fmt(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *base,
			 மாறிலி எழுத்து *pathname, unsigned mode, வெறுமை *context)
{
	கட்டமைப்பு ls_மரம்_options *options = context;
	முழு recurse = 0;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	எண்ணிக்கை object_type type = object_type(mode);
	மாறிலி எழுத்து *format = options->format;

	எனில் (type == OBJ_மரம் && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_மரம்_RECURSIVE;
	எனில் (type == OBJ_மரம் && recurse && !(options->ls_options & LS_SHOW_TREES))
		திரும்பு recurse;
	எனில் (type == OBJ_BLOB && (options->ls_options & LS_மரம்_ONLY))
		திரும்பு 0;

	போது (strbuf_expand_step(&sb, &format)) {
		size_t len;

		எனில் (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		இல்லையெனில் எனில் ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		இல்லையெனில் எனில் (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		இல்லையெனில் எனில் (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		இல்லையெனில் எனில் (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		இல்லையெனில் எனில் (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		இல்லையெனில் எனில் (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		இல்லையெனில் எனில் (skip_prefix(format, "(path)", &format)) {
			மாறிலி எழுத்து *name;
			மாறிலி எழுத்து *prefix = options->prefix;
			கட்டமைப்பு strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, வெற்று, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} இல்லையெனில்
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	திரும்பு recurse;
}

நிலையான முழு show_மரம்_common(கட்டமைப்பு ls_மரம்_options *options, முழு *recurse,
			    கட்டமைப்பு strbuf *base, மாறிலி எழுத்து *pathname,
			    எண்ணிக்கை object_type type)
{
	முழு ret = -1;
	*recurse = 0;

	எனில் (type == OBJ_BLOB) {
		எனில் (options->ls_options & LS_மரம்_ONLY)
			ret = 0;
	} இல்லையெனில் எனில் (type == OBJ_மரம் &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_மரம்_RECURSIVE;
		எனில் (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	திரும்பு ret;
}

நிலையான வெறுமை show_மரம்_common_default_long(கட்டமைப்பு ls_மரம்_options *options,
					  கட்டமைப்பு strbuf *base,
					  மாறிலி எழுத்து *pathname,
					  மாறிலி size_t baselen)
{
	மாறிலி எழுத்து *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	எனில் (options->null_termination) {
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		மாறிலி எழுத்து *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} இல்லையெனில் {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

நிலையான முழு show_மரம்_default(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *base,
			     மாறிலி எழுத்து *pathname, unsigned mode,
			     வெறுமை *context)
{
	கட்டமைப்பு ls_மரம்_options *options = context;
	முழு early;
	முழு recurse;
	எண்ணிக்கை object_type type = object_type(mode);

	early = show_மரம்_common(options, &recurse, base, pathname, type);
	எனில் (early >= 0)
		திரும்பு early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_மரம்_common_default_long(options, base, pathname, base->len);
	திரும்பு recurse;
}

நிலையான முழு show_மரம்_long(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *base,
			  மாறிலி எழுத்து *pathname, unsigned mode,
			  வெறுமை *context)
{
	கட்டமைப்பு ls_மரம்_options *options = context;
	முழு early;
	முழு recurse;
	எழுத்து size_text[24];
	எண்ணிக்கை object_type type = object_type(mode);

	early = show_மரம்_common(options, &recurse, base, pathname, type);
	எனில் (early >= 0)
		திரும்பு early;

	எனில் (type == OBJ_BLOB) {
		size_t size;
		எனில் (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, அளவு(size_text), "BAD");
		இல்லையெனில்
			xsnprintf(size_text, அளவு(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} இல்லையெனில் {
		xsnprintf(size_text, அளவு(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_மரம்_common_default_long(options, base, pathname, base->len);
	திரும்பு recurse;
}

நிலையான முழு show_மரம்_name_only(மாறிலி கட்டமைப்பு object_id *oid UNUSED,
			       கட்டமைப்பு strbuf *base,
			       மாறிலி எழுத்து *pathname, unsigned mode,
			       வெறுமை *context)
{
	கட்டமைப்பு ls_மரம்_options *options = context;
	முழு early;
	முழு recurse;
	மாறிலி size_t baselen = base->len;
	எண்ணிக்கை object_type type = object_type(mode);
	மாறிலி எழுத்து *prefix;

	early = show_மரம்_common(options, &recurse, base, pathname, type);
	எனில் (early >= 0)
		திரும்பு early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	எனில் (options->null_termination) {
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		மாறிலி எழுத்து *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} இல்லையெனில் {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	திரும்பு recurse;
}

நிலையான முழு show_மரம்_object(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *base,
			    மாறிலி எழுத்து *pathname, unsigned mode,
			    வெறுமை *context)
{
	கட்டமைப்பு ls_மரம்_options *options = context;
	முழு early;
	முழு recurse;
	எண்ணிக்கை object_type type = object_type(mode);
	மாறிலி எழுத்து *str;

	early = show_மரம்_common(options, &recurse, base, pathname, type);
	எனில் (early >= 0)
		திரும்பு early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	எனில் (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} இல்லையெனில்  {
		puts(str);
	}
	திரும்பு recurse;
}

எண்ணிக்கை ls_மரம்_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

கட்டமைப்பு ls_மரம்_cmdmode_to_fmt {
	எண்ணிக்கை ls_மரம்_cmdmode mode;
	மாறிலி எழுத்து *மாறிலி fmt;
	read_மரம்_fn_t fn;
};

நிலையான கட்டமைப்பு ls_மரம்_cmdmode_to_fmt ls_மரம்_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_மரம்_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_மரம்_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_மரம்_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_மரம்_object
	},
	{
		/* fallback */
		.fn = show_மரம்_default,
	},
};

முழு cmd_ls_மரம்(முழு argc,
		மாறிலி எழுத்து **argv,
		மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு மரம் *மரம்;
	முழு i, full_மரம் = 0;
	முழு full_name = !prefix || !*prefix;
	read_மரம்_fn_t fn = வெற்று;
	எண்ணிக்கை ls_மரம்_cmdmode cmdmode = MODE_DEFAULT;
	முழு null_termination = 0;
	கட்டமைப்பு ls_மரம்_options options = { 0 };
	மாறிலி கட்டமைப்பு option ls_மரம்_options[] = {
		OPT_BIT('d', வெற்று, &options.ls_options, N_("only show trees"),
			LS_மரம்_ONLY),
		OPT_BIT('r', வெற்று, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', வெற்று, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', வெற்று, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_மரம்,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	கட்டமைப்பு ls_மரம்_cmdmode_to_fmt *m2f = ls_மரம்_cmdmode_format;
	முழு ret;

	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, ls_மரம்_options,
			     ls_மரம்_usage, 0);
	options.null_termination = null_termination;

	எனில் (full_மரம்)
		prefix = வெற்று;
	options.prefix = full_name ? வெற்று : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	எனில் (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	எனில் ( (LS_மரம்_ONLY|LS_RECURSIVE) ==
	    ((LS_மரம்_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	எனில் (options.format && cmdmode)
		usage_செய்தி_opt(
			_("--format can't be combined with other format-altering options"),
			ls_மரம்_usage, ls_மரம்_options);
	எனில் (argc < 1)
		usage_with_options(ls_மரம்_usage, ls_மரம்_options);
	எனில் (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	க்கு (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.பொருட்கள்[i].nowildcard_len = options.pathspec.பொருட்கள்[i].len;
	options.pathspec.has_wildcard = 0;
	மரம் = repo_parse_மரம்_indirect(the_repository, &oid);
	எனில் (!மரம்)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	போது (m2f) {
		எனில் (!m2f->fmt) {
			fn = options.format ? show_மரம்_fmt : show_மரம்_default;
		} இல்லையெனில் எனில் (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} இல்லையெனில் எனில் (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} இல்லையெனில் {
			m2f++;
			தொடர்;
		}
		நிறுத்து;
	}

	ret = !!read_மரம்(the_repository, மரம், &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	திரும்பு ret;
}
