#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_ಗಣ_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ಖಾಲಿ
};

#ವ್ಯಾಖ್ಯಾನಿಸು CONFIG_LOCATION_OPTIONS(opts) \
	OPT_ಗುಂಪು(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.ಮೂಲ.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.ಮೂಲ.blob, N_("blob-id"), N_("read config from given blob object"))

ರಚನೆ config_location_options {
	ರಚನೆ git_config_ಮೂಲ ಮೂಲ;
	ರಚನೆ config_options options;
	ಅಕ್ಷರ *file_ಗಮ್ಯ_free;
	ಪೂರ್ಣಾಂಕ use_global_config;
	ಪೂರ್ಣಾಂಕ use_system_config;
	ಪೂರ್ಣಾಂಕ use_local_config;
	ಪೂರ್ಣಾಂಕ use_worktree_config;
	ಪೂರ್ಣಾಂಕ respect_includes_opt;
};
#ವ್ಯಾಖ್ಯಾನಿಸು CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

ಎಣಿಕೆ config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_ಮಾರ್ಗ,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#ವ್ಯಾಖ್ಯಾನಿಸು CONFIG_TYPE_OPTIONS(type) \
	OPT_ಗುಂಪು(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_ಮಾರ್ಗ), \
	OPT_CALLBACK_ಮೌಲ್ಯ(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#ವ್ಯಾಖ್ಯಾನಿಸು CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_ಗುಂಪು(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.ಕೊನೆ_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_ಮೌಲ್ಯಗಳು, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_ಕೀಲಿಗಳು, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

ರಚನೆ config_display_options {
	ಪೂರ್ಣಾಂಕ ಕೊನೆ_nul;
	ಪೂರ್ಣಾಂಕ omit_ಮೌಲ್ಯಗಳು;
	ಪೂರ್ಣಾಂಕ show_origin;
	ಪೂರ್ಣಾಂಕ show_scope;
	ಪೂರ್ಣಾಂಕ show_ಕೀಲಿಗಳು;
	ಎಣಿಕೆ config_type type;
	ಅಕ್ಷರ *default_ಮೌಲ್ಯ;
	/* Populated via `display_options_init()`. */
	ಪೂರ್ಣಾಂಕ ಪದ;
	ಪೂರ್ಣಾಂಕ delim;
	ಪೂರ್ಣಾಂಕ ಕೀಲಿ_delim;
};
#ವ್ಯಾಖ್ಯಾನಿಸು CONFIG_DISPLAY_OPTIONS_INIT { \
	.ಪದ = '\n', \
	.delim = '=', \
	.ಕೀಲಿ_delim = ' ', \
	.type = TYPE_NONE, \
}

#ವ್ಯಾಖ್ಯಾನಿಸು OPT_CALLBACK_ಮೌಲ್ಯ(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.ಮೌಲ್ಯ = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_type(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			     ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ new_type, *ಗಮ್ಯ_type;

	ಆದರೆ (unset) {
		*((ಪೂರ್ಣಾಂಕ *) opt->ಮೌಲ್ಯ) = 0;
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	ಆದರೆ (!new_type) {
		ಆದರೆ (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "path"))
			new_type = TYPE_ಮಾರ್ಗ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("unrecognized --type argument, %s"), arg);
	}

	ಗಮ್ಯ_type = opt->ಮೌಲ್ಯ;
	ಆದರೆ (*ಗಮ್ಯ_type && *ಗಮ್ಯ_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		ನಿರ್ಗಮಿಸು(129);
	}
	*ಗಮ್ಯ_type = new_type;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_argc(ಪೂರ್ಣಾಂಕ argc, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max)
{
	ಆದರೆ (argc >= min && argc <= max)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	ಇಲ್ಲದಿದ್ದರೆ
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	ನಿರ್ಗಮಿಸು(129);
}

ಸ್ಥಿರ NORETURN ಶೂನ್ಯ die_missing_ಗಣ_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆಯ_dot = strrchr(arg, '.');
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *eq = ಕೊನೆಯ_dot ? strchr(ಕೊನೆಯ_dot + 1, '=') : ಖಾಲಿ;
	ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = eq ? xstrndup(arg, eq - arg) : ಖಾಲಿ;

	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && git_config_ಕೀಲಿ_is_valid(ಪೂರ್ವಪ್ರತ್ಯಯ)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       ಪೂರ್ವಪ್ರತ್ಯಯ, eq + 1);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (git_config_ಕೀಲಿ_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} ಇಲ್ಲದಿದ್ದರೆ {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	ಬಿಡುಗಡೆ(ಪೂರ್ವಪ್ರತ್ಯಯ);
	ನಿರ್ಗಮಿಸು(129);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_config_origin(ಸ್ಥಿರಾಂಕ ರಚನೆ config_display_options *opts,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			       ರಚನೆ strbuf *buf)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಪದ = opts->ಕೊನೆ_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	ಆದರೆ (opts->ಕೊನೆ_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	ಇಲ್ಲದಿದ್ದರೆ
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ಖಾಲಿ, 0);
	strbuf_addch(buf, ಪದ);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_config_scope(ಸ್ಥಿರಾಂಕ ರಚನೆ config_display_options *opts,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			      ರಚನೆ strbuf *buf)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಪದ = opts->ಕೊನೆ_nul ? '\0' : '\t';
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, ಪದ);
}

ರಚನೆ strbuf_list {
	ರಚನೆ strbuf *ವಸ್ತುಗಳು;
	ಪೂರ್ಣಾಂಕ nr;
	ಪೂರ್ಣಾಂಕ alloc;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config_int64(ರಚನೆ strbuf *buf,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			       ಪೂರ್ಣಾಂಕ gently)
{
	int64_t v = 0;
	ಆದರೆ (gently) {
		ಆದರೆ (!git_parse_int64(ಮೌಲ್ಯ_2, &v))
			ಹಿಂತಿರುಗಿ -1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* may die() */
		v = git_config_int64(ಕೀಲಿ_2, ಮೌಲ್ಯ_2 ? ಮೌಲ್ಯ_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config_bool(ರಚನೆ strbuf *buf,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
			      ಪೂರ್ಣಾಂಕ gently)
{
	ಪೂರ್ಣಾಂಕ v = 0;
	ಆದರೆ (gently) {
		ಆದರೆ ((v = git_parse_maybe_bool(ಮೌಲ್ಯ_2)) < 0)
			ಹಿಂತಿರುಗಿ -1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* may die() */
		v = git_config_bool(ಕೀಲಿ_2, ಮೌಲ್ಯ_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config_bool_or_int(ರಚನೆ strbuf *buf,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
				     ಪೂರ್ಣಾಂಕ gently)
{
	ಪೂರ್ಣಾಂಕ v, is_bool = 0;

	ಆದರೆ (gently) {
		v = git_parse_maybe_bool_ಪಠ್ಯ(ಮೌಲ್ಯ_2);

		ಆದರೆ (v >= 0)
			is_bool = 1;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!git_parse_int(ಮೌಲ್ಯ_2, &v))
			ಹಿಂತಿರುಗಿ -1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		v = git_config_bool_or_int(ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi,
					   &is_bool);
	}

	ಆದರೆ (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(buf, "%d", v);

	ಹಿಂತಿರುಗಿ 0;
}

/* This mode is always gentle. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config_bool_or_str(ರಚನೆ strbuf *buf,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2)
{
	ಪೂರ್ಣಾಂಕ v = git_parse_maybe_bool(ಮೌಲ್ಯ_2);
	ಆದರೆ (v < 0)
		strbuf_addstr(buf, ಮೌಲ್ಯ_2);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addstr(buf, v ? "true" : "false");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config_ಮಾರ್ಗ(ರಚನೆ strbuf *buf,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
			      ಪೂರ್ಣಾಂಕ gently)
{
	ಅಕ್ಷರ *v;

	ಆದರೆ (git_config_pathname(&v, ಕೀಲಿ_2, ಮೌಲ್ಯ_2) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (v)
		strbuf_addstr(buf, v);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ gently ? -1 : 1; /* :(optional)no-such-file */

	ಬಿಡುಗಡೆ(v);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config_expiry_date(ರಚನೆ strbuf *buf,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
				     ಪೂರ್ಣಾಂಕ quietly)
{
	timestamp_t t;
	ಆದರೆ (quietly) {
		ಆದರೆ (parse_expiry_date(ಮೌಲ್ಯ_2, &t))
			ಹಿಂತಿರುಗಿ -1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (git_config_expiry_date(&t, ಕೀಲಿ_2, ಮೌಲ್ಯ_2) < 0) {
		ಹಿಂತಿರುಗಿ -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config_color(ರಚನೆ strbuf *buf,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
			       ಪೂರ್ಣಾಂಕ gently)
{
	ಅಕ್ಷರ v[COLOR_MAXLEN];

	ಆದರೆ (gently) {
		ಆದರೆ (color_parse_quietly(ಮೌಲ್ಯ_2, v) < 0)
			ಹಿಂತಿರುಗಿ -1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (git_config_color(v, ಕೀಲಿ_2, ಮೌಲ್ಯ_2) < 0) {
		ಹಿಂತಿರುಗಿ -1;
	}

	strbuf_addstr(buf, v);
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_config(ಸ್ಥಿರಾಂಕ ರಚನೆ config_display_options *opts,
			 ರಚನೆ strbuf *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2, ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi,
			 ಪೂರ್ಣಾಂಕ gently)
{
	ಪೂರ್ಣಾಂಕ res = 0;
	ಆದರೆ (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	ಆದರೆ (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	ಆದರೆ (opts->show_ಕೀಲಿಗಳು)
		strbuf_addstr(buf, ಕೀಲಿ_2);

	ಆದರೆ (opts->omit_ಮೌಲ್ಯಗಳು)
		ಹೋಗು terminator;

	ಆದರೆ (opts->show_ಕೀಲಿಗಳು)
		strbuf_addch(buf, opts->ಕೀಲಿ_delim);

	ಆಯ್ಕೆ (opts->type) {
	ಸ್ಥಿತಿ TYPE_INT:
		res = format_config_int64(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, gently);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ TYPE_BOOL:
		res = format_config_bool(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, gently);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, ಮೌಲ್ಯ_2);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ TYPE_ಮಾರ್ಗ:
		res = format_config_ಮಾರ್ಗ(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ TYPE_COLOR:
		res = format_config_color(buf, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, gently);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ TYPE_NONE:
		ಆದರೆ (ಮೌಲ್ಯ_2) {
			strbuf_addstr(buf, ಮೌಲ್ಯ_2);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Just show the key name; back out delimiter */
			ಆದರೆ (opts->show_ಕೀಲಿಗಳು)
				strbuf_setlen(buf, buf->len - 1);
		}
		ನಿಲ್ಲಿಸು;

	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->ಪದ);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_all_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx,
			   ಶೂನ್ಯ *cb)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ config_display_options *opts = cb;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;
	ರಚನೆ strbuf formatted = STRBUF_INIT;

	ಆದರೆ (format_config(opts, &formatted, ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು GET_ಮೌಲ್ಯ_ALL        (1 << 0)
#ವ್ಯಾಖ್ಯಾನಿಸು GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP (1 << 1)

ರಚನೆ collect_config_ದತ್ತಾಂಶ {
	ಸ್ಥಿರಾಂಕ ರಚನೆ config_display_options *display_opts;
	ರಚನೆ strbuf_list *ಮೌಲ್ಯಗಳು;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_ನಮೂನೆ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ;
	regex_t *regexp;
	regex_t *ಕೀಲಿ_regexp;
	ಪೂರ್ಣಾಂಕ do_not_ಹೊಂದಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ get_ಮೌಲ್ಯ_flags;
	ಚಿಹ್ನೆರಹಿತ flags;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collect_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_2,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ collect_config_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb;
	ರಚನೆ strbuf_list *ಮೌಲ್ಯಗಳು = ದತ್ತಾಂಶ->ಮೌಲ್ಯಗಳು;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;

	ಆದರೆ (!(ದತ್ತಾಂಶ->get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) &&
	    strcmp(ಕೀಲಿ_2, ದತ್ತಾಂಶ->ಕೀಲಿ))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((ದತ್ತಾಂಶ->get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) &&
	    regexec(ದತ್ತಾಂಶ->ಕೀಲಿ_regexp, ಕೀಲಿ_2, 0, ಖಾಲಿ, 0))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((ದತ್ತಾಂಶ->flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) &&
	    strcmp(ದತ್ತಾಂಶ->ಮೌಲ್ಯ_ನಮೂನೆ, (ಮೌಲ್ಯ_2?ಮೌಲ್ಯ_2:"")))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (ದತ್ತಾಂಶ->regexp &&
	    (ದತ್ತಾಂಶ->do_not_ಹೊಂದಿಕೆ ^ !!regexec(ದತ್ತಾಂಶ->regexp, (ಮೌಲ್ಯ_2?ಮೌಲ್ಯ_2:""), 0, ಖಾಲಿ, 0)))
		ಹಿಂತಿರುಗಿ 0;

	ALLOC_GROW(ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು, ಮೌಲ್ಯಗಳು->nr + 1, ಮೌಲ್ಯಗಳು->alloc);
	strbuf_init(&ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[ಮೌಲ್ಯಗಳು->nr], 0);

	ಸ್ಥಿತಿ_2 = format_config(ದತ್ತಾಂಶ->display_opts, &ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[ಮೌಲ್ಯಗಳು->nr++],
			       ಕೀಲಿ_2, ಮೌಲ್ಯ_2, kvi, 0);
	ಆದರೆ (ಸ್ಥಿತಿ_2 < 0)
		ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
	ಆದರೆ (ಸ್ಥಿತಿ_2) {
		strbuf_release(&ಮೌಲ್ಯಗಳು->ವಸ್ತುಗಳು[--ಮೌಲ್ಯಗಳು->nr]);
		ಸ್ಥಿತಿ_2 = 0;
	}
	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ config_location_options *opts,
		     ಸ್ಥಿರಾಂಕ ರಚನೆ config_display_options *display_opts,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *regex_,
		     ಚಿಹ್ನೆರಹಿತ get_ಮೌಲ್ಯ_flags, ಚಿಹ್ನೆರಹಿತ flags)
{
	ಪೂರ್ಣಾಂಕ ret = CONFIG_GENERIC_ERROR;
	ರಚನೆ strbuf_list ಮೌಲ್ಯಗಳು = {ಖಾಲಿ};
	ರಚನೆ collect_config_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.display_opts = display_opts,
		.ಮೌಲ್ಯಗಳು = &ಮೌಲ್ಯಗಳು,
		.get_ಮೌಲ್ಯ_flags = get_ಮೌಲ್ಯ_flags,
		.flags = flags,
	};
	ಅಕ್ಷರ *ಕೀಲಿ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) {
		ಅಕ್ಷರ *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		ಕೀಲಿ = xstrdup(ಕೀಲಿ_2);
		ಗಾಗಿ (tl = ಕೀಲಿ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಕೀಲಿ) - 1;
		     tl >= ಕೀಲಿ && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		ಗಾಗಿ (tl = ಕೀಲಿ; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		ದತ್ತಾಂಶ.ಕೀಲಿ_regexp = (regex_t*)xmalloc(ಗಾತ್ರ(regex_t));
		ಆದರೆ (regcomp(ದತ್ತಾಂಶ.ಕೀಲಿ_regexp, ಕೀಲಿ, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), ಕೀಲಿ_2);
			FREE_AND_NULL(ದತ್ತಾಂಶ.ಕೀಲಿ_regexp);
			ret = CONFIG_INVALID_ನಮೂನೆ;
			ಹೋಗು free_ಅಕ್ಷರಸರಣಿಗಳು;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (git_config_parse_ಕೀಲಿ(ಕೀಲಿ_2, &ಕೀಲಿ, ಖಾಲಿ)) {
			ret = CONFIG_INVALID_ಕೀಲಿ;
			ಹೋಗು free_ಅಕ್ಷರಸರಣಿಗಳು;
		}

		ದತ್ತಾಂಶ.ಕೀಲಿ = ಕೀಲಿ;
	}

	ಆದರೆ (regex_ && (flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ))
		ದತ್ತಾಂಶ.ಮೌಲ್ಯ_ನಮೂನೆ = regex_;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (regex_) {
		ಆದರೆ (regex_[0] == '!') {
			ದತ್ತಾಂಶ.do_not_ಹೊಂದಿಕೆ = 1;
			regex_++;
		}

		ದತ್ತಾಂಶ.regexp = (regex_t*)xmalloc(ಗಾತ್ರ(regex_t));
		ಆದರೆ (regcomp(ದತ್ತಾಂಶ.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(ದತ್ತಾಂಶ.regexp);
			ret = CONFIG_INVALID_ನಮೂನೆ;
			ಹೋಗು free_ಅಕ್ಷರಸರಣಿಗಳು;
		}
	}

	config_with_options(collect_config, &ದತ್ತಾಂಶ,
			    &opts->ಮೂಲ, the_repository,
			    &opts->options);

	ಆದರೆ (!ಮೌಲ್ಯಗಳು.nr && display_opts->default_ಮೌಲ್ಯ) {
		ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info kvi = KVI_INIT;
		ರಚನೆ strbuf *ಅಂಶ;
		ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;

		kvi_from_param(&kvi);
		ALLOC_GROW(ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು, ಮೌಲ್ಯಗಳು.nr + 1, ಮೌಲ್ಯಗಳು.alloc);
		ಅಂಶ = &ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು[ಮೌಲ್ಯಗಳು.nr++];
		strbuf_init(ಅಂಶ, 0);

		ಸ್ಥಿತಿ_2 = format_config(display_opts, ಅಂಶ, ಕೀಲಿ_2,
				       display_opts->default_ಮೌಲ್ಯ, &kvi, 0);
		ಆದರೆ (ಸ್ಥಿತಿ_2 < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_ಮೌಲ್ಯ);
		ಆದರೆ (ಸ್ಥಿತಿ_2) {
			/* default was a missing optional value */
			ಮೌಲ್ಯಗಳು.nr--;
			strbuf_release(ಅಂಶ);
		}
	}

	ret = !ಮೌಲ್ಯಗಳು.nr;

	ಗಾಗಿ (i = 0; i < ಮೌಲ್ಯಗಳು.nr; i++) {
		ರಚನೆ strbuf *buf = ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು + i;
		ಆದರೆ ((get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ALL) || i == ಮೌಲ್ಯಗಳು.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	ಬಿಡುಗಡೆ(ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು);

free_ಅಕ್ಷರಸರಣಿಗಳು:
	ಬಿಡುಗಡೆ(ಕೀಲಿ);
	ಆದರೆ (ದತ್ತಾಂಶ.ಕೀಲಿ_regexp) {
		regfree(ದತ್ತಾಂಶ.ಕೀಲಿ_regexp);
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ.ಕೀಲಿ_regexp);
	}
	ಆದರೆ (ದತ್ತಾಂಶ.regexp) {
		regfree(ದತ್ತಾಂಶ.regexp);
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ.regexp);
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಅಕ್ಷರ *normalize_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			     ಪೂರ್ಣಾಂಕ type, ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi)
{
	ಆದರೆ (!ಮೌಲ್ಯ)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (type == 0 || type == TYPE_ಮಾರ್ಗ || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		ಹಿಂತಿರುಗಿ xstrdup(ಮೌಲ್ಯ);
	ಆದರೆ (type == TYPE_INT)
		ಹಿಂತಿರುಗಿ xstrfmt("%"PRId64, git_config_int64(ಕೀಲಿ, ಮೌಲ್ಯ, kvi));
	ಆದರೆ (type == TYPE_BOOL)
		ಹಿಂತಿರುಗಿ xstrdup(git_config_bool(ಕೀಲಿ, ಮೌಲ್ಯ) ?  "true" : "false");
	ಆದರೆ (type == TYPE_BOOL_OR_INT) {
		ಪೂರ್ಣಾಂಕ is_bool, v;
		v = git_config_bool_or_int(ಕೀಲಿ, ಮೌಲ್ಯ, kvi, &is_bool);
		ಆದರೆ (!is_bool)
			ಹಿಂತಿರುಗಿ xstrfmt("%d", v);
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ xstrdup(v ? "true" : "false");
	}
	ಆದರೆ (type == TYPE_BOOL_OR_STR) {
		ಪೂರ್ಣಾಂಕ v = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಆದರೆ (v < 0)
			ಹಿಂತಿರುಗಿ xstrdup(ಮೌಲ್ಯ);
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ xstrdup(v ? "true" : "false");
	}
	ಆದರೆ (type == TYPE_COLOR) {
		ಅಕ್ಷರ v[COLOR_MAXLEN];
		ಆದರೆ (git_config_color(v, ಕೀಲಿ, ಮೌಲ್ಯ))
			die(_("cannot parse color '%s'"), ಮೌಲ್ಯ);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		ಹಿಂತಿರುಗಿ xstrdup(ಮೌಲ್ಯ);
	}

	BUG("cannot normalize type %d", type);
}

ರಚನೆ get_color_config_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ get_color_ಸಿಕ್ಕಿತು;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_color_ಸ್ಥಾನ;
	ಅಕ್ಷರ parsed_color[COLOR_MAXLEN];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_get_color_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
				ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx ಬಳಸದ,
				ಶೂನ್ಯ *cb)
{
	ರಚನೆ get_color_config_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb;

	ಆದರೆ (!strcmp(var, ದತ್ತಾಂಶ->get_color_ಸ್ಥಾನ)) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			config_error_nonbool(var);
		ಆದರೆ (color_parse(ಮೌಲ್ಯ, ದತ್ತಾಂಶ->parsed_color) < 0)
			ಹಿಂತಿರುಗಿ -1;
		ದತ್ತಾಂಶ->get_color_ಸಿಕ್ಕಿತು = 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_color(ಸ್ಥಿರಾಂಕ ರಚನೆ config_location_options *opts,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *def_color)
{
	ರಚನೆ get_color_config_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.get_color_ಸ್ಥಾನ = var,
		.parsed_color[0] = '\0',
	};
	ಪೂರ್ಣಾಂಕ ret;

	config_with_options(git_get_color_config, &ದತ್ತಾಂಶ,
			    &opts->ಮೂಲ, the_repository,
			    &opts->options);

	ಆದರೆ (!ದತ್ತಾಂಶ.get_color_ಸಿಕ್ಕಿತು && def_color) {
		ಆದರೆ (color_parse(def_color, ದತ್ತಾಂಶ.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	ret = 0;

ಹೊರಹೋಗು:
	fputs(ದತ್ತಾಂಶ.parsed_color, stdout);
	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ get_colorbool_config_ದತ್ತಾಂಶ {
	ಎಣಿಕೆ git_colorbool get_colorbool_ಸಿಕ್ಕಿತು;
	ಎಣಿಕೆ git_colorbool get_diff_color_ಸಿಕ್ಕಿತು;
	ಎಣಿಕೆ git_colorbool get_color_ui_ಸಿಕ್ಕಿತು;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_colorbool_ಸ್ಥಾನ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_get_colorbool_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx ಬಳಸದ,
				    ಶೂನ್ಯ *cb)
{
	ರಚನೆ get_colorbool_config_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb;

	ಆದರೆ (!strcmp(var, ದತ್ತಾಂಶ->get_colorbool_ಸ್ಥಾನ))
		ದತ್ತಾಂಶ->get_colorbool_ಸಿಕ್ಕಿತು = git_config_colorbool(var, ಮೌಲ್ಯ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(var, "diff.color"))
		ದತ್ತಾಂಶ->get_diff_color_ಸಿಕ್ಕಿತು = git_config_colorbool(var, ಮೌಲ್ಯ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(var, "color.ui"))
		ದತ್ತಾಂಶ->get_color_ui_ಸಿಕ್ಕಿತು = git_config_colorbool(var, ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_colorbool(ಸ್ಥಿರಾಂಕ ರಚನೆ config_location_options *opts,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಪೂರ್ಣಾಂಕ print)
{
	ರಚನೆ get_colorbool_config_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.get_colorbool_ಸ್ಥಾನ = var,
		.get_colorbool_ಸಿಕ್ಕಿತು = GIT_COLOR_UNKNOWN,
		.get_diff_color_ಸಿಕ್ಕಿತು = GIT_COLOR_UNKNOWN,
		.get_color_ui_ಸಿಕ್ಕಿತು = GIT_COLOR_UNKNOWN,
	};
	bool ಫಲಿತಾಂಶ;

	config_with_options(git_get_colorbool_config, &ದತ್ತಾಂಶ,
			    &opts->ಮೂಲ, the_repository,
			    &opts->options);

	ಆದರೆ (ದತ್ತಾಂಶ.get_colorbool_ಸಿಕ್ಕಿತು == GIT_COLOR_UNKNOWN) {
		ಆದರೆ (!strcmp(ದತ್ತಾಂಶ.get_colorbool_ಸ್ಥಾನ, "color.diff"))
			ದತ್ತಾಂಶ.get_colorbool_ಸಿಕ್ಕಿತು = ದತ್ತಾಂಶ.get_diff_color_ಸಿಕ್ಕಿತು;
		ಆದರೆ (ದತ್ತಾಂಶ.get_colorbool_ಸಿಕ್ಕಿತು == GIT_COLOR_UNKNOWN)
			ದತ್ತಾಂಶ.get_colorbool_ಸಿಕ್ಕಿತು = ದತ್ತಾಂಶ.get_color_ui_ಸಿಕ್ಕಿತು;
	}

	ಆದರೆ (ದತ್ತಾಂಶ.get_colorbool_ಸಿಕ್ಕಿತು == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		ದತ್ತಾಂಶ.get_colorbool_ಸಿಕ್ಕಿತು = GIT_COLOR_AUTO;

	ಫಲಿತಾಂಶ = want_color(ದತ್ತಾಂಶ.get_colorbool_ಸಿಕ್ಕಿತು);

	ಆದರೆ (print) {
		ಬರೆ("%s\n", ಫಲಿತಾಂಶ ? "true" : "false");
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ ? 0 : 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_write(ಸ್ಥಿರಾಂಕ ರಚನೆ git_config_ಮೂಲ *ಮೂಲ)
{
	ಆದರೆ (!ಮೂಲ->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	ಆದರೆ (ಮೂಲ->use_stdin)
		die(_("writing to stdin is not supported"));

	ಆದರೆ (ಮೂಲ->blob)
		die(_("writing config blobs is not supported"));
}

ರಚನೆ urlmatch_current_candidate_ಮೌಲ್ಯ {
	ಅಕ್ಷರ ಮೌಲ್ಯ_is_null;
	ರಚನೆ strbuf ಮೌಲ್ಯ;
	ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info kvi;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ urlmatch_collect_fn(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx,
			       ಶೂನ್ಯ *cb)
{
	ರಚನೆ string_list *ಮೌಲ್ಯಗಳು = cb;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ = string_list_ಸೇರಿಸು(ಮೌಲ್ಯಗಳು, var);
	ರಚನೆ urlmatch_current_candidate_ಮೌಲ್ಯ *matched = ಅಂಶ->util;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;

	ಆದರೆ (!matched) {
		matched = xmalloc(ಗಾತ್ರ(*matched));
		strbuf_init(&matched->ಮೌಲ್ಯ, 0);
		ಅಂಶ->util = matched;
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_reset(&matched->ಮೌಲ್ಯ);
	}
	matched->kvi = *kvi;

	ಆದರೆ (ಮೌಲ್ಯ) {
		strbuf_addstr(&matched->ಮೌಲ್ಯ, ಮೌಲ್ಯ);
		matched->ಮೌಲ್ಯ_is_null = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		matched->ಮೌಲ್ಯ_is_null = 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_urlmatch(ಸ್ಥಿರಾಂಕ ರಚನೆ config_location_options *opts,
			ಸ್ಥಿರಾಂಕ ರಚನೆ config_display_options *_display_opts,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *url)
{
	ಪೂರ್ಣಾಂಕ ret;
	ಅಕ್ಷರ *section;
	ಅಕ್ಷರ *section_ಬಾಲ;
	ರಚನೆ config_display_options display_opts = *_display_opts;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ urlmatch_config config = URLMATCH_CONFIG_INIT;
	ರಚನೆ string_list ಮೌಲ್ಯಗಳು = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ಖಾಲಿ;
	config.cb = &ಮೌಲ್ಯಗಳು;

	ಆದರೆ (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_ಬಾಲ = strchr(section, '.');
	ಆದರೆ (section_ಬಾಲ) {
		*section_ಬಾಲ = '\0';
		config.ಕೀಲಿ = section_ಬಾಲ + 1;
		display_opts.show_ಕೀಲಿಗಳು = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		config.ಕೀಲಿ = ಖಾಲಿ;
		display_opts.show_ಕೀಲಿಗಳು = 1;
	}

	config_with_options(urlmatch_config_ನಮೂದು, &config,
			    &opts->ಮೂಲ, the_repository,
			    &opts->options);

	ret = !ಮೌಲ್ಯಗಳು.nr;

	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಮೌಲ್ಯಗಳು) {
		ರಚನೆ urlmatch_current_candidate_ಮೌಲ್ಯ *matched = ಅಂಶ->util;
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;

		ಸ್ಥಿತಿ_2 = format_config(&display_opts, &buf, ಅಂಶ->string,
				       matched->ಮೌಲ್ಯ_is_null ? ಖಾಲಿ : matched->ಮೌಲ್ಯ.buf,
				       &matched->kvi, 0);
		ಆದರೆ (!ಸ್ಥಿತಿ_2)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->ಮೌಲ್ಯ);
	}
	urlmatch_config_release(&config);
	string_list_clear(&ಮೌಲ್ಯಗಳು, 1);
	ಬಿಡುಗಡೆ(config.url.url);

	ಬಿಡುಗಡೆ(section);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಅಕ್ಷರ *default_user_config(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	ಹಿಂತಿರುಗಿ strbuf_detach(&buf, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ location_options_init(ರಚನೆ config_location_options *opts,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಆದರೆ (!opts->ಮೂಲ.file)
		opts->ಮೂಲ.file = opts->file_ಗಮ್ಯ_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	ಆದರೆ (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->ಮೂಲ.file + !!opts->ಮೂಲ.blob > 1) {
		error(_("only one config file at a time"));
		ನಿರ್ಗಮಿಸು(129);
	}

	ಆದರೆ (!startup_info->have_repository) {
		ಆದರೆ (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		ಆದರೆ (opts->ಮೂಲ.blob)
			die(_("--blob can only be used inside a git repository"));
		ಆದರೆ (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	ಆದರೆ (opts->ಮೂಲ.file &&
			!strcmp(opts->ಮೂಲ.file, "-")) {
		opts->ಮೂಲ.file = ಖಾಲಿ;
		opts->ಮೂಲ.use_stdin = 1;
		opts->ಮೂಲ.scope = CONFIG_SCOPE_COMMAND;
	}

	ಆದರೆ (opts->use_global_config) {
		opts->ಮೂಲ.file = opts->file_ಗಮ್ಯ_free = git_global_config();
		ಆದರೆ (!opts->ಮೂಲ.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->ಮೂಲ.scope = CONFIG_SCOPE_GLOBAL;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->use_system_config) {
		opts->ಮೂಲ.file = opts->file_ಗಮ್ಯ_free = git_system_config();
		opts->ಮೂಲ.scope = CONFIG_SCOPE_SYSTEM;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->use_local_config) {
		opts->ಮೂಲ.file = opts->file_ಗಮ್ಯ_free = repo_git_ಮಾರ್ಗ(the_repository, "config");
		opts->ಮೂಲ.scope = CONFIG_SCOPE_LOCAL;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->use_worktree_config) {
		ರಚನೆ worktree **worktrees = get_worktrees(the_repository);
		ಆದರೆ (the_repository->repository_format_worktree_config)
			opts->ಮೂಲ.file = opts->file_ಗಮ್ಯ_free =
				repo_git_ಮಾರ್ಗ(the_repository, "config.worktree");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		ಇಲ್ಲದಿದ್ದರೆ
			opts->ಮೂಲ.file = opts->file_ಗಮ್ಯ_free =
				repo_git_ಮಾರ್ಗ(the_repository, "config");
		opts->ಮೂಲ.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->ಮೂಲ.file) {
		ಆದರೆ (!is_absolute_ಮಾರ್ಗ(opts->ಮೂಲ.file) && ಪೂರ್ವಪ್ರತ್ಯಯ)
			opts->ಮೂಲ.file = opts->file_ಗಮ್ಯ_free =
				ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, opts->ಮೂಲ.file);
		opts->ಮೂಲ.scope = CONFIG_SCOPE_COMMAND;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->ಮೂಲ.blob) {
		opts->ಮೂಲ.scope = CONFIG_SCOPE_COMMAND;
	}

	ಆದರೆ (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->ಮೂಲ.file;
	ಇಲ್ಲದಿದ್ದರೆ
		opts->options.respect_includes = opts->respect_includes_opt;
	ಆದರೆ (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ location_options_release(ರಚನೆ config_location_options *opts)
{
	ಬಿಡುಗಡೆ(opts->file_ಗಮ್ಯ_free);
}

ಸ್ಥಿರ ಶೂನ್ಯ display_options_init(ರಚನೆ config_display_options *opts)
{
	ಆದರೆ (opts->ಕೊನೆ_nul) {
		opts->ಪದ = '\0';
		opts->delim = '\n';
		opts->ಕೀಲಿ_delim = '\n';
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ display_options_init_list(ರಚನೆ config_display_options *opts)
{
	opts->show_ಕೀಲಿಗಳು = 1;

	ಆದರೆ (opts->ಕೊನೆ_nul) {
		display_options_init(opts);
	} ಇಲ್ಲದಿದ್ದರೆ {
		opts->ಪದ = '\n';
		opts->delim = ' ';
		opts->ಕೀಲಿ_delim = '=';
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_list(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ರಚನೆ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ಗುಂಪು(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	display_options_init_list(&display_opts);

	ಸಿದ್ಧತೆ_auto_pager("config", 1);

	ಆದರೆ (config_with_options(show_all_config, &display_opts,
				&location_opts.ಮೂಲ, the_repository,
				&location_opts.options) < 0) {
		ಆದರೆ (location_opts.ಮೂಲ.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.ಮೂಲ.file);
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_get(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ರಚನೆ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_ನಮೂನೆ = ಖಾಲಿ, *url = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ flags = 0;
	ಚಿಹ್ನೆರಹಿತ get_ಮೌಲ್ಯ_flags = 0;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ಗುಂಪು(N_("Filter options")),
		OPT_ಬಿಟ್(0, "all", &get_ಮೌಲ್ಯ_flags, N_("return all values for multi-valued config options"), GET_ಮೌಲ್ಯ_ALL),
		OPT_ಬಿಟ್(0, "regexp", &get_ಮೌಲ್ಯ_flags, N_("interpret the name as a regular expression"), GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP),
		OPT_STRING(0, "value", &ಮೌಲ್ಯ_ನಮೂನೆ, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_ಬಿಟ್(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ಗುಂಪು(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_ಮೌಲ್ಯ,
			   N_("value"), N_("use default value when missing entry")),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ಆದರೆ ((flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) && !ಮೌಲ್ಯ_ನಮೂನೆ)
		die(_("--fixed-value only applies with 'value-pattern'"));
	ಆದರೆ (display_opts.default_ಮೌಲ್ಯ &&
	    ((get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	ಆದರೆ (url && ((get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ALL) ||
		    (get_ಮೌಲ್ಯ_flags & GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP) ||
		    ಮೌಲ್ಯ_ನಮೂನೆ))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	display_options_init(&display_opts);

	ಆದರೆ (display_opts.type != TYPE_COLOR)
		ಸಿದ್ಧತೆ_auto_pager("config", 1);

	ಆದರೆ (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (display_opts.type == TYPE_COLOR && !ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(argv[0]) && display_opts.default_ಮೌಲ್ಯ)
		ret = get_color(&location_opts, "", display_opts.default_ಮೌಲ್ಯ);
	ಇಲ್ಲದಿದ್ದರೆ
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], ಮೌಲ್ಯ_ನಮೂನೆ,
				get_ಮೌಲ್ಯ_flags, flags);

	location_options_release(&location_opts);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_ಗಣ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			  ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_ನಮೂನೆ = ಖಾಲಿ, *comment_arg = ಖಾಲಿ;
	ಅಕ್ಷರ *comment = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ flags = 0, append = 0, type = 0;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_ಗುಂಪು(N_("Filter")),
		OPT_ಬಿಟ್(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ಮೌಲ್ಯ_ನಮೂನೆ, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_ಬಿಟ್(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_ಗುಂಪು(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info default_kvi = KVI_INIT;
	ಅಕ್ಷರ *ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, builtin_config_ಗಣ_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ಆದರೆ (argc == 1)
		die_missing_ಗಣ_ಮೌಲ್ಯ(argv[0]);
	check_argc(argc, 2, 2);

	ಆದರೆ ((flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) && !ಮೌಲ್ಯ_ನಮೂನೆ)
		die(_("--fixed-value only applies with --value=<pattern>"));
	ಆದರೆ (append && ಮೌಲ್ಯ_ನಮೂನೆ)
		die(_("--append cannot be used with --value=<pattern>"));
	ಆದರೆ (append)
		ಮೌಲ್ಯ_ನಮೂನೆ = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	check_write(&location_opts.ಮೂಲ);

	ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], type, &default_kvi);

	ಆದರೆ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ಮೌಲ್ಯ_ನಮೂನೆ) {
		ret = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
							      argv[0], ಮೌಲ್ಯ, ಮೌಲ್ಯ_ನಮೂನೆ,
							      comment, flags);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = repo_config_ಗಣ_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
						     argv[0], comment, ಮೌಲ್ಯ);
		ಆದರೆ (ret == CONFIG_NOTHING_ಗಣ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	ಬಿಡುಗಡೆ(comment);
	ಬಿಡುಗಡೆ(ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_unset(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			    ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_ನಮೂನೆ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ flags = 0;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ಗುಂಪು(N_("Filter")),
		OPT_ಬಿಟ್(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ಮೌಲ್ಯ_ನಮೂನೆ, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_ಬಿಟ್(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ಆದರೆ ((flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) && !ಮೌಲ್ಯ_ನಮೂನೆ)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	check_write(&location_opts.ಮೂಲ);

	ಆದರೆ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ಮೌಲ್ಯ_ನಮೂನೆ)
		ret = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
							      argv[0], ಖಾಲಿ, ಮೌಲ್ಯ_ನಮೂನೆ,
							      ಖಾಲಿ, flags);
	ಇಲ್ಲದಿದ್ದರೆ
		ret = repo_config_ಗಣ_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file, argv[0],
						     ಖಾಲಿ, ಖಾಲಿ);

	location_options_release(&location_opts);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_rename_section(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				     ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	check_write(&location_opts.ಮೂಲ);

	ret = repo_config_rename_section_ಒಳ_file(the_repository, location_opts.ಮೂಲ.file,
						 argv[0], argv[1]);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

ಹೊರಹೋಗು:
	location_options_release(&location_opts);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_remove_section(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				     ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	check_write(&location_opts.ಮೂಲ);

	ret = repo_config_rename_section_ಒಳ_file(the_repository, location_opts.ಮೂಲ.file,
						 argv[0], ಖಾಲಿ);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

ಹೊರಹೋಗು:
	location_options_release(&location_opts);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_editor(ರಚನೆ config_location_options *opts)
{
	ಅಕ್ಷರ *config_file;

	ಆದರೆ (!opts->ಮೂಲ.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	ಆದರೆ (opts->ಮೂಲ.use_stdin)
		die(_("editing stdin is not supported"));
	ಆದರೆ (opts->ಮೂಲ.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	config_file = opts->ಮೂಲ.file ?
			xstrdup(opts->ಮೂಲ.file) :
			repo_git_ಮಾರ್ಗ(the_repository, "config");
	ಆದರೆ (opts->use_global_config) {
		ಪೂರ್ಣಾಂಕ fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		ಆದರೆ (fd >= 0) {
			ಅಕ್ಷರ *content = default_user_config();
			write_str_ಒಳ_ತುಂಬಿದ(fd, content);
			ಬಿಡುಗಡೆ(content);
			close(fd);
		}
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	ಆದರೆ (launch_editor(config_file, ಖಾಲಿ, ಖಾಲಿ)) {
		ಬಿಡುಗಡೆ(config_file);
		ಹಿಂತಿರುಗಿ -1;
	}
	ಬಿಡುಗಡೆ(config_file);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_edit(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ಕೊನೆ(),
	};
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	check_write(&location_opts.ಮೂಲ);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_config_actions(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಎಣಿಕೆ {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_ಕೂಡಿಸು = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_ಗಣ = (1<<11),
		ACTION_ಗಣ_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	ರಚನೆ config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ರಚನೆ config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *comment_arg = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ actions = 0;
	ಚಿಹ್ನೆರಹಿತ flags = 0;
	ರಚನೆ option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ಗುಂಪು(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_ಕೂಡಿಸು),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ಗುಂಪು(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_ಮೌಲ್ಯ,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_ಬಿಟ್(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_ಕೊನೆ(),
	};
	ಅಕ್ಷರ *ಮೌಲ್ಯ = ಖಾಲಿ, *comment = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಪೂರ್ಣಾಂಕ actions_implicit;
	ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, ಪೂರ್ವಪ್ರತ್ಯಯ);
	display_options_init(&display_opts);

	ಆದರೆ ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		ನಿರ್ಗಮಿಸು(129);
	}

	actions_implicit = (actions == 0);
	ಆದರೆ (actions_implicit)
		ಆಯ್ಕೆ (argc) {
		ಸ್ಥಿತಿ 1: actions = ACTION_GET; ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 2: actions = ACTION_ಗಣ; ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 3: actions = ACTION_ಗಣ_ALL; ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			error(_("no action specified"));
			ನಿರ್ಗಮಿಸು(129);
		}
	ಆದರೆ (actions_implicit && argc == 1) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆಯ_dot = strrchr(argv[0], '.');
		ಆದರೆ (ಕೊನೆಯ_dot && strchr(ಕೊನೆಯ_dot + 1, '='))
			die_missing_ಗಣ_ಮೌಲ್ಯ(argv[0]);
	}
	ಆದರೆ (display_opts.omit_ಮೌಲ್ಯಗಳು &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		ನಿರ್ಗಮಿಸು(129);
	}

	ಆದರೆ (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		ನಿರ್ಗಮಿಸು(129);
	}

	ಆದರೆ (display_opts.default_ಮೌಲ್ಯ && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		ನಿರ್ಗಮಿಸು(129);
	}

	ಆದರೆ (comment_arg &&
	    !(actions & (ACTION_ಕೂಡಿಸು|ACTION_ಗಣ|ACTION_ಗಣ_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		ನಿರ್ಗಮಿಸು(129);
	}

	/* check usage of --fixed-value */
	ಆದರೆ (flags & CONFIG_FLAGS_FIXED_ಮೌಲ್ಯ) {
		ಪೂರ್ಣಾಂಕ allowed_usage = 0;

		ಆಯ್ಕೆ (actions) {
		/* git config --get <name> <value-pattern> */
		ಸ್ಥಿತಿ ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		ಸ್ಥಿತಿ ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		ಸ್ಥಿತಿ ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		ಸ್ಥಿತಿ ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		ಸ್ಥಿತಿ ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			ನಿಲ್ಲಿಸು;

		/* git config <name> <value> <value-pattern> */
		ಸ್ಥಿತಿ ACTION_ಗಣ_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		ಸ್ಥಿತಿ ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			ನಿಲ್ಲಿಸು;

		/* other options don't allow --fixed-value */
		}

		ಆದರೆ (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			ನಿರ್ಗಮಿಸು(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	ಆದರೆ (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		ಸಿದ್ಧತೆ_auto_pager("config", 1);

	ಆದರೆ (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		ಆದರೆ (config_with_options(show_all_config, &display_opts,
					&location_opts.ಮೂಲ, the_repository,
					&location_opts.options) < 0) {
			ಆದರೆ (location_opts.ಮೂಲ.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.ಮೂಲ.file);
			ಇಲ್ಲದಿದ್ದರೆ
				die(_("error processing config file(s)"));
		}
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_ಗಣ) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 2, 2);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ಗಣ_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file, argv[0], comment, ಮೌಲ್ಯ);
		ಆದರೆ (ret == CONFIG_NOTHING_ಗಣ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_ಗಣ_ALL) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 2, 3);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
							      argv[0], ಮೌಲ್ಯ, argv[2],
							      comment, flags);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_ಕೂಡಿಸು) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 2, 2);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
							      argv[0], ಮೌಲ್ಯ,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 2, 3);
		ಮೌಲ್ಯ = normalize_ಮೌಲ್ಯ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
							      argv[0], ಮೌಲ್ಯ, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ಮೌಲ್ಯ_ALL, flags);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_GET_REGEXP) {
		display_opts.show_ಕೀಲಿಗಳು = 1;
		check_argc(argc, 1, 2);
		ret = get_ಮೌಲ್ಯ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ಮೌಲ್ಯ_ALL|GET_ಮೌಲ್ಯ_ಕೀಲಿ_REGEXP, flags);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_UNSET) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 1, 2);
		ಆದರೆ (argc == 2)
			ret = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
								      argv[0], ಖಾಲಿ, argv[1],
								      ಖಾಲಿ, flags);
		ಇಲ್ಲದಿದ್ದರೆ
			ret = repo_config_ಗಣ_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
							     argv[0], ಖಾಲಿ, ಖಾಲಿ);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 1, 2);
		ret = repo_config_ಗಣ_multivar_ಒಳ_file_gently(the_repository, location_opts.ಮೂಲ.file,
							      argv[0], ಖಾಲಿ, argv[1],
							      ಖಾಲಿ, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_ಒಳ_file(the_repository, location_opts.ಮೂಲ.file,
							 argv[0], argv[1]);
		ಆದರೆ (ret < 0)
			ಹೋಗು ಹೊರಹೋಗು;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ret)
			die(_("no such section: %s"), argv[0]);
		ಇಲ್ಲದಿದ್ದರೆ
			ret = 0;
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.ಮೂಲ);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_ಒಳ_file(the_repository, location_opts.ಮೂಲ.file,
							 argv[0], ಖಾಲಿ);
		ಆದರೆ (ret < 0)
			ಹೋಗು ಹೊರಹೋಗು;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ret)
			die(_("no such section: %s"), argv[0]);
		ಇಲ್ಲದಿದ್ದರೆ
			ret = 0;
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		ಆದರೆ (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

ಹೊರಹೋಗು:
	location_options_release(&location_opts);
	ಬಿಡುಗಡೆ(comment);
	ಬಿಡುಗಡೆ(ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_config(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ಖಾಲಿ;
	ರಚನೆ option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_ಗಣ),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_ಕೊನೆ(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	ಆದರೆ (subcommand) {
		argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		ಹಿಂತಿರುಗಿ subcommand(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
	}

	ಹಿಂತಿರುಗಿ cmd_config_actions(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ);
}
