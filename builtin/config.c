#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "color.h"
#شامل "date.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "ident.h"
#شامل "parse-options.h"
#شامل "urlmatch.h"
#شامل "path.h"
#شامل "quote.h"
#شامل "setup.h"
#شامل "strbuf.h"
#شامل "worktree.h"

ایستا ثابت نویسه *ثابت builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	پوچ
};

ایستا ثابت نویسه *ثابت builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	پوچ
};

ایستا ثابت نویسه *ثابت builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	پوچ
};

ایستا ثابت نویسه *ثابت builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	پوچ
};

ایستا ثابت نویسه *ثابت builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	پوچ
};

ایستا ثابت نویسه *ثابت builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	پوچ
};

ایستا ثابت نویسه *ثابت builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	پوچ
};

ایستا ثابت نویسه *ثابت builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	پوچ
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

ساختار config_location_options {
	ساختار git_config_source source;
	ساختار config_options options;
	نویسه *file_to_free;
	صحیح use_global_config;
	صحیح use_system_config;
	صحیح use_local_config;
	صحیح use_worktree_config;
	صحیح respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

شمارش config_type {
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
	OPT_CALLBACK_مقدار(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_مقدار(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_مقدار(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_مقدار(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_مقدار(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_مقدار(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_مقادیر, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_کلیدها, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

ساختار config_display_options {
	صحیح end_nul;
	صحیح omit_مقادیر;
	صحیح show_origin;
	صحیح show_scope;
	صحیح show_کلیدها;
	شمارش config_type type;
	نویسه *default_مقدار;
	/* Populated via `display_options_init()`. */
	صحیح term;
	صحیح delim;
	صحیح کلید_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.کلید_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_مقدار(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.مقدار = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ایستا صحیح option_parse_type(ثابت ساختار option *opt, ثابت نویسه *arg,
			     صحیح unset)
{
	صحیح new_type, *to_type;

	اگر (unset) {
		*((صحیح *) opt->مقدار) = 0;
		بازگشت 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	اگر (!new_type) {
		اگر (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		وگرنه اگر (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		وگرنه اگر (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		وگرنه اگر (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		وگرنه اگر (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		وگرنه اگر (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		وگرنه اگر (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		وگرنه
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->مقدار;
	اگر (*to_type && *to_type != new_type) {
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

	بازگشت 0;
}

ایستا تهی check_argc(صحیح argc, صحیح min, صحیح max)
{
	اگر (argc >= min && argc <= max)
		بازگشت;
	اگر (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	وگرنه
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

ایستا NORETURN تهی die_missing_set_مقدار(ثابت نویسه *arg)
{
	ثابت نویسه *last_dot = strrchr(arg, '.');
	ثابت نویسه *eq = last_dot ? strchr(last_dot + 1, '=') : پوچ;
	نویسه *prefix = eq ? xstrndup(arg, eq - arg) : پوچ;

	اگر (prefix && git_config_کلید_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} وگرنه اگر (git_config_کلید_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} وگرنه {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	آزاد(prefix);
	exit(129);
}

ایستا تهی show_config_origin(ثابت ساختار config_display_options *opts,
			       ثابت ساختار کلید_مقدار_info *kvi,
			       ساختار strbuf *buf)
{
	ثابت نویسه term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	اگر (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	وگرنه
		quote_c_style(kvi->filename ? kvi->filename : "", buf, پوچ, 0);
	strbuf_addch(buf, term);
}

ایستا تهی show_config_scope(ثابت ساختار config_display_options *opts,
			      ثابت ساختار کلید_مقدار_info *kvi,
			      ساختار strbuf *buf)
{
	ثابت نویسه term = opts->end_nul ? '\0' : '\t';
	ثابت نویسه *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

ساختار strbuf_list {
	ساختار strbuf *اقلام;
	صحیح nr;
	صحیح alloc;
};

ایستا صحیح format_config_int64(ساختار strbuf *buf,
			       ثابت نویسه *کلید_2,
			       ثابت نویسه *مقدار_2,
			       ثابت ساختار کلید_مقدار_info *kvi,
			       صحیح gently)
{
	int64_t v = 0;
	اگر (gently) {
		اگر (!git_parse_int64(مقدار_2, &v))
			بازگشت -1;
	} وگرنه {
		/* may die() */
		v = git_config_int64(کلید_2, مقدار_2 ? مقدار_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	بازگشت 0;
}

ایستا صحیح format_config_bool(ساختار strbuf *buf,
			      ثابت نویسه *کلید_2,
			      ثابت نویسه *مقدار_2,
			      صحیح gently)
{
	صحیح v = 0;
	اگر (gently) {
		اگر ((v = git_parse_maybe_bool(مقدار_2)) < 0)
			بازگشت -1;
	} وگرنه {
		/* may die() */
		v = git_config_bool(کلید_2, مقدار_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	بازگشت 0;
}

ایستا صحیح format_config_bool_or_int(ساختار strbuf *buf,
				     ثابت نویسه *کلید_2,
				     ثابت نویسه *مقدار_2,
				     ثابت ساختار کلید_مقدار_info *kvi,
				     صحیح gently)
{
	صحیح v, is_bool = 0;

	اگر (gently) {
		v = git_parse_maybe_bool_text(مقدار_2);

		اگر (v >= 0)
			is_bool = 1;
		وگرنه اگر (!git_parse_int(مقدار_2, &v))
			بازگشت -1;
	} وگرنه {
		v = git_config_bool_or_int(کلید_2, مقدار_2, kvi,
					   &is_bool);
	}

	اگر (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	وگرنه
		strbuf_addf(buf, "%d", v);

	بازگشت 0;
}

/* This mode is always gentle. */
ایستا صحیح format_config_bool_or_str(ساختار strbuf *buf,
				     ثابت نویسه *مقدار_2)
{
	صحیح v = git_parse_maybe_bool(مقدار_2);
	اگر (v < 0)
		strbuf_addstr(buf, مقدار_2);
	وگرنه
		strbuf_addstr(buf, v ? "true" : "false");
	بازگشت 0;
}

ایستا صحیح format_config_path(ساختار strbuf *buf,
			      ثابت نویسه *کلید_2,
			      ثابت نویسه *مقدار_2,
			      صحیح gently)
{
	نویسه *v;

	اگر (git_config_pathname(&v, کلید_2, مقدار_2) < 0)
		بازگشت -1;

	اگر (v)
		strbuf_addstr(buf, v);
	وگرنه
		بازگشت gently ? -1 : 1; /* :(optional)no-such-file */

	آزاد(v);
	بازگشت 0;
}

ایستا صحیح format_config_expiry_date(ساختار strbuf *buf,
				     ثابت نویسه *کلید_2,
				     ثابت نویسه *مقدار_2,
				     صحیح quietly)
{
	timestamp_t t;
	اگر (quietly) {
		اگر (parse_expiry_date(مقدار_2, &t))
			بازگشت -1;
	} وگرنه اگر (git_config_expiry_date(&t, کلید_2, مقدار_2) < 0) {
		بازگشت -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	بازگشت 0;
}

ایستا صحیح format_config_color(ساختار strbuf *buf,
			       ثابت نویسه *کلید_2,
			       ثابت نویسه *مقدار_2,
			       صحیح gently)
{
	نویسه v[COLOR_MAXLEN];

	اگر (gently) {
		اگر (color_parse_quietly(مقدار_2, v) < 0)
			بازگشت -1;
	} وگرنه اگر (git_config_color(v, کلید_2, مقدار_2) < 0) {
		بازگشت -1;
	}

	strbuf_addstr(buf, v);
	بازگشت 0;
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
ایستا صحیح format_config(ثابت ساختار config_display_options *opts,
			 ساختار strbuf *buf, ثابت نویسه *کلید_2,
			 ثابت نویسه *مقدار_2, ثابت ساختار کلید_مقدار_info *kvi,
			 صحیح gently)
{
	صحیح res = 0;
	اگر (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	اگر (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	اگر (opts->show_کلیدها)
		strbuf_addstr(buf, کلید_2);

	اگر (opts->omit_مقادیر)
		برو terminator;

	اگر (opts->show_کلیدها)
		strbuf_addch(buf, opts->کلید_delim);

	انتخاب (opts->type) {
	حالت TYPE_INT:
		res = format_config_int64(buf, کلید_2, مقدار_2, kvi, gently);
		بشکن;

	حالت TYPE_BOOL:
		res = format_config_bool(buf, کلید_2, مقدار_2, gently);
		بشکن;

	حالت TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, کلید_2, مقدار_2, kvi, gently);
		بشکن;

	حالت TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, مقدار_2);
		بشکن;

	حالت TYPE_PATH:
		res = format_config_path(buf, کلید_2, مقدار_2, gently);
		بشکن;

	حالت TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, کلید_2, مقدار_2, gently);
		بشکن;

	حالت TYPE_COLOR:
		res = format_config_color(buf, کلید_2, مقدار_2, gently);
		بشکن;

	حالت TYPE_NONE:
		اگر (مقدار_2) {
			strbuf_addstr(buf, مقدار_2);
		} وگرنه {
			/* Just show the key name; back out delimiter */
			اگر (opts->show_کلیدها)
				strbuf_setlen(buf, buf->len - 1);
		}
		بشکن;

	پیشفرض:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	بازگشت res;
}

ایستا صحیح show_all_config(ثابت نویسه *کلید_2, ثابت نویسه *مقدار_2,
			   ثابت ساختار config_context *ctx,
			   تهی *cb)
{
	ثابت ساختار config_display_options *opts = cb;
	ثابت ساختار کلید_مقدار_info *kvi = ctx->kvi;
	ساختار strbuf formatted = STRBUF_INIT;

	اگر (format_config(opts, &formatted, کلید_2, مقدار_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	بازگشت 0;
}

#define GET_مقدار_ALL        (1 << 0)
#define GET_مقدار_کلید_REGEXP (1 << 1)

ساختار collect_config_data {
	ثابت ساختار config_display_options *display_opts;
	ساختار strbuf_list *مقادیر;
	ثابت نویسه *مقدار_pattern;
	ثابت نویسه *کلید;
	regex_t *regexp;
	regex_t *کلید_regexp;
	صحیح do_not_match;
	unsigned get_مقدار_flags;
	unsigned flags;
};

ایستا صحیح collect_config(ثابت نویسه *کلید_2, ثابت نویسه *مقدار_2,
			  ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار collect_config_data *data = cb;
	ساختار strbuf_list *مقادیر = data->مقادیر;
	ثابت ساختار کلید_مقدار_info *kvi = ctx->kvi;
	صحیح status;

	اگر (!(data->get_مقدار_flags & GET_مقدار_کلید_REGEXP) &&
	    strcmp(کلید_2, data->کلید))
		بازگشت 0;
	اگر ((data->get_مقدار_flags & GET_مقدار_کلید_REGEXP) &&
	    regexec(data->کلید_regexp, کلید_2, 0, پوچ, 0))
		بازگشت 0;
	اگر ((data->flags & CONFIG_FLAGS_FIXED_مقدار) &&
	    strcmp(data->مقدار_pattern, (مقدار_2?مقدار_2:"")))
		بازگشت 0;
	اگر (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (مقدار_2?مقدار_2:""), 0, پوچ, 0)))
		بازگشت 0;

	ALLOC_GROW(مقادیر->اقلام, مقادیر->nr + 1, مقادیر->alloc);
	strbuf_init(&مقادیر->اقلام[مقادیر->nr], 0);

	status = format_config(data->display_opts, &مقادیر->اقلام[مقادیر->nr++],
			       کلید_2, مقدار_2, kvi, 0);
	اگر (status < 0)
		بازگشت status;
	اگر (status) {
		strbuf_release(&مقادیر->اقلام[--مقادیر->nr]);
		status = 0;
	}
	بازگشت status;
}

ایستا صحیح get_مقدار(ثابت ساختار config_location_options *opts,
		     ثابت ساختار config_display_options *display_opts,
		     ثابت نویسه *کلید_2, ثابت نویسه *regex_,
		     unsigned get_مقدار_flags, unsigned flags)
{
	صحیح ret = CONFIG_GENERIC_ERROR;
	ساختار strbuf_list مقادیر = {پوچ};
	ساختار collect_config_data data = {
		.display_opts = display_opts,
		.مقادیر = &مقادیر,
		.get_مقدار_flags = get_مقدار_flags,
		.flags = flags,
	};
	نویسه *کلید = پوچ;
	صحیح i;

	اگر (get_مقدار_flags & GET_مقدار_کلید_REGEXP) {
		نویسه *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		کلید = xstrdup(کلید_2);
		برای (tl = کلید + strlen(کلید) - 1;
		     tl >= کلید && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		برای (tl = کلید; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.کلید_regexp = (regex_t*)xmalloc(اندازه(regex_t));
		اگر (regcomp(data.کلید_regexp, کلید, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), کلید_2);
			FREE_AND_NULL(data.کلید_regexp);
			ret = CONFIG_INVALID_PATTERN;
			برو free_strings;
		}
	} وگرنه {
		اگر (git_config_parse_کلید(کلید_2, &کلید, پوچ)) {
			ret = CONFIG_INVALID_کلید;
			برو free_strings;
		}

		data.کلید = کلید;
	}

	اگر (regex_ && (flags & CONFIG_FLAGS_FIXED_مقدار))
		data.مقدار_pattern = regex_;
	وگرنه اگر (regex_) {
		اگر (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(اندازه(regex_t));
		اگر (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			برو free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	اگر (!مقادیر.nr && display_opts->default_مقدار) {
		ساختار کلید_مقدار_info kvi = KVI_INIT;
		ساختار strbuf *item;
		صحیح status;

		kvi_from_param(&kvi);
		ALLOC_GROW(مقادیر.اقلام, مقادیر.nr + 1, مقادیر.alloc);
		item = &مقادیر.اقلام[مقادیر.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, کلید_2,
				       display_opts->default_مقدار, &kvi, 0);
		اگر (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_مقدار);
		اگر (status) {
			/* default was a missing optional value */
			مقادیر.nr--;
			strbuf_release(item);
		}
	}

	ret = !مقادیر.nr;

	برای (i = 0; i < مقادیر.nr; i++) {
		ساختار strbuf *buf = مقادیر.اقلام + i;
		اگر ((get_مقدار_flags & GET_مقدار_ALL) || i == مقادیر.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	آزاد(مقادیر.اقلام);

free_strings:
	آزاد(کلید);
	اگر (data.کلید_regexp) {
		regfree(data.کلید_regexp);
		آزاد(data.کلید_regexp);
	}
	اگر (data.regexp) {
		regfree(data.regexp);
		آزاد(data.regexp);
	}

	بازگشت ret;
}

ایستا نویسه *normalize_مقدار(ثابت نویسه *کلید, ثابت نویسه *مقدار,
			     صحیح type, ساختار کلید_مقدار_info *kvi)
{
	اگر (!مقدار)
		بازگشت پوچ;

	اگر (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		بازگشت xstrdup(مقدار);
	اگر (type == TYPE_INT)
		بازگشت xstrfmt("%"PRId64, git_config_int64(کلید, مقدار, kvi));
	اگر (type == TYPE_BOOL)
		بازگشت xstrdup(git_config_bool(کلید, مقدار) ?  "true" : "false");
	اگر (type == TYPE_BOOL_OR_INT) {
		صحیح is_bool, v;
		v = git_config_bool_or_int(کلید, مقدار, kvi, &is_bool);
		اگر (!is_bool)
			بازگشت xstrfmt("%d", v);
		وگرنه
			بازگشت xstrdup(v ? "true" : "false");
	}
	اگر (type == TYPE_BOOL_OR_STR) {
		صحیح v = git_parse_maybe_bool(مقدار);
		اگر (v < 0)
			بازگشت xstrdup(مقدار);
		وگرنه
			بازگشت xstrdup(v ? "true" : "false");
	}
	اگر (type == TYPE_COLOR) {
		نویسه v[COLOR_MAXLEN];
		اگر (git_config_color(v, کلید, مقدار))
			die(_("cannot parse color '%s'"), مقدار);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		بازگشت xstrdup(مقدار);
	}

	BUG("cannot normalize type %d", type);
}

ساختار get_color_config_data {
	صحیح get_color_found;
	ثابت نویسه *get_color_slot;
	نویسه parsed_color[COLOR_MAXLEN];
};

ایستا صحیح git_get_color_config(ثابت نویسه *var, ثابت نویسه *مقدار,
				ثابت ساختار config_context *ctx UNUSED,
				تهی *cb)
{
	ساختار get_color_config_data *data = cb;

	اگر (!strcmp(var, data->get_color_slot)) {
		اگر (!مقدار)
			config_error_nonbool(var);
		اگر (color_parse(مقدار, data->parsed_color) < 0)
			بازگشت -1;
		data->get_color_found = 1;
	}
	بازگشت 0;
}

ایستا صحیح get_color(ثابت ساختار config_location_options *opts,
		      ثابت نویسه *var, ثابت نویسه *def_color)
{
	ساختار get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	صحیح ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	اگر (!data.get_color_found && def_color) {
		اگر (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			برو out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	بازگشت ret;
}

ساختار get_colorbool_config_data {
	شمارش git_colorbool get_colorbool_found;
	شمارش git_colorbool get_diff_color_found;
	شمارش git_colorbool get_color_ui_found;
	ثابت نویسه *get_colorbool_slot;
};

ایستا صحیح git_get_colorbool_config(ثابت نویسه *var, ثابت نویسه *مقدار,
				    ثابت ساختار config_context *ctx UNUSED,
				    تهی *cb)
{
	ساختار get_colorbool_config_data *data = cb;

	اگر (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, مقدار);
	وگرنه اگر (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, مقدار);
	وگرنه اگر (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, مقدار);
	بازگشت 0;
}

ایستا صحیح get_colorbool(ثابت ساختار config_location_options *opts,
			 ثابت نویسه *var, صحیح print)
{
	ساختار get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	اگر (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		اگر (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		اگر (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	اگر (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	اگر (print) {
		printf("%s\n", result ? "true" : "false");
		بازگشت 0;
	} وگرنه
		بازگشت result ? 0 : 1;
}

ایستا تهی check_write(ثابت ساختار git_config_source *source)
{
	اگر (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	اگر (source->use_stdin)
		die(_("writing to stdin is not supported"));

	اگر (source->blob)
		die(_("writing config blobs is not supported"));
}

ساختار urlmatch_current_candidate_مقدار {
	نویسه مقدار_is_null;
	ساختار strbuf مقدار;
	ساختار کلید_مقدار_info kvi;
};

ایستا صحیح urlmatch_collect_fn(ثابت نویسه *var, ثابت نویسه *مقدار,
			       ثابت ساختار config_context *ctx,
			       تهی *cb)
{
	ساختار string_list *مقادیر = cb;
	ساختار string_list_item *item = string_list_درج(مقادیر, var);
	ساختار urlmatch_current_candidate_مقدار *matched = item->util;
	ثابت ساختار کلید_مقدار_info *kvi = ctx->kvi;

	اگر (!matched) {
		matched = xmalloc(اندازه(*matched));
		strbuf_init(&matched->مقدار, 0);
		item->util = matched;
	} وگرنه {
		strbuf_reset(&matched->مقدار);
	}
	matched->kvi = *kvi;

	اگر (مقدار) {
		strbuf_addstr(&matched->مقدار, مقدار);
		matched->مقدار_is_null = 0;
	} وگرنه {
		matched->مقدار_is_null = 1;
	}
	بازگشت 0;
}

ایستا صحیح get_urlmatch(ثابت ساختار config_location_options *opts,
			ثابت ساختار config_display_options *_display_opts,
			ثابت نویسه *var, ثابت نویسه *url)
{
	صحیح ret;
	نویسه *section;
	نویسه *section_tail;
	ساختار config_display_options display_opts = *_display_opts;
	ساختار string_list_item *item;
	ساختار urlmatch_config config = URLMATCH_CONFIG_INIT;
	ساختار string_list مقادیر = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = پوچ;
	config.cb = &مقادیر;

	اگر (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	اگر (section_tail) {
		*section_tail = '\0';
		config.کلید = section_tail + 1;
		display_opts.show_کلیدها = 0;
	} وگرنه {
		config.کلید = پوچ;
		display_opts.show_کلیدها = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !مقادیر.nr;

	for_each_string_list_item(item, &مقادیر) {
		ساختار urlmatch_current_candidate_مقدار *matched = item->util;
		ساختار strbuf buf = STRBUF_INIT;
		صحیح status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->مقدار_is_null ? پوچ : matched->مقدار.buf,
				       &matched->kvi, 0);
		اگر (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->مقدار);
	}
	urlmatch_config_release(&config);
	string_list_clear(&مقادیر, 1);
	آزاد(config.url.url);

	آزاد(section);
	بازگشت ret;
}

ایستا نویسه *default_user_config(تهی)
{
	ساختار strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	بازگشت strbuf_detach(&buf, پوچ);
}

ایستا تهی location_options_init(ساختار config_location_options *opts,
				  ثابت نویسه *prefix)
{
	اگر (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	اگر (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	اگر (!startup_info->have_repository) {
		اگر (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		اگر (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		اگر (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	اگر (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = پوچ;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	اگر (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		اگر (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} وگرنه اگر (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} وگرنه اگر (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} وگرنه اگر (opts->use_worktree_config) {
		ساختار worktree **worktrees = get_worktrees(the_repository);
		اگر (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		وگرنه اگر (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		وگرنه
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} وگرنه اگر (opts->source.file) {
		اگر (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} وگرنه اگر (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	اگر (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	وگرنه
		opts->options.respect_includes = opts->respect_includes_opt;
	اگر (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ایستا تهی location_options_release(ساختار config_location_options *opts)
{
	آزاد(opts->file_to_free);
}

ایستا تهی display_options_init(ساختار config_display_options *opts)
{
	اگر (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->کلید_delim = '\n';
	}
}

ایستا تهی display_options_init_list(ساختار config_display_options *opts)
{
	opts->show_کلیدها = 1;

	اگر (opts->end_nul) {
		display_options_init(opts);
	} وگرنه {
		opts->term = '\n';
		opts->delim = ' ';
		opts->کلید_delim = '=';
	}
}

ایستا صحیح cmd_config_list(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساختار config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ساختار option opts[] = {
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

	اگر (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		اگر (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		وگرنه
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	بازگشت 0;
}

ایستا صحیح cmd_config_get(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			  ساختار repository *repo UNUSED)
{
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساختار config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت نویسه *مقدار_pattern = پوچ, *url = پوچ;
	صحیح flags = 0;
	unsigned get_مقدار_flags = 0;
	ساختار option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_مقدار_flags, N_("return all values for multi-valued config options"), GET_مقدار_ALL),
		OPT_BIT(0, "regexp", &get_مقدار_flags, N_("interpret the name as a regular expression"), GET_مقدار_کلید_REGEXP),
		OPT_STRING(0, "value", &مقدار_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_مقدار,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	اگر ((flags & CONFIG_FLAGS_FIXED_مقدار) && !مقدار_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	اگر (display_opts.default_مقدار &&
	    ((get_مقدار_flags & GET_مقدار_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	اگر (url && ((get_مقدار_flags & GET_مقدار_ALL) ||
		    (get_مقدار_flags & GET_مقدار_کلید_REGEXP) ||
		    مقدار_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	اگر (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	اگر (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	وگرنه اگر (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_مقدار)
		ret = get_color(&location_opts, "", display_opts.default_مقدار);
	وگرنه
		ret = get_مقدار(&location_opts, &display_opts, argv[0], مقدار_pattern,
				get_مقدار_flags, flags);

	location_options_release(&location_opts);
	بازگشت ret;
}

ایستا صحیح cmd_config_set(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			  ساختار repository *repo UNUSED)
{
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت نویسه *مقدار_pattern = پوچ, *comment_arg = پوچ;
	نویسه *comment = پوچ;
	صحیح flags = 0, append = 0, type = 0;
	ساختار option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &مقدار_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	ساختار کلید_مقدار_info default_kvi = KVI_INIT;
	نویسه *مقدار;
	صحیح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (argc == 1)
		die_missing_set_مقدار(argv[0]);
	check_argc(argc, 2, 2);

	اگر ((flags & CONFIG_FLAGS_FIXED_مقدار) && !مقدار_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	اگر (append && مقدار_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	اگر (append)
		مقدار_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	مقدار = normalize_مقدار(argv[0], argv[1], type, &default_kvi);

	اگر ((flags & CONFIG_FLAGS_MULTI_REPLACE) || مقدار_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], مقدار, مقدار_pattern,
							      comment, flags);
	} وگرنه {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, مقدار);
		اگر (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	آزاد(comment);
	آزاد(مقدار);
	بازگشت ret;
}

ایستا صحیح cmd_config_unset(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			    ساختار repository *repo UNUSED)
{
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت نویسه *مقدار_pattern = پوچ;
	صحیح flags = 0;
	ساختار option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &مقدار_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_END(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	اگر ((flags & CONFIG_FLAGS_FIXED_مقدار) && !مقدار_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	اگر ((flags & CONFIG_FLAGS_MULTI_REPLACE) || مقدار_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], پوچ, مقدار_pattern,
							      پوچ, flags);
	وگرنه
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     پوچ, پوچ);

	location_options_release(&location_opts);
	بازگشت ret;
}

ایستا صحیح cmd_config_rename_section(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				     ساختار repository *repo UNUSED)
{
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساختار option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	اگر (ret < 0)
		برو out;
	وگرنه اگر (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	بازگشت ret;
}

ایستا صحیح cmd_config_remove_section(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				     ساختار repository *repo UNUSED)
{
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساختار option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], پوچ);
	اگر (ret < 0)
		برو out;
	وگرنه اگر (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	بازگشت ret;
}

ایستا صحیح show_editor(ساختار config_location_options *opts)
{
	نویسه *config_file;

	اگر (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	اگر (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	اگر (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, پوچ);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	اگر (opts->use_global_config) {
		صحیح fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		اگر (fd >= 0) {
			نویسه *content = default_user_config();
			write_str_in_full(fd, content);
			آزاد(content);
			close(fd);
		}
		وگرنه اگر (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	اگر (launch_editor(config_file, پوچ, پوچ)) {
		آزاد(config_file);
		بازگشت -1;
	}
	آزاد(config_file);

	بازگشت 0;
}

ایستا صحیح cmd_config_edit(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساختار option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	بازگشت ret;
}

ایستا صحیح cmd_config_actions(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix)
{
	شمارش {
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
	ساختار config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساختار config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت نویسه *comment_arg = پوچ;
	صحیح actions = 0;
	unsigned flags = 0;
	ساختار option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_مقدار,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	نویسه *مقدار = پوچ, *comment = پوچ;
	صحیح ret = 0;
	صحیح actions_implicit;
	ساختار کلید_مقدار_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	اگر ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	اگر (actions_implicit)
		انتخاب (argc) {
		حالت 1: actions = ACTION_GET; بشکن;
		حالت 2: actions = ACTION_SET; بشکن;
		حالت 3: actions = ACTION_SET_ALL; بشکن;
		پیشفرض:
			error(_("no action specified"));
			exit(129);
		}
	اگر (actions_implicit && argc == 1) {
		ثابت نویسه *last_dot = strrchr(argv[0], '.');
		اگر (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_مقدار(argv[0]);
	}
	اگر (display_opts.omit_مقادیر &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	اگر (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	اگر (display_opts.default_مقدار && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	اگر (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	اگر (flags & CONFIG_FLAGS_FIXED_مقدار) {
		صحیح allowed_usage = 0;

		انتخاب (actions) {
		/* git config --get <name> <value-pattern> */
		حالت ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		حالت ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		حالت ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		حالت ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		حالت ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			بشکن;

		/* git config <name> <value> <value-pattern> */
		حالت ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		حالت ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			بشکن;

		/* other options don't allow --fixed-value */
		}

		اگر (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	اگر (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	اگر (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		اگر (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			اگر (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			وگرنه
				die(_("error processing config file(s)"));
		}
	}
	وگرنه اگر (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	وگرنه اگر (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, مقدار);
		اگر (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	وگرنه اگر (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], مقدار, argv[2],
							      comment, flags);
	}
	وگرنه اگر (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], مقدار,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	وگرنه اگر (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], مقدار, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وگرنه اگر (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_مقدار(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	وگرنه اگر (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_مقدار(&location_opts, &display_opts, argv[0], argv[1],
				GET_مقدار_ALL, flags);
	}
	وگرنه اگر (actions == ACTION_GET_REGEXP) {
		display_opts.show_کلیدها = 1;
		check_argc(argc, 1, 2);
		ret = get_مقدار(&location_opts, &display_opts, argv[0], argv[1],
				GET_مقدار_ALL|GET_مقدار_کلید_REGEXP, flags);
	}
	وگرنه اگر (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	وگرنه اگر (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		اگر (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], پوچ, argv[1],
								      پوچ, flags);
		وگرنه
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], پوچ, پوچ);
	}
	وگرنه اگر (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], پوچ, argv[1],
							      پوچ, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وگرنه اگر (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		اگر (ret < 0)
			برو out;
		وگرنه اگر (!ret)
			die(_("no such section: %s"), argv[0]);
		وگرنه
			ret = 0;
	}
	وگرنه اگر (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], پوچ);
		اگر (ret < 0)
			برو out;
		وگرنه اگر (!ret)
			die(_("no such section: %s"), argv[0]);
		وگرنه
			ret = 0;
	}
	وگرنه اگر (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	وگرنه اگر (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		اگر (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	آزاد(comment);
	آزاد(مقدار);
	بازگشت ret;
}

صحیح cmd_config(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo)
{
	parse_opt_subcommand_fn *subcommand = پوچ;
	ساختار option subcommand_opts[] = {
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
	اگر (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		بازگشت subcommand(argc, argv, prefix, repo);
	}

	بازگشت cmd_config_actions(argc, argv, prefix);
}
