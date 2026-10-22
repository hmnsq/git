#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ident.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "urlmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_ਸੈੱਟ_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ਨਿਰਾ
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONFIG_LOCATION_OPTIONS(opts) \
	OPT_ਸਮੂਹ(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.ਸਰੋਤ.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.ਸਰੋਤ.blob, N_("blob-id"), N_("read config from given blob object"))

ਬਣਤਰ config_location_options {
	ਬਣਤਰ git_config_ਸਰੋਤ ਸਰੋਤ;
	ਬਣਤਰ config_options options;
	ਅੱਖਰ *file_ਮੰਜ਼ਿਲ_free;
	ਪੂਰਨ use_global_config;
	ਪੂਰਨ use_system_config;
	ਪੂਰਨ use_local_config;
	ਪੂਰਨ use_worktree_config;
	ਪੂਰਨ respect_includes_opt;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

ਗਿਣਤੀ config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_ਰਸਤਾ,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONFIG_TYPE_OPTIONS(type) \
	OPT_ਸਮੂਹ(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_ਮੁੱਲ(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_ਮੁੱਲ(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_ਮੁੱਲ(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_ਮੁੱਲ(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_ਮੁੱਲ(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_ਰਸਤਾ), \
	OPT_CALLBACK_ਮੁੱਲ(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_ਸਮੂਹ(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.ਸਿਰਾ_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_ਮੁੱਲਾਂ, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_ਕੁੰਜੀਆਂ, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

ਬਣਤਰ config_display_options {
	ਪੂਰਨ ਸਿਰਾ_nul;
	ਪੂਰਨ omit_ਮੁੱਲਾਂ;
	ਪੂਰਨ show_origin;
	ਪੂਰਨ show_scope;
	ਪੂਰਨ show_ਕੁੰਜੀਆਂ;
	ਗਿਣਤੀ config_type type;
	ਅੱਖਰ *default_ਮੁੱਲ;
	/* Populated via `display_options_init()`. */
	ਪੂਰਨ ਪਦ;
	ਪੂਰਨ delim;
	ਪੂਰਨ ਕੁੰਜੀ_delim;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONFIG_DISPLAY_OPTIONS_INIT { \
	.ਪਦ = '\n', \
	.delim = '=', \
	.ਕੁੰਜੀ_delim = ' ', \
	.type = TYPE_NONE, \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPT_CALLBACK_ਮੁੱਲ(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.ਮੁੱਲ = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ਸਥਿਰ ਪੂਰਨ option_parse_type(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			     ਪੂਰਨ unset)
{
	ਪੂਰਨ new_type, *ਮੰਜ਼ਿਲ_type;

	ਜੇ (unset) {
		*((ਪੂਰਨ *) opt->ਮੁੱਲ) = 0;
		ਵਾਪਸ 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	ਜੇ (!new_type) {
		ਜੇ (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "path"))
			new_type = TYPE_ਰਸਤਾ;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		ਨਹੀਂਤਾਂ
			die(_("unrecognized --type argument, %s"), arg);
	}

	ਮੰਜ਼ਿਲ_type = opt->ਮੁੱਲ;
	ਜੇ (*ਮੰਜ਼ਿਲ_type && *ਮੰਜ਼ਿਲ_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		ਬਾਹਰ_ਨਿਕਲੋ(129);
	}
	*ਮੰਜ਼ਿਲ_type = new_type;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ check_argc(ਪੂਰਨ argc, ਪੂਰਨ min, ਪੂਰਨ max)
{
	ਜੇ (argc >= min && argc <= max)
		ਵਾਪਸ;
	ਜੇ (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	ਨਹੀਂਤਾਂ
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	ਬਾਹਰ_ਨਿਕਲੋ(129);
}

ਸਥਿਰ NORETURN ਖਾਲੀ die_missing_ਸੈੱਟ_ਮੁੱਲ(ਅਚਲ ਅੱਖਰ *arg)
{
	ਅਚਲ ਅੱਖਰ *ਆਖਰੀ_dot = strrchr(arg, '.');
	ਅਚਲ ਅੱਖਰ *eq = ਆਖਰੀ_dot ? strchr(ਆਖਰੀ_dot + 1, '=') : ਨਿਰਾ;
	ਅੱਖਰ *ਅਗੇਤਰ = eq ? xstrndup(arg, eq - arg) : ਨਿਰਾ;

	ਜੇ (ਅਗੇਤਰ && git_config_ਕੁੰਜੀ_is_valid(ਅਗੇਤਰ)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       ਅਗੇਤਰ, eq + 1);
	} ਨਹੀਂਤਾਂ ਜੇ (git_config_ਕੁੰਜੀ_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} ਨਹੀਂਤਾਂ {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	ਮੁਕਤ(ਅਗੇਤਰ);
	ਬਾਹਰ_ਨਿਕਲੋ(129);
}

ਸਥਿਰ ਖਾਲੀ show_config_origin(ਅਚਲ ਬਣਤਰ config_display_options *opts,
			       ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi,
			       ਬਣਤਰ strbuf *buf)
{
	ਅਚਲ ਅੱਖਰ ਪਦ = opts->ਸਿਰਾ_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	ਜੇ (opts->ਸਿਰਾ_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	ਨਹੀਂਤਾਂ
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ਨਿਰਾ, 0);
	strbuf_addch(buf, ਪਦ);
}

ਸਥਿਰ ਖਾਲੀ show_config_scope(ਅਚਲ ਬਣਤਰ config_display_options *opts,
			      ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi,
			      ਬਣਤਰ strbuf *buf)
{
	ਅਚਲ ਅੱਖਰ ਪਦ = opts->ਸਿਰਾ_nul ? '\0' : '\t';
	ਅਚਲ ਅੱਖਰ *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, ਪਦ);
}

ਬਣਤਰ strbuf_list {
	ਬਣਤਰ strbuf *ਚੀਜ਼ਾਂ;
	ਪੂਰਨ nr;
	ਪੂਰਨ alloc;
};

ਸਥਿਰ ਪੂਰਨ format_config_int64(ਬਣਤਰ strbuf *buf,
			       ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2,
			       ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
			       ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi,
			       ਪੂਰਨ gently)
{
	int64_t v = 0;
	ਜੇ (gently) {
		ਜੇ (!git_parse_int64(ਮੁੱਲ_2, &v))
			ਵਾਪਸ -1;
	} ਨਹੀਂਤਾਂ {
		/* may die() */
		v = git_config_int64(ਕੁੰਜੀ_2, ਮੁੱਲ_2 ? ਮੁੱਲ_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ format_config_bool(ਬਣਤਰ strbuf *buf,
			      ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2,
			      ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
			      ਪੂਰਨ gently)
{
	ਪੂਰਨ v = 0;
	ਜੇ (gently) {
		ਜੇ ((v = git_parse_maybe_bool(ਮੁੱਲ_2)) < 0)
			ਵਾਪਸ -1;
	} ਨਹੀਂਤਾਂ {
		/* may die() */
		v = git_config_bool(ਕੁੰਜੀ_2, ਮੁੱਲ_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ format_config_bool_or_int(ਬਣਤਰ strbuf *buf,
				     ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2,
				     ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
				     ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi,
				     ਪੂਰਨ gently)
{
	ਪੂਰਨ v, is_bool = 0;

	ਜੇ (gently) {
		v = git_parse_maybe_bool_ਪਾਠ(ਮੁੱਲ_2);

		ਜੇ (v >= 0)
			is_bool = 1;
		ਨਹੀਂਤਾਂ ਜੇ (!git_parse_int(ਮੁੱਲ_2, &v))
			ਵਾਪਸ -1;
	} ਨਹੀਂਤਾਂ {
		v = git_config_bool_or_int(ਕੁੰਜੀ_2, ਮੁੱਲ_2, kvi,
					   &is_bool);
	}

	ਜੇ (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	ਨਹੀਂਤਾਂ
		strbuf_addf(buf, "%d", v);

	ਵਾਪਸ 0;
}

/* This mode is always gentle. */
ਸਥਿਰ ਪੂਰਨ format_config_bool_or_str(ਬਣਤਰ strbuf *buf,
				     ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2)
{
	ਪੂਰਨ v = git_parse_maybe_bool(ਮੁੱਲ_2);
	ਜੇ (v < 0)
		strbuf_addstr(buf, ਮੁੱਲ_2);
	ਨਹੀਂਤਾਂ
		strbuf_addstr(buf, v ? "true" : "false");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ format_config_ਰਸਤਾ(ਬਣਤਰ strbuf *buf,
			      ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2,
			      ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
			      ਪੂਰਨ gently)
{
	ਅੱਖਰ *v;

	ਜੇ (git_config_pathname(&v, ਕੁੰਜੀ_2, ਮੁੱਲ_2) < 0)
		ਵਾਪਸ -1;

	ਜੇ (v)
		strbuf_addstr(buf, v);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ gently ? -1 : 1; /* :(optional)no-such-file */

	ਮੁਕਤ(v);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ format_config_expiry_date(ਬਣਤਰ strbuf *buf,
				     ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2,
				     ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
				     ਪੂਰਨ quietly)
{
	timestamp_t t;
	ਜੇ (quietly) {
		ਜੇ (parse_expiry_date(ਮੁੱਲ_2, &t))
			ਵਾਪਸ -1;
	} ਨਹੀਂਤਾਂ ਜੇ (git_config_expiry_date(&t, ਕੁੰਜੀ_2, ਮੁੱਲ_2) < 0) {
		ਵਾਪਸ -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ format_config_color(ਬਣਤਰ strbuf *buf,
			       ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2,
			       ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
			       ਪੂਰਨ gently)
{
	ਅੱਖਰ v[COLOR_MAXLEN];

	ਜੇ (gently) {
		ਜੇ (color_parse_quietly(ਮੁੱਲ_2, v) < 0)
			ਵਾਪਸ -1;
	} ਨਹੀਂਤਾਂ ਜੇ (git_config_color(v, ਕੁੰਜੀ_2, ਮੁੱਲ_2) < 0) {
		ਵਾਪਸ -1;
	}

	strbuf_addstr(buf, v);
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ format_config(ਅਚਲ ਬਣਤਰ config_display_options *opts,
			 ਬਣਤਰ strbuf *buf, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2,
			 ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2, ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi,
			 ਪੂਰਨ gently)
{
	ਪੂਰਨ res = 0;
	ਜੇ (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	ਜੇ (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	ਜੇ (opts->show_ਕੁੰਜੀਆਂ)
		strbuf_addstr(buf, ਕੁੰਜੀ_2);

	ਜੇ (opts->omit_ਮੁੱਲਾਂ)
		ਜਾਓ terminator;

	ਜੇ (opts->show_ਕੁੰਜੀਆਂ)
		strbuf_addch(buf, opts->ਕੁੰਜੀ_delim);

	ਚੋਣ (opts->type) {
	ਹਾਲਤ TYPE_INT:
		res = format_config_int64(buf, ਕੁੰਜੀ_2, ਮੁੱਲ_2, kvi, gently);
		ਰੋਕੋ;

	ਹਾਲਤ TYPE_BOOL:
		res = format_config_bool(buf, ਕੁੰਜੀ_2, ਮੁੱਲ_2, gently);
		ਰੋਕੋ;

	ਹਾਲਤ TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, ਕੁੰਜੀ_2, ਮੁੱਲ_2, kvi, gently);
		ਰੋਕੋ;

	ਹਾਲਤ TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, ਮੁੱਲ_2);
		ਰੋਕੋ;

	ਹਾਲਤ TYPE_ਰਸਤਾ:
		res = format_config_ਰਸਤਾ(buf, ਕੁੰਜੀ_2, ਮੁੱਲ_2, gently);
		ਰੋਕੋ;

	ਹਾਲਤ TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, ਕੁੰਜੀ_2, ਮੁੱਲ_2, gently);
		ਰੋਕੋ;

	ਹਾਲਤ TYPE_COLOR:
		res = format_config_color(buf, ਕੁੰਜੀ_2, ਮੁੱਲ_2, gently);
		ਰੋਕੋ;

	ਹਾਲਤ TYPE_NONE:
		ਜੇ (ਮੁੱਲ_2) {
			strbuf_addstr(buf, ਮੁੱਲ_2);
		} ਨਹੀਂਤਾਂ {
			/* Just show the key name; back out delimiter */
			ਜੇ (opts->show_ਕੁੰਜੀਆਂ)
				strbuf_setlen(buf, buf->len - 1);
		}
		ਰੋਕੋ;

	ਮੂਲ:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->ਪਦ);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ show_all_config(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
			   ਅਚਲ ਬਣਤਰ config_context *ctx,
			   ਖਾਲੀ *cb)
{
	ਅਚਲ ਬਣਤਰ config_display_options *opts = cb;
	ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi = ctx->kvi;
	ਬਣਤਰ strbuf formatted = STRBUF_INIT;

	ਜੇ (format_config(opts, &formatted, ਕੁੰਜੀ_2, ਮੁੱਲ_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_ਮੁੱਲ_ALL        (1 << 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_ਮੁੱਲ_ਕੁੰਜੀ_REGEXP (1 << 1)

ਬਣਤਰ collect_config_ਅੰਕੜੇ {
	ਅਚਲ ਬਣਤਰ config_display_options *display_opts;
	ਬਣਤਰ strbuf_list *ਮੁੱਲਾਂ;
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_ਨਮੂਨਾ;
	ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ;
	regex_t *regexp;
	regex_t *ਕੁੰਜੀ_regexp;
	ਪੂਰਨ do_not_ਮੇਲ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਮੁੱਲ_flags;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags;
};

ਸਥਿਰ ਪੂਰਨ collect_config(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_2,
			  ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ collect_config_ਅੰਕੜੇ *ਅੰਕੜੇ = cb;
	ਬਣਤਰ strbuf_list *ਮੁੱਲਾਂ = ਅੰਕੜੇ->ਮੁੱਲਾਂ;
	ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi = ctx->kvi;
	ਪੂਰਨ ਸਥਿਤੀ;

	ਜੇ (!(ਅੰਕੜੇ->get_ਮੁੱਲ_flags & GET_ਮੁੱਲ_ਕੁੰਜੀ_REGEXP) &&
	    strcmp(ਕੁੰਜੀ_2, ਅੰਕੜੇ->ਕੁੰਜੀ))
		ਵਾਪਸ 0;
	ਜੇ ((ਅੰਕੜੇ->get_ਮੁੱਲ_flags & GET_ਮੁੱਲ_ਕੁੰਜੀ_REGEXP) &&
	    regexec(ਅੰਕੜੇ->ਕੁੰਜੀ_regexp, ਕੁੰਜੀ_2, 0, ਨਿਰਾ, 0))
		ਵਾਪਸ 0;
	ਜੇ ((ਅੰਕੜੇ->flags & CONFIG_FLAGS_FIXED_ਮੁੱਲ) &&
	    strcmp(ਅੰਕੜੇ->ਮੁੱਲ_ਨਮੂਨਾ, (ਮੁੱਲ_2?ਮੁੱਲ_2:"")))
		ਵਾਪਸ 0;
	ਜੇ (ਅੰਕੜੇ->regexp &&
	    (ਅੰਕੜੇ->do_not_ਮੇਲ ^ !!regexec(ਅੰਕੜੇ->regexp, (ਮੁੱਲ_2?ਮੁੱਲ_2:""), 0, ਨਿਰਾ, 0)))
		ਵਾਪਸ 0;

	ALLOC_GROW(ਮੁੱਲਾਂ->ਚੀਜ਼ਾਂ, ਮੁੱਲਾਂ->nr + 1, ਮੁੱਲਾਂ->alloc);
	strbuf_init(&ਮੁੱਲਾਂ->ਚੀਜ਼ਾਂ[ਮੁੱਲਾਂ->nr], 0);

	ਸਥਿਤੀ = format_config(ਅੰਕੜੇ->display_opts, &ਮੁੱਲਾਂ->ਚੀਜ਼ਾਂ[ਮੁੱਲਾਂ->nr++],
			       ਕੁੰਜੀ_2, ਮੁੱਲ_2, kvi, 0);
	ਜੇ (ਸਥਿਤੀ < 0)
		ਵਾਪਸ ਸਥਿਤੀ;
	ਜੇ (ਸਥਿਤੀ) {
		strbuf_release(&ਮੁੱਲਾਂ->ਚੀਜ਼ਾਂ[--ਮੁੱਲਾਂ->nr]);
		ਸਥਿਤੀ = 0;
	}
	ਵਾਪਸ ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ get_ਮੁੱਲ(ਅਚਲ ਬਣਤਰ config_location_options *opts,
		     ਅਚਲ ਬਣਤਰ config_display_options *display_opts,
		     ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ_2, ਅਚਲ ਅੱਖਰ *regex_,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਮੁੱਲ_flags, ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਪੂਰਨ ret = CONFIG_GENERIC_ERROR;
	ਬਣਤਰ strbuf_list ਮੁੱਲਾਂ = {ਨਿਰਾ};
	ਬਣਤਰ collect_config_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.display_opts = display_opts,
		.ਮੁੱਲਾਂ = &ਮੁੱਲਾਂ,
		.get_ਮੁੱਲ_flags = get_ਮੁੱਲ_flags,
		.flags = flags,
	};
	ਅੱਖਰ *ਕੁੰਜੀ = ਨਿਰਾ;
	ਪੂਰਨ i;

	ਜੇ (get_ਮੁੱਲ_flags & GET_ਮੁੱਲ_ਕੁੰਜੀ_REGEXP) {
		ਅੱਖਰ *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		ਕੁੰਜੀ = xstrdup(ਕੁੰਜੀ_2);
		ਲਈ (tl = ਕੁੰਜੀ + ਲੜੀ_ਲੰਬਾਈ(ਕੁੰਜੀ) - 1;
		     tl >= ਕੁੰਜੀ && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		ਲਈ (tl = ਕੁੰਜੀ; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		ਅੰਕੜੇ.ਕੁੰਜੀ_regexp = (regex_t*)xmalloc(ਆਕਾਰ(regex_t));
		ਜੇ (regcomp(ਅੰਕੜੇ.ਕੁੰਜੀ_regexp, ਕੁੰਜੀ, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), ਕੁੰਜੀ_2);
			FREE_AND_NULL(ਅੰਕੜੇ.ਕੁੰਜੀ_regexp);
			ret = CONFIG_INVALID_ਨਮੂਨਾ;
			ਜਾਓ free_ਲੜੀਆਂ;
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (git_config_parse_ਕੁੰਜੀ(ਕੁੰਜੀ_2, &ਕੁੰਜੀ, ਨਿਰਾ)) {
			ret = CONFIG_INVALID_ਕੁੰਜੀ;
			ਜਾਓ free_ਲੜੀਆਂ;
		}

		ਅੰਕੜੇ.ਕੁੰਜੀ = ਕੁੰਜੀ;
	}

	ਜੇ (regex_ && (flags & CONFIG_FLAGS_FIXED_ਮੁੱਲ))
		ਅੰਕੜੇ.ਮੁੱਲ_ਨਮੂਨਾ = regex_;
	ਨਹੀਂਤਾਂ ਜੇ (regex_) {
		ਜੇ (regex_[0] == '!') {
			ਅੰਕੜੇ.do_not_ਮੇਲ = 1;
			regex_++;
		}

		ਅੰਕੜੇ.regexp = (regex_t*)xmalloc(ਆਕਾਰ(regex_t));
		ਜੇ (regcomp(ਅੰਕੜੇ.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(ਅੰਕੜੇ.regexp);
			ret = CONFIG_INVALID_ਨਮੂਨਾ;
			ਜਾਓ free_ਲੜੀਆਂ;
		}
	}

	config_with_options(collect_config, &ਅੰਕੜੇ,
			    &opts->ਸਰੋਤ, the_repository,
			    &opts->options);

	ਜੇ (!ਮੁੱਲਾਂ.nr && display_opts->default_ਮੁੱਲ) {
		ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info kvi = KVI_INIT;
		ਬਣਤਰ strbuf *ਵਸਤੂ;
		ਪੂਰਨ ਸਥਿਤੀ;

		kvi_from_param(&kvi);
		ALLOC_GROW(ਮੁੱਲਾਂ.ਚੀਜ਼ਾਂ, ਮੁੱਲਾਂ.nr + 1, ਮੁੱਲਾਂ.alloc);
		ਵਸਤੂ = &ਮੁੱਲਾਂ.ਚੀਜ਼ਾਂ[ਮੁੱਲਾਂ.nr++];
		strbuf_init(ਵਸਤੂ, 0);

		ਸਥਿਤੀ = format_config(display_opts, ਵਸਤੂ, ਕੁੰਜੀ_2,
				       display_opts->default_ਮੁੱਲ, &kvi, 0);
		ਜੇ (ਸਥਿਤੀ < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_ਮੁੱਲ);
		ਜੇ (ਸਥਿਤੀ) {
			/* default was a missing optional value */
			ਮੁੱਲਾਂ.nr--;
			strbuf_release(ਵਸਤੂ);
		}
	}

	ret = !ਮੁੱਲਾਂ.nr;

	ਲਈ (i = 0; i < ਮੁੱਲਾਂ.nr; i++) {
		ਬਣਤਰ strbuf *buf = ਮੁੱਲਾਂ.ਚੀਜ਼ਾਂ + i;
		ਜੇ ((get_ਮੁੱਲ_flags & GET_ਮੁੱਲ_ALL) || i == ਮੁੱਲਾਂ.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	ਮੁਕਤ(ਮੁੱਲਾਂ.ਚੀਜ਼ਾਂ);

free_ਲੜੀਆਂ:
	ਮੁਕਤ(ਕੁੰਜੀ);
	ਜੇ (ਅੰਕੜੇ.ਕੁੰਜੀ_regexp) {
		regfree(ਅੰਕੜੇ.ਕੁੰਜੀ_regexp);
		ਮੁਕਤ(ਅੰਕੜੇ.ਕੁੰਜੀ_regexp);
	}
	ਜੇ (ਅੰਕੜੇ.regexp) {
		regfree(ਅੰਕੜੇ.regexp);
		ਮੁਕਤ(ਅੰਕੜੇ.regexp);
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅੱਖਰ *normalize_ਮੁੱਲ(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			     ਪੂਰਨ type, ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi)
{
	ਜੇ (!ਮੁੱਲ)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (type == 0 || type == TYPE_ਰਸਤਾ || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		ਵਾਪਸ xstrdup(ਮੁੱਲ);
	ਜੇ (type == TYPE_INT)
		ਵਾਪਸ xstrfmt("%"PRId64, git_config_int64(ਕੁੰਜੀ, ਮੁੱਲ, kvi));
	ਜੇ (type == TYPE_BOOL)
		ਵਾਪਸ xstrdup(git_config_bool(ਕੁੰਜੀ, ਮੁੱਲ) ?  "true" : "false");
	ਜੇ (type == TYPE_BOOL_OR_INT) {
		ਪੂਰਨ is_bool, v;
		v = git_config_bool_or_int(ਕੁੰਜੀ, ਮੁੱਲ, kvi, &is_bool);
		ਜੇ (!is_bool)
			ਵਾਪਸ xstrfmt("%d", v);
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ xstrdup(v ? "true" : "false");
	}
	ਜੇ (type == TYPE_BOOL_OR_STR) {
		ਪੂਰਨ v = git_parse_maybe_bool(ਮੁੱਲ);
		ਜੇ (v < 0)
			ਵਾਪਸ xstrdup(ਮੁੱਲ);
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ xstrdup(v ? "true" : "false");
	}
	ਜੇ (type == TYPE_COLOR) {
		ਅੱਖਰ v[COLOR_MAXLEN];
		ਜੇ (git_config_color(v, ਕੁੰਜੀ, ਮੁੱਲ))
			die(_("cannot parse color '%s'"), ਮੁੱਲ);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		ਵਾਪਸ xstrdup(ਮੁੱਲ);
	}

	BUG("cannot normalize type %d", type);
}

ਬਣਤਰ get_color_config_ਅੰਕੜੇ {
	ਪੂਰਨ get_color_ਮਿਲਿਆ;
	ਅਚਲ ਅੱਖਰ *get_color_ਥਾਂ;
	ਅੱਖਰ parsed_color[COLOR_MAXLEN];
};

ਸਥਿਰ ਪੂਰਨ git_get_color_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
				ਅਚਲ ਬਣਤਰ config_context *ctx ਅਣਵਰਤਿਆ,
				ਖਾਲੀ *cb)
{
	ਬਣਤਰ get_color_config_ਅੰਕੜੇ *ਅੰਕੜੇ = cb;

	ਜੇ (!strcmp(var, ਅੰਕੜੇ->get_color_ਥਾਂ)) {
		ਜੇ (!ਮੁੱਲ)
			config_error_nonbool(var);
		ਜੇ (color_parse(ਮੁੱਲ, ਅੰਕੜੇ->parsed_color) < 0)
			ਵਾਪਸ -1;
		ਅੰਕੜੇ->get_color_ਮਿਲਿਆ = 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_color(ਅਚਲ ਬਣਤਰ config_location_options *opts,
		      ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *def_color)
{
	ਬਣਤਰ get_color_config_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.get_color_ਥਾਂ = var,
		.parsed_color[0] = '\0',
	};
	ਪੂਰਨ ret;

	config_with_options(git_get_color_config, &ਅੰਕੜੇ,
			    &opts->ਸਰੋਤ, the_repository,
			    &opts->options);

	ਜੇ (!ਅੰਕੜੇ.get_color_ਮਿਲਿਆ && def_color) {
		ਜੇ (color_parse(def_color, ਅੰਕੜੇ.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	ret = 0;

ਬਾਹਰਲਾ:
	fputs(ਅੰਕੜੇ.parsed_color, stdout);
	ਵਾਪਸ ret;
}

ਬਣਤਰ get_colorbool_config_ਅੰਕੜੇ {
	ਗਿਣਤੀ git_colorbool get_colorbool_ਮਿਲਿਆ;
	ਗਿਣਤੀ git_colorbool get_diff_color_ਮਿਲਿਆ;
	ਗਿਣਤੀ git_colorbool get_color_ui_ਮਿਲਿਆ;
	ਅਚਲ ਅੱਖਰ *get_colorbool_ਥਾਂ;
};

ਸਥਿਰ ਪੂਰਨ git_get_colorbool_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
				    ਅਚਲ ਬਣਤਰ config_context *ctx ਅਣਵਰਤਿਆ,
				    ਖਾਲੀ *cb)
{
	ਬਣਤਰ get_colorbool_config_ਅੰਕੜੇ *ਅੰਕੜੇ = cb;

	ਜੇ (!strcmp(var, ਅੰਕੜੇ->get_colorbool_ਥਾਂ))
		ਅੰਕੜੇ->get_colorbool_ਮਿਲਿਆ = git_config_colorbool(var, ਮੁੱਲ);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(var, "diff.color"))
		ਅੰਕੜੇ->get_diff_color_ਮਿਲਿਆ = git_config_colorbool(var, ਮੁੱਲ);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(var, "color.ui"))
		ਅੰਕੜੇ->get_color_ui_ਮਿਲਿਆ = git_config_colorbool(var, ਮੁੱਲ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_colorbool(ਅਚਲ ਬਣਤਰ config_location_options *opts,
			 ਅਚਲ ਅੱਖਰ *var, ਪੂਰਨ print)
{
	ਬਣਤਰ get_colorbool_config_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.get_colorbool_ਥਾਂ = var,
		.get_colorbool_ਮਿਲਿਆ = GIT_COLOR_UNKNOWN,
		.get_diff_color_ਮਿਲਿਆ = GIT_COLOR_UNKNOWN,
		.get_color_ui_ਮਿਲਿਆ = GIT_COLOR_UNKNOWN,
	};
	bool ਨਤੀਜਾ;

	config_with_options(git_get_colorbool_config, &ਅੰਕੜੇ,
			    &opts->ਸਰੋਤ, the_repository,
			    &opts->options);

	ਜੇ (ਅੰਕੜੇ.get_colorbool_ਮਿਲਿਆ == GIT_COLOR_UNKNOWN) {
		ਜੇ (!strcmp(ਅੰਕੜੇ.get_colorbool_ਥਾਂ, "color.diff"))
			ਅੰਕੜੇ.get_colorbool_ਮਿਲਿਆ = ਅੰਕੜੇ.get_diff_color_ਮਿਲਿਆ;
		ਜੇ (ਅੰਕੜੇ.get_colorbool_ਮਿਲਿਆ == GIT_COLOR_UNKNOWN)
			ਅੰਕੜੇ.get_colorbool_ਮਿਲਿਆ = ਅੰਕੜੇ.get_color_ui_ਮਿਲਿਆ;
	}

	ਜੇ (ਅੰਕੜੇ.get_colorbool_ਮਿਲਿਆ == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		ਅੰਕੜੇ.get_colorbool_ਮਿਲਿਆ = GIT_COLOR_AUTO;

	ਨਤੀਜਾ = want_color(ਅੰਕੜੇ.get_colorbool_ਮਿਲਿਆ);

	ਜੇ (print) {
		ਲਿਖੋ("%s\n", ਨਤੀਜਾ ? "true" : "false");
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ
		ਵਾਪਸ ਨਤੀਜਾ ? 0 : 1;
}

ਸਥਿਰ ਖਾਲੀ check_write(ਅਚਲ ਬਣਤਰ git_config_ਸਰੋਤ *ਸਰੋਤ)
{
	ਜੇ (!ਸਰੋਤ->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	ਜੇ (ਸਰੋਤ->use_stdin)
		die(_("writing to stdin is not supported"));

	ਜੇ (ਸਰੋਤ->blob)
		die(_("writing config blobs is not supported"));
}

ਬਣਤਰ urlmatch_current_candidate_ਮੁੱਲ {
	ਅੱਖਰ ਮੁੱਲ_is_null;
	ਬਣਤਰ strbuf ਮੁੱਲ;
	ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info kvi;
};

ਸਥਿਰ ਪੂਰਨ urlmatch_collect_fn(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			       ਅਚਲ ਬਣਤਰ config_context *ctx,
			       ਖਾਲੀ *cb)
{
	ਬਣਤਰ string_list *ਮੁੱਲਾਂ = cb;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ = string_list_ਪਾਓ(ਮੁੱਲਾਂ, var);
	ਬਣਤਰ urlmatch_current_candidate_ਮੁੱਲ *matched = ਵਸਤੂ->util;
	ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi = ctx->kvi;

	ਜੇ (!matched) {
		matched = xmalloc(ਆਕਾਰ(*matched));
		strbuf_init(&matched->ਮੁੱਲ, 0);
		ਵਸਤੂ->util = matched;
	} ਨਹੀਂਤਾਂ {
		strbuf_reset(&matched->ਮੁੱਲ);
	}
	matched->kvi = *kvi;

	ਜੇ (ਮੁੱਲ) {
		strbuf_addstr(&matched->ਮੁੱਲ, ਮੁੱਲ);
		matched->ਮੁੱਲ_is_null = 0;
	} ਨਹੀਂਤਾਂ {
		matched->ਮੁੱਲ_is_null = 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_urlmatch(ਅਚਲ ਬਣਤਰ config_location_options *opts,
			ਅਚਲ ਬਣਤਰ config_display_options *_display_opts,
			ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *url)
{
	ਪੂਰਨ ret;
	ਅੱਖਰ *section;
	ਅੱਖਰ *section_ਪੂਛ;
	ਬਣਤਰ config_display_options display_opts = *_display_opts;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ urlmatch_config config = URLMATCH_CONFIG_INIT;
	ਬਣਤਰ string_list ਮੁੱਲਾਂ = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ਨਿਰਾ;
	config.cb = &ਮੁੱਲਾਂ;

	ਜੇ (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_ਪੂਛ = strchr(section, '.');
	ਜੇ (section_ਪੂਛ) {
		*section_ਪੂਛ = '\0';
		config.ਕੁੰਜੀ = section_ਪੂਛ + 1;
		display_opts.show_ਕੁੰਜੀਆਂ = 0;
	} ਨਹੀਂਤਾਂ {
		config.ਕੁੰਜੀ = ਨਿਰਾ;
		display_opts.show_ਕੁੰਜੀਆਂ = 1;
	}

	config_with_options(urlmatch_config_ਇੰਦਰਾਜ, &config,
			    &opts->ਸਰੋਤ, the_repository,
			    &opts->options);

	ret = !ਮੁੱਲਾਂ.nr;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਮੁੱਲਾਂ) {
		ਬਣਤਰ urlmatch_current_candidate_ਮੁੱਲ *matched = ਵਸਤੂ->util;
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਪੂਰਨ ਸਥਿਤੀ;

		ਸਥਿਤੀ = format_config(&display_opts, &buf, ਵਸਤੂ->string,
				       matched->ਮੁੱਲ_is_null ? ਨਿਰਾ : matched->ਮੁੱਲ.buf,
				       &matched->kvi, 0);
		ਜੇ (!ਸਥਿਤੀ)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->ਮੁੱਲ);
	}
	urlmatch_config_release(&config);
	string_list_clear(&ਮੁੱਲਾਂ, 1);
	ਮੁਕਤ(config.url.url);

	ਮੁਕਤ(section);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅੱਖਰ *default_user_config(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	ਵਾਪਸ strbuf_detach(&buf, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ location_options_init(ਬਣਤਰ config_location_options *opts,
				  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਜੇ (!opts->ਸਰੋਤ.file)
		opts->ਸਰੋਤ.file = opts->file_ਮੰਜ਼ਿਲ_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	ਜੇ (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->ਸਰੋਤ.file + !!opts->ਸਰੋਤ.blob > 1) {
		error(_("only one config file at a time"));
		ਬਾਹਰ_ਨਿਕਲੋ(129);
	}

	ਜੇ (!startup_info->have_repository) {
		ਜੇ (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		ਜੇ (opts->ਸਰੋਤ.blob)
			die(_("--blob can only be used inside a git repository"));
		ਜੇ (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	ਜੇ (opts->ਸਰੋਤ.file &&
			!strcmp(opts->ਸਰੋਤ.file, "-")) {
		opts->ਸਰੋਤ.file = ਨਿਰਾ;
		opts->ਸਰੋਤ.use_stdin = 1;
		opts->ਸਰੋਤ.scope = CONFIG_SCOPE_COMMAND;
	}

	ਜੇ (opts->use_global_config) {
		opts->ਸਰੋਤ.file = opts->file_ਮੰਜ਼ਿਲ_free = git_global_config();
		ਜੇ (!opts->ਸਰੋਤ.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->ਸਰੋਤ.scope = CONFIG_SCOPE_GLOBAL;
	} ਨਹੀਂਤਾਂ ਜੇ (opts->use_system_config) {
		opts->ਸਰੋਤ.file = opts->file_ਮੰਜ਼ਿਲ_free = git_system_config();
		opts->ਸਰੋਤ.scope = CONFIG_SCOPE_SYSTEM;
	} ਨਹੀਂਤਾਂ ਜੇ (opts->use_local_config) {
		opts->ਸਰੋਤ.file = opts->file_ਮੰਜ਼ਿਲ_free = repo_git_ਰਸਤਾ(the_repository, "config");
		opts->ਸਰੋਤ.scope = CONFIG_SCOPE_LOCAL;
	} ਨਹੀਂਤਾਂ ਜੇ (opts->use_worktree_config) {
		ਬਣਤਰ worktree **worktrees = get_worktrees(the_repository);
		ਜੇ (the_repository->repository_format_worktree_config)
			opts->ਸਰੋਤ.file = opts->file_ਮੰਜ਼ਿਲ_free =
				repo_git_ਰਸਤਾ(the_repository, "config.worktree");
		ਨਹੀਂਤਾਂ ਜੇ (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		ਨਹੀਂਤਾਂ
			opts->ਸਰੋਤ.file = opts->file_ਮੰਜ਼ਿਲ_free =
				repo_git_ਰਸਤਾ(the_repository, "config");
		opts->ਸਰੋਤ.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} ਨਹੀਂਤਾਂ ਜੇ (opts->ਸਰੋਤ.file) {
		ਜੇ (!is_absolute_ਰਸਤਾ(opts->ਸਰੋਤ.file) && ਅਗੇਤਰ)
			opts->ਸਰੋਤ.file = opts->file_ਮੰਜ਼ਿਲ_free =
				ਅਗੇਤਰ_filename(ਅਗੇਤਰ, opts->ਸਰੋਤ.file);
		opts->ਸਰੋਤ.scope = CONFIG_SCOPE_COMMAND;
	} ਨਹੀਂਤਾਂ ਜੇ (opts->ਸਰੋਤ.blob) {
		opts->ਸਰੋਤ.scope = CONFIG_SCOPE_COMMAND;
	}

	ਜੇ (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->ਸਰੋਤ.file;
	ਨਹੀਂਤਾਂ
		opts->options.respect_includes = opts->respect_includes_opt;
	ਜੇ (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ਸਥਿਰ ਖਾਲੀ location_options_release(ਬਣਤਰ config_location_options *opts)
{
	ਮੁਕਤ(opts->file_ਮੰਜ਼ਿਲ_free);
}

ਸਥਿਰ ਖਾਲੀ display_options_init(ਬਣਤਰ config_display_options *opts)
{
	ਜੇ (opts->ਸਿਰਾ_nul) {
		opts->ਪਦ = '\0';
		opts->delim = '\n';
		opts->ਕੁੰਜੀ_delim = '\n';
	}
}

ਸਥਿਰ ਖਾਲੀ display_options_init_list(ਬਣਤਰ config_display_options *opts)
{
	opts->show_ਕੁੰਜੀਆਂ = 1;

	ਜੇ (opts->ਸਿਰਾ_nul) {
		display_options_init(opts);
	} ਨਹੀਂਤਾਂ {
		opts->ਪਦ = '\n';
		opts->delim = ' ';
		opts->ਕੁੰਜੀ_delim = '=';
	}
}

ਸਥਿਰ ਪੂਰਨ cmd_config_list(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਬਣਤਰ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ਸਮੂਹ(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, ਅਗੇਤਰ);
	display_options_init_list(&display_opts);

	ਤਿਆਰੀ_auto_pager("config", 1);

	ਜੇ (config_with_options(show_all_config, &display_opts,
				&location_opts.ਸਰੋਤ, the_repository,
				&location_opts.options) < 0) {
		ਜੇ (location_opts.ਸਰੋਤ.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.ਸਰੋਤ.file);
		ਨਹੀਂਤਾਂ
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_config_get(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਬਣਤਰ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_ਨਮੂਨਾ = ਨਿਰਾ, *url = ਨਿਰਾ;
	ਪੂਰਨ flags = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਮੁੱਲ_flags = 0;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ਸਮੂਹ(N_("Filter options")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "all", &get_ਮੁੱਲ_flags, N_("return all values for multi-valued config options"), GET_ਮੁੱਲ_ALL),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "regexp", &get_ਮੁੱਲ_flags, N_("interpret the name as a regular expression"), GET_ਮੁੱਲ_ਕੁੰਜੀ_REGEXP),
		OPT_STRING(0, "value", &ਮੁੱਲ_ਨਮੂਨਾ, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ਮੁੱਲ),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ਸਮੂਹ(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_ਮੁੱਲ,
			   N_("value"), N_("use default value when missing entry")),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ਜੇ ((flags & CONFIG_FLAGS_FIXED_ਮੁੱਲ) && !ਮੁੱਲ_ਨਮੂਨਾ)
		die(_("--fixed-value only applies with 'value-pattern'"));
	ਜੇ (display_opts.default_ਮੁੱਲ &&
	    ((get_ਮੁੱਲ_flags & GET_ਮੁੱਲ_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	ਜੇ (url && ((get_ਮੁੱਲ_flags & GET_ਮੁੱਲ_ALL) ||
		    (get_ਮੁੱਲ_flags & GET_ਮੁੱਲ_ਕੁੰਜੀ_REGEXP) ||
		    ਮੁੱਲ_ਨਮੂਨਾ))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, ਅਗੇਤਰ);
	display_options_init(&display_opts);

	ਜੇ (display_opts.type != TYPE_COLOR)
		ਤਿਆਰੀ_auto_pager("config", 1);

	ਜੇ (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	ਨਹੀਂਤਾਂ ਜੇ (display_opts.type == TYPE_COLOR && !ਲੜੀ_ਲੰਬਾਈ(argv[0]) && display_opts.default_ਮੁੱਲ)
		ret = get_color(&location_opts, "", display_opts.default_ਮੁੱਲ);
	ਨਹੀਂਤਾਂ
		ret = get_ਮੁੱਲ(&location_opts, &display_opts, argv[0], ਮੁੱਲ_ਨਮੂਨਾ,
				get_ਮੁੱਲ_flags, flags);

	location_options_release(&location_opts);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_config_ਸੈੱਟ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_ਨਮੂਨਾ = ਨਿਰਾ, *comment_arg = ਨਿਰਾ;
	ਅੱਖਰ *comment = ਨਿਰਾ;
	ਪੂਰਨ flags = 0, append = 0, type = 0;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_ਸਮੂਹ(N_("Filter")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ਮੁੱਲ_ਨਮੂਨਾ, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ਮੁੱਲ),
		OPT_ਸਮੂਹ(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info default_kvi = KVI_INIT;
	ਅੱਖਰ *ਮੁੱਲ;
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, builtin_config_ਸੈੱਟ_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ਜੇ (argc == 1)
		die_missing_ਸੈੱਟ_ਮੁੱਲ(argv[0]);
	check_argc(argc, 2, 2);

	ਜੇ ((flags & CONFIG_FLAGS_FIXED_ਮੁੱਲ) && !ਮੁੱਲ_ਨਮੂਨਾ)
		die(_("--fixed-value only applies with --value=<pattern>"));
	ਜੇ (append && ਮੁੱਲ_ਨਮੂਨਾ)
		die(_("--append cannot be used with --value=<pattern>"));
	ਜੇ (append)
		ਮੁੱਲ_ਨਮੂਨਾ = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, ਅਗੇਤਰ);
	check_write(&location_opts.ਸਰੋਤ);

	ਮੁੱਲ = normalize_ਮੁੱਲ(argv[0], argv[1], type, &default_kvi);

	ਜੇ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ਮੁੱਲ_ਨਮੂਨਾ) {
		ret = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
							      argv[0], ਮੁੱਲ, ਮੁੱਲ_ਨਮੂਨਾ,
							      comment, flags);
	} ਨਹੀਂਤਾਂ {
		ret = repo_config_ਸੈੱਟ_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
						     argv[0], comment, ਮੁੱਲ);
		ਜੇ (ret == CONFIG_NOTHING_ਸੈੱਟ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	ਮੁਕਤ(comment);
	ਮੁਕਤ(ਮੁੱਲ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_config_unset(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_ਨਮੂਨਾ = ਨਿਰਾ;
	ਪੂਰਨ flags = 0;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ਸਮੂਹ(N_("Filter")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ਮੁੱਲ_ਨਮੂਨਾ, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ਮੁੱਲ),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ਜੇ ((flags & CONFIG_FLAGS_FIXED_ਮੁੱਲ) && !ਮੁੱਲ_ਨਮੂਨਾ)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, ਅਗੇਤਰ);
	check_write(&location_opts.ਸਰੋਤ);

	ਜੇ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ਮੁੱਲ_ਨਮੂਨਾ)
		ret = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
							      argv[0], ਨਿਰਾ, ਮੁੱਲ_ਨਮੂਨਾ,
							      ਨਿਰਾ, flags);
	ਨਹੀਂਤਾਂ
		ret = repo_config_ਸੈੱਟ_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file, argv[0],
						     ਨਿਰਾ, ਨਿਰਾ);

	location_options_release(&location_opts);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_config_rename_section(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, ਅਗੇਤਰ);
	check_write(&location_opts.ਸਰੋਤ);

	ret = repo_config_rename_section_ਵਿੱਚ_file(the_repository, location_opts.ਸਰੋਤ.file,
						 argv[0], argv[1]);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;
	ਨਹੀਂਤਾਂ ਜੇ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

ਬਾਹਰਲਾ:
	location_options_release(&location_opts);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_config_remove_section(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, ਅਗੇਤਰ);
	check_write(&location_opts.ਸਰੋਤ);

	ret = repo_config_rename_section_ਵਿੱਚ_file(the_repository, location_opts.ਸਰੋਤ.file,
						 argv[0], ਨਿਰਾ);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;
	ਨਹੀਂਤਾਂ ਜੇ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

ਬਾਹਰਲਾ:
	location_options_release(&location_opts);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ show_editor(ਬਣਤਰ config_location_options *opts)
{
	ਅੱਖਰ *config_file;

	ਜੇ (!opts->ਸਰੋਤ.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	ਜੇ (opts->ਸਰੋਤ.use_stdin)
		die(_("editing stdin is not supported"));
	ਜੇ (opts->ਸਰੋਤ.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	config_file = opts->ਸਰੋਤ.file ?
			xstrdup(opts->ਸਰੋਤ.file) :
			repo_git_ਰਸਤਾ(the_repository, "config");
	ਜੇ (opts->use_global_config) {
		ਪੂਰਨ fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		ਜੇ (fd >= 0) {
			ਅੱਖਰ *content = default_user_config();
			write_str_ਵਿੱਚ_ਭਰਿਆ(fd, content);
			ਮੁਕਤ(content);
			close(fd);
		}
		ਨਹੀਂਤਾਂ ਜੇ (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	ਜੇ (launch_editor(config_file, ਨਿਰਾ, ਨਿਰਾ)) {
		ਮੁਕਤ(config_file);
		ਵਾਪਸ -1;
	}
	ਮੁਕਤ(config_file);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_config_edit(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ਸਿਰਾ(),
	};
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, ਅਗੇਤਰ);
	check_write(&location_opts.ਸਰੋਤ);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_config_actions(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਗਿਣਤੀ {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_ਜੋੜੋ = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_ਸੈੱਟ = (1<<11),
		ACTION_ਸੈੱਟ_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	ਬਣਤਰ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ਬਣਤਰ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ਅਚਲ ਅੱਖਰ *comment_arg = ਨਿਰਾ;
	ਪੂਰਨ actions = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;
	ਬਣਤਰ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ਸਮੂਹ(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_ਜੋੜੋ),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ਸਮੂਹ(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_ਮੁੱਲ,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ਮੁੱਲ),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_ਸਿਰਾ(),
	};
	ਅੱਖਰ *ਮੁੱਲ = ਨਿਰਾ, *comment = ਨਿਰਾ;
	ਪੂਰਨ ret = 0;
	ਪੂਰਨ actions_implicit;
	ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, ਅਗੇਤਰ);
	display_options_init(&display_opts);

	ਜੇ ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		ਬਾਹਰ_ਨਿਕਲੋ(129);
	}

	actions_implicit = (actions == 0);
	ਜੇ (actions_implicit)
		ਚੋਣ (argc) {
		ਹਾਲਤ 1: actions = ACTION_GET; ਰੋਕੋ;
		ਹਾਲਤ 2: actions = ACTION_ਸੈੱਟ; ਰੋਕੋ;
		ਹਾਲਤ 3: actions = ACTION_ਸੈੱਟ_ALL; ਰੋਕੋ;
		ਮੂਲ:
			error(_("no action specified"));
			ਬਾਹਰ_ਨਿਕਲੋ(129);
		}
	ਜੇ (actions_implicit && argc == 1) {
		ਅਚਲ ਅੱਖਰ *ਆਖਰੀ_dot = strrchr(argv[0], '.');
		ਜੇ (ਆਖਰੀ_dot && strchr(ਆਖਰੀ_dot + 1, '='))
			die_missing_ਸੈੱਟ_ਮੁੱਲ(argv[0]);
	}
	ਜੇ (display_opts.omit_ਮੁੱਲਾਂ &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		ਬਾਹਰ_ਨਿਕਲੋ(129);
	}

	ਜੇ (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		ਬਾਹਰ_ਨਿਕਲੋ(129);
	}

	ਜੇ (display_opts.default_ਮੁੱਲ && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		ਬਾਹਰ_ਨਿਕਲੋ(129);
	}

	ਜੇ (comment_arg &&
	    !(actions & (ACTION_ਜੋੜੋ|ACTION_ਸੈੱਟ|ACTION_ਸੈੱਟ_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		ਬਾਹਰ_ਨਿਕਲੋ(129);
	}

	/* check usage of --fixed-value */
	ਜੇ (flags & CONFIG_FLAGS_FIXED_ਮੁੱਲ) {
		ਪੂਰਨ allowed_usage = 0;

		ਚੋਣ (actions) {
		/* git config --get <name> <value-pattern> */
		ਹਾਲਤ ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		ਹਾਲਤ ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		ਹਾਲਤ ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		ਹਾਲਤ ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		ਹਾਲਤ ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			ਰੋਕੋ;

		/* git config <name> <value> <value-pattern> */
		ਹਾਲਤ ACTION_ਸੈੱਟ_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		ਹਾਲਤ ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			ਰੋਕੋ;

		/* other options don't allow --fixed-value */
		}

		ਜੇ (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			ਬਾਹਰ_ਨਿਕਲੋ(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	ਜੇ (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		ਤਿਆਰੀ_auto_pager("config", 1);

	ਜੇ (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		ਜੇ (config_with_options(show_all_config, &display_opts,
					&location_opts.ਸਰੋਤ, the_repository,
					&location_opts.options) < 0) {
			ਜੇ (location_opts.ਸਰੋਤ.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.ਸਰੋਤ.file);
			ਨਹੀਂਤਾਂ
				die(_("error processing config file(s)"));
		}
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_ਸੈੱਟ) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 2, 2);
		ਮੁੱਲ = normalize_ਮੁੱਲ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ਸੈੱਟ_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file, argv[0], comment, ਮੁੱਲ);
		ਜੇ (ret == CONFIG_NOTHING_ਸੈੱਟ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_ਸੈੱਟ_ALL) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 2, 3);
		ਮੁੱਲ = normalize_ਮੁੱਲ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
							      argv[0], ਮੁੱਲ, argv[2],
							      comment, flags);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_ਜੋੜੋ) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 2, 2);
		ਮੁੱਲ = normalize_ਮੁੱਲ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
							      argv[0], ਮੁੱਲ,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 2, 3);
		ਮੁੱਲ = normalize_ਮੁੱਲ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
							      argv[0], ਮੁੱਲ, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_ਮੁੱਲ(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_ਮੁੱਲ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ਮੁੱਲ_ALL, flags);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_GET_REGEXP) {
		display_opts.show_ਕੁੰਜੀਆਂ = 1;
		check_argc(argc, 1, 2);
		ret = get_ਮੁੱਲ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ਮੁੱਲ_ALL|GET_ਮੁੱਲ_ਕੁੰਜੀ_REGEXP, flags);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_UNSET) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 1, 2);
		ਜੇ (argc == 2)
			ret = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
								      argv[0], ਨਿਰਾ, argv[1],
								      ਨਿਰਾ, flags);
		ਨਹੀਂਤਾਂ
			ret = repo_config_ਸੈੱਟ_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
							     argv[0], ਨਿਰਾ, ਨਿਰਾ);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 1, 2);
		ret = repo_config_ਸੈੱਟ_multivar_ਵਿੱਚ_file_gently(the_repository, location_opts.ਸਰੋਤ.file,
							      argv[0], ਨਿਰਾ, argv[1],
							      ਨਿਰਾ, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_ਵਿੱਚ_file(the_repository, location_opts.ਸਰੋਤ.file,
							 argv[0], argv[1]);
		ਜੇ (ret < 0)
			ਜਾਓ ਬਾਹਰਲਾ;
		ਨਹੀਂਤਾਂ ਜੇ (!ret)
			die(_("no such section: %s"), argv[0]);
		ਨਹੀਂਤਾਂ
			ret = 0;
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.ਸਰੋਤ);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_ਵਿੱਚ_file(the_repository, location_opts.ਸਰੋਤ.file,
							 argv[0], ਨਿਰਾ);
		ਜੇ (ret < 0)
			ਜਾਓ ਬਾਹਰਲਾ;
		ਨਹੀਂਤਾਂ ਜੇ (!ret)
			die(_("no such section: %s"), argv[0]);
		ਨਹੀਂਤਾਂ
			ret = 0;
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	ਨਹੀਂਤਾਂ ਜੇ (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		ਜੇ (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

ਬਾਹਰਲਾ:
	location_options_release(&location_opts);
	ਮੁਕਤ(comment);
	ਮੁਕਤ(ਮੁੱਲ);
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_config(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ਨਿਰਾ;
	ਬਣਤਰ option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_ਸੈੱਟ),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_ਸਿਰਾ(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, ਅਗੇਤਰ, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	ਜੇ (subcommand) {
		argc = parse_options(argc, argv, ਅਗੇਤਰ, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		ਵਾਪਸ subcommand(argc, argv, ਅਗੇਤਰ, repo);
	}

	ਵਾਪਸ cmd_config_actions(argc, argv, ਅਗੇਤਰ);
}
