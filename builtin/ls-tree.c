/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "pathspec.h"

સ્થિર અચલ અક્ષર * અચલ ls_વૃક્ષ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ખાલી
};

સ્થિર શૂન્ય expand_objectsize(સંરચના strbuf *લીટી, અચલ સંરચના object_id *oid,
			      અચલ ગણના object_type type, unsigned પૂર્ણાંક padded)
{
	સ્થિર અચલ અક્ષર padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = લીટી->len;
	size_t len;

	જો (type == OBJ_BLOB) {
		size_t size;
		જો (odb_read_object_info(the_repository->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(લીટી, size);
	} નહીંતર {
		strbuf_addstr(લીટી, "-");
	}
	len = લીટી->len - orig_len;
	જો (len < min_len)
		strbuf_ઉમેરો(લીટી, orig_len, padding, min_len - len);
}

સંરચના ls_વૃક્ષ_options {
	unsigned null_termination:1;
	પૂર્ણાંક abbrev;
	ગણના ls_વૃક્ષ_path_options {
		LS_RECURSIVE = 1 << 0,
		LS_વૃક્ષ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	સંરચના pathspec pathspec;
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *format;
};

સ્થિર પૂર્ણાંક show_recursive(સંરચના ls_વૃક્ષ_options *options, અચલ અક્ષર *base,
			  size_t baselen, અચલ અક્ષર *pathname)
{
	પૂર્ણાંક i;

	જો (options->ls_options & LS_RECURSIVE)
		પાછા 1;

	જો (!options->pathspec.nr)
		પાછા 0;

	માટે (i = 0; i < options->pathspec.nr; i++) {
		અચલ અક્ષર *spec = options->pathspec.વસ્તુઓ[i].match;
		size_t len, speclen;

		જો (strncmp(base, spec, baselen))
			ચાલુ;
		len = strlen(pathname);
		spec += baselen;
		speclen = strlen(spec);
		જો (speclen <= len)
			ચાલુ;
		જો (spec[len] && spec[len] != '/')
			ચાલુ;
		જો (memcmp(pathname, spec, len))
			ચાલુ;
		પાછા 1;
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક show_વૃક્ષ_fmt(અચલ સંરચના object_id *oid, સંરચના strbuf *base,
			 અચલ અક્ષર *pathname, unsigned mode, શૂન્ય *context)
{
	સંરચના ls_વૃક્ષ_options *options = context;
	પૂર્ણાંક recurse = 0;
	સંરચના strbuf sb = STRBUF_INIT;
	ગણના object_type type = object_type(mode);
	અચલ અક્ષર *format = options->format;

	જો (type == OBJ_વૃક્ષ && show_recursive(options, base->buf, base->len, pathname))
		recurse = READ_વૃક્ષ_RECURSIVE;
	જો (type == OBJ_વૃક્ષ && recurse && !(options->ls_options & LS_SHOW_TREES))
		પાછા recurse;
	જો (type == OBJ_BLOB && (options->ls_options & LS_વૃક્ષ_ONLY))
		પાછા 0;

	જ્યારે (strbuf_expand_step(&sb, &format)) {
		size_t len;

		જો (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		નહીંતર જો ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		નહીંતર જો (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", mode);
		નહીંતર જો (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		નહીંતર જો (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		નહીંતર જો (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		નહીંતર જો (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, oid, options->abbrev);
		નહીંતર જો (skip_prefix(format, "(path)", &format)) {
			અચલ અક્ષર *name;
			અચલ અક્ષર *prefix = options->prefix;
			સંરચના strbuf sbuf = STRBUF_INIT;
			size_t baselen = base->len;

			strbuf_addstr(base, pathname);
			name = relative_path(base->buf, prefix, &sbuf);
			quote_c_style(name, &sb, ખાલી, 0);
			strbuf_setlen(base, baselen);
			strbuf_release(&sbuf);
		} નહીંતર
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	પાછા recurse;
}

સ્થિર પૂર્ણાંક show_વૃક્ષ_common(સંરચના ls_વૃક્ષ_options *options, પૂર્ણાંક *recurse,
			    સંરચના strbuf *base, અચલ અક્ષર *pathname,
			    ગણના object_type type)
{
	પૂર્ણાંક ret = -1;
	*recurse = 0;

	જો (type == OBJ_BLOB) {
		જો (options->ls_options & LS_વૃક્ષ_ONLY)
			ret = 0;
	} નહીંતર જો (type == OBJ_વૃક્ષ &&
		   show_recursive(options, base->buf, base->len, pathname)) {
		*recurse = READ_વૃક્ષ_RECURSIVE;
		જો (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	પાછા ret;
}

સ્થિર શૂન્ય show_વૃક્ષ_common_default_long(સંરચના ls_વૃક્ષ_options *options,
					  સંરચના strbuf *base,
					  અચલ અક્ષર *pathname,
					  અચલ size_t baselen)
{
	અચલ અક્ષર *prefix = options->prefix;

	strbuf_addstr(base, pathname);

	જો (options->null_termination) {
		સંરચના strbuf sb = STRBUF_INIT;
		અચલ અક્ષર *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} નહીંતર {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}

	strbuf_setlen(base, baselen);
}

સ્થિર પૂર્ણાંક show_વૃક્ષ_default(અચલ સંરચના object_id *oid, સંરચના strbuf *base,
			     અચલ અક્ષર *pathname, unsigned mode,
			     શૂન્ય *context)
{
	સંરચના ls_વૃક્ષ_options *options = context;
	પૂર્ણાંક early;
	પૂર્ણાંક recurse;
	ગણના object_type type = object_type(mode);

	early = show_વૃક્ષ_common(options, &recurse, base, pathname, type);
	જો (early >= 0)
		પાછા early;

	printf("%06o %s %s\t", mode, type_name(object_type(mode)),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev));
	show_વૃક્ષ_common_default_long(options, base, pathname, base->len);
	પાછા recurse;
}

સ્થિર પૂર્ણાંક show_વૃક્ષ_long(અચલ સંરચના object_id *oid, સંરચના strbuf *base,
			  અચલ અક્ષર *pathname, unsigned mode,
			  શૂન્ય *context)
{
	સંરચના ls_વૃક્ષ_options *options = context;
	પૂર્ણાંક early;
	પૂર્ણાંક recurse;
	અક્ષર size_text[24];
	ગણના object_type type = object_type(mode);

	early = show_વૃક્ષ_common(options, &recurse, base, pathname, type);
	જો (early >= 0)
		પાછા early;

	જો (type == OBJ_BLOB) {
		size_t size;
		જો (odb_read_object_info(the_repository->objects, oid, &size) == OBJ_BAD)
			xsnprintf(size_text, કદ(size_text), "BAD");
		નહીંતર
			xsnprintf(size_text, કદ(size_text),
				  "%" PRIuMAX, (uintmax_t)size);
	} નહીંતર {
		xsnprintf(size_text, કદ(size_text), "-");
	}

	printf("%06o %s %s %7s\t", mode, type_name(type),
	       repo_find_unique_abbrev(the_repository, oid, options->abbrev),
	       size_text);
	show_વૃક્ષ_common_default_long(options, base, pathname, base->len);
	પાછા recurse;
}

સ્થિર પૂર્ણાંક show_વૃક્ષ_name_only(અચલ સંરચના object_id *oid UNUSED,
			       સંરચના strbuf *base,
			       અચલ અક્ષર *pathname, unsigned mode,
			       શૂન્ય *context)
{
	સંરચના ls_વૃક્ષ_options *options = context;
	પૂર્ણાંક early;
	પૂર્ણાંક recurse;
	અચલ size_t baselen = base->len;
	ગણના object_type type = object_type(mode);
	અચલ અક્ષર *prefix;

	early = show_વૃક્ષ_common(options, &recurse, base, pathname, type);
	જો (early >= 0)
		પાછા early;

	prefix = options->prefix;
	strbuf_addstr(base, pathname);
	જો (options->null_termination) {
		સંરચના strbuf sb = STRBUF_INIT;
		અચલ અક્ષર *name = relative_path(base->buf, prefix, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} નહીંતર {
		write_name_quoted_relative(base->buf, prefix, stdout, '\n');
	}
	strbuf_setlen(base, baselen);
	પાછા recurse;
}

સ્થિર પૂર્ણાંક show_વૃક્ષ_object(અચલ સંરચના object_id *oid, સંરચના strbuf *base,
			    અચલ અક્ષર *pathname, unsigned mode,
			    શૂન્ય *context)
{
	સંરચના ls_વૃક્ષ_options *options = context;
	પૂર્ણાંક early;
	પૂર્ણાંક recurse;
	ગણના object_type type = object_type(mode);
	અચલ અક્ષર *str;

	early = show_વૃક્ષ_common(options, &recurse, base, pathname, type);
	જો (early >= 0)
		પાછા early;

	str = repo_find_unique_abbrev(the_repository, oid, options->abbrev);
	જો (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} નહીંતર  {
		puts(str);
	}
	પાછા recurse;
}

ગણના ls_વૃક્ષ_cmdmode {
	MODE_DEFAULT = 0,
	MODE_LONG,
	MODE_NAME_ONLY,
	MODE_NAME_STATUS,
	MODE_OBJECT_ONLY,
};

સંરચના ls_વૃક્ષ_cmdmode_to_fmt {
	ગણના ls_વૃક્ષ_cmdmode mode;
	અચલ અક્ષર *અચલ fmt;
	read_વૃક્ષ_fn_t fn;
};

સ્થિર સંરચના ls_વૃક્ષ_cmdmode_to_fmt ls_વૃક્ષ_cmdmode_format[] = {
	{
		.mode = MODE_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_વૃક્ષ_default,
	},
	{
		.mode = MODE_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_વૃક્ષ_long,
	},
	{
		.mode = MODE_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_વૃક્ષ_name_only,
	},
	{
		.mode = MODE_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_વૃક્ષ_object
	},
	{
		/* fallback */
		.fn = show_વૃક્ષ_default,
	},
};

પૂર્ણાંક cmd_ls_વૃક્ષ(પૂર્ણાંક argc,
		અચલ અક્ષર **argv,
		અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	સંરચના object_id oid;
	સંરચના વૃક્ષ *વૃક્ષ;
	પૂર્ણાંક i, full_વૃક્ષ = 0;
	પૂર્ણાંક full_name = !prefix || !*prefix;
	read_વૃક્ષ_fn_t fn = ખાલી;
	ગણના ls_વૃક્ષ_cmdmode cmdmode = MODE_DEFAULT;
	પૂર્ણાંક null_termination = 0;
	સંરચના ls_વૃક્ષ_options options = { 0 };
	અચલ સંરચના option ls_વૃક્ષ_options[] = {
		OPT_BIT('d', ખાલી, &options.ls_options, N_("only show trees"),
			LS_વૃક્ષ_ONLY),
		OPT_BIT('r', ખાલી, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_BIT('t', ખાલી, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ખાલી, &null_termination,
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
		OPT_BOOL(0, "full-tree", &full_વૃક્ષ,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_END()
	};
	સંરચના ls_વૃક્ષ_cmdmode_to_fmt *m2f = ls_વૃક્ષ_cmdmode_format;
	પૂર્ણાંક ret;

	repo_config(the_repository, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, ls_વૃક્ષ_options,
			     ls_વૃક્ષ_usage, 0);
	options.null_termination = null_termination;

	જો (full_વૃક્ષ)
		prefix = ખાલી;
	options.prefix = full_name ? ખાલી : prefix;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	જો (cmdmode == MODE_NAME_STATUS)
		cmdmode = MODE_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	જો ( (LS_વૃક્ષ_ONLY|LS_RECURSIVE) ==
	    ((LS_વૃક્ષ_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	જો (options.format && cmdmode)
		usage_સંદેશ_opt(
			_("--format can't be combined with other format-altering options"),
			ls_વૃક્ષ_usage, ls_વૃક્ષ_options);
	જો (argc < 1)
		usage_with_options(ls_વૃક્ષ_usage, ls_વૃક્ષ_options);
	જો (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
	માટે (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.વસ્તુઓ[i].nowildcard_len = options.pathspec.વસ્તુઓ[i].len;
	options.pathspec.has_wildcard = 0;
	વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, &oid);
	જો (!વૃક્ષ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	જ્યારે (m2f) {
		જો (!m2f->fmt) {
			fn = options.format ? show_વૃક્ષ_fmt : show_વૃક્ષ_default;
		} નહીંતર જો (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->mode;
			fn = m2f->fn;
		} નહીંતર જો (!options.format && cmdmode == m2f->mode) {
			fn = m2f->fn;
		} નહીંતર {
			m2f++;
			ચાલુ;
		}
		તોડો;
	}

	ret = !!read_વૃક્ષ(the_repository, વૃક્ષ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	પાછા ret;
}
