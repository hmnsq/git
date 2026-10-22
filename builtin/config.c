#تعریف_کن USE_THE_REPOSITORY_VARIABLE
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

ساكن ثابت حرف *ثابت builtin_config_مجموعه_usage[] = {
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

#تعریف_کن CONFIG_LOCATION_OPTIONS(opts) \
	OPT_گروه(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.منبع.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.منبع.blob, N_("blob-id"), N_("read config from given blob object"))

بنية config_location_options {
	بنية git_config_منبع منبع;
	بنية config_options options;
	حرف *file_مقصد_free;
	صحيح use_global_config;
	صحيح use_system_config;
	صحيح use_local_config;
	صحيح use_worktree_config;
	صحيح respect_includes_opt;
};
#تعریف_کن CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

تعداد config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_مسیر,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#تعریف_کن CONFIG_TYPE_OPTIONS(type) \
	OPT_گروه(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_مقدار(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_مقدار(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_مقدار(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_مقدار(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_مقدار(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_مسیر), \
	OPT_CALLBACK_مقدار(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#تعریف_کن CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_گروه(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.انتها_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_مقادیر, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_کلیدها, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

بنية config_display_options {
	صحيح انتها_nul;
	صحيح omit_مقادیر;
	صحيح show_origin;
	صحيح show_scope;
	صحيح show_کلیدها;
	تعداد config_type type;
	حرف *default_مقدار;
	/* Populated via `display_options_init()`. */
	صحيح جمله;
	صحيح delim;
	صحيح کلید_delim;
};
#تعریف_کن CONFIG_DISPLAY_OPTIONS_INIT { \
	.جمله = '\n', \
	.delim = '=', \
	.کلید_delim = ' ', \
	.type = TYPE_NONE, \
}

#تعریف_کن OPT_CALLBACK_مقدار(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.مقدار = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ساكن صحيح option_parse_type(ثابت بنية option *opt, ثابت حرف *arg,
			     صحيح unset)
{
	صحيح new_type, *مقصد_type;

	إذا (unset) {
		*((صحيح *) opt->مقدار) = 0;
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
			new_type = TYPE_مسیر;
		وإلا إذا (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		وإلا إذا (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		وإلا
			die(_("unrecognized --type argument, %s"), arg);
	}

	مقصد_type = opt->مقدار;
	إذا (*مقصد_type && *مقصد_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		خروج(129);
	}
	*مقصد_type = new_type;

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
	خروج(129);
}

ساكن NORETURN فراغ die_missing_مجموعه_مقدار(ثابت حرف *arg)
{
	ثابت حرف *آخرین_dot = strrchr(arg, '.');
	ثابت حرف *eq = آخرین_dot ? strchr(آخرین_dot + 1, '=') : عدم;
	حرف *پیشوند = eq ? xstrndup(arg, eq - arg) : عدم;

	إذا (پیشوند && git_config_کلید_is_valid(پیشوند)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       پیشوند, eq + 1);
	} وإلا إذا (git_config_کلید_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} وإلا {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	حرر(پیشوند);
	خروج(129);
}

ساكن فراغ show_config_origin(ثابت بنية config_display_options *opts,
			       ثابت بنية کلید_مقدار_info *kvi,
			       بنية strbuf *buf)
{
	ثابت حرف جمله = opts->انتها_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	إذا (opts->انتها_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	وإلا
		quote_c_style(kvi->filename ? kvi->filename : "", buf, عدم, 0);
	strbuf_addch(buf, جمله);
}

ساكن فراغ show_config_scope(ثابت بنية config_display_options *opts,
			      ثابت بنية کلید_مقدار_info *kvi,
			      بنية strbuf *buf)
{
	ثابت حرف جمله = opts->انتها_nul ? '\0' : '\t';
	ثابت حرف *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, جمله);
}

بنية strbuf_list {
	بنية strbuf *اقلام;
	صحيح nr;
	صحيح alloc;
};

ساكن صحيح format_config_int64(بنية strbuf *buf,
			       ثابت حرف *کلید_2,
			       ثابت حرف *مقدار_2,
			       ثابت بنية کلید_مقدار_info *kvi,
			       صحيح gently)
{
	int64_t v = 0;
	إذا (gently) {
		إذا (!git_parse_int64(مقدار_2, &v))
			أرجع -1;
	} وإلا {
		/* may die() */
		v = git_config_int64(کلید_2, مقدار_2 ? مقدار_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	أرجع 0;
}

ساكن صحيح format_config_bool(بنية strbuf *buf,
			      ثابت حرف *کلید_2,
			      ثابت حرف *مقدار_2,
			      صحيح gently)
{
	صحيح v = 0;
	إذا (gently) {
		إذا ((v = git_parse_maybe_bool(مقدار_2)) < 0)
			أرجع -1;
	} وإلا {
		/* may die() */
		v = git_config_bool(کلید_2, مقدار_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	أرجع 0;
}

ساكن صحيح format_config_bool_or_int(بنية strbuf *buf,
				     ثابت حرف *کلید_2,
				     ثابت حرف *مقدار_2,
				     ثابت بنية کلید_مقدار_info *kvi,
				     صحيح gently)
{
	صحيح v, is_bool = 0;

	إذا (gently) {
		v = git_parse_maybe_bool_متن(مقدار_2);

		إذا (v >= 0)
			is_bool = 1;
		وإلا إذا (!git_parse_int(مقدار_2, &v))
			أرجع -1;
	} وإلا {
		v = git_config_bool_or_int(کلید_2, مقدار_2, kvi,
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
				     ثابت حرف *مقدار_2)
{
	صحيح v = git_parse_maybe_bool(مقدار_2);
	إذا (v < 0)
		strbuf_addstr(buf, مقدار_2);
	وإلا
		strbuf_addstr(buf, v ? "true" : "false");
	أرجع 0;
}

ساكن صحيح format_config_مسیر(بنية strbuf *buf,
			      ثابت حرف *کلید_2,
			      ثابت حرف *مقدار_2,
			      صحيح gently)
{
	حرف *v;

	إذا (git_config_pathname(&v, کلید_2, مقدار_2) < 0)
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
				     ثابت حرف *مقدار_2,
				     صحيح quietly)
{
	timestamp_t t;
	إذا (quietly) {
		إذا (parse_expiry_date(مقدار_2, &t))
			أرجع -1;
	} وإلا إذا (git_config_expiry_date(&t, کلید_2, مقدار_2) < 0) {
		أرجع -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	أرجع 0;
}

ساكن صحيح format_config_color(بنية strbuf *buf,
			       ثابت حرف *کلید_2,
			       ثابت حرف *مقدار_2,
			       صحيح gently)
{
	حرف v[COLOR_MAXLEN];

	إذا (gently) {
		إذا (color_parse_quietly(مقدار_2, v) < 0)
			أرجع -1;
	} وإلا إذا (git_config_color(v, کلید_2, مقدار_2) < 0) {
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
			 ثابت حرف *مقدار_2, ثابت بنية کلید_مقدار_info *kvi,
			 صحيح gently)
{
	صحيح res = 0;
	إذا (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	إذا (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	إذا (opts->show_کلیدها)
		strbuf_addstr(buf, کلید_2);

	إذا (opts->omit_مقادیر)
		اذهب terminator;

	إذا (opts->show_کلیدها)
		strbuf_addch(buf, opts->کلید_delim);

	بدل (opts->type) {
	حالة TYPE_INT:
		res = format_config_int64(buf, کلید_2, مقدار_2, kvi, gently);
		اكسر;

	حالة TYPE_BOOL:
		res = format_config_bool(buf, کلید_2, مقدار_2, gently);
		اكسر;

	حالة TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, کلید_2, مقدار_2, kvi, gently);
		اكسر;

	حالة TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, مقدار_2);
		اكسر;

	حالة TYPE_مسیر:
		res = format_config_مسیر(buf, کلید_2, مقدار_2, gently);
		اكسر;

	حالة TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, کلید_2, مقدار_2, gently);
		اكسر;

	حالة TYPE_COLOR:
		res = format_config_color(buf, کلید_2, مقدار_2, gently);
		اكسر;

	حالة TYPE_NONE:
		إذا (مقدار_2) {
			strbuf_addstr(buf, مقدار_2);
		} وإلا {
			/* Just show the key name; back out delimiter */
			إذا (opts->show_کلیدها)
				strbuf_setlen(buf, buf->len - 1);
		}
		اكسر;

	افتراضي:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->جمله);
	أرجع res;
}

ساكن صحيح show_all_config(ثابت حرف *کلید_2, ثابت حرف *مقدار_2,
			   ثابت بنية config_context *ctx,
			   فراغ *cb)
{
	ثابت بنية config_display_options *opts = cb;
	ثابت بنية کلید_مقدار_info *kvi = ctx->kvi;
	بنية strbuf formatted = STRBUF_INIT;

	إذا (format_config(opts, &formatted, کلید_2, مقدار_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	أرجع 0;
}

#تعریف_کن GET_مقدار_ALL        (1 << 0)
#تعریف_کن GET_مقدار_کلید_REGEXP (1 << 1)

بنية collect_config_داده {
	ثابت بنية config_display_options *display_opts;
	بنية strbuf_list *مقادیر;
	ثابت حرف *مقدار_الگو;
	ثابت حرف *کلید;
	regex_t *regexp;
	regex_t *کلید_regexp;
	صحيح do_not_تطابق;
	بیعلامت get_مقدار_flags;
	بیعلامت flags;
};

ساكن صحيح collect_config(ثابت حرف *کلید_2, ثابت حرف *مقدار_2,
			  ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية collect_config_داده *داده = cb;
	بنية strbuf_list *مقادیر = داده->مقادیر;
	ثابت بنية کلید_مقدار_info *kvi = ctx->kvi;
	صحيح وضعیت;

	إذا (!(داده->get_مقدار_flags & GET_مقدار_کلید_REGEXP) &&
	    strcmp(کلید_2, داده->کلید))
		أرجع 0;
	إذا ((داده->get_مقدار_flags & GET_مقدار_کلید_REGEXP) &&
	    regexec(داده->کلید_regexp, کلید_2, 0, عدم, 0))
		أرجع 0;
	إذا ((داده->flags & CONFIG_FLAGS_FIXED_مقدار) &&
	    strcmp(داده->مقدار_الگو, (مقدار_2?مقدار_2:"")))
		أرجع 0;
	إذا (داده->regexp &&
	    (داده->do_not_تطابق ^ !!regexec(داده->regexp, (مقدار_2?مقدار_2:""), 0, عدم, 0)))
		أرجع 0;

	ALLOC_GROW(مقادیر->اقلام, مقادیر->nr + 1, مقادیر->alloc);
	strbuf_init(&مقادیر->اقلام[مقادیر->nr], 0);

	وضعیت = format_config(داده->display_opts, &مقادیر->اقلام[مقادیر->nr++],
			       کلید_2, مقدار_2, kvi, 0);
	إذا (وضعیت < 0)
		أرجع وضعیت;
	إذا (وضعیت) {
		strbuf_release(&مقادیر->اقلام[--مقادیر->nr]);
		وضعیت = 0;
	}
	أرجع وضعیت;
}

ساكن صحيح get_مقدار(ثابت بنية config_location_options *opts,
		     ثابت بنية config_display_options *display_opts,
		     ثابت حرف *کلید_2, ثابت حرف *regex_,
		     بیعلامت get_مقدار_flags, بیعلامت flags)
{
	صحيح ret = CONFIG_GENERIC_ERROR;
	بنية strbuf_list مقادیر = {عدم};
	بنية collect_config_داده داده = {
		.display_opts = display_opts,
		.مقادیر = &مقادیر,
		.get_مقدار_flags = get_مقدار_flags,
		.flags = flags,
	};
	حرف *کلید = عدم;
	صحيح i;

	إذا (get_مقدار_flags & GET_مقدار_کلید_REGEXP) {
		حرف *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		کلید = xstrdup(کلید_2);
		لكل (tl = کلید + طول_رشته(کلید) - 1;
		     tl >= کلید && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		لكل (tl = کلید; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		داده.کلید_regexp = (regex_t*)xmalloc(حجم(regex_t));
		إذا (regcomp(داده.کلید_regexp, کلید, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), کلید_2);
			FREE_AND_NULL(داده.کلید_regexp);
			ret = CONFIG_INVALID_الگو;
			اذهب free_رشتهها;
		}
	} وإلا {
		إذا (git_config_parse_کلید(کلید_2, &کلید, عدم)) {
			ret = CONFIG_INVALID_کلید;
			اذهب free_رشتهها;
		}

		داده.کلید = کلید;
	}

	إذا (regex_ && (flags & CONFIG_FLAGS_FIXED_مقدار))
		داده.مقدار_الگو = regex_;
	وإلا إذا (regex_) {
		إذا (regex_[0] == '!') {
			داده.do_not_تطابق = 1;
			regex_++;
		}

		داده.regexp = (regex_t*)xmalloc(حجم(regex_t));
		إذا (regcomp(داده.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(داده.regexp);
			ret = CONFIG_INVALID_الگو;
			اذهب free_رشتهها;
		}
	}

	config_with_options(collect_config, &داده,
			    &opts->منبع, the_repository,
			    &opts->options);

	إذا (!مقادیر.nr && display_opts->default_مقدار) {
		بنية کلید_مقدار_info kvi = KVI_INIT;
		بنية strbuf *قلم;
		صحيح وضعیت;

		kvi_from_param(&kvi);
		ALLOC_GROW(مقادیر.اقلام, مقادیر.nr + 1, مقادیر.alloc);
		قلم = &مقادیر.اقلام[مقادیر.nr++];
		strbuf_init(قلم, 0);

		وضعیت = format_config(display_opts, قلم, کلید_2,
				       display_opts->default_مقدار, &kvi, 0);
		إذا (وضعیت < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_مقدار);
		إذا (وضعیت) {
			/* default was a missing optional value */
			مقادیر.nr--;
			strbuf_release(قلم);
		}
	}

	ret = !مقادیر.nr;

	لكل (i = 0; i < مقادیر.nr; i++) {
		بنية strbuf *buf = مقادیر.اقلام + i;
		إذا ((get_مقدار_flags & GET_مقدار_ALL) || i == مقادیر.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	حرر(مقادیر.اقلام);

free_رشتهها:
	حرر(کلید);
	إذا (داده.کلید_regexp) {
		regfree(داده.کلید_regexp);
		حرر(داده.کلید_regexp);
	}
	إذا (داده.regexp) {
		regfree(داده.regexp);
		حرر(داده.regexp);
	}

	أرجع ret;
}

ساكن حرف *normalize_مقدار(ثابت حرف *کلید, ثابت حرف *مقدار,
			     صحيح type, بنية کلید_مقدار_info *kvi)
{
	إذا (!مقدار)
		أرجع عدم;

	إذا (type == 0 || type == TYPE_مسیر || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		أرجع xstrdup(مقدار);
	إذا (type == TYPE_INT)
		أرجع xstrfmt("%"PRId64, git_config_int64(کلید, مقدار, kvi));
	إذا (type == TYPE_BOOL)
		أرجع xstrdup(git_config_bool(کلید, مقدار) ?  "true" : "false");
	إذا (type == TYPE_BOOL_OR_INT) {
		صحيح is_bool, v;
		v = git_config_bool_or_int(کلید, مقدار, kvi, &is_bool);
		إذا (!is_bool)
			أرجع xstrfmt("%d", v);
		وإلا
			أرجع xstrdup(v ? "true" : "false");
	}
	إذا (type == TYPE_BOOL_OR_STR) {
		صحيح v = git_parse_maybe_bool(مقدار);
		إذا (v < 0)
			أرجع xstrdup(مقدار);
		وإلا
			أرجع xstrdup(v ? "true" : "false");
	}
	إذا (type == TYPE_COLOR) {
		حرف v[COLOR_MAXLEN];
		إذا (git_config_color(v, کلید, مقدار))
			die(_("cannot parse color '%s'"), مقدار);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		أرجع xstrdup(مقدار);
	}

	BUG("cannot normalize type %d", type);
}

بنية get_color_config_داده {
	صحيح get_color_یافت;
	ثابت حرف *get_color_جایگاه;
	حرف parsed_color[COLOR_MAXLEN];
};

ساكن صحيح git_get_color_config(ثابت حرف *var, ثابت حرف *مقدار,
				ثابت بنية config_context *ctx استفادهنشده,
				فراغ *cb)
{
	بنية get_color_config_داده *داده = cb;

	إذا (!strcmp(var, داده->get_color_جایگاه)) {
		إذا (!مقدار)
			config_error_nonbool(var);
		إذا (color_parse(مقدار, داده->parsed_color) < 0)
			أرجع -1;
		داده->get_color_یافت = 1;
	}
	أرجع 0;
}

ساكن صحيح get_color(ثابت بنية config_location_options *opts,
		      ثابت حرف *var, ثابت حرف *def_color)
{
	بنية get_color_config_داده داده = {
		.get_color_جایگاه = var,
		.parsed_color[0] = '\0',
	};
	صحيح ret;

	config_with_options(git_get_color_config, &داده,
			    &opts->منبع, the_repository,
			    &opts->options);

	إذا (!داده.get_color_یافت && def_color) {
		إذا (color_parse(def_color, داده.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			اذهب بیرون;
		}
	}

	ret = 0;

بیرون:
	fputs(داده.parsed_color, stdout);
	أرجع ret;
}

بنية get_colorbool_config_داده {
	تعداد git_colorbool get_colorbool_یافت;
	تعداد git_colorbool get_diff_color_یافت;
	تعداد git_colorbool get_color_ui_یافت;
	ثابت حرف *get_colorbool_جایگاه;
};

ساكن صحيح git_get_colorbool_config(ثابت حرف *var, ثابت حرف *مقدار,
				    ثابت بنية config_context *ctx استفادهنشده,
				    فراغ *cb)
{
	بنية get_colorbool_config_داده *داده = cb;

	إذا (!strcmp(var, داده->get_colorbool_جایگاه))
		داده->get_colorbool_یافت = git_config_colorbool(var, مقدار);
	وإلا إذا (!strcmp(var, "diff.color"))
		داده->get_diff_color_یافت = git_config_colorbool(var, مقدار);
	وإلا إذا (!strcmp(var, "color.ui"))
		داده->get_color_ui_یافت = git_config_colorbool(var, مقدار);
	أرجع 0;
}

ساكن صحيح get_colorbool(ثابت بنية config_location_options *opts,
			 ثابت حرف *var, صحيح print)
{
	بنية get_colorbool_config_داده داده = {
		.get_colorbool_جایگاه = var,
		.get_colorbool_یافت = GIT_COLOR_UNKNOWN,
		.get_diff_color_یافت = GIT_COLOR_UNKNOWN,
		.get_color_ui_یافت = GIT_COLOR_UNKNOWN,
	};
	bool نتیجه;

	config_with_options(git_get_colorbool_config, &داده,
			    &opts->منبع, the_repository,
			    &opts->options);

	إذا (داده.get_colorbool_یافت == GIT_COLOR_UNKNOWN) {
		إذا (!strcmp(داده.get_colorbool_جایگاه, "color.diff"))
			داده.get_colorbool_یافت = داده.get_diff_color_یافت;
		إذا (داده.get_colorbool_یافت == GIT_COLOR_UNKNOWN)
			داده.get_colorbool_یافت = داده.get_color_ui_یافت;
	}

	إذا (داده.get_colorbool_یافت == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		داده.get_colorbool_یافت = GIT_COLOR_AUTO;

	نتیجه = want_color(داده.get_colorbool_یافت);

	إذا (print) {
		چاپ_کن("%s\n", نتیجه ? "true" : "false");
		أرجع 0;
	} وإلا
		أرجع نتیجه ? 0 : 1;
}

ساكن فراغ check_write(ثابت بنية git_config_منبع *منبع)
{
	إذا (!منبع->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	إذا (منبع->use_stdin)
		die(_("writing to stdin is not supported"));

	إذا (منبع->blob)
		die(_("writing config blobs is not supported"));
}

بنية urlmatch_current_candidate_مقدار {
	حرف مقدار_is_null;
	بنية strbuf مقدار;
	بنية کلید_مقدار_info kvi;
};

ساكن صحيح urlmatch_collect_fn(ثابت حرف *var, ثابت حرف *مقدار,
			       ثابت بنية config_context *ctx,
			       فراغ *cb)
{
	بنية string_list *مقادیر = cb;
	بنية string_list_قلم *قلم = string_list_درج(مقادیر, var);
	بنية urlmatch_current_candidate_مقدار *matched = قلم->util;
	ثابت بنية کلید_مقدار_info *kvi = ctx->kvi;

	إذا (!matched) {
		matched = xmalloc(حجم(*matched));
		strbuf_init(&matched->مقدار, 0);
		قلم->util = matched;
	} وإلا {
		strbuf_reset(&matched->مقدار);
	}
	matched->kvi = *kvi;

	إذا (مقدار) {
		strbuf_addstr(&matched->مقدار, مقدار);
		matched->مقدار_is_null = 0;
	} وإلا {
		matched->مقدار_is_null = 1;
	}
	أرجع 0;
}

ساكن صحيح get_urlmatch(ثابت بنية config_location_options *opts,
			ثابت بنية config_display_options *_display_opts,
			ثابت حرف *var, ثابت حرف *url)
{
	صحيح ret;
	حرف *section;
	حرف *section_دم;
	بنية config_display_options display_opts = *_display_opts;
	بنية string_list_قلم *قلم;
	بنية urlmatch_config config = URLMATCH_CONFIG_INIT;
	بنية string_list مقادیر = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = عدم;
	config.cb = &مقادیر;

	إذا (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_دم = strchr(section, '.');
	إذا (section_دم) {
		*section_دم = '\0';
		config.کلید = section_دم + 1;
		display_opts.show_کلیدها = 0;
	} وإلا {
		config.کلید = عدم;
		display_opts.show_کلیدها = 1;
	}

	config_with_options(urlmatch_config_مدخل, &config,
			    &opts->منبع, the_repository,
			    &opts->options);

	ret = !مقادیر.nr;

	for_each_string_list_قلم(قلم, &مقادیر) {
		بنية urlmatch_current_candidate_مقدار *matched = قلم->util;
		بنية strbuf buf = STRBUF_INIT;
		صحيح وضعیت;

		وضعیت = format_config(&display_opts, &buf, قلم->string,
				       matched->مقدار_is_null ? عدم : matched->مقدار.buf,
				       &matched->kvi, 0);
		إذا (!وضعیت)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->مقدار);
	}
	urlmatch_config_release(&config);
	string_list_clear(&مقادیر, 1);
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
				  ثابت حرف *پیشوند)
{
	إذا (!opts->منبع.file)
		opts->منبع.file = opts->file_مقصد_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	إذا (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->منبع.file + !!opts->منبع.blob > 1) {
		error(_("only one config file at a time"));
		خروج(129);
	}

	إذا (!startup_info->have_repository) {
		إذا (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		إذا (opts->منبع.blob)
			die(_("--blob can only be used inside a git repository"));
		إذا (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	إذا (opts->منبع.file &&
			!strcmp(opts->منبع.file, "-")) {
		opts->منبع.file = عدم;
		opts->منبع.use_stdin = 1;
		opts->منبع.scope = CONFIG_SCOPE_COMMAND;
	}

	إذا (opts->use_global_config) {
		opts->منبع.file = opts->file_مقصد_free = git_global_config();
		إذا (!opts->منبع.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->منبع.scope = CONFIG_SCOPE_GLOBAL;
	} وإلا إذا (opts->use_system_config) {
		opts->منبع.file = opts->file_مقصد_free = git_system_config();
		opts->منبع.scope = CONFIG_SCOPE_SYSTEM;
	} وإلا إذا (opts->use_local_config) {
		opts->منبع.file = opts->file_مقصد_free = repo_git_مسیر(the_repository, "config");
		opts->منبع.scope = CONFIG_SCOPE_LOCAL;
	} وإلا إذا (opts->use_worktree_config) {
		بنية worktree **worktrees = get_worktrees(the_repository);
		إذا (the_repository->repository_format_worktree_config)
			opts->منبع.file = opts->file_مقصد_free =
				repo_git_مسیر(the_repository, "config.worktree");
		وإلا إذا (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		وإلا
			opts->منبع.file = opts->file_مقصد_free =
				repo_git_مسیر(the_repository, "config");
		opts->منبع.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} وإلا إذا (opts->منبع.file) {
		إذا (!is_absolute_مسیر(opts->منبع.file) && پیشوند)
			opts->منبع.file = opts->file_مقصد_free =
				پیشوند_filename(پیشوند, opts->منبع.file);
		opts->منبع.scope = CONFIG_SCOPE_COMMAND;
	} وإلا إذا (opts->منبع.blob) {
		opts->منبع.scope = CONFIG_SCOPE_COMMAND;
	}

	إذا (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->منبع.file;
	وإلا
		opts->options.respect_includes = opts->respect_includes_opt;
	إذا (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ساكن فراغ location_options_release(بنية config_location_options *opts)
{
	حرر(opts->file_مقصد_free);
}

ساكن فراغ display_options_init(بنية config_display_options *opts)
{
	إذا (opts->انتها_nul) {
		opts->جمله = '\0';
		opts->delim = '\n';
		opts->کلید_delim = '\n';
	}
}

ساكن فراغ display_options_init_list(بنية config_display_options *opts)
{
	opts->show_کلیدها = 1;

	إذا (opts->انتها_nul) {
		display_options_init(opts);
	} وإلا {
		opts->جمله = '\n';
		opts->delim = ' ';
		opts->کلید_delim = '=';
	}
}

ساكن صحيح cmd_config_list(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo استفادهنشده)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_گروه(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, پیشوند);
	display_options_init_list(&display_opts);

	راهاندازی_auto_pager("config", 1);

	إذا (config_with_options(show_all_config, &display_opts,
				&location_opts.منبع, the_repository,
				&location_opts.options) < 0) {
		إذا (location_opts.منبع.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.منبع.file);
		وإلا
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	أرجع 0;
}

ساكن صحيح cmd_config_get(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			  بنية repository *repo استفادهنشده)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *مقدار_الگو = عدم, *url = عدم;
	صحيح flags = 0;
	بیعلامت get_مقدار_flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_گروه(N_("Filter options")),
		OPT_بیت(0, "all", &get_مقدار_flags, N_("return all values for multi-valued config options"), GET_مقدار_ALL),
		OPT_بیت(0, "regexp", &get_مقدار_flags, N_("interpret the name as a regular expression"), GET_مقدار_کلید_REGEXP),
		OPT_STRING(0, "value", &مقدار_الگو, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_بیت(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_گروه(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_مقدار,
			   N_("value"), N_("use default value when missing entry")),
		OPT_انتها(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	إذا ((flags & CONFIG_FLAGS_FIXED_مقدار) && !مقدار_الگو)
		die(_("--fixed-value only applies with 'value-pattern'"));
	إذا (display_opts.default_مقدار &&
	    ((get_مقدار_flags & GET_مقدار_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	إذا (url && ((get_مقدار_flags & GET_مقدار_ALL) ||
		    (get_مقدار_flags & GET_مقدار_کلید_REGEXP) ||
		    مقدار_الگو))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, پیشوند);
	display_options_init(&display_opts);

	إذا (display_opts.type != TYPE_COLOR)
		راهاندازی_auto_pager("config", 1);

	إذا (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	وإلا إذا (display_opts.type == TYPE_COLOR && !طول_رشته(argv[0]) && display_opts.default_مقدار)
		ret = get_color(&location_opts, "", display_opts.default_مقدار);
	وإلا
		ret = get_مقدار(&location_opts, &display_opts, argv[0], مقدار_الگو,
				get_مقدار_flags, flags);

	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_مجموعه(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			  بنية repository *repo استفادهنشده)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *مقدار_الگو = عدم, *comment_arg = عدم;
	حرف *comment = عدم;
	صحيح flags = 0, append = 0, type = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_گروه(N_("Filter")),
		OPT_بیت(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &مقدار_الگو, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_بیت(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_گروه(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_انتها(),
	};
	بنية کلید_مقدار_info default_kvi = KVI_INIT;
	حرف *مقدار;
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, builtin_config_مجموعه_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (argc == 1)
		die_missing_مجموعه_مقدار(argv[0]);
	check_argc(argc, 2, 2);

	إذا ((flags & CONFIG_FLAGS_FIXED_مقدار) && !مقدار_الگو)
		die(_("--fixed-value only applies with --value=<pattern>"));
	إذا (append && مقدار_الگو)
		die(_("--append cannot be used with --value=<pattern>"));
	إذا (append)
		مقدار_الگو = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, پیشوند);
	check_write(&location_opts.منبع);

	مقدار = normalize_مقدار(argv[0], argv[1], type, &default_kvi);

	إذا ((flags & CONFIG_FLAGS_MULTI_REPLACE) || مقدار_الگو) {
		ret = repo_config_مجموعه_multivar_درون_file_gently(the_repository, location_opts.منبع.file,
							      argv[0], مقدار, مقدار_الگو,
							      comment, flags);
	} وإلا {
		ret = repo_config_مجموعه_درون_file_gently(the_repository, location_opts.منبع.file,
						     argv[0], comment, مقدار);
		إذا (ret == CONFIG_NOTHING_مجموعه)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	حرر(comment);
	حرر(مقدار);
	أرجع ret;
}

ساكن صحيح cmd_config_unset(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			    بنية repository *repo استفادهنشده)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *مقدار_الگو = عدم;
	صحيح flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_گروه(N_("Filter")),
		OPT_بیت(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &مقدار_الگو, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_بیت(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_انتها(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	إذا ((flags & CONFIG_FLAGS_FIXED_مقدار) && !مقدار_الگو)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, پیشوند);
	check_write(&location_opts.منبع);

	إذا ((flags & CONFIG_FLAGS_MULTI_REPLACE) || مقدار_الگو)
		ret = repo_config_مجموعه_multivar_درون_file_gently(the_repository, location_opts.منبع.file,
							      argv[0], عدم, مقدار_الگو,
							      عدم, flags);
	وإلا
		ret = repo_config_مجموعه_درون_file_gently(the_repository, location_opts.منبع.file, argv[0],
						     عدم, عدم);

	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_rename_section(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				     بنية repository *repo استفادهنشده)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_انتها(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, پیشوند);
	check_write(&location_opts.منبع);

	ret = repo_config_rename_section_درون_file(the_repository, location_opts.منبع.file,
						 argv[0], argv[1]);
	إذا (ret < 0)
		اذهب بیرون;
	وإلا إذا (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

بیرون:
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_remove_section(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
				     بنية repository *repo استفادهنشده)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_انتها(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, پیشوند);
	check_write(&location_opts.منبع);

	ret = repo_config_rename_section_درون_file(the_repository, location_opts.منبع.file,
						 argv[0], عدم);
	إذا (ret < 0)
		اذهب بیرون;
	وإلا إذا (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

بیرون:
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح show_editor(بنية config_location_options *opts)
{
	حرف *config_file;

	إذا (!opts->منبع.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	إذا (opts->منبع.use_stdin)
		die(_("editing stdin is not supported"));
	إذا (opts->منبع.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, عدم);
	config_file = opts->منبع.file ?
			xstrdup(opts->منبع.file) :
			repo_git_مسیر(the_repository, "config");
	إذا (opts->use_global_config) {
		صحيح fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		إذا (fd >= 0) {
			حرف *content = default_user_config();
			write_str_درون_پر(fd, content);
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

ساكن صحيح cmd_config_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			   بنية repository *repo استفادهنشده)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_انتها(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, پیشوند);
	check_write(&location_opts.منبع);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_actions(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند)
{
	تعداد {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_جمع_کن = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_مجموعه = (1<<11),
		ACTION_مجموعه_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *comment_arg = عدم;
	صحيح actions = 0;
	بیعلامت flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_گروه(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_جمع_کن),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_گروه(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_مقدار,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_بیت(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_مقدار),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_انتها(),
	};
	حرف *مقدار = عدم, *comment = عدم;
	صحيح ret = 0;
	صحيح actions_implicit;
	بنية کلید_مقدار_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, پیشوند, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, پیشوند);
	display_options_init(&display_opts);

	إذا ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		خروج(129);
	}

	actions_implicit = (actions == 0);
	إذا (actions_implicit)
		بدل (argc) {
		حالة 1: actions = ACTION_GET; اكسر;
		حالة 2: actions = ACTION_مجموعه; اكسر;
		حالة 3: actions = ACTION_مجموعه_ALL; اكسر;
		افتراضي:
			error(_("no action specified"));
			خروج(129);
		}
	إذا (actions_implicit && argc == 1) {
		ثابت حرف *آخرین_dot = strrchr(argv[0], '.');
		إذا (آخرین_dot && strchr(آخرین_dot + 1, '='))
			die_missing_مجموعه_مقدار(argv[0]);
	}
	إذا (display_opts.omit_مقادیر &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		خروج(129);
	}

	إذا (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		خروج(129);
	}

	إذا (display_opts.default_مقدار && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		خروج(129);
	}

	إذا (comment_arg &&
	    !(actions & (ACTION_جمع_کن|ACTION_مجموعه|ACTION_مجموعه_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		خروج(129);
	}

	/* check usage of --fixed-value */
	إذا (flags & CONFIG_FLAGS_FIXED_مقدار) {
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
		حالة ACTION_مجموعه_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		حالة ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			اكسر;

		/* other options don't allow --fixed-value */
		}

		إذا (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			خروج(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	إذا (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		راهاندازی_auto_pager("config", 1);

	إذا (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		إذا (config_with_options(show_all_config, &display_opts,
					&location_opts.منبع, the_repository,
					&location_opts.options) < 0) {
			إذا (location_opts.منبع.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.منبع.file);
			وإلا
				die(_("error processing config file(s)"));
		}
	}
	وإلا إذا (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	وإلا إذا (actions == ACTION_مجموعه) {
		check_write(&location_opts.منبع);
		check_argc(argc, 2, 2);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_مجموعه_درون_file_gently(the_repository, location_opts.منبع.file, argv[0], comment, مقدار);
		إذا (ret == CONFIG_NOTHING_مجموعه)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	وإلا إذا (actions == ACTION_مجموعه_ALL) {
		check_write(&location_opts.منبع);
		check_argc(argc, 2, 3);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_مجموعه_multivar_درون_file_gently(the_repository, location_opts.منبع.file,
							      argv[0], مقدار, argv[2],
							      comment, flags);
	}
	وإلا إذا (actions == ACTION_جمع_کن) {
		check_write(&location_opts.منبع);
		check_argc(argc, 2, 2);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_مجموعه_multivar_درون_file_gently(the_repository, location_opts.منبع.file,
							      argv[0], مقدار,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	وإلا إذا (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.منبع);
		check_argc(argc, 2, 3);
		مقدار = normalize_مقدار(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_مجموعه_multivar_درون_file_gently(the_repository, location_opts.منبع.file,
							      argv[0], مقدار, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وإلا إذا (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_مقدار(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	وإلا إذا (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_مقدار(&location_opts, &display_opts, argv[0], argv[1],
				GET_مقدار_ALL, flags);
	}
	وإلا إذا (actions == ACTION_GET_REGEXP) {
		display_opts.show_کلیدها = 1;
		check_argc(argc, 1, 2);
		ret = get_مقدار(&location_opts, &display_opts, argv[0], argv[1],
				GET_مقدار_ALL|GET_مقدار_کلید_REGEXP, flags);
	}
	وإلا إذا (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	وإلا إذا (actions == ACTION_UNSET) {
		check_write(&location_opts.منبع);
		check_argc(argc, 1, 2);
		إذا (argc == 2)
			ret = repo_config_مجموعه_multivar_درون_file_gently(the_repository, location_opts.منبع.file,
								      argv[0], عدم, argv[1],
								      عدم, flags);
		وإلا
			ret = repo_config_مجموعه_درون_file_gently(the_repository, location_opts.منبع.file,
							     argv[0], عدم, عدم);
	}
	وإلا إذا (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.منبع);
		check_argc(argc, 1, 2);
		ret = repo_config_مجموعه_multivar_درون_file_gently(the_repository, location_opts.منبع.file,
							      argv[0], عدم, argv[1],
							      عدم, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وإلا إذا (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.منبع);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_درون_file(the_repository, location_opts.منبع.file,
							 argv[0], argv[1]);
		إذا (ret < 0)
			اذهب بیرون;
		وإلا إذا (!ret)
			die(_("no such section: %s"), argv[0]);
		وإلا
			ret = 0;
	}
	وإلا إذا (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.منبع);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_درون_file(the_repository, location_opts.منبع.file,
							 argv[0], عدم);
		إذا (ret < 0)
			اذهب بیرون;
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

بیرون:
	location_options_release(&location_opts);
	حرر(comment);
	حرر(مقدار);
	أرجع ret;
}

صحيح cmd_config(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *پیشوند,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *subcommand = عدم;
	بنية option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_مجموعه),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_انتها(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, پیشوند, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	إذا (subcommand) {
		argc = parse_options(argc, argv, پیشوند, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		أرجع subcommand(argc, argv, پیشوند, repo);
	}

	أرجع cmd_config_actions(argc, argv, پیشوند);
}
