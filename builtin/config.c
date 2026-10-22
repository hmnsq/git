#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "date.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "ident.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "urlmatch.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ರಿಕ್ತ
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

ಸಂರಚನಾ config_location_options {
	ಸಂರಚನಾ git_config_source source;
	ಸಂರಚನಾ config_options options;
	ವರ್ಣ *file_to_free;
	ಪೂರ್ಣಾಙ್ಕ use_global_config;
	ಪೂರ್ಣಾಙ್ಕ use_system_config;
	ಪೂರ್ಣಾಙ್ಕ use_local_config;
	ಪೂರ್ಣಾಙ್ಕ use_worktree_config;
	ಪೂರ್ಣಾಙ್ಕ respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

ಗಣನಾ config_type {
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
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_ಮೌಲ್ಯಗಳು, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_ಕೀಲಿಗಳು, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

ಸಂರಚನಾ config_display_options {
	ಪೂರ್ಣಾಙ್ಕ end_nul;
	ಪೂರ್ಣಾಙ್ಕ omit_ಮೌಲ್ಯಗಳು;
	ಪೂರ್ಣಾಙ್ಕ show_origin;
	ಪೂರ್ಣಾಙ್ಕ show_scope;
	ಪೂರ್ಣಾಙ್ಕ show_ಕೀಲಿಗಳು;
	ಗಣನಾ config_type type;
	ವರ್ಣ *default_ಮೌಲ್ಯ;
	/* Populated via `display_options_init()`. */
	ಪೂರ್ಣಾಙ್ಕ term;
	ಪೂರ್ಣಾಙ್ಕ delim;
	ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.ಕೀಲಿ_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_ಮೌಲ್ಯ(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.ಮೌಲ್ಯ = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_type(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			     ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ new_type, *to_type;

	ಯದಿ (unset) {
		*((ಪೂರ್ಣಾಙ್ಕ *) opt->ಮೌಲ್ಯ) = 0;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	ಯದಿ (!new_type) {
		ಯದಿ (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		ಅನ್ಯಥಾ
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->ಮೌಲ್ಯ;
	ಯದಿ (*to_type && *to_type != new_type) {
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

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_argc(ಪೂರ್ಣಾಙ್ಕ argc, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max)
{
	ಯದಿ (argc >= min && argc <= max)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	ಅನ್ಯಥಾ
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

ಸ್ಥಿರ NORETURN ಶೂನ್ಯ die_missing_set_ಮೌಲ್ಯ(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಧ್ರುವ ವರ್ಣ *last_dot = strrchr(arg, '.');
	ಧ್ರುವ ವರ್ಣ *eq = last_dot ? strchr(last_dot + 1, '=') : ರಿಕ್ತ;
	ವರ್ಣ *prefix = eq ? xstrndup(arg, eq - arg) : ರಿಕ್ತ;

	ಯದಿ (prefix && git_config_ಕೀಲಿ_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} ಅನ್ಯಥಾ ಯದಿ (git_config_ಕೀಲಿ_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} ಅನ್ಯಥಾ {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	ಮುಕ್ತ(prefix);
	exit(129);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_config_origin(ಧ್ರುವ ಸಂರಚನಾ config_display_options *opts,
			       ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			       ಸಂರಚನಾ strbuf *buf)
{
	ಧ್ರುವ ವರ್ಣ term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	ಯದಿ (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	ಅನ್ಯಥಾ
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ರಿಕ್ತ, 0);
	strbuf_addch(buf, term);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_config_scope(ಧ್ರುವ ಸಂರಚನಾ config_display_options *opts,
			      ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			      ಸಂರಚನಾ strbuf *buf)
{
	ಧ್ರುವ ವರ್ಣ term = opts->end_nul ? '\0' : '\t';
	ಧ್ರುವ ವರ್ಣ *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

ಸಂರಚನಾ strbuf_list {
	ಸಂರಚನಾ strbuf *ವಸ್ತುಗಳು;
	ಪೂರ್ಣಾಙ್ಕ nr;
	ಪೂರ್ಣಾಙ್ಕ alloc;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config_int64(ಸಂರಚನಾ strbuf *buf,
			       ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2,
			       ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
			       ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			       ಪೂರ್ಣಾಙ್ಕ gently)
{
	int64_t v = 0;
	ಯದಿ (gently) {
		ಯದಿ (!git_parse_int64(ಮೌಲ್ಯ_2, &v))
			ಪ್ರತ್ಯಾಗಮ -1;
	} ಅನ್ಯಥಾ {
		/* may die() */
		v = git_config_int64(ಕೀಲಿ_2, ಮೌಲ್ಯ_2 ? ಮೌಲ್ಯ_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config_bool(ಸಂರಚನಾ strbuf *buf,
			      ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2,
			      ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
			      ಪೂರ್ಣಾಙ್ಕ gently)
{
	ಪೂರ್ಣಾಙ್ಕ v = 0;
	ಯದಿ (gently) {
		ಯದಿ ((v = git_parse_maybe_bool(ಮೌಲ್ಯ_2)) < 0)
			ಪ್ರತ್ಯಾಗಮ -1;
	} ಅನ್ಯಥಾ {
		/* may die() */
		v = git_config_bool(ಕೀಲಿ_2, ಮೌಲ್ಯ_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config_bool_or_int(ಸಂರಚನಾ strbuf *buf,
				     ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2,
				     ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
				     ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
				     ಪೂರ್ಣಾಙ್ಕ gently)
{
	ಪೂರ್ಣಾಙ್ಕ v, is_bool = 0;

	ಯದಿ (gently) {
		v = git_parse_maybe_bool_text(ಮೌಲ್ಯ_2);

		ಯದಿ (v >= 0)
			is_bool = 1;
		ಅನ್ಯಥಾ ಯದಿ (!git_parse_int(ಮೌಲ್ಯ_2, &v))
			ಪ್ರತ್ಯಾಗಮ -1;
	} ಅನ್ಯಥಾ {
		v = git_config_bool_or_int(ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi,
					   &is_bool);
	}

	ಯದಿ (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	ಅನ್ಯಥಾ
		strbuf_addf(buf, "%d", v);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* This mode is always gentle. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config_bool_or_str(ಸಂರಚನಾ strbuf *buf,
				     ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2)
{
	ಪೂರ್ಣಾಙ್ಕ v = git_parse_maybe_bool(ಮೌಲ್ಯ_2);
	ಯದಿ (v < 0)
		strbuf_addstr(buf, ಮೌಲ್ಯ_2);
	ಅನ್ಯಥಾ
		strbuf_addstr(buf, v ? "true" : "false");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config_path(ಸಂರಚನಾ strbuf *buf,
			      ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2,
			      ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
			      ಪೂರ್ಣಾಙ್ಕ gently)
{
	ವರ್ಣ *v;

	ಯದಿ (git_config_pathname(&v, ಕೀಲಿ_2, ಮೌಲ್ಯ_2) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (v)
		strbuf_addstr(buf, v);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ gently ? -1 : 1; /* :(optional)no-such-file */

	ಮುಕ್ತ(v);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config_expiry_date(ಸಂರಚನಾ strbuf *buf,
				     ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2,
				     ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
				     ಪೂರ್ಣಾಙ್ಕ quietly)
{
	timestamp_t t;
	ಯದಿ (quietly) {
		ಯದಿ (parse_expiry_date(ಮೌಲ್ಯ_2, &t))
			ಪ್ರತ್ಯಾಗಮ -1;
	} ಅನ್ಯಥಾ ಯದಿ (git_config_expiry_date(&t, ಕೀಲಿ_2, ಮೌಲ್ಯ_2) < 0) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config_color(ಸಂರಚನಾ strbuf *buf,
			       ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2,
			       ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
			       ಪೂರ್ಣಾಙ್ಕ gently)
{
	ವರ್ಣ v[COLOR_MAXLEN];

	ಯದಿ (gently) {
		ಯದಿ (color_parse_quietly(ಮೌಲ್ಯ_2, v) < 0)
			ಪ್ರತ್ಯಾಗಮ -1;
	} ಅನ್ಯಥಾ ಯದಿ (git_config_color(v, ಕೀಲಿ_2, ಮೌಲ್ಯ_2) < 0) {
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	strbuf_addstr(buf, v);
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_config(ಧ್ರುವ ಸಂರಚನಾ config_display_options *opts,
			 ಸಂರಚನಾ strbuf *buf, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2,
			 ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2, ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			 ಪೂರ್ಣಾಙ್ಕ gently)
{
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	ಯದಿ (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	ಯದಿ (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	ಯದಿ (opts->show_ಕೀಲಿಗಳು)
		strbuf_addstr(buf, ಕೀಲಿ_2);

	ಯದಿ (opts->omit_ಮೌಲ್ಯಗಳು)
		ಗಚ್ಛ terminator;

	ಯದಿ (opts->show_ಕೀಲಿಗಳು)
		strbuf_addch(buf, opts->ಕೀಲಿ_delim);

	ವಿಕಲ್ಪ (opts->type) {
	ಪಕ್ಷ TYPE_INT:
		res = format_config_int64(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, gently);
		ಭಙ್ಗ;

	ಪಕ್ಷ TYPE_BOOL:
		res = format_config_bool(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ಭಙ್ಗ;

	ಪಕ್ಷ TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, gently);
		ಭಙ್ಗ;

	ಪಕ್ಷ TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, ಮೌಲ್ಯ_2);
		ಭಙ್ಗ;

	ಪಕ್ಷ TYPE_PATH:
		res = format_config_path(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ಭಙ್ಗ;

	ಪಕ್ಷ TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ಭಙ್ಗ;

	ಪಕ್ಷ TYPE_COLOR:
		res = format_config_color(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ಭಙ್ಗ;

	ಪಕ್ಷ TYPE_NONE:
		ಯದಿ (ಮೌಲ್ಯ_2) {
			strbuf_addstr(buf, ಮೌಲ್ಯ_2);
		} ಅನ್ಯಥಾ {
			/* Just show the key name; back out delimiter */
			ಯದಿ (opts->show_ಕೀಲಿಗಳು)
				strbuf_setlen(buf, buf->len - 1);
		}
		ಭಙ್ಗ;

	ಸ್ವಭಾವ:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_all_config(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
			   ಧ್ರುವ ಸಂರಚನಾ config_context *ctx,
			   ಶೂನ್ಯ *cb)
{
	ಧ್ರುವ ಸಂರಚನಾ config_display_options *opts = cb;
	ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;
	ಸಂರಚನಾ strbuf formatted = STRBUF_INIT;

	ಯದಿ (format_config(opts, &formatted, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	ಪ್ರತ್ಯಾಗಮ 0;
}

#define GET_ಮೌಲ್ಯ_ALL        (1 << 0)
#define GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP (1 << 1)

ಸಂರಚನಾ collect_config_data {
	ಧ್ರುವ ಸಂರಚನಾ config_display_options *display_opts;
	ಸಂರಚನಾ strbuf_list *ಮೌಲ್ಯಗಳು;
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_pattern;
	ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ;
	regex_t *regexp;
	regex_t *ಕೀಲಿ_regexp;
	ಪೂರ್ಣಾಙ್ಕ do_not_match;
	unsigned get_ಮೌಲ್ಯ_flags;
	unsigned flags;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collect_config(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_2,
			  ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ collect_config_data *data = cb;
	ಸಂರಚನಾ strbuf_list *ಮೌಲ್ಯಗಳು = data->ಮೌಲ್ಯಗಳು;
	ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;
	ಪೂರ್ಣಾಙ್ಕ status;

	ಯದಿ (!(data->get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) &&
	    strcmp(ಕೀಲಿ_2, data->ಕೀಲಿ))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ ((data->get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) &&
	    regexec(data->ಕೀಲಿ_regexp, ಕೀಲಿ_2, 0, ರಿಕ್ತ, 0))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ ((data->flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) &&
	    strcmp(data->ಮೌಲ್ಯ_pattern, (ಮೌಲ್ಯ_2?ಮೌಲ್ಯ_2:"")))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (ಮೌಲ್ಯ_2?ಮೌಲ್ಯ_2:""), 0, ರಿಕ್ತ, 0)))
		ಪ್ರತ್ಯಾಗಮ 0;

	ALLOC_GROW(ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು, ಮೌಲ್ಯಗಳು->nr + 1, ಮೌಲ್ಯಗಳು->alloc);
	strbuf_init(&ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[ಮೌಲ್ಯಗಳು->nr], 0);

	status = format_config(data->display_opts, &ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[ಮೌಲ್ಯಗಳು->nr++],
			       ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, 0);
	ಯದಿ (status < 0)
		ಪ್ರತ್ಯಾಗಮ status;
	ಯದಿ (status) {
		strbuf_release(&ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[--ಮೌಲ್ಯಗಳು->nr]);
		status = 0;
	}
	ಪ್ರತ್ಯಾಗಮ status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_ಮೌಲ್ಯ(ಧ್ರುವ ಸಂರಚನಾ config_location_options *opts,
		     ಧ್ರುವ ಸಂರಚನಾ config_display_options *display_opts,
		     ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ_2, ಧ್ರುವ ವರ್ಣ *regex_,
		     unsigned get_ಮೌಲ್ಯ_flags, unsigned flags)
{
	ಪೂರ್ಣಾಙ್ಕ ret = CONFIG_GENERIC_ERROR;
	ಸಂರಚನಾ strbuf_list ಮೌಲ್ಯಗಳು = {ರಿಕ್ತ};
	ಸಂರಚನಾ collect_config_data data = {
		.display_opts = display_opts,
		.ಮೌಲ್ಯಗಳು = &ಮೌಲ್ಯಗಳು,
		.get_ಮೌಲ್ಯ_flags = get_ಮೌಲ್ಯ_flags,
		.flags = flags,
	};
	ವರ್ಣ *ಕೀಲಿ = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) {
		ವರ್ಣ *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		ಕೀಲಿ = xstrdup(ಕೀಲಿ_2);
		ಪ್ರತಿ (tl = ಕೀಲಿ + strlen(ಕೀಲಿ) - 1;
		     tl >= ಕೀಲಿ && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		ಪ್ರತಿ (tl = ಕೀಲಿ; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.ಕೀಲಿ_regexp = (regex_t*)xmalloc(ಪರಿಮಾಣ(regex_t));
		ಯದಿ (regcomp(data.ಕೀಲಿ_regexp, ಕೀಲಿ, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), ಕೀಲಿ_2);
			FREE_AND_NULL(data.ಕೀಲಿ_regexp);
			ret = CONFIG_INVALID_PATTERN;
			ಗಚ್ಛ free_strings;
		}
	} ಅನ್ಯಥಾ {
		ಯದಿ (git_config_parse_ಕೀಲಿ(ಕೀಲಿ_2, &ಕೀಲಿ, ರಿಕ್ತ)) {
			ret = CONFIG_INVALID_ಕೀಲಿ;
			ಗಚ್ಛ free_strings;
		}

		data.ಕೀಲಿ = ಕೀಲಿ;
	}

	ಯದಿ (regex_ && (flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ))
		data.ಮೌಲ್ಯ_pattern = regex_;
	ಅನ್ಯಥಾ ಯದಿ (regex_) {
		ಯದಿ (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(ಪರಿಮಾಣ(regex_t));
		ಯದಿ (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			ಗಚ್ಛ free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ಯದಿ (!ಮೌಲ್ಯಗಳು.nr && display_opts->default_ಮೌಲ್ಯ) {
		ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info kvi = KVI_INIT;
		ಸಂರಚನಾ strbuf *item;
		ಪೂರ್ಣಾಙ್ಕ status;

		kvi_from_param(&kvi);
		ALLOC_GROW(ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು, ಮೌಲ್ಯಗಳು.nr + 1, ಮೌಲ್ಯಗಳು.alloc);
		item = &ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು[ಮೌಲ್ಯಗಳು.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, ಕೀಲಿ_2,
				       display_opts->default_ಮೌಲ್ಯ, &kvi, 0);
		ಯದಿ (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_ಮೌಲ್ಯ);
		ಯದಿ (status) {
			/* default was a missing optional value */
			ಮೌಲ್ಯಗಳು.nr--;
			strbuf_release(item);
		}
	}

	ret = !ಮೌಲ್ಯಗಳು.nr;

	ಪ್ರತಿ (i = 0; i < ಮೌಲ್ಯಗಳು.nr; i++) {
		ಸಂರಚನಾ strbuf *buf = ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು + i;
		ಯದಿ ((get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ALL) || i == ಮೌಲ್ಯಗಳು.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	ಮುಕ್ತ(ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು);

free_strings:
	ಮುಕ್ತ(ಕೀಲಿ);
	ಯದಿ (data.ಕೀಲಿ_regexp) {
		regfree(data.ಕೀಲಿ_regexp);
		ಮುಕ್ತ(data.ಕೀಲಿ_regexp);
	}
	ಯದಿ (data.regexp) {
		regfree(data.regexp);
		ಮುಕ್ತ(data.regexp);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ವರ್ಣ *normalize_ಮೌಲ್ಯ(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			     ಪೂರ್ಣಾಙ್ಕ type, ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi)
{
	ಯದಿ (!ಮೌಲ್ಯ)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		ಪ್ರತ್ಯಾಗಮ xstrdup(ಮೌಲ್ಯ);
	ಯದಿ (type == TYPE_INT)
		ಪ್ರತ್ಯಾಗಮ xstrfmt("%"PRId64, git_config_int64(ಕೀಲಿ, ಮೌಲ್ಯ, kvi));
	ಯದಿ (type == TYPE_BOOL)
		ಪ್ರತ್ಯಾಗಮ xstrdup(git_config_bool(ಕೀಲಿ, ಮೌಲ್ಯ) ?  "true" : "false");
	ಯದಿ (type == TYPE_BOOL_OR_INT) {
		ಪೂರ್ಣಾಙ್ಕ is_bool, v;
		v = git_config_bool_or_int(ಕೀಲಿ, ಮೌಲ್ಯ, kvi, &is_bool);
		ಯದಿ (!is_bool)
			ಪ್ರತ್ಯಾಗಮ xstrfmt("%d", v);
		ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ xstrdup(v ? "true" : "false");
	}
	ಯದಿ (type == TYPE_BOOL_OR_STR) {
		ಪೂರ್ಣಾಙ್ಕ v = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಯದಿ (v < 0)
			ಪ್ರತ್ಯಾಗಮ xstrdup(ಮೌಲ್ಯ);
		ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ xstrdup(v ? "true" : "false");
	}
	ಯದಿ (type == TYPE_COLOR) {
		ವರ್ಣ v[COLOR_MAXLEN];
		ಯದಿ (git_config_color(v, ಕೀಲಿ, ಮೌಲ್ಯ))
			die(_("cannot parse color '%s'"), ಮೌಲ್ಯ);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		ಪ್ರತ್ಯಾಗಮ xstrdup(ಮೌಲ್ಯ);
	}

	BUG("cannot normalize type %d", type);
}

ಸಂರಚನಾ get_color_config_data {
	ಪೂರ್ಣಾಙ್ಕ get_color_found;
	ಧ್ರುವ ವರ್ಣ *get_color_slot;
	ವರ್ಣ parsed_color[COLOR_MAXLEN];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_get_color_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
				ಧ್ರುವ ಸಂರಚನಾ config_context *ctx UNUSED,
				ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ get_color_config_data *data = cb;

	ಯದಿ (!strcmp(var, data->get_color_slot)) {
		ಯದಿ (!ಮೌಲ್ಯ)
			config_error_nonbool(var);
		ಯದಿ (color_parse(ಮೌಲ್ಯ, data->parsed_color) < 0)
			ಪ್ರತ್ಯಾಗಮ -1;
		data->get_color_found = 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_color(ಧ್ರುವ ಸಂರಚನಾ config_location_options *opts,
		      ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *def_color)
{
	ಸಂರಚನಾ get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ಯದಿ (!data.get_color_found && def_color) {
		ಯದಿ (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			ಗಚ್ಛ out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ get_colorbool_config_data {
	ಗಣನಾ git_colorbool get_colorbool_found;
	ಗಣನಾ git_colorbool get_diff_color_found;
	ಗಣನಾ git_colorbool get_color_ui_found;
	ಧ್ರುವ ವರ್ಣ *get_colorbool_slot;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_get_colorbool_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
				    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx UNUSED,
				    ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ get_colorbool_config_data *data = cb;

	ಯದಿ (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, ಮೌಲ್ಯ);
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, ಮೌಲ್ಯ);
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_colorbool(ಧ್ರುವ ಸಂರಚನಾ config_location_options *opts,
			 ಧ್ರುವ ವರ್ಣ *var, ಪೂರ್ಣಾಙ್ಕ print)
{
	ಸಂರಚನಾ get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ಯದಿ (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		ಯದಿ (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		ಯದಿ (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	ಯದಿ (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	ಯದಿ (print) {
		printf("%s\n", result ? "true" : "false");
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ result ? 0 : 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_write(ಧ್ರುವ ಸಂರಚನಾ git_config_source *source)
{
	ಯದಿ (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	ಯದಿ (source->use_stdin)
		die(_("writing to stdin is not supported"));

	ಯದಿ (source->blob)
		die(_("writing config blobs is not supported"));
}

ಸಂರಚನಾ urlmatch_current_candidate_ಮೌಲ್ಯ {
	ವರ್ಣ ಮೌಲ್ಯ_is_null;
	ಸಂರಚನಾ strbuf ಮೌಲ್ಯ;
	ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info kvi;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ urlmatch_collect_fn(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			       ಧ್ರುವ ಸಂರಚನಾ config_context *ctx,
			       ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ string_list *ಮೌಲ್ಯಗಳು = cb;
	ಸಂರಚನಾ string_list_item *item = string_list_ಸೇರಿಸು(ಮೌಲ್ಯಗಳು, var);
	ಸಂರಚನಾ urlmatch_current_candidate_ಮೌಲ್ಯ *matched = item->util;
	ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;

	ಯದಿ (!matched) {
		matched = xmalloc(ಪರಿಮಾಣ(*matched));
		strbuf_init(&matched->ಮೌಲ್ಯ, 0);
		item->util = matched;
	} ಅನ್ಯಥಾ {
		strbuf_reset(&matched->ಮೌಲ್ಯ);
	}
	matched->kvi = *kvi;

	ಯದಿ (ಮೌಲ್ಯ) {
		strbuf_addstr(&matched->ಮೌಲ್ಯ, ಮೌಲ್ಯ);
		matched->ಮೌಲ್ಯ_is_null = 0;
	} ಅನ್ಯಥಾ {
		matched->ಮೌಲ್ಯ_is_null = 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_urlmatch(ಧ್ರುವ ಸಂರಚನಾ config_location_options *opts,
			ಧ್ರುವ ಸಂರಚನಾ config_display_options *_display_opts,
			ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *url)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ವರ್ಣ *section;
	ವರ್ಣ *section_tail;
	ಸಂರಚನಾ config_display_options display_opts = *_display_opts;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ urlmatch_config config = URLMATCH_CONFIG_INIT;
	ಸಂರಚನಾ string_list ಮೌಲ್ಯಗಳು = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ರಿಕ್ತ;
	config.cb = &ಮೌಲ್ಯಗಳು;

	ಯದಿ (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	ಯದಿ (section_tail) {
		*section_tail = '\0';
		config.ಕೀಲಿ = section_tail + 1;
		display_opts.show_ಕೀಲಿಗಳು = 0;
	} ಅನ್ಯಥಾ {
		config.ಕೀಲಿ = ರಿಕ್ತ;
		display_opts.show_ಕೀಲಿಗಳು = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !ಮೌಲ್ಯಗಳು.nr;

	for_each_string_list_item(item, &ಮೌಲ್ಯಗಳು) {
		ಸಂರಚನಾ urlmatch_current_candidate_ಮೌಲ್ಯ *matched = item->util;
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
		ಪೂರ್ಣಾಙ್ಕ status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->ಮೌಲ್ಯ_is_null ? ರಿಕ್ತ : matched->ಮೌಲ್ಯ.buf,
				       &matched->kvi, 0);
		ಯದಿ (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->ಮೌಲ್ಯ);
	}
	urlmatch_config_release(&config);
	string_list_clear(&ಮೌಲ್ಯಗಳು, 1);
	ಮುಕ್ತ(config.url.url);

	ಮುಕ್ತ(section);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ವರ್ಣ *default_user_config(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&buf, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ location_options_init(ಸಂರಚನಾ config_location_options *opts,
				  ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಯದಿ (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	ಯದಿ (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	ಯದಿ (!startup_info->have_repository) {
		ಯದಿ (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		ಯದಿ (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		ಯದಿ (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	ಯದಿ (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = ರಿಕ್ತ;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	ಯದಿ (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		ಯದಿ (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} ಅನ್ಯಥಾ ಯದಿ (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} ಅನ್ಯಥಾ ಯದಿ (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} ಅನ್ಯಥಾ ಯದಿ (opts->use_worktree_config) {
		ಸಂರಚನಾ worktree **worktrees = get_worktrees(the_repository);
		ಯದಿ (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		ಅನ್ಯಥಾ ಯದಿ (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		ಅನ್ಯಥಾ
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} ಅನ್ಯಥಾ ಯದಿ (opts->source.file) {
		ಯದಿ (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} ಅನ್ಯಥಾ ಯದಿ (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	ಯದಿ (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	ಅನ್ಯಥಾ
		opts->options.respect_includes = opts->respect_includes_opt;
	ಯದಿ (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ location_options_release(ಸಂರಚನಾ config_location_options *opts)
{
	ಮುಕ್ತ(opts->file_to_free);
}

ಸ್ಥಿರ ಶೂನ್ಯ display_options_init(ಸಂರಚನಾ config_display_options *opts)
{
	ಯದಿ (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->ಕೀಲಿ_delim = '\n';
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ display_options_init_list(ಸಂರಚನಾ config_display_options *opts)
{
	opts->show_ಕೀಲಿಗಳು = 1;

	ಯದಿ (opts->end_nul) {
		display_options_init(opts);
	} ಅನ್ಯಥಾ {
		opts->term = '\n';
		opts->delim = ' ';
		opts->ಕೀಲಿ_delim = '=';
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_list(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸಂರಚನಾ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ಸಂರಚನಾ option opts[] = {
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

	ಯದಿ (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		ಯದಿ (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		ಅನ್ಯಥಾ
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_get(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸಂರಚನಾ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_pattern = ರಿಕ್ತ, *url = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ flags = 0;
	unsigned get_ಮೌಲ್ಯ_flags = 0;
	ಸಂರಚನಾ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_ಮೌಲ್ಯ_flags, N_("return all values for multi-valued config options"), GET_ಮೌಲ್ಯ_ALL),
		OPT_BIT(0, "regexp", &get_ಮೌಲ್ಯ_flags, N_("interpret the name as a regular expression"), GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP),
		OPT_STRING(0, "value", &ಮೌಲ್ಯ_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_ಮೌಲ್ಯ,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ಯದಿ ((flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) && !ಮೌಲ್ಯ_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	ಯದಿ (display_opts.default_ಮೌಲ್ಯ &&
	    ((get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	ಯದಿ (url && ((get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ALL) ||
		    (get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) ||
		    ಮೌಲ್ಯ_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	ಯದಿ (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	ಯದಿ (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	ಅನ್ಯಥಾ ಯದಿ (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_ಮೌಲ್ಯ)
		ret = get_color(&location_opts, "", display_opts.default_ಮೌಲ್ಯ);
	ಅನ್ಯಥಾ
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], ಮೌಲ್ಯ_pattern,
				get_ಮೌಲ್ಯ_flags, flags);

	location_options_release(&location_opts);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_set(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_pattern = ರಿಕ್ತ, *comment_arg = ರಿಕ್ತ;
	ವರ್ಣ *comment = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ flags = 0, append = 0, type = 0;
	ಸಂರಚನಾ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ಮೌಲ್ಯ_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info default_kvi = KVI_INIT;
	ವರ್ಣ *ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ಯದಿ (argc == 1)
		die_missing_set_ಮೌಲ್ಯ(argv[0]);
	check_argc(argc, 2, 2);

	ಯದಿ ((flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) && !ಮೌಲ್ಯ_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	ಯದಿ (append && ಮೌಲ್ಯ_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	ಯದಿ (append)
		ಮೌಲ್ಯ_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], type, &default_kvi);

	ಯದಿ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ಮೌಲ್ಯ_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ಮೌಲ್ಯ, ಮೌಲ್ಯ_pattern,
							      comment, flags);
	} ಅನ್ಯಥಾ {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, ಮೌಲ್ಯ);
		ಯದಿ (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	ಮುಕ್ತ(comment);
	ಮುಕ್ತ(ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_unset(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_pattern = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ flags = 0;
	ಸಂರಚನಾ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ಮೌಲ್ಯ_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ಯದಿ ((flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) && !ಮೌಲ್ಯ_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ಯದಿ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ಮೌಲ್ಯ_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ರಿಕ್ತ, ಮೌಲ್ಯ_pattern,
							      ರಿಕ್ತ, flags);
	ಅನ್ಯಥಾ
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     ರಿಕ್ತ, ರಿಕ್ತ);

	location_options_release(&location_opts);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_rename_section(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸಂರಚನಾ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;
	ಅನ್ಯಥಾ ಯದಿ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_remove_section(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸಂರಚನಾ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], ರಿಕ್ತ);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;
	ಅನ್ಯಥಾ ಯದಿ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_editor(ಸಂರಚನಾ config_location_options *opts)
{
	ವರ್ಣ *config_file;

	ಯದಿ (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	ಯದಿ (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	ಯದಿ (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	ಯದಿ (opts->use_global_config) {
		ಪೂರ್ಣಾಙ್ಕ fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		ಯದಿ (fd >= 0) {
			ವರ್ಣ *content = default_user_config();
			write_str_in_full(fd, content);
			ಮುಕ್ತ(content);
			close(fd);
		}
		ಅನ್ಯಥಾ ಯದಿ (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	ಯದಿ (launch_editor(config_file, ರಿಕ್ತ, ರಿಕ್ತ)) {
		ಮುಕ್ತ(config_file);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಮುಕ್ತ(config_file);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_edit(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸಂರಚನಾ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_config_actions(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಗಣನಾ {
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
	ಸಂರಚನಾ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸಂರಚನಾ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ಧ್ರುವ ವರ್ಣ *comment_arg = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ actions = 0;
	unsigned flags = 0;
	ಸಂರಚನಾ option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_ಮೌಲ್ಯ,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	ವರ್ಣ *ಮೌಲ್ಯ = ರಿಕ್ತ, *comment = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಪೂರ್ಣಾಙ್ಕ actions_implicit;
	ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	ಯದಿ ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	ಯದಿ (actions_implicit)
		ವಿಕಲ್ಪ (argc) {
		ಪಕ್ಷ 1: actions = ACTION_GET; ಭಙ್ಗ;
		ಪಕ್ಷ 2: actions = ACTION_SET; ಭಙ್ಗ;
		ಪಕ್ಷ 3: actions = ACTION_SET_ALL; ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			error(_("no action specified"));
			exit(129);
		}
	ಯದಿ (actions_implicit && argc == 1) {
		ಧ್ರುವ ವರ್ಣ *last_dot = strrchr(argv[0], '.');
		ಯದಿ (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_ಮೌಲ್ಯ(argv[0]);
	}
	ಯದಿ (display_opts.omit_ಮೌಲ್ಯಗಳು &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	ಯದಿ (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	ಯದಿ (display_opts.default_ಮೌಲ್ಯ && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	ಯದಿ (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	ಯದಿ (flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) {
		ಪೂರ್ಣಾಙ್ಕ allowed_usage = 0;

		ವಿಕಲ್ಪ (actions) {
		/* git config --get <name> <value-pattern> */
		ಪಕ್ಷ ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		ಪಕ್ಷ ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		ಪಕ್ಷ ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		ಪಕ್ಷ ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		ಪಕ್ಷ ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			ಭಙ್ಗ;

		/* git config <name> <value> <value-pattern> */
		ಪಕ್ಷ ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		ಪಕ್ಷ ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			ಭಙ್ಗ;

		/* other options don't allow --fixed-value */
		}

		ಯದಿ (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	ಯದಿ (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	ಯದಿ (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		ಯದಿ (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			ಯದಿ (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			ಅನ್ಯಥಾ
				die(_("error processing config file(s)"));
		}
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, ಮೌಲ್ಯ);
		ಯದಿ (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ಮೌಲ್ಯ, argv[2],
							      comment, flags);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ಮೌಲ್ಯ,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ಮೌಲ್ಯ, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ಮೌಲ್ಯ_ALL, flags);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_GET_REGEXP) {
		display_opts.show_ಕೀಲಿಗಳು = 1;
		check_argc(argc, 1, 2);
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ಮೌಲ್ಯ_ALL|GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP, flags);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ಯದಿ (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], ರಿಕ್ತ, argv[1],
								      ರಿಕ್ತ, flags);
		ಅನ್ಯಥಾ
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], ರಿಕ್ತ, ರಿಕ್ತ);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ರಿಕ್ತ, argv[1],
							      ರಿಕ್ತ, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		ಯದಿ (ret < 0)
			ಗಚ್ಛ out;
		ಅನ್ಯಥಾ ಯದಿ (!ret)
			die(_("no such section: %s"), argv[0]);
		ಅನ್ಯಥಾ
			ret = 0;
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], ರಿಕ್ತ);
		ಯದಿ (ret < 0)
			ಗಚ್ಛ out;
		ಅನ್ಯಥಾ ಯದಿ (!ret)
			die(_("no such section: %s"), argv[0]);
		ಅನ್ಯಥಾ
			ret = 0;
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	ಅನ್ಯಥಾ ಯದಿ (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		ಯದಿ (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	ಮುಕ್ತ(comment);
	ಮುಕ್ತ(ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ cmd_config(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ರಿಕ್ತ;
	ಸಂರಚನಾ option subcommand_opts[] = {
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
	ಯದಿ (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		ಪ್ರತ್ಯಾಗಮ subcommand(argc, argv, prefix, repo);
	}

	ಪ್ರತ್ಯಾಗಮ cmd_config_actions(argc, argv, prefix);
}
