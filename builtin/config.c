#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "date.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "ident.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "urlmatch.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "worktree.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ശൂന്യ
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

ഘടന config_location_options {
	ഘടന git_config_source source;
	ഘടന config_options options;
	അക്ഷരം *file_to_free;
	പൂർണ്ണസംഖ്യ use_global_config;
	പൂർണ്ണസംഖ്യ use_system_config;
	പൂർണ്ണസംഖ്യ use_local_config;
	പൂർണ്ണസംഖ്യ use_worktree_config;
	പൂർണ്ണസംഖ്യ respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

എണ്ണൽ config_type {
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
	OPT_CALLBACK_മൂല്യം(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_മൂല്യം(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_മൂല്യം(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_മൂല്യം(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_മൂല്യം(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_മൂല്യം(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_മൂല്യങ്ങൾ, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_താക്കോലുകൾ, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

ഘടന config_display_options {
	പൂർണ്ണസംഖ്യ end_nul;
	പൂർണ്ണസംഖ്യ omit_മൂല്യങ്ങൾ;
	പൂർണ്ണസംഖ്യ show_origin;
	പൂർണ്ണസംഖ്യ show_scope;
	പൂർണ്ണസംഖ്യ show_താക്കോലുകൾ;
	എണ്ണൽ config_type type;
	അക്ഷരം *default_മൂല്യം;
	/* Populated via `display_options_init()`. */
	പൂർണ്ണസംഖ്യ term;
	പൂർണ്ണസംഖ്യ delim;
	പൂർണ്ണസംഖ്യ താക്കോൽ_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.താക്കോൽ_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_മൂല്യം(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.മൂല്യം = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_type(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			     പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ new_type, *to_type;

	എങ്കിൽ (unset) {
		*((പൂർണ്ണസംഖ്യ *) opt->മൂല്യം) = 0;
		തിരികെ 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	എങ്കിൽ (!new_type) {
		എങ്കിൽ (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		അല്ലെങ്കിൽ
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->മൂല്യം;
	എങ്കിൽ (*to_type && *to_type != new_type) {
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

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം check_argc(പൂർണ്ണസംഖ്യ argc, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max)
{
	എങ്കിൽ (argc >= min && argc <= max)
		തിരികെ;
	എങ്കിൽ (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	അല്ലെങ്കിൽ
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

സ്ഥിതം NORETURN ശൂന്യം die_missing_set_മൂല്യം(സ്ഥിരം അക്ഷരം *arg)
{
	സ്ഥിരം അക്ഷരം *last_dot = strrchr(arg, '.');
	സ്ഥിരം അക്ഷരം *eq = last_dot ? strchr(last_dot + 1, '=') : ശൂന്യ;
	അക്ഷരം *prefix = eq ? xstrndup(arg, eq - arg) : ശൂന്യ;

	എങ്കിൽ (prefix && git_config_താക്കോൽ_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} അല്ലെങ്കിൽ എങ്കിൽ (git_config_താക്കോൽ_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} അല്ലെങ്കിൽ {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	സ്വതന്ത്രം(prefix);
	exit(129);
}

സ്ഥിതം ശൂന്യം show_config_origin(സ്ഥിരം ഘടന config_display_options *opts,
			       സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi,
			       ഘടന strbuf *buf)
{
	സ്ഥിരം അക്ഷരം term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	എങ്കിൽ (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	അല്ലെങ്കിൽ
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ശൂന്യ, 0);
	strbuf_addch(buf, term);
}

സ്ഥിതം ശൂന്യം show_config_scope(സ്ഥിരം ഘടന config_display_options *opts,
			      സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi,
			      ഘടന strbuf *buf)
{
	സ്ഥിരം അക്ഷരം term = opts->end_nul ? '\0' : '\t';
	സ്ഥിരം അക്ഷരം *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

ഘടന strbuf_list {
	ഘടന strbuf *ഇനങ്ങൾ;
	പൂർണ്ണസംഖ്യ nr;
	പൂർണ്ണസംഖ്യ alloc;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ format_config_int64(ഘടന strbuf *buf,
			       സ്ഥിരം അക്ഷരം *താക്കോൽ_2,
			       സ്ഥിരം അക്ഷരം *മൂല്യം_2,
			       സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi,
			       പൂർണ്ണസംഖ്യ gently)
{
	int64_t v = 0;
	എങ്കിൽ (gently) {
		എങ്കിൽ (!git_parse_int64(മൂല്യം_2, &v))
			തിരികെ -1;
	} അല്ലെങ്കിൽ {
		/* may die() */
		v = git_config_int64(താക്കോൽ_2, മൂല്യം_2 ? മൂല്യം_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ format_config_bool(ഘടന strbuf *buf,
			      സ്ഥിരം അക്ഷരം *താക്കോൽ_2,
			      സ്ഥിരം അക്ഷരം *മൂല്യം_2,
			      പൂർണ്ണസംഖ്യ gently)
{
	പൂർണ്ണസംഖ്യ v = 0;
	എങ്കിൽ (gently) {
		എങ്കിൽ ((v = git_parse_maybe_bool(മൂല്യം_2)) < 0)
			തിരികെ -1;
	} അല്ലെങ്കിൽ {
		/* may die() */
		v = git_config_bool(താക്കോൽ_2, മൂല്യം_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ format_config_bool_or_int(ഘടന strbuf *buf,
				     സ്ഥിരം അക്ഷരം *താക്കോൽ_2,
				     സ്ഥിരം അക്ഷരം *മൂല്യം_2,
				     സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi,
				     പൂർണ്ണസംഖ്യ gently)
{
	പൂർണ്ണസംഖ്യ v, is_bool = 0;

	എങ്കിൽ (gently) {
		v = git_parse_maybe_bool_text(മൂല്യം_2);

		എങ്കിൽ (v >= 0)
			is_bool = 1;
		അല്ലെങ്കിൽ എങ്കിൽ (!git_parse_int(മൂല്യം_2, &v))
			തിരികെ -1;
	} അല്ലെങ്കിൽ {
		v = git_config_bool_or_int(താക്കോൽ_2, മൂല്യം_2, kvi,
					   &is_bool);
	}

	എങ്കിൽ (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	അല്ലെങ്കിൽ
		strbuf_addf(buf, "%d", v);

	തിരികെ 0;
}

/* This mode is always gentle. */
സ്ഥിതം പൂർണ്ണസംഖ്യ format_config_bool_or_str(ഘടന strbuf *buf,
				     സ്ഥിരം അക്ഷരം *മൂല്യം_2)
{
	പൂർണ്ണസംഖ്യ v = git_parse_maybe_bool(മൂല്യം_2);
	എങ്കിൽ (v < 0)
		strbuf_addstr(buf, മൂല്യം_2);
	അല്ലെങ്കിൽ
		strbuf_addstr(buf, v ? "true" : "false");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ format_config_path(ഘടന strbuf *buf,
			      സ്ഥിരം അക്ഷരം *താക്കോൽ_2,
			      സ്ഥിരം അക്ഷരം *മൂല്യം_2,
			      പൂർണ്ണസംഖ്യ gently)
{
	അക്ഷരം *v;

	എങ്കിൽ (git_config_pathname(&v, താക്കോൽ_2, മൂല്യം_2) < 0)
		തിരികെ -1;

	എങ്കിൽ (v)
		strbuf_addstr(buf, v);
	അല്ലെങ്കിൽ
		തിരികെ gently ? -1 : 1; /* :(optional)no-such-file */

	സ്വതന്ത്രം(v);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ format_config_expiry_date(ഘടന strbuf *buf,
				     സ്ഥിരം അക്ഷരം *താക്കോൽ_2,
				     സ്ഥിരം അക്ഷരം *മൂല്യം_2,
				     പൂർണ്ണസംഖ്യ quietly)
{
	timestamp_t t;
	എങ്കിൽ (quietly) {
		എങ്കിൽ (parse_expiry_date(മൂല്യം_2, &t))
			തിരികെ -1;
	} അല്ലെങ്കിൽ എങ്കിൽ (git_config_expiry_date(&t, താക്കോൽ_2, മൂല്യം_2) < 0) {
		തിരികെ -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ format_config_color(ഘടന strbuf *buf,
			       സ്ഥിരം അക്ഷരം *താക്കോൽ_2,
			       സ്ഥിരം അക്ഷരം *മൂല്യം_2,
			       പൂർണ്ണസംഖ്യ gently)
{
	അക്ഷരം v[COLOR_MAXLEN];

	എങ്കിൽ (gently) {
		എങ്കിൽ (color_parse_quietly(മൂല്യം_2, v) < 0)
			തിരികെ -1;
	} അല്ലെങ്കിൽ എങ്കിൽ (git_config_color(v, താക്കോൽ_2, മൂല്യം_2) < 0) {
		തിരികെ -1;
	}

	strbuf_addstr(buf, v);
	തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ format_config(സ്ഥിരം ഘടന config_display_options *opts,
			 ഘടന strbuf *buf, സ്ഥിരം അക്ഷരം *താക്കോൽ_2,
			 സ്ഥിരം അക്ഷരം *മൂല്യം_2, സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi,
			 പൂർണ്ണസംഖ്യ gently)
{
	പൂർണ്ണസംഖ്യ res = 0;
	എങ്കിൽ (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	എങ്കിൽ (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	എങ്കിൽ (opts->show_താക്കോലുകൾ)
		strbuf_addstr(buf, താക്കോൽ_2);

	എങ്കിൽ (opts->omit_മൂല്യങ്ങൾ)
		പോകുക terminator;

	എങ്കിൽ (opts->show_താക്കോലുകൾ)
		strbuf_addch(buf, opts->താക്കോൽ_delim);

	തിരഞ്ഞെടുപ്പ് (opts->type) {
	അവസ്ഥ TYPE_INT:
		res = format_config_int64(buf, താക്കോൽ_2, മൂല്യം_2, kvi, gently);
		നിർത്തുക;

	അവസ്ഥ TYPE_BOOL:
		res = format_config_bool(buf, താക്കോൽ_2, മൂല്യം_2, gently);
		നിർത്തുക;

	അവസ്ഥ TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, താക്കോൽ_2, മൂല്യം_2, kvi, gently);
		നിർത്തുക;

	അവസ്ഥ TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, മൂല്യം_2);
		നിർത്തുക;

	അവസ്ഥ TYPE_PATH:
		res = format_config_path(buf, താക്കോൽ_2, മൂല്യം_2, gently);
		നിർത്തുക;

	അവസ്ഥ TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, താക്കോൽ_2, മൂല്യം_2, gently);
		നിർത്തുക;

	അവസ്ഥ TYPE_COLOR:
		res = format_config_color(buf, താക്കോൽ_2, മൂല്യം_2, gently);
		നിർത്തുക;

	അവസ്ഥ TYPE_NONE:
		എങ്കിൽ (മൂല്യം_2) {
			strbuf_addstr(buf, മൂല്യം_2);
		} അല്ലെങ്കിൽ {
			/* Just show the key name; back out delimiter */
			എങ്കിൽ (opts->show_താക്കോലുകൾ)
				strbuf_setlen(buf, buf->len - 1);
		}
		നിർത്തുക;

	സ്ഥിരസ്ഥിതി:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_all_config(സ്ഥിരം അക്ഷരം *താക്കോൽ_2, സ്ഥിരം അക്ഷരം *മൂല്യം_2,
			   സ്ഥിരം ഘടന config_context *ctx,
			   ശൂന്യം *cb)
{
	സ്ഥിരം ഘടന config_display_options *opts = cb;
	സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi = ctx->kvi;
	ഘടന strbuf formatted = STRBUF_INIT;

	എങ്കിൽ (format_config(opts, &formatted, താക്കോൽ_2, മൂല്യം_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	തിരികെ 0;
}

#define GET_മൂല്യം_ALL        (1 << 0)
#define GET_മൂല്യം_താക്കോൽ_REGEXP (1 << 1)

ഘടന collect_config_data {
	സ്ഥിരം ഘടന config_display_options *display_opts;
	ഘടന strbuf_list *മൂല്യങ്ങൾ;
	സ്ഥിരം അക്ഷരം *മൂല്യം_pattern;
	സ്ഥിരം അക്ഷരം *താക്കോൽ;
	regex_t *regexp;
	regex_t *താക്കോൽ_regexp;
	പൂർണ്ണസംഖ്യ do_not_match;
	unsigned get_മൂല്യം_flags;
	unsigned flags;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ collect_config(സ്ഥിരം അക്ഷരം *താക്കോൽ_2, സ്ഥിരം അക്ഷരം *മൂല്യം_2,
			  സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന collect_config_data *data = cb;
	ഘടന strbuf_list *മൂല്യങ്ങൾ = data->മൂല്യങ്ങൾ;
	സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi = ctx->kvi;
	പൂർണ്ണസംഖ്യ status;

	എങ്കിൽ (!(data->get_മൂല്യം_flags & GET_മൂല്യം_താക്കോൽ_REGEXP) &&
	    strcmp(താക്കോൽ_2, data->താക്കോൽ))
		തിരികെ 0;
	എങ്കിൽ ((data->get_മൂല്യം_flags & GET_മൂല്യം_താക്കോൽ_REGEXP) &&
	    regexec(data->താക്കോൽ_regexp, താക്കോൽ_2, 0, ശൂന്യ, 0))
		തിരികെ 0;
	എങ്കിൽ ((data->flags & CONFIG_FLAGS_FIXED_മൂല്യം) &&
	    strcmp(data->മൂല്യം_pattern, (മൂല്യം_2?മൂല്യം_2:"")))
		തിരികെ 0;
	എങ്കിൽ (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (മൂല്യം_2?മൂല്യം_2:""), 0, ശൂന്യ, 0)))
		തിരികെ 0;

	ALLOC_GROW(മൂല്യങ്ങൾ->ഇനങ്ങൾ, മൂല്യങ്ങൾ->nr + 1, മൂല്യങ്ങൾ->alloc);
	strbuf_init(&മൂല്യങ്ങൾ->ഇനങ്ങൾ[മൂല്യങ്ങൾ->nr], 0);

	status = format_config(data->display_opts, &മൂല്യങ്ങൾ->ഇനങ്ങൾ[മൂല്യങ്ങൾ->nr++],
			       താക്കോൽ_2, മൂല്യം_2, kvi, 0);
	എങ്കിൽ (status < 0)
		തിരികെ status;
	എങ്കിൽ (status) {
		strbuf_release(&മൂല്യങ്ങൾ->ഇനങ്ങൾ[--മൂല്യങ്ങൾ->nr]);
		status = 0;
	}
	തിരികെ status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_മൂല്യം(സ്ഥിരം ഘടന config_location_options *opts,
		     സ്ഥിരം ഘടന config_display_options *display_opts,
		     സ്ഥിരം അക്ഷരം *താക്കോൽ_2, സ്ഥിരം അക്ഷരം *regex_,
		     unsigned get_മൂല്യം_flags, unsigned flags)
{
	പൂർണ്ണസംഖ്യ ret = CONFIG_GENERIC_ERROR;
	ഘടന strbuf_list മൂല്യങ്ങൾ = {ശൂന്യ};
	ഘടന collect_config_data data = {
		.display_opts = display_opts,
		.മൂല്യങ്ങൾ = &മൂല്യങ്ങൾ,
		.get_മൂല്യം_flags = get_മൂല്യം_flags,
		.flags = flags,
	};
	അക്ഷരം *താക്കോൽ = ശൂന്യ;
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (get_മൂല്യം_flags & GET_മൂല്യം_താക്കോൽ_REGEXP) {
		അക്ഷരം *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		താക്കോൽ = xstrdup(താക്കോൽ_2);
		വേണ്ടി (tl = താക്കോൽ + strlen(താക്കോൽ) - 1;
		     tl >= താക്കോൽ && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		വേണ്ടി (tl = താക്കോൽ; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.താക്കോൽ_regexp = (regex_t*)xmalloc(വലുപ്പം(regex_t));
		എങ്കിൽ (regcomp(data.താക്കോൽ_regexp, താക്കോൽ, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), താക്കോൽ_2);
			FREE_AND_NULL(data.താക്കോൽ_regexp);
			ret = CONFIG_INVALID_PATTERN;
			പോകുക free_strings;
		}
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (git_config_parse_താക്കോൽ(താക്കോൽ_2, &താക്കോൽ, ശൂന്യ)) {
			ret = CONFIG_INVALID_താക്കോൽ;
			പോകുക free_strings;
		}

		data.താക്കോൽ = താക്കോൽ;
	}

	എങ്കിൽ (regex_ && (flags & CONFIG_FLAGS_FIXED_മൂല്യം))
		data.മൂല്യം_pattern = regex_;
	അല്ലെങ്കിൽ എങ്കിൽ (regex_) {
		എങ്കിൽ (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(വലുപ്പം(regex_t));
		എങ്കിൽ (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			പോകുക free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	എങ്കിൽ (!മൂല്യങ്ങൾ.nr && display_opts->default_മൂല്യം) {
		ഘടന താക്കോൽ_മൂല്യം_info kvi = KVI_INIT;
		ഘടന strbuf *item;
		പൂർണ്ണസംഖ്യ status;

		kvi_from_param(&kvi);
		ALLOC_GROW(മൂല്യങ്ങൾ.ഇനങ്ങൾ, മൂല്യങ്ങൾ.nr + 1, മൂല്യങ്ങൾ.alloc);
		item = &മൂല്യങ്ങൾ.ഇനങ്ങൾ[മൂല്യങ്ങൾ.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, താക്കോൽ_2,
				       display_opts->default_മൂല്യം, &kvi, 0);
		എങ്കിൽ (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_മൂല്യം);
		എങ്കിൽ (status) {
			/* default was a missing optional value */
			മൂല്യങ്ങൾ.nr--;
			strbuf_release(item);
		}
	}

	ret = !മൂല്യങ്ങൾ.nr;

	വേണ്ടി (i = 0; i < മൂല്യങ്ങൾ.nr; i++) {
		ഘടന strbuf *buf = മൂല്യങ്ങൾ.ഇനങ്ങൾ + i;
		എങ്കിൽ ((get_മൂല്യം_flags & GET_മൂല്യം_ALL) || i == മൂല്യങ്ങൾ.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	സ്വതന്ത്രം(മൂല്യങ്ങൾ.ഇനങ്ങൾ);

free_strings:
	സ്വതന്ത്രം(താക്കോൽ);
	എങ്കിൽ (data.താക്കോൽ_regexp) {
		regfree(data.താക്കോൽ_regexp);
		സ്വതന്ത്രം(data.താക്കോൽ_regexp);
	}
	എങ്കിൽ (data.regexp) {
		regfree(data.regexp);
		സ്വതന്ത്രം(data.regexp);
	}

	തിരികെ ret;
}

സ്ഥിതം അക്ഷരം *normalize_മൂല്യം(സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *മൂല്യം,
			     പൂർണ്ണസംഖ്യ type, ഘടന താക്കോൽ_മൂല്യം_info *kvi)
{
	എങ്കിൽ (!മൂല്യം)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		തിരികെ xstrdup(മൂല്യം);
	എങ്കിൽ (type == TYPE_INT)
		തിരികെ xstrfmt("%"PRId64, git_config_int64(താക്കോൽ, മൂല്യം, kvi));
	എങ്കിൽ (type == TYPE_BOOL)
		തിരികെ xstrdup(git_config_bool(താക്കോൽ, മൂല്യം) ?  "true" : "false");
	എങ്കിൽ (type == TYPE_BOOL_OR_INT) {
		പൂർണ്ണസംഖ്യ is_bool, v;
		v = git_config_bool_or_int(താക്കോൽ, മൂല്യം, kvi, &is_bool);
		എങ്കിൽ (!is_bool)
			തിരികെ xstrfmt("%d", v);
		അല്ലെങ്കിൽ
			തിരികെ xstrdup(v ? "true" : "false");
	}
	എങ്കിൽ (type == TYPE_BOOL_OR_STR) {
		പൂർണ്ണസംഖ്യ v = git_parse_maybe_bool(മൂല്യം);
		എങ്കിൽ (v < 0)
			തിരികെ xstrdup(മൂല്യം);
		അല്ലെങ്കിൽ
			തിരികെ xstrdup(v ? "true" : "false");
	}
	എങ്കിൽ (type == TYPE_COLOR) {
		അക്ഷരം v[COLOR_MAXLEN];
		എങ്കിൽ (git_config_color(v, താക്കോൽ, മൂല്യം))
			die(_("cannot parse color '%s'"), മൂല്യം);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		തിരികെ xstrdup(മൂല്യം);
	}

	BUG("cannot normalize type %d", type);
}

ഘടന get_color_config_data {
	പൂർണ്ണസംഖ്യ get_color_found;
	സ്ഥിരം അക്ഷരം *get_color_slot;
	അക്ഷരം parsed_color[COLOR_MAXLEN];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ git_get_color_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
				സ്ഥിരം ഘടന config_context *ctx UNUSED,
				ശൂന്യം *cb)
{
	ഘടന get_color_config_data *data = cb;

	എങ്കിൽ (!strcmp(var, data->get_color_slot)) {
		എങ്കിൽ (!മൂല്യം)
			config_error_nonbool(var);
		എങ്കിൽ (color_parse(മൂല്യം, data->parsed_color) < 0)
			തിരികെ -1;
		data->get_color_found = 1;
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_color(സ്ഥിരം ഘടന config_location_options *opts,
		      സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *def_color)
{
	ഘടന get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	പൂർണ്ണസംഖ്യ ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	എങ്കിൽ (!data.get_color_found && def_color) {
		എങ്കിൽ (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			പോകുക out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	തിരികെ ret;
}

ഘടന get_colorbool_config_data {
	എണ്ണൽ git_colorbool get_colorbool_found;
	എണ്ണൽ git_colorbool get_diff_color_found;
	എണ്ണൽ git_colorbool get_color_ui_found;
	സ്ഥിരം അക്ഷരം *get_colorbool_slot;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ git_get_colorbool_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
				    സ്ഥിരം ഘടന config_context *ctx UNUSED,
				    ശൂന്യം *cb)
{
	ഘടന get_colorbool_config_data *data = cb;

	എങ്കിൽ (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, മൂല്യം);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, മൂല്യം);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, മൂല്യം);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_colorbool(സ്ഥിരം ഘടന config_location_options *opts,
			 സ്ഥിരം അക്ഷരം *var, പൂർണ്ണസംഖ്യ print)
{
	ഘടന get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	എങ്കിൽ (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		എങ്കിൽ (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		എങ്കിൽ (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	എങ്കിൽ (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	എങ്കിൽ (print) {
		printf("%s\n", result ? "true" : "false");
		തിരികെ 0;
	} അല്ലെങ്കിൽ
		തിരികെ result ? 0 : 1;
}

സ്ഥിതം ശൂന്യം check_write(സ്ഥിരം ഘടന git_config_source *source)
{
	എങ്കിൽ (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	എങ്കിൽ (source->use_stdin)
		die(_("writing to stdin is not supported"));

	എങ്കിൽ (source->blob)
		die(_("writing config blobs is not supported"));
}

ഘടന urlmatch_current_candidate_മൂല്യം {
	അക്ഷരം മൂല്യം_is_null;
	ഘടന strbuf മൂല്യം;
	ഘടന താക്കോൽ_മൂല്യം_info kvi;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ urlmatch_collect_fn(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			       സ്ഥിരം ഘടന config_context *ctx,
			       ശൂന്യം *cb)
{
	ഘടന string_list *മൂല്യങ്ങൾ = cb;
	ഘടന string_list_item *item = string_list_തിരുകുക(മൂല്യങ്ങൾ, var);
	ഘടന urlmatch_current_candidate_മൂല്യം *matched = item->util;
	സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം_info *kvi = ctx->kvi;

	എങ്കിൽ (!matched) {
		matched = xmalloc(വലുപ്പം(*matched));
		strbuf_init(&matched->മൂല്യം, 0);
		item->util = matched;
	} അല്ലെങ്കിൽ {
		strbuf_reset(&matched->മൂല്യം);
	}
	matched->kvi = *kvi;

	എങ്കിൽ (മൂല്യം) {
		strbuf_addstr(&matched->മൂല്യം, മൂല്യം);
		matched->മൂല്യം_is_null = 0;
	} അല്ലെങ്കിൽ {
		matched->മൂല്യം_is_null = 1;
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_urlmatch(സ്ഥിരം ഘടന config_location_options *opts,
			സ്ഥിരം ഘടന config_display_options *_display_opts,
			സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *url)
{
	പൂർണ്ണസംഖ്യ ret;
	അക്ഷരം *section;
	അക്ഷരം *section_tail;
	ഘടന config_display_options display_opts = *_display_opts;
	ഘടന string_list_item *item;
	ഘടന urlmatch_config config = URLMATCH_CONFIG_INIT;
	ഘടന string_list മൂല്യങ്ങൾ = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ശൂന്യ;
	config.cb = &മൂല്യങ്ങൾ;

	എങ്കിൽ (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	എങ്കിൽ (section_tail) {
		*section_tail = '\0';
		config.താക്കോൽ = section_tail + 1;
		display_opts.show_താക്കോലുകൾ = 0;
	} അല്ലെങ്കിൽ {
		config.താക്കോൽ = ശൂന്യ;
		display_opts.show_താക്കോലുകൾ = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !മൂല്യങ്ങൾ.nr;

	for_each_string_list_item(item, &മൂല്യങ്ങൾ) {
		ഘടന urlmatch_current_candidate_മൂല്യം *matched = item->util;
		ഘടന strbuf buf = STRBUF_INIT;
		പൂർണ്ണസംഖ്യ status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->മൂല്യം_is_null ? ശൂന്യ : matched->മൂല്യം.buf,
				       &matched->kvi, 0);
		എങ്കിൽ (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->മൂല്യം);
	}
	urlmatch_config_release(&config);
	string_list_clear(&മൂല്യങ്ങൾ, 1);
	സ്വതന്ത്രം(config.url.url);

	സ്വതന്ത്രം(section);
	തിരികെ ret;
}

സ്ഥിതം അക്ഷരം *default_user_config(ശൂന്യം)
{
	ഘടന strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	തിരികെ strbuf_detach(&buf, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം location_options_init(ഘടന config_location_options *opts,
				  സ്ഥിരം അക്ഷരം *prefix)
{
	എങ്കിൽ (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	എങ്കിൽ (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	എങ്കിൽ (!startup_info->have_repository) {
		എങ്കിൽ (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		എങ്കിൽ (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		എങ്കിൽ (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	എങ്കിൽ (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = ശൂന്യ;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	എങ്കിൽ (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		എങ്കിൽ (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->use_worktree_config) {
		ഘടന worktree **worktrees = get_worktrees(the_repository);
		എങ്കിൽ (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		അല്ലെങ്കിൽ എങ്കിൽ (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		അല്ലെങ്കിൽ
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->source.file) {
		എങ്കിൽ (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} അല്ലെങ്കിൽ എങ്കിൽ (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	എങ്കിൽ (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	അല്ലെങ്കിൽ
		opts->options.respect_includes = opts->respect_includes_opt;
	എങ്കിൽ (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

സ്ഥിതം ശൂന്യം location_options_release(ഘടന config_location_options *opts)
{
	സ്വതന്ത്രം(opts->file_to_free);
}

സ്ഥിതം ശൂന്യം display_options_init(ഘടന config_display_options *opts)
{
	എങ്കിൽ (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->താക്കോൽ_delim = '\n';
	}
}

സ്ഥിതം ശൂന്യം display_options_init_list(ഘടന config_display_options *opts)
{
	opts->show_താക്കോലുകൾ = 1;

	എങ്കിൽ (opts->end_nul) {
		display_options_init(opts);
	} അല്ലെങ്കിൽ {
		opts->term = '\n';
		opts->delim = ' ';
		opts->താക്കോൽ_delim = '=';
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_list(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ഘടന config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ഘടന option opts[] = {
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

	എങ്കിൽ (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		എങ്കിൽ (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		അല്ലെങ്കിൽ
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_get(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo UNUSED)
{
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ഘടന config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	സ്ഥിരം അക്ഷരം *മൂല്യം_pattern = ശൂന്യ, *url = ശൂന്യ;
	പൂർണ്ണസംഖ്യ flags = 0;
	unsigned get_മൂല്യം_flags = 0;
	ഘടന option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_മൂല്യം_flags, N_("return all values for multi-valued config options"), GET_മൂല്യം_ALL),
		OPT_BIT(0, "regexp", &get_മൂല്യം_flags, N_("interpret the name as a regular expression"), GET_മൂല്യം_താക്കോൽ_REGEXP),
		OPT_STRING(0, "value", &മൂല്യം_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_മൂല്യം),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_മൂല്യം,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	എങ്കിൽ ((flags & CONFIG_FLAGS_FIXED_മൂല്യം) && !മൂല്യം_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	എങ്കിൽ (display_opts.default_മൂല്യം &&
	    ((get_മൂല്യം_flags & GET_മൂല്യം_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	എങ്കിൽ (url && ((get_മൂല്യം_flags & GET_മൂല്യം_ALL) ||
		    (get_മൂല്യം_flags & GET_മൂല്യം_താക്കോൽ_REGEXP) ||
		    മൂല്യം_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	എങ്കിൽ (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	എങ്കിൽ (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	അല്ലെങ്കിൽ എങ്കിൽ (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_മൂല്യം)
		ret = get_color(&location_opts, "", display_opts.default_മൂല്യം);
	അല്ലെങ്കിൽ
		ret = get_മൂല്യം(&location_opts, &display_opts, argv[0], മൂല്യം_pattern,
				get_മൂല്യം_flags, flags);

	location_options_release(&location_opts);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_set(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			  ഘടന repository *repo UNUSED)
{
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	സ്ഥിരം അക്ഷരം *മൂല്യം_pattern = ശൂന്യ, *comment_arg = ശൂന്യ;
	അക്ഷരം *comment = ശൂന്യ;
	പൂർണ്ണസംഖ്യ flags = 0, append = 0, type = 0;
	ഘടന option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &മൂല്യം_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_മൂല്യം),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	ഘടന താക്കോൽ_മൂല്യം_info default_kvi = KVI_INIT;
	അക്ഷരം *മൂല്യം;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	എങ്കിൽ (argc == 1)
		die_missing_set_മൂല്യം(argv[0]);
	check_argc(argc, 2, 2);

	എങ്കിൽ ((flags & CONFIG_FLAGS_FIXED_മൂല്യം) && !മൂല്യം_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	എങ്കിൽ (append && മൂല്യം_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	എങ്കിൽ (append)
		മൂല്യം_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	മൂല്യം = normalize_മൂല്യം(argv[0], argv[1], type, &default_kvi);

	എങ്കിൽ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || മൂല്യം_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], മൂല്യം, മൂല്യം_pattern,
							      comment, flags);
	} അല്ലെങ്കിൽ {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, മൂല്യം);
		എങ്കിൽ (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	സ്വതന്ത്രം(comment);
	സ്വതന്ത്രം(മൂല്യം);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_unset(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			    ഘടന repository *repo UNUSED)
{
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	സ്ഥിരം അക്ഷരം *മൂല്യം_pattern = ശൂന്യ;
	പൂർണ്ണസംഖ്യ flags = 0;
	ഘടന option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &മൂല്യം_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_മൂല്യം),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	എങ്കിൽ ((flags & CONFIG_FLAGS_FIXED_മൂല്യം) && !മൂല്യം_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	എങ്കിൽ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || മൂല്യം_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ശൂന്യ, മൂല്യം_pattern,
							      ശൂന്യ, flags);
	അല്ലെങ്കിൽ
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     ശൂന്യ, ശൂന്യ);

	location_options_release(&location_opts);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_rename_section(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				     ഘടന repository *repo UNUSED)
{
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ഘടന option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	എങ്കിൽ (ret < 0)
		പോകുക out;
	അല്ലെങ്കിൽ എങ്കിൽ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_remove_section(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				     ഘടന repository *repo UNUSED)
{
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ഘടന option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], ശൂന്യ);
	എങ്കിൽ (ret < 0)
		പോകുക out;
	അല്ലെങ്കിൽ എങ്കിൽ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_editor(ഘടന config_location_options *opts)
{
	അക്ഷരം *config_file;

	എങ്കിൽ (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	എങ്കിൽ (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	എങ്കിൽ (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ശൂന്യ);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	എങ്കിൽ (opts->use_global_config) {
		പൂർണ്ണസംഖ്യ fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		എങ്കിൽ (fd >= 0) {
			അക്ഷരം *content = default_user_config();
			write_str_in_full(fd, content);
			സ്വതന്ത്രം(content);
			close(fd);
		}
		അല്ലെങ്കിൽ എങ്കിൽ (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	എങ്കിൽ (launch_editor(config_file, ശൂന്യ, ശൂന്യ)) {
		സ്വതന്ത്രം(config_file);
		തിരികെ -1;
	}
	സ്വതന്ത്രം(config_file);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_edit(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ഘടന option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_config_actions(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix)
{
	എണ്ണൽ {
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
	ഘടന config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ഘടന config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	സ്ഥിരം അക്ഷരം *comment_arg = ശൂന്യ;
	പൂർണ്ണസംഖ്യ actions = 0;
	unsigned flags = 0;
	ഘടന option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_മൂല്യം,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_മൂല്യം),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	അക്ഷരം *മൂല്യം = ശൂന്യ, *comment = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = 0;
	പൂർണ്ണസംഖ്യ actions_implicit;
	ഘടന താക്കോൽ_മൂല്യം_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	എങ്കിൽ ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	എങ്കിൽ (actions_implicit)
		തിരഞ്ഞെടുപ്പ് (argc) {
		അവസ്ഥ 1: actions = ACTION_GET; നിർത്തുക;
		അവസ്ഥ 2: actions = ACTION_SET; നിർത്തുക;
		അവസ്ഥ 3: actions = ACTION_SET_ALL; നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			error(_("no action specified"));
			exit(129);
		}
	എങ്കിൽ (actions_implicit && argc == 1) {
		സ്ഥിരം അക്ഷരം *last_dot = strrchr(argv[0], '.');
		എങ്കിൽ (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_മൂല്യം(argv[0]);
	}
	എങ്കിൽ (display_opts.omit_മൂല്യങ്ങൾ &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	എങ്കിൽ (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	എങ്കിൽ (display_opts.default_മൂല്യം && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	എങ്കിൽ (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	എങ്കിൽ (flags & CONFIG_FLAGS_FIXED_മൂല്യം) {
		പൂർണ്ണസംഖ്യ allowed_usage = 0;

		തിരഞ്ഞെടുപ്പ് (actions) {
		/* git config --get <name> <value-pattern> */
		അവസ്ഥ ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		അവസ്ഥ ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		അവസ്ഥ ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		അവസ്ഥ ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		അവസ്ഥ ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			നിർത്തുക;

		/* git config <name> <value> <value-pattern> */
		അവസ്ഥ ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		അവസ്ഥ ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			നിർത്തുക;

		/* other options don't allow --fixed-value */
		}

		എങ്കിൽ (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	എങ്കിൽ (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	എങ്കിൽ (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		എങ്കിൽ (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			എങ്കിൽ (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			അല്ലെങ്കിൽ
				die(_("error processing config file(s)"));
		}
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		മൂല്യം = normalize_മൂല്യം(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, മൂല്യം);
		എങ്കിൽ (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		മൂല്യം = normalize_മൂല്യം(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], മൂല്യം, argv[2],
							      comment, flags);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		മൂല്യം = normalize_മൂല്യം(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], മൂല്യം,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		മൂല്യം = normalize_മൂല്യം(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], മൂല്യം, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_മൂല്യം(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_മൂല്യം(&location_opts, &display_opts, argv[0], argv[1],
				GET_മൂല്യം_ALL, flags);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_GET_REGEXP) {
		display_opts.show_താക്കോലുകൾ = 1;
		check_argc(argc, 1, 2);
		ret = get_മൂല്യം(&location_opts, &display_opts, argv[0], argv[1],
				GET_മൂല്യം_ALL|GET_മൂല്യം_താക്കോൽ_REGEXP, flags);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		എങ്കിൽ (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], ശൂന്യ, argv[1],
								      ശൂന്യ, flags);
		അല്ലെങ്കിൽ
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], ശൂന്യ, ശൂന്യ);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ശൂന്യ, argv[1],
							      ശൂന്യ, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		എങ്കിൽ (ret < 0)
			പോകുക out;
		അല്ലെങ്കിൽ എങ്കിൽ (!ret)
			die(_("no such section: %s"), argv[0]);
		അല്ലെങ്കിൽ
			ret = 0;
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], ശൂന്യ);
		എങ്കിൽ (ret < 0)
			പോകുക out;
		അല്ലെങ്കിൽ എങ്കിൽ (!ret)
			die(_("no such section: %s"), argv[0]);
		അല്ലെങ്കിൽ
			ret = 0;
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		എങ്കിൽ (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	സ്വതന്ത്രം(comment);
	സ്വതന്ത്രം(മൂല്യം);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_config(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ശൂന്യ;
	ഘടന option subcommand_opts[] = {
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
	എങ്കിൽ (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		തിരികെ subcommand(argc, argv, prefix, repo);
	}

	തിരികെ cmd_config_actions(argc, argv, prefix);
}
