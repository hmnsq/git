/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ls_ವೃಕ್ಷ_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಶೂನ್ಯ expand_objectsize(ರಚನೆ strbuf *ಸಾಲು, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			      ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ object_type type, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ padded)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ padding[] = "       ";
	ಗಾತ್ರ_t min_len = padded ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(padding) : 0;
	ಗಾತ್ರ_t orig_len = ಸಾಲು->len;
	ಗಾತ್ರ_t len;

	ಆದರೆ (type == OBJ_BLOB) {
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಆದರೆ (odb_read_object_info(the_repository->objects, oid, &ಗಾತ್ರ_2) < 0)
			die(_("could not get object info about '%s'"),
			    oid_ಗಮ್ಯ_hex(oid));
		strbuf_ಕೂಡಿಸು_uint(ಸಾಲು, ಗಾತ್ರ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(ಸಾಲು, "-");
	}
	len = ಸಾಲು->len - orig_len;
	ಆದರೆ (len < min_len)
		strbuf_ಸೇರಿಸು(ಸಾಲು, orig_len, padding, min_len - len);
}

ರಚನೆ ls_ವೃಕ್ಷ_options {
	ಚಿಹ್ನೆರಹಿತ null_termination:1;
	ಪೂರ್ಣಾಂಕ abbrev;
	ಎಣಿಕೆ ls_ವೃಕ್ಷ_ಮಾರ್ಗ_options {
		LS_RECURSIVE = 1 << 0,
		LS_ವೃಕ್ಷ_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	ರಚನೆ pathspec pathspec;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_recursive(ರಚನೆ ls_ವೃಕ್ಷ_options *options, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ,
			  ಗಾತ್ರ_t baselen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname)
{
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (options->ls_options & LS_RECURSIVE)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (!options->pathspec.nr)
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (i = 0; i < options->pathspec.nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec = options->pathspec.ವಸ್ತುಗಳು[i].ಹೊಂದಿಕೆ;
		ಗಾತ್ರ_t len, speclen;

		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ತಳ, spec, baselen))
			ಮುಂದುವರಿ;
		len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(pathname);
		spec += baselen;
		speclen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(spec);
		ಆದರೆ (speclen <= len)
			ಮುಂದುವರಿ;
		ಆದರೆ (spec[len] && spec[len] != '/')
			ಮುಂದುವರಿ;
		ಆದರೆ (memcmp(pathname, spec, len))
			ಮುಂದುವರಿ;
		ಹಿಂತಿರುಗಿ 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವೃಕ್ಷ_fmt(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *ತಳ,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಶೂನ್ಯ *context)
{
	ರಚನೆ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಂಕ recurse = 0;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಎಣಿಕೆ object_type type = object_type(ಬಹುಲಕ);
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format = options->format;

	ಆದರೆ (type == OBJ_ವೃಕ್ಷ && show_recursive(options, ತಳ->buf, ತಳ->len, pathname))
		recurse = READ_ವೃಕ್ಷ_RECURSIVE;
	ಆದರೆ (type == OBJ_ವೃಕ್ಷ && recurse && !(options->ls_options & LS_SHOW_TREES))
		ಹಿಂತಿರುಗಿ recurse;
	ಆದರೆ (type == OBJ_BLOB && (options->ls_options & LS_ವೃಕ್ಷ_ONLY))
		ಹಿಂತಿರುಗಿ 0;

	ಆಗ (strbuf_expand_ಹೆಜ್ಜೆ(&sb, &format)) {
		ಗಾತ್ರ_t len;

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "%", &format))
			strbuf_addch(&sb, '%');
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ಬಹುಲಕ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectname)", &format))
			strbuf_ಕೂಡಿಸು_unique_abbrev(&sb, oid, options->abbrev);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(path)", &format)) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = options->ಪೂರ್ವಪ್ರತ್ಯಯ;
			ರಚನೆ strbuf sbuf = STRBUF_INIT;
			ಗಾತ್ರ_t baselen = ತಳ->len;

			strbuf_addstr(ತಳ, pathname);
			name = relative_ಮಾರ್ಗ(ತಳ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &sbuf);
			quote_c_style(name, &sb, ಖಾಲಿ, 0);
			strbuf_setlen(ತಳ, baselen);
			strbuf_release(&sbuf);
		} ಇಲ್ಲದಿದ್ದರೆ
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವೃಕ್ಷ_common(ರಚನೆ ls_ವೃಕ್ಷ_options *options, ಪೂರ್ಣಾಂಕ *recurse,
			    ರಚನೆ strbuf *ತಳ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname,
			    ಎಣಿಕೆ object_type type)
{
	ಪೂರ್ಣಾಂಕ ret = -1;
	*recurse = 0;

	ಆದರೆ (type == OBJ_BLOB) {
		ಆದರೆ (options->ls_options & LS_ವೃಕ್ಷ_ONLY)
			ret = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == OBJ_ವೃಕ್ಷ &&
		   show_recursive(options, ತಳ->buf, ತಳ->len, pathname)) {
		*recurse = READ_ವೃಕ್ಷ_RECURSIVE;
		ಆದರೆ (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ವೃಕ್ಷ_common_default_long(ರಚನೆ ls_ವೃಕ್ಷ_options *options,
					  ರಚನೆ strbuf *ತಳ,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname,
					  ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t baselen)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = options->ಪೂರ್ವಪ್ರತ್ಯಯ;

	strbuf_addstr(ತಳ, pathname);

	ಆದರೆ (options->null_termination) {
		ರಚನೆ strbuf sb = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = relative_ಮಾರ್ಗ(ತಳ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ಇಲ್ಲದಿದ್ದರೆ {
		write_name_quoted_relative(ತಳ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, stdout, '\n');
	}

	strbuf_setlen(ತಳ, baselen);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವೃಕ್ಷ_default(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *ತಳ,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ,
			     ಶೂನ್ಯ *context)
{
	ರಚನೆ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಂಕ early;
	ಪೂರ್ಣಾಂಕ recurse;
	ಎಣಿಕೆ object_type type = object_type(ಬಹುಲಕ);

	early = show_ವೃಕ್ಷ_common(options, &recurse, ತಳ, pathname, type);
	ಆದರೆ (early >= 0)
		ಹಿಂತಿರುಗಿ early;

	ಬರೆ("%06o %s %s\t", ಬಹುಲಕ, type_name(object_type(ಬಹುಲಕ)),
	       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, oid, options->abbrev));
	show_ವೃಕ್ಷ_common_default_long(options, ತಳ, pathname, ತಳ->len);
	ಹಿಂತಿರುಗಿ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವೃಕ್ಷ_long(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *ತಳ,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ,
			  ಶೂನ್ಯ *context)
{
	ರಚನೆ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಂಕ early;
	ಪೂರ್ಣಾಂಕ recurse;
	ಅಕ್ಷರ ಗಾತ್ರ_ಪಠ್ಯ[24];
	ಎಣಿಕೆ object_type type = object_type(ಬಹುಲಕ);

	early = show_ವೃಕ್ಷ_common(options, &recurse, ತಳ, pathname, type);
	ಆದರೆ (early >= 0)
		ಹಿಂತಿರುಗಿ early;

	ಆದರೆ (type == OBJ_BLOB) {
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಆದರೆ (odb_read_object_info(the_repository->objects, oid, &ಗಾತ್ರ_2) == OBJ_BAD)
			xsnprintf(ಗಾತ್ರ_ಪಠ್ಯ, ಗಾತ್ರ(ಗಾತ್ರ_ಪಠ್ಯ), "BAD");
		ಇಲ್ಲದಿದ್ದರೆ
			xsnprintf(ಗಾತ್ರ_ಪಠ್ಯ, ಗಾತ್ರ(ಗಾತ್ರ_ಪಠ್ಯ),
				  "%" PRIuMAX, (uintmax_t)ಗಾತ್ರ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		xsnprintf(ಗಾತ್ರ_ಪಠ್ಯ, ಗಾತ್ರ(ಗಾತ್ರ_ಪಠ್ಯ), "-");
	}

	ಬರೆ("%06o %s %s %7s\t", ಬಹುಲಕ, type_name(type),
	       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, oid, options->abbrev),
	       ಗಾತ್ರ_ಪಠ್ಯ);
	show_ವೃಕ್ಷ_common_default_long(options, ತಳ, pathname, ತಳ->len);
	ಹಿಂತಿರುಗಿ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವೃಕ್ಷ_name_only(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid ಬಳಸದ,
			       ರಚನೆ strbuf *ತಳ,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ,
			       ಶೂನ್ಯ *context)
{
	ರಚನೆ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಂಕ early;
	ಪೂರ್ಣಾಂಕ recurse;
	ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t baselen = ತಳ->len;
	ಎಣಿಕೆ object_type type = object_type(ಬಹುಲಕ);
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;

	early = show_ವೃಕ್ಷ_common(options, &recurse, ತಳ, pathname, type);
	ಆದರೆ (early >= 0)
		ಹಿಂತಿರುಗಿ early;

	ಪೂರ್ವಪ್ರತ್ಯಯ = options->ಪೂರ್ವಪ್ರತ್ಯಯ;
	strbuf_addstr(ತಳ, pathname);
	ಆದರೆ (options->null_termination) {
		ರಚನೆ strbuf sb = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = relative_ಮಾರ್ಗ(ತಳ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} ಇಲ್ಲದಿದ್ದರೆ {
		write_name_quoted_relative(ತಳ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, stdout, '\n');
	}
	strbuf_setlen(ತಳ, baselen);
	ಹಿಂತಿರುಗಿ recurse;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವೃಕ್ಷ_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *ತಳ,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ,
			    ಶೂನ್ಯ *context)
{
	ರಚನೆ ls_ವೃಕ್ಷ_options *options = context;
	ಪೂರ್ಣಾಂಕ early;
	ಪೂರ್ಣಾಂಕ recurse;
	ಎಣಿಕೆ object_type type = object_type(ಬಹುಲಕ);
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str;

	early = show_ವೃಕ್ಷ_common(options, &recurse, ತಳ, pathname, type);
	ಆದರೆ (early >= 0)
		ಹಿಂತಿರುಗಿ early;

	str = repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, oid, options->abbrev);
	ಆದರೆ (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} ಇಲ್ಲದಿದ್ದರೆ  {
		puts(str);
	}
	ಹಿಂತಿರುಗಿ recurse;
}

ಎಣಿಕೆ ls_ವೃಕ್ಷ_cmdmode {
	ಬಹುಲಕ_DEFAULT = 0,
	ಬಹುಲಕ_LONG,
	ಬಹುಲಕ_NAME_ONLY,
	ಬಹುಲಕ_NAME_ಸ್ಥಿತಿ,
	ಬಹುಲಕ_OBJECT_ONLY,
};

ರಚನೆ ls_ವೃಕ್ಷ_cmdmode_ಗಮ್ಯ_fmt {
	ಎಣಿಕೆ ls_ವೃಕ್ಷ_cmdmode ಬಹುಲಕ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ fmt;
	read_ವೃಕ್ಷ_fn_t fn;
};

ಸ್ಥಿರ ರಚನೆ ls_ವೃಕ್ಷ_cmdmode_ಗಮ್ಯ_fmt ls_ವೃಕ್ಷ_cmdmode_format[] = {
	{
		.ಬಹುಲಕ = ಬಹುಲಕ_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_ವೃಕ್ಷ_default,
	},
	{
		.ಬಹುಲಕ = ಬಹುಲಕ_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_ವೃಕ್ಷ_long,
	},
	{
		.ಬಹುಲಕ = ಬಹುಲಕ_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_ವೃಕ್ಷ_name_only,
	},
	{
		.ಬಹುಲಕ = ಬಹುಲಕ_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_ವೃಕ್ಷ_object
	},
	{
		/* fallback */
		.fn = show_ವೃಕ್ಷ_default,
	},
};

ಪೂರ್ಣಾಂಕ cmd_ls_ವೃಕ್ಷ(ಪೂರ್ಣಾಂಕ argc,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ object_id oid;
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಪೂರ್ಣಾಂಕ i, ತುಂಬಿದ_ವೃಕ್ಷ = 0;
	ಪೂರ್ಣಾಂಕ ತುಂಬಿದ_name = !ಪೂರ್ವಪ್ರತ್ಯಯ || !*ಪೂರ್ವಪ್ರತ್ಯಯ;
	read_ವೃಕ್ಷ_fn_t fn = ಖಾಲಿ;
	ಎಣಿಕೆ ls_ವೃಕ್ಷ_cmdmode cmdmode = ಬಹುಲಕ_DEFAULT;
	ಪೂರ್ಣಾಂಕ null_termination = 0;
	ರಚನೆ ls_ವೃಕ್ಷ_options options = { 0 };
	ಸ್ಥಿರಾಂಕ ರಚನೆ option ls_ವೃಕ್ಷ_options[] = {
		OPT_ಬಿಟ್('d', ಖಾಲಿ, &options.ls_options, N_("only show trees"),
			LS_ವೃಕ್ಷ_ONLY),
		OPT_ಬಿಟ್('r', ಖಾಲಿ, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_ಬಿಟ್('t', ಖಾಲಿ, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ಖಾಲಿ, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    ಬಹುಲಕ_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    ಬಹುಲಕ_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    ಬಹುಲಕ_NAME_ಸ್ಥಿತಿ),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    ಬಹುಲಕ_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &ತುಂಬಿದ_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &ತುಂಬಿದ_ವೃಕ್ಷ,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ ls_ವೃಕ್ಷ_cmdmode_ಗಮ್ಯ_fmt *m2f = ls_ವೃಕ್ಷ_cmdmode_format;
	ಪೂರ್ಣಾಂಕ ret;

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, ls_ವೃಕ್ಷ_options,
			     ls_ವೃಕ್ಷ_usage, 0);
	options.null_termination = null_termination;

	ಆದರೆ (ತುಂಬಿದ_ವೃಕ್ಷ)
		ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
	options.ಪೂರ್ವಪ್ರತ್ಯಯ = ತುಂಬಿದ_name ? ಖಾಲಿ : ಪೂರ್ವಪ್ರತ್ಯಯ;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	ಆದರೆ (cmdmode == ಬಹುಲಕ_NAME_ಸ್ಥಿತಿ)
		cmdmode = ಬಹುಲಕ_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	ಆದರೆ ( (LS_ವೃಕ್ಷ_ONLY|LS_RECURSIVE) ==
	    ((LS_ವೃಕ್ಷ_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	ಆದರೆ (options.format && cmdmode)
		usage_ಸಂದೇಶ_opt(
			_("--format can't be combined with other format-altering options"),
			ls_ವೃಕ್ಷ_usage, ls_ವೃಕ್ಷ_options);
	ಆದರೆ (argc < 1)
		usage_with_options(ls_ವೃಕ್ಷ_usage, ls_ವೃಕ್ಷ_options);
	ಆದರೆ (repo_get_oid_with_flags(the_repository, argv[0], &oid,
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
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv + 1);
	ಗಾಗಿ (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.ವಸ್ತುಗಳು[i].nowildcard_len = options.pathspec.ವಸ್ತುಗಳು[i].len;
	options.pathspec.has_wildcard = 0;
	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, &oid);
	ಆದರೆ (!ವೃಕ್ಷ)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	ಆಗ (m2f) {
		ಆದರೆ (!m2f->fmt) {
			fn = options.format ? show_ವೃಕ್ಷ_fmt : show_ವೃಕ್ಷ_default;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->ಬಹುಲಕ;
			fn = m2f->fn;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!options.format && cmdmode == m2f->ಬಹುಲಕ) {
			fn = m2f->fn;
		} ಇಲ್ಲದಿದ್ದರೆ {
			m2f++;
			ಮುಂದುವರಿ;
		}
		ನಿಲ್ಲಿಸು;
	}

	ret = !!read_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	ಹಿಂತಿರುಗಿ ret;
}
