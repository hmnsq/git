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

स्थिर ध्रुव वर्ण *ध्रुव builtin_config_set_usage[] = {
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
	OPT_CALLBACK_मानम्(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_मानम्(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_मानम्(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_मानम्(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_मानम्(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_मानम्(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_मानानि, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_कुञ्चिकाः, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

संरचना config_display_options {
	पूर्णाङ्क end_nul;
	पूर्णाङ्क omit_मानानि;
	पूर्णाङ्क show_origin;
	पूर्णाङ्क show_scope;
	पूर्णाङ्क show_कुञ्चिकाः;
	गणना config_type type;
	वर्ण *default_मानम्;
	/* Populated via `display_options_init()`. */
	पूर्णाङ्क term;
	पूर्णाङ्क delim;
	पूर्णाङ्क कुञ्चिका_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.कुञ्चिका_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_मानम्(s, l, v, h, i) { \
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
	पूर्णाङ्क new_type, *to_type;

	सचे (unset) {
		*((पूर्णाङ्क *) opt->मानम्) = 0;
		पटिनिवत्त 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	सचे (!new_type) {
		सचे (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		अञ्ञथा सचे (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		अञ्ञथा सचे (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		अञ्ञथा सचे (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		अञ्ञथा सचे (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		अञ्ञथा सचे (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		अञ्ञथा सचे (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		अञ्ञथा
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->मानम्;
	सचे (*to_type && *to_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		exit(129);
	}
	*to_type = new_type;

	पटिनिवत्त 0;
}

स्थिर शून्य check_argc(पूर्णाङ्क argc, पूर्णाङ्क min, पूर्णाङ्क max)
{
	सचे (argc >= min && argc <= max)
		पटिनिवत्त;
	सचे (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	अञ्ञथा
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

स्थिर NORETURN शून्य die_missing_set_मानम्(ध्रुव वर्ण *arg)
{
	ध्रुव वर्ण *last_dot = strrchr(arg, '.');
	ध्रुव वर्ण *eq = last_dot ? strchr(last_dot + 1, '=') : रिक्त;
	वर्ण *prefix = eq ? xstrndup(arg, eq - arg) : रिक्त;

	सचे (prefix && git_config_कुञ्चिका_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} अञ्ञथा सचे (git_config_कुञ्चिका_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} अञ्ञथा {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	मुक्त(prefix);
	exit(129);
}

स्थिर शून्य show_config_origin(ध्रुव संरचना config_display_options *opts,
			       ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
			       संरचना strbuf *buf)
{
	ध्रुव वर्ण term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	सचे (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	अञ्ञथा
		quote_c_style(kvi->filename ? kvi->filename : "", buf, रिक्त, 0);
	strbuf_addch(buf, term);
}

स्थिर शून्य show_config_scope(ध्रुव संरचना config_display_options *opts,
			      ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
			      संरचना strbuf *buf)
{
	ध्रुव वर्ण term = opts->end_nul ? '\0' : '\t';
	ध्रुव वर्ण *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
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
	सचे (gently) {
		सचे (!git_parse_int64(मानम्_2, &v))
			पटिनिवत्त -1;
	} अञ्ञथा {
		/* may die() */
		v = git_config_int64(कुञ्चिका_2, मानम्_2 ? मानम्_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क format_config_bool(संरचना strbuf *buf,
			      ध्रुव वर्ण *कुञ्चिका_2,
			      ध्रुव वर्ण *मानम्_2,
			      पूर्णाङ्क gently)
{
	पूर्णाङ्क v = 0;
	सचे (gently) {
		सचे ((v = git_parse_maybe_bool(मानम्_2)) < 0)
			पटिनिवत्त -1;
	} अञ्ञथा {
		/* may die() */
		v = git_config_bool(कुञ्चिका_2, मानम्_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क format_config_bool_or_int(संरचना strbuf *buf,
				     ध्रुव वर्ण *कुञ्चिका_2,
				     ध्रुव वर्ण *मानम्_2,
				     ध्रुव संरचना कुञ्चिका_मानम्_info *kvi,
				     पूर्णाङ्क gently)
{
	पूर्णाङ्क v, is_bool = 0;

	सचे (gently) {
		v = git_parse_maybe_bool_text(मानम्_2);

		सचे (v >= 0)
			is_bool = 1;
		अञ्ञथा सचे (!git_parse_int(मानम्_2, &v))
			पटिनिवत्त -1;
	} अञ्ञथा {
		v = git_config_bool_or_int(कुञ्चिका_2, मानम्_2, kvi,
					   &is_bool);
	}

	सचे (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	अञ्ञथा
		strbuf_addf(buf, "%d", v);

	पटिनिवत्त 0;
}

/* This mode is always gentle. */
स्थिर पूर्णाङ्क format_config_bool_or_str(संरचना strbuf *buf,
				     ध्रुव वर्ण *मानम्_2)
{
	पूर्णाङ्क v = git_parse_maybe_bool(मानम्_2);
	सचे (v < 0)
		strbuf_addstr(buf, मानम्_2);
	अञ्ञथा
		strbuf_addstr(buf, v ? "true" : "false");
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क format_config_path(संरचना strbuf *buf,
			      ध्रुव वर्ण *कुञ्चिका_2,
			      ध्रुव वर्ण *मानम्_2,
			      पूर्णाङ्क gently)
{
	वर्ण *v;

	सचे (git_config_pathname(&v, कुञ्चिका_2, मानम्_2) < 0)
		पटिनिवत्त -1;

	सचे (v)
		strbuf_addstr(buf, v);
	अञ्ञथा
		पटिनिवत्त gently ? -1 : 1; /* :(optional)no-such-file */

	मुक्त(v);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क format_config_expiry_date(संरचना strbuf *buf,
				     ध्रुव वर्ण *कुञ्चिका_2,
				     ध्रुव वर्ण *मानम्_2,
				     पूर्णाङ्क quietly)
{
	timestamp_t t;
	सचे (quietly) {
		सचे (parse_expiry_date(मानम्_2, &t))
			पटिनिवत्त -1;
	} अञ्ञथा सचे (git_config_expiry_date(&t, कुञ्चिका_2, मानम्_2) < 0) {
		पटिनिवत्त -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क format_config_color(संरचना strbuf *buf,
			       ध्रुव वर्ण *कुञ्चिका_2,
			       ध्रुव वर्ण *मानम्_2,
			       पूर्णाङ्क gently)
{
	वर्ण v[COLOR_MAXLEN];

	सचे (gently) {
		सचे (color_parse_quietly(मानम्_2, v) < 0)
			पटिनिवत्त -1;
	} अञ्ञथा सचे (git_config_color(v, कुञ्चिका_2, मानम्_2) < 0) {
		पटिनिवत्त -1;
	}

	strbuf_addstr(buf, v);
	पटिनिवत्त 0;
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
	सचे (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	सचे (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	सचे (opts->show_कुञ्चिकाः)
		strbuf_addstr(buf, कुञ्चिका_2);

	सचे (opts->omit_मानानि)
		गच्छ terminator;

	सचे (opts->show_कुञ्चिकाः)
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

	पक्ष TYPE_PATH:
		res = format_config_path(buf, कुञ्चिका_2, मानम्_2, gently);
		भङ्ग;

	पक्ष TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, कुञ्चिका_2, मानम्_2, gently);
		भङ्ग;

	पक्ष TYPE_COLOR:
		res = format_config_color(buf, कुञ्चिका_2, मानम्_2, gently);
		भङ्ग;

	पक्ष TYPE_NONE:
		सचे (मानम्_2) {
			strbuf_addstr(buf, मानम्_2);
		} अञ्ञथा {
			/* Just show the key name; back out delimiter */
			सचे (opts->show_कुञ्चिकाः)
				strbuf_setlen(buf, buf->len - 1);
		}
		भङ्ग;

	स्वभाव:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	पटिनिवत्त res;
}

स्थिर पूर्णाङ्क show_all_config(ध्रुव वर्ण *कुञ्चिका_2, ध्रुव वर्ण *मानम्_2,
			   ध्रुव संरचना config_context *ctx,
			   शून्य *cb)
{
	ध्रुव संरचना config_display_options *opts = cb;
	ध्रुव संरचना कुञ्चिका_मानम्_info *kvi = ctx->kvi;
	संरचना strbuf formatted = STRBUF_INIT;

	सचे (format_config(opts, &formatted, कुञ्चिका_2, मानम्_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	पटिनिवत्त 0;
}

#define GET_मानम्_ALL        (1 << 0)
#define GET_मानम्_कुञ्चिका_REGEXP (1 << 1)

संरचना collect_config_data {
	ध्रुव संरचना config_display_options *display_opts;
	संरचना strbuf_list *मानानि;
	ध्रुव वर्ण *मानम्_pattern;
	ध्रुव वर्ण *कुञ्चिका;
	regex_t *regexp;
	regex_t *कुञ्चिका_regexp;
	पूर्णाङ्क do_not_match;
	unsigned get_मानम्_flags;
	unsigned flags;
};

स्थिर पूर्णाङ्क collect_config(ध्रुव वर्ण *कुञ्चिका_2, ध्रुव वर्ण *मानम्_2,
			  ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना collect_config_data *data = cb;
	संरचना strbuf_list *मानानि = data->मानानि;
	ध्रुव संरचना कुञ्चिका_मानम्_info *kvi = ctx->kvi;
	पूर्णाङ्क status;

	सचे (!(data->get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) &&
	    strcmp(कुञ्चिका_2, data->कुञ्चिका))
		पटिनिवत्त 0;
	सचे ((data->get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) &&
	    regexec(data->कुञ्चिका_regexp, कुञ्चिका_2, 0, रिक्त, 0))
		पटिनिवत्त 0;
	सचे ((data->flags & CONFIG_FLAGS_FIXED_मानम्) &&
	    strcmp(data->मानम्_pattern, (मानम्_2?मानम्_2:"")))
		पटिनिवत्त 0;
	सचे (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (मानम्_2?मानम्_2:""), 0, रिक्त, 0)))
		पटिनिवत्त 0;

	ALLOC_GROW(मानानि->वस्तूनि, मानानि->nr + 1, मानानि->alloc);
	strbuf_init(&मानानि->वस्तूनि[मानानि->nr], 0);

	status = format_config(data->display_opts, &मानानि->वस्तूनि[मानानि->nr++],
			       कुञ्चिका_2, मानम्_2, kvi, 0);
	सचे (status < 0)
		पटिनिवत्त status;
	सचे (status) {
		strbuf_release(&मानानि->वस्तूनि[--मानानि->nr]);
		status = 0;
	}
	पटिनिवत्त status;
}

स्थिर पूर्णाङ्क get_मानम्(ध्रुव संरचना config_location_options *opts,
		     ध्रुव संरचना config_display_options *display_opts,
		     ध्रुव वर्ण *कुञ्चिका_2, ध्रुव वर्ण *regex_,
		     unsigned get_मानम्_flags, unsigned flags)
{
	पूर्णाङ्क ret = CONFIG_GENERIC_ERROR;
	संरचना strbuf_list मानानि = {रिक्त};
	संरचना collect_config_data data = {
		.display_opts = display_opts,
		.मानानि = &मानानि,
		.get_मानम्_flags = get_मानम्_flags,
		.flags = flags,
	};
	वर्ण *कुञ्चिका = रिक्त;
	पूर्णाङ्क i;

	सचे (get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) {
		वर्ण *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		कुञ्चिका = xstrdup(कुञ्चिका_2);
		अत्थाय (tl = कुञ्चिका + strlen(कुञ्चिका) - 1;
		     tl >= कुञ्चिका && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		अत्थाय (tl = कुञ्चिका; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.कुञ्चिका_regexp = (regex_t*)xmalloc(परिमाण(regex_t));
		सचे (regcomp(data.कुञ्चिका_regexp, कुञ्चिका, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), कुञ्चिका_2);
			FREE_AND_NULL(data.कुञ्चिका_regexp);
			ret = CONFIG_INVALID_PATTERN;
			गच्छ free_strings;
		}
	} अञ्ञथा {
		सचे (git_config_parse_कुञ्चिका(कुञ्चिका_2, &कुञ्चिका, रिक्त)) {
			ret = CONFIG_INVALID_कुञ्चिका;
			गच्छ free_strings;
		}

		data.कुञ्चिका = कुञ्चिका;
	}

	सचे (regex_ && (flags & CONFIG_FLAGS_FIXED_मानम्))
		data.मानम्_pattern = regex_;
	अञ्ञथा सचे (regex_) {
		सचे (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(परिमाण(regex_t));
		सचे (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			गच्छ free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	सचे (!मानानि.nr && display_opts->default_मानम्) {
		संरचना कुञ्चिका_मानम्_info kvi = KVI_INIT;
		संरचना strbuf *item;
		पूर्णाङ्क status;

		kvi_from_param(&kvi);
		ALLOC_GROW(मानानि.वस्तूनि, मानानि.nr + 1, मानानि.alloc);
		item = &मानानि.वस्तूनि[मानानि.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, कुञ्चिका_2,
				       display_opts->default_मानम्, &kvi, 0);
		सचे (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_मानम्);
		सचे (status) {
			/* default was a missing optional value */
			मानानि.nr--;
			strbuf_release(item);
		}
	}

	ret = !मानानि.nr;

	अत्थाय (i = 0; i < मानानि.nr; i++) {
		संरचना strbuf *buf = मानानि.वस्तूनि + i;
		सचे ((get_मानम्_flags & GET_मानम्_ALL) || i == मानानि.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	मुक्त(मानानि.वस्तूनि);

free_strings:
	मुक्त(कुञ्चिका);
	सचे (data.कुञ्चिका_regexp) {
		regfree(data.कुञ्चिका_regexp);
		मुक्त(data.कुञ्चिका_regexp);
	}
	सचे (data.regexp) {
		regfree(data.regexp);
		मुक्त(data.regexp);
	}

	पटिनिवत्त ret;
}

स्थिर वर्ण *normalize_मानम्(ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्,
			     पूर्णाङ्क type, संरचना कुञ्चिका_मानम्_info *kvi)
{
	सचे (!मानम्)
		पटिनिवत्त रिक्त;

	सचे (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		पटिनिवत्त xstrdup(मानम्);
	सचे (type == TYPE_INT)
		पटिनिवत्त xstrfmt("%"PRId64, git_config_int64(कुञ्चिका, मानम्, kvi));
	सचे (type == TYPE_BOOL)
		पटिनिवत्त xstrdup(git_config_bool(कुञ्चिका, मानम्) ?  "true" : "false");
	सचे (type == TYPE_BOOL_OR_INT) {
		पूर्णाङ्क is_bool, v;
		v = git_config_bool_or_int(कुञ्चिका, मानम्, kvi, &is_bool);
		सचे (!is_bool)
			पटिनिवत्त xstrfmt("%d", v);
		अञ्ञथा
			पटिनिवत्त xstrdup(v ? "true" : "false");
	}
	सचे (type == TYPE_BOOL_OR_STR) {
		पूर्णाङ्क v = git_parse_maybe_bool(मानम्);
		सचे (v < 0)
			पटिनिवत्त xstrdup(मानम्);
		अञ्ञथा
			पटिनिवत्त xstrdup(v ? "true" : "false");
	}
	सचे (type == TYPE_COLOR) {
		वर्ण v[COLOR_MAXLEN];
		सचे (git_config_color(v, कुञ्चिका, मानम्))
			die(_("cannot parse color '%s'"), मानम्);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		पटिनिवत्त xstrdup(मानम्);
	}

	BUG("cannot normalize type %d", type);
}

संरचना get_color_config_data {
	पूर्णाङ्क get_color_found;
	ध्रुव वर्ण *get_color_slot;
	वर्ण parsed_color[COLOR_MAXLEN];
};

स्थिर पूर्णाङ्क git_get_color_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				ध्रुव संरचना config_context *ctx UNUSED,
				शून्य *cb)
{
	संरचना get_color_config_data *data = cb;

	सचे (!strcmp(var, data->get_color_slot)) {
		सचे (!मानम्)
			config_error_nonbool(var);
		सचे (color_parse(मानम्, data->parsed_color) < 0)
			पटिनिवत्त -1;
		data->get_color_found = 1;
	}
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क get_color(ध्रुव संरचना config_location_options *opts,
		      ध्रुव वर्ण *var, ध्रुव वर्ण *def_color)
{
	संरचना get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	पूर्णाङ्क ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	सचे (!data.get_color_found && def_color) {
		सचे (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			गच्छ out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	पटिनिवत्त ret;
}

संरचना get_colorbool_config_data {
	गणना git_colorbool get_colorbool_found;
	गणना git_colorbool get_diff_color_found;
	गणना git_colorbool get_color_ui_found;
	ध्रुव वर्ण *get_colorbool_slot;
};

स्थिर पूर्णाङ्क git_get_colorbool_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				    ध्रुव संरचना config_context *ctx UNUSED,
				    शून्य *cb)
{
	संरचना get_colorbool_config_data *data = cb;

	सचे (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, मानम्);
	अञ्ञथा सचे (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, मानम्);
	अञ्ञथा सचे (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, मानम्);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क get_colorbool(ध्रुव संरचना config_location_options *opts,
			 ध्रुव वर्ण *var, पूर्णाङ्क print)
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

	सचे (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		सचे (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		सचे (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	सचे (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	सचे (print) {
		printf("%s\n", result ? "true" : "false");
		पटिनिवत्त 0;
	} अञ्ञथा
		पटिनिवत्त result ? 0 : 1;
}

स्थिर शून्य check_write(ध्रुव संरचना git_config_source *source)
{
	सचे (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	सचे (source->use_stdin)
		die(_("writing to stdin is not supported"));

	सचे (source->blob)
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

	सचे (!matched) {
		matched = xmalloc(परिमाण(*matched));
		strbuf_init(&matched->मानम्, 0);
		item->util = matched;
	} अञ्ञथा {
		strbuf_reset(&matched->मानम्);
	}
	matched->kvi = *kvi;

	सचे (मानम्) {
		strbuf_addstr(&matched->मानम्, मानम्);
		matched->मानम्_is_null = 0;
	} अञ्ञथा {
		matched->मानम्_is_null = 1;
	}
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क get_urlmatch(ध्रुव संरचना config_location_options *opts,
			ध्रुव संरचना config_display_options *_display_opts,
			ध्रुव वर्ण *var, ध्रुव वर्ण *url)
{
	पूर्णाङ्क ret;
	वर्ण *section;
	वर्ण *section_tail;
	संरचना config_display_options display_opts = *_display_opts;
	संरचना string_list_item *item;
	संरचना urlmatch_config config = URLMATCH_CONFIG_INIT;
	संरचना string_list मानानि = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = रिक्त;
	config.cb = &मानानि;

	सचे (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	सचे (section_tail) {
		*section_tail = '\0';
		config.कुञ्चिका = section_tail + 1;
		display_opts.show_कुञ्चिकाः = 0;
	} अञ्ञथा {
		config.कुञ्चिका = रिक्त;
		display_opts.show_कुञ्चिकाः = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !मानानि.nr;

	for_each_string_list_item(item, &मानानि) {
		संरचना urlmatch_current_candidate_मानम् *matched = item->util;
		संरचना strbuf buf = STRBUF_INIT;
		पूर्णाङ्क status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->मानम्_is_null ? रिक्त : matched->मानम्.buf,
				       &matched->kvi, 0);
		सचे (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->मानम्);
	}
	urlmatch_config_release(&config);
	string_list_clear(&मानानि, 1);
	मुक्त(config.url.url);

	मुक्त(section);
	पटिनिवत्त ret;
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
	पटिनिवत्त strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य location_options_init(संरचना config_location_options *opts,
				  ध्रुव वर्ण *prefix)
{
	सचे (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	सचे (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	सचे (!startup_info->have_repository) {
		सचे (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		सचे (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		सचे (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	सचे (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = रिक्त;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	सचे (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		सचे (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} अञ्ञथा सचे (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} अञ्ञथा सचे (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} अञ्ञथा सचे (opts->use_worktree_config) {
		संरचना worktree **worktrees = get_worktrees(the_repository);
		सचे (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		अञ्ञथा सचे (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		अञ्ञथा
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} अञ्ञथा सचे (opts->source.file) {
		सचे (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} अञ्ञथा सचे (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	सचे (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	अञ्ञथा
		opts->options.respect_includes = opts->respect_includes_opt;
	सचे (startup_info->have_repository) {
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
	सचे (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->कुञ्चिका_delim = '\n';
	}
}

स्थिर शून्य display_options_init_list(संरचना config_display_options *opts)
{
	opts->show_कुञ्चिकाः = 1;

	सचे (opts->end_nul) {
		display_options_init(opts);
	} अञ्ञथा {
		opts->term = '\n';
		opts->delim = ' ';
		opts->कुञ्चिका_delim = '=';
	}
}

स्थिर पूर्णाङ्क cmd_config_list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
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

	सचे (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		सचे (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		अञ्ञथा
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क cmd_config_get(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	संरचना config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ध्रुव वर्ण *मानम्_pattern = रिक्त, *url = रिक्त;
	पूर्णाङ्क flags = 0;
	unsigned get_मानम्_flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_मानम्_flags, N_("return all values for multi-valued config options"), GET_मानम्_ALL),
		OPT_BIT(0, "regexp", &get_मानम्_flags, N_("interpret the name as a regular expression"), GET_मानम्_कुञ्चिका_REGEXP),
		OPT_STRING(0, "value", &मानम्_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_मानम्,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	सचे ((flags & CONFIG_FLAGS_FIXED_मानम्) && !मानम्_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	सचे (display_opts.default_मानम् &&
	    ((get_मानम्_flags & GET_मानम्_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	सचे (url && ((get_मानम्_flags & GET_मानम्_ALL) ||
		    (get_मानम्_flags & GET_मानम्_कुञ्चिका_REGEXP) ||
		    मानम्_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	सचे (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	सचे (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	अञ्ञथा सचे (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_मानम्)
		ret = get_color(&location_opts, "", display_opts.default_मानम्);
	अञ्ञथा
		ret = get_मानम्(&location_opts, &display_opts, argv[0], मानम्_pattern,
				get_मानम्_flags, flags);

	location_options_release(&location_opts);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_config_set(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			  संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ध्रुव वर्ण *मानम्_pattern = रिक्त, *comment_arg = रिक्त;
	वर्ण *comment = रिक्त;
	पूर्णाङ्क flags = 0, append = 0, type = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मानम्_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	संरचना कुञ्चिका_मानम्_info default_kvi = KVI_INIT;
	वर्ण *मानम्;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	सचे (argc == 1)
		die_missing_set_मानम्(argv[0]);
	check_argc(argc, 2, 2);

	सचे ((flags & CONFIG_FLAGS_FIXED_मानम्) && !मानम्_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	सचे (append && मानम्_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	सचे (append)
		मानम्_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	मानम् = normalize_मानम्(argv[0], argv[1], type, &default_kvi);

	सचे ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मानम्_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्, मानम्_pattern,
							      comment, flags);
	} अञ्ञथा {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, मानम्);
		सचे (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मानम्);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_config_unset(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			    संरचना repository *repo UNUSED)
{
	संरचना config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ध्रुव वर्ण *मानम्_pattern = रिक्त;
	पूर्णाङ्क flags = 0;
	संरचना option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मानम्_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_END(),
	};
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	सचे ((flags & CONFIG_FLAGS_FIXED_मानम्) && !मानम्_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	सचे ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मानम्_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], रिक्त, मानम्_pattern,
							      रिक्त, flags);
	अञ्ञथा
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     रिक्त, रिक्त);

	location_options_release(&location_opts);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_config_rename_section(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
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
	सचे (ret < 0)
		गच्छ out;
	अञ्ञथा सचे (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_config_remove_section(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
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
	सचे (ret < 0)
		गच्छ out;
	अञ्ञथा सचे (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क show_editor(संरचना config_location_options *opts)
{
	वर्ण *config_file;

	सचे (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	सचे (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	सचे (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, रिक्त);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	सचे (opts->use_global_config) {
		पूर्णाङ्क fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		सचे (fd >= 0) {
			वर्ण *content = default_user_config();
			write_str_in_full(fd, content);
			मुक्त(content);
			close(fd);
		}
		अञ्ञथा सचे (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	सचे (launch_editor(config_file, रिक्त, रिक्त)) {
		मुक्त(config_file);
		पटिनिवत्त -1;
	}
	मुक्त(config_file);

	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क cmd_config_edit(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
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
	पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क cmd_config_actions(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix)
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
	ध्रुव वर्ण *comment_arg = रिक्त;
	पूर्णाङ्क actions = 0;
	unsigned flags = 0;
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
		OPT_STRING(0, "default", &display_opts.default_मानम्,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मानम्),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	वर्ण *मानम् = रिक्त, *comment = रिक्त;
	पूर्णाङ्क ret = 0;
	पूर्णाङ्क actions_implicit;
	संरचना कुञ्चिका_मानम्_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	सचे ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	सचे (actions_implicit)
		विकल्प (argc) {
		पक्ष 1: actions = ACTION_GET; भङ्ग;
		पक्ष 2: actions = ACTION_SET; भङ्ग;
		पक्ष 3: actions = ACTION_SET_ALL; भङ्ग;
		स्वभाव:
			error(_("no action specified"));
			exit(129);
		}
	सचे (actions_implicit && argc == 1) {
		ध्रुव वर्ण *last_dot = strrchr(argv[0], '.');
		सचे (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_मानम्(argv[0]);
	}
	सचे (display_opts.omit_मानानि &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	सचे (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	सचे (display_opts.default_मानम् && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	सचे (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	सचे (flags & CONFIG_FLAGS_FIXED_मानम्) {
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
		पक्ष ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		पक्ष ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			भङ्ग;

		/* other options don't allow --fixed-value */
		}

		सचे (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	सचे (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	सचे (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		सचे (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			सचे (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			अञ्ञथा
				die(_("error processing config file(s)"));
		}
	}
	अञ्ञथा सचे (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	अञ्ञथा सचे (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, मानम्);
		सचे (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	अञ्ञथा सचे (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्, argv[2],
							      comment, flags);
	}
	अञ्ञथा सचे (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	अञ्ञथा सचे (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मानम् = normalize_मानम्(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मानम्, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अञ्ञथा सचे (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_मानम्(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	अञ्ञथा सचे (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_मानम्(&location_opts, &display_opts, argv[0], argv[1],
				GET_मानम्_ALL, flags);
	}
	अञ्ञथा सचे (actions == ACTION_GET_REGEXP) {
		display_opts.show_कुञ्चिकाः = 1;
		check_argc(argc, 1, 2);
		ret = get_मानम्(&location_opts, &display_opts, argv[0], argv[1],
				GET_मानम्_ALL|GET_मानम्_कुञ्चिका_REGEXP, flags);
	}
	अञ्ञथा सचे (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	अञ्ञथा सचे (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		सचे (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], रिक्त, argv[1],
								      रिक्त, flags);
		अञ्ञथा
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], रिक्त, रिक्त);
	}
	अञ्ञथा सचे (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], रिक्त, argv[1],
							      रिक्त, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अञ्ञथा सचे (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		सचे (ret < 0)
			गच्छ out;
		अञ्ञथा सचे (!ret)
			die(_("no such section: %s"), argv[0]);
		अञ्ञथा
			ret = 0;
	}
	अञ्ञथा सचे (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], रिक्त);
		सचे (ret < 0)
			गच्छ out;
		अञ्ञथा सचे (!ret)
			die(_("no such section: %s"), argv[0]);
		अञ्ञथा
			ret = 0;
	}
	अञ्ञथा सचे (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	अञ्ञथा सचे (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		सचे (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मानम्);
	पटिनिवत्त ret;
}

पूर्णाङ्क cmd_config(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
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
	सचे (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		पटिनिवत्त subcommand(argc, argv, prefix, repo);
	}

	पटिनिवत्त cmd_config_actions(argc, argv, prefix);
}
