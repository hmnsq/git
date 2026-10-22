#परिभाषय USE_THE_REPOSITORY_VARIABLE
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

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_usage[] = {
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

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	रिक्त
};

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_समुच्चयः_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	रिक्त
};

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	रिक्त
};

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	रिक्त
};

#परिभाषय CONFIG_LOCATION_OPTIONS(opts) \
	OPT_गणः(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

संरचना config_location_options {
	संरचना git_config_source source;
	संरचना config_options options;
	वर्ण *file_गन्तव्यम्_free;
	पूर्णाङ्क use_global_config;
	पूर्णाङ्क use_system_config;
	पूर्णाङ्क use_local_config;
	पूर्णाङ्क use_worktree_config;
	पूर्णाङ्क respect_includes_opt;
};
#परिभाषय CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

गणना config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_मार्गः,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#परिभाषय CONFIG_TYPE_OPTIONS(type) \
	OPT_गणः(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_मानम्(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_मानम्(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_मानम्(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_मानम्(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_मानम्(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_मार्गः), \
	OPT_CALLBACK_मानम्(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#परिभाषय CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_गणः(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.अन्तः_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_मानानि, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_कुञ्चिकाः, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

संरचना config_display_options {
	पूर्णाङ्क अन्तः_nul;
	पूर्णाङ्क omit_मानानि;
	पूर्णाङ्क show_origin;
	पूर्णाङ्क show_scope;
	पूर्णाङ्क show_कुञ्चिकाः;
	गणना config_type type;
	वर्ण *default_मानम्;
	/* Populated via `display_options_init()`. */
	पूर्णाङ्क पदांशः;
	पूर्णाङ्क delim;
	पूर्णाङ्क कुञ्चिका_delim;
};
#परिभाषय CONFIG_DISPLAY_OPTIONS_INIT { \
	.पदांशः = '\n', \
	.delim = '=', \
	.कुञ्चिका_delim = ' ', \
	.type = TYPE_NONE, \
}

#परिभाषय OPT_CALLBACK_मानम्(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.मानम् = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

स्थिर पूर्णाङ्क option_parse_type(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			     पूर्णाङ्क unset)
{
	पूर्णाङ्क new_type, *गन्तव्यम्_type;

	यदि (unset) {
		*((पूर्णाङ्क *) opt->मानम्) = 0;
		प्रत्यागम 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	यदि (!new_type) {
		यदि (!वाक्यं_तुलय(arg, "bool"))
			new_type = TYPE_BOOL;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "int"))
			new_type = TYPE_INT;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "path"))
			new_type = TYPE_मार्गः;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		अन्यथा यदि (!वाक्यं_तुलय(arg, "color"))
			new_type = TYPE_COLOR;
		अन्यथा
			die(_("unrecognized --type argument, %s"), arg);
	}

	गन्तव्यम्_type = opt->मानम्;
	यदि (*गन्तव्यम्_type && *गन्तव्यम्_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		निर्गच्छ(129);
	}
	*गन्तव्यम्_type = new_type;

	प्रत्यागम 0;
}

स्थिर शून्य check_argc(पूर्णाङ्क argc, पूर्णाङ्क min, पूर्णाङ्क max)
{
	यदि (argc >= min && argc <= max)
		प्रत्यागम;
	यदि (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	अन्यथा
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	निर्गच्छ(129);
}

स्थिर NORETURN शून्य die_missing_समुच्चयः_मानम्(ध्रुव वर्ण *arg)
{
	ध्रुव वर्ण *चरमः_dot = strrchr(arg, '.');
	ध्रुव वर्ण *eq = चरमः_dot ? strchr(चरमः_dot + 1, '=') : रिक्त;
	वर्ण *उपसर्गः = eq ? xstrndup(arg, eq - arg) : रिक्त;

	यदि (उपसर्गः && git_config_कुञ्चिका_is_valid(उपसर्गः)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       उपसर्गः, eq + 1);
	} अन्यथा यदि (git_config_कुञ्चिका_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} अन्यथा {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	मुक्त(उपसर्गः);
	निर्गच्छ(129);
}

स्थिर शून्य show_config_origin(ध्रुव संरचना config_display_options *opts,
			       ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
			       संरचना strbuf *buf)
{
	ध्रुव वर्ण पदांशः = opts->अन्तः_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	यदि (opts->अन्तः_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	अन्यथा
		quote_c_style(kvi->filename ? kvi->filename : "", buf, रिक्त, 0);
	strbuf_addch(buf, पदांशः);
}

स्थिर शून्य show_config_scope(ध्रुव संरचना config_display_options *opts,
			      ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
			      संरचना strbuf *buf)
{
	ध्रुव वर्ण पदांशः = opts->अन्तः_nul ? '\0' : '\t';
	ध्रुव वर्ण *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, पदांशः);
}

संरचना strbuf_list {
	संरचना strbuf *वस्तूनि;
	पूर्णाङ्क nr;
	पूर्णाङ्क alloc;
};

स्थिर पूर्णाङ्क format_config_int64(संरचना strbuf *buf,
			       ध्रुव वर्ण *कुञ्चिका_2,
			       ध्रुव वर्ण *मानम्_2,
			       ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
			       पूर्णाङ्क gently)
{
	int64_t v = 0;
	यदि (gently) {
		यदि (!git_parse_int64(मानम्_2, &v))
			प्रत्यागम -1;
	} अन्यथा {
		/* may die() */
		v = git_config_int64(कुञ्चिका_2, मानम्_2 ? मानम्_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क format_config_bool(संरचना strbuf *buf,
			      ध्रुव वर्ण *कुञ्चिका_2,
			      ध्रुव वर्ण *मानम्_2,
			      पूर्णाङ्क gently)
{
	पूर्णाङ्क v = 0;
	यदि (gently) {
		यदि ((v = git_parse_maybe_bool(मानम्_2)) < 0)
			प्रत्यागम -1;
	} अन्यथा {
		/* may die() */
		v = git_config_bool(कुञ्चिका_2, मानम्_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क format_config_bool_or_int(संरचना strbuf *buf,
				     ध्रुव वर्ण *कुञ्चिका_2,
				     ध्रुव वर्ण *मानम्_2,
				     ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
				     पूर्णाङ्क gently)
{
	पूर्णाङ्क v, is_bool = 0;

	यदि (gently) {
		v = git_parse_maybe_bool_text(मानम्_2);

		यदि (v >= 0)
			is_bool = 1;
		अन्यथा यदि (!git_parse_int(मानम्_2, &v))
			प्रत्यागम -1;
	} अन्यथा {
		v = git_config_bool_or_int(कुञ्चिका_2, मानम्_2, kvi,
					   &is_bool);
	}

	यदि (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	अन्यथा
		strbuf_addf(buf, "%d", v);

	प्रत्यागम 0;
}

/* This mode is always gentle. */
स्थिर पूर्णाङ्क format_config_bool_or_str(संरचना strbuf *buf,
				     ध्रुव वर्ण *मानम्_2)
{
	पूर्णाङ्क v = git_parse_maybe_bool(मानम्_2);
	यदि (v < 0)
		strbuf_addstr(buf, मानम्_2);
	अन्यथा
		strbuf_addstr(buf, v ? "true" : "false");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क format_config_मार्गः(संरचना strbuf *buf,
			      ध्रुव वर्ण *कुञ्चिका_2,
			      ध्रुव वर्ण *मानम्_2,
			      पूर्णाङ्क gently)
{
	वर्ण *v;

	यदि (git_config_pathname(&v, कुञ्चिका_2, मानम्_2) < 0)
		प्रत्यागम -1;

	यदि (v)
		strbuf_addstr(buf, v);
	अन्यथा
		प्रत्यागम gently ? -1 : 1; /* :(optional)no-such-file */

	मुक्त(v);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क format_config_expiry_date(संरचना strbuf *buf,
				     ध्रुव वर्ण *कुञ्चिका_2,
				     ध्रुव वर्ण *मानम्_2,
				     पूर्णाङ्क quietly)
{
	timestamp_t t;
	यदि (quietly) {
		यदि (parse_expiry_date(मानम्_2, &t))
			प्रत्यागम -1;
	} अन्यथा यदि (git_config_expiry_date(&t, कुञ्चिका_2, मानम्_2) < 0) {
		प्रत्यागम -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क format_config_color(संरचना strbuf *buf,
			       ध्रुव वर्ण *कुञ्चिका_2,
			       ध्रुव वर्ण *मानम्_2,
			       पूर्णाङ्क gently)
{
	वर्ण v[COLOR_MAXLEN];

	यदि (gently) {
		यदि (color_parse_quietly(मानम्_2, v) < 0)
			प्रत्यागम -1;
	} अन्यथा यदि (git_config_color(v, कुञ्चिका_2, मानम्_2) < 0) {
		प्रत्यागम -1;
	}

	strbuf_addstr(buf, v);
	प्रत्यागम 0;
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
स्थिर पूर्णाङ्क format_config(ध्रुव संरचना config_display_options *opts,
			 संरचना strbuf *buf, ध्रुव वर्ण *कुञ्चिका_2,
			 ध्रुव वर्ण *मानम्_2, ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
			 पूर्णाङ्क gently)
{
	पूर्णाङ्क res = 0;
	यदि (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	यदि (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	यदि (opts->show_कुञ्चिकाः)
		strbuf_addstr(buf, कुञ्चिका_2);

	यदि (opts->omit_मानानि)
		गच्छ terminator;

	यदि (opts->show_कुञ्चिकाः)
		strbuf_addch(buf, opts->कुञ्चिका_delim);

	विकल्प (opts->type) {
	पक्ष TYPE_INT:
		res = format_config_int64(buf, कुञ्चिका_2, मानम्_2, kvi, gently);
		भङ्ग;

	पक्ष TYPE_BOOL:
		res = format_config_bool(buf, कुञ्चिका_2, मानम्_2, gently);
		भङ्ग;

	पक्ष TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, कुञ्चिका_2, मानम्_2, kvi, gently);
		भङ्ग;

	पक्ष TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, मानम्_2);
		भङ्ग;

	पक्ष TYPE_मार्गः:
		res = format_config_मार्गः(buf, कुञ्चिका_2, मानम्_2, gently);
		भङ्ग;

	पक्ष TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, कुञ्चिका_2, मानम्_2, gently);
		भङ्ग;

	पक्ष TYPE_COLOR:
		res = format_config_color(buf, कुञ्चिका_2, मानम्_2, gently);
		भङ्ग;

	पक्ष TYPE_NONE:
		यदि (मानम्_2) {
			strbuf_addstr(buf, मानम्_2);
		} अन्यथा {
			/* Just show the key name; back out delimiter */
			यदि (opts->show_कुञ्चिकाः)
				strbuf_setlen(buf, buf->len - 1);
		}
		भङ्ग;

	स्वभाव:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->पदांशः);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क show_all_config(ध्रुव वर्ण *कुञ्चिका_2, ध्रुव वर्ण *मानम्_2,
			   ध्रुव संरचना config_context *ctx,
			   शून्य *cb)
{
	ध्रुव संरचना config_display_options *opts = cb;
	ध्रुव संरचना कुञ्चिका_मानम्_info *kvi = ctx->kvi;
	संरचना strbuf formatted = STRBUF_INIT;

	यदि (format_config(opts, &formatted, कुञ्चिका_2, मानम्_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	प्रत्यागम 0;
}

#परिभाषय GET_मानम्_ALL        (1 << 0)
#परिभाषय GET_मानम्_कुञ्चिका_REGEXP (1 << 1)

संरचना collect_config_दत्त {
	ध्रुव संरचना config_display_options *display_opts;
	संरचना strbuf_list *मानानि;
	ध्रुव वर्ण *मानम्_आदर्शः;
	ध्रुव वर्ण *कुञ्चिका;
	regex_t *regexp;
	regex_t *कुञ्चिका_regexp;
	पूर्णाङ्क do_not_मेलनम्;
	अचिह्नित get_मानम्_flags;
	अचिह्नित flags;
};

स्थिर पूर्णाङ्क collect_config(ध्रुव वर्ण *कुञ्चिका_2, ध्रुव वर्ण *मानम्_2,
			  ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना collect_config_दत्त *दत्त = cb;
	संरचना strbuf_list *मानानि = दत्त->मानानि;
	ध्रुव संरचना कुञ्चिका_मानम्_info *kvi = ctx->kvi;
	पूर्णाङ्क स्थितिः;

	यदि (!(दत्त->get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) &&
	    वाक्यं_तुलय(कुञ्चिका_2, दत्त->कुञ्चिका))
		प्रत्यागम 0;
	यदि ((दत्त->get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) &&
	    regexec(दत्त->कुञ्चिका_regexp, कुञ्चिका_2, 0, रिक्त, 0))
		प्रत्यागम 0;
	यदि ((दत्त->flags & CONFIG_FLAGS_FIXED_मानम्) &&
	    वाक्यं_तुलय(दत्त->मानम्_आदर्शः, (मानम्_2?मानम्_2:"")))
		प्रत्यागम 0;
	यदि (दत्त->regexp &&
	    (दत्त->do_not_मेलनम् ^ !!regexec(दत्त->regexp, (मानम्_2?मानम्_2:""), 0, रिक्त, 0)))
		प्रत्यागम 0;

	ALLOC_GROW(मानानि->वस्तूनि, मानानि->nr + 1, मानानि->alloc);
	strbuf_init(&मानानि->वस्तूनि[मानानि->nr], 0);

	स्थितिः = format_config(दत्त->display_opts, &मानानि->वस्तूनि[मानानि->nr++],
			       कुञ्चिका_2, मानम्_2, kvi, 0);
	यदि (स्थितिः < 0)
		प्रत्यागम स्थितिः;
	यदि (स्थितिः) {
		strbuf_release(&मानानि->वस्तूनि[--मानानि->nr]);
		स्थितिः = 0;
	}
	प्रत्यागम स्थितिः;
}

स्थिर पूर्णाङ्क get_मानम्(ध्रुव संरचना config_location_options *opts,
		     ध्रुव संरचना config_display_options *display_opts,
		     ध्रुव वर्ण *कुञ्चिका_2, ध्रुव वर्ण *regex_,
		     अचिह्नित get_मानम्_flags, अचिह्नित flags)
{
	पूर्णाङ्क ret = CONFIG_GENERIC_ERROR;
	संरचना strbuf_list मानानि = {रिक्त};
	संरचना collect_config_दत्त दत्त = {
		.display_opts = display_opts,
		.मानानि = &मानानि,
		.get_मानम्_flags = get_मानम्_flags,
		.flags = flags,
	};
	वर्ण *कुञ्चिका = रिक्त;
	पूर्णाङ्क i;

	यदि (get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) {
		वर्ण *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		कुञ्चिका = xstrdup(कुञ्चिका_2);
		प्रति (tl = कुञ्चिका + वाक्यदैर्घ्यम्(कुञ्चिका) - 1;
		     tl >= कुञ्चिका && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		प्रति (tl = कुञ्चिका; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		दत्त.कुञ्चिका_regexp = (regex_t*)xmalloc(परिमाण(regex_t));
		यदि (regcomp(दत्त.कुञ्चिका_regexp, कुञ्चिका, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), कुञ्चिका_2);
			FREE_AND_NULL(दत्त.कुञ्चिका_regexp);
			ret = CONFIG_INVALID_आदर्शः;
			गच्छ free_वाक्यानि;
		}
	} अन्यथा {
		यदि (git_config_parse_कुञ्चिका(कुञ्चिका_2, &कुञ्चिका, रिक्त)) {
			ret = CONFIG_INVALID_कुञ्चिका;
			गच्छ free_वाक्यानि;
		}

		दत्त.कुञ्चिका = कुञ्चिका;
	}

	यदि (regex_ && (flags & CONFIG_FLAGS_FIXED_मानम्))
		दत्त.मानम्_आदर्शः = regex_;
	अन्यथा यदि (regex_) {
		यदि (regex_[0] == '!') {
			दत्त.do_not_मेलनम् = 1;
			regex_++;
		}

		दत्त.regexp = (regex_t*)xmalloc(परिमाण(regex_t));
		यदि (regcomp(दत्त.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(दत्त.regexp);
			ret = CONFIG_INVALID_आदर्शः;
			गच्छ free_वाक्यानि;
		}
	}

	config_with_options(collect_config, &दत्त,
			    &opts->source, the_repository,
			    &opts->options);

	यदि (!मानानि.nr && display_opts->default_मानम्) {
		संरचना कुञ्चिका_मानम्_info kvi = KVI_INIT;
		संरचना strbuf *item;
		पूर्णाङ्क स्थितिः;

		kvi_from_param(&kvi);
		ALLOC_GROW(मानानि.वस्तूनि, मानानि.nr + 1, मानानि.alloc);
		item = &मानानि.वस्तूनि[मानानि.nr++];
		strbuf_init(item, 0);

		स्थितिः = format_config(display_opts, item, कुञ्चिका_2,
				       display_opts->default_मानम्, &kvi, 0);
		यदि (स्थितिः < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_मानम्);
		यदि (स्थितिः) {
			/* default was a missing optional value */
			मानानि.nr--;
			strbuf_release(item);
		}
	}

	ret = !मानानि.nr;

	प्रति (i = 0; i < मानानि.nr; i++) {
		संरचना strbuf *buf = मानानि.वस्तूनि + i;
		यदि ((get_मानम्_flags & GET_मानम्_ALL) || i == मानानि.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	मुक्त(मानानि.वस्तूनि);

free_वाक्यानि:
	मुक्त(कुञ्चिका);
	यदि (दत्त.कुञ्चिका_regexp) {
		regfree(दत्त.कुञ्चिका_regexp);
		मुक्त(दत्त.कुञ्चिका_regexp);
	}
	यदि (दत्त.regexp) {
		regfree(दत्त.regexp);
		मुक्त(दत्त.regexp);
	}

	प्रत्यागम ret;
}

स्थिर वर्ण *normalize_मानम्(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्,
			     पूर्णाङ्क type, संरचना कुञ्चिका_मानम्_info *kvi)
{
	यदि (!मानम्)
		प्रत्यागम रिक्त;

	यदि (type == 0 || type == TYPE_मार्गः || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		प्रत्यागम xstrdup(मानम्);
	यदि (type == TYPE_INT)
		प्रत्यागम xstrfmt("%"PRId64, git_config_int64(कुञ्चिका, मानम्, kvi));
	यदि (type == TYPE_BOOL)
		प्रत्यागम xstrdup(git_config_bool(कुञ्चिका, मानम्) ?  "true" : "false");
	यदि (type == TYPE_BOOL_OR_INT) {
		पूर्णाङ्क is_bool, v;
		v = git_config_bool_or_int(कुञ्चिका, मानम्, kvi, &is_bool);
		यदि (!is_bool)
			प्रत्यागम xstrfmt("%d", v);
		अन्यथा
			प्रत्यागम xstrdup(v ? "true" : "false");
	}
	यदि (type == TYPE_BOOL_OR_STR) {
		पूर्णाङ्क v = git_parse_maybe_bool(मानम्);
		यदि (v < 0)
			प्रत्यागम xstrdup(मानम्);
		अन्यथा
			प्रत्यागम xstrdup(v ? "true" : "false");
	}
	यदि (type == TYPE_COLOR) {
		वर्ण v[COLOR_MAXLEN];
		यदि (git_config_color(v, कुञ्चिका, मानम्))
			die(_("cannot parse color '%s'"), मानम्);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		प्रत्यागम xstrdup(मानम्);
	}

	BUG("cannot normalize type %d", type);
}

संरचना get_color_config_दत्त {
	पूर्णाङ्क get_color_प्राप्तम्;
	ध्रुव वर्ण *get_color_slot;
	वर्ण parsed_color[COLOR_MAXLEN];
};

स्थिर पूर्णाङ्क git_get_color_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				ध्रुव संरचना config_context *ctx अप्रयुक्तम्,
				शून्य *cb)
{
	संरचना get_color_config_दत्त *दत्त = cb;

	यदि (!वाक्यं_तुलय(var, दत्त->get_color_slot)) {
		यदि (!मानम्)
			config_error_nonbool(var);
		यदि (color_parse(मानम्, दत्त->parsed_color) < 0)
			प्रत्यागम -1;
		दत्त->get_color_प्राप्तम् = 1;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_color(ध्रुव संरचना config_location_options *opts,
		      ध्रुव वर्ण *var, ध्रुव वर्ण *def_color)
{
	संरचना get_color_config_दत्त दत्त = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	पूर्णाङ्क ret;

	config_with_options(git_get_color_config, &दत्त,
			    &opts->source, the_repository,
			    &opts->options);

	यदि (!दत्त.get_color_प्राप्तम् && def_color) {
		यदि (color_parse(def_color, दत्त.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			गच्छ बहिर्गमनम्;
		}
	}

	ret = 0;

बहिर्गमनम्:
	fputs(दत्त.parsed_color, stdout);
	प्रत्यागम ret;
}

संरचना get_colorbool_config_दत्त {
	गणना git_colorbool get_colorbool_प्राप्तम्;
	गणना git_colorbool get_diff_color_प्राप्तम्;
	गणना git_colorbool get_color_ui_प्राप्तम्;
	ध्रुव वर्ण *get_colorbool_slot;
};

स्थिर पूर्णाङ्क git_get_colorbool_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				    ध्रुव संरचना config_context *ctx अप्रयुक्तम्,
				    शून्य *cb)
{
	संरचना get_colorbool_config_दत्त *दत्त = cb;

	यदि (!वाक्यं_तुलय(var, दत्त->get_colorbool_slot))
		दत्त->get_colorbool_प्राप्तम् = git_config_colorbool(var, मानम्);
	अन्यथा यदि (!वाक्यं_तुलय(var, "diff.color"))
		दत्त->get_diff_color_प्राप्तम् = git_config_colorbool(var, मानम्);
	अन्यथा यदि (!वाक्यं_तुलय(var, "color.ui"))
		दत्त->get_color_ui_प्राप्तम् = git_config_colorbool(var, मानम्);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_colorbool(ध्रुव संरचना config_location_options *opts,
			 ध्रुव वर्ण *var, पूर्णाङ्क print)
{
	संरचना get_colorbool_config_दत्त दत्त = {
		.get_colorbool_slot = var,
		.get_colorbool_प्राप्तम् = GIT_COLOR_UNKNOWN,
		.get_diff_color_प्राप्तम् = GIT_COLOR_UNKNOWN,
		.get_color_ui_प्राप्तम् = GIT_COLOR_UNKNOWN,
	};
	bool परिणाम;

	config_with_options(git_get_colorbool_config, &दत्त,
			    &opts->source, the_repository,
			    &opts->options);

	यदि (दत्त.get_colorbool_प्राप्तम् == GIT_COLOR_UNKNOWN) {
		यदि (!वाक्यं_तुलय(दत्त.get_colorbool_slot, "color.diff"))
			दत्त.get_colorbool_प्राप्तम् = दत्त.get_diff_color_प्राप्तम्;
		यदि (दत्त.get_colorbool_प्राप्तम् == GIT_COLOR_UNKNOWN)
			दत्त.get_colorbool_प्राप्तम् = दत्त.get_color_ui_प्राप्तम्;
	}

	यदि (दत्त.get_colorbool_प्राप्तम् == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		दत्त.get_colorbool_प्राप्तम् = GIT_COLOR_AUTO;

	परिणाम = want_color(दत्त.get_colorbool_प्राप्तम्);

	यदि (print) {
		प्रारूप_लिख("%s\n", परिणाम ? "true" : "false");
		प्रत्यागम 0;
	} अन्यथा
		प्रत्यागम परिणाम ? 0 : 1;
}

स्थिर शून्य check_write(ध्रुव संरचना git_config_source *source)
{
	यदि (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	यदि (source->use_stdin)
		die(_("writing to stdin is not supported"));

	यदि (source->blob)
		die(_("writing config blobs is not supported"));
}

संरचना urlmatch_current_candidate_मानम् {
	वर्ण मानम्_is_null;
	संरचना strbuf मानम्;
	संरचना कुञ्चिका_मानम्_info kvi;
};

स्थिर पूर्णाङ्क urlmatch_collect_fn(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			       ध्रुव संरचना config_context *ctx,
			       शून्य *cb)
{
	संरचना string_list *मानानि = cb;
	संरचना string_list_item *item = string_list_निवेशय(मानानि, var);
	संरचना urlmatch_current_candidate_मानम् *matched = item->util;
	ध्रुव संरचना कुञ्चिका_मानम्_info *kvi = ctx->kvi;

	यदि (!matched) {
		matched = xmalloc(परिमाण(*matched));
		strbuf_init(&matched->मानम्, 0);
		item->util = matched;
	} अन्यथा {
		strbuf_reset(&matched->मानम्);
	}
	matched->kvi = *kvi;

	यदि (मानम्) {
		strbuf_addstr(&matched->मानम्, मानम्);
		matched->मानम्_is_null = 0;
	} अन्यथा {
		matched->मानम्_is_null = 1;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_urlmatch(ध्रुव संरचना config_location_options *opts,
			ध्रुव संरचना config_display_options *_display_opts,
			ध्रुव वर्ण *var, ध्रुव वर्ण *url)
{
	पूर्णाङ्क ret;
	वर्ण *section;
	वर्ण *section_पुच्छ;
	संरचना config_display_options display_opts = *_display_opts;
	संरचना string_list_item *item;
	संरचना urlmatch_config config = URLMATCH_CONFIG_INIT;
	संरचना string_list मानानि = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = रिक्त;
	config.cb = &मानानि;

	यदि (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_पुच्छ = strchr(section, '.');
	यदि (section_पुच्छ) {
		*section_पुच्छ = '\0';
		config.कुञ्चिका = section_पुच्छ + 1;
		display_opts.show_कुञ्चिकाः = 0;
	} अन्यथा {
		config.कुञ्चिका = रिक्त;
		display_opts.show_कुञ्चिकाः = 1;
	}

	config_with_options(urlmatch_config_प्रविष्टिः, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !मानानि.nr;

	for_each_string_list_item(item, &मानानि) {
		संरचना urlmatch_current_candidate_मानम् *matched = item->util;
		संरचना strbuf buf = STRBUF_INIT;
		पूर्णाङ्क स्थितिः;

		स्थितिः = format_config(&display_opts, &buf, item->string,
				       matched->मानम्_is_null ? रिक्त : matched->मानम्.buf,
				       &matched->kvi, 0);
		यदि (!स्थितिः)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->मानम्);
	}
	urlmatch_config_release(&config);
	string_list_clear(&मानानि, 1);
	मुक्त(config.url.url);

	मुक्त(section);
	प्रत्यागम ret;
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
	प्रत्यागम strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य location_options_init(संरचना config_location_options *opts,
				  ध्रुव वर्ण *उपसर्गः)
{
	यदि (!opts->source.file)
		opts->source.file = opts->file_गन्तव्यम्_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	यदि (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		निर्गच्छ(129);
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
			!वाक्यं_तुलय(opts->source.file, "-")) {
		opts->source.file = रिक्त;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	यदि (opts->use_global_config) {
		opts->source.file = opts->file_गन्तव्यम्_free = git_global_config();
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
		opts->source.file = opts->file_गन्तव्यम्_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} अन्यथा यदि (opts->use_local_config) {
		opts->source.file = opts->file_गन्तव्यम्_free = repo_git_मार्गः(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} अन्यथा यदि (opts->use_worktree_config) {
		संरचना worktree **worktrees = get_worktrees(the_repository);
		यदि (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_गन्तव्यम्_free =
				repo_git_मार्गः(the_repository, "config.worktree");
		अन्यथा यदि (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		अन्यथा
			opts->source.file = opts->file_गन्तव्यम्_free =
				repo_git_मार्गः(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} अन्यथा यदि (opts->source.file) {
		यदि (!is_absolute_मार्गः(opts->source.file) && उपसर्गः)
			opts->source.file = opts->file_गन्तव्यम्_free =
				उपसर्गः_filename(उपसर्गः, opts->source.file);
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
	मुक्त(opts->file_गन्तव्यम्_free);
}

स्थिर शून्य display_options_init(संरचना config_display_options *opts)
{
	यदि (opts->अन्तः_nul) {
		opts->पदांशः = '\0';
		opts->delim = '\n';
		opts->कुञ्चिका_delim = '\n';
	}
}

स्थिर शून्य display_options_init_list(संरचना config_display_options *opts)
{
	opts->show_कुञ्चिकाः = 1;

	यदि (opts->अन्तः_nul) {
		display_options_init(opts);
	} अन्यथा {
		opts->पदांशः = '\n';
		opts->delim = ' ';
		opts->कुञ्चिका_delim = '=';
	}
}

स्थिर पूर्णाङ्क cmd_config_list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_गणः(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_अन्तः(),
	};

	argc = parse_options(argc, argv, उपसर्गः, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, उपसर्गः);
	display_options_init_list(&display_opts);

	सज्जता_auto_pager("config", 1);

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
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_config_get(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			  संरचना repository *repo अप्रयुक्तम्)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ध्रुव वर्ण *मानम्_आदर्शः = रिक्त, *url = रिक्त;
	पूर्णाङ्क flags = 0;
	अचिह्नित get_मानम्_flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_गणः(N_("Filter options")),
		OPT_BIT(0, "all", &get_मानम्_flags, N_("return all values for multi-valued config options"), GET_मानम्_ALL),
		OPT_BIT(0, "regexp", &get_मानम्_flags, N_("interpret the name as a regular expression"), GET_मानम्_कुञ्चिका_REGEXP),
		OPT_STRING(0, "value", &मानम्_आदर्शः, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_गणः(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_मानम्,
			   N_("value"), N_("use default value when missing entry")),
		OPT_अन्तः(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	यदि ((flags & CONFIG_FLAGS_FIXED_मानम्) && !मानम्_आदर्शः)
		die(_("--fixed-value only applies with 'value-pattern'"));
	यदि (display_opts.default_मानम् &&
	    ((get_मानम्_flags & GET_मानम्_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	यदि (url && ((get_मानम्_flags & GET_मानम्_ALL) ||
		    (get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) ||
		    मानम्_आदर्शः))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, उपसर्गः);
	display_options_init(&display_opts);

	यदि (display_opts.type != TYPE_COLOR)
		सज्जता_auto_pager("config", 1);

	यदि (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	अन्यथा यदि (display_opts.type == TYPE_COLOR && !वाक्यदैर्घ्यम्(argv[0]) && display_opts.default_मानम्)
		ret = get_color(&location_opts, "", display_opts.default_मानम्);
	अन्यथा
		ret = get_मानम्(&location_opts, &display_opts, argv[0], मानम्_आदर्शः,
				get_मानम्_flags, flags);

	location_options_release(&location_opts);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_config_समुच्चयः(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			  संरचना repository *repo अप्रयुक्तम्)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ध्रुव वर्ण *मानम्_आदर्शः = रिक्त, *comment_arg = रिक्त;
	वर्ण *comment = रिक्त;
	पूर्णाङ्क flags = 0, append = 0, type = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_गणः(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मानम्_आदर्शः, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_गणः(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_अन्तः(),
	};
	संरचना कुञ्चिका_मानम्_info default_kvi = KVI_INIT;
	वर्ण *मानम्;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, opts, builtin_config_समुच्चयः_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	यदि (argc == 1)
		die_missing_समुच्चयः_मानम्(argv[0]);
	check_argc(argc, 2, 2);

	यदि ((flags & CONFIG_FLAGS_FIXED_मानम्) && !मानम्_आदर्शः)
		die(_("--fixed-value only applies with --value=<pattern>"));
	यदि (append && मानम्_आदर्शः)
		die(_("--append cannot be used with --value=<pattern>"));
	यदि (append)
		मानम्_आदर्शः = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, उपसर्गः);
	check_write(&location_opts.source);

	मानम् = normalize_मानम्(argv[0], argv[1], type, &default_kvi);

	यदि ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मानम्_आदर्शः) {
		ret = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्, मानम्_आदर्शः,
							      comment, flags);
	} अन्यथा {
		ret = repo_config_समुच्चयः_अन्तर्_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, मानम्);
		यदि (ret == CONFIG_NOTHING_समुच्चयः)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मानम्);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_config_unset(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			    संरचना repository *repo अप्रयुक्तम्)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ध्रुव वर्ण *मानम्_आदर्शः = रिक्त;
	पूर्णाङ्क flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_गणः(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मानम्_आदर्शः, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_अन्तः(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	यदि ((flags & CONFIG_FLAGS_FIXED_मानम्) && !मानम्_आदर्शः)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, उपसर्गः);
	check_write(&location_opts.source);

	यदि ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मानम्_आदर्शः)
		ret = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository, location_opts.source.file,
							      argv[0], रिक्त, मानम्_आदर्शः,
							      रिक्त, flags);
	अन्यथा
		ret = repo_config_समुच्चयः_अन्तर्_file_gently(the_repository, location_opts.source.file, argv[0],
						     रिक्त, रिक्त);

	location_options_release(&location_opts);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_config_rename_section(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				     संरचना repository *repo अप्रयुक्तम्)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_अन्तः(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, उपसर्गः);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_अन्तर्_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;
	अन्यथा यदि (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

बहिर्गमनम्:
	location_options_release(&location_opts);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_config_remove_section(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				     संरचना repository *repo अप्रयुक्तम्)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_अन्तः(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, उपसर्गः);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_अन्तर्_file(the_repository, location_opts.source.file,
						 argv[0], रिक्त);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;
	अन्यथा यदि (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

बहिर्गमनम्:
	location_options_release(&location_opts);
	प्रत्यागम ret;
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
			repo_git_मार्गः(the_repository, "config");
	यदि (opts->use_global_config) {
		पूर्णाङ्क fd = उद्घाटय(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		यदि (fd >= 0) {
			वर्ण *content = default_user_config();
			write_str_अन्तर्_पूर्णम्(fd, content);
			मुक्त(content);
			close(fd);
		}
		अन्यथा यदि (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	यदि (launch_editor(config_file, रिक्त, रिक्त)) {
		मुक्त(config_file);
		प्रत्यागम -1;
	}
	मुक्त(config_file);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_config_edit(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_अन्तः(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, उपसर्गः);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_config_actions(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः)
{
	गणना {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_सङ्कलय = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_समुच्चयः = (1<<11),
		ACTION_समुच्चयः_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ध्रुव वर्ण *comment_arg = रिक्त;
	पूर्णाङ्क actions = 0;
	अचिह्नित flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_गणः(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_सङ्कलय),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_गणः(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_मानम्,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_अन्तः(),
	};
	वर्ण *मानम् = रिक्त, *comment = रिक्त;
	पूर्णाङ्क ret = 0;
	पूर्णाङ्क actions_implicit;
	संरचना कुञ्चिका_मानम्_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, उपसर्गः, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, उपसर्गः);
	display_options_init(&display_opts);

	यदि ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		निर्गच्छ(129);
	}

	actions_implicit = (actions == 0);
	यदि (actions_implicit)
		विकल्प (argc) {
		पक्ष 1: actions = ACTION_GET; भङ्ग;
		पक्ष 2: actions = ACTION_समुच्चयः; भङ्ग;
		पक्ष 3: actions = ACTION_समुच्चयः_ALL; भङ्ग;
		स्वभाव:
			error(_("no action specified"));
			निर्गच्छ(129);
		}
	यदि (actions_implicit && argc == 1) {
		ध्रुव वर्ण *चरमः_dot = strrchr(argv[0], '.');
		यदि (चरमः_dot && strchr(चरमः_dot + 1, '='))
			die_missing_समुच्चयः_मानम्(argv[0]);
	}
	यदि (display_opts.omit_मानानि &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		निर्गच्छ(129);
	}

	यदि (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		निर्गच्छ(129);
	}

	यदि (display_opts.default_मानम् && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		निर्गच्छ(129);
	}

	यदि (comment_arg &&
	    !(actions & (ACTION_सङ्कलय|ACTION_समुच्चयः|ACTION_समुच्चयः_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		निर्गच्छ(129);
	}

	/* check usage of --fixed-value */
	यदि (flags & CONFIG_FLAGS_FIXED_मानम्) {
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
			भङ्ग;

		/* git config <name> <value> <value-pattern> */
		पक्ष ACTION_समुच्चयः_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		पक्ष ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			भङ्ग;

		/* other options don't allow --fixed-value */
		}

		यदि (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			निर्गच्छ(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	यदि (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		सज्जता_auto_pager("config", 1);

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
	अन्यथा यदि (actions == ACTION_समुच्चयः) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_समुच्चयः_अन्तर्_file_gently(the_repository, location_opts.source.file, argv[0], comment, मानम्);
		यदि (ret == CONFIG_NOTHING_समुच्चयः)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	अन्यथा यदि (actions == ACTION_समुच्चयः_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्, argv[2],
							      comment, flags);
	}
	अन्यथा यदि (actions == ACTION_सङ्कलय) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	अन्यथा यदि (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अन्यथा यदि (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_मानम्(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	अन्यथा यदि (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_मानम्(&location_opts, &display_opts, argv[0], argv[1],
				GET_मानम्_ALL, flags);
	}
	अन्यथा यदि (actions == ACTION_GET_REGEXP) {
		display_opts.show_कुञ्चिकाः = 1;
		check_argc(argc, 1, 2);
		ret = get_मानम्(&location_opts, &display_opts, argv[0], argv[1],
				GET_मानम्_ALL|GET_मानम्_कुञ्चिका_REGEXP, flags);
	}
	अन्यथा यदि (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	अन्यथा यदि (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		यदि (argc == 2)
			ret = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository, location_opts.source.file,
								      argv[0], रिक्त, argv[1],
								      रिक्त, flags);
		अन्यथा
			ret = repo_config_समुच्चयः_अन्तर्_file_gently(the_repository, location_opts.source.file,
							     argv[0], रिक्त, रिक्त);
	}
	अन्यथा यदि (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_समुच्चयः_multivar_अन्तर्_file_gently(the_repository, location_opts.source.file,
							      argv[0], रिक्त, argv[1],
							      रिक्त, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अन्यथा यदि (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_अन्तर्_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		यदि (ret < 0)
			गच्छ बहिर्गमनम्;
		अन्यथा यदि (!ret)
			die(_("no such section: %s"), argv[0]);
		अन्यथा
			ret = 0;
	}
	अन्यथा यदि (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_अन्तर्_file(the_repository, location_opts.source.file,
							 argv[0], रिक्त);
		यदि (ret < 0)
			गच्छ बहिर्गमनम्;
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

बहिर्गमनम्:
	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मानम्);
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_config(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo)
{
	parse_opt_subcommand_fn *subcommand = रिक्त;
	संरचना option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_समुच्चयः),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_अन्तः(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, उपसर्गः, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	यदि (subcommand) {
		argc = parse_options(argc, argv, उपसर्गः, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		प्रत्यागम subcommand(argc, argv, उपसर्गः, repo);
	}

	प्रत्यागम cmd_config_actions(argc, argv, उपसर्गः);
}
