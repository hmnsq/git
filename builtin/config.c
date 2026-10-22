#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن ثابت حرف *ثابت builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	تہی
};

ساکن ثابت حرف *ثابت builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	تہی
};

ساکن ثابت حرف *ثابت builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	تہی
};

ساکن ثابت حرف *ثابت builtin_config_سیٹ_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	تہی
};

ساکن ثابت حرف *ثابت builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	تہی
};

ساکن ثابت حرف *ثابت builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	تہی
};

ساکن ثابت حرف *ثابت builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	تہی
};

ساکن ثابت حرف *ثابت builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	تہی
};

#تعریف_کرو CONFIG_LOCATION_OPTIONS(opts) \
	OPT_گروہ(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.ماخذ.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.ماخذ.blob, N_("blob-id"), N_("read config from given blob object"))

ساخت config_location_options {
	ساخت git_config_ماخذ ماخذ;
	ساخت config_options options;
	حرف *file_مقصد_free;
	صحیح use_global_config;
	صحیح use_system_config;
	صحیح use_local_config;
	صحیح use_worktree_config;
	صحیح respect_includes_opt;
};
#تعریف_کرو CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

شمار config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_راستہ,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#تعریف_کرو CONFIG_TYPE_OPTIONS(type) \
	OPT_گروہ(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_قدر(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_قدر(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_قدر(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_قدر(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_قدر(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_راستہ), \
	OPT_CALLBACK_قدر(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#تعریف_کرو CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_گروہ(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.انتہا_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_اقدار, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_کلیدیں, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

ساخت config_display_options {
	صحیح انتہا_nul;
	صحیح omit_اقدار;
	صحیح show_origin;
	صحیح show_scope;
	صحیح show_کلیدیں;
	شمار config_type type;
	حرف *default_قدر;
	/* Populated via `display_options_init()`. */
	صحیح رکن;
	صحیح delim;
	صحیح کلید_delim;
};
#تعریف_کرو CONFIG_DISPLAY_OPTIONS_INIT { \
	.رکن = '\n', \
	.delim = '=', \
	.کلید_delim = ' ', \
	.type = TYPE_NONE, \
}

#تعریف_کرو OPT_CALLBACK_قدر(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.قدر = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ساکن صحیح option_parse_type(ثابت ساخت option *opt, ثابت حرف *arg,
			     صحیح unset)
{
	صحیح new_type, *مقصد_type;

	اگر (unset) {
		*((صحیح *) opt->قدر) = 0;
		واپس 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	اگر (!new_type) {
		اگر (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		ورنہ اگر (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		ورنہ اگر (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		ورنہ اگر (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		ورنہ اگر (!strcmp(arg, "path"))
			new_type = TYPE_راستہ;
		ورنہ اگر (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		ورنہ اگر (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		ورنہ
			die(_("unrecognized --type argument, %s"), arg);
	}

	مقصد_type = opt->قدر;
	اگر (*مقصد_type && *مقصد_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		باہر_نکلو(129);
	}
	*مقصد_type = new_type;

	واپس 0;
}

ساکن خالی check_argc(صحیح argc, صحیح min, صحیح max)
{
	اگر (argc >= min && argc <= max)
		واپس;
	اگر (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	ورنہ
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	باہر_نکلو(129);
}

ساکن NORETURN خالی die_missing_سیٹ_قدر(ثابت حرف *arg)
{
	ثابت حرف *آخری_dot = strrchr(arg, '.');
	ثابت حرف *eq = آخری_dot ? strchr(آخری_dot + 1, '=') : تہی;
	حرف *سابقہ = eq ? xstrndup(arg, eq - arg) : تہی;

	اگر (سابقہ && git_config_کلید_is_valid(سابقہ)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       سابقہ, eq + 1);
	} ورنہ اگر (git_config_کلید_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} ورنہ {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	آزاد(سابقہ);
	باہر_نکلو(129);
}

ساکن خالی show_config_origin(ثابت ساخت config_display_options *opts,
			       ثابت ساخت کلید_قدر_info *kvi,
			       ساخت strbuf *buf)
{
	ثابت حرف رکن = opts->انتہا_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	اگر (opts->انتہا_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	ورنہ
		quote_c_style(kvi->filename ? kvi->filename : "", buf, تہی, 0);
	strbuf_addch(buf, رکن);
}

ساکن خالی show_config_scope(ثابت ساخت config_display_options *opts,
			      ثابت ساخت کلید_قدر_info *kvi,
			      ساخت strbuf *buf)
{
	ثابت حرف رکن = opts->انتہا_nul ? '\0' : '\t';
	ثابت حرف *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, رکن);
}

ساخت strbuf_list {
	ساخت strbuf *اشیاء;
	صحیح nr;
	صحیح alloc;
};

ساکن صحیح format_config_int64(ساخت strbuf *buf,
			       ثابت حرف *کلید_2,
			       ثابت حرف *قدر_2,
			       ثابت ساخت کلید_قدر_info *kvi,
			       صحیح gently)
{
	int64_t v = 0;
	اگر (gently) {
		اگر (!git_parse_int64(قدر_2, &v))
			واپس -1;
	} ورنہ {
		/* may die() */
		v = git_config_int64(کلید_2, قدر_2 ? قدر_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	واپس 0;
}

ساکن صحیح format_config_bool(ساخت strbuf *buf,
			      ثابت حرف *کلید_2,
			      ثابت حرف *قدر_2,
			      صحیح gently)
{
	صحیح v = 0;
	اگر (gently) {
		اگر ((v = git_parse_maybe_bool(قدر_2)) < 0)
			واپس -1;
	} ورنہ {
		/* may die() */
		v = git_config_bool(کلید_2, قدر_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	واپس 0;
}

ساکن صحیح format_config_bool_or_int(ساخت strbuf *buf,
				     ثابت حرف *کلید_2,
				     ثابت حرف *قدر_2,
				     ثابت ساخت کلید_قدر_info *kvi,
				     صحیح gently)
{
	صحیح v, is_bool = 0;

	اگر (gently) {
		v = git_parse_maybe_bool_متن(قدر_2);

		اگر (v >= 0)
			is_bool = 1;
		ورنہ اگر (!git_parse_int(قدر_2, &v))
			واپس -1;
	} ورنہ {
		v = git_config_bool_or_int(کلید_2, قدر_2, kvi,
					   &is_bool);
	}

	اگر (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	ورنہ
		strbuf_addf(buf, "%d", v);

	واپس 0;
}

/* This mode is always gentle. */
ساکن صحیح format_config_bool_or_str(ساخت strbuf *buf,
				     ثابت حرف *قدر_2)
{
	صحیح v = git_parse_maybe_bool(قدر_2);
	اگر (v < 0)
		strbuf_addstr(buf, قدر_2);
	ورنہ
		strbuf_addstr(buf, v ? "true" : "false");
	واپس 0;
}

ساکن صحیح format_config_راستہ(ساخت strbuf *buf,
			      ثابت حرف *کلید_2,
			      ثابت حرف *قدر_2,
			      صحیح gently)
{
	حرف *v;

	اگر (git_config_pathname(&v, کلید_2, قدر_2) < 0)
		واپس -1;

	اگر (v)
		strbuf_addstr(buf, v);
	ورنہ
		واپس gently ? -1 : 1; /* :(optional)no-such-file */

	آزاد(v);
	واپس 0;
}

ساکن صحیح format_config_expiry_date(ساخت strbuf *buf,
				     ثابت حرف *کلید_2,
				     ثابت حرف *قدر_2,
				     صحیح quietly)
{
	timestamp_t t;
	اگر (quietly) {
		اگر (parse_expiry_date(قدر_2, &t))
			واپس -1;
	} ورنہ اگر (git_config_expiry_date(&t, کلید_2, قدر_2) < 0) {
		واپس -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	واپس 0;
}

ساکن صحیح format_config_color(ساخت strbuf *buf,
			       ثابت حرف *کلید_2,
			       ثابت حرف *قدر_2,
			       صحیح gently)
{
	حرف v[COLOR_MAXLEN];

	اگر (gently) {
		اگر (color_parse_quietly(قدر_2, v) < 0)
			واپس -1;
	} ورنہ اگر (git_config_color(v, کلید_2, قدر_2) < 0) {
		واپس -1;
	}

	strbuf_addstr(buf, v);
	واپس 0;
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
ساکن صحیح format_config(ثابت ساخت config_display_options *opts,
			 ساخت strbuf *buf, ثابت حرف *کلید_2,
			 ثابت حرف *قدر_2, ثابت ساخت کلید_قدر_info *kvi,
			 صحیح gently)
{
	صحیح res = 0;
	اگر (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	اگر (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	اگر (opts->show_کلیدیں)
		strbuf_addstr(buf, کلید_2);

	اگر (opts->omit_اقدار)
		جاؤ terminator;

	اگر (opts->show_کلیدیں)
		strbuf_addch(buf, opts->کلید_delim);

	انتخاب (opts->type) {
	حالت TYPE_INT:
		res = format_config_int64(buf, کلید_2, قدر_2, kvi, gently);
		توڑو;

	حالت TYPE_BOOL:
		res = format_config_bool(buf, کلید_2, قدر_2, gently);
		توڑو;

	حالت TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, کلید_2, قدر_2, kvi, gently);
		توڑو;

	حالت TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, قدر_2);
		توڑو;

	حالت TYPE_راستہ:
		res = format_config_راستہ(buf, کلید_2, قدر_2, gently);
		توڑو;

	حالت TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, کلید_2, قدر_2, gently);
		توڑو;

	حالت TYPE_COLOR:
		res = format_config_color(buf, کلید_2, قدر_2, gently);
		توڑو;

	حالت TYPE_NONE:
		اگر (قدر_2) {
			strbuf_addstr(buf, قدر_2);
		} ورنہ {
			/* Just show the key name; back out delimiter */
			اگر (opts->show_کلیدیں)
				strbuf_setlen(buf, buf->len - 1);
		}
		توڑو;

	طےشدہ:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->رکن);
	واپس res;
}

ساکن صحیح show_all_config(ثابت حرف *کلید_2, ثابت حرف *قدر_2,
			   ثابت ساخت config_context *ctx,
			   خالی *cb)
{
	ثابت ساخت config_display_options *opts = cb;
	ثابت ساخت کلید_قدر_info *kvi = ctx->kvi;
	ساخت strbuf formatted = STRBUF_INIT;

	اگر (format_config(opts, &formatted, کلید_2, قدر_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	واپس 0;
}

#تعریف_کرو GET_قدر_ALL        (1 << 0)
#تعریف_کرو GET_قدر_کلید_REGEXP (1 << 1)

ساخت collect_config_ڈیٹا {
	ثابت ساخت config_display_options *display_opts;
	ساخت strbuf_list *اقدار;
	ثابت حرف *قدر_نمونہ;
	ثابت حرف *کلید;
	regex_t *regexp;
	regex_t *کلید_regexp;
	صحیح do_not_مطابقت;
	غیرعلامتی get_قدر_flags;
	غیرعلامتی flags;
};

ساکن صحیح collect_config(ثابت حرف *کلید_2, ثابت حرف *قدر_2,
			  ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت collect_config_ڈیٹا *ڈیٹا = cb;
	ساخت strbuf_list *اقدار = ڈیٹا->اقدار;
	ثابت ساخت کلید_قدر_info *kvi = ctx->kvi;
	صحیح حالت_2;

	اگر (!(ڈیٹا->get_قدر_flags & GET_قدر_کلید_REGEXP) &&
	    strcmp(کلید_2, ڈیٹا->کلید))
		واپس 0;
	اگر ((ڈیٹا->get_قدر_flags & GET_قدر_کلید_REGEXP) &&
	    regexec(ڈیٹا->کلید_regexp, کلید_2, 0, تہی, 0))
		واپس 0;
	اگر ((ڈیٹا->flags & CONFIG_FLAGS_FIXED_قدر) &&
	    strcmp(ڈیٹا->قدر_نمونہ, (قدر_2?قدر_2:"")))
		واپس 0;
	اگر (ڈیٹا->regexp &&
	    (ڈیٹا->do_not_مطابقت ^ !!regexec(ڈیٹا->regexp, (قدر_2?قدر_2:""), 0, تہی, 0)))
		واپس 0;

	ALLOC_GROW(اقدار->اشیاء, اقدار->nr + 1, اقدار->alloc);
	strbuf_init(&اقدار->اشیاء[اقدار->nr], 0);

	حالت_2 = format_config(ڈیٹا->display_opts, &اقدار->اشیاء[اقدار->nr++],
			       کلید_2, قدر_2, kvi, 0);
	اگر (حالت_2 < 0)
		واپس حالت_2;
	اگر (حالت_2) {
		strbuf_release(&اقدار->اشیاء[--اقدار->nr]);
		حالت_2 = 0;
	}
	واپس حالت_2;
}

ساکن صحیح get_قدر(ثابت ساخت config_location_options *opts,
		     ثابت ساخت config_display_options *display_opts,
		     ثابت حرف *کلید_2, ثابت حرف *regex_,
		     غیرعلامتی get_قدر_flags, غیرعلامتی flags)
{
	صحیح ret = CONFIG_GENERIC_ERROR;
	ساخت strbuf_list اقدار = {تہی};
	ساخت collect_config_ڈیٹا ڈیٹا = {
		.display_opts = display_opts,
		.اقدار = &اقدار,
		.get_قدر_flags = get_قدر_flags,
		.flags = flags,
	};
	حرف *کلید = تہی;
	صحیح i;

	اگر (get_قدر_flags & GET_قدر_کلید_REGEXP) {
		حرف *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		کلید = xstrdup(کلید_2);
		برائے (tl = کلید + لڑی_لمبائی(کلید) - 1;
		     tl >= کلید && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		برائے (tl = کلید; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		ڈیٹا.کلید_regexp = (regex_t*)xmalloc(حجم(regex_t));
		اگر (regcomp(ڈیٹا.کلید_regexp, کلید, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), کلید_2);
			FREE_AND_NULL(ڈیٹا.کلید_regexp);
			ret = CONFIG_INVALID_نمونہ;
			جاؤ free_لڑیاں;
		}
	} ورنہ {
		اگر (git_config_parse_کلید(کلید_2, &کلید, تہی)) {
			ret = CONFIG_INVALID_کلید;
			جاؤ free_لڑیاں;
		}

		ڈیٹا.کلید = کلید;
	}

	اگر (regex_ && (flags & CONFIG_FLAGS_FIXED_قدر))
		ڈیٹا.قدر_نمونہ = regex_;
	ورنہ اگر (regex_) {
		اگر (regex_[0] == '!') {
			ڈیٹا.do_not_مطابقت = 1;
			regex_++;
		}

		ڈیٹا.regexp = (regex_t*)xmalloc(حجم(regex_t));
		اگر (regcomp(ڈیٹا.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(ڈیٹا.regexp);
			ret = CONFIG_INVALID_نمونہ;
			جاؤ free_لڑیاں;
		}
	}

	config_with_options(collect_config, &ڈیٹا,
			    &opts->ماخذ, the_repository,
			    &opts->options);

	اگر (!اقدار.nr && display_opts->default_قدر) {
		ساخت کلید_قدر_info kvi = KVI_INIT;
		ساخت strbuf *شے;
		صحیح حالت_2;

		kvi_from_param(&kvi);
		ALLOC_GROW(اقدار.اشیاء, اقدار.nr + 1, اقدار.alloc);
		شے = &اقدار.اشیاء[اقدار.nr++];
		strbuf_init(شے, 0);

		حالت_2 = format_config(display_opts, شے, کلید_2,
				       display_opts->default_قدر, &kvi, 0);
		اگر (حالت_2 < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_قدر);
		اگر (حالت_2) {
			/* default was a missing optional value */
			اقدار.nr--;
			strbuf_release(شے);
		}
	}

	ret = !اقدار.nr;

	برائے (i = 0; i < اقدار.nr; i++) {
		ساخت strbuf *buf = اقدار.اشیاء + i;
		اگر ((get_قدر_flags & GET_قدر_ALL) || i == اقدار.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	آزاد(اقدار.اشیاء);

free_لڑیاں:
	آزاد(کلید);
	اگر (ڈیٹا.کلید_regexp) {
		regfree(ڈیٹا.کلید_regexp);
		آزاد(ڈیٹا.کلید_regexp);
	}
	اگر (ڈیٹا.regexp) {
		regfree(ڈیٹا.regexp);
		آزاد(ڈیٹا.regexp);
	}

	واپس ret;
}

ساکن حرف *normalize_قدر(ثابت حرف *کلید, ثابت حرف *قدر,
			     صحیح type, ساخت کلید_قدر_info *kvi)
{
	اگر (!قدر)
		واپس تہی;

	اگر (type == 0 || type == TYPE_راستہ || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		واپس xstrdup(قدر);
	اگر (type == TYPE_INT)
		واپس xstrfmt("%"PRId64, git_config_int64(کلید, قدر, kvi));
	اگر (type == TYPE_BOOL)
		واپس xstrdup(git_config_bool(کلید, قدر) ?  "true" : "false");
	اگر (type == TYPE_BOOL_OR_INT) {
		صحیح is_bool, v;
		v = git_config_bool_or_int(کلید, قدر, kvi, &is_bool);
		اگر (!is_bool)
			واپس xstrfmt("%d", v);
		ورنہ
			واپس xstrdup(v ? "true" : "false");
	}
	اگر (type == TYPE_BOOL_OR_STR) {
		صحیح v = git_parse_maybe_bool(قدر);
		اگر (v < 0)
			واپس xstrdup(قدر);
		ورنہ
			واپس xstrdup(v ? "true" : "false");
	}
	اگر (type == TYPE_COLOR) {
		حرف v[COLOR_MAXLEN];
		اگر (git_config_color(v, کلید, قدر))
			die(_("cannot parse color '%s'"), قدر);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		واپس xstrdup(قدر);
	}

	BUG("cannot normalize type %d", type);
}

ساخت get_color_config_ڈیٹا {
	صحیح get_color_ملا;
	ثابت حرف *get_color_خانہ;
	حرف parsed_color[COLOR_MAXLEN];
};

ساکن صحیح git_get_color_config(ثابت حرف *var, ثابت حرف *قدر,
				ثابت ساخت config_context *ctx غیر_مستعمل,
				خالی *cb)
{
	ساخت get_color_config_ڈیٹا *ڈیٹا = cb;

	اگر (!strcmp(var, ڈیٹا->get_color_خانہ)) {
		اگر (!قدر)
			config_error_nonbool(var);
		اگر (color_parse(قدر, ڈیٹا->parsed_color) < 0)
			واپس -1;
		ڈیٹا->get_color_ملا = 1;
	}
	واپس 0;
}

ساکن صحیح get_color(ثابت ساخت config_location_options *opts,
		      ثابت حرف *var, ثابت حرف *def_color)
{
	ساخت get_color_config_ڈیٹا ڈیٹا = {
		.get_color_خانہ = var,
		.parsed_color[0] = '\0',
	};
	صحیح ret;

	config_with_options(git_get_color_config, &ڈیٹا,
			    &opts->ماخذ, the_repository,
			    &opts->options);

	اگر (!ڈیٹا.get_color_ملا && def_color) {
		اگر (color_parse(def_color, ڈیٹا.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			جاؤ برآمد;
		}
	}

	ret = 0;

برآمد:
	fputs(ڈیٹا.parsed_color, stdout);
	واپس ret;
}

ساخت get_colorbool_config_ڈیٹا {
	شمار git_colorbool get_colorbool_ملا;
	شمار git_colorbool get_diff_color_ملا;
	شمار git_colorbool get_color_ui_ملا;
	ثابت حرف *get_colorbool_خانہ;
};

ساکن صحیح git_get_colorbool_config(ثابت حرف *var, ثابت حرف *قدر,
				    ثابت ساخت config_context *ctx غیر_مستعمل,
				    خالی *cb)
{
	ساخت get_colorbool_config_ڈیٹا *ڈیٹا = cb;

	اگر (!strcmp(var, ڈیٹا->get_colorbool_خانہ))
		ڈیٹا->get_colorbool_ملا = git_config_colorbool(var, قدر);
	ورنہ اگر (!strcmp(var, "diff.color"))
		ڈیٹا->get_diff_color_ملا = git_config_colorbool(var, قدر);
	ورنہ اگر (!strcmp(var, "color.ui"))
		ڈیٹا->get_color_ui_ملا = git_config_colorbool(var, قدر);
	واپس 0;
}

ساکن صحیح get_colorbool(ثابت ساخت config_location_options *opts,
			 ثابت حرف *var, صحیح print)
{
	ساخت get_colorbool_config_ڈیٹا ڈیٹا = {
		.get_colorbool_خانہ = var,
		.get_colorbool_ملا = GIT_COLOR_UNKNOWN,
		.get_diff_color_ملا = GIT_COLOR_UNKNOWN,
		.get_color_ui_ملا = GIT_COLOR_UNKNOWN,
	};
	bool نتیجہ;

	config_with_options(git_get_colorbool_config, &ڈیٹا,
			    &opts->ماخذ, the_repository,
			    &opts->options);

	اگر (ڈیٹا.get_colorbool_ملا == GIT_COLOR_UNKNOWN) {
		اگر (!strcmp(ڈیٹا.get_colorbool_خانہ, "color.diff"))
			ڈیٹا.get_colorbool_ملا = ڈیٹا.get_diff_color_ملا;
		اگر (ڈیٹا.get_colorbool_ملا == GIT_COLOR_UNKNOWN)
			ڈیٹا.get_colorbool_ملا = ڈیٹا.get_color_ui_ملا;
	}

	اگر (ڈیٹا.get_colorbool_ملا == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		ڈیٹا.get_colorbool_ملا = GIT_COLOR_AUTO;

	نتیجہ = want_color(ڈیٹا.get_colorbool_ملا);

	اگر (print) {
		لکھو("%s\n", نتیجہ ? "true" : "false");
		واپس 0;
	} ورنہ
		واپس نتیجہ ? 0 : 1;
}

ساکن خالی check_write(ثابت ساخت git_config_ماخذ *ماخذ)
{
	اگر (!ماخذ->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	اگر (ماخذ->use_stdin)
		die(_("writing to stdin is not supported"));

	اگر (ماخذ->blob)
		die(_("writing config blobs is not supported"));
}

ساخت urlmatch_current_candidate_قدر {
	حرف قدر_is_null;
	ساخت strbuf قدر;
	ساخت کلید_قدر_info kvi;
};

ساکن صحیح urlmatch_collect_fn(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت ساخت config_context *ctx,
			       خالی *cb)
{
	ساخت string_list *اقدار = cb;
	ساخت string_list_شے *شے = string_list_داخل_کرو(اقدار, var);
	ساخت urlmatch_current_candidate_قدر *matched = شے->util;
	ثابت ساخت کلید_قدر_info *kvi = ctx->kvi;

	اگر (!matched) {
		matched = xmalloc(حجم(*matched));
		strbuf_init(&matched->قدر, 0);
		شے->util = matched;
	} ورنہ {
		strbuf_reset(&matched->قدر);
	}
	matched->kvi = *kvi;

	اگر (قدر) {
		strbuf_addstr(&matched->قدر, قدر);
		matched->قدر_is_null = 0;
	} ورنہ {
		matched->قدر_is_null = 1;
	}
	واپس 0;
}

ساکن صحیح get_urlmatch(ثابت ساخت config_location_options *opts,
			ثابت ساخت config_display_options *_display_opts,
			ثابت حرف *var, ثابت حرف *url)
{
	صحیح ret;
	حرف *section;
	حرف *section_دم;
	ساخت config_display_options display_opts = *_display_opts;
	ساخت string_list_شے *شے;
	ساخت urlmatch_config config = URLMATCH_CONFIG_INIT;
	ساخت string_list اقدار = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = تہی;
	config.cb = &اقدار;

	اگر (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_دم = strchr(section, '.');
	اگر (section_دم) {
		*section_دم = '\0';
		config.کلید = section_دم + 1;
		display_opts.show_کلیدیں = 0;
	} ورنہ {
		config.کلید = تہی;
		display_opts.show_کلیدیں = 1;
	}

	config_with_options(urlmatch_config_اندراج, &config,
			    &opts->ماخذ, the_repository,
			    &opts->options);

	ret = !اقدار.nr;

	for_each_string_list_شے(شے, &اقدار) {
		ساخت urlmatch_current_candidate_قدر *matched = شے->util;
		ساخت strbuf buf = STRBUF_INIT;
		صحیح حالت_2;

		حالت_2 = format_config(&display_opts, &buf, شے->string,
				       matched->قدر_is_null ? تہی : matched->قدر.buf,
				       &matched->kvi, 0);
		اگر (!حالت_2)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->قدر);
	}
	urlmatch_config_release(&config);
	string_list_clear(&اقدار, 1);
	آزاد(config.url.url);

	آزاد(section);
	واپس ret;
}

ساکن حرف *default_user_config(خالی)
{
	ساخت strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	واپس strbuf_detach(&buf, تہی);
}

ساکن خالی location_options_init(ساخت config_location_options *opts,
				  ثابت حرف *سابقہ)
{
	اگر (!opts->ماخذ.file)
		opts->ماخذ.file = opts->file_مقصد_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	اگر (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->ماخذ.file + !!opts->ماخذ.blob > 1) {
		error(_("only one config file at a time"));
		باہر_نکلو(129);
	}

	اگر (!startup_info->have_repository) {
		اگر (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		اگر (opts->ماخذ.blob)
			die(_("--blob can only be used inside a git repository"));
		اگر (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	اگر (opts->ماخذ.file &&
			!strcmp(opts->ماخذ.file, "-")) {
		opts->ماخذ.file = تہی;
		opts->ماخذ.use_stdin = 1;
		opts->ماخذ.scope = CONFIG_SCOPE_COMMAND;
	}

	اگر (opts->use_global_config) {
		opts->ماخذ.file = opts->file_مقصد_free = git_global_config();
		اگر (!opts->ماخذ.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->ماخذ.scope = CONFIG_SCOPE_GLOBAL;
	} ورنہ اگر (opts->use_system_config) {
		opts->ماخذ.file = opts->file_مقصد_free = git_system_config();
		opts->ماخذ.scope = CONFIG_SCOPE_SYSTEM;
	} ورنہ اگر (opts->use_local_config) {
		opts->ماخذ.file = opts->file_مقصد_free = repo_git_راستہ(the_repository, "config");
		opts->ماخذ.scope = CONFIG_SCOPE_LOCAL;
	} ورنہ اگر (opts->use_worktree_config) {
		ساخت worktree **worktrees = get_worktrees(the_repository);
		اگر (the_repository->repository_format_worktree_config)
			opts->ماخذ.file = opts->file_مقصد_free =
				repo_git_راستہ(the_repository, "config.worktree");
		ورنہ اگر (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		ورنہ
			opts->ماخذ.file = opts->file_مقصد_free =
				repo_git_راستہ(the_repository, "config");
		opts->ماخذ.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} ورنہ اگر (opts->ماخذ.file) {
		اگر (!is_absolute_راستہ(opts->ماخذ.file) && سابقہ)
			opts->ماخذ.file = opts->file_مقصد_free =
				سابقہ_filename(سابقہ, opts->ماخذ.file);
		opts->ماخذ.scope = CONFIG_SCOPE_COMMAND;
	} ورنہ اگر (opts->ماخذ.blob) {
		opts->ماخذ.scope = CONFIG_SCOPE_COMMAND;
	}

	اگر (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->ماخذ.file;
	ورنہ
		opts->options.respect_includes = opts->respect_includes_opt;
	اگر (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ساکن خالی location_options_release(ساخت config_location_options *opts)
{
	آزاد(opts->file_مقصد_free);
}

ساکن خالی display_options_init(ساخت config_display_options *opts)
{
	اگر (opts->انتہا_nul) {
		opts->رکن = '\0';
		opts->delim = '\n';
		opts->کلید_delim = '\n';
	}
}

ساکن خالی display_options_init_list(ساخت config_display_options *opts)
{
	opts->show_کلیدیں = 1;

	اگر (opts->انتہا_nul) {
		display_options_init(opts);
	} ورنہ {
		opts->رکن = '\n';
		opts->delim = ' ';
		opts->کلید_delim = '=';
	}
}

ساکن صحیح cmd_config_list(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساخت config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_گروہ(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, سابقہ);
	display_options_init_list(&display_opts);

	تیاری_auto_pager("config", 1);

	اگر (config_with_options(show_all_config, &display_opts,
				&location_opts.ماخذ, the_repository,
				&location_opts.options) < 0) {
		اگر (location_opts.ماخذ.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.ماخذ.file);
		ورنہ
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	واپس 0;
}

ساکن صحیح cmd_config_get(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			  ساخت repository *repo غیر_مستعمل)
{
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساخت config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *قدر_نمونہ = تہی, *url = تہی;
	صحیح flags = 0;
	غیرعلامتی get_قدر_flags = 0;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_گروہ(N_("Filter options")),
		OPT_ثنائی_ہندسہ(0, "all", &get_قدر_flags, N_("return all values for multi-valued config options"), GET_قدر_ALL),
		OPT_ثنائی_ہندسہ(0, "regexp", &get_قدر_flags, N_("interpret the name as a regular expression"), GET_قدر_کلید_REGEXP),
		OPT_STRING(0, "value", &قدر_نمونہ, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_ثنائی_ہندسہ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_قدر),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_گروہ(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_قدر,
			   N_("value"), N_("use default value when missing entry")),
		OPT_انتہا(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	اگر ((flags & CONFIG_FLAGS_FIXED_قدر) && !قدر_نمونہ)
		die(_("--fixed-value only applies with 'value-pattern'"));
	اگر (display_opts.default_قدر &&
	    ((get_قدر_flags & GET_قدر_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	اگر (url && ((get_قدر_flags & GET_قدر_ALL) ||
		    (get_قدر_flags & GET_قدر_کلید_REGEXP) ||
		    قدر_نمونہ))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, سابقہ);
	display_options_init(&display_opts);

	اگر (display_opts.type != TYPE_COLOR)
		تیاری_auto_pager("config", 1);

	اگر (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	ورنہ اگر (display_opts.type == TYPE_COLOR && !لڑی_لمبائی(argv[0]) && display_opts.default_قدر)
		ret = get_color(&location_opts, "", display_opts.default_قدر);
	ورنہ
		ret = get_قدر(&location_opts, &display_opts, argv[0], قدر_نمونہ,
				get_قدر_flags, flags);

	location_options_release(&location_opts);
	واپس ret;
}

ساکن صحیح cmd_config_سیٹ(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			  ساخت repository *repo غیر_مستعمل)
{
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *قدر_نمونہ = تہی, *comment_arg = تہی;
	حرف *comment = تہی;
	صحیح flags = 0, append = 0, type = 0;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_گروہ(N_("Filter")),
		OPT_ثنائی_ہندسہ(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &قدر_نمونہ, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_ثنائی_ہندسہ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_قدر),
		OPT_گروہ(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_انتہا(),
	};
	ساخت کلید_قدر_info default_kvi = KVI_INIT;
	حرف *قدر;
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, builtin_config_سیٹ_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (argc == 1)
		die_missing_سیٹ_قدر(argv[0]);
	check_argc(argc, 2, 2);

	اگر ((flags & CONFIG_FLAGS_FIXED_قدر) && !قدر_نمونہ)
		die(_("--fixed-value only applies with --value=<pattern>"));
	اگر (append && قدر_نمونہ)
		die(_("--append cannot be used with --value=<pattern>"));
	اگر (append)
		قدر_نمونہ = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, سابقہ);
	check_write(&location_opts.ماخذ);

	قدر = normalize_قدر(argv[0], argv[1], type, &default_kvi);

	اگر ((flags & CONFIG_FLAGS_MULTI_REPLACE) || قدر_نمونہ) {
		ret = repo_config_سیٹ_multivar_اندر_file_gently(the_repository, location_opts.ماخذ.file,
							      argv[0], قدر, قدر_نمونہ,
							      comment, flags);
	} ورنہ {
		ret = repo_config_سیٹ_اندر_file_gently(the_repository, location_opts.ماخذ.file,
						     argv[0], comment, قدر);
		اگر (ret == CONFIG_NOTHING_سیٹ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	آزاد(comment);
	آزاد(قدر);
	واپس ret;
}

ساکن صحیح cmd_config_unset(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			    ساخت repository *repo غیر_مستعمل)
{
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *قدر_نمونہ = تہی;
	صحیح flags = 0;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_گروہ(N_("Filter")),
		OPT_ثنائی_ہندسہ(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &قدر_نمونہ, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_ثنائی_ہندسہ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_قدر),
		OPT_انتہا(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	اگر ((flags & CONFIG_FLAGS_FIXED_قدر) && !قدر_نمونہ)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, سابقہ);
	check_write(&location_opts.ماخذ);

	اگر ((flags & CONFIG_FLAGS_MULTI_REPLACE) || قدر_نمونہ)
		ret = repo_config_سیٹ_multivar_اندر_file_gently(the_repository, location_opts.ماخذ.file,
							      argv[0], تہی, قدر_نمونہ,
							      تہی, flags);
	ورنہ
		ret = repo_config_سیٹ_اندر_file_gently(the_repository, location_opts.ماخذ.file, argv[0],
						     تہی, تہی);

	location_options_release(&location_opts);
	واپس ret;
}

ساکن صحیح cmd_config_rename_section(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				     ساخت repository *repo غیر_مستعمل)
{
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_انتہا(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, سابقہ);
	check_write(&location_opts.ماخذ);

	ret = repo_config_rename_section_اندر_file(the_repository, location_opts.ماخذ.file,
						 argv[0], argv[1]);
	اگر (ret < 0)
		جاؤ برآمد;
	ورنہ اگر (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

برآمد:
	location_options_release(&location_opts);
	واپس ret;
}

ساکن صحیح cmd_config_remove_section(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				     ساخت repository *repo غیر_مستعمل)
{
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_انتہا(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, سابقہ);
	check_write(&location_opts.ماخذ);

	ret = repo_config_rename_section_اندر_file(the_repository, location_opts.ماخذ.file,
						 argv[0], تہی);
	اگر (ret < 0)
		جاؤ برآمد;
	ورنہ اگر (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

برآمد:
	location_options_release(&location_opts);
	واپس ret;
}

ساکن صحیح show_editor(ساخت config_location_options *opts)
{
	حرف *config_file;

	اگر (!opts->ماخذ.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	اگر (opts->ماخذ.use_stdin)
		die(_("editing stdin is not supported"));
	اگر (opts->ماخذ.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, تہی);
	config_file = opts->ماخذ.file ?
			xstrdup(opts->ماخذ.file) :
			repo_git_راستہ(the_repository, "config");
	اگر (opts->use_global_config) {
		صحیح fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		اگر (fd >= 0) {
			حرف *content = default_user_config();
			write_str_اندر_بھرا(fd, content);
			آزاد(content);
			close(fd);
		}
		ورنہ اگر (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	اگر (launch_editor(config_file, تہی, تہی)) {
		آزاد(config_file);
		واپس -1;
	}
	آزاد(config_file);

	واپس 0;
}

ساکن صحیح cmd_config_edit(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_انتہا(),
	};
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, سابقہ);
	check_write(&location_opts.ماخذ);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	واپس ret;
}

ساکن صحیح cmd_config_actions(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ)
{
	شمار {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_جوڑو = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_سیٹ = (1<<11),
		ACTION_سیٹ_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	ساخت config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ساخت config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *comment_arg = تہی;
	صحیح actions = 0;
	غیرعلامتی flags = 0;
	ساخت option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_گروہ(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_جوڑو),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_گروہ(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_قدر,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_ثنائی_ہندسہ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_قدر),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_انتہا(),
	};
	حرف *قدر = تہی, *comment = تہی;
	صحیح ret = 0;
	صحیح actions_implicit;
	ساخت کلید_قدر_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, سابقہ, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, سابقہ);
	display_options_init(&display_opts);

	اگر ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		باہر_نکلو(129);
	}

	actions_implicit = (actions == 0);
	اگر (actions_implicit)
		انتخاب (argc) {
		حالت 1: actions = ACTION_GET; توڑو;
		حالت 2: actions = ACTION_سیٹ; توڑو;
		حالت 3: actions = ACTION_سیٹ_ALL; توڑو;
		طےشدہ:
			error(_("no action specified"));
			باہر_نکلو(129);
		}
	اگر (actions_implicit && argc == 1) {
		ثابت حرف *آخری_dot = strrchr(argv[0], '.');
		اگر (آخری_dot && strchr(آخری_dot + 1, '='))
			die_missing_سیٹ_قدر(argv[0]);
	}
	اگر (display_opts.omit_اقدار &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		باہر_نکلو(129);
	}

	اگر (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		باہر_نکلو(129);
	}

	اگر (display_opts.default_قدر && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		باہر_نکلو(129);
	}

	اگر (comment_arg &&
	    !(actions & (ACTION_جوڑو|ACTION_سیٹ|ACTION_سیٹ_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		باہر_نکلو(129);
	}

	/* check usage of --fixed-value */
	اگر (flags & CONFIG_FLAGS_FIXED_قدر) {
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
			توڑو;

		/* git config <name> <value> <value-pattern> */
		حالت ACTION_سیٹ_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		حالت ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			توڑو;

		/* other options don't allow --fixed-value */
		}

		اگر (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			باہر_نکلو(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	اگر (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		تیاری_auto_pager("config", 1);

	اگر (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		اگر (config_with_options(show_all_config, &display_opts,
					&location_opts.ماخذ, the_repository,
					&location_opts.options) < 0) {
			اگر (location_opts.ماخذ.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.ماخذ.file);
			ورنہ
				die(_("error processing config file(s)"));
		}
	}
	ورنہ اگر (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	ورنہ اگر (actions == ACTION_سیٹ) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 2, 2);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_اندر_file_gently(the_repository, location_opts.ماخذ.file, argv[0], comment, قدر);
		اگر (ret == CONFIG_NOTHING_سیٹ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	ورنہ اگر (actions == ACTION_سیٹ_ALL) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 2, 3);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_multivar_اندر_file_gently(the_repository, location_opts.ماخذ.file,
							      argv[0], قدر, argv[2],
							      comment, flags);
	}
	ورنہ اگر (actions == ACTION_جوڑو) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 2, 2);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_multivar_اندر_file_gently(the_repository, location_opts.ماخذ.file,
							      argv[0], قدر,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	ورنہ اگر (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 2, 3);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_multivar_اندر_file_gently(the_repository, location_opts.ماخذ.file,
							      argv[0], قدر, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ورنہ اگر (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_قدر(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	ورنہ اگر (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_قدر(&location_opts, &display_opts, argv[0], argv[1],
				GET_قدر_ALL, flags);
	}
	ورنہ اگر (actions == ACTION_GET_REGEXP) {
		display_opts.show_کلیدیں = 1;
		check_argc(argc, 1, 2);
		ret = get_قدر(&location_opts, &display_opts, argv[0], argv[1],
				GET_قدر_ALL|GET_قدر_کلید_REGEXP, flags);
	}
	ورنہ اگر (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	ورنہ اگر (actions == ACTION_UNSET) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 1, 2);
		اگر (argc == 2)
			ret = repo_config_سیٹ_multivar_اندر_file_gently(the_repository, location_opts.ماخذ.file,
								      argv[0], تہی, argv[1],
								      تہی, flags);
		ورنہ
			ret = repo_config_سیٹ_اندر_file_gently(the_repository, location_opts.ماخذ.file,
							     argv[0], تہی, تہی);
	}
	ورنہ اگر (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 1, 2);
		ret = repo_config_سیٹ_multivar_اندر_file_gently(the_repository, location_opts.ماخذ.file,
							      argv[0], تہی, argv[1],
							      تہی, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ورنہ اگر (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_اندر_file(the_repository, location_opts.ماخذ.file,
							 argv[0], argv[1]);
		اگر (ret < 0)
			جاؤ برآمد;
		ورنہ اگر (!ret)
			die(_("no such section: %s"), argv[0]);
		ورنہ
			ret = 0;
	}
	ورنہ اگر (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.ماخذ);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_اندر_file(the_repository, location_opts.ماخذ.file,
							 argv[0], تہی);
		اگر (ret < 0)
			جاؤ برآمد;
		ورنہ اگر (!ret)
			die(_("no such section: %s"), argv[0]);
		ورنہ
			ret = 0;
	}
	ورنہ اگر (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	ورنہ اگر (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		اگر (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

برآمد:
	location_options_release(&location_opts);
	آزاد(comment);
	آزاد(قدر);
	واپس ret;
}

صحیح cmd_config(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo)
{
	parse_opt_subcommand_fn *subcommand = تہی;
	ساخت option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_سیٹ),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_انتہا(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, سابقہ, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	اگر (subcommand) {
		argc = parse_options(argc, argv, سابقہ, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		واپس subcommand(argc, argv, سابقہ, repo);
	}

	واپس cmd_config_actions(argc, argv, سابقہ);
}
