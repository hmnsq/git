#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "ident.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "urlmatch.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "worktree.h"

स्थिर अचर वर्ण *अचर builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	रिक्त
};

स्थिर अचर वर्ण *अचर builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	रिक्त
};

स्थिर अचर वर्ण *अचर builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	रिक्त
};

स्थिर अचर वर्ण *अचर builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	रिक्त
};

स्थिर अचर वर्ण *अचर builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	रिक्त
};

स्थिर अचर वर्ण *अचर builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	रिक्त
};

स्थिर अचर वर्ण *अचर builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	रिक्त
};

स्थिर अचर वर्ण *अचर builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	रिक्त
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

संरचना config_location_options {
	संरचना git_config_source source;
	संरचना config_options options;
	वर्ण *file_to_free;
	पूर्णाङ्क use_global_config;
	पूर्णाङ्क use_system_config;
	पूर्णाङ्क use_local_config;
	पूर्णाङ्क use_worktree_config;
	पूर्णाङ्क respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

गणना config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_PATH,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#define CONFIG_TYPE_OPTIONS(type) \
	OPT_GROUP(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_मान(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_मान(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_मान(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_मान(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_मान(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_मान(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_मानहरू, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_साँचोहरू, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

संरचना config_display_options {
	पूर्णाङ्क end_nul;
	पूर्णाङ्क omit_मानहरू;
	पूर्णाङ्क show_origin;
	पूर्णाङ्क show_scope;
	पूर्णाङ्क show_साँचोहरू;
	गणना config_type type;
	वर्ण *default_मान;
	/* Populated via `display_options_init()`. */
	पूर्णाङ्क term;
	पूर्णाङ्क delim;
	पूर्णाङ्क साँचो_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.साँचो_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_मान(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.मान = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

स्थिर पूर्णाङ्क option_parse_type(अचर संरचना option *opt, अचर वर्ण *arg,
			     पूर्णाङ्क unset)
{
	पूर्णाङ्क new_type, *to_type;

	यदि (unset) {
		*((पूर्णाङ्क *) opt->मान) = 0;
		फर्काऊ 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	यदि (!new_type) {
		यदि (!म_भेद(arg, "bool"))
			new_type = TYPE_BOOL;
		अन्यथा यदि (!म_भेद(arg, "int"))
			new_type = TYPE_INT;
		अन्यथा यदि (!म_भेद(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		अन्यथा यदि (!म_भेद(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		अन्यथा यदि (!म_भेद(arg, "path"))
			new_type = TYPE_PATH;
		अन्यथा यदि (!म_भेद(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		अन्यथा यदि (!म_भेद(arg, "color"))
			new_type = TYPE_COLOR;
		अन्यथा
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->मान;
	यदि (*to_type && *to_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		निकास(129);
	}
	*to_type = new_type;

	फर्काऊ 0;
}

स्थिर शून्य check_argc(पूर्णाङ्क argc, पूर्णाङ्क min, पूर्णाङ्क max)
{
	यदि (argc >= min && argc <= max)
		फर्काऊ;
	यदि (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	अन्यथा
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	निकास(129);
}

स्थिर NORETURN शून्य die_missing_set_मान(अचर वर्ण *arg)
{
	अचर वर्ण *last_dot = म_खोजप(arg, '.');
	अचर वर्ण *eq = last_dot ? म_अक्षर(last_dot + 1, '=') : रिक्त;
	वर्ण *prefix = eq ? xstrndup(arg, eq - arg) : रिक्त;

	यदि (prefix && git_config_साँचो_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} अन्यथा यदि (git_config_साँचो_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} अन्यथा {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	मुक्त(prefix);
	निकास(129);
}

स्थिर शून्य show_config_origin(अचर संरचना config_display_options *opts,
			       अचर संरचना साँचो_मान_info *kvi,
			       संरचना strbuf *buf)
{
	अचर वर्ण term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	यदि (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	अन्यथा
		quote_c_style(kvi->filename ? kvi->filename : "", buf, रिक्त, 0);
	strbuf_addch(buf, term);
}

स्थिर शून्य show_config_scope(अचर संरचना config_display_options *opts,
			      अचर संरचना साँचो_मान_info *kvi,
			      संरचना strbuf *buf)
{
	अचर वर्ण term = opts->end_nul ? '\0' : '\t';
	अचर वर्ण *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

संरचना strbuf_list {
	संरचना strbuf *वस्तुहरू;
	पूर्णाङ्क nr;
	पूर्णाङ्क alloc;
};

स्थिर पूर्णाङ्क format_config_int64(संरचना strbuf *buf,
			       अचर वर्ण *साँचो_2,
			       अचर वर्ण *मान_2,
			       अचर संरचना साँचो_मान_info *kvi,
			       पूर्णाङ्क gently)
{
	int64_t v = 0;
	यदि (gently) {
		यदि (!git_parse_int64(मान_2, &v))
			फर्काऊ -1;
	} अन्यथा {
		/* may die() */
		v = git_config_int64(साँचो_2, मान_2 ? मान_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क format_config_bool(संरचना strbuf *buf,
			      अचर वर्ण *साँचो_2,
			      अचर वर्ण *मान_2,
			      पूर्णाङ्क gently)
{
	पूर्णाङ्क v = 0;
	यदि (gently) {
		यदि ((v = git_parse_maybe_bool(मान_2)) < 0)
			फर्काऊ -1;
	} अन्यथा {
		/* may die() */
		v = git_config_bool(साँचो_2, मान_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क format_config_bool_or_int(संरचना strbuf *buf,
				     अचर वर्ण *साँचो_2,
				     अचर वर्ण *मान_2,
				     अचर संरचना साँचो_मान_info *kvi,
				     पूर्णाङ्क gently)
{
	पूर्णाङ्क v, is_bool = 0;

	यदि (gently) {
		v = git_parse_maybe_bool_text(मान_2);

		यदि (v >= 0)
			is_bool = 1;
		अन्यथा यदि (!git_parse_int(मान_2, &v))
			फर्काऊ -1;
	} अन्यथा {
		v = git_config_bool_or_int(साँचो_2, मान_2, kvi,
					   &is_bool);
	}

	यदि (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	अन्यथा
		strbuf_addf(buf, "%d", v);

	फर्काऊ 0;
}

/* This mode is always gentle. */
स्थिर पूर्णाङ्क format_config_bool_or_str(संरचना strbuf *buf,
				     अचर वर्ण *मान_2)
{
	पूर्णाङ्क v = git_parse_maybe_bool(मान_2);
	यदि (v < 0)
		strbuf_addstr(buf, मान_2);
	अन्यथा
		strbuf_addstr(buf, v ? "true" : "false");
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क format_config_path(संरचना strbuf *buf,
			      अचर वर्ण *साँचो_2,
			      अचर वर्ण *मान_2,
			      पूर्णाङ्क gently)
{
	वर्ण *v;

	यदि (git_config_pathname(&v, साँचो_2, मान_2) < 0)
		फर्काऊ -1;

	यदि (v)
		strbuf_addstr(buf, v);
	अन्यथा
		फर्काऊ gently ? -1 : 1; /* :(optional)no-such-file */

	मुक्त(v);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क format_config_expiry_date(संरचना strbuf *buf,
				     अचर वर्ण *साँचो_2,
				     अचर वर्ण *मान_2,
				     पूर्णाङ्क quietly)
{
	timestamp_t t;
	यदि (quietly) {
		यदि (parse_expiry_date(मान_2, &t))
			फर्काऊ -1;
	} अन्यथा यदि (git_config_expiry_date(&t, साँचो_2, मान_2) < 0) {
		फर्काऊ -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क format_config_color(संरचना strbuf *buf,
			       अचर वर्ण *साँचो_2,
			       अचर वर्ण *मान_2,
			       पूर्णाङ्क gently)
{
	वर्ण v[COLOR_MAXLEN];

	यदि (gently) {
		यदि (color_parse_quietly(मान_2, v) < 0)
			फर्काऊ -1;
	} अन्यथा यदि (git_config_color(v, साँचो_2, मान_2) < 0) {
		फर्काऊ -1;
	}

	strbuf_addstr(buf, v);
	फर्काऊ 0;
}

/*
 * Format the configuration key-value pair (`key_`, `value_`) and
 * append it into strbuf `buf`.  Returns a negative value on failure,
 * 0 on success, 1 on a missing optional value (i.e., telling the
 * caller to pretend that <key_,value_> did not exist).
 *
 * Note: 'gently' is currently ignored, but will be implemented in
 * a future change.
 */
स्थिर पूर्णाङ्क format_config(अचर संरचना config_display_options *opts,
			 संरचना strbuf *buf, अचर वर्ण *साँचो_2,
			 अचर वर्ण *मान_2, अचर संरचना साँचो_मान_info *kvi,
			 पूर्णाङ्क gently)
{
	पूर्णाङ्क res = 0;
	यदि (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	यदि (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	यदि (opts->show_साँचोहरू)
		strbuf_addstr(buf, साँचो_2);

	यदि (opts->omit_मानहरू)
		गच्छ terminator;

	यदि (opts->show_साँचोहरू)
		strbuf_addch(buf, opts->साँचो_delim);

	विकल्प (opts->type) {
	पक्ष TYPE_INT:
		res = format_config_int64(buf, साँचो_2, मान_2, kvi, gently);
		रोक;

	पक्ष TYPE_BOOL:
		res = format_config_bool(buf, साँचो_2, मान_2, gently);
		रोक;

	पक्ष TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, साँचो_2, मान_2, kvi, gently);
		रोक;

	पक्ष TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, मान_2);
		रोक;

	पक्ष TYPE_PATH:
		res = format_config_path(buf, साँचो_2, मान_2, gently);
		रोक;

	पक्ष TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, साँचो_2, मान_2, gently);
		रोक;

	पक्ष TYPE_COLOR:
		res = format_config_color(buf, साँचो_2, मान_2, gently);
		रोक;

	पक्ष TYPE_NONE:
		यदि (मान_2) {
			strbuf_addstr(buf, मान_2);
		} अन्यथा {
			/* Just show the key name; back out delimiter */
			यदि (opts->show_साँचोहरू)
				strbuf_setlen(buf, buf->len - 1);
		}
		रोक;

	स्वभाव:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क show_all_config(अचर वर्ण *साँचो_2, अचर वर्ण *मान_2,
			   अचर संरचना config_context *ctx,
			   शून्य *cb)
{
	अचर संरचना config_display_options *opts = cb;
	अचर संरचना साँचो_मान_info *kvi = ctx->kvi;
	संरचना strbuf formatted = STRBUF_INIT;

	यदि (format_config(opts, &formatted, साँचो_2, मान_2, kvi, 1) >= 0)
		ख_डालो(formatted.buf, 1, formatted.len, मानक_निकास);

	strbuf_release(&formatted);
	फर्काऊ 0;
}

#define GET_मान_ALL        (1 << 0)
#define GET_मान_साँचो_REGEXP (1 << 1)

संरचना collect_config_data {
	अचर संरचना config_display_options *display_opts;
	संरचना strbuf_list *मानहरू;
	अचर वर्ण *मान_pattern;
	अचर वर्ण *साँचो;
	regex_t *regexp;
	regex_t *साँचो_regexp;
	पूर्णाङ्क do_not_match;
	अचिन्हित get_मान_flags;
	अचिन्हित flags;
};

स्थिर पूर्णाङ्क collect_config(अचर वर्ण *साँचो_2, अचर वर्ण *मान_2,
			  अचर संरचना config_context *ctx, शून्य *cb)
{
	संरचना collect_config_data *data = cb;
	संरचना strbuf_list *मानहरू = data->मानहरू;
	अचर संरचना साँचो_मान_info *kvi = ctx->kvi;
	पूर्णाङ्क status;

	यदि (!(data->get_मान_flags & GET_मान_साँचो_REGEXP) &&
	    म_भेद(साँचो_2, data->साँचो))
		फर्काऊ 0;
	यदि ((data->get_मान_flags & GET_मान_साँचो_REGEXP) &&
	    regexec(data->साँचो_regexp, साँचो_2, 0, रिक्त, 0))
		फर्काऊ 0;
	यदि ((data->flags & CONFIG_FLAGS_FIXED_मान) &&
	    म_भेद(data->मान_pattern, (मान_2?मान_2:"")))
		फर्काऊ 0;
	यदि (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (मान_2?मान_2:""), 0, रिक्त, 0)))
		फर्काऊ 0;

	ALLOC_GROW(मानहरू->वस्तुहरू, मानहरू->nr + 1, मानहरू->alloc);
	strbuf_init(&मानहरू->वस्तुहरू[मानहरू->nr], 0);

	status = format_config(data->display_opts, &मानहरू->वस्तुहरू[मानहरू->nr++],
			       साँचो_2, मान_2, kvi, 0);
	यदि (status < 0)
		फर्काऊ status;
	यदि (status) {
		strbuf_release(&मानहरू->वस्तुहरू[--मानहरू->nr]);
		status = 0;
	}
	फर्काऊ status;
}

स्थिर पूर्णाङ्क get_मान(अचर संरचना config_location_options *opts,
		     अचर संरचना config_display_options *display_opts,
		     अचर वर्ण *साँचो_2, अचर वर्ण *regex_,
		     अचिन्हित get_मान_flags, अचिन्हित flags)
{
	पूर्णाङ्क ret = CONFIG_GENERIC_ERROR;
	संरचना strbuf_list मानहरू = {रिक्त};
	संरचना collect_config_data data = {
		.display_opts = display_opts,
		.मानहरू = &मानहरू,
		.get_मान_flags = get_मान_flags,
		.flags = flags,
	};
	वर्ण *साँचो = रिक्त;
	पूर्णाङ्क i;

	यदि (get_मान_flags & GET_मान_साँचो_REGEXP) {
		वर्ण *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		साँचो = xstrdup(साँचो_2);
		लागि (tl = साँचो + म_माप(साँचो) - 1;
		     tl >= साँचो && *tl != '.';
		     tl--)
			*tl = छोटे(*tl);
		लागि (tl = साँचो; *tl && *tl != '.'; tl++)
			*tl = छोटे(*tl);

		data.साँचो_regexp = (regex_t*)xmalloc(परिमाण(regex_t));
		यदि (regcomp(data.साँचो_regexp, साँचो, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), साँचो_2);
			FREE_AND_NULL(data.साँचो_regexp);
			ret = CONFIG_INVALID_PATTERN;
			गच्छ free_strings;
		}
	} अन्यथा {
		यदि (git_config_parse_साँचो(साँचो_2, &साँचो, रिक्त)) {
			ret = CONFIG_INVALID_साँचो;
			गच्छ free_strings;
		}

		data.साँचो = साँचो;
	}

	यदि (regex_ && (flags & CONFIG_FLAGS_FIXED_मान))
		data.मान_pattern = regex_;
	अन्यथा यदि (regex_) {
		यदि (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(परिमाण(regex_t));
		यदि (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			गच्छ free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	यदि (!मानहरू.nr && display_opts->default_मान) {
		संरचना साँचो_मान_info kvi = KVI_INIT;
		संरचना strbuf *item;
		पूर्णाङ्क status;

		kvi_from_param(&kvi);
		ALLOC_GROW(मानहरू.वस्तुहरू, मानहरू.nr + 1, मानहरू.alloc);
		item = &मानहरू.वस्तुहरू[मानहरू.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, साँचो_2,
				       display_opts->default_मान, &kvi, 0);
		यदि (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_मान);
		यदि (status) {
			/* default was a missing optional value */
			मानहरू.nr--;
			strbuf_release(item);
		}
	}

	ret = !मानहरू.nr;

	लागि (i = 0; i < मानहरू.nr; i++) {
		संरचना strbuf *buf = मानहरू.वस्तुहरू + i;
		यदि ((get_मान_flags & GET_मान_ALL) || i == मानहरू.nr - 1)
			ख_डालो(buf->buf, 1, buf->len, मानक_निकास);
		strbuf_release(buf);
	}
	मुक्त(मानहरू.वस्तुहरू);

free_strings:
	मुक्त(साँचो);
	यदि (data.साँचो_regexp) {
		regfree(data.साँचो_regexp);
		मुक्त(data.साँचो_regexp);
	}
	यदि (data.regexp) {
		regfree(data.regexp);
		मुक्त(data.regexp);
	}

	फर्काऊ ret;
}

स्थिर वर्ण *normalize_मान(अचर वर्ण *साँचो, अचर वर्ण *मान,
			     पूर्णाङ्क type, संरचना साँचो_मान_info *kvi)
{
	यदि (!मान)
		फर्काऊ रिक्त;

	यदि (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		फर्काऊ xstrdup(मान);
	यदि (type == TYPE_INT)
		फर्काऊ xstrfmt("%"PRId64, git_config_int64(साँचो, मान, kvi));
	यदि (type == TYPE_BOOL)
		फर्काऊ xstrdup(git_config_bool(साँचो, मान) ?  "true" : "false");
	यदि (type == TYPE_BOOL_OR_INT) {
		पूर्णाङ्क is_bool, v;
		v = git_config_bool_or_int(साँचो, मान, kvi, &is_bool);
		यदि (!is_bool)
			फर्काऊ xstrfmt("%d", v);
		अन्यथा
			फर्काऊ xstrdup(v ? "true" : "false");
	}
	यदि (type == TYPE_BOOL_OR_STR) {
		पूर्णाङ्क v = git_parse_maybe_bool(मान);
		यदि (v < 0)
			फर्काऊ xstrdup(मान);
		अन्यथा
			फर्काऊ xstrdup(v ? "true" : "false");
	}
	यदि (type == TYPE_COLOR) {
		वर्ण v[COLOR_MAXLEN];
		यदि (git_config_color(v, साँचो, मान))
			die(_("cannot parse color '%s'"), मान);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		फर्काऊ xstrdup(मान);
	}

	BUG("cannot normalize type %d", type);
}

संरचना get_color_config_data {
	पूर्णाङ्क get_color_found;
	अचर वर्ण *get_color_slot;
	वर्ण parsed_color[COLOR_MAXLEN];
};

स्थिर पूर्णाङ्क git_get_color_config(अचर वर्ण *var, अचर वर्ण *मान,
				अचर संरचना config_context *ctx UNUSED,
				शून्य *cb)
{
	संरचना get_color_config_data *data = cb;

	यदि (!म_भेद(var, data->get_color_slot)) {
		यदि (!मान)
			config_error_nonbool(var);
		यदि (color_parse(मान, data->parsed_color) < 0)
			फर्काऊ -1;
		data->get_color_found = 1;
	}
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_color(अचर संरचना config_location_options *opts,
		      अचर वर्ण *var, अचर वर्ण *def_color)
{
	संरचना get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	पूर्णाङ्क ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	यदि (!data.get_color_found && def_color) {
		यदि (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			गच्छ out;
		}
	}

	ret = 0;

out:
	ख_माला_दो(data.parsed_color, मानक_निकास);
	फर्काऊ ret;
}

संरचना get_colorbool_config_data {
	गणना git_colorbool get_colorbool_found;
	गणना git_colorbool get_diff_color_found;
	गणना git_colorbool get_color_ui_found;
	अचर वर्ण *get_colorbool_slot;
};

स्थिर पूर्णाङ्क git_get_colorbool_config(अचर वर्ण *var, अचर वर्ण *मान,
				    अचर संरचना config_context *ctx UNUSED,
				    शून्य *cb)
{
	संरचना get_colorbool_config_data *data = cb;

	यदि (!म_भेद(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, मान);
	अन्यथा यदि (!म_भेद(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, मान);
	अन्यथा यदि (!म_भेद(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, मान);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_colorbool(अचर संरचना config_location_options *opts,
			 अचर वर्ण *var, पूर्णाङ्क print)
{
	संरचना get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	यदि (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		यदि (!म_भेद(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		यदि (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	यदि (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	यदि (print) {
		म_लिखो("%s\n", result ? "true" : "false");
		फर्काऊ 0;
	} अन्यथा
		फर्काऊ result ? 0 : 1;
}

स्थिर शून्य check_write(अचर संरचना git_config_source *source)
{
	यदि (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	यदि (source->use_stdin)
		die(_("writing to stdin is not supported"));

	यदि (source->blob)
		die(_("writing config blobs is not supported"));
}

संरचना urlmatch_current_candidate_मान {
	वर्ण मान_is_null;
	संरचना strbuf मान;
	संरचना साँचो_मान_info kvi;
};

स्थिर पूर्णाङ्क urlmatch_collect_fn(अचर वर्ण *var, अचर वर्ण *मान,
			       अचर संरचना config_context *ctx,
			       शून्य *cb)
{
	संरचना string_list *मानहरू = cb;
	संरचना string_list_item *item = string_list_घुसाउ(मानहरू, var);
	संरचना urlmatch_current_candidate_मान *matched = item->util;
	अचर संरचना साँचो_मान_info *kvi = ctx->kvi;

	यदि (!matched) {
		matched = xmalloc(परिमाण(*matched));
		strbuf_init(&matched->मान, 0);
		item->util = matched;
	} अन्यथा {
		strbuf_reset(&matched->मान);
	}
	matched->kvi = *kvi;

	यदि (मान) {
		strbuf_addstr(&matched->मान, मान);
		matched->मान_is_null = 0;
	} अन्यथा {
		matched->मान_is_null = 1;
	}
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_urlmatch(अचर संरचना config_location_options *opts,
			अचर संरचना config_display_options *_display_opts,
			अचर वर्ण *var, अचर वर्ण *url)
{
	पूर्णाङ्क ret;
	वर्ण *section;
	वर्ण *section_tail;
	संरचना config_display_options display_opts = *_display_opts;
	संरचना string_list_item *item;
	संरचना urlmatch_config config = URLMATCH_CONFIG_INIT;
	संरचना string_list मानहरू = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = रिक्त;
	config.cb = &मानहरू;

	यदि (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = म_अक्षर(section, '.');
	यदि (section_tail) {
		*section_tail = '\0';
		config.साँचो = section_tail + 1;
		display_opts.show_साँचोहरू = 0;
	} अन्यथा {
		config.साँचो = रिक्त;
		display_opts.show_साँचोहरू = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !मानहरू.nr;

	for_each_string_list_item(item, &मानहरू) {
		संरचना urlmatch_current_candidate_मान *matched = item->util;
		संरचना strbuf buf = STRBUF_INIT;
		पूर्णाङ्क status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->मान_is_null ? रिक्त : matched->मान.buf,
				       &matched->kvi, 0);
		यदि (!status)
			ख_डालो(buf.buf, 1, buf.len, मानक_निकास);
		strbuf_release(&buf);

		strbuf_release(&matched->मान);
	}
	urlmatch_config_release(&config);
	string_list_clear(&मानहरू, 1);
	मुक्त(config.url.url);

	मुक्त(section);
	फर्काऊ ret;
}

स्थिर वर्ण *default_user_config(शून्य)
{
	संरचना strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	फर्काऊ strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य location_options_init(संरचना config_location_options *opts,
				  अचर वर्ण *prefix)
{
	यदि (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(दो_पर्या(CONFIG_ENVIRONMENT));

	यदि (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		निकास(129);
	}

	यदि (!startup_info->have_repository) {
		यदि (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		यदि (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		यदि (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	यदि (opts->source.file &&
			!म_भेद(opts->source.file, "-")) {
		opts->source.file = रिक्त;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	यदि (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		यदि (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} अन्यथा यदि (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} अन्यथा यदि (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} अन्यथा यदि (opts->use_worktree_config) {
		संरचना worktree **worktrees = get_worktrees(the_repository);
		यदि (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		अन्यथा यदि (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		अन्यथा
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} अन्यथा यदि (opts->source.file) {
		यदि (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} अन्यथा यदि (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	यदि (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	अन्यथा
		opts->options.respect_includes = opts->respect_includes_opt;
	यदि (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

स्थिर शून्य location_options_release(संरचना config_location_options *opts)
{
	मुक्त(opts->file_to_free);
}

स्थिर शून्य display_options_init(संरचना config_display_options *opts)
{
	यदि (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->साँचो_delim = '\n';
	}
}

स्थिर शून्य display_options_init_list(संरचना config_display_options *opts)
{
	opts->show_साँचोहरू = 1;

	यदि (opts->end_nul) {
		display_options_init(opts);
	} अन्यथा {
		opts->term = '\n';
		opts->delim = ' ';
		opts->साँचो_delim = '=';
	}
}

स्थिर पूर्णाङ्क cmd_config_list(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	display_options_init_list(&display_opts);

	setup_auto_pager("config", 1);

	यदि (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		यदि (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		अन्यथा
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क cmd_config_get(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	अचर वर्ण *मान_pattern = रिक्त, *url = रिक्त;
	पूर्णाङ्क flags = 0;
	अचिन्हित get_मान_flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_मान_flags, N_("return all values for multi-valued config options"), GET_मान_ALL),
		OPT_BIT(0, "regexp", &get_मान_flags, N_("interpret the name as a regular expression"), GET_मान_साँचो_REGEXP),
		OPT_STRING(0, "value", &मान_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मान),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_मान,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	यदि ((flags & CONFIG_FLAGS_FIXED_मान) && !मान_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	यदि (display_opts.default_मान &&
	    ((get_मान_flags & GET_मान_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	यदि (url && ((get_मान_flags & GET_मान_ALL) ||
		    (get_मान_flags & GET_मान_साँचो_REGEXP) ||
		    मान_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	यदि (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	यदि (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	अन्यथा यदि (display_opts.type == TYPE_COLOR && !म_माप(argv[0]) && display_opts.default_मान)
		ret = get_color(&location_opts, "", display_opts.default_मान);
	अन्यथा
		ret = get_मान(&location_opts, &display_opts, argv[0], मान_pattern,
				get_मान_flags, flags);

	location_options_release(&location_opts);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क cmd_config_set(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	अचर वर्ण *मान_pattern = रिक्त, *comment_arg = रिक्त;
	वर्ण *comment = रिक्त;
	पूर्णाङ्क flags = 0, append = 0, type = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मान_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मान),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	संरचना साँचो_मान_info default_kvi = KVI_INIT;
	वर्ण *मान;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	यदि (argc == 1)
		die_missing_set_मान(argv[0]);
	check_argc(argc, 2, 2);

	यदि ((flags & CONFIG_FLAGS_FIXED_मान) && !मान_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	यदि (append && मान_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	यदि (append)
		मान_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	मान = normalize_मान(argv[0], argv[1], type, &default_kvi);

	यदि ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मान_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मान, मान_pattern,
							      comment, flags);
	} अन्यथा {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, मान);
		यदि (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मान);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क cmd_config_unset(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			    संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	अचर वर्ण *मान_pattern = रिक्त;
	पूर्णाङ्क flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मान_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मान),
		OPT_END(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	यदि ((flags & CONFIG_FLAGS_FIXED_मान) && !मान_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	यदि ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मान_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], रिक्त, मान_pattern,
							      रिक्त, flags);
	अन्यथा
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     रिक्त, रिक्त);

	location_options_release(&location_opts);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क cmd_config_rename_section(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
				     संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	यदि (ret < 0)
		गच्छ out;
	अन्यथा यदि (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क cmd_config_remove_section(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
				     संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], रिक्त);
	यदि (ret < 0)
		गच्छ out;
	अन्यथा यदि (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क show_editor(संरचना config_location_options *opts)
{
	वर्ण *config_file;

	यदि (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	यदि (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	यदि (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, रिक्त);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	यदि (opts->use_global_config) {
		पूर्णाङ्क fd = खोलो(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		यदि (fd >= 0) {
			वर्ण *content = default_user_config();
			write_str_in_full(fd, content);
			मुक्त(content);
			बंद(fd);
		}
		अन्यथा यदि (त्रुटि_सं != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	यदि (launch_editor(config_file, रिक्त, रिक्त)) {
		मुक्त(config_file);
		फर्काऊ -1;
	}
	मुक्त(config_file);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क cmd_config_edit(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			   संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क cmd_config_actions(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix)
{
	गणना {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_ADD = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_SET = (1<<11),
		ACTION_SET_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	अचर वर्ण *comment_arg = रिक्त;
	पूर्णाङ्क actions = 0;
	अचिन्हित flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_ADD),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_मान,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मान),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	वर्ण *मान = रिक्त, *comment = रिक्त;
	पूर्णाङ्क ret = 0;
	पूर्णाङ्क actions_implicit;
	संरचना साँचो_मान_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	यदि ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		निकास(129);
	}

	actions_implicit = (actions == 0);
	यदि (actions_implicit)
		विकल्प (argc) {
		पक्ष 1: actions = ACTION_GET; रोक;
		पक्ष 2: actions = ACTION_SET; रोक;
		पक्ष 3: actions = ACTION_SET_ALL; रोक;
		स्वभाव:
			error(_("no action specified"));
			निकास(129);
		}
	यदि (actions_implicit && argc == 1) {
		अचर वर्ण *last_dot = म_खोजप(argv[0], '.');
		यदि (last_dot && म_अक्षर(last_dot + 1, '='))
			die_missing_set_मान(argv[0]);
	}
	यदि (display_opts.omit_मानहरू &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		निकास(129);
	}

	यदि (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		निकास(129);
	}

	यदि (display_opts.default_मान && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		निकास(129);
	}

	यदि (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		निकास(129);
	}

	/* check usage of --fixed-value */
	यदि (flags & CONFIG_FLAGS_FIXED_मान) {
		पूर्णाङ्क allowed_usage = 0;

		विकल्प (actions) {
		/* git config --get <name> <value-pattern> */
		पक्ष ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		पक्ष ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		पक्ष ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		पक्ष ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		पक्ष ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			रोक;

		/* git config <name> <value> <value-pattern> */
		पक्ष ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		पक्ष ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			रोक;

		/* other options don't allow --fixed-value */
		}

		यदि (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			निकास(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	यदि (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	यदि (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		यदि (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			यदि (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			अन्यथा
				die(_("error processing config file(s)"));
		}
	}
	अन्यथा यदि (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	अन्यथा यदि (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मान = normalize_मान(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, मान);
		यदि (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	अन्यथा यदि (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मान = normalize_मान(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मान, argv[2],
							      comment, flags);
	}
	अन्यथा यदि (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मान = normalize_मान(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मान,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	अन्यथा यदि (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मान = normalize_मान(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मान, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अन्यथा यदि (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_मान(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	अन्यथा यदि (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_मान(&location_opts, &display_opts, argv[0], argv[1],
				GET_मान_ALL, flags);
	}
	अन्यथा यदि (actions == ACTION_GET_REGEXP) {
		display_opts.show_साँचोहरू = 1;
		check_argc(argc, 1, 2);
		ret = get_मान(&location_opts, &display_opts, argv[0], argv[1],
				GET_मान_ALL|GET_मान_साँचो_REGEXP, flags);
	}
	अन्यथा यदि (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	अन्यथा यदि (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		यदि (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], रिक्त, argv[1],
								      रिक्त, flags);
		अन्यथा
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], रिक्त, रिक्त);
	}
	अन्यथा यदि (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], रिक्त, argv[1],
							      रिक्त, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अन्यथा यदि (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		यदि (ret < 0)
			गच्छ out;
		अन्यथा यदि (!ret)
			die(_("no such section: %s"), argv[0]);
		अन्यथा
			ret = 0;
	}
	अन्यथा यदि (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], रिक्त);
		यदि (ret < 0)
			गच्छ out;
		अन्यथा यदि (!ret)
			die(_("no such section: %s"), argv[0]);
		अन्यथा
			ret = 0;
	}
	अन्यथा यदि (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	अन्यथा यदि (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		यदि (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मान);
	फर्काऊ ret;
}

पूर्णाङ्क cmd_config(पूर्णाङ्क argc,
	       अचर वर्ण **argv,
	       अचर वर्ण *prefix,
	       संरचना repository *repo)
{
	parse_opt_subcommand_fn *subcommand = रिक्त;
	संरचना option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_set),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_END(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	यदि (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		फर्काऊ subcommand(argc, argv, prefix, repo);
	}

	फर्काऊ cmd_config_actions(argc, argv, prefix);
}
