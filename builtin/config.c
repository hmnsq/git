#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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

ساكن ثابت حرف *ثابت builtin_config_سېټ_usage[] = {
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

#تعریف_کړه CONFIG_LOCATION_OPTIONS(opts) \
	OPT_ډله(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.سرچینه.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.سرچینه.blob, N_("blob-id"), N_("read config from given blob object"))

بنية config_location_options {
	بنية git_config_سرچینه سرچینه;
	بنية config_options options;
	حرف *file_منزل_free;
	صحيح use_global_config;
	صحيح use_system_config;
	صحيح use_local_config;
	صحيح use_worktree_config;
	صحيح respect_includes_opt;
};
#تعریف_کړه CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

تعداد config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_لار,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#تعریف_کړه CONFIG_TYPE_OPTIONS(type) \
	OPT_ډله(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_ارزښت(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_ارزښت(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_ارزښت(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_ارزښت(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_ارزښت(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_لار), \
	OPT_CALLBACK_ارزښت(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#تعریف_کړه CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_ډله(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.پای_ټکی_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_ارزښتونه, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_کلیګانې, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

بنية config_display_options {
	صحيح پای_ټکی_nul;
	صحيح omit_ارزښتونه;
	صحيح show_origin;
	صحيح show_scope;
	صحيح show_کلیګانې;
	تعداد config_type type;
	حرف *default_ارزښت;
	/* Populated via `display_options_init()`. */
	صحيح غړی;
	صحيح delim;
	صحيح کلی_delim;
};
#تعریف_کړه CONFIG_DISPLAY_OPTIONS_INIT { \
	.غړی = '\n', \
	.delim = '=', \
	.کلی_delim = ' ', \
	.type = TYPE_NONE, \
}

#تعریف_کړه OPT_CALLBACK_ارزښت(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.ارزښت = (v), \
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
		*((صحيح *) opt->ارزښت) = 0;
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
			new_type = TYPE_لار;
		وإلا إذا (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		وإلا إذا (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		وإلا
			die(_("unrecognized --type argument, %s"), arg);
	}

	منزل_type = opt->ارزښت;
	إذا (*منزل_type && *منزل_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		وتل(129);
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
	وتل(129);
}

ساكن NORETURN فراغ die_missing_سېټ_ارزښت(ثابت حرف *arg)
{
	ثابت حرف *وروستی_dot = strrchr(arg, '.');
	ثابت حرف *eq = وروستی_dot ? strchr(وروستی_dot + 1, '=') : عدم;
	حرف *مختاړی = eq ? xstrndup(arg, eq - arg) : عدم;

	إذا (مختاړی && git_config_کلی_is_valid(مختاړی)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       مختاړی, eq + 1);
	} وإلا إذا (git_config_کلی_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} وإلا {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	حرر(مختاړی);
	وتل(129);
}

ساكن فراغ show_config_origin(ثابت بنية config_display_options *opts,
			       ثابت بنية کلی_ارزښت_info *kvi,
			       بنية strbuf *buf)
{
	ثابت حرف غړی = opts->پای_ټکی_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	إذا (opts->پای_ټکی_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	وإلا
		quote_c_style(kvi->filename ? kvi->filename : "", buf, عدم, 0);
	strbuf_addch(buf, غړی);
}

ساكن فراغ show_config_scope(ثابت بنية config_display_options *opts,
			      ثابت بنية کلی_ارزښت_info *kvi,
			      بنية strbuf *buf)
{
	ثابت حرف غړی = opts->پای_ټکی_nul ? '\0' : '\t';
	ثابت حرف *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, غړی);
}

بنية strbuf_list {
	بنية strbuf *توکي;
	صحيح nr;
	صحيح alloc;
};

ساكن صحيح format_config_int64(بنية strbuf *buf,
			       ثابت حرف *کلی_2,
			       ثابت حرف *ارزښت_2,
			       ثابت بنية کلی_ارزښت_info *kvi,
			       صحيح gently)
{
	int64_t v = 0;
	إذا (gently) {
		إذا (!git_parse_int64(ارزښت_2, &v))
			أرجع -1;
	} وإلا {
		/* may die() */
		v = git_config_int64(کلی_2, ارزښت_2 ? ارزښت_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	أرجع 0;
}

ساكن صحيح format_config_bool(بنية strbuf *buf,
			      ثابت حرف *کلی_2,
			      ثابت حرف *ارزښت_2,
			      صحيح gently)
{
	صحيح v = 0;
	إذا (gently) {
		إذا ((v = git_parse_maybe_bool(ارزښت_2)) < 0)
			أرجع -1;
	} وإلا {
		/* may die() */
		v = git_config_bool(کلی_2, ارزښت_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	أرجع 0;
}

ساكن صحيح format_config_bool_or_int(بنية strbuf *buf,
				     ثابت حرف *کلی_2,
				     ثابت حرف *ارزښت_2,
				     ثابت بنية کلی_ارزښت_info *kvi,
				     صحيح gently)
{
	صحيح v, is_bool = 0;

	إذا (gently) {
		v = git_parse_maybe_bool_متن(ارزښت_2);

		إذا (v >= 0)
			is_bool = 1;
		وإلا إذا (!git_parse_int(ارزښت_2, &v))
			أرجع -1;
	} وإلا {
		v = git_config_bool_or_int(کلی_2, ارزښت_2, kvi,
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
				     ثابت حرف *ارزښت_2)
{
	صحيح v = git_parse_maybe_bool(ارزښت_2);
	إذا (v < 0)
		strbuf_addstr(buf, ارزښت_2);
	وإلا
		strbuf_addstr(buf, v ? "true" : "false");
	أرجع 0;
}

ساكن صحيح format_config_لار(بنية strbuf *buf,
			      ثابت حرف *کلی_2,
			      ثابت حرف *ارزښت_2,
			      صحيح gently)
{
	حرف *v;

	إذا (git_config_pathname(&v, کلی_2, ارزښت_2) < 0)
		أرجع -1;

	إذا (v)
		strbuf_addstr(buf, v);
	وإلا
		أرجع gently ? -1 : 1; /* :(optional)no-such-file */

	حرر(v);
	أرجع 0;
}

ساكن صحيح format_config_expiry_date(بنية strbuf *buf,
				     ثابت حرف *کلی_2,
				     ثابت حرف *ارزښت_2,
				     صحيح quietly)
{
	timestamp_t t;
	إذا (quietly) {
		إذا (parse_expiry_date(ارزښت_2, &t))
			أرجع -1;
	} وإلا إذا (git_config_expiry_date(&t, کلی_2, ارزښت_2) < 0) {
		أرجع -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	أرجع 0;
}

ساكن صحيح format_config_color(بنية strbuf *buf,
			       ثابت حرف *کلی_2,
			       ثابت حرف *ارزښت_2,
			       صحيح gently)
{
	حرف v[COLOR_MAXLEN];

	إذا (gently) {
		إذا (color_parse_quietly(ارزښت_2, v) < 0)
			أرجع -1;
	} وإلا إذا (git_config_color(v, کلی_2, ارزښت_2) < 0) {
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
			 بنية strbuf *buf, ثابت حرف *کلی_2,
			 ثابت حرف *ارزښت_2, ثابت بنية کلی_ارزښت_info *kvi,
			 صحيح gently)
{
	صحيح res = 0;
	إذا (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	إذا (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	إذا (opts->show_کلیګانې)
		strbuf_addstr(buf, کلی_2);

	إذا (opts->omit_ارزښتونه)
		اذهب terminator;

	إذا (opts->show_کلیګانې)
		strbuf_addch(buf, opts->کلی_delim);

	بدل (opts->type) {
	حالة TYPE_INT:
		res = format_config_int64(buf, کلی_2, ارزښت_2, kvi, gently);
		اكسر;

	حالة TYPE_BOOL:
		res = format_config_bool(buf, کلی_2, ارزښت_2, gently);
		اكسر;

	حالة TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, کلی_2, ارزښت_2, kvi, gently);
		اكسر;

	حالة TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, ارزښت_2);
		اكسر;

	حالة TYPE_لار:
		res = format_config_لار(buf, کلی_2, ارزښت_2, gently);
		اكسر;

	حالة TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, کلی_2, ارزښت_2, gently);
		اكسر;

	حالة TYPE_COLOR:
		res = format_config_color(buf, کلی_2, ارزښت_2, gently);
		اكسر;

	حالة TYPE_NONE:
		إذا (ارزښت_2) {
			strbuf_addstr(buf, ارزښت_2);
		} وإلا {
			/* Just show the key name; back out delimiter */
			إذا (opts->show_کلیګانې)
				strbuf_setlen(buf, buf->len - 1);
		}
		اكسر;

	افتراضي:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->غړی);
	أرجع res;
}

ساكن صحيح show_all_config(ثابت حرف *کلی_2, ثابت حرف *ارزښت_2,
			   ثابت بنية config_context *ctx,
			   فراغ *cb)
{
	ثابت بنية config_display_options *opts = cb;
	ثابت بنية کلی_ارزښت_info *kvi = ctx->kvi;
	بنية strbuf formatted = STRBUF_INIT;

	إذا (format_config(opts, &formatted, کلی_2, ارزښت_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	أرجع 0;
}

#تعریف_کړه GET_ارزښت_ALL        (1 << 0)
#تعریف_کړه GET_ارزښت_کلی_REGEXP (1 << 1)

بنية collect_config_معلومات {
	ثابت بنية config_display_options *display_opts;
	بنية strbuf_list *ارزښتونه;
	ثابت حرف *ارزښت_نمونه;
	ثابت حرف *کلی;
	regex_t *regexp;
	regex_t *کلی_regexp;
	صحيح do_not_سمون;
	بینښه get_ارزښت_flags;
	بینښه flags;
};

ساكن صحيح collect_config(ثابت حرف *کلی_2, ثابت حرف *ارزښت_2,
			  ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية collect_config_معلومات *معلومات = cb;
	بنية strbuf_list *ارزښتونه = معلومات->ارزښتونه;
	ثابت بنية کلی_ارزښت_info *kvi = ctx->kvi;
	صحيح حالت;

	إذا (!(معلومات->get_ارزښت_flags & GET_ارزښت_کلی_REGEXP) &&
	    strcmp(کلی_2, معلومات->کلی))
		أرجع 0;
	إذا ((معلومات->get_ارزښت_flags & GET_ارزښت_کلی_REGEXP) &&
	    regexec(معلومات->کلی_regexp, کلی_2, 0, عدم, 0))
		أرجع 0;
	إذا ((معلومات->flags & CONFIG_FLAGS_FIXED_ارزښت) &&
	    strcmp(معلومات->ارزښت_نمونه, (ارزښت_2?ارزښت_2:"")))
		أرجع 0;
	إذا (معلومات->regexp &&
	    (معلومات->do_not_سمون ^ !!regexec(معلومات->regexp, (ارزښت_2?ارزښت_2:""), 0, عدم, 0)))
		أرجع 0;

	ALLOC_GROW(ارزښتونه->توکي, ارزښتونه->nr + 1, ارزښتونه->alloc);
	strbuf_init(&ارزښتونه->توکي[ارزښتونه->nr], 0);

	حالت = format_config(معلومات->display_opts, &ارزښتونه->توکي[ارزښتونه->nr++],
			       کلی_2, ارزښت_2, kvi, 0);
	إذا (حالت < 0)
		أرجع حالت;
	إذا (حالت) {
		strbuf_release(&ارزښتونه->توکي[--ارزښتونه->nr]);
		حالت = 0;
	}
	أرجع حالت;
}

ساكن صحيح get_ارزښت(ثابت بنية config_location_options *opts,
		     ثابت بنية config_display_options *display_opts,
		     ثابت حرف *کلی_2, ثابت حرف *regex_,
		     بینښه get_ارزښت_flags, بینښه flags)
{
	صحيح ret = CONFIG_GENERIC_ERROR;
	بنية strbuf_list ارزښتونه = {عدم};
	بنية collect_config_معلومات معلومات = {
		.display_opts = display_opts,
		.ارزښتونه = &ارزښتونه,
		.get_ارزښت_flags = get_ارزښت_flags,
		.flags = flags,
	};
	حرف *کلی = عدم;
	صحيح i;

	إذا (get_ارزښت_flags & GET_ارزښت_کلی_REGEXP) {
		حرف *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		کلی = xstrdup(کلی_2);
		لكل (tl = کلی + مزي_اوږدوالی(کلی) - 1;
		     tl >= کلی && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		لكل (tl = کلی; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		معلومات.کلی_regexp = (regex_t*)xmalloc(حجم(regex_t));
		إذا (regcomp(معلومات.کلی_regexp, کلی, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), کلی_2);
			FREE_AND_NULL(معلومات.کلی_regexp);
			ret = CONFIG_INVALID_نمونه;
			اذهب free_مزي_ګانې;
		}
	} وإلا {
		إذا (git_config_parse_کلی(کلی_2, &کلی, عدم)) {
			ret = CONFIG_INVALID_کلی;
			اذهب free_مزي_ګانې;
		}

		معلومات.کلی = کلی;
	}

	إذا (regex_ && (flags & CONFIG_FLAGS_FIXED_ارزښت))
		معلومات.ارزښت_نمونه = regex_;
	وإلا إذا (regex_) {
		إذا (regex_[0] == '!') {
			معلومات.do_not_سمون = 1;
			regex_++;
		}

		معلومات.regexp = (regex_t*)xmalloc(حجم(regex_t));
		إذا (regcomp(معلومات.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(معلومات.regexp);
			ret = CONFIG_INVALID_نمونه;
			اذهب free_مزي_ګانې;
		}
	}

	config_with_options(collect_config, &معلومات,
			    &opts->سرچینه, the_repository,
			    &opts->options);

	إذا (!ارزښتونه.nr && display_opts->default_ارزښت) {
		بنية کلی_ارزښت_info kvi = KVI_INIT;
		بنية strbuf *توکی;
		صحيح حالت;

		kvi_from_param(&kvi);
		ALLOC_GROW(ارزښتونه.توکي, ارزښتونه.nr + 1, ارزښتونه.alloc);
		توکی = &ارزښتونه.توکي[ارزښتونه.nr++];
		strbuf_init(توکی, 0);

		حالت = format_config(display_opts, توکی, کلی_2,
				       display_opts->default_ارزښت, &kvi, 0);
		إذا (حالت < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_ارزښت);
		إذا (حالت) {
			/* default was a missing optional value */
			ارزښتونه.nr--;
			strbuf_release(توکی);
		}
	}

	ret = !ارزښتونه.nr;

	لكل (i = 0; i < ارزښتونه.nr; i++) {
		بنية strbuf *buf = ارزښتونه.توکي + i;
		إذا ((get_ارزښت_flags & GET_ارزښت_ALL) || i == ارزښتونه.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	حرر(ارزښتونه.توکي);

free_مزي_ګانې:
	حرر(کلی);
	إذا (معلومات.کلی_regexp) {
		regfree(معلومات.کلی_regexp);
		حرر(معلومات.کلی_regexp);
	}
	إذا (معلومات.regexp) {
		regfree(معلومات.regexp);
		حرر(معلومات.regexp);
	}

	أرجع ret;
}

ساكن حرف *normalize_ارزښت(ثابت حرف *کلی, ثابت حرف *ارزښت,
			     صحيح type, بنية کلی_ارزښت_info *kvi)
{
	إذا (!ارزښت)
		أرجع عدم;

	إذا (type == 0 || type == TYPE_لار || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		أرجع xstrdup(ارزښت);
	إذا (type == TYPE_INT)
		أرجع xstrfmt("%"PRId64, git_config_int64(کلی, ارزښت, kvi));
	إذا (type == TYPE_BOOL)
		أرجع xstrdup(git_config_bool(کلی, ارزښت) ?  "true" : "false");
	إذا (type == TYPE_BOOL_OR_INT) {
		صحيح is_bool, v;
		v = git_config_bool_or_int(کلی, ارزښت, kvi, &is_bool);
		إذا (!is_bool)
			أرجع xstrfmt("%d", v);
		وإلا
			أرجع xstrdup(v ? "true" : "false");
	}
	إذا (type == TYPE_BOOL_OR_STR) {
		صحيح v = git_parse_maybe_bool(ارزښت);
		إذا (v < 0)
			أرجع xstrdup(ارزښت);
		وإلا
			أرجع xstrdup(v ? "true" : "false");
	}
	إذا (type == TYPE_COLOR) {
		حرف v[COLOR_MAXLEN];
		إذا (git_config_color(v, کلی, ارزښت))
			die(_("cannot parse color '%s'"), ارزښت);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		أرجع xstrdup(ارزښت);
	}

	BUG("cannot normalize type %d", type);
}

بنية get_color_config_معلومات {
	صحيح get_color_وموندل;
	ثابت حرف *get_color_خونه;
	حرف parsed_color[COLOR_MAXLEN];
};

ساكن صحيح git_get_color_config(ثابت حرف *var, ثابت حرف *ارزښت,
				ثابت بنية config_context *ctx ناکارول_شوی,
				فراغ *cb)
{
	بنية get_color_config_معلومات *معلومات = cb;

	إذا (!strcmp(var, معلومات->get_color_خونه)) {
		إذا (!ارزښت)
			config_error_nonbool(var);
		إذا (color_parse(ارزښت, معلومات->parsed_color) < 0)
			أرجع -1;
		معلومات->get_color_وموندل = 1;
	}
	أرجع 0;
}

ساكن صحيح get_color(ثابت بنية config_location_options *opts,
		      ثابت حرف *var, ثابت حرف *def_color)
{
	بنية get_color_config_معلومات معلومات = {
		.get_color_خونه = var,
		.parsed_color[0] = '\0',
	};
	صحيح ret;

	config_with_options(git_get_color_config, &معلومات,
			    &opts->سرچینه, the_repository,
			    &opts->options);

	إذا (!معلومات.get_color_وموندل && def_color) {
		إذا (color_parse(def_color, معلومات.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			اذهب بهر;
		}
	}

	ret = 0;

بهر:
	fputs(معلومات.parsed_color, stdout);
	أرجع ret;
}

بنية get_colorbool_config_معلومات {
	تعداد git_colorbool get_colorbool_وموندل;
	تعداد git_colorbool get_diff_color_وموندل;
	تعداد git_colorbool get_color_ui_وموندل;
	ثابت حرف *get_colorbool_خونه;
};

ساكن صحيح git_get_colorbool_config(ثابت حرف *var, ثابت حرف *ارزښت,
				    ثابت بنية config_context *ctx ناکارول_شوی,
				    فراغ *cb)
{
	بنية get_colorbool_config_معلومات *معلومات = cb;

	إذا (!strcmp(var, معلومات->get_colorbool_خونه))
		معلومات->get_colorbool_وموندل = git_config_colorbool(var, ارزښت);
	وإلا إذا (!strcmp(var, "diff.color"))
		معلومات->get_diff_color_وموندل = git_config_colorbool(var, ارزښت);
	وإلا إذا (!strcmp(var, "color.ui"))
		معلومات->get_color_ui_وموندل = git_config_colorbool(var, ارزښت);
	أرجع 0;
}

ساكن صحيح get_colorbool(ثابت بنية config_location_options *opts,
			 ثابت حرف *var, صحيح print)
{
	بنية get_colorbool_config_معلومات معلومات = {
		.get_colorbool_خونه = var,
		.get_colorbool_وموندل = GIT_COLOR_UNKNOWN,
		.get_diff_color_وموندل = GIT_COLOR_UNKNOWN,
		.get_color_ui_وموندل = GIT_COLOR_UNKNOWN,
	};
	bool پایله;

	config_with_options(git_get_colorbool_config, &معلومات,
			    &opts->سرچینه, the_repository,
			    &opts->options);

	إذا (معلومات.get_colorbool_وموندل == GIT_COLOR_UNKNOWN) {
		إذا (!strcmp(معلومات.get_colorbool_خونه, "color.diff"))
			معلومات.get_colorbool_وموندل = معلومات.get_diff_color_وموندل;
		إذا (معلومات.get_colorbool_وموندل == GIT_COLOR_UNKNOWN)
			معلومات.get_colorbool_وموندل = معلومات.get_color_ui_وموندل;
	}

	إذا (معلومات.get_colorbool_وموندل == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		معلومات.get_colorbool_وموندل = GIT_COLOR_AUTO;

	پایله = want_color(معلومات.get_colorbool_وموندل);

	إذا (print) {
		چاپ_کړه("%s\n", پایله ? "true" : "false");
		أرجع 0;
	} وإلا
		أرجع پایله ? 0 : 1;
}

ساكن فراغ check_write(ثابت بنية git_config_سرچینه *سرچینه)
{
	إذا (!سرچینه->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	إذا (سرچینه->use_stdin)
		die(_("writing to stdin is not supported"));

	إذا (سرچینه->blob)
		die(_("writing config blobs is not supported"));
}

بنية urlmatch_current_candidate_ارزښت {
	حرف ارزښت_is_null;
	بنية strbuf ارزښت;
	بنية کلی_ارزښت_info kvi;
};

ساكن صحيح urlmatch_collect_fn(ثابت حرف *var, ثابت حرف *ارزښت,
			       ثابت بنية config_context *ctx,
			       فراغ *cb)
{
	بنية string_list *ارزښتونه = cb;
	بنية string_list_توکی *توکی = string_list_ورننباسه(ارزښتونه, var);
	بنية urlmatch_current_candidate_ارزښت *matched = توکی->util;
	ثابت بنية کلی_ارزښت_info *kvi = ctx->kvi;

	إذا (!matched) {
		matched = xmalloc(حجم(*matched));
		strbuf_init(&matched->ارزښت, 0);
		توکی->util = matched;
	} وإلا {
		strbuf_reset(&matched->ارزښت);
	}
	matched->kvi = *kvi;

	إذا (ارزښت) {
		strbuf_addstr(&matched->ارزښت, ارزښت);
		matched->ارزښت_is_null = 0;
	} وإلا {
		matched->ارزښت_is_null = 1;
	}
	أرجع 0;
}

ساكن صحيح get_urlmatch(ثابت بنية config_location_options *opts,
			ثابت بنية config_display_options *_display_opts,
			ثابت حرف *var, ثابت حرف *url)
{
	صحيح ret;
	حرف *section;
	حرف *section_لکۍ;
	بنية config_display_options display_opts = *_display_opts;
	بنية string_list_توکی *توکی;
	بنية urlmatch_config config = URLMATCH_CONFIG_INIT;
	بنية string_list ارزښتونه = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = عدم;
	config.cb = &ارزښتونه;

	إذا (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_لکۍ = strchr(section, '.');
	إذا (section_لکۍ) {
		*section_لکۍ = '\0';
		config.کلی = section_لکۍ + 1;
		display_opts.show_کلیګانې = 0;
	} وإلا {
		config.کلی = عدم;
		display_opts.show_کلیګانې = 1;
	}

	config_with_options(urlmatch_config_ثبت, &config,
			    &opts->سرچینه, the_repository,
			    &opts->options);

	ret = !ارزښتونه.nr;

	for_each_string_list_توکی(توکی, &ارزښتونه) {
		بنية urlmatch_current_candidate_ارزښت *matched = توکی->util;
		بنية strbuf buf = STRBUF_INIT;
		صحيح حالت;

		حالت = format_config(&display_opts, &buf, توکی->string,
				       matched->ارزښت_is_null ? عدم : matched->ارزښت.buf,
				       &matched->kvi, 0);
		إذا (!حالت)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->ارزښت);
	}
	urlmatch_config_release(&config);
	string_list_clear(&ارزښتونه, 1);
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
				  ثابت حرف *مختاړی)
{
	إذا (!opts->سرچینه.file)
		opts->سرچینه.file = opts->file_منزل_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	إذا (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->سرچینه.file + !!opts->سرچینه.blob > 1) {
		error(_("only one config file at a time"));
		وتل(129);
	}

	إذا (!startup_info->have_repository) {
		إذا (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		إذا (opts->سرچینه.blob)
			die(_("--blob can only be used inside a git repository"));
		إذا (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	إذا (opts->سرچینه.file &&
			!strcmp(opts->سرچینه.file, "-")) {
		opts->سرچینه.file = عدم;
		opts->سرچینه.use_stdin = 1;
		opts->سرچینه.scope = CONFIG_SCOPE_COMMAND;
	}

	إذا (opts->use_global_config) {
		opts->سرچینه.file = opts->file_منزل_free = git_global_config();
		إذا (!opts->سرچینه.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->سرچینه.scope = CONFIG_SCOPE_GLOBAL;
	} وإلا إذا (opts->use_system_config) {
		opts->سرچینه.file = opts->file_منزل_free = git_system_config();
		opts->سرچینه.scope = CONFIG_SCOPE_SYSTEM;
	} وإلا إذا (opts->use_local_config) {
		opts->سرچینه.file = opts->file_منزل_free = repo_git_لار(the_repository, "config");
		opts->سرچینه.scope = CONFIG_SCOPE_LOCAL;
	} وإلا إذا (opts->use_worktree_config) {
		بنية worktree **worktrees = get_worktrees(the_repository);
		إذا (the_repository->repository_format_worktree_config)
			opts->سرچینه.file = opts->file_منزل_free =
				repo_git_لار(the_repository, "config.worktree");
		وإلا إذا (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		وإلا
			opts->سرچینه.file = opts->file_منزل_free =
				repo_git_لار(the_repository, "config");
		opts->سرچینه.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} وإلا إذا (opts->سرچینه.file) {
		إذا (!is_absolute_لار(opts->سرچینه.file) && مختاړی)
			opts->سرچینه.file = opts->file_منزل_free =
				مختاړی_filename(مختاړی, opts->سرچینه.file);
		opts->سرچینه.scope = CONFIG_SCOPE_COMMAND;
	} وإلا إذا (opts->سرچینه.blob) {
		opts->سرچینه.scope = CONFIG_SCOPE_COMMAND;
	}

	إذا (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->سرچینه.file;
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
	إذا (opts->پای_ټکی_nul) {
		opts->غړی = '\0';
		opts->delim = '\n';
		opts->کلی_delim = '\n';
	}
}

ساكن فراغ display_options_init_list(بنية config_display_options *opts)
{
	opts->show_کلیګانې = 1;

	إذا (opts->پای_ټکی_nul) {
		display_options_init(opts);
	} وإلا {
		opts->غړی = '\n';
		opts->delim = ' ';
		opts->کلی_delim = '=';
	}
}

ساكن صحيح cmd_config_list(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			   بنية repository *repo ناکارول_شوی)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ډله(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_پای_ټکی(),
	};

	argc = parse_options(argc, argv, مختاړی, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, مختاړی);
	display_options_init_list(&display_opts);

	تیاری_auto_pager("config", 1);

	إذا (config_with_options(show_all_config, &display_opts,
				&location_opts.سرچینه, the_repository,
				&location_opts.options) < 0) {
		إذا (location_opts.سرچینه.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.سرچینه.file);
		وإلا
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	أرجع 0;
}

ساكن صحيح cmd_config_get(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			  بنية repository *repo ناکارول_شوی)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *ارزښت_نمونه = عدم, *url = عدم;
	صحيح flags = 0;
	بینښه get_ارزښت_flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ډله(N_("Filter options")),
		OPT_بټ(0, "all", &get_ارزښت_flags, N_("return all values for multi-valued config options"), GET_ارزښت_ALL),
		OPT_بټ(0, "regexp", &get_ارزښت_flags, N_("interpret the name as a regular expression"), GET_ارزښت_کلی_REGEXP),
		OPT_STRING(0, "value", &ارزښت_نمونه, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_بټ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ارزښت),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ډله(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_ارزښت,
			   N_("value"), N_("use default value when missing entry")),
		OPT_پای_ټکی(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	إذا ((flags & CONFIG_FLAGS_FIXED_ارزښت) && !ارزښت_نمونه)
		die(_("--fixed-value only applies with 'value-pattern'"));
	إذا (display_opts.default_ارزښت &&
	    ((get_ارزښت_flags & GET_ارزښت_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	إذا (url && ((get_ارزښت_flags & GET_ارزښت_ALL) ||
		    (get_ارزښت_flags & GET_ارزښت_کلی_REGEXP) ||
		    ارزښت_نمونه))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, مختاړی);
	display_options_init(&display_opts);

	إذا (display_opts.type != TYPE_COLOR)
		تیاری_auto_pager("config", 1);

	إذا (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	وإلا إذا (display_opts.type == TYPE_COLOR && !مزي_اوږدوالی(argv[0]) && display_opts.default_ارزښت)
		ret = get_color(&location_opts, "", display_opts.default_ارزښت);
	وإلا
		ret = get_ارزښت(&location_opts, &display_opts, argv[0], ارزښت_نمونه,
				get_ارزښت_flags, flags);

	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_سېټ(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			  بنية repository *repo ناکارول_شوی)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *ارزښت_نمونه = عدم, *comment_arg = عدم;
	حرف *comment = عدم;
	صحيح flags = 0, append = 0, type = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_ډله(N_("Filter")),
		OPT_بټ(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ارزښت_نمونه, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_بټ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ارزښت),
		OPT_ډله(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_پای_ټکی(),
	};
	بنية کلی_ارزښت_info default_kvi = KVI_INIT;
	حرف *ارزښت;
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, opts, builtin_config_سېټ_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (argc == 1)
		die_missing_سېټ_ارزښت(argv[0]);
	check_argc(argc, 2, 2);

	إذا ((flags & CONFIG_FLAGS_FIXED_ارزښت) && !ارزښت_نمونه)
		die(_("--fixed-value only applies with --value=<pattern>"));
	إذا (append && ارزښت_نمونه)
		die(_("--append cannot be used with --value=<pattern>"));
	إذا (append)
		ارزښت_نمونه = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, مختاړی);
	check_write(&location_opts.سرچینه);

	ارزښت = normalize_ارزښت(argv[0], argv[1], type, &default_kvi);

	إذا ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ارزښت_نمونه) {
		ret = repo_config_سېټ_multivar_دننه_file_gently(the_repository, location_opts.سرچینه.file,
							      argv[0], ارزښت, ارزښت_نمونه,
							      comment, flags);
	} وإلا {
		ret = repo_config_سېټ_دننه_file_gently(the_repository, location_opts.سرچینه.file,
						     argv[0], comment, ارزښت);
		إذا (ret == CONFIG_NOTHING_سېټ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	حرر(comment);
	حرر(ارزښت);
	أرجع ret;
}

ساكن صحيح cmd_config_unset(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			    بنية repository *repo ناکارول_شوی)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *ارزښت_نمونه = عدم;
	صحيح flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ډله(N_("Filter")),
		OPT_بټ(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &ارزښت_نمونه, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_بټ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ارزښت),
		OPT_پای_ټکی(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	إذا ((flags & CONFIG_FLAGS_FIXED_ارزښت) && !ارزښت_نمونه)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, مختاړی);
	check_write(&location_opts.سرچینه);

	إذا ((flags & CONFIG_FLAGS_MULTI_REPLACE) || ارزښت_نمونه)
		ret = repo_config_سېټ_multivar_دننه_file_gently(the_repository, location_opts.سرچینه.file,
							      argv[0], عدم, ارزښت_نمونه,
							      عدم, flags);
	وإلا
		ret = repo_config_سېټ_دننه_file_gently(the_repository, location_opts.سرچینه.file, argv[0],
						     عدم, عدم);

	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_rename_section(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				     بنية repository *repo ناکارول_شوی)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_پای_ټکی(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, مختاړی);
	check_write(&location_opts.سرچینه);

	ret = repo_config_rename_section_دننه_file(the_repository, location_opts.سرچینه.file,
						 argv[0], argv[1]);
	إذا (ret < 0)
		اذهب بهر;
	وإلا إذا (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

بهر:
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_remove_section(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				     بنية repository *repo ناکارول_شوی)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_پای_ټکی(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, مختاړی);
	check_write(&location_opts.سرچینه);

	ret = repo_config_rename_section_دننه_file(the_repository, location_opts.سرچینه.file,
						 argv[0], عدم);
	إذا (ret < 0)
		اذهب بهر;
	وإلا إذا (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

بهر:
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح show_editor(بنية config_location_options *opts)
{
	حرف *config_file;

	إذا (!opts->سرچینه.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	إذا (opts->سرچینه.use_stdin)
		die(_("editing stdin is not supported"));
	إذا (opts->سرچینه.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, عدم);
	config_file = opts->سرچینه.file ?
			xstrdup(opts->سرچینه.file) :
			repo_git_لار(the_repository, "config");
	إذا (opts->use_global_config) {
		صحيح fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		إذا (fd >= 0) {
			حرف *content = default_user_config();
			write_str_دننه_ډک(fd, content);
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

ساكن صحيح cmd_config_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			   بنية repository *repo ناکارول_شوی)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_پای_ټکی(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, مختاړی);
	check_write(&location_opts.سرچینه);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	أرجع ret;
}

ساكن صحيح cmd_config_actions(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی)
{
	تعداد {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_جمع_کړه = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_سېټ = (1<<11),
		ACTION_سېټ_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *comment_arg = عدم;
	صحيح actions = 0;
	بینښه flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_ډله(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_جمع_کړه),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_ډله(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_ارزښت,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_بټ(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_ارزښت),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_پای_ټکی(),
	};
	حرف *ارزښت = عدم, *comment = عدم;
	صحيح ret = 0;
	صحيح actions_implicit;
	بنية کلی_ارزښت_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, مختاړی, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, مختاړی);
	display_options_init(&display_opts);

	إذا ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		وتل(129);
	}

	actions_implicit = (actions == 0);
	إذا (actions_implicit)
		بدل (argc) {
		حالة 1: actions = ACTION_GET; اكسر;
		حالة 2: actions = ACTION_سېټ; اكسر;
		حالة 3: actions = ACTION_سېټ_ALL; اكسر;
		افتراضي:
			error(_("no action specified"));
			وتل(129);
		}
	إذا (actions_implicit && argc == 1) {
		ثابت حرف *وروستی_dot = strrchr(argv[0], '.');
		إذا (وروستی_dot && strchr(وروستی_dot + 1, '='))
			die_missing_سېټ_ارزښت(argv[0]);
	}
	إذا (display_opts.omit_ارزښتونه &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		وتل(129);
	}

	إذا (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		وتل(129);
	}

	إذا (display_opts.default_ارزښت && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		وتل(129);
	}

	إذا (comment_arg &&
	    !(actions & (ACTION_جمع_کړه|ACTION_سېټ|ACTION_سېټ_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		وتل(129);
	}

	/* check usage of --fixed-value */
	إذا (flags & CONFIG_FLAGS_FIXED_ارزښت) {
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
		حالة ACTION_سېټ_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		حالة ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			اكسر;

		/* other options don't allow --fixed-value */
		}

		إذا (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			وتل(129);
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
					&location_opts.سرچینه, the_repository,
					&location_opts.options) < 0) {
			إذا (location_opts.سرچینه.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.سرچینه.file);
			وإلا
				die(_("error processing config file(s)"));
		}
	}
	وإلا إذا (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	وإلا إذا (actions == ACTION_سېټ) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 2, 2);
		ارزښت = normalize_ارزښت(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سېټ_دننه_file_gently(the_repository, location_opts.سرچینه.file, argv[0], comment, ارزښت);
		إذا (ret == CONFIG_NOTHING_سېټ)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	وإلا إذا (actions == ACTION_سېټ_ALL) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 2, 3);
		ارزښت = normalize_ارزښت(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سېټ_multivar_دننه_file_gently(the_repository, location_opts.سرچینه.file,
							      argv[0], ارزښت, argv[2],
							      comment, flags);
	}
	وإلا إذا (actions == ACTION_جمع_کړه) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 2, 2);
		ارزښت = normalize_ارزښت(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سېټ_multivar_دننه_file_gently(the_repository, location_opts.سرچینه.file,
							      argv[0], ارزښت,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	وإلا إذا (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 2, 3);
		ارزښت = normalize_ارزښت(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_سېټ_multivar_دننه_file_gently(the_repository, location_opts.سرچینه.file,
							      argv[0], ارزښت, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وإلا إذا (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_ارزښت(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	وإلا إذا (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_ارزښت(&location_opts, &display_opts, argv[0], argv[1],
				GET_ارزښت_ALL, flags);
	}
	وإلا إذا (actions == ACTION_GET_REGEXP) {
		display_opts.show_کلیګانې = 1;
		check_argc(argc, 1, 2);
		ret = get_ارزښت(&location_opts, &display_opts, argv[0], argv[1],
				GET_ارزښت_ALL|GET_ارزښت_کلی_REGEXP, flags);
	}
	وإلا إذا (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	وإلا إذا (actions == ACTION_UNSET) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 1, 2);
		إذا (argc == 2)
			ret = repo_config_سېټ_multivar_دننه_file_gently(the_repository, location_opts.سرچینه.file,
								      argv[0], عدم, argv[1],
								      عدم, flags);
		وإلا
			ret = repo_config_سېټ_دننه_file_gently(the_repository, location_opts.سرچینه.file,
							     argv[0], عدم, عدم);
	}
	وإلا إذا (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 1, 2);
		ret = repo_config_سېټ_multivar_دننه_file_gently(the_repository, location_opts.سرچینه.file,
							      argv[0], عدم, argv[1],
							      عدم, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	وإلا إذا (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_دننه_file(the_repository, location_opts.سرچینه.file,
							 argv[0], argv[1]);
		إذا (ret < 0)
			اذهب بهر;
		وإلا إذا (!ret)
			die(_("no such section: %s"), argv[0]);
		وإلا
			ret = 0;
	}
	وإلا إذا (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.سرچینه);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_دننه_file(the_repository, location_opts.سرچینه.file,
							 argv[0], عدم);
		إذا (ret < 0)
			اذهب بهر;
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

بهر:
	location_options_release(&location_opts);
	حرر(comment);
	حرر(ارزښت);
	أرجع ret;
}

صحيح cmd_config(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *مختاړی,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *subcommand = عدم;
	بنية option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_سېټ),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_پای_ټکی(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, مختاړی, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	إذا (subcommand) {
		argc = parse_options(argc, argv, مختاړی, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		أرجع subcommand(argc, argv, مختاړی, repo);
	}

	أرجع cmd_config_actions(argc, argv, مختاړی);
}
