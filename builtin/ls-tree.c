/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"

#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-name.h"
#include "odb.h"
#include "tree.h"
#include "path.h"
#include "quote.h"
#include "parse-options.h"
#include "pathspec.h"

static const var_na * const ls_tree_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	NULL
};

static void expand_objectsize(sa_nracanaa strbuf *line, const sa_nracanaa object_id *oid,
			      const ga_nanaa object_type type, unsigned puur_naa_nka padded)
{
	static const var_na padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = line->len;
	size_t len;

	ᮚᮦᮔ᮪ (type == OBJ_BLOB) {
		size_t size;
		ᮚᮦᮔ᮪ (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(line, size);
	} ᮜᮤᮚ {
		strbuf_addstr(line, "-");
	}
	len = line->len - orig_len;
	ᮚᮦᮔ᮪ (len < min_len)
		strbuf_insert(line, orig_len, padding, min_len - len);
}

sa_nracanaa ls_tree_options {
	unsigned null_termination:1;
	puur_naa_nka abbrev;
	ga_nanaa ls_tree_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_TREE_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	sa_nracanaa pathspec pathspec;
	const var_na *prefix;
	const var_na *format;
};

static puur_naa_nka show_recursive(sa_nracanaa ls_tree_options *options, const var_na *base,
			  size_t baselen, const var_na *pathname)
{
	puur_naa_nka i;

	ᮚᮦᮔ᮪ (options->ls_options & LS_RECURSIVE)
		ᮝᮜᮤ 1;

	ᮚᮦᮔ᮪ (!options->pathspec.nr)
		ᮝᮜᮤ 0;

	for (i = 0; i < options->pathspec.nr; i++) {
		const var_na *spec = options->pathspec.items[i].match;
		size_t len, speclen;

		ᮚᮦᮔ᮪ (strncmp(base, spec, baselen))
			continue;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		ᮚᮦᮔ᮪ (speclen <= len)
			continue;
		ᮚᮦᮔ᮪ (spec[len] && spec[len] != '/')
			continue;
		ᮚᮦᮔ᮪ (memcmp(pathname, spec, len))
			continue;
		ᮝᮜᮤ 1;
	}
	ᮝᮜᮤ 0;
}

static puur_naa_nka show_tree_fmt(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			 const var_na *pathname, unsigned mode, void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	puur_naa_nka recurse = 0;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ga_nanaa object_type type = object_type(mode);
	const var_na *format = options->format;

	ᮚᮦᮔ᮪ (type == OBJ_TREE && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_TREE_RECURSIVE;
	ᮚᮦᮔ᮪ (type == OBJ_TREE && recurse && !(options->ls_options & LS_SHOW_TREES))
		ᮝᮜᮤ recurse;
	ᮚᮦᮔ᮪ (type == OBJ_BLOB && (options->ls_options & LS_TREE_ONLY))
		ᮝᮜᮤ 0;

	ᮞᮨᮓᮨᮜ (strbuf_expand_step(&sb, &format)) {
		size_t len;

		ᮚᮦᮔ᮪ (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		ᮜᮤᮚ ᮚᮦᮔ᮪ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ᮜᮤᮚ ᮚᮦᮔ᮪ (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		ᮜᮤᮚ ᮚᮦᮔ᮪ (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		ᮜᮤᮚ ᮚᮦᮔ᮪ (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		ᮜᮤᮚ ᮚᮦᮔ᮪ (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		ᮜᮤᮚ ᮚᮦᮔ᮪ (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		ᮜᮤᮚ ᮚᮦᮔ᮪ (skip_prefix(format, "(path)", &format)) {
			const var_na *name;
			const var_na *prefix = options->prefix;
			sa_nracanaa strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, NULL, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} ᮜᮤᮚ
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ᮝᮜᮤ recurse;
}

static puur_naa_nka show_tree_common(sa_nracanaa ls_tree_options *options, puur_naa_nka *recurse,
			    sa_nracanaa strbuf *base, const var_na *pathname,
			    ga_nanaa object_type type)
{
	puur_naa_nka ret = -1;
	*recurse = 0;

	ᮚᮦᮔ᮪ (type == OBJ_BLOB) {
		ᮚᮦᮔ᮪ (options->ls_options & LS_TREE_ONLY)
			ret = 0;
	} ᮜᮤᮚ ᮚᮦᮔ᮪ (type == OBJ_TREE &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_TREE_RECURSIVE;
		ᮚᮦᮔ᮪ (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	ᮝᮜᮤ ret;
}

static void show_tree_common_default_long(sa_nracanaa ls_tree_options *options,
					  sa_nracanaa strbuf *base,
					  const var_na *pathname,
					  const size_t baselen)
{
	const var_na *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	ᮚᮦᮔ᮪ (options->null_termination) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		const var_na *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ᮜᮤᮚ {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

static puur_naa_nka show_tree_default(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			     const var_na *pathname, unsigned mode,
			     void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	puur_naa_nka early;
	puur_naa_nka recurse;
	ga_nanaa object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	ᮚᮦᮔ᮪ (early >= 0)
		ᮝᮜᮤ early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_tree_common_default_long(options, base, pathname, base->len);
	ᮝᮜᮤ recurse;
}

static puur_naa_nka show_tree_long(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			  const var_na *pathname, unsigned mode,
			  void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	puur_naa_nka early;
	puur_naa_nka recurse;
	var_na size_text[24];
	ga_nanaa object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	ᮚᮦᮔ᮪ (early >= 0)
		ᮝᮜᮤ early;

	ᮚᮦᮔ᮪ (type == OBJ_BLOB) {
		size_t size;
		ᮚᮦᮔ᮪ (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, parimaa_na(size_text), "BAD");
		ᮜᮤᮚ
			xsnprintf(size_text, parimaa_na(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} ᮜᮤᮚ {
		xsnprintf(size_text, parimaa_na(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_tree_common_default_long(options, base, pathname, base->len);
	ᮝᮜᮤ recurse;
}

static puur_naa_nka show_tree_name_only(const sa_nracanaa object_id *oid UNUSED,
			       sa_nracanaa strbuf *base,
			       const var_na *pathname, unsigned mode,
			       void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	puur_naa_nka early;
	puur_naa_nka recurse;
	const size_t baselen = base->len;
	ga_nanaa object_type type = object_type(mode);
	const var_na *prefix;

	early = show_tree_common(options, &recurse, base, pathname, type);
	ᮚᮦᮔ᮪ (early >= 0)
		ᮝᮜᮤ early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	ᮚᮦᮔ᮪ (options->null_termination) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		const var_na *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ᮜᮤᮚ {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	ᮝᮜᮤ recurse;
}

static puur_naa_nka show_tree_object(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			    const var_na *pathname, unsigned mode,
			    void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	puur_naa_nka early;
	puur_naa_nka recurse;
	ga_nanaa object_type type = object_type(mode);
	const var_na *str;

	early = show_tree_common(options, &recurse, base, pathname, type);
	ᮚᮦᮔ᮪ (early >= 0)
		ᮝᮜᮤ early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	ᮚᮦᮔ᮪ (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} ᮜᮤᮚ  {
		puts(str);
	}
	ᮝᮜᮤ recurse;
}

ga_nanaa ls_tree_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

sa_nracanaa ls_tree_cmdmode_to_fmt {
	ga_nanaa ls_tree_cmdmode mode;
	const var_na *const fmt;
	read_tree_fn_t fn;
};

static sa_nracanaa ls_tree_cmdmode_to_fmt ls_tree_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_tree_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_tree_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_tree_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_tree_object
	},
	{
		/* fallback */
		.fn = show_tree_default,
	},
};

puur_naa_nka cmd_ls_tree(puur_naa_nka argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa object_id oid;
	sa_nracanaa tree *tree;
	puur_naa_nka i, full_tree = 0;
	puur_naa_nka full_name = !prefix || !*prefix;
	read_tree_fn_t fn = NULL;
	ga_nanaa ls_tree_cmdmode cmdmode = MODE_DEFAULT;
	puur_naa_nka null_termination = 0;
	sa_nracanaa ls_tree_options options = { 0 };
	const sa_nracanaa option ls_tree_options[] = {
		OPT_BIT('d', NULL, &options.ls_options, N_("only show trees"),
			LS_TREE_ONLY),
		OPT_BIT('r', NULL, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', NULL, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', NULL, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_tree,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	sa_nracanaa ls_tree_cmdmode_to_fmt *m2f = ls_tree_cmdmode_format;
	puur_naa_nka ret;

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, ls_tree_options,
			     ls_tree_usage, 0);
	options.null_termination = null_termination;

	ᮚᮦᮔ᮪ (full_tree)
		prefix = NULL;
	options.prefix = full_name ? NULL : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	ᮚᮦᮔ᮪ (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	ᮚᮦᮔ᮪ ( (LS_TREE_ONLY|LS_RECURSIVE) ==
	    ((LS_TREE_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	ᮚᮦᮔ᮪ (options.format && cmdmode)
		usage_msg_opt(
			_("--format can't be combined with other format-altering options"),
			ls_tree_usage, ls_tree_options);
	ᮚᮦᮔ᮪ (argc < 1)
		usage_with_options(ls_tree_usage, ls_tree_options);
	ᮚᮦᮔ᮪ (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	for (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.items[i].nowildcard_len = options.pathspec.items[i].len;
	options.pathspec.has_wildcard = 0;
	tree = repo_parse_tree_indirect(the_repository, &oid);
	ᮚᮦᮔ᮪ (!tree)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	ᮞᮨᮓᮨᮜ (m2f) {
		ᮚᮦᮔ᮪ (!m2f->fmt) {
			fn = options.format ? show_tree_fmt : show_tree_default;
		} ᮜᮤᮚ ᮚᮦᮔ᮪ (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} ᮜᮤᮚ ᮚᮦᮔ᮪ (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} ᮜᮤᮚ {
			m2f++;
			continue;
		}
		bha_nga;
	}

	ret = !!read_tree(the_repository, tree, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ᮝᮜᮤ ret;
}
