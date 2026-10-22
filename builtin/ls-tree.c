/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ ls_ਰੁੱਖ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਖਾਲੀ expand_objectsize(ਬਣਤਰ strbuf *ਸਤਰ, ਅਚਲ ਬਣਤਰ object_id *oid,
			      ਅਚਲ ਗਿਣਤੀ object_type type, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ padded)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ padding[] = "       ";
	ਮਾਪ_t min_len = padded ? ਲੜੀ_ਲੰਬਾਈ(padding) : 0;
	ਮਾਪ_t orig_len = ਸਤਰ->len;
	ਮਾਪ_t len;

	ਜੇ (type == OBJ_BLOB) {
		ਮਾਪ_t ਮਾਪ;
		ਜੇ (odb_read_object_info(the_repository->objects, oid, &ਮਾਪ) < 0)
			die(_("could not get object info about '%s'"),
			    oid_ਮੰਜ਼ਿਲ_hex(oid));
		strbuf_ਜੋੜੋ_uint(ਸਤਰ, ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(ਸਤਰ, "-");
	}
	len = ਸਤਰ->len - orig_len;
	ਜੇ (len < min_len)
		strbuf_ਪਾਓ(ਸਤਰ, orig_len, padding, min_len - len);
}

ਬਣਤਰ ls_ਰੁੱਖ_options {
	ਬਿਨਾਨਿਸ਼ਾਨੀ null_termination:1;
	ਪੂਰਨ abbrev;
	ਗਿਣਤੀ ls_ਰੁੱਖ_ਰਸਤਾ_options {
		LS_RECURSIVE = 1 << 0,
		LS_ਰੁੱਖ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	ਬਣਤਰ pathspec pathspec;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *format;
};

ਸਥਿਰ ਪੂਰਨ show_recursive(ਬਣਤਰ ls_ਰੁੱਖ_options *options, ਅਚਲ ਅੱਖਰ *ਅਧਾਰ,
			  ਮਾਪ_t baselen, ਅਚਲ ਅੱਖਰ *pathname)
{
	ਪੂਰਨ i;

	ਜੇ (options->ls_options & LS_RECURSIVE)
		ਵਾਪਸ 1;

	ਜੇ (!options->pathspec.nr)
		ਵਾਪਸ 0;

	ਲਈ (i = 0; i < options->pathspec.nr; i++) {
		ਅਚਲ ਅੱਖਰ *spec = options->pathspec.ਚੀਜ਼ਾਂ[i].ਮੇਲ;
		ਮਾਪ_t len, speclen;

		ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ਅਧਾਰ, spec, baselen))
			ਜਾਰੀ;
		len = ਲੜੀ_ਲੰਬਾਈ(pathname);
		spec += baselen;
		speclen = ਲੜੀ_ਲੰਬਾਈ(spec);
		ਜੇ (speclen <= len)
			ਜਾਰੀ;
		ਜੇ (spec[len] && spec[len] != '/')
			ਜਾਰੀ;
		ਜੇ (memcmp(pathname, spec, len))
			ਜਾਰੀ;
		ਵਾਪਸ 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_ਰੁੱਖ_fmt(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *ਅਧਾਰ,
			 ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਖਾਲੀ *context)
{
	ਬਣਤਰ ls_ਰੁੱਖ_options *options = context;
	ਪੂਰਨ recurse = 0;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਗਿਣਤੀ object_type type = object_type(ਬਹੁਲਕ);
	ਅਚਲ ਅੱਖਰ *format = options->format;

	ਜੇ (type == OBJ_ਰੁੱਖ && show_recursive(options, ਅਧਾਰ->buf, ਅਧਾਰ->len, pathname))
		recurse = READ_ਰੁੱਖ_RECURSIVE;
	ਜੇ (type == OBJ_ਰੁੱਖ && recurse && !(options->ls_options & LS_SHOW_TREES))
		ਵਾਪਸ recurse;
	ਜੇ (type == OBJ_BLOB && (options->ls_options & LS_ਰੁੱਖ_ONLY))
		ਵਾਪਸ 0;

	ਜਦੋਂ (strbuf_expand_ਕਦਮ(&sb, &format)) {
		ਮਾਪ_t len;

		ਜੇ (skip_ਅਗੇਤਰ(format, "%", &format))
			strbuf_addch(&sb, '%');
		ਨਹੀਂਤਾਂ ਜੇ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ਬਹੁਲਕ);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectname)", &format))
			strbuf_ਜੋੜੋ_unique_abbrev(&sb, oid, options->abbrev);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(path)", &format)) {
			ਅਚਲ ਅੱਖਰ *name;
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = options->ਅਗੇਤਰ;
			ਬਣਤਰ strbuf sbuf = STRBUF_INIT;
			ਮਾਪ_t baselen = ਅਧਾਰ->len;

			strbuf_addstr(ਅਧਾਰ, pathname);
			name = relative_ਰਸਤਾ(ਅਧਾਰ->buf, ਅਗੇਤਰ, &sbuf);
			quote_c_style(name, &sb, ਨਿਰਾ, 0);
			strbuf_setlen(ਅਧਾਰ, baselen);
			strbuf_release(&sbuf);
		} ਨਹੀਂਤਾਂ
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ਵਾਪਸ recurse;
}

ਸਥਿਰ ਪੂਰਨ show_ਰੁੱਖ_common(ਬਣਤਰ ls_ਰੁੱਖ_options *options, ਪੂਰਨ *recurse,
			    ਬਣਤਰ strbuf *ਅਧਾਰ, ਅਚਲ ਅੱਖਰ *pathname,
			    ਗਿਣਤੀ object_type type)
{
	ਪੂਰਨ ret = -1;
	*recurse = 0;

	ਜੇ (type == OBJ_BLOB) {
		ਜੇ (options->ls_options & LS_ਰੁੱਖ_ONLY)
			ret = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (type == OBJ_ਰੁੱਖ &&
		   show_recursive(options, ਅਧਾਰ->buf, ਅਧਾਰ->len, pathname)) {
		*recurse = READ_ਰੁੱਖ_RECURSIVE;
		ਜੇ (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ show_ਰੁੱਖ_common_default_long(ਬਣਤਰ ls_ਰੁੱਖ_options *options,
					  ਬਣਤਰ strbuf *ਅਧਾਰ,
					  ਅਚਲ ਅੱਖਰ *pathname,
					  ਅਚਲ ਮਾਪ_t baselen)
{
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = options->ਅਗੇਤਰ;

	strbuf_addstr(ਅਧਾਰ, pathname);

	ਜੇ (options->null_termination) {
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *name = relative_ਰਸਤਾ(ਅਧਾਰ->buf, ਅਗੇਤਰ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ਨਹੀਂਤਾਂ {
		write_name_quoted_relative(ਅਧਾਰ->buf, ਅਗੇਤਰ, stdout, '\n');
	}

	strbuf_setlen(ਅਧਾਰ, baselen);
}

ਸਥਿਰ ਪੂਰਨ show_ਰੁੱਖ_default(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *ਅਧਾਰ,
			     ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ,
			     ਖਾਲੀ *context)
{
	ਬਣਤਰ ls_ਰੁੱਖ_options *options = context;
	ਪੂਰਨ early;
	ਪੂਰਨ recurse;
	ਗਿਣਤੀ object_type type = object_type(ਬਹੁਲਕ);

	early = show_ਰੁੱਖ_common(options, &recurse, ਅਧਾਰ, pathname, type);
	ਜੇ (early >= 0)
		ਵਾਪਸ early;

	ਲਿਖੋ("%06o %s %s\t", ਬਹੁਲਕ, type_name(object_type(ਬਹੁਲਕ)),
	       repo_ਲੱਭੋ_unique_abbrev(the_repository, oid, options->abbrev));
	show_ਰੁੱਖ_common_default_long(options, ਅਧਾਰ, pathname, ਅਧਾਰ->len);
	ਵਾਪਸ recurse;
}

ਸਥਿਰ ਪੂਰਨ show_ਰੁੱਖ_long(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *ਅਧਾਰ,
			  ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ,
			  ਖਾਲੀ *context)
{
	ਬਣਤਰ ls_ਰੁੱਖ_options *options = context;
	ਪੂਰਨ early;
	ਪੂਰਨ recurse;
	ਅੱਖਰ ਮਾਪ_ਪਾਠ[24];
	ਗਿਣਤੀ object_type type = object_type(ਬਹੁਲਕ);

	early = show_ਰੁੱਖ_common(options, &recurse, ਅਧਾਰ, pathname, type);
	ਜੇ (early >= 0)
		ਵਾਪਸ early;

	ਜੇ (type == OBJ_BLOB) {
		ਮਾਪ_t ਮਾਪ;
		ਜੇ (odb_read_object_info(the_repository->objects, oid, &ਮਾਪ) == OBJ_BAD)
			xsnprintf(ਮਾਪ_ਪਾਠ, ਆਕਾਰ(ਮਾਪ_ਪਾਠ), "BAD");
		ਨਹੀਂਤਾਂ
			xsnprintf(ਮਾਪ_ਪਾਠ, ਆਕਾਰ(ਮਾਪ_ਪਾਠ),
				  "%" PRIuMAX, (uintmax_t)ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		xsnprintf(ਮਾਪ_ਪਾਠ, ਆਕਾਰ(ਮਾਪ_ਪਾਠ), "-");
	}

	ਲਿਖੋ("%06o %s %s %7s\t", ਬਹੁਲਕ, type_name(type),
	       repo_ਲੱਭੋ_unique_abbrev(the_repository, oid, options->abbrev),
	       ਮਾਪ_ਪਾਠ);
	show_ਰੁੱਖ_common_default_long(options, ਅਧਾਰ, pathname, ਅਧਾਰ->len);
	ਵਾਪਸ recurse;
}

ਸਥਿਰ ਪੂਰਨ show_ਰੁੱਖ_name_only(ਅਚਲ ਬਣਤਰ object_id *oid ਅਣਵਰਤਿਆ,
			       ਬਣਤਰ strbuf *ਅਧਾਰ,
			       ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ,
			       ਖਾਲੀ *context)
{
	ਬਣਤਰ ls_ਰੁੱਖ_options *options = context;
	ਪੂਰਨ early;
	ਪੂਰਨ recurse;
	ਅਚਲ ਮਾਪ_t baselen = ਅਧਾਰ->len;
	ਗਿਣਤੀ object_type type = object_type(ਬਹੁਲਕ);
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;

	early = show_ਰੁੱਖ_common(options, &recurse, ਅਧਾਰ, pathname, type);
	ਜੇ (early >= 0)
		ਵਾਪਸ early;

	ਅਗੇਤਰ = options->ਅਗੇਤਰ;
	strbuf_addstr(ਅਧਾਰ, pathname);
	ਜੇ (options->null_termination) {
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *name = relative_ਰਸਤਾ(ਅਧਾਰ->buf, ਅਗੇਤਰ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ਨਹੀਂਤਾਂ {
		write_name_quoted_relative(ਅਧਾਰ->buf, ਅਗੇਤਰ, stdout, '\n');
	}
	strbuf_setlen(ਅਧਾਰ, baselen);
	ਵਾਪਸ recurse;
}

ਸਥਿਰ ਪੂਰਨ show_ਰੁੱਖ_object(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *ਅਧਾਰ,
			    ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ,
			    ਖਾਲੀ *context)
{
	ਬਣਤਰ ls_ਰੁੱਖ_options *options = context;
	ਪੂਰਨ early;
	ਪੂਰਨ recurse;
	ਗਿਣਤੀ object_type type = object_type(ਬਹੁਲਕ);
	ਅਚਲ ਅੱਖਰ *str;

	early = show_ਰੁੱਖ_common(options, &recurse, ਅਧਾਰ, pathname, type);
	ਜੇ (early >= 0)
		ਵਾਪਸ early;

	str = repo_ਲੱਭੋ_unique_abbrev(the_repository, oid, options->abbrev);
	ਜੇ (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} ਨਹੀਂਤਾਂ  {
		puts(str);
	}
	ਵਾਪਸ recurse;
}

ਗਿਣਤੀ ls_ਰੁੱਖ_cmdmode {
	ਬਹੁਲਕ_DEFAULT = 0,
	ਬਹੁਲਕ_LONG,
	ਬਹੁਲਕ_NAME_ONLY,
	ਬਹੁਲਕ_NAME_ਸਥਿਤੀ,
	ਬਹੁਲਕ_OBJECT_ONLY,
};

ਬਣਤਰ ls_ਰੁੱਖ_cmdmode_ਮੰਜ਼ਿਲ_fmt {
	ਗਿਣਤੀ ls_ਰੁੱਖ_cmdmode ਬਹੁਲਕ;
	ਅਚਲ ਅੱਖਰ *ਅਚਲ fmt;
	read_ਰੁੱਖ_fn_t fn;
};

ਸਥਿਰ ਬਣਤਰ ls_ਰੁੱਖ_cmdmode_ਮੰਜ਼ਿਲ_fmt ls_ਰੁੱਖ_cmdmode_format[] = {
	{
		.ਬਹੁਲਕ = ਬਹੁਲਕ_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_ਰੁੱਖ_default,
	},
	{
		.ਬਹੁਲਕ = ਬਹੁਲਕ_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_ਰੁੱਖ_long,
	},
	{
		.ਬਹੁਲਕ = ਬਹੁਲਕ_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_ਰੁੱਖ_name_only,
	},
	{
		.ਬਹੁਲਕ = ਬਹੁਲਕ_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_ਰੁੱਖ_object
	},
	{
		/* fallback */
		.fn = show_ਰੁੱਖ_default,
	},
};

ਪੂਰਨ cmd_ls_ਰੁੱਖ(ਪੂਰਨ argc,
		ਅਚਲ ਅੱਖਰ **argv,
		ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਪੂਰਨ i, ਭਰਿਆ_ਰੁੱਖ = 0;
	ਪੂਰਨ ਭਰਿਆ_name = !ਅਗੇਤਰ || !*ਅਗੇਤਰ;
	read_ਰੁੱਖ_fn_t fn = ਨਿਰਾ;
	ਗਿਣਤੀ ls_ਰੁੱਖ_cmdmode cmdmode = ਬਹੁਲਕ_DEFAULT;
	ਪੂਰਨ null_termination = 0;
	ਬਣਤਰ ls_ਰੁੱਖ_options options = { 0 };
	ਅਚਲ ਬਣਤਰ option ls_ਰੁੱਖ_options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('d', ਨਿਰਾ, &options.ls_options, N_("only show trees"),
			LS_ਰੁੱਖ_ONLY),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('r', ਨਿਰਾ, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('t', ਨਿਰਾ, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ਨਿਰਾ, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    ਬਹੁਲਕ_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    ਬਹੁਲਕ_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    ਬਹੁਲਕ_NAME_ਸਥਿਤੀ),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    ਬਹੁਲਕ_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &ਭਰਿਆ_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &ਭਰਿਆ_ਰੁੱਖ,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ ls_ਰੁੱਖ_cmdmode_ਮੰਜ਼ਿਲ_fmt *m2f = ls_ਰੁੱਖ_cmdmode_format;
	ਪੂਰਨ ret;

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, ls_ਰੁੱਖ_options,
			     ls_ਰੁੱਖ_usage, 0);
	options.null_termination = null_termination;

	ਜੇ (ਭਰਿਆ_ਰੁੱਖ)
		ਅਗੇਤਰ = ਨਿਰਾ;
	options.ਅਗੇਤਰ = ਭਰਿਆ_name ? ਨਿਰਾ : ਅਗੇਤਰ;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	ਜੇ (cmdmode == ਬਹੁਲਕ_NAME_ਸਥਿਤੀ)
		cmdmode = ਬਹੁਲਕ_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	ਜੇ ( (LS_ਰੁੱਖ_ONLY|LS_RECURSIVE) ==
	    ((LS_ਰੁੱਖ_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	ਜੇ (options.format && cmdmode)
		usage_ਸੁਨੇਹਾ_opt(
			_("--format can't be combined with other format-altering options"),
			ls_ਰੁੱਖ_usage, ls_ਰੁੱਖ_options);
	ਜੇ (argc < 1)
		usage_with_options(ls_ਰੁੱਖ_usage, ls_ਰੁੱਖ_options);
	ਜੇ (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
		       ਅਗੇਤਰ, argv + 1);
	ਲਈ (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.ਚੀਜ਼ਾਂ[i].nowildcard_len = options.pathspec.ਚੀਜ਼ਾਂ[i].len;
	options.pathspec.has_wildcard = 0;
	ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, &oid);
	ਜੇ (!ਰੁੱਖ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	ਜਦੋਂ (m2f) {
		ਜੇ (!m2f->fmt) {
			fn = options.format ? show_ਰੁੱਖ_fmt : show_ਰੁੱਖ_default;
		} ਨਹੀਂਤਾਂ ਜੇ (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->ਬਹੁਲਕ;
			fn = m2f->fn;
		} ਨਹੀਂਤਾਂ ਜੇ (!options.format && cmdmode == m2f->ਬਹੁਲਕ) {
			fn = m2f->fn;
		} ਨਹੀਂਤਾਂ {
			m2f++;
			ਜਾਰੀ;
		}
		ਰੋਕੋ;
	}

	ret = !!read_ਰੁੱਖ(the_repository, ਰੁੱਖ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ਵਾਪਸ ret;
}
