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

static const char * const ls_tree_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	NULL
};

static void expand_objectsize(struct strbuf *line, const struct object_id *oid,
			      const enum object_type type, unsigned 정수 padded)
{
	static const char padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = line->len;
	size_t len;

	만약 (type == OBJ_BLOB) {
		size_t size;
		만약 (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(line, size);
	} 아니면 {
		strbuf_addstr(line, "-");
	}
	len = line->len - orig_len;
	만약 (len < min_len)
		strbuf_insert(line, orig_len, padding, min_len - len);
}

struct ls_tree_options {
	unsigned null_termination:1;
	정수 abbrev;
	enum ls_tree_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_TREE_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	struct pathspec pathspec;
	const char *prefix;
	const char *format;
};

static 정수 show_recursive(struct ls_tree_options *options, const char *base,
			  size_t baselen, const char *pathname)
{
	정수 i;

	만약 (options->ls_options & LS_RECURSIVE)
		반환 1;

	만약 (!options->pathspec.nr)
		반환 0;

	위해 (i = 0; i < options->pathspec.nr; i++) {
		const char *spec = options->pathspec.items[i].match;
		size_t len, speclen;

		만약 (strncmp(base, spec, baselen))
			continue;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		만약 (speclen <= len)
			continue;
		만약 (spec[len] && spec[len] != '/')
			continue;
		만약 (memcmp(pathname, spec, len))
			continue;
		반환 1;
	}
	반환 0;
}

static 정수 show_tree_fmt(const struct object_id *oid, struct strbuf *base,
			 const char *pathname, unsigned mode, void *context)
{
	struct ls_tree_options *options = context;
	정수 recurse = 0;
	struct strbuf sb = STRBUF_INIT;
	enum object_type type = object_type(mode);
	const char *format = options->format;

	만약 (type == OBJ_TREE && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_TREE_RECURSIVE;
	만약 (type == OBJ_TREE && recurse && !(options->ls_options & LS_SHOW_TREES))
		반환 recurse;
	만약 (type == OBJ_BLOB && (options->ls_options & LS_TREE_ONLY))
		반환 0;

	동안 (strbuf_expand_step(&sb, &format)) {
		size_t len;

		만약 (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		아니면 만약 ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		아니면 만약 (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		아니면 만약 (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		아니면 만약 (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		아니면 만약 (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		아니면 만약 (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		아니면 만약 (skip_prefix(format, "(path)", &format)) {
			const char *name;
			const char *prefix = options->prefix;
			struct strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, NULL, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} 아니면
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	반환 recurse;
}

static 정수 show_tree_common(struct ls_tree_options *options, 정수 *recurse,
			    struct strbuf *base, const char *pathname,
			    enum object_type type)
{
	정수 ret = -1;
	*recurse = 0;

	만약 (type == OBJ_BLOB) {
		만약 (options->ls_options & LS_TREE_ONLY)
			ret = 0;
	} 아니면 만약 (type == OBJ_TREE &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_TREE_RECURSIVE;
		만약 (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	반환 ret;
}

static void show_tree_common_default_long(struct ls_tree_options *options,
					  struct strbuf *base,
					  const char *pathname,
					  const size_t baselen)
{
	const char *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	만약 (options->null_termination) {
		struct strbuf sb = STRBUF_INIT;
		const char *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} 아니면 {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

static 정수 show_tree_default(const struct object_id *oid, struct strbuf *base,
			     const char *pathname, unsigned mode,
			     void *context)
{
	struct ls_tree_options *options = context;
	정수 early;
	정수 recurse;
	enum object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	만약 (early >= 0)
		반환 early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_tree_common_default_long(options, base, pathname, base->len);
	반환 recurse;
}

static 정수 show_tree_long(const struct object_id *oid, struct strbuf *base,
			  const char *pathname, unsigned mode,
			  void *context)
{
	struct ls_tree_options *options = context;
	정수 early;
	정수 recurse;
	char size_text[24];
	enum object_type type = object_type(mode);

	early = show_tree_common(options, &recurse, base, pathname, type);
	만약 (early >= 0)
		반환 early;

	만약 (type == OBJ_BLOB) {
		size_t size;
		만약 (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, sizeof(size_text), "BAD");
		아니면
			xsnprintf(size_text, sizeof(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} 아니면 {
		xsnprintf(size_text, sizeof(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_tree_common_default_long(options, base, pathname, base->len);
	반환 recurse;
}

static 정수 show_tree_name_only(const struct object_id *oid UNUSED,
			       struct strbuf *base,
			       const char *pathname, unsigned mode,
			       void *context)
{
	struct ls_tree_options *options = context;
	정수 early;
	정수 recurse;
	const size_t baselen = base->len;
	enum object_type type = object_type(mode);
	const char *prefix;

	early = show_tree_common(options, &recurse, base, pathname, type);
	만약 (early >= 0)
		반환 early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	만약 (options->null_termination) {
		struct strbuf sb = STRBUF_INIT;
		const char *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} 아니면 {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	반환 recurse;
}

static 정수 show_tree_object(const struct object_id *oid, struct strbuf *base,
			    const char *pathname, unsigned mode,
			    void *context)
{
	struct ls_tree_options *options = context;
	정수 early;
	정수 recurse;
	enum object_type type = object_type(mode);
	const char *str;

	early = show_tree_common(options, &recurse, base, pathname, type);
	만약 (early >= 0)
		반환 early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	만약 (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} 아니면  {
		puts(str);
	}
	반환 recurse;
}

enum ls_tree_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

struct ls_tree_cmdmode_to_fmt {
	enum ls_tree_cmdmode mode;
	const char *const fmt;
	read_tree_fn_t fn;
};

static struct ls_tree_cmdmode_to_fmt ls_tree_cmdmode_format[] = {
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

정수 cmd_ls_tree(정수 argc,
		const char **argv,
		const char *prefix,
		struct repository *repo UNUSED)
{
	struct object_id oid;
	struct tree *tree;
	정수 i, full_tree = 0;
	정수 full_name = !prefix || !*prefix;
	read_tree_fn_t fn = NULL;
	enum ls_tree_cmdmode cmdmode = MODE_DEFAULT;
	정수 null_termination = 0;
	struct ls_tree_options options = { 0 };
	const struct option ls_tree_options[] = {
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
	struct ls_tree_cmdmode_to_fmt *m2f = ls_tree_cmdmode_format;
	정수 ret;

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, ls_tree_options,
			     ls_tree_usage, 0);
	options.null_termination = null_termination;

	만약 (full_tree)
		prefix = NULL;
	options.prefix = full_name ? NULL : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	만약 (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	만약 ( (LS_TREE_ONLY|LS_RECURSIVE) ==
	    ((LS_TREE_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	만약 (options.format && cmdmode)
		usage_msg_opt(
			_("--format can't be combined with other format-altering options"),
			ls_tree_usage, ls_tree_options);
	만약 (argc < 1)
		usage_with_options(ls_tree_usage, ls_tree_options);
	만약 (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	위해 (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.items[i].nowildcard_len = options.pathspec.items[i].len;
	options.pathspec.has_wildcard = 0;
	tree = repo_parse_tree_indirect(the_repository, &oid);
	만약 (!tree)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	동안 (m2f) {
		만약 (!m2f->fmt) {
			fn = options.format ? show_tree_fmt : show_tree_default;
		} 아니면 만약 (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} 아니면 만약 (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} 아니면 {
			m2f++;
			continue;
		}
		break;
	}

	ret = !!read_tree(the_repository, tree, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	반환 ret;
}
