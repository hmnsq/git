/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ ls_ବୃକ୍ଷ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ଖାଲି
};

ସ୍ଥିର ଶୂନ୍ୟ expand_objectsize(ସଂରଚନା strbuf *ଧାଡ଼ି, ଅଚଳ ସଂରଚନା object_id *oid,
			      ଅଚଳ ଗଣନା object_type type, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା padded)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = ଧାଡ଼ି->len;
	size_t len;

	ଯଦି (type == OBJ_BLOB) {
		size_t size;
		ଯଦି (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(ଧାଡ଼ି, size);
	} ଅନ୍ୟଥା {
		strbuf_addstr(ଧାଡ଼ି, "-");
	}
	len = ଧାଡ଼ି->len - orig_len;
	ଯଦି (len < min_len)
		strbuf_ଭର୍ତ୍ତି(ଧାଡ଼ି, orig_len, padding, min_len - len);
}

ସଂରଚନା ls_ବୃକ୍ଷ_options {
	unsigned null_termination:1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev;
	ଗଣନା ls_ବୃକ୍ଷ_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_ବୃକ୍ଷ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	ସଂରଚନା pathspec pathspec;
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *format;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_recursive(ସଂରଚନା ls_ବୃକ୍ଷ_options *options, ଅଚଳ ଅକ୍ଷର *base,
			  size_t baselen, ଅଚଳ ଅକ୍ଷର *pathname)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (options->ls_options & LS_RECURSIVE)
		ଫେରନ୍ତୁ 1;

	ଯଦି (!options->pathspec.nr)
		ଫେରନ୍ତୁ 0;

	ପାଇଁ (i = 0; i < options->pathspec.nr; i++) {
		ଅଚଳ ଅକ୍ଷର *spec = options->pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].match;
		size_t len, speclen;

		ଯଦି (strncmp(base, spec, baselen))
			ଜାରି;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		ଯଦି (speclen <= len)
			ଜାରି;
		ଯଦି (spec[len] && spec[len] != '/')
			ଜାରି;
		ଯଦି (memcmp(pathname, spec, len))
			ଜାରି;
		ଫେରନ୍ତୁ 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ବୃକ୍ଷ_fmt(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *base,
			 ଅଚଳ ଅକ୍ଷର *pathname, unsigned mode, ଶୂନ୍ୟ *context)
{
	ସଂରଚନା ls_ବୃକ୍ଷ_options *options = context;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse = 0;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଗଣନା object_type type = object_type(mode);
	ଅଚଳ ଅକ୍ଷର *format = options->format;

	ଯଦି (type == OBJ_ବୃକ୍ଷ && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_ବୃକ୍ଷ_RECURSIVE;
	ଯଦି (type == OBJ_ବୃକ୍ଷ && recurse && !(options->ls_options & LS_SHOW_TREES))
		ଫେରନ୍ତୁ recurse;
	ଯଦି (type == OBJ_BLOB && (options->ls_options & LS_ବୃକ୍ଷ_ONLY))
		ଫେରନ୍ତୁ 0;

	ଯେତେବେଳେ (strbuf_expand_step(&sb, &format)) {
		size_t len;

		ଯଦି (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		ଅନ୍ୟଥା ଯଦି ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(path)", &format)) {
			ଅଚଳ ଅକ୍ଷର *name;
			ଅଚଳ ଅକ୍ଷର *prefix = options->prefix;
			ସଂରଚନା strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, ଖାଲି, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} ଅନ୍ୟଥା
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ଫେରନ୍ତୁ recurse;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ବୃକ୍ଷ_common(ସଂରଚନା ls_ବୃକ୍ଷ_options *options, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *recurse,
			    ସଂରଚନା strbuf *base, ଅଚଳ ଅକ୍ଷର *pathname,
			    ଗଣନା object_type type)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	*recurse = 0;

	ଯଦି (type == OBJ_BLOB) {
		ଯଦି (options->ls_options & LS_ବୃକ୍ଷ_ONLY)
			ret = 0;
	} ଅନ୍ୟଥା ଯଦି (type == OBJ_ବୃକ୍ଷ &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_ବୃକ୍ଷ_RECURSIVE;
		ଯଦି (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ show_ବୃକ୍ଷ_common_default_long(ସଂରଚନା ls_ବୃକ୍ଷ_options *options,
					  ସଂରଚନା strbuf *base,
					  ଅଚଳ ଅକ୍ଷର *pathname,
					  ଅଚଳ size_t baselen)
{
	ଅଚଳ ଅକ୍ଷର *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	ଯଦି (options->null_termination) {
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ଅନ୍ୟଥା {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ବୃକ୍ଷ_default(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *base,
			     ଅଚଳ ଅକ୍ଷର *pathname, unsigned mode,
			     ଶୂନ୍ୟ *context)
{
	ସଂରଚନା ls_ବୃକ୍ଷ_options *options = context;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା early;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse;
	ଗଣନା object_type type = object_type(mode);

	early = show_ବୃକ୍ଷ_common(options, &recurse, base, pathname, type);
	ଯଦି (early >= 0)
		ଫେରନ୍ତୁ early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_ବୃକ୍ଷ_common_default_long(options, base, pathname, base->len);
	ଫେରନ୍ତୁ recurse;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ବୃକ୍ଷ_long(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *base,
			  ଅଚଳ ଅକ୍ଷର *pathname, unsigned mode,
			  ଶୂନ୍ୟ *context)
{
	ସଂରଚନା ls_ବୃକ୍ଷ_options *options = context;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା early;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse;
	ଅକ୍ଷର size_text[24];
	ଗଣନା object_type type = object_type(mode);

	early = show_ବୃକ୍ଷ_common(options, &recurse, base, pathname, type);
	ଯଦି (early >= 0)
		ଫେରନ୍ତୁ early;

	ଯଦି (type == OBJ_BLOB) {
		size_t size;
		ଯଦି (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, ଆକାର(size_text), "BAD");
		ଅନ୍ୟଥା
			xsnprintf(size_text, ଆକାର(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} ଅନ୍ୟଥା {
		xsnprintf(size_text, ଆକାର(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_ବୃକ୍ଷ_common_default_long(options, base, pathname, base->len);
	ଫେରନ୍ତୁ recurse;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ବୃକ୍ଷ_name_only(ଅଚଳ ସଂରଚନା object_id *oid UNUSED,
			       ସଂରଚନା strbuf *base,
			       ଅଚଳ ଅକ୍ଷର *pathname, unsigned mode,
			       ଶୂନ୍ୟ *context)
{
	ସଂରଚନା ls_ବୃକ୍ଷ_options *options = context;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା early;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse;
	ଅଚଳ size_t baselen = base->len;
	ଗଣନା object_type type = object_type(mode);
	ଅଚଳ ଅକ୍ଷର *prefix;

	early = show_ବୃକ୍ଷ_common(options, &recurse, base, pathname, type);
	ଯଦି (early >= 0)
		ଫେରନ୍ତୁ early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	ଯଦି (options->null_termination) {
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ଅନ୍ୟଥା {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	ଫେରନ୍ତୁ recurse;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ବୃକ୍ଷ_object(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *base,
			    ଅଚଳ ଅକ୍ଷର *pathname, unsigned mode,
			    ଶୂନ୍ୟ *context)
{
	ସଂରଚନା ls_ବୃକ୍ଷ_options *options = context;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା early;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse;
	ଗଣନା object_type type = object_type(mode);
	ଅଚଳ ଅକ୍ଷର *str;

	early = show_ବୃକ୍ଷ_common(options, &recurse, base, pathname, type);
	ଯଦି (early >= 0)
		ଫେରନ୍ତୁ early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	ଯଦି (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} ଅନ୍ୟଥା  {
		puts(str);
	}
	ଫେରନ୍ତୁ recurse;
}

ଗଣନା ls_ବୃକ୍ଷ_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

ସଂରଚନା ls_ବୃକ୍ଷ_cmdmode_to_fmt {
	ଗଣନା ls_ବୃକ୍ଷ_cmdmode mode;
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ fmt;
	read_ବୃକ୍ଷ_fn_t fn;
};

ସ୍ଥିର ସଂରଚନା ls_ବୃକ୍ଷ_cmdmode_to_fmt ls_ବୃକ୍ଷ_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_ବୃକ୍ଷ_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_ବୃକ୍ଷ_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_ବୃକ୍ଷ_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_ବୃକ୍ଷ_object
	},
	{
		/* fallback */
		.fn = show_ବୃକ୍ଷ_default,
	},
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_ls_ବୃକ୍ଷ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		ଅଚଳ ଅକ୍ଷର **argv,
		ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା object_id oid;
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, full_ବୃକ୍ଷ = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା full_name = !prefix || !*prefix;
	read_ବୃକ୍ଷ_fn_t fn = ଖାଲି;
	ଗଣନା ls_ବୃକ୍ଷ_cmdmode cmdmode = MODE_DEFAULT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା null_termination = 0;
	ସଂରଚନା ls_ବୃକ୍ଷ_options options = { 0 };
	ଅଚଳ ସଂରଚନା option ls_ବୃକ୍ଷ_options[] = {
		OPT_BIT('d', ଖାଲି, &options.ls_options, N_("only show trees"),
			LS_ବୃକ୍ଷ_ONLY),
		OPT_BIT('r', ଖାଲି, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', ଖାଲି, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ଖାଲି, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_ବୃକ୍ଷ,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	ସଂରଚନା ls_ବୃକ୍ଷ_cmdmode_to_fmt *m2f = ls_ବୃକ୍ଷ_cmdmode_format;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	repo_config(the_repository, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, ls_ବୃକ୍ଷ_options,
			     ls_ବୃକ୍ଷ_usage, 0);
	options.null_termination = null_termination;

	ଯଦି (full_ବୃକ୍ଷ)
		prefix = ଖାଲି;
	options.prefix = full_name ? ଖାଲି : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	ଯଦି (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	ଯଦି ( (LS_ବୃକ୍ଷ_ONLY|LS_RECURSIVE) ==
	    ((LS_ବୃକ୍ଷ_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	ଯଦି (options.format && cmdmode)
		usage_ସନ୍ଦେଶ_opt(
			_("--format can't be combined with other format-altering options"),
			ls_ବୃକ୍ଷ_usage, ls_ବୃକ୍ଷ_options);
	ଯଦି (argc < 1)
		usage_with_options(ls_ବୃକ୍ଷ_usage, ls_ବୃକ୍ଷ_options);
	ଯଦି (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	ପାଇଁ (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].nowildcard_len = options.pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].len;
	options.pathspec.has_wildcard = 0;
	ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, &oid);
	ଯଦି (!ବୃକ୍ଷ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	ଯେତେବେଳେ (m2f) {
		ଯଦି (!m2f->fmt) {
			fn = options.format ? show_ବୃକ୍ଷ_fmt : show_ବୃକ୍ଷ_default;
		} ଅନ୍ୟଥା ଯଦି (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} ଅନ୍ୟଥା ଯଦି (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} ଅନ୍ୟଥା {
			m2f++;
			ଜାରି;
		}
		ଭାଙ୍ଗ;
	}

	ret = !!read_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ଫେରନ୍ତୁ ret;
}
