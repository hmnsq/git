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
			      const ga_nanaa object_type type, unsigned පූර්ණ_සංඛ්‍යා padded)
{
	static const var_na padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = line->len;
	size_t len;

	නම් (type == OBJ_BLOB) {
		size_t size;
		නම් (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(line, size);
	} නැතහොත් {
		strbuf_addstr(line, "-");
	}
	len = line->len - orig_len;
	නම් (len < min_len)
		strbuf_insert(line, orig_len, padding, min_len - len);
}

sa_nracanaa ls_tree_options {
	unsigned null_termination:1;
	පූර්ණ_සංඛ්‍යා abbrev;
	ga_nanaa ls_tree_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_TREE_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	sa_nracanaa pathspec pathspec;
	const var_na *prefix;
	const var_na *format;
};

static පූර්ණ_සංඛ්‍යා show_recursive(sa_nracanaa ls_tree_options *options, const var_na *base,
			  size_t baselen, const var_na *pathname)
{
	පූර්ණ_සංඛ්‍යා i;

	නම් (options->ls_options & LS_RECURSIVE)
		ආපසු 1;

	නම් (!options->pathspec.nr)
		ආපසු 0;

	සඳහා (i = 0; i < options->pathspec.nr; i++) {
		const var_na *spec = options->pathspec.items[i].match;
		size_t len, speclen;

		නම් (strncmp(base, spec, baselen))
			continue;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		නම් (speclen <= len)
			continue;
		නම් (spec[len] && spec[len] != '/')
			continue;
		නම් (memcmp(pathname, spec, len))
			continue;
		ආපසු 1;
	}
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා show_tree_fmt(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			 const var_na *pathname, unsigned mode, void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	පූර්ණ_සංඛ්‍යා recurse = 0;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ga_nanaa object_type type = object_type(mode);
	const var_na *format = options->format;

	නම් (type == OBJ_TREE && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_TREE_RECURSIVE;
	නම් (type == OBJ_TREE && recurse && !(options->ls_options & LS_SHOW_TREES))
		ආපසු recurse;
	නම් (type == OBJ_BLOB && (options->ls_options & LS_TREE_ONLY))
		ආපසු 0;

	අතරතුර (strbuf_expand_step(&sb, &format)) {
		size_t len;

		නම් (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		නැතහොත් නම් ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		නැතහොත් නම් (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		නැතහොත් නම් (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		නැතහොත් නම් (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		නැතහොත් නම් (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		නැතහොත් නම් (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		නැතහොත් නම් (skip_prefix(format, "(path)", &format)) {
			const var_na *name;
			const var_na *prefix = options->prefix;
			sa_nracanaa strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, NULL, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} නැතහොත්
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ආපසු recurse;
}

static පූර්ණ_සංඛ්‍යා show_tree_common(sa_nracanaa ls_tree_options *options, පූර්ණ_සංඛ්‍යා *recurse,
			    sa_nracanaa strbuf *base, const var_na *pathname,
			    ga_nanaa object_type type)
{
	පූර්ණ_සංඛ්‍යා ret = -1;
	*recurse = 0;

	නම් (type == OBJ_BLOB) {
		නම් (options->ls_options & LS_TREE_ONLY)
			ret = 0;
	} නැතහොත් නම් (type == OBJ_TREE &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_TREE_RECURSIVE;
		නම් (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	ආපසු ret;
}

static void show_tree_common_default_long(sa_nracanaa ls_tree_options *options,
					  sa_nracanaa strbuf *base,
					  const var_na *pathname,
					  const size_t baselen)
{
	const var_na *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	නම් (options->null_termination) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		const var_na *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} නැතහොත් {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

static පූර්ණ_සංඛ්‍යා show_tree_default(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			     const var_na *pathname, unsigned mode,
			     void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	පූර්ණ_සංඛ්‍යා early;
	පූර්ණ_සංඛ්‍යා recurse;
	ga_nanaa object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	නම් (early >= 0)
		ආපසු early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_tree_common_default_long(options, base, pathname, base->len);
	ආපසු recurse;
}

static පූර්ණ_සංඛ්‍යා show_tree_long(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			  const var_na *pathname, unsigned mode,
			  void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	පූර්ණ_සංඛ්‍යා early;
	පූර්ණ_සංඛ්‍යා recurse;
	var_na size_text[24];
	ga_nanaa object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	නම් (early >= 0)
		ආපසු early;

	නම් (type == OBJ_BLOB) {
		size_t size;
		නම් (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, parimaa_na(size_text), "BAD");
		නැතහොත්
			xsnprintf(size_text, parimaa_na(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} නැතහොත් {
		xsnprintf(size_text, parimaa_na(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_tree_common_default_long(options, base, pathname, base->len);
	ආපසු recurse;
}

static පූර්ණ_සංඛ්‍යා show_tree_name_only(const sa_nracanaa object_id *oid UNUSED,
			       sa_nracanaa strbuf *base,
			       const var_na *pathname, unsigned mode,
			       void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	පූර්ණ_සංඛ්‍යා early;
	පූර්ණ_සංඛ්‍යා recurse;
	const size_t baselen = base->len;
	ga_nanaa object_type type = object_type(mode);
	const var_na *prefix;

	early = show_tree_common(options, &recurse, base, pathname, type);
	නම් (early >= 0)
		ආපසු early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	නම් (options->null_termination) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		const var_na *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} නැතහොත් {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	ආපසු recurse;
}

static පූර්ණ_සංඛ්‍යා show_tree_object(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *base,
			    const var_na *pathname, unsigned mode,
			    void *context)
{
	sa_nracanaa ls_tree_options *options = context;
	පූර්ණ_සංඛ්‍යා early;
	පූර්ණ_සංඛ්‍යා recurse;
	ga_nanaa object_type type = object_type(mode);
	const var_na *str;

	early = show_tree_common(options, &recurse, base, pathname, type);
	නම් (early >= 0)
		ආපසු early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	නම් (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} නැතහොත්  {
		puts(str);
	}
	ආපසු recurse;
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

පූර්ණ_සංඛ්‍යා cmd_ls_tree(පූර්ණ_සංඛ්‍යා argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa object_id oid;
	sa_nracanaa tree *tree;
	පූර්ණ_සංඛ්‍යා i, full_tree = 0;
	පූර්ණ_සංඛ්‍යා full_name = !prefix || !*prefix;
	read_tree_fn_t fn = NULL;
	ga_nanaa ls_tree_cmdmode cmdmode = MODE_DEFAULT;
	පූර්ණ_සංඛ්‍යා null_termination = 0;
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
	පූර්ණ_සංඛ්‍යා ret;

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, ls_tree_options,
			     ls_tree_usage, 0);
	options.null_termination = null_termination;

	නම් (full_tree)
		prefix = NULL;
	options.prefix = full_name ? NULL : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	නම් (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	නම් ( (LS_TREE_ONLY|LS_RECURSIVE) ==
	    ((LS_TREE_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	නම් (options.format && cmdmode)
		usage_msg_opt(
			_("--format can't be combined with other format-altering options"),
			ls_tree_usage, ls_tree_options);
	නම් (argc < 1)
		usage_with_options(ls_tree_usage, ls_tree_options);
	නම් (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	සඳහා (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.items[i].nowildcard_len = options.pathspec.items[i].len;
	options.pathspec.has_wildcard = 0;
	tree = repo_parse_tree_indirect(the_repository, &oid);
	නම් (!tree)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	අතරතුර (m2f) {
		නම් (!m2f->fmt) {
			fn = options.format ? show_tree_fmt : show_tree_default;
		} නැතහොත් නම් (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} නැතහොත් නම් (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} නැතහොත් {
			m2f++;
			continue;
		}
		bha_nga;
	}

	ret = !!read_tree(the_repository, tree, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ආපසු ret;
}
