#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "date.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "ident.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "urlmatch.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ଖାଲି
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

ସଂରଚନା config_location_options {
	ସଂରଚନା git_config_source source;
	ସଂରଚନା config_options options;
	ଅକ୍ଷର *file_to_free;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_global_config;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_system_config;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_local_config;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_worktree_config;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

ଗଣନା config_type {
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
	OPT_CALLBACK_ମୂଲ୍ୟ(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_ମୂଲ୍ୟ(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_ମୂଲ୍ୟ(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_ମୂଲ୍ୟ(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_ମୂଲ୍ୟ(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_ମୂଲ୍ୟ(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_ମୂଲ୍ୟଗୁଡ଼ିକ, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_ଚାବିଗୁଡ଼ିକ, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

ସଂରଚନା config_display_options {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା end_nul;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା omit_ମୂଲ୍ୟଗୁଡ଼ିକ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_origin;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_scope;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ଚାବିଗୁଡ଼ିକ;
	ଗଣନା config_type type;
	ଅକ୍ଷର *default_ମୂଲ୍ୟ;
	/* Populated via `display_options_init()`. */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା term;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା delim;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.ଚାବି_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_ମୂଲ୍ୟ(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.ମୂଲ୍ୟ = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_type(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_type, *to_type;

	ଯଦି (unset) {
		*((ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) opt->ମୂଲ୍ୟ) = 0;
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	ଯଦି (!new_type) {
		ଯଦି (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		ଅନ୍ୟଥା
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->ମୂଲ୍ୟ;
	ଯଦି (*to_type && *to_type != new_type) {
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

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ check_argc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
	ଯଦି (argc >= min && argc <= max)
		ଫେରନ୍ତୁ;
	ଯଦି (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	ଅନ୍ୟଥା
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

ସ୍ଥିର NORETURN ଶୂନ୍ୟ die_missing_set_ମୂଲ୍ୟ(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଅଚଳ ଅକ୍ଷର *last_dot = strrchr(arg, '.');
	ଅଚଳ ଅକ୍ଷର *eq = last_dot ? strchr(last_dot + 1, '=') : ଖାଲି;
	ଅକ୍ଷର *prefix = eq ? xstrndup(arg, eq - arg) : ଖାଲି;

	ଯଦି (prefix && git_config_ଚାବି_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} ଅନ୍ୟଥା ଯଦି (git_config_ଚାବି_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} ଅନ୍ୟଥା {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	ମୁକ୍ତ(prefix);
	exit(129);
}

ସ୍ଥିର ଶୂନ୍ୟ show_config_origin(ଅଚଳ ସଂରଚନା config_display_options *opts,
			       ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi,
			       ସଂରଚନା strbuf *buf)
{
	ଅଚଳ ଅକ୍ଷର term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	ଯଦି (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	ଅନ୍ୟଥା
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ଖାଲି, 0);
	strbuf_addch(buf, term);
}

ସ୍ଥିର ଶୂନ୍ୟ show_config_scope(ଅଚଳ ସଂରଚନା config_display_options *opts,
			      ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi,
			      ସଂରଚନା strbuf *buf)
{
	ଅଚଳ ଅକ୍ଷର term = opts->end_nul ? '\0' : '\t';
	ଅଚଳ ଅକ୍ଷର *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

ସଂରଚନା strbuf_list {
	ସଂରଚନା strbuf *ବସ୍ତୁଗୁଡ଼ିକ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config_int64(ସଂରଚନା strbuf *buf,
			       ଅଚଳ ଅକ୍ଷର *ଚାବି_2,
			       ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
			       ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା gently)
{
	int64_t v = 0;
	ଯଦି (gently) {
		ଯଦି (!git_parse_int64(ମୂଲ୍ୟ_2, &v))
			ଫେରନ୍ତୁ -1;
	} ଅନ୍ୟଥା {
		/* may die() */
		v = git_config_int64(ଚାବି_2, ମୂଲ୍ୟ_2 ? ମୂଲ୍ୟ_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config_bool(ସଂରଚନା strbuf *buf,
			      ଅଚଳ ଅକ୍ଷର *ଚାବି_2,
			      ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା gently)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = 0;
	ଯଦି (gently) {
		ଯଦି ((v = git_parse_maybe_bool(ମୂଲ୍ୟ_2)) < 0)
			ଫେରନ୍ତୁ -1;
	} ଅନ୍ୟଥା {
		/* may die() */
		v = git_config_bool(ଚାବି_2, ମୂଲ୍ୟ_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config_bool_or_int(ସଂରଚନା strbuf *buf,
				     ଅଚଳ ଅକ୍ଷର *ଚାବି_2,
				     ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
				     ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା gently)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା v, is_bool = 0;

	ଯଦି (gently) {
		v = git_parse_maybe_bool_text(ମୂଲ୍ୟ_2);

		ଯଦି (v >= 0)
			is_bool = 1;
		ଅନ୍ୟଥା ଯଦି (!git_parse_int(ମୂଲ୍ୟ_2, &v))
			ଫେରନ୍ତୁ -1;
	} ଅନ୍ୟଥା {
		v = git_config_bool_or_int(ଚାବି_2, ମୂଲ୍ୟ_2, kvi,
					   &is_bool);
	}

	ଯଦି (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	ଅନ୍ୟଥା
		strbuf_addf(buf, "%d", v);

	ଫେରନ୍ତୁ 0;
}

/* This mode is always gentle. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config_bool_or_str(ସଂରଚନା strbuf *buf,
				     ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = git_parse_maybe_bool(ମୂଲ୍ୟ_2);
	ଯଦି (v < 0)
		strbuf_addstr(buf, ମୂଲ୍ୟ_2);
	ଅନ୍ୟଥା
		strbuf_addstr(buf, v ? "true" : "false");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config_path(ସଂରଚନା strbuf *buf,
			      ଅଚଳ ଅକ୍ଷର *ଚାବି_2,
			      ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା gently)
{
	ଅକ୍ଷର *v;

	ଯଦି (git_config_pathname(&v, ଚାବି_2, ମୂଲ୍ୟ_2) < 0)
		ଫେରନ୍ତୁ -1;

	ଯଦି (v)
		strbuf_addstr(buf, v);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ gently ? -1 : 1; /* :(optional)no-such-file */

	ମୁକ୍ତ(v);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config_expiry_date(ସଂରଚନା strbuf *buf,
				     ଅଚଳ ଅକ୍ଷର *ଚାବି_2,
				     ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା quietly)
{
	timestamp_t t;
	ଯଦି (quietly) {
		ଯଦି (parse_expiry_date(ମୂଲ୍ୟ_2, &t))
			ଫେରନ୍ତୁ -1;
	} ଅନ୍ୟଥା ଯଦି (git_config_expiry_date(&t, ଚାବି_2, ମୂଲ୍ୟ_2) < 0) {
		ଫେରନ୍ତୁ -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config_color(ସଂରଚନା strbuf *buf,
			       ଅଚଳ ଅକ୍ଷର *ଚାବି_2,
			       ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା gently)
{
	ଅକ୍ଷର v[COLOR_MAXLEN];

	ଯଦି (gently) {
		ଯଦି (color_parse_quietly(ମୂଲ୍ୟ_2, v) < 0)
			ଫେରନ୍ତୁ -1;
	} ଅନ୍ୟଥା ଯଦି (git_config_color(v, ଚାବି_2, ମୂଲ୍ୟ_2) < 0) {
		ଫେରନ୍ତୁ -1;
	}

	strbuf_addstr(buf, v);
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_config(ଅଚଳ ସଂରଚନା config_display_options *opts,
			 ସଂରଚନା strbuf *buf, ଅଚଳ ଅକ୍ଷର *ଚାବି_2,
			 ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2, ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା gently)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	ଯଦି (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	ଯଦି (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	ଯଦି (opts->show_ଚାବିଗୁଡ଼ିକ)
		strbuf_addstr(buf, ଚାବି_2);

	ଯଦି (opts->omit_ମୂଲ୍ୟଗୁଡ଼ିକ)
		ଯାଅ terminator;

	ଯଦି (opts->show_ଚାବିଗୁଡ଼ିକ)
		strbuf_addch(buf, opts->ଚାବି_delim);

	ବାଛ (opts->type) {
	ସ୍ଥିତି TYPE_INT:
		res = format_config_int64(buf, ଚାବି_2, ମୂଲ୍ୟ_2, kvi, gently);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି TYPE_BOOL:
		res = format_config_bool(buf, ଚାବି_2, ମୂଲ୍ୟ_2, gently);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, ଚାବି_2, ମୂଲ୍ୟ_2, kvi, gently);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, ମୂଲ୍ୟ_2);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି TYPE_PATH:
		res = format_config_path(buf, ଚାବି_2, ମୂଲ୍ୟ_2, gently);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, ଚାବି_2, ମୂଲ୍ୟ_2, gently);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି TYPE_COLOR:
		res = format_config_color(buf, ଚାବି_2, ମୂଲ୍ୟ_2, gently);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି TYPE_NONE:
		ଯଦି (ମୂଲ୍ୟ_2) {
			strbuf_addstr(buf, ମୂଲ୍ୟ_2);
		} ଅନ୍ୟଥା {
			/* Just show the key name; back out delimiter */
			ଯଦି (opts->show_ଚାବିଗୁଡ଼ିକ)
				strbuf_setlen(buf, buf->len - 1);
		}
		ଭାଙ୍ଗ;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_all_config(ଅଚଳ ଅକ୍ଷର *ଚାବି_2, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
			   ଅଚଳ ସଂରଚନା config_context *ctx,
			   ଶୂନ୍ୟ *cb)
{
	ଅଚଳ ସଂରଚନା config_display_options *opts = cb;
	ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi = ctx->kvi;
	ସଂରଚନା strbuf formatted = STRBUF_INIT;

	ଯଦି (format_config(opts, &formatted, ଚାବି_2, ମୂଲ୍ୟ_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	ଫେରନ୍ତୁ 0;
}

#define GET_ମୂଲ୍ୟ_ALL        (1 << 0)
#define GET_ମୂଲ୍ୟ_ଚାବି_REGEXP (1 << 1)

ସଂରଚନା collect_config_data {
	ଅଚଳ ସଂରଚନା config_display_options *display_opts;
	ସଂରଚନା strbuf_list *ମୂଲ୍ୟଗୁଡ଼ିକ;
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_pattern;
	ଅଚଳ ଅକ୍ଷର *ଚାବି;
	regex_t *regexp;
	regex_t *ଚାବି_regexp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_not_match;
	unsigned get_ମୂଲ୍ୟ_flags;
	unsigned flags;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_config(ଅଚଳ ଅକ୍ଷର *ଚାବି_2, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_2,
			  ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା collect_config_data *data = cb;
	ସଂରଚନା strbuf_list *ମୂଲ୍ୟଗୁଡ଼ିକ = data->ମୂଲ୍ୟଗୁଡ଼ିକ;
	ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi = ctx->kvi;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;

	ଯଦି (!(data->get_ମୂଲ୍ୟ_flags & GET_ମୂଲ୍ୟ_ଚାବି_REGEXP) &&
	    strcmp(ଚାବି_2, data->ଚାବି))
		ଫେରନ୍ତୁ 0;
	ଯଦି ((data->get_ମୂଲ୍ୟ_flags & GET_ମୂଲ୍ୟ_ଚାବି_REGEXP) &&
	    regexec(data->ଚାବି_regexp, ଚାବି_2, 0, ଖାଲି, 0))
		ଫେରନ୍ତୁ 0;
	ଯଦି ((data->flags & CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ) &&
	    strcmp(data->ମୂଲ୍ୟ_pattern, (ମୂଲ୍ୟ_2?ମୂଲ୍ୟ_2:"")))
		ଫେରନ୍ତୁ 0;
	ଯଦି (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (ମୂଲ୍ୟ_2?ମୂଲ୍ୟ_2:""), 0, ଖାଲି, 0)))
		ଫେରନ୍ତୁ 0;

	ALLOC_GROW(ମୂଲ୍ୟଗୁଡ଼ିକ->ବସ୍ତୁଗୁଡ଼ିକ, ମୂଲ୍ୟଗୁଡ଼ିକ->nr + 1, ମୂଲ୍ୟଗୁଡ଼ିକ->alloc);
	strbuf_init(&ମୂଲ୍ୟଗୁଡ଼ିକ->ବସ୍ତୁଗୁଡ଼ିକ[ମୂଲ୍ୟଗୁଡ଼ିକ->nr], 0);

	status = format_config(data->display_opts, &ମୂଲ୍ୟଗୁଡ଼ିକ->ବସ୍ତୁଗୁଡ଼ିକ[ମୂଲ୍ୟଗୁଡ଼ିକ->nr++],
			       ଚାବି_2, ମୂଲ୍ୟ_2, kvi, 0);
	ଯଦି (status < 0)
		ଫେରନ୍ତୁ status;
	ଯଦି (status) {
		strbuf_release(&ମୂଲ୍ୟଗୁଡ଼ିକ->ବସ୍ତୁଗୁଡ଼ିକ[--ମୂଲ୍ୟଗୁଡ଼ିକ->nr]);
		status = 0;
	}
	ଫେରନ୍ତୁ status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ମୂଲ୍ୟ(ଅଚଳ ସଂରଚନା config_location_options *opts,
		     ଅଚଳ ସଂରଚନା config_display_options *display_opts,
		     ଅଚଳ ଅକ୍ଷର *ଚାବି_2, ଅଚଳ ଅକ୍ଷର *regex_,
		     unsigned get_ମୂଲ୍ୟ_flags, unsigned flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = CONFIG_GENERIC_ERROR;
	ସଂରଚନା strbuf_list ମୂଲ୍ୟଗୁଡ଼ିକ = {ଖାଲି};
	ସଂରଚନା collect_config_data data = {
		.display_opts = display_opts,
		.ମୂଲ୍ୟଗୁଡ଼ିକ = &ମୂଲ୍ୟଗୁଡ଼ିକ,
		.get_ମୂଲ୍ୟ_flags = get_ମୂଲ୍ୟ_flags,
		.flags = flags,
	};
	ଅକ୍ଷର *ଚାବି = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (get_ମୂଲ୍ୟ_flags & GET_ମୂଲ୍ୟ_ଚାବି_REGEXP) {
		ଅକ୍ଷର *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		ଚାବି = xstrdup(ଚାବି_2);
		ପାଇଁ (tl = ଚାବି + strlen(ଚାବି) - 1;
		     tl >= ଚାବି && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		ପାଇଁ (tl = ଚାବି; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.ଚାବି_regexp = (regex_t*)xmalloc(ଆକାର(regex_t));
		ଯଦି (regcomp(data.ଚାବି_regexp, ଚାବି, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), ଚାବି_2);
			FREE_AND_NULL(data.ଚାବି_regexp);
			ret = CONFIG_INVALID_PATTERN;
			ଯାଅ free_strings;
		}
	} ଅନ୍ୟଥା {
		ଯଦି (git_config_parse_ଚାବି(ଚାବି_2, &ଚାବି, ଖାଲି)) {
			ret = CONFIG_INVALID_ଚାବି;
			ଯାଅ free_strings;
		}

		data.ଚାବି = ଚାବି;
	}

	ଯଦି (regex_ && (flags & CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ))
		data.ମୂଲ୍ୟ_pattern = regex_;
	ଅନ୍ୟଥା ଯଦି (regex_) {
		ଯଦି (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(ଆକାର(regex_t));
		ଯଦି (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			ଯାଅ free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ଯଦି (!ମୂଲ୍ୟଗୁଡ଼ିକ.nr && display_opts->default_ମୂଲ୍ୟ) {
		ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info kvi = KVI_INIT;
		ସଂରଚନା strbuf *item;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;

		kvi_from_param(&kvi);
		ALLOC_GROW(ମୂଲ୍ୟଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ, ମୂଲ୍ୟଗୁଡ଼ିକ.nr + 1, ମୂଲ୍ୟଗୁଡ଼ିକ.alloc);
		item = &ମୂଲ୍ୟଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ[ମୂଲ୍ୟଗୁଡ଼ିକ.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, ଚାବି_2,
				       display_opts->default_ମୂଲ୍ୟ, &kvi, 0);
		ଯଦି (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_ମୂଲ୍ୟ);
		ଯଦି (status) {
			/* default was a missing optional value */
			ମୂଲ୍ୟଗୁଡ଼ିକ.nr--;
			strbuf_release(item);
		}
	}

	ret = !ମୂଲ୍ୟଗୁଡ଼ିକ.nr;

	ପାଇଁ (i = 0; i < ମୂଲ୍ୟଗୁଡ଼ିକ.nr; i++) {
		ସଂରଚନା strbuf *buf = ମୂଲ୍ୟଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ + i;
		ଯଦି ((get_ମୂଲ୍ୟ_flags & GET_ମୂଲ୍ୟ_ALL) || i == ମୂଲ୍ୟଗୁଡ଼ିକ.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	ମୁକ୍ତ(ମୂଲ୍ୟଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ);

free_strings:
	ମୁକ୍ତ(ଚାବି);
	ଯଦି (data.ଚାବି_regexp) {
		regfree(data.ଚାବି_regexp);
		ମୁକ୍ତ(data.ଚାବି_regexp);
	}
	ଯଦି (data.regexp) {
		regfree(data.regexp);
		ମୁକ୍ତ(data.regexp);
	}

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅକ୍ଷର *normalize_ମୂଲ୍ୟ(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi)
{
	ଯଦି (!ମୂଲ୍ୟ)
		ଫେରନ୍ତୁ ଖାଲି;

	ଯଦି (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		ଫେରନ୍ତୁ xstrdup(ମୂଲ୍ୟ);
	ଯଦି (type == TYPE_INT)
		ଫେରନ୍ତୁ xstrfmt("%"PRId64, git_config_int64(ଚାବି, ମୂଲ୍ୟ, kvi));
	ଯଦି (type == TYPE_BOOL)
		ଫେରନ୍ତୁ xstrdup(git_config_bool(ଚାବି, ମୂଲ୍ୟ) ?  "true" : "false");
	ଯଦି (type == TYPE_BOOL_OR_INT) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_bool, v;
		v = git_config_bool_or_int(ଚାବି, ମୂଲ୍ୟ, kvi, &is_bool);
		ଯଦି (!is_bool)
			ଫେରନ୍ତୁ xstrfmt("%d", v);
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ xstrdup(v ? "true" : "false");
	}
	ଯଦି (type == TYPE_BOOL_OR_STR) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = git_parse_maybe_bool(ମୂଲ୍ୟ);
		ଯଦି (v < 0)
			ଫେରନ୍ତୁ xstrdup(ମୂଲ୍ୟ);
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ xstrdup(v ? "true" : "false");
	}
	ଯଦି (type == TYPE_COLOR) {
		ଅକ୍ଷର v[COLOR_MAXLEN];
		ଯଦି (git_config_color(v, ଚାବି, ମୂଲ୍ୟ))
			die(_("cannot parse color '%s'"), ମୂଲ୍ୟ);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		ଫେରନ୍ତୁ xstrdup(ମୂଲ୍ୟ);
	}

	BUG("cannot normalize type %d", type);
}

ସଂରଚନା get_color_config_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_color_found;
	ଅଚଳ ଅକ୍ଷର *get_color_slot;
	ଅକ୍ଷର parsed_color[COLOR_MAXLEN];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_get_color_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
				ଅଚଳ ସଂରଚନା config_context *ctx UNUSED,
				ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା get_color_config_data *data = cb;

	ଯଦି (!strcmp(var, data->get_color_slot)) {
		ଯଦି (!ମୂଲ୍ୟ)
			config_error_nonbool(var);
		ଯଦି (color_parse(ମୂଲ୍ୟ, data->parsed_color) < 0)
			ଫେରନ୍ତୁ -1;
		data->get_color_found = 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_color(ଅଚଳ ସଂରଚନା config_location_options *opts,
		      ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *def_color)
{
	ସଂରଚନା get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ଯଦି (!data.get_color_found && def_color) {
		ଯଦି (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			ଯାଅ out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା get_colorbool_config_data {
	ଗଣନା git_colorbool get_colorbool_found;
	ଗଣନା git_colorbool get_diff_color_found;
	ଗଣନା git_colorbool get_color_ui_found;
	ଅଚଳ ଅକ୍ଷର *get_colorbool_slot;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_get_colorbool_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
				    ଅଚଳ ସଂରଚନା config_context *ctx UNUSED,
				    ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା get_colorbool_config_data *data = cb;

	ଯଦି (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, ମୂଲ୍ୟ);
	ଅନ୍ୟଥା ଯଦି (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, ମୂଲ୍ୟ);
	ଅନ୍ୟଥା ଯଦି (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_colorbool(ଅଚଳ ସଂରଚନା config_location_options *opts,
			 ଅଚଳ ଅକ୍ଷର *var, ପୂର୍ଣ୍ଣସଂଖ୍ୟା print)
{
	ସଂରଚନା get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ଯଦି (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		ଯଦି (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		ଯଦି (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	ଯଦି (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	ଯଦି (print) {
		printf("%s\n", result ? "true" : "false");
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା
		ଫେରନ୍ତୁ result ? 0 : 1;
}

ସ୍ଥିର ଶୂନ୍ୟ check_write(ଅଚଳ ସଂରଚନା git_config_source *source)
{
	ଯଦି (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	ଯଦି (source->use_stdin)
		die(_("writing to stdin is not supported"));

	ଯଦି (source->blob)
		die(_("writing config blobs is not supported"));
}

ସଂରଚନା urlmatch_current_candidate_ମୂଲ୍ୟ {
	ଅକ୍ଷର ମୂଲ୍ୟ_is_null;
	ସଂରଚନା strbuf ମୂଲ୍ୟ;
	ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info kvi;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା urlmatch_collect_fn(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			       ଅଚଳ ସଂରଚନା config_context *ctx,
			       ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା string_list *ମୂଲ୍ୟଗୁଡ଼ିକ = cb;
	ସଂରଚନା string_list_item *item = string_list_ଭର୍ତ୍ତି(ମୂଲ୍ୟଗୁଡ଼ିକ, var);
	ସଂରଚନା urlmatch_current_candidate_ମୂଲ୍ୟ *matched = item->util;
	ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi = ctx->kvi;

	ଯଦି (!matched) {
		matched = xmalloc(ଆକାର(*matched));
		strbuf_init(&matched->ମୂଲ୍ୟ, 0);
		item->util = matched;
	} ଅନ୍ୟଥା {
		strbuf_reset(&matched->ମୂଲ୍ୟ);
	}
	matched->kvi = *kvi;

	ଯଦି (ମୂଲ୍ୟ) {
		strbuf_addstr(&matched->ମୂଲ୍ୟ, ମୂଲ୍ୟ);
		matched->ମୂଲ୍ୟ_is_null = 0;
	} ଅନ୍ୟଥା {
		matched->ମୂଲ୍ୟ_is_null = 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_urlmatch(ଅଚଳ ସଂରଚନା config_location_options *opts,
			ଅଚଳ ସଂରଚନା config_display_options *_display_opts,
			ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *url)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ଅକ୍ଷର *section;
	ଅକ୍ଷର *section_tail;
	ସଂରଚନା config_display_options display_opts = *_display_opts;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା urlmatch_config config = URLMATCH_CONFIG_INIT;
	ସଂରଚନା string_list ମୂଲ୍ୟଗୁଡ଼ିକ = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ଖାଲି;
	config.cb = &ମୂଲ୍ୟଗୁଡ଼ିକ;

	ଯଦି (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	ଯଦି (section_tail) {
		*section_tail = '\0';
		config.ଚାବି = section_tail + 1;
		display_opts.show_ଚାବିଗୁଡ଼ିକ = 0;
	} ଅନ୍ୟଥା {
		config.ଚାବି = ଖାଲି;
		display_opts.show_ଚାବିଗୁଡ଼ିକ = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !ମୂଲ୍ୟଗୁଡ଼ିକ.nr;

	for_each_string_list_item(item, &ମୂଲ୍ୟଗୁଡ଼ିକ) {
		ସଂରଚନା urlmatch_current_candidate_ମୂଲ୍ୟ *matched = item->util;
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->ମୂଲ୍ୟ_is_null ? ଖାଲି : matched->ମୂଲ୍ୟ.buf,
				       &matched->kvi, 0);
		ଯଦି (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->ମୂଲ୍ୟ);
	}
	urlmatch_config_release(&config);
	string_list_clear(&ମୂଲ୍ୟଗୁଡ଼ିକ, 1);
	ମୁକ୍ତ(config.url.url);

	ମୁକ୍ତ(section);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅକ୍ଷର *default_user_config(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	ଫେରନ୍ତୁ strbuf_detach(&buf, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ location_options_init(ସଂରଚନା config_location_options *opts,
				  ଅଚଳ ଅକ୍ଷର *prefix)
{
	ଯଦି (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	ଯଦି (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	ଯଦି (!startup_info->have_repository) {
		ଯଦି (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		ଯଦି (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		ଯଦି (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	ଯଦି (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = ଖାଲି;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	ଯଦି (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		ଯଦି (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} ଅନ୍ୟଥା ଯଦି (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} ଅନ୍ୟଥା ଯଦି (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} ଅନ୍ୟଥା ଯଦି (opts->use_worktree_config) {
		ସଂରଚନା worktree **worktrees = get_worktrees(the_repository);
		ଯଦି (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		ଅନ୍ୟଥା ଯଦି (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		ଅନ୍ୟଥା
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} ଅନ୍ୟଥା ଯଦି (opts->source.file) {
		ଯଦି (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} ଅନ୍ୟଥା ଯଦି (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	ଯଦି (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	ଅନ୍ୟଥା
		opts->options.respect_includes = opts->respect_includes_opt;
	ଯଦି (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ location_options_release(ସଂରଚନା config_location_options *opts)
{
	ମୁକ୍ତ(opts->file_to_free);
}

ସ୍ଥିର ଶୂନ୍ୟ display_options_init(ସଂରଚନା config_display_options *opts)
{
	ଯଦି (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->ଚାବି_delim = '\n';
	}
}

ସ୍ଥିର ଶୂନ୍ୟ display_options_init_list(ସଂରଚନା config_display_options *opts)
{
	opts->show_ଚାବିଗୁଡ଼ିକ = 1;

	ଯଦି (opts->end_nul) {
		display_options_init(opts);
	} ଅନ୍ୟଥା {
		opts->term = '\n';
		opts->delim = ' ';
		opts->ଚାବି_delim = '=';
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ସଂରଚନା config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ସଂରଚନା option opts[] = {
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

	ଯଦି (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		ଯଦି (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		ଅନ୍ୟଥା
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_get(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ସଂରଚନା config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_pattern = ଖାଲି, *url = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	unsigned get_ମୂଲ୍ୟ_flags = 0;
	ସଂରଚନା option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_ମୂଲ୍ୟ_flags, N_("return all values for multi-valued config options"), GET_ମୂଲ୍ୟ_ALL),
		OPT_BIT(0, "regexp", &get_ମୂଲ୍ୟ_flags, N_("interpret the name as a regular expression"), GET_ମୂଲ୍ୟ_ଚାବି_REGEXP),
		OPT_STRING(0, "value", &ମୂଲ୍ୟ_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_ମୂଲ୍ୟ,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ଯଦି ((flags & CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ) && !ମୂଲ୍ୟ_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	ଯଦି (display_opts.default_ମୂଲ୍ୟ &&
	    ((get_ମୂଲ୍ୟ_flags & GET_ମୂଲ୍ୟ_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	ଯଦି (url && ((get_ମୂଲ୍ୟ_flags & GET_ମୂଲ୍ୟ_ALL) ||
		    (get_ମୂଲ୍ୟ_flags & GET_ମୂଲ୍ୟ_ଚାବି_REGEXP) ||
		    ମୂଲ୍ୟ_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	ଯଦି (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	ଯଦି (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	ଅନ୍ୟଥା ଯଦି (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_ମୂଲ୍ୟ)
		ret = get_color(&location_opts, "", display_opts.default_ମୂଲ୍ୟ);
	ଅନ୍ୟଥା
		ret = get_ମୂଲ୍ୟ(&location_opts, &display_opts, argv[0], ମୂଲ୍ୟ_pattern,
				get_ମୂଲ୍ୟ_flags, flags);

	location_options_release(&location_opts);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			  ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_pattern = ଖାଲି, *comment_arg = ଖାଲି;
	ଅକ୍ଷର *comment = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0, append = 0, type = 0;
	ସଂରଚନା option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ମୂଲ୍ୟ_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info default_kvi = KVI_INIT;
	ଅକ୍ଷର *ମୂଲ୍ୟ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ଯଦି (argc == 1)
		die_missing_set_ମୂଲ୍ୟ(argv[0]);
	check_argc(argc, 2, 2);

	ଯଦି ((flags & CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ) && !ମୂଲ୍ୟ_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	ଯଦି (append && ମୂଲ୍ୟ_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	ଯଦି (append)
		ମୂଲ୍ୟ_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ମୂଲ୍ୟ = normalize_ମୂଲ୍ୟ(argv[0], argv[1], type, &default_kvi);

	ଯଦି ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ମୂଲ୍ୟ_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ମୂଲ୍ୟ, ମୂଲ୍ୟ_pattern,
							      comment, flags);
	} ଅନ୍ୟଥା {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, ମୂଲ୍ୟ);
		ଯଦି (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	ମୁକ୍ତ(comment);
	ମୁକ୍ତ(ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_unset(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			    ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_pattern = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ସଂରଚନା option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ମୂଲ୍ୟ_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ),
		OPT_END(),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ଯଦି ((flags & CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ) && !ମୂଲ୍ୟ_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ଯଦି ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ମୂଲ୍ୟ_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ଖାଲି, ମୂଲ୍ୟ_pattern,
							      ଖାଲି, flags);
	ଅନ୍ୟଥା
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     ଖାଲି, ଖାଲି);

	location_options_release(&location_opts);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_rename_section(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				     ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ସଂରଚନା option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	ଯଦି (ret < 0)
		ଯାଅ out;
	ଅନ୍ୟଥା ଯଦି (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_remove_section(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				     ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ସଂରଚନା option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], ଖାଲି);
	ଯଦି (ret < 0)
		ଯାଅ out;
	ଅନ୍ୟଥା ଯଦି (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_editor(ସଂରଚନା config_location_options *opts)
{
	ଅକ୍ଷର *config_file;

	ଯଦି (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	ଯଦି (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	ଯଦି (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ଖାଲି);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	ଯଦି (opts->use_global_config) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		ଯଦି (fd >= 0) {
			ଅକ୍ଷର *content = default_user_config();
			write_str_in_full(fd, content);
			ମୁକ୍ତ(content);
			close(fd);
		}
		ଅନ୍ୟଥା ଯଦି (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	ଯଦି (launch_editor(config_file, ଖାଲି, ଖାଲି)) {
		ମୁକ୍ତ(config_file);
		ଫେରନ୍ତୁ -1;
	}
	ମୁକ୍ତ(config_file);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_edit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ସଂରଚନା option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config_actions(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix)
{
	ଗଣନା {
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
	ସଂରଚନା config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ସଂରଚନା config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ଅଚଳ ଅକ୍ଷର *comment_arg = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା actions = 0;
	unsigned flags = 0;
	ସଂରଚନା option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_ମୂଲ୍ୟ,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	ଅକ୍ଷର *ମୂଲ୍ୟ = ଖାଲି, *comment = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା actions_implicit;
	ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	ଯଦି ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	ଯଦି (actions_implicit)
		ବାଛ (argc) {
		ସ୍ଥିତି 1: actions = ACTION_GET; ଭାଙ୍ଗ;
		ସ୍ଥିତି 2: actions = ACTION_SET; ଭାଙ୍ଗ;
		ସ୍ଥିତି 3: actions = ACTION_SET_ALL; ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			error(_("no action specified"));
			exit(129);
		}
	ଯଦି (actions_implicit && argc == 1) {
		ଅଚଳ ଅକ୍ଷର *last_dot = strrchr(argv[0], '.');
		ଯଦି (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_ମୂଲ୍ୟ(argv[0]);
	}
	ଯଦି (display_opts.omit_ମୂଲ୍ୟଗୁଡ଼ିକ &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	ଯଦି (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	ଯଦି (display_opts.default_ମୂଲ୍ୟ && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	ଯଦି (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	ଯଦି (flags & CONFIG_FLAGS_FIXED_ମୂଲ୍ୟ) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା allowed_usage = 0;

		ବାଛ (actions) {
		/* git config --get <name> <value-pattern> */
		ସ୍ଥିତି ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		ସ୍ଥିତି ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		ସ୍ଥିତି ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		ସ୍ଥିତି ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		ସ୍ଥିତି ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			ଭାଙ୍ଗ;

		/* git config <name> <value> <value-pattern> */
		ସ୍ଥିତି ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		ସ୍ଥିତି ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			ଭାଙ୍ଗ;

		/* other options don't allow --fixed-value */
		}

		ଯଦି (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	ଯଦି (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	ଯଦି (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		ଯଦି (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			ଯଦି (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			ଅନ୍ୟଥା
				die(_("error processing config file(s)"));
		}
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ମୂଲ୍ୟ = normalize_ମୂଲ୍ୟ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, ମୂଲ୍ୟ);
		ଯଦି (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		ମୂଲ୍ୟ = normalize_ମୂଲ୍ୟ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ମୂଲ୍ୟ, argv[2],
							      comment, flags);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ମୂଲ୍ୟ = normalize_ମୂଲ୍ୟ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ମୂଲ୍ୟ,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		ମୂଲ୍ୟ = normalize_ମୂଲ୍ୟ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ମୂଲ୍ୟ, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_ମୂଲ୍ୟ(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_ମୂଲ୍ୟ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ମୂଲ୍ୟ_ALL, flags);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_GET_REGEXP) {
		display_opts.show_ଚାବିଗୁଡ଼ିକ = 1;
		check_argc(argc, 1, 2);
		ret = get_ମୂଲ୍ୟ(&location_opts, &display_opts, argv[0], argv[1],
				GET_ମୂଲ୍ୟ_ALL|GET_ମୂଲ୍ୟ_ଚାବି_REGEXP, flags);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ଯଦି (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], ଖାଲି, argv[1],
								      ଖାଲି, flags);
		ଅନ୍ୟଥା
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], ଖାଲି, ଖାଲି);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ଖାଲି, argv[1],
							      ଖାଲି, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		ଯଦି (ret < 0)
			ଯାଅ out;
		ଅନ୍ୟଥା ଯଦି (!ret)
			die(_("no such section: %s"), argv[0]);
		ଅନ୍ୟଥା
			ret = 0;
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], ଖାଲି);
		ଯଦି (ret < 0)
			ଯାଅ out;
		ଅନ୍ୟଥା ଯଦି (!ret)
			die(_("no such section: %s"), argv[0]);
		ଅନ୍ୟଥା
			ret = 0;
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	ଅନ୍ୟଥା ଯଦି (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		ଯଦି (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	ମୁକ୍ତ(comment);
	ମୁକ୍ତ(ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_config(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ଖାଲି;
	ସଂରଚନା option subcommand_opts[] = {
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
	ଯଦି (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		ଫେରନ୍ତୁ subcommand(argc, argv, prefix, repo);
	}

	ଫେରନ୍ତୁ cmd_config_actions(argc, argv, prefix);
}
