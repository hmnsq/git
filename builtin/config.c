#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "color.h"
#ضمن "date.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "ident.h"
#ضمن "parse-options.h"
#ضمن "urlmatch.h"
#ضمن "path.h"
#ضمن "quote.h"
#ضمن "setup.h"
#ضمن "strbuf.h"
#ضمن "worktree.h"

ساكن ثابت حرف *ثابت builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	عدم
};

ساكن ثابت حرف *ثابت builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	عدم
};

ساكن ثابت حرف *ثابت builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	عدم
};

ساكن ثابت حرف *ثابت builtin_config_سیٹ_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	عدم
};

ساكن ثابت حرف *ثابت builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	عدم
};

ساكن ثابت حرف *ثابت builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	عدم
};

ساكن ثابت حرف *ثابت builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	عدم
};

ساكن ثابت حرف *ثابت builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	عدم
};

#تعریف_کرو CONFIG_LOCATION_OPTIONS(opts) \
	OPT_گروہ(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.سروت.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.سروت.blob, N_("blob-id"), N_("read config from given blob object"))

بنية config_location_options {
	بنية git_config_سروت سروت;
	بنية config_options options;
	حرف *file_منزل_free;
	صحيح use_global_config;
	صحيح use_system_config;
	صحيح use_local_config;
	صحيح use_worktree_config;
	صحيح respect_includes_opt;
};
#تعریف_کرو CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

تعداد config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_رستہ,
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
	OPT_CALLBACK_قدر(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_رستہ), \
	OPT_CALLBACK_قدر(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#تعریف_کرو CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_گروہ(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.سرا_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_اقدار, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_کلیدیں, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

بنية config_display_options {
	صحيح سرا_nul;
	صحيح omit_اقدار;
	صحيح show_origin;
	صحيح show_scope;
	صحيح show_کلیدیں;
	تعداد config_type type;
	حرف *default_قدر;
	/* Populated via `display_options_init()`. */
	صحيح رکن;
	صحيح delim;
	صحيح کلید_delim;
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

ساكن صحيح option_parse_type(ثابت بنية option *opt, ثابت حرف *arg,
			     صحيح unset)
{
	صحيح new_type, *منزل_type;

	إذا (unset) {
		*((صحيح *) opt->قدر) = 0;
		أرجع 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	إذا (!new_type) {
		إذا (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		وإلا إذا (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		وإلا إذا (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		وإلا إذا (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		وإلا إذا (!strcmp(arg, "path"))
			new_type = TYPE_رستہ;
		وإلا إذا (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		وإلا إذا (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		وإلا
			die(_("unrecognized --type argument, %s"), arg);
	}

	منزل_type = opt->قدر;
	إذا (*منزل_type && *منزل_type != new_type) {
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
	*منزل_type = new_type;

	أرجع 0;
}

ساكن فراغ check_argc(صحيح argc, صحيح min, صحيح max)
{
	إذا (argc >= min && argc <= max)
		أرجع;
	إذا (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	وإلا
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	باہر_نکلو(129);
}

ساكن NORETURN فراغ die_missing_سیٹ_قدر(ثابت حرف *arg)
{
	ثابت حرف *آخری_dot = strrchr(arg, '.');
	ثابت حرف *eq = آخری_dot ? strchr(آخری_dot + 1, '=') : عدم;
	حرف *اگیتر = eq ? xstrndup(arg, eq - arg) : عدم;

	إذا (اگیتر && git_config_کلید_is_valid(اگیتر)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       اگیتر, eq + 1);
	} وإلا إذا (git_config_کلید_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} وإلا {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	حرر(اگیتر);
	باہر_نکلو(129);
}

ساكن فراغ show_config_origin(ثابت بنية config_display_options *opts,
			       ثابت بنية کلید_قدر_info *kvi,
			       بنية strbuf *buf)
{
	ثابت حرف رکن = opts->سرا_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	إذا (opts->سرا_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	وإلا
		quote_c_style(kvi->filename ? kvi->filename : "", buf, عدم, 0);
	strbuf_addch(buf, رکن);
}

ساكن فراغ show_config_scope(ثابت بنية config_display_options *opts,
			      ثابت بنية کلید_قدر_info *kvi,
			      بنية strbuf *buf)
{
	ثابت حرف رکن = opts->سرا_nul ? '\0' : '\t';
	ثابت حرف *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, رکن);
}

بنية strbuf_list {
	بنية strbuf *اشیاء;
	صحيح nr;
	صحيح alloc;
};

ساكن صحيح format_config_int64(بنية strbuf *buf,
			       ثابت حرف *کلید_2,
			       ثابت حرف *قدر_2,
			       ثابت بنية کلید_قدر_info *kvi,
			       صحيح gently)
{
	int64_t v = 0;
	إذا (gently) {
		إذا (!git_parse_int64(قدر_2, &v))
			أرجع -1;
	} وإلا {
		/* may die() */
		v = git_config_int64(کلید_2, قدر_2 ? قدر_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	أرجع 0;
}

ساكن صحيح format_config_bool(بنية strbuf *buf,
			      ثابت حرف *کلید_2,
			      ثابت حرف *قدر_2,
			      صحيح gently)
{
	صحيح v = 0;
	إذا (gently) {
		إذا ((v = git_parse_maybe_bool(قدر_2)) < 0)
			أرجع -1;
	} وإلا {
		/* may die() */
		v = git_config_bool(کلید_2, قدر_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	أرجع 0;
}

ساكن صحيح format_config_bool_or_int(بنية strbuf *buf,
				     ثابت حرف *کلید_2,
				     ثابت حرف *قدر_2,
				     ثابت بنية کلید_قدر_info *kvi,
				     صحيح gently)
{
	صحيح v, is_bool = 0;

	إذا (gently) {
		v = git_parse_maybe_bool_متن(قدر_2);

		إذا (v >= 0)
			is_bool = 1;
		وإلا إذا (!git_parse_int(قدر_2, &v))
			أرجع -1;
	} وإلا {
		v = git_config_bool_or_int(کلید_2, قدر_2, kvi,
					   &is_bool);
	}

	إذا (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	وإلا
		strbuf_addf(buf, "%d", v);

	أرجع 0;
}

/* This mode is always gentle. */
ساكن صحيح format_config_bool_or_str(بنية strbuf *buf,
				     ثابت حرف *قدر_2)
{
	صحيح v = git_parse_maybe_bool(قدر_2);
	إذا (v < 0)
		strbuf_addstr(buf, قدر_2);
	وإلا
		strbuf_addstr(buf, v ? "true" : "false");
	أرجع 0;
}

ساكن صحيح format_config_رستہ(بنية strbuf *buf,
			      ثابت حرف *کلید_2,
			      ثابت حرف *قدر_2,
			      صحيح gently)
{
	حرف *v;

	إذا (git_config_pathname(&v, کلید_2, قدر_2) < 0)
		أرجع -1;

	إذا (v)
		strbuf_addstr(buf, v);
	وإلا
		أرجع gently ? -1 : 1; /* :(optional)no-such-file */

	حرر(v);
	أرجع 0;
}

ساكن صحيح format_config_expiry_date(بنية strbuf *buf,
				     ثابت حرف *کلید_2,
				     ثابت حرف *قدر_2,
				     صحيح quietly)
{
	timestamp_t t;
	إذا (quietly) {
		إذا (parse_expiry_date(قدر_2, &t))
			أرجع -1;
	} وإلا إذا (git_config_expiry_date(&t, کلید_2, قدر_2) < 0) {
		أرجع -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	أرجع 0;
}

ساكن صحيح format_config_color(بنية strbuf *buf,
			       ثابت حرف *کلید_2,
			       ثابت حرف *قدر_2,
			       صحيح gently)
{
	حرف v[COLOR_MAXLEN];

	إذا (gently) {
		إذا (color_parse_quietly(قدر_2, v) < 0)
			أرجع -1;
	} وإلا إذا (git_config_color(v, کلید_2, قدر_2) < 0) {
		أرجع -1;
	}

	strbuf_addstr(buf, v);
	أرجع 0;
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
ساكن صحيح format_config(ثابت بنية config_display_options *opts,
			 بنية strbuf *buf, ثابت حرف *کلید_2,
			 ثابت حرف *قدر_2, ثابت بنية کلید_قدر_info *kvi,
			 صحيح gently)
{
	صحيح res = 0;
	إذا (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	إذا (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	إذا (opts->show_کلیدیں)
		strbuf_addstr(buf, کلید_2);

	إذا (opts->omit_اقدار)
		اذهب terminator;

	إذا (opts->show_کلیدیں)
		strbuf_addch(buf, opts->کلید_delim);

	بدل (opts->type) {
	حالة TYPE_INT:
		res = format_config_int64(buf, کلید_2, قدر_2, kvi, gently);
		اكسر;

	حالة TYPE_BOOL:
		res = format_config_bool(buf, کلید_2, قدر_2, gently);
		اكسر;

	حالة TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, کلید_2, قدر_2, kvi, gently);
		اكسر;

	حالة TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, قدر_2);
		اكسر;

	حالة TYPE_رستہ:
		res = format_config_رستہ(buf, کلید_2, قدر_2, gently);
		اكسر;

	حالة TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, کلید_2, قدر_2, gently);
		اكسر;

	حالة TYPE_COLOR:
		res = format_config_color(buf, کلید_2, قدر_2, gently);
		اكسر;

	حالة TYPE_NONE:
		إذا (قدر_2) {
			strbuf_addstr(buf, قدر_2);
		} وإلا {
			/* Just show the key name; back out delimiter */
			إذا (opts->show_کلیدیں)
				strbuf_setlen(buf, buf->len - 1);
		}
		اكسر;

	افتراضي:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->رکن);
	أرجع res;
}

ساكن صحيح show_all_config(ثابت حرف *کلید_2, ثابت حرف *قدر_2,
			   ثابت بنية config_context *ctx,
			   فراغ *cb)
{
	ثابت بنية config_display_options *opts = cb;
	ثابت بنية کلید_قدر_info *kvi = ctx->kvi;
	بنية strbuf formatted = STRBUF_INIT;

	إذا (format_config(opts, &formatted, کلید_2, قدر_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	أرجع 0;
}

#تعریف_کرو GET_قدر_ALL        (1 << 0)
#تعریف_کرو GET_قدر_کلید_REGEXP (1 << 1)

بنية collect_config_انکڑے {
	ثابت بنية config_display_options *display_opts;
	بنية strbuf_list *اقدار;
	ثابت حرف *قدر_نمونہ;
	ثابت حرف *کلید;
	regex_t *regexp;
	regex_t *کلید_regexp;
	صحيح do_not_میل;
	بنانشانی get_قدر_flags;
	بنانشانی flags;
};

ساكن صحيح collect_config(ثابت حرف *کلید_2, ثابت حرف *قدر_2,
			  ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية collect_config_انکڑے *انکڑے = cb;
	بنية strbuf_list *اقدار = انکڑے->اقدار;
	ثابت بنية کلید_قدر_info *kvi = ctx->kvi;
	صحيح حالت;

	إذا (!(انکڑے->get_قدر_flags & GET_قدر_کلید_REGEXP) &&
	    strcmp(کلید_2, انکڑے->کلید))
		أرجع 0;
	إذا ((انکڑے->get_قدر_flags & GET_قدر_کلید_REGEXP) &&
	    regexec(انکڑے->کلید_regexp, کلید_2, 0, عدم, 0))
		أرجع 0;
	إذا ((انکڑے->flags & CONFIG_FLAGS_FIXED_قدر) &&
	    strcmp(انکڑے->قدر_نمونہ, (قدر_2?قدر_2:"")))
		أرجع 0;
	إذا (انکڑے->regexp &&
	    (انکڑے->do_not_میل ^ !!regexec(انکڑے->regexp, (قدر_2?قدر_2:""), 0, عدم, 0)))
		أرجع 0;

	ALLOC_GROW(اقدار->اشیاء, اقدار->nr + 1, اقدار->alloc);
	strbuf_init(&اقدار->اشیاء[اقدار->nr], 0);

	حالت = format_config(انکڑے->display_opts, &اقدار->اشیاء[اقدار->nr++],
			       کلید_2, قدر_2, kvi, 0);
	إذا (حالت < 0)
		أرجع حالت;
	إذا (حالت) {
		strbuf_release(&اقدار->اشیاء[--اقدار->nr]);
		حالت = 0;
	}
	أرجع حالت;
}

ساكن صحيح get_قدر(ثابت بنية config_location_options *opts,
		     ثابت بنية config_display_options *display_opts,
		     ثابت حرف *کلید_2, ثابت حرف *regex_,
		     بنانشانی get_قدر_flags, بنانشانی flags)
{
	صحيح ret = CONFIG_GENERIC_ERROR;
	بنية strbuf_list اقدار = {عدم};
	بنية collect_config_انکڑے انکڑے = {
		.display_opts = display_opts,
		.اقدار = &اقدار,
		.get_قدر_flags = get_قدر_flags,
		.flags = flags,
	};
	حرف *کلید = عدم;
	صحيح i;

	إذا (get_قدر_flags & GET_قدر_کلید_REGEXP) {
		حرف *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		کلید = xstrdup(کلید_2);
		لكل (tl = کلید + لڑی_لمبائی(کلید) - 1;
		     tl >= کلید && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		لكل (tl = کلید; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		انکڑے.کلید_regexp = (regex_t*)xmalloc(حجم(regex_t));
		إذا (regcomp(انکڑے.کلید_regexp, کلید, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), کلید_2);
			FREE_AND_NULL(انکڑے.کلید_regexp);
			ret = CONFIG_INVALID_نمونہ;
			اذهب free_لڑیاں;
		}
	} وإلا {
		إذا (git_config_parse_کلید(کلید_2, &کلید, عدم)) {
			ret = CONFIG_INVALID_کلید;
			اذهب free_لڑیاں;
		}

		انکڑے.کلید = کلید;
	}

	إذا (regex_ && (flags & CONFIG_FLAGS_FIXED_قدر))
		انکڑے.قدر_نمونہ = regex_;
	وإلا إذا (regex_) {
		إذا (regex_[0] == '!') {
			انکڑے.do_not_میل = 1;
			regex_++;
		}

		انکڑے.regexp = (regex_t*)xmalloc(حجم(regex_t));
		إذا (regcomp(انکڑے.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(انکڑے.regexp);
			ret = CONFIG_INVALID_نمونہ;
			اذهب free_لڑیاں;
		}
	}

	config_with_options(collect_config, &انکڑے,
			    &opts->سروت, the_repository,
			    &opts->options);

	إذا (!اقدار.nr && display_opts->default_قدر) {
		بنية کلید_قدر_info kvi = KVI_INIT;
		بنية strbuf *شے;
		صحيح حالت;

		kvi_from_param(&kvi);
		ALLOC_GROW(اقدار.اشیاء, اقدار.nr + 1, اقدار.alloc);
		شے = &اقدار.اشیاء[اقدار.nr++];
		strbuf_init(شے, 0);

		حالت = format_config(display_opts, شے, کلید_2,
				       display_opts->default_قدر, &kvi, 0);
		إذا (حالت < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_قدر);
		إذا (حالت) {
			/* default was a missing optional value */
			اقدار.nr--;
			strbuf_release(شے);
		}
	}

	ret = !اقدار.nr;

	لكل (i = 0; i < اقدار.nr; i++) {
		بنية strbuf *buf = اقدار.اشیاء + i;
		إذا ((get_قدر_flags & GET_قدر_ALL) || i == اقدار.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	حرر(اقدار.اشیاء);

free_لڑیاں:
	حرر(کلید);
	إذا (انکڑے.کلید_regexp) {
		regfree(انکڑے.کلید_regexp);
		حرر(انکڑے.کلید_regexp);
	}
	إذا (انکڑے.regexp) {
		regfree(انکڑے.regexp);
		حرر(انکڑے.regexp);
	}

	أرجع ret;
}

ساكن حرف *normalize_قدر(ثابت حرف *کلید, ثابت حرف *قدر,
			     صحيح type, بنية کلید_قدر_info *kvi)
{
	إذا (!قدر)
		أرجع عدم;

	إذا (type == 0 || type == TYPE_رستہ || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		أرجع xstrdup(قدر);
	إذا (type == TYPE_INT)
		أرجع xstrfmt("%"PRId64, git_config_int64(کلید, قدر, kvi));
	إذا (type == TYPE_BOOL)
		أرجع xstrdup(git_config_bool(کلید, قدر) ?  "true" : "false");
	إذا (type == TYPE_BOOL_OR_INT) {
		صحيح is_bool, v;
		v = git_config_bool_or_int(کلید, قدر, kvi, &is_bool);
		إذا (!is_bool)
			أرجع xstrfmt("%d", v);
		وإلا
			أرجع xstrdup(v ? "true" : "false");
	}
	إذا (type == TYPE_BOOL_OR_STR) {
		صحيح v = git_parse_maybe_bool(قدر);
		إذا (v < 0)
			أرجع xstrdup(قدر);
		وإلا
			أرجع xstrdup(v ? "true" : "false");
	}
	إذا (type == TYPE_COLOR) {
		حرف v[COLOR_MAXLEN];
		إذا (git_config_color(v, کلید, قدر))
			die(_("cannot parse color '%s'"), قدر);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		أرجع xstrdup(قدر);
	}

	BUG("cannot normalize type %d", type);
}

بنية get_color_config_انکڑے {
	صحيح get_color_لبھیا;
	ثابت حرف *get_color_تھاں;
	حرف parsed_color[COLOR_MAXLEN];
};

ساكن صحيح git_get_color_config(ثابت حرف *var, ثابت حرف *قدر,
				ثابت بنية config_context *ctx بن_ورتیا,
				فراغ *cb)
{
	بنية get_color_config_انکڑے *انکڑے = cb;

	إذا (!strcmp(var, انکڑے->get_color_تھاں)) {
		إذا (!قدر)
			config_error_nonbool(var);
		إذا (color_parse(قدر, انکڑے->parsed_color) < 0)
			أرجع -1;
		انکڑے->get_color_لبھیا = 1;
	}
	أرجع 0;
}

ساكن صحيح get_color(ثابت بنية config_location_options *opts,
		      ثابت حرف *var, ثابت حرف *def_color)
{
	بنية get_color_config_انکڑے انکڑے = {
		.get_color_تھاں = var,
		.parsed_color[0] = '\0',
	};
	صحيح ret;

	config_with_options(git_get_color_config, &انکڑے,
			    &opts->سروت, the_repository,
			    &opts->options);

	إذا (!انکڑے.get_color_لبھیا && def_color) {
		إذا (color_parse(def_color, انکڑے.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			اذهب باہرلا;
		}
	}

	ret = 0;

باہرلا:
	fputs(انکڑے.parsed_color, stdout);
	أرجع ret;
}

بنية get_colorbool_config_انکڑے {
	تعداد git_colorbool get_colorbool_لبھیا;
	تعداد git_colorbool get_diff_color_لبھیا;
	تعداد git_colorbool get_color_ui_لبھیا;
	ثابت حرف *get_colorbool_تھاں;
};

ساكن صحيح git_get_colorbool_config(ثابت حرف *var, ثابت حرف *قدر,
				    ثابت بنية config_context *ctx بن_ورتیا,
				    فراغ *cb)
{
	بنية get_colorbool_config_انکڑے *انکڑے = cb;

	إذا (!strcmp(var, انکڑے->get_colorbool_تھاں))
		انکڑے->get_colorbool_لبھیا = git_config_colorbool(var, قدر);
	وإلا إذا (!strcmp(var, "diff.color"))
		انکڑے->get_diff_color_لبھیا = git_config_colorbool(var, قدر);
	وإلا إذا (!strcmp(var, "color.ui"))
		انکڑے->get_color_ui_لبھیا = git_config_colorbool(var, قدر);
	أرجع 0;
}

ساكن صحيح get_colorbool(ثابت بنية config_location_options *opts,
			 ثابت حرف *var, صحيح print)
{
	بنية get_colorbool_config_انکڑے انکڑے = {
		.get_colorbool_تھاں = var,
		.get_colorbool_لبھیا = GIT_COLOR_UNKNOWN,
		.get_diff_color_لبھیا = GIT_COLOR_UNKNOWN,
		.get_color_ui_لبھیا = GIT_COLOR_UNKNOWN,
	};
	bool نتیجہ;

	config_with_options(git_get_colorbool_config, &انکڑے,
			    &opts->سروت, the_repository,
			    &opts->options);

	إذا (انکڑے.get_colorbool_لبھیا == GIT_COLOR_UNKNOWN) {
		إذا (!strcmp(انکڑے.get_colorbool_تھاں, "color.diff"))
			انکڑے.get_colorbool_لبھیا = انکڑے.get_diff_color_لبھیا;
		إذا (انکڑے.get_colorbool_لبھیا == GIT_COLOR_UNKNOWN)
			انکڑے.get_colorbool_لبھیا = انکڑے.get_color_ui_لبھیا;
	}

	إذا (انکڑے.get_colorbool_لبھیا == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		انکڑے.get_colorbool_لبھیا = GIT_COLOR_AUTO;

	نتیجہ = want_color(انکڑے.get_colorbool_لبھیا);

	إذا (print) {
		لکھو("%s\n", نتیجہ ? "true" : "false");
		أرجع 0;
	} وإلا
		أرجع نتیجہ ? 0 : 1;
}

ساكن فراغ check_write(ثابت بنية git_config_سروت *سروت)
{
	إذا (!سروت->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	إذا (سروت->use_stdin)
		die(_("writing to stdin is not supported"));

	إذا (سروت->blob)
		die(_("writing config blobs is not supported"));
}

بنية urlmatch_current_candidate_قدر {
	حرف قدر_is_null;
	بنية strbuf قدر;
	بنية کلید_قدر_info kvi;
};

ساكن صحيح urlmatch_collect_fn(ثابت حرف *var, ثابت حرف *قدر,
			       ثابت بنية config_context *ctx,
			       فراغ *cb)
{
	بنية string_list *اقدار = cb;
	بنية string_list_شے *شے = string_list_داخل_کرو(اقدار, var);
	بنية urlmatch_current_candidate_قدر *matched = شے->util;
	ثابت بنية کلید_قدر_info *kvi = ctx->kvi;

	إذا (!matched) {
		matched = xmalloc(حجم(*matched));
		strbuf_init(&matched->قدر, 0);
		شے->util = matched;
	} وإلا {
		strbuf_reset(&matched->قدر);
	}
	matched->kvi = *kvi;

	إذا (قدر) {
		strbuf_addstr(&matched->قدر, قدر);
		matched->قدر_is_null = 0;
	} وإلا {
		matched->قدر_is_null = 1;
	}
	أرجع 0;
}

ساكن صحيح get_urlmatch(ثابت بنية config_location_options *opts,
			ثابت بنية config_display_options *_display_opts,
			ثابت حرف *var, ثابت حرف *url)
{
	صحيح ret;
	حرف *section;
	حرف *section_پوچھ;
	بنية config_display_options display_opts = *_display_opts;
	بنية string_list_شے *شے;
	بنية urlmatch_config config = URLMATCH_CONFIG_INIT;
	بنية string_list اقدار = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = عدم;
	config.cb = &اقدار;

	إذا (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_پوچھ = strchr(section, '.');
	إذا (section_پوچھ) {
		*section_پوچھ = '\0';
		config.کلید = section_پوچھ + 1;
		display_opts.show_کلیدیں = 0;
	} وإلا {
		config.کلید = عدم;
		display_opts.show_کلیدیں = 1;
	}

	config_with_options(urlmatch_config_اندراج, &config,
			    &opts->سروت, the_repository,
			    &opts->options);

	ret = !اقدار.nr;

	for_each_string_list_شے(شے, &اقدار) {
		بنية urlmatch_current_candidate_قدر *matched = شے->util;
		بنية strbuf buf = STRBUF_INIT;
		صحيح حالت;

		حالت = format_config(&display_opts, &buf, شے->string,
				       matched->قدر_is_null ? عدم : matched->قدر.buf,
				       &matched->kvi, 0);
		إذا (!حالت)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->قدر);
	}
	urlmatch_config_release(&config);
	string_list_clear(&اقدار, 1);
	حرر(config.url.url);

	حرر(section);
	أرجع ret;
}

ساكن حرف *default_user_config(فراغ)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	أرجع strbuf_detach(&buf, عدم);
}

ساكن فراغ location_options_init(بنية config_location_options *opts,
				  ثابت حرف *اگیتر)
{
	إذا (!opts->سروت.file)
		opts->سروت.file = opts->file_منزل_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	إذا (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->سروت.file + !!opts->سروت.blob > 1) {
		error(_("only one config file at a time"));
		باہر_نکلو(129);
	}

	إذا (!startup_info->have_repository) {
		إذا (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		إذا (opts->سروت.blob)
			die(_("--blob can only be used inside a git repository"));
		إذا (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	إذا (opts->سروت.file &&
			!strcmp(opts->سروت.file, "-")) {
		opts->سروت.file = عدم;
		opts->سروت.use_stdin = 1;
		opts->سروت.scope = CONFIG_SCOPE_COMMAND;
	}

	إذا (opts->use_global_config) {
		opts->سروت.file = opts->file_منزل_free = git_global_config();
		إذا (!opts->سروت.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->سروت.scope = CONFIG_SCOPE_GLOBAL;
	} وإلا إذا (opts->use_system_config) {
		opts->سروت.file = opts->file_منزل_free = git_system_config();
		opts->سروت.scope = CONFIG_SCOPE_SYSTEM;
	} وإلا إذا (opts->use_local_config) {
		opts->سروت.file = opts->file_منزل_free = repo_git_رستہ(the_repository, "config");
		opts->سروت.scope = CONFIG_SCOPE_LOCAL;
	} وإلا إذا (opts->use_worktree_config) {
		بنية worktree **worktrees = get_worktrees(the_repository);
		إذا (the_repository->repository_format_worktree_config)
			opts->سروت.file = opts->file_منزل_free =
				repo_git_رستہ(the_repository, "config.worktree");
		وإلا إذا (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		وإلا
			opts->سروت.file = opts->file_منزل_free =
				repo_git_رستہ(the_repository, "config");
		opts->سروت.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} وإلا إذا (opts->سروت.file) {
		إذا (!is_absolute_رستہ(opts->سروت.file) && اگیتر)
			opts->سروت.file = opts->file_منزل_free =
				اگیتر_filename(اگیتر, opts->سروت.file);
		opts->سروت.scope = CONFIG_SCOPE_COMMAND;
	} وإلا إذا (opts->سروت.blob) {
		opts->سروت.scope = CONFIG_SCOPE_COMMAND;
	}

	إذا (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->سروت.file;
	وإلا
		opts->options.respect_includes = opts->respect_includes_opt;
	إذا (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ساكن فراغ location_options_release(بنية config_location_options *opts)
{
	حرر(opts->file_منزل_free);
}

ساكن فراغ display_options_init(بنية config_display_options *opts)
{
	إذا (opts->سرا_nul) {
		opts->رکن = '\0';
		opts->delim = '\n';
		opts->کلید_delim = '\n';
	}
}

ساكن فراغ display_options_init_list(بنية config_display_options *opts)
{
	opts->show_کلیدیں = 1;

	إذا (opts->سرا_nul) {
		display_options_init(opts);
	} وإلا {
		opts->رکن = '\n';
		opts->delim = ' ';
		opts->کلید_delim = '=';
	}
}

ساكن صحيح cmd_config_list(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			   بنية repository *repo بن_ورتیا)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_گروہ(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_سرا(),
	};

	argc = parse_options(argc, argv, اگیتر, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, اگیتر);
	display_options_init_list(&display_opts);

	تیاری_auto_pager("config", 1);

	إذا (config_with_options(show_all_config, &display_opts,
				&location_opts.سروت, the_repository,
				&location_opts.options) < 0) {
		إذا (location_opts.سروت.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.سروت.file);
		وإلا
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	أرجع 0;
}

ساكن صحيح cmd_config_get(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			  بنية repository *repo بن_ورتیا)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *قدر_نمونہ = عدم, *url = عدم;
	صحيح flags = 0;
	بنانشانی get_قدر_flags = 0;
	بنية option opts[] = {
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
		OPT_سرا(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	إذا ((flags & CONFIG_FLAGS_FIXED_قدر) && !قدر_نمونہ)
		die(_("--fixed-value only applies with 'value-pattern'"));
	إذا (display_opts.default_قدر &&
	    ((get_قدر_flags & GET_قدر_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	إذا (url && ((get_قدر_flags & GET_قدر_ALL) ||
		    (get_قدر_flags & GET_قدر_کلید_REGEXP) ||
		    قدر_نمونہ))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, اگیتر);
	display_options_init(&display_opts);

	إذا (display_opts.type != TYPE_COLOR)
		تیاری_auto_pager("config", 1);

	إذا (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	وإلا إذا (display_opts.type == TYPE_COLOR && !لڑی_لمبائی(argv[0]) && display_opts.default_قدر)
		ret = get_color(&location_opts, "", display_opts.default_قدر);
	وإلا
		ret = get_قدر(&location_opts, &display_opts, argv[0], قدر_نمونہ,
				get_قدر_flags, flags);

	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_سیٹ(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			  بنية repository *repo بن_ورتیا)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *قدر_نمونہ = عدم, *comment_arg = عدم;
	حرف *comment = عدم;
	صحيح flags = 0, append = 0, type = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_گروہ(N_("Filter")),
		OPT_ثنائی_ہندسہ(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &قدر_نمونہ, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_ثنائی_ہندسہ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_قدر),
		OPT_گروہ(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_سرا(),
	};
	بنية کلید_قدر_info default_kvi = KVI_INIT;
	حرف *قدر;
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, opts, builtin_config_سیٹ_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (argc == 1)
		die_missing_سیٹ_قدر(argv[0]);
	check_argc(argc, 2, 2);

	إذا ((flags & CONFIG_FLAGS_FIXED_قدر) && !قدر_نمونہ)
		die(_("--fixed-value only applies with --value=<pattern>"));
	إذا (append && قدر_نمونہ)
		die(_("--append cannot be used with --value=<pattern>"));
	إذا (append)
		قدر_نمونہ = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, اگیتر);
	check_write(&location_opts.سروت);

	قدر = normalize_قدر(argv[0], argv[1], type, &default_kvi);

	إذا ((flags & CONFIG_FLAGS_MULTI_REPLACE) || قدر_نمونہ) {
		ret = repo_config_سیٹ_multivar_وچ_file_gently(the_repository, location_opts.سروت.file,
							      argv[0], قدر, قدر_نمونہ,
							      comment, flags);
	} وإلا {
		ret = repo_config_سیٹ_وچ_file_gently(the_repository, location_opts.سروت.file,
						     argv[0], comment, قدر);
		إذا (ret == CONFIG_NOTHING_سیٹ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	حرر(comment);
	حرر(قدر);
	أرجع ret;
}

ساكن صحيح cmd_config_unset(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			    بنية repository *repo بن_ورتیا)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *قدر_نمونہ = عدم;
	صحيح flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_گروہ(N_("Filter")),
		OPT_ثنائی_ہندسہ(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &قدر_نمونہ, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_ثنائی_ہندسہ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_قدر),
		OPT_سرا(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	إذا ((flags & CONFIG_FLAGS_FIXED_قدر) && !قدر_نمونہ)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, اگیتر);
	check_write(&location_opts.سروت);

	إذا ((flags & CONFIG_FLAGS_MULTI_REPLACE) || قدر_نمونہ)
		ret = repo_config_سیٹ_multivar_وچ_file_gently(the_repository, location_opts.سروت.file,
							      argv[0], عدم, قدر_نمونہ,
							      عدم, flags);
	وإلا
		ret = repo_config_سیٹ_وچ_file_gently(the_repository, location_opts.سروت.file, argv[0],
						     عدم, عدم);

	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_rename_section(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
				     بنية repository *repo بن_ورتیا)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_سرا(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, اگیتر);
	check_write(&location_opts.سروت);

	ret = repo_config_rename_section_وچ_file(the_repository, location_opts.سروت.file,
						 argv[0], argv[1]);
	إذا (ret < 0)
		اذهب باہرلا;
	وإلا إذا (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

باہرلا:
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_remove_section(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
				     بنية repository *repo بن_ورتیا)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_سرا(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, اگیتر);
	check_write(&location_opts.سروت);

	ret = repo_config_rename_section_وچ_file(the_repository, location_opts.سروت.file,
						 argv[0], عدم);
	إذا (ret < 0)
		اذهب باہرلا;
	وإلا إذا (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

باہرلا:
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح show_editor(بنية config_location_options *opts)
{
	حرف *config_file;

	إذا (!opts->سروت.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	إذا (opts->سروت.use_stdin)
		die(_("editing stdin is not supported"));
	إذا (opts->سروت.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, عدم);
	config_file = opts->سروت.file ?
			xstrdup(opts->سروت.file) :
			repo_git_رستہ(the_repository, "config");
	إذا (opts->use_global_config) {
		صحيح fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		إذا (fd >= 0) {
			حرف *content = default_user_config();
			write_str_وچ_بھریا(fd, content);
			حرر(content);
			close(fd);
		}
		وإلا إذا (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	إذا (launch_editor(config_file, عدم, عدم)) {
		حرر(config_file);
		أرجع -1;
	}
	حرر(config_file);

	أرجع 0;
}

ساكن صحيح cmd_config_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			   بنية repository *repo بن_ورتیا)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_سرا(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, اگیتر);
	check_write(&location_opts.سروت);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_actions(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر)
{
	تعداد {
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
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *comment_arg = عدم;
	صحيح actions = 0;
	بنانشانی flags = 0;
	بنية option opts[] = {
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
		OPT_سرا(),
	};
	حرف *قدر = عدم, *comment = عدم;
	صحيح ret = 0;
	صحيح actions_implicit;
	بنية کلید_قدر_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, اگیتر, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, اگیتر);
	display_options_init(&display_opts);

	إذا ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		باہر_نکلو(129);
	}

	actions_implicit = (actions == 0);
	إذا (actions_implicit)
		بدل (argc) {
		حالة 1: actions = ACTION_GET; اكسر;
		حالة 2: actions = ACTION_سیٹ; اكسر;
		حالة 3: actions = ACTION_سیٹ_ALL; اكسر;
		افتراضي:
			error(_("no action specified"));
			باہر_نکلو(129);
		}
	إذا (actions_implicit && argc == 1) {
		ثابت حرف *آخری_dot = strrchr(argv[0], '.');
		إذا (آخری_dot && strchr(آخری_dot + 1, '='))
			die_missing_سیٹ_قدر(argv[0]);
	}
	إذا (display_opts.omit_اقدار &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		باہر_نکلو(129);
	}

	إذا (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		باہر_نکلو(129);
	}

	إذا (display_opts.default_قدر && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		باہر_نکلو(129);
	}

	إذا (comment_arg &&
	    !(actions & (ACTION_جوڑو|ACTION_سیٹ|ACTION_سیٹ_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		باہر_نکلو(129);
	}

	/* check usage of --fixed-value */
	إذا (flags & CONFIG_FLAGS_FIXED_قدر) {
		صحيح allowed_usage = 0;

		بدل (actions) {
		/* git config --get <name> <value-pattern> */
		حالة ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		حالة ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		حالة ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		حالة ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		حالة ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			اكسر;

		/* git config <name> <value> <value-pattern> */
		حالة ACTION_سیٹ_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		حالة ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			اكسر;

		/* other options don't allow --fixed-value */
		}

		إذا (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			باہر_نکلو(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	إذا (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		تیاری_auto_pager("config", 1);

	إذا (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		إذا (config_with_options(show_all_config, &display_opts,
					&location_opts.سروت, the_repository,
					&location_opts.options) < 0) {
			إذا (location_opts.سروت.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.سروت.file);
			وإلا
				die(_("error processing config file(s)"));
		}
	}
	وإلا إذا (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	وإلا إذا (actions == ACTION_سیٹ) {
		check_write(&location_opts.سروت);
		check_argc(argc, 2, 2);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_وچ_file_gently(the_repository, location_opts.سروت.file, argv[0], comment, قدر);
		إذا (ret == CONFIG_NOTHING_سیٹ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	وإلا إذا (actions == ACTION_سیٹ_ALL) {
		check_write(&location_opts.سروت);
		check_argc(argc, 2, 3);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_multivar_وچ_file_gently(the_repository, location_opts.سروت.file,
							      argv[0], قدر, argv[2],
							      comment, flags);
	}
	وإلا إذا (actions == ACTION_جوڑو) {
		check_write(&location_opts.سروت);
		check_argc(argc, 2, 2);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_multivar_وچ_file_gently(the_repository, location_opts.سروت.file,
							      argv[0], قدر,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	وإلا إذا (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.سروت);
		check_argc(argc, 2, 3);
		قدر = normalize_قدر(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سیٹ_multivar_وچ_file_gently(the_repository, location_opts.سروت.file,
							      argv[0], قدر, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وإلا إذا (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_قدر(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	وإلا إذا (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_قدر(&location_opts, &display_opts, argv[0], argv[1],
				GET_قدر_ALL, flags);
	}
	وإلا إذا (actions == ACTION_GET_REGEXP) {
		display_opts.show_کلیدیں = 1;
		check_argc(argc, 1, 2);
		ret = get_قدر(&location_opts, &display_opts, argv[0], argv[1],
				GET_قدر_ALL|GET_قدر_کلید_REGEXP, flags);
	}
	وإلا إذا (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	وإلا إذا (actions == ACTION_UNSET) {
		check_write(&location_opts.سروت);
		check_argc(argc, 1, 2);
		إذا (argc == 2)
			ret = repo_config_سیٹ_multivar_وچ_file_gently(the_repository, location_opts.سروت.file,
								      argv[0], عدم, argv[1],
								      عدم, flags);
		وإلا
			ret = repo_config_سیٹ_وچ_file_gently(the_repository, location_opts.سروت.file,
							     argv[0], عدم, عدم);
	}
	وإلا إذا (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.سروت);
		check_argc(argc, 1, 2);
		ret = repo_config_سیٹ_multivar_وچ_file_gently(the_repository, location_opts.سروت.file,
							      argv[0], عدم, argv[1],
							      عدم, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وإلا إذا (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.سروت);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_وچ_file(the_repository, location_opts.سروت.file,
							 argv[0], argv[1]);
		إذا (ret < 0)
			اذهب باہرلا;
		وإلا إذا (!ret)
			die(_("no such section: %s"), argv[0]);
		وإلا
			ret = 0;
	}
	وإلا إذا (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.سروت);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_وچ_file(the_repository, location_opts.سروت.file,
							 argv[0], عدم);
		إذا (ret < 0)
			اذهب باہرلا;
		وإلا إذا (!ret)
			die(_("no such section: %s"), argv[0]);
		وإلا
			ret = 0;
	}
	وإلا إذا (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	وإلا إذا (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		إذا (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

باہرلا:
	location_options_release(&location_opts);
	حرر(comment);
	حرر(قدر);
	أرجع ret;
}

صحيح cmd_config(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اگیتر,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *subcommand = عدم;
	بنية option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_سیٹ),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_سرا(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, اگیتر, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	إذا (subcommand) {
		argc = parse_options(argc, argv, اگیتر, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		أرجع subcommand(argc, argv, اگیتر, repo);
	}

	أرجع cmd_config_actions(argc, argv, اگیتر);
}
