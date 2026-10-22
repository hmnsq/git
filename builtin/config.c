#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "date.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "ident.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "urlmatch.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "worktree.h"

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	வெற்று
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

கட்டமைப்பு config_location_options {
	கட்டமைப்பு git_config_source source;
	கட்டமைப்பு config_options options;
	எழுத்து *file_to_free;
	முழு use_global_config;
	முழு use_system_config;
	முழு use_local_config;
	முழு use_worktree_config;
	முழு respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

எண்ணிக்கை config_type {
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
	OPT_CALLBACK_மதிப்பு(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_மதிப்பு(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_மதிப்பு(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_மதிப்பு(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_மதிப்பு(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_மதிப்பு(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_மதிப்புகள், N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_சாவிகள், N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

கட்டமைப்பு config_display_options {
	முழு end_nul;
	முழு omit_மதிப்புகள்;
	முழு show_origin;
	முழு show_scope;
	முழு show_சாவிகள்;
	எண்ணிக்கை config_type type;
	எழுத்து *default_மதிப்பு;
	/* Populated via `display_options_init()`. */
	முழு term;
	முழு delim;
	முழு சாவி_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.சாவி_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_மதிப்பு(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.மதிப்பு = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

நிலையான முழு option_parse_type(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			     முழு unset)
{
	முழு new_type, *to_type;

	எனில் (unset) {
		*((முழு *) opt->மதிப்பு) = 0;
		திரும்பு 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	எனில் (!new_type) {
		எனில் (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		இல்லையெனில் எனில் (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		இல்லையெனில் எனில் (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		இல்லையெனில் எனில் (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		இல்லையெனில் எனில் (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		இல்லையெனில் எனில் (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		இல்லையெனில் எனில் (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		இல்லையெனில்
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->மதிப்பு;
	எனில் (*to_type && *to_type != new_type) {
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

	திரும்பு 0;
}

நிலையான வெறுமை check_argc(முழு argc, முழு min, முழு max)
{
	எனில் (argc >= min && argc <= max)
		திரும்பு;
	எனில் (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	இல்லையெனில்
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

நிலையான NORETURN வெறுமை die_missing_set_மதிப்பு(மாறிலி எழுத்து *arg)
{
	மாறிலி எழுத்து *last_dot = strrchr(arg, '.');
	மாறிலி எழுத்து *eq = last_dot ? strchr(last_dot + 1, '=') : வெற்று;
	எழுத்து *prefix = eq ? xstrndup(arg, eq - arg) : வெற்று;

	எனில் (prefix && git_config_சாவி_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} இல்லையெனில் எனில் (git_config_சாவி_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} இல்லையெனில் {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	விடுவி(prefix);
	exit(129);
}

நிலையான வெறுமை show_config_origin(மாறிலி கட்டமைப்பு config_display_options *opts,
			       மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi,
			       கட்டமைப்பு strbuf *buf)
{
	மாறிலி எழுத்து term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	எனில் (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	இல்லையெனில்
		quote_c_style(kvi->filename ? kvi->filename : "", buf, வெற்று, 0);
	strbuf_addch(buf, term);
}

நிலையான வெறுமை show_config_scope(மாறிலி கட்டமைப்பு config_display_options *opts,
			      மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi,
			      கட்டமைப்பு strbuf *buf)
{
	மாறிலி எழுத்து term = opts->end_nul ? '\0' : '\t';
	மாறிலி எழுத்து *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

கட்டமைப்பு strbuf_list {
	கட்டமைப்பு strbuf *பொருட்கள்;
	முழு nr;
	முழு alloc;
};

நிலையான முழு format_config_int64(கட்டமைப்பு strbuf *buf,
			       மாறிலி எழுத்து *சாவி_2,
			       மாறிலி எழுத்து *மதிப்பு_2,
			       மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi,
			       முழு gently)
{
	int64_t v = 0;
	எனில் (gently) {
		எனில் (!git_parse_int64(மதிப்பு_2, &v))
			திரும்பு -1;
	} இல்லையெனில் {
		/* may die() */
		v = git_config_int64(சாவி_2, மதிப்பு_2 ? மதிப்பு_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	திரும்பு 0;
}

நிலையான முழு format_config_bool(கட்டமைப்பு strbuf *buf,
			      மாறிலி எழுத்து *சாவி_2,
			      மாறிலி எழுத்து *மதிப்பு_2,
			      முழு gently)
{
	முழு v = 0;
	எனில் (gently) {
		எனில் ((v = git_parse_maybe_bool(மதிப்பு_2)) < 0)
			திரும்பு -1;
	} இல்லையெனில் {
		/* may die() */
		v = git_config_bool(சாவி_2, மதிப்பு_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	திரும்பு 0;
}

நிலையான முழு format_config_bool_or_int(கட்டமைப்பு strbuf *buf,
				     மாறிலி எழுத்து *சாவி_2,
				     மாறிலி எழுத்து *மதிப்பு_2,
				     மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi,
				     முழு gently)
{
	முழு v, is_bool = 0;

	எனில் (gently) {
		v = git_parse_maybe_bool_text(மதிப்பு_2);

		எனில் (v >= 0)
			is_bool = 1;
		இல்லையெனில் எனில் (!git_parse_int(மதிப்பு_2, &v))
			திரும்பு -1;
	} இல்லையெனில் {
		v = git_config_bool_or_int(சாவி_2, மதிப்பு_2, kvi,
					   &is_bool);
	}

	எனில் (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	இல்லையெனில்
		strbuf_addf(buf, "%d", v);

	திரும்பு 0;
}

/* This mode is always gentle. */
நிலையான முழு format_config_bool_or_str(கட்டமைப்பு strbuf *buf,
				     மாறிலி எழுத்து *மதிப்பு_2)
{
	முழு v = git_parse_maybe_bool(மதிப்பு_2);
	எனில் (v < 0)
		strbuf_addstr(buf, மதிப்பு_2);
	இல்லையெனில்
		strbuf_addstr(buf, v ? "true" : "false");
	திரும்பு 0;
}

நிலையான முழு format_config_path(கட்டமைப்பு strbuf *buf,
			      மாறிலி எழுத்து *சாவி_2,
			      மாறிலி எழுத்து *மதிப்பு_2,
			      முழு gently)
{
	எழுத்து *v;

	எனில் (git_config_pathname(&v, சாவி_2, மதிப்பு_2) < 0)
		திரும்பு -1;

	எனில் (v)
		strbuf_addstr(buf, v);
	இல்லையெனில்
		திரும்பு gently ? -1 : 1; /* :(optional)no-such-file */

	விடுவி(v);
	திரும்பு 0;
}

நிலையான முழு format_config_expiry_date(கட்டமைப்பு strbuf *buf,
				     மாறிலி எழுத்து *சாவி_2,
				     மாறிலி எழுத்து *மதிப்பு_2,
				     முழு quietly)
{
	timestamp_t t;
	எனில் (quietly) {
		எனில் (parse_expiry_date(மதிப்பு_2, &t))
			திரும்பு -1;
	} இல்லையெனில் எனில் (git_config_expiry_date(&t, சாவி_2, மதிப்பு_2) < 0) {
		திரும்பு -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	திரும்பு 0;
}

நிலையான முழு format_config_color(கட்டமைப்பு strbuf *buf,
			       மாறிலி எழுத்து *சாவி_2,
			       மாறிலி எழுத்து *மதிப்பு_2,
			       முழு gently)
{
	எழுத்து v[COLOR_MAXLEN];

	எனில் (gently) {
		எனில் (color_parse_quietly(மதிப்பு_2, v) < 0)
			திரும்பு -1;
	} இல்லையெனில் எனில் (git_config_color(v, சாவி_2, மதிப்பு_2) < 0) {
		திரும்பு -1;
	}

	strbuf_addstr(buf, v);
	திரும்பு 0;
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
நிலையான முழு format_config(மாறிலி கட்டமைப்பு config_display_options *opts,
			 கட்டமைப்பு strbuf *buf, மாறிலி எழுத்து *சாவி_2,
			 மாறிலி எழுத்து *மதிப்பு_2, மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi,
			 முழு gently)
{
	முழு res = 0;
	எனில் (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	எனில் (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	எனில் (opts->show_சாவிகள்)
		strbuf_addstr(buf, சாவி_2);

	எனில் (opts->omit_மதிப்புகள்)
		செல் terminator;

	எனில் (opts->show_சாவிகள்)
		strbuf_addch(buf, opts->சாவி_delim);

	தேர்வு (opts->type) {
	நிலை TYPE_INT:
		res = format_config_int64(buf, சாவி_2, மதிப்பு_2, kvi, gently);
		நிறுத்து;

	நிலை TYPE_BOOL:
		res = format_config_bool(buf, சாவி_2, மதிப்பு_2, gently);
		நிறுத்து;

	நிலை TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, சாவி_2, மதிப்பு_2, kvi, gently);
		நிறுத்து;

	நிலை TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, மதிப்பு_2);
		நிறுத்து;

	நிலை TYPE_PATH:
		res = format_config_path(buf, சாவி_2, மதிப்பு_2, gently);
		நிறுத்து;

	நிலை TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, சாவி_2, மதிப்பு_2, gently);
		நிறுத்து;

	நிலை TYPE_COLOR:
		res = format_config_color(buf, சாவி_2, மதிப்பு_2, gently);
		நிறுத்து;

	நிலை TYPE_NONE:
		எனில் (மதிப்பு_2) {
			strbuf_addstr(buf, மதிப்பு_2);
		} இல்லையெனில் {
			/* Just show the key name; back out delimiter */
			எனில் (opts->show_சாவிகள்)
				strbuf_setlen(buf, buf->len - 1);
		}
		நிறுத்து;

	இயல்பு:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	திரும்பு res;
}

நிலையான முழு show_all_config(மாறிலி எழுத்து *சாவி_2, மாறிலி எழுத்து *மதிப்பு_2,
			   மாறிலி கட்டமைப்பு config_context *ctx,
			   வெறுமை *cb)
{
	மாறிலி கட்டமைப்பு config_display_options *opts = cb;
	மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi = ctx->kvi;
	கட்டமைப்பு strbuf formatted = STRBUF_INIT;

	எனில் (format_config(opts, &formatted, சாவி_2, மதிப்பு_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	திரும்பு 0;
}

#define GET_மதிப்பு_ALL        (1 << 0)
#define GET_மதிப்பு_சாவி_REGEXP (1 << 1)

கட்டமைப்பு collect_config_data {
	மாறிலி கட்டமைப்பு config_display_options *display_opts;
	கட்டமைப்பு strbuf_list *மதிப்புகள்;
	மாறிலி எழுத்து *மதிப்பு_pattern;
	மாறிலி எழுத்து *சாவி;
	regex_t *regexp;
	regex_t *சாவி_regexp;
	முழு do_not_match;
	unsigned get_மதிப்பு_flags;
	unsigned flags;
};

நிலையான முழு collect_config(மாறிலி எழுத்து *சாவி_2, மாறிலி எழுத்து *மதிப்பு_2,
			  மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு collect_config_data *data = cb;
	கட்டமைப்பு strbuf_list *மதிப்புகள் = data->மதிப்புகள்;
	மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi = ctx->kvi;
	முழு status;

	எனில் (!(data->get_மதிப்பு_flags & GET_மதிப்பு_சாவி_REGEXP) &&
	    strcmp(சாவி_2, data->சாவி))
		திரும்பு 0;
	எனில் ((data->get_மதிப்பு_flags & GET_மதிப்பு_சாவி_REGEXP) &&
	    regexec(data->சாவி_regexp, சாவி_2, 0, வெற்று, 0))
		திரும்பு 0;
	எனில் ((data->flags & CONFIG_FLAGS_FIXED_மதிப்பு) &&
	    strcmp(data->மதிப்பு_pattern, (மதிப்பு_2?மதிப்பு_2:"")))
		திரும்பு 0;
	எனில் (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (மதிப்பு_2?மதிப்பு_2:""), 0, வெற்று, 0)))
		திரும்பு 0;

	ALLOC_GROW(மதிப்புகள்->பொருட்கள், மதிப்புகள்->nr + 1, மதிப்புகள்->alloc);
	strbuf_init(&மதிப்புகள்->பொருட்கள்[மதிப்புகள்->nr], 0);

	status = format_config(data->display_opts, &மதிப்புகள்->பொருட்கள்[மதிப்புகள்->nr++],
			       சாவி_2, மதிப்பு_2, kvi, 0);
	எனில் (status < 0)
		திரும்பு status;
	எனில் (status) {
		strbuf_release(&மதிப்புகள்->பொருட்கள்[--மதிப்புகள்->nr]);
		status = 0;
	}
	திரும்பு status;
}

நிலையான முழு get_மதிப்பு(மாறிலி கட்டமைப்பு config_location_options *opts,
		     மாறிலி கட்டமைப்பு config_display_options *display_opts,
		     மாறிலி எழுத்து *சாவி_2, மாறிலி எழுத்து *regex_,
		     unsigned get_மதிப்பு_flags, unsigned flags)
{
	முழு ret = CONFIG_GENERIC_ERROR;
	கட்டமைப்பு strbuf_list மதிப்புகள் = {வெற்று};
	கட்டமைப்பு collect_config_data data = {
		.display_opts = display_opts,
		.மதிப்புகள் = &மதிப்புகள்,
		.get_மதிப்பு_flags = get_மதிப்பு_flags,
		.flags = flags,
	};
	எழுத்து *சாவி = வெற்று;
	முழு i;

	எனில் (get_மதிப்பு_flags & GET_மதிப்பு_சாவி_REGEXP) {
		எழுத்து *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		சாவி = xstrdup(சாவி_2);
		க்கு (tl = சாவி + strlen(சாவி) - 1;
		     tl >= சாவி && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		க்கு (tl = சாவி; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.சாவி_regexp = (regex_t*)xmalloc(அளவு(regex_t));
		எனில் (regcomp(data.சாவி_regexp, சாவி, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), சாவி_2);
			FREE_AND_NULL(data.சாவி_regexp);
			ret = CONFIG_INVALID_PATTERN;
			செல் free_strings;
		}
	} இல்லையெனில் {
		எனில் (git_config_parse_சாவி(சாவி_2, &சாவி, வெற்று)) {
			ret = CONFIG_INVALID_சாவி;
			செல் free_strings;
		}

		data.சாவி = சாவி;
	}

	எனில் (regex_ && (flags & CONFIG_FLAGS_FIXED_மதிப்பு))
		data.மதிப்பு_pattern = regex_;
	இல்லையெனில் எனில் (regex_) {
		எனில் (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(அளவு(regex_t));
		எனில் (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			செல் free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	எனில் (!மதிப்புகள்.nr && display_opts->default_மதிப்பு) {
		கட்டமைப்பு சாவி_மதிப்பு_info kvi = KVI_INIT;
		கட்டமைப்பு strbuf *item;
		முழு status;

		kvi_from_param(&kvi);
		ALLOC_GROW(மதிப்புகள்.பொருட்கள், மதிப்புகள்.nr + 1, மதிப்புகள்.alloc);
		item = &மதிப்புகள்.பொருட்கள்[மதிப்புகள்.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, சாவி_2,
				       display_opts->default_மதிப்பு, &kvi, 0);
		எனில் (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_மதிப்பு);
		எனில் (status) {
			/* default was a missing optional value */
			மதிப்புகள்.nr--;
			strbuf_release(item);
		}
	}

	ret = !மதிப்புகள்.nr;

	க்கு (i = 0; i < மதிப்புகள்.nr; i++) {
		கட்டமைப்பு strbuf *buf = மதிப்புகள்.பொருட்கள் + i;
		எனில் ((get_மதிப்பு_flags & GET_மதிப்பு_ALL) || i == மதிப்புகள்.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	விடுவி(மதிப்புகள்.பொருட்கள்);

free_strings:
	விடுவி(சாவி);
	எனில் (data.சாவி_regexp) {
		regfree(data.சாவி_regexp);
		விடுவி(data.சாவி_regexp);
	}
	எனில் (data.regexp) {
		regfree(data.regexp);
		விடுவி(data.regexp);
	}

	திரும்பு ret;
}

நிலையான எழுத்து *normalize_மதிப்பு(மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு,
			     முழு type, கட்டமைப்பு சாவி_மதிப்பு_info *kvi)
{
	எனில் (!மதிப்பு)
		திரும்பு வெற்று;

	எனில் (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		திரும்பு xstrdup(மதிப்பு);
	எனில் (type == TYPE_INT)
		திரும்பு xstrfmt("%"PRId64, git_config_int64(சாவி, மதிப்பு, kvi));
	எனில் (type == TYPE_BOOL)
		திரும்பு xstrdup(git_config_bool(சாவி, மதிப்பு) ?  "true" : "false");
	எனில் (type == TYPE_BOOL_OR_INT) {
		முழு is_bool, v;
		v = git_config_bool_or_int(சாவி, மதிப்பு, kvi, &is_bool);
		எனில் (!is_bool)
			திரும்பு xstrfmt("%d", v);
		இல்லையெனில்
			திரும்பு xstrdup(v ? "true" : "false");
	}
	எனில் (type == TYPE_BOOL_OR_STR) {
		முழு v = git_parse_maybe_bool(மதிப்பு);
		எனில் (v < 0)
			திரும்பு xstrdup(மதிப்பு);
		இல்லையெனில்
			திரும்பு xstrdup(v ? "true" : "false");
	}
	எனில் (type == TYPE_COLOR) {
		எழுத்து v[COLOR_MAXLEN];
		எனில் (git_config_color(v, சாவி, மதிப்பு))
			die(_("cannot parse color '%s'"), மதிப்பு);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		திரும்பு xstrdup(மதிப்பு);
	}

	BUG("cannot normalize type %d", type);
}

கட்டமைப்பு get_color_config_data {
	முழு get_color_found;
	மாறிலி எழுத்து *get_color_slot;
	எழுத்து parsed_color[COLOR_MAXLEN];
};

நிலையான முழு git_get_color_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
				மாறிலி கட்டமைப்பு config_context *ctx UNUSED,
				வெறுமை *cb)
{
	கட்டமைப்பு get_color_config_data *data = cb;

	எனில் (!strcmp(var, data->get_color_slot)) {
		எனில் (!மதிப்பு)
			config_error_nonbool(var);
		எனில் (color_parse(மதிப்பு, data->parsed_color) < 0)
			திரும்பு -1;
		data->get_color_found = 1;
	}
	திரும்பு 0;
}

நிலையான முழு get_color(மாறிலி கட்டமைப்பு config_location_options *opts,
		      மாறிலி எழுத்து *var, மாறிலி எழுத்து *def_color)
{
	கட்டமைப்பு get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	முழு ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	எனில் (!data.get_color_found && def_color) {
		எனில் (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			செல் out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	திரும்பு ret;
}

கட்டமைப்பு get_colorbool_config_data {
	எண்ணிக்கை git_colorbool get_colorbool_found;
	எண்ணிக்கை git_colorbool get_diff_color_found;
	எண்ணிக்கை git_colorbool get_color_ui_found;
	மாறிலி எழுத்து *get_colorbool_slot;
};

நிலையான முழு git_get_colorbool_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
				    மாறிலி கட்டமைப்பு config_context *ctx UNUSED,
				    வெறுமை *cb)
{
	கட்டமைப்பு get_colorbool_config_data *data = cb;

	எனில் (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, மதிப்பு);
	இல்லையெனில் எனில் (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, மதிப்பு);
	இல்லையெனில் எனில் (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, மதிப்பு);
	திரும்பு 0;
}

நிலையான முழு get_colorbool(மாறிலி கட்டமைப்பு config_location_options *opts,
			 மாறிலி எழுத்து *var, முழு print)
{
	கட்டமைப்பு get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	எனில் (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		எனில் (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		எனில் (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	எனில் (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	எனில் (print) {
		printf("%s\n", result ? "true" : "false");
		திரும்பு 0;
	} இல்லையெனில்
		திரும்பு result ? 0 : 1;
}

நிலையான வெறுமை check_write(மாறிலி கட்டமைப்பு git_config_source *source)
{
	எனில் (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	எனில் (source->use_stdin)
		die(_("writing to stdin is not supported"));

	எனில் (source->blob)
		die(_("writing config blobs is not supported"));
}

கட்டமைப்பு urlmatch_current_candidate_மதிப்பு {
	எழுத்து மதிப்பு_is_null;
	கட்டமைப்பு strbuf மதிப்பு;
	கட்டமைப்பு சாவி_மதிப்பு_info kvi;
};

நிலையான முழு urlmatch_collect_fn(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			       மாறிலி கட்டமைப்பு config_context *ctx,
			       வெறுமை *cb)
{
	கட்டமைப்பு string_list *மதிப்புகள் = cb;
	கட்டமைப்பு string_list_item *item = string_list_செருகு(மதிப்புகள், var);
	கட்டமைப்பு urlmatch_current_candidate_மதிப்பு *matched = item->util;
	மாறிலி கட்டமைப்பு சாவி_மதிப்பு_info *kvi = ctx->kvi;

	எனில் (!matched) {
		matched = xmalloc(அளவு(*matched));
		strbuf_init(&matched->மதிப்பு, 0);
		item->util = matched;
	} இல்லையெனில் {
		strbuf_reset(&matched->மதிப்பு);
	}
	matched->kvi = *kvi;

	எனில் (மதிப்பு) {
		strbuf_addstr(&matched->மதிப்பு, மதிப்பு);
		matched->மதிப்பு_is_null = 0;
	} இல்லையெனில் {
		matched->மதிப்பு_is_null = 1;
	}
	திரும்பு 0;
}

நிலையான முழு get_urlmatch(மாறிலி கட்டமைப்பு config_location_options *opts,
			மாறிலி கட்டமைப்பு config_display_options *_display_opts,
			மாறிலி எழுத்து *var, மாறிலி எழுத்து *url)
{
	முழு ret;
	எழுத்து *section;
	எழுத்து *section_tail;
	கட்டமைப்பு config_display_options display_opts = *_display_opts;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு urlmatch_config config = URLMATCH_CONFIG_INIT;
	கட்டமைப்பு string_list மதிப்புகள் = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = வெற்று;
	config.cb = &மதிப்புகள்;

	எனில் (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	எனில் (section_tail) {
		*section_tail = '\0';
		config.சாவி = section_tail + 1;
		display_opts.show_சாவிகள் = 0;
	} இல்லையெனில் {
		config.சாவி = வெற்று;
		display_opts.show_சாவிகள் = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !மதிப்புகள்.nr;

	for_each_string_list_item(item, &மதிப்புகள்) {
		கட்டமைப்பு urlmatch_current_candidate_மதிப்பு *matched = item->util;
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		முழு status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->மதிப்பு_is_null ? வெற்று : matched->மதிப்பு.buf,
				       &matched->kvi, 0);
		எனில் (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->மதிப்பு);
	}
	urlmatch_config_release(&config);
	string_list_clear(&மதிப்புகள், 1);
	விடுவி(config.url.url);

	விடுவி(section);
	திரும்பு ret;
}

நிலையான எழுத்து *default_user_config(வெறுமை)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	திரும்பு strbuf_detach(&buf, வெற்று);
}

நிலையான வெறுமை location_options_init(கட்டமைப்பு config_location_options *opts,
				  மாறிலி எழுத்து *prefix)
{
	எனில் (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	எனில் (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	எனில் (!startup_info->have_repository) {
		எனில் (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		எனில் (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		எனில் (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	எனில் (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = வெற்று;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	எனில் (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		எனில் (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} இல்லையெனில் எனில் (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} இல்லையெனில் எனில் (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} இல்லையெனில் எனில் (opts->use_worktree_config) {
		கட்டமைப்பு worktree **worktrees = get_worktrees(the_repository);
		எனில் (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		இல்லையெனில் எனில் (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		இல்லையெனில்
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} இல்லையெனில் எனில் (opts->source.file) {
		எனில் (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} இல்லையெனில் எனில் (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	எனில் (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	இல்லையெனில்
		opts->options.respect_includes = opts->respect_includes_opt;
	எனில் (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

நிலையான வெறுமை location_options_release(கட்டமைப்பு config_location_options *opts)
{
	விடுவி(opts->file_to_free);
}

நிலையான வெறுமை display_options_init(கட்டமைப்பு config_display_options *opts)
{
	எனில் (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->சாவி_delim = '\n';
	}
}

நிலையான வெறுமை display_options_init_list(கட்டமைப்பு config_display_options *opts)
{
	opts->show_சாவிகள் = 1;

	எனில் (opts->end_nul) {
		display_options_init(opts);
	} இல்லையெனில் {
		opts->term = '\n';
		opts->delim = ' ';
		opts->சாவி_delim = '=';
	}
}

நிலையான முழு cmd_config_list(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	கட்டமைப்பு config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	கட்டமைப்பு option opts[] = {
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

	எனில் (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		எனில் (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		இல்லையெனில்
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	திரும்பு 0;
}

நிலையான முழு cmd_config_get(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			  கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	கட்டமைப்பு config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	மாறிலி எழுத்து *மதிப்பு_pattern = வெற்று, *url = வெற்று;
	முழு flags = 0;
	unsigned get_மதிப்பு_flags = 0;
	கட்டமைப்பு option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_மதிப்பு_flags, N_("return all values for multi-valued config options"), GET_மதிப்பு_ALL),
		OPT_BIT(0, "regexp", &get_மதிப்பு_flags, N_("interpret the name as a regular expression"), GET_மதிப்பு_சாவி_REGEXP),
		OPT_STRING(0, "value", &மதிப்பு_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_மதிப்பு),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_மதிப்பு,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	முழு ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	எனில் ((flags & CONFIG_FLAGS_FIXED_மதிப்பு) && !மதிப்பு_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	எனில் (display_opts.default_மதிப்பு &&
	    ((get_மதிப்பு_flags & GET_மதிப்பு_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	எனில் (url && ((get_மதிப்பு_flags & GET_மதிப்பு_ALL) ||
		    (get_மதிப்பு_flags & GET_மதிப்பு_சாவி_REGEXP) ||
		    மதிப்பு_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	எனில் (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	எனில் (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	இல்லையெனில் எனில் (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_மதிப்பு)
		ret = get_color(&location_opts, "", display_opts.default_மதிப்பு);
	இல்லையெனில்
		ret = get_மதிப்பு(&location_opts, &display_opts, argv[0], மதிப்பு_pattern,
				get_மதிப்பு_flags, flags);

	location_options_release(&location_opts);
	திரும்பு ret;
}

நிலையான முழு cmd_config_set(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			  கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	மாறிலி எழுத்து *மதிப்பு_pattern = வெற்று, *comment_arg = வெற்று;
	எழுத்து *comment = வெற்று;
	முழு flags = 0, append = 0, type = 0;
	கட்டமைப்பு option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &மதிப்பு_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_மதிப்பு),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	கட்டமைப்பு சாவி_மதிப்பு_info default_kvi = KVI_INIT;
	எழுத்து *மதிப்பு;
	முழு ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	எனில் (argc == 1)
		die_missing_set_மதிப்பு(argv[0]);
	check_argc(argc, 2, 2);

	எனில் ((flags & CONFIG_FLAGS_FIXED_மதிப்பு) && !மதிப்பு_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	எனில் (append && மதிப்பு_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	எனில் (append)
		மதிப்பு_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	மதிப்பு = normalize_மதிப்பு(argv[0], argv[1], type, &default_kvi);

	எனில் ((flags & CONFIG_FLAGS_MULTI_REPLACE) || மதிப்பு_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], மதிப்பு, மதிப்பு_pattern,
							      comment, flags);
	} இல்லையெனில் {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, மதிப்பு);
		எனில் (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	விடுவி(comment);
	விடுவி(மதிப்பு);
	திரும்பு ret;
}

நிலையான முழு cmd_config_unset(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			    கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	மாறிலி எழுத்து *மதிப்பு_pattern = வெற்று;
	முழு flags = 0;
	கட்டமைப்பு option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &மதிப்பு_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_மதிப்பு),
		OPT_END(),
	};
	முழு ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	எனில் ((flags & CONFIG_FLAGS_FIXED_மதிப்பு) && !மதிப்பு_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	எனில் ((flags & CONFIG_FLAGS_MULTI_REPLACE) || மதிப்பு_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], வெற்று, மதிப்பு_pattern,
							      வெற்று, flags);
	இல்லையெனில்
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     வெற்று, வெற்று);

	location_options_release(&location_opts);
	திரும்பு ret;
}

நிலையான முழு cmd_config_rename_section(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				     கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	கட்டமைப்பு option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	முழு ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	எனில் (ret < 0)
		செல் out;
	இல்லையெனில் எனில் (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	திரும்பு ret;
}

நிலையான முழு cmd_config_remove_section(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				     கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	கட்டமைப்பு option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	முழு ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], வெற்று);
	எனில் (ret < 0)
		செல் out;
	இல்லையெனில் எனில் (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	திரும்பு ret;
}

நிலையான முழு show_editor(கட்டமைப்பு config_location_options *opts)
{
	எழுத்து *config_file;

	எனில் (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	எனில் (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	எனில் (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, வெற்று);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	எனில் (opts->use_global_config) {
		முழு fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		எனில் (fd >= 0) {
			எழுத்து *content = default_user_config();
			write_str_in_full(fd, content);
			விடுவி(content);
			close(fd);
		}
		இல்லையெனில் எனில் (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	எனில் (launch_editor(config_file, வெற்று, வெற்று)) {
		விடுவி(config_file);
		திரும்பு -1;
	}
	விடுவி(config_file);

	திரும்பு 0;
}

நிலையான முழு cmd_config_edit(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	கட்டமைப்பு option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	முழு ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	திரும்பு ret;
}

நிலையான முழு cmd_config_actions(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix)
{
	எண்ணிக்கை {
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
	கட்டமைப்பு config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	கட்டமைப்பு config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	மாறிலி எழுத்து *comment_arg = வெற்று;
	முழு actions = 0;
	unsigned flags = 0;
	கட்டமைப்பு option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_மதிப்பு,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_மதிப்பு),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	எழுத்து *மதிப்பு = வெற்று, *comment = வெற்று;
	முழு ret = 0;
	முழு actions_implicit;
	கட்டமைப்பு சாவி_மதிப்பு_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	எனில் ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	எனில் (actions_implicit)
		தேர்வு (argc) {
		நிலை 1: actions = ACTION_GET; நிறுத்து;
		நிலை 2: actions = ACTION_SET; நிறுத்து;
		நிலை 3: actions = ACTION_SET_ALL; நிறுத்து;
		இயல்பு:
			error(_("no action specified"));
			exit(129);
		}
	எனில் (actions_implicit && argc == 1) {
		மாறிலி எழுத்து *last_dot = strrchr(argv[0], '.');
		எனில் (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_மதிப்பு(argv[0]);
	}
	எனில் (display_opts.omit_மதிப்புகள் &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	எனில் (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	எனில் (display_opts.default_மதிப்பு && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	எனில் (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	எனில் (flags & CONFIG_FLAGS_FIXED_மதிப்பு) {
		முழு allowed_usage = 0;

		தேர்வு (actions) {
		/* git config --get <name> <value-pattern> */
		நிலை ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		நிலை ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		நிலை ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		நிலை ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		நிலை ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			நிறுத்து;

		/* git config <name> <value> <value-pattern> */
		நிலை ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		நிலை ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			நிறுத்து;

		/* other options don't allow --fixed-value */
		}

		எனில் (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	எனில் (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	எனில் (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		எனில் (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			எனில் (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			இல்லையெனில்
				die(_("error processing config file(s)"));
		}
	}
	இல்லையெனில் எனில் (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	இல்லையெனில் எனில் (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		மதிப்பு = normalize_மதிப்பு(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, மதிப்பு);
		எனில் (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	இல்லையெனில் எனில் (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		மதிப்பு = normalize_மதிப்பு(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], மதிப்பு, argv[2],
							      comment, flags);
	}
	இல்லையெனில் எனில் (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		மதிப்பு = normalize_மதிப்பு(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], மதிப்பு,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	இல்லையெனில் எனில் (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		மதிப்பு = normalize_மதிப்பு(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], மதிப்பு, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	இல்லையெனில் எனில் (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_மதிப்பு(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	இல்லையெனில் எனில் (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_மதிப்பு(&location_opts, &display_opts, argv[0], argv[1],
				GET_மதிப்பு_ALL, flags);
	}
	இல்லையெனில் எனில் (actions == ACTION_GET_REGEXP) {
		display_opts.show_சாவிகள் = 1;
		check_argc(argc, 1, 2);
		ret = get_மதிப்பு(&location_opts, &display_opts, argv[0], argv[1],
				GET_மதிப்பு_ALL|GET_மதிப்பு_சாவி_REGEXP, flags);
	}
	இல்லையெனில் எனில் (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	இல்லையெனில் எனில் (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		எனில் (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], வெற்று, argv[1],
								      வெற்று, flags);
		இல்லையெனில்
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], வெற்று, வெற்று);
	}
	இல்லையெனில் எனில் (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], வெற்று, argv[1],
							      வெற்று, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	இல்லையெனில் எனில் (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		எனில் (ret < 0)
			செல் out;
		இல்லையெனில் எனில் (!ret)
			die(_("no such section: %s"), argv[0]);
		இல்லையெனில்
			ret = 0;
	}
	இல்லையெனில் எனில் (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], வெற்று);
		எனில் (ret < 0)
			செல் out;
		இல்லையெனில் எனில் (!ret)
			die(_("no such section: %s"), argv[0]);
		இல்லையெனில்
			ret = 0;
	}
	இல்லையெனில் எனில் (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	இல்லையெனில் எனில் (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		எனில் (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	விடுவி(comment);
	விடுவி(மதிப்பு);
	திரும்பு ret;
}

முழு cmd_config(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *subcommand = வெற்று;
	கட்டமைப்பு option subcommand_opts[] = {
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
	எனில் (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		திரும்பு subcommand(argc, argv, prefix, repo);
	}

	திரும்பு cmd_config_actions(argc, argv, prefix);
}
