#define USE_THE_REPOSITORY_VARIABLE
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

ساكن ثابت حرف *ثابت builtin_config_set_usage[] = {
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

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

بنية config_location_options {
	بنية git_config_source source;
	بنية config_options options;
	حرف *file_to_free;
	صحيح use_global_config;
	صحيح use_system_config;
	صحيح use_local_config;
	صحيح use_worktree_config;
	صحيح respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

تعداد config_type {
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
	OPT_CALLBACK_VALUE(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_VALUE(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_VALUE(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_VALUE(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_VALUE(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_VALUE(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_values, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_keys, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

بنية config_display_options {
	صحيح end_nul;
	صحيح omit_values;
	صحيح show_origin;
	صحيح show_scope;
	صحيح show_keys;
	تعداد config_type type;
	حرف *default_value;
	/* Populated via `display_options_init()`. */
	صحيح term;
	صحيح delim;
	صحيح key_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.key_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_VALUE(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.value = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

ساكن صحيح option_parse_type(ثابت بنية option *opt, ثابت حرف *arg,
			     صحيح unset)
{
	صحيح new_type, *to_type;

	אם (unset) {
		*((صحيح *) opt->value) = 0;
		החזר 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	אם (!new_type) {
		אם (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		אחרת אם (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		אחרת אם (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		אחרת אם (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		אחרת אם (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		אחרת אם (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		אחרת אם (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		אחרת
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->value;
	אם (*to_type && *to_type != new_type) {
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

	החזר 0;
}

ساكن فراغ check_argc(صحيح argc, صحيح min, صحيح max)
{
	אם (argc >= min && argc <= max)
		החזר;
	אם (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	אחרת
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

ساكن NORETURN فراغ die_missing_set_value(ثابت حرف *arg)
{
	ثابت حرف *last_dot = strrchr(arg, '.');
	ثابت حرف *eq = last_dot ? strchr(last_dot + 1, '=') : عدم;
	حرف *prefix = eq ? xstrndup(arg, eq - arg) : عدم;

	אם (prefix && git_config_key_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} אחרת אם (git_config_key_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} אחרת {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	حرر(prefix);
	exit(129);
}

ساكن فراغ show_config_origin(ثابت بنية config_display_options *opts,
			       ثابت بنية key_value_info *kvi,
			       بنية strbuf *buf)
{
	ثابت حرف term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	אם (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	אחרת
		quote_c_style(kvi->filename ? kvi->filename : "", buf, عدم, 0);
	strbuf_addch(buf, term);
}

ساكن فراغ show_config_scope(ثابت بنية config_display_options *opts,
			      ثابت بنية key_value_info *kvi,
			      بنية strbuf *buf)
{
	ثابت حرف term = opts->end_nul ? '\0' : '\t';
	ثابت حرف *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

بنية strbuf_list {
	بنية strbuf *items;
	صحيح nr;
	صحيح alloc;
};

ساكن صحيح format_config_int64(بنية strbuf *buf,
			       ثابت حرف *key_,
			       ثابت حرف *value_,
			       ثابت بنية key_value_info *kvi,
			       صحيح gently)
{
	int64_t v = 0;
	אם (gently) {
		אם (!git_parse_int64(value_, &v))
			החזר -1;
	} אחרת {
		/* may die() */
		v = git_config_int64(key_, value_ ? value_ : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	החזר 0;
}

ساكن صحيح format_config_bool(بنية strbuf *buf,
			      ثابت حرف *key_,
			      ثابت حرف *value_,
			      صحيح gently)
{
	صحيح v = 0;
	אם (gently) {
		אם ((v = git_parse_maybe_bool(value_)) < 0)
			החזר -1;
	} אחרת {
		/* may die() */
		v = git_config_bool(key_, value_);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	החזר 0;
}

ساكن صحيح format_config_bool_or_int(بنية strbuf *buf,
				     ثابت حرف *key_,
				     ثابت حرف *value_,
				     ثابت بنية key_value_info *kvi,
				     صحيح gently)
{
	صحيح v, is_bool = 0;

	אם (gently) {
		v = git_parse_maybe_bool_text(value_);

		אם (v >= 0)
			is_bool = 1;
		אחרת אם (!git_parse_int(value_, &v))
			החזר -1;
	} אחרת {
		v = git_config_bool_or_int(key_, value_, kvi,
					   &is_bool);
	}

	אם (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	אחרת
		strbuf_addf(buf, "%d", v);

	החזר 0;
}

/* This mode is always gentle. */
ساكن صحيح format_config_bool_or_str(بنية strbuf *buf,
				     ثابت حرف *value_)
{
	صحيح v = git_parse_maybe_bool(value_);
	אם (v < 0)
		strbuf_addstr(buf, value_);
	אחרת
		strbuf_addstr(buf, v ? "true" : "false");
	החזר 0;
}

ساكن صحيح format_config_path(بنية strbuf *buf,
			      ثابت حرف *key_,
			      ثابت حرف *value_,
			      صحيح gently)
{
	حرف *v;

	אם (git_config_pathname(&v, key_, value_) < 0)
		החזר -1;

	אם (v)
		strbuf_addstr(buf, v);
	אחרת
		החזר gently ? -1 : 1; /* :(optional)no-such-file */

	حرر(v);
	החזר 0;
}

ساكن صحيح format_config_expiry_date(بنية strbuf *buf,
				     ثابت حرف *key_,
				     ثابت حرف *value_,
				     صحيح quietly)
{
	timestamp_t t;
	אם (quietly) {
		אם (parse_expiry_date(value_, &t))
			החזר -1;
	} אחרת אם (git_config_expiry_date(&t, key_, value_) < 0) {
		החזר -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	החזר 0;
}

ساكن صحيح format_config_color(بنية strbuf *buf,
			       ثابت حرف *key_,
			       ثابت حرف *value_,
			       صحيح gently)
{
	حرف v[COLOR_MAXLEN];

	אם (gently) {
		אם (color_parse_quietly(value_, v) < 0)
			החזר -1;
	} אחרת אם (git_config_color(v, key_, value_) < 0) {
		החזר -1;
	}

	strbuf_addstr(buf, v);
	החזר 0;
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
			 بنية strbuf *buf, ثابت حرف *key_,
			 ثابت حرف *value_, ثابت بنية key_value_info *kvi,
			 صحيح gently)
{
	صحيح res = 0;
	אם (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	אם (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	אם (opts->show_keys)
		strbuf_addstr(buf, key_);

	אם (opts->omit_values)
		اذهب terminator;

	אם (opts->show_keys)
		strbuf_addch(buf, opts->key_delim);

	بدل (opts->type) {
	حالة TYPE_INT:
		res = format_config_int64(buf, key_, value_, kvi, gently);
		اكسر;

	حالة TYPE_BOOL:
		res = format_config_bool(buf, key_, value_, gently);
		اكسر;

	حالة TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, key_, value_, kvi, gently);
		اكسر;

	حالة TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, value_);
		اكسر;

	حالة TYPE_PATH:
		res = format_config_path(buf, key_, value_, gently);
		اكسر;

	حالة TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, key_, value_, gently);
		اكسر;

	حالة TYPE_COLOR:
		res = format_config_color(buf, key_, value_, gently);
		اكسر;

	حالة TYPE_NONE:
		אם (value_) {
			strbuf_addstr(buf, value_);
		} אחרת {
			/* Just show the key name; back out delimiter */
			אם (opts->show_keys)
				strbuf_setlen(buf, buf->len - 1);
		}
		اكسر;

	افتراضي:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	החזר res;
}

ساكن صحيح show_all_config(ثابت حرف *key_, ثابت حرف *value_,
			   ثابت بنية config_context *ctx,
			   فراغ *cb)
{
	ثابت بنية config_display_options *opts = cb;
	ثابت بنية key_value_info *kvi = ctx->kvi;
	بنية strbuf formatted = STRBUF_INIT;

	אם (format_config(opts, &formatted, key_, value_, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	החזר 0;
}

#define GET_VALUE_ALL        (1 << 0)
#define GET_VALUE_KEY_REGEXP (1 << 1)

بنية collect_config_data {
	ثابت بنية config_display_options *display_opts;
	بنية strbuf_list *values;
	ثابت حرف *value_pattern;
	ثابت حرف *key;
	regex_t *regexp;
	regex_t *key_regexp;
	صحيح do_not_match;
	unsigned get_value_flags;
	unsigned flags;
};

ساكن صحيح collect_config(ثابت حرف *key_, ثابت حرف *value_,
			  ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية collect_config_data *data = cb;
	بنية strbuf_list *values = data->values;
	ثابت بنية key_value_info *kvi = ctx->kvi;
	صحيح status;

	אם (!(data->get_value_flags & GET_VALUE_KEY_REGEXP) &&
	    strcmp(key_, data->key))
		החזר 0;
	אם ((data->get_value_flags & GET_VALUE_KEY_REGEXP) &&
	    regexec(data->key_regexp, key_, 0, عدم, 0))
		החזר 0;
	אם ((data->flags & CONFIG_FLAGS_FIXED_VALUE) &&
	    strcmp(data->value_pattern, (value_?value_:"")))
		החזר 0;
	אם (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (value_?value_:""), 0, عدم, 0)))
		החזר 0;

	ALLOC_GROW(values->items, values->nr + 1, values->alloc);
	strbuf_init(&values->items[values->nr], 0);

	status = format_config(data->display_opts, &values->items[values->nr++],
			       key_, value_, kvi, 0);
	אם (status < 0)
		החזר status;
	אם (status) {
		strbuf_release(&values->items[--values->nr]);
		status = 0;
	}
	החזר status;
}

ساكن صحيح get_value(ثابت بنية config_location_options *opts,
		     ثابت بنية config_display_options *display_opts,
		     ثابت حرف *key_, ثابت حرف *regex_,
		     unsigned get_value_flags, unsigned flags)
{
	صحيح ret = CONFIG_GENERIC_ERROR;
	بنية strbuf_list values = {عدم};
	بنية collect_config_data data = {
		.display_opts = display_opts,
		.values = &values,
		.get_value_flags = get_value_flags,
		.flags = flags,
	};
	حرف *key = عدم;
	صحيح i;

	אם (get_value_flags & GET_VALUE_KEY_REGEXP) {
		حرف *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		key = xstrdup(key_);
		עבור (tl = key + strlen(key) - 1;
		     tl >= key && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		עבור (tl = key; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.key_regexp = (regex_t*)xmalloc(حجم(regex_t));
		אם (regcomp(data.key_regexp, key, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), key_);
			FREE_AND_NULL(data.key_regexp);
			ret = CONFIG_INVALID_PATTERN;
			اذهب free_strings;
		}
	} אחרת {
		אם (git_config_parse_key(key_, &key, عدم)) {
			ret = CONFIG_INVALID_KEY;
			اذهب free_strings;
		}

		data.key = key;
	}

	אם (regex_ && (flags & CONFIG_FLAGS_FIXED_VALUE))
		data.value_pattern = regex_;
	אחרת אם (regex_) {
		אם (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(حجم(regex_t));
		אם (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			اذهب free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	אם (!values.nr && display_opts->default_value) {
		بنية key_value_info kvi = KVI_INIT;
		بنية strbuf *item;
		صحيح status;

		kvi_from_param(&kvi);
		ALLOC_GROW(values.items, values.nr + 1, values.alloc);
		item = &values.items[values.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, key_,
				       display_opts->default_value, &kvi, 0);
		אם (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_value);
		אם (status) {
			/* default was a missing optional value */
			values.nr--;
			strbuf_release(item);
		}
	}

	ret = !values.nr;

	עבור (i = 0; i < values.nr; i++) {
		بنية strbuf *buf = values.items + i;
		אם ((get_value_flags & GET_VALUE_ALL) || i == values.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	حرر(values.items);

free_strings:
	حرر(key);
	אם (data.key_regexp) {
		regfree(data.key_regexp);
		حرر(data.key_regexp);
	}
	אם (data.regexp) {
		regfree(data.regexp);
		حرر(data.regexp);
	}

	החזר ret;
}

ساكن حرف *normalize_value(ثابت حرف *key, ثابت حرف *value,
			     صحيح type, بنية key_value_info *kvi)
{
	אם (!value)
		החזר عدم;

	אם (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		החזר xstrdup(value);
	אם (type == TYPE_INT)
		החזר xstrfmt("%"PRId64, git_config_int64(key, value, kvi));
	אם (type == TYPE_BOOL)
		החזר xstrdup(git_config_bool(key, value) ?  "true" : "false");
	אם (type == TYPE_BOOL_OR_INT) {
		صحيح is_bool, v;
		v = git_config_bool_or_int(key, value, kvi, &is_bool);
		אם (!is_bool)
			החזר xstrfmt("%d", v);
		אחרת
			החזר xstrdup(v ? "true" : "false");
	}
	אם (type == TYPE_BOOL_OR_STR) {
		صحيح v = git_parse_maybe_bool(value);
		אם (v < 0)
			החזר xstrdup(value);
		אחרת
			החזר xstrdup(v ? "true" : "false");
	}
	אם (type == TYPE_COLOR) {
		حرف v[COLOR_MAXLEN];
		אם (git_config_color(v, key, value))
			die(_("cannot parse color '%s'"), value);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		החזר xstrdup(value);
	}

	BUG("cannot normalize type %d", type);
}

بنية get_color_config_data {
	صحيح get_color_found;
	ثابت حرف *get_color_slot;
	حرف parsed_color[COLOR_MAXLEN];
};

ساكن صحيح git_get_color_config(ثابت حرف *var, ثابت حرف *value,
				ثابت بنية config_context *ctx UNUSED,
				فراغ *cb)
{
	بنية get_color_config_data *data = cb;

	אם (!strcmp(var, data->get_color_slot)) {
		אם (!value)
			config_error_nonbool(var);
		אם (color_parse(value, data->parsed_color) < 0)
			החזר -1;
		data->get_color_found = 1;
	}
	החזר 0;
}

ساكن صحيح get_color(ثابت بنية config_location_options *opts,
		      ثابت حرف *var, ثابت حرف *def_color)
{
	بنية get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	صحيح ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	אם (!data.get_color_found && def_color) {
		אם (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			اذهب out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	החזר ret;
}

بنية get_colorbool_config_data {
	تعداد git_colorbool get_colorbool_found;
	تعداد git_colorbool get_diff_color_found;
	تعداد git_colorbool get_color_ui_found;
	ثابت حرف *get_colorbool_slot;
};

ساكن صحيح git_get_colorbool_config(ثابت حرف *var, ثابت حرف *value,
				    ثابت بنية config_context *ctx UNUSED,
				    فراغ *cb)
{
	بنية get_colorbool_config_data *data = cb;

	אם (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, value);
	אחרת אם (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, value);
	אחרת אם (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, value);
	החזר 0;
}

ساكن صحيح get_colorbool(ثابت بنية config_location_options *opts,
			 ثابت حرف *var, صحيح print)
{
	بنية get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	אם (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		אם (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		אם (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	אם (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	אם (print) {
		printf("%s\n", result ? "true" : "false");
		החזר 0;
	} אחרת
		החזר result ? 0 : 1;
}

ساكن فراغ check_write(ثابت بنية git_config_source *source)
{
	אם (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	אם (source->use_stdin)
		die(_("writing to stdin is not supported"));

	אם (source->blob)
		die(_("writing config blobs is not supported"));
}

بنية urlmatch_current_candidate_value {
	حرف value_is_null;
	بنية strbuf value;
	بنية key_value_info kvi;
};

ساكن صحيح urlmatch_collect_fn(ثابت حرف *var, ثابت حرف *value,
			       ثابت بنية config_context *ctx,
			       فراغ *cb)
{
	بنية string_list *values = cb;
	بنية string_list_item *item = string_list_insert(values, var);
	بنية urlmatch_current_candidate_value *matched = item->util;
	ثابت بنية key_value_info *kvi = ctx->kvi;

	אם (!matched) {
		matched = xmalloc(حجم(*matched));
		strbuf_init(&matched->value, 0);
		item->util = matched;
	} אחרת {
		strbuf_reset(&matched->value);
	}
	matched->kvi = *kvi;

	אם (value) {
		strbuf_addstr(&matched->value, value);
		matched->value_is_null = 0;
	} אחרת {
		matched->value_is_null = 1;
	}
	החזר 0;
}

ساكن صحيح get_urlmatch(ثابت بنية config_location_options *opts,
			ثابت بنية config_display_options *_display_opts,
			ثابت حرف *var, ثابت حرف *url)
{
	صحيح ret;
	حرف *section;
	حرف *section_tail;
	بنية config_display_options display_opts = *_display_opts;
	بنية string_list_item *item;
	بنية urlmatch_config config = URLMATCH_CONFIG_INIT;
	بنية string_list values = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = عدم;
	config.cb = &values;

	אם (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	אם (section_tail) {
		*section_tail = '\0';
		config.key = section_tail + 1;
		display_opts.show_keys = 0;
	} אחרת {
		config.key = عدم;
		display_opts.show_keys = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !values.nr;

	for_each_string_list_item(item, &values) {
		بنية urlmatch_current_candidate_value *matched = item->util;
		بنية strbuf buf = STRBUF_INIT;
		صحيح status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->value_is_null ? عدم : matched->value.buf,
				       &matched->kvi, 0);
		אם (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->value);
	}
	urlmatch_config_release(&config);
	string_list_clear(&values, 1);
	حرر(config.url.url);

	حرر(section);
	החזר ret;
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
	החזר strbuf_detach(&buf, عدم);
}

ساكن فراغ location_options_init(بنية config_location_options *opts,
				  ثابت حرف *prefix)
{
	אם (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	אם (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	אם (!startup_info->have_repository) {
		אם (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		אם (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		אם (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	אם (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = عدم;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	אם (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		אם (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} אחרת אם (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} אחרת אם (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} אחרת אם (opts->use_worktree_config) {
		بنية worktree **worktrees = get_worktrees(the_repository);
		אם (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		אחרת אם (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		אחרת
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} אחרת אם (opts->source.file) {
		אם (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} אחרת אם (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	אם (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	אחרת
		opts->options.respect_includes = opts->respect_includes_opt;
	אם (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

ساكن فراغ location_options_release(بنية config_location_options *opts)
{
	حرر(opts->file_to_free);
}

ساكن فراغ display_options_init(بنية config_display_options *opts)
{
	אם (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->key_delim = '\n';
	}
}

ساكن فراغ display_options_init_list(بنية config_display_options *opts)
{
	opts->show_keys = 1;

	אם (opts->end_nul) {
		display_options_init(opts);
	} אחרת {
		opts->term = '\n';
		opts->delim = ' ';
		opts->key_delim = '=';
	}
}

ساكن صحيح cmd_config_list(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			   بنية repository *repo UNUSED)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	بنية option opts[] = {
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

	אם (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		אם (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		אחרת
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	החזר 0;
}

ساكن صحيح cmd_config_get(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			  بنية repository *repo UNUSED)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *value_pattern = عدم, *url = عدم;
	صحيح flags = 0;
	unsigned get_value_flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_value_flags, N_("return all values for multi-valued config options"), GET_VALUE_ALL),
		OPT_BIT(0, "regexp", &get_value_flags, N_("interpret the name as a regular expression"), GET_VALUE_KEY_REGEXP),
		OPT_STRING(0, "value", &value_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_VALUE),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_value,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	אם ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	אם (display_opts.default_value &&
	    ((get_value_flags & GET_VALUE_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	אם (url && ((get_value_flags & GET_VALUE_ALL) ||
		    (get_value_flags & GET_VALUE_KEY_REGEXP) ||
		    value_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	אם (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	אם (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	אחרת אם (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_value)
		ret = get_color(&location_opts, "", display_opts.default_value);
	אחרת
		ret = get_value(&location_opts, &display_opts, argv[0], value_pattern,
				get_value_flags, flags);

	location_options_release(&location_opts);
	החזר ret;
}

ساكن صحيح cmd_config_set(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			  بنية repository *repo UNUSED)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *value_pattern = عدم, *comment_arg = عدم;
	حرف *comment = عدم;
	صحيح flags = 0, append = 0, type = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &value_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_VALUE),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	بنية key_value_info default_kvi = KVI_INIT;
	حرف *value;
	صحيح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	אם (argc == 1)
		die_missing_set_value(argv[0]);
	check_argc(argc, 2, 2);

	אם ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	אם (append && value_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	אם (append)
		value_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	value = normalize_value(argv[0], argv[1], type, &default_kvi);

	אם ((flags & CONFIG_FLAGS_MULTI_REPLACE) || value_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, value_pattern,
							      comment, flags);
	} אחרת {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, value);
		אם (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	حرر(comment);
	حرر(value);
	החזר ret;
}

ساكن صحيح cmd_config_unset(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			    بنية repository *repo UNUSED)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ثابت حرف *value_pattern = عدم;
	صحيح flags = 0;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &value_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_VALUE),
		OPT_END(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	אם ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	אם ((flags & CONFIG_FLAGS_MULTI_REPLACE) || value_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], عدم, value_pattern,
							      عدم, flags);
	אחרת
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     عدم, عدم);

	location_options_release(&location_opts);
	החזר ret;
}

ساكن صحيح cmd_config_rename_section(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				     بنية repository *repo UNUSED)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	אם (ret < 0)
		اذهب out;
	אחרת אם (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	החזר ret;
}

ساكن صحيح cmd_config_remove_section(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				     بنية repository *repo UNUSED)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], عدم);
	אם (ret < 0)
		اذهب out;
	אחרת אם (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	החזר ret;
}

ساكن صحيح show_editor(بنية config_location_options *opts)
{
	حرف *config_file;

	אם (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	אם (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	אם (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, عدم);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	אם (opts->use_global_config) {
		صحيح fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		אם (fd >= 0) {
			حرف *content = default_user_config();
			write_str_in_full(fd, content);
			حرر(content);
			close(fd);
		}
		אחרת אם (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	אם (launch_editor(config_file, عدم, عدم)) {
		حرر(config_file);
		החזר -1;
	}
	حرر(config_file);

	החזר 0;
}

ساكن صحيح cmd_config_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			   بنية repository *repo UNUSED)
{
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	صحيح ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	החזר ret;
}

ساكن صحيح cmd_config_actions(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix)
{
	تعداد {
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
	بنية config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	بنية config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ثابت حرف *comment_arg = عدم;
	صحيح actions = 0;
	unsigned flags = 0;
	بنية option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_value,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_VALUE),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	حرف *value = عدم, *comment = عدم;
	صحيح ret = 0;
	صحيح actions_implicit;
	بنية key_value_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	אם ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	אם (actions_implicit)
		بدل (argc) {
		حالة 1: actions = ACTION_GET; اكسر;
		حالة 2: actions = ACTION_SET; اكسر;
		حالة 3: actions = ACTION_SET_ALL; اكسر;
		افتراضي:
			error(_("no action specified"));
			exit(129);
		}
	אם (actions_implicit && argc == 1) {
		ثابت حرف *last_dot = strrchr(argv[0], '.');
		אם (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_value(argv[0]);
	}
	אם (display_opts.omit_values &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	אם (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	אם (display_opts.default_value && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	אם (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	אם (flags & CONFIG_FLAGS_FIXED_VALUE) {
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
		حالة ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		حالة ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			اكسر;

		/* other options don't allow --fixed-value */
		}

		אם (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	אם (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	אם (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		אם (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			אם (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			אחרת
				die(_("error processing config file(s)"));
		}
	}
	אחרת אם (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	אחרת אם (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, value);
		אם (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	אחרת אם (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, argv[2],
							      comment, flags);
	}
	אחרת אם (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	אחרת אם (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	אחרת אם (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	אחרת אם (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				GET_VALUE_ALL, flags);
	}
	אחרת אם (actions == ACTION_GET_REGEXP) {
		display_opts.show_keys = 1;
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				GET_VALUE_ALL|GET_VALUE_KEY_REGEXP, flags);
	}
	אחרת אם (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	אחרת אם (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		אם (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], عدم, argv[1],
								      عدم, flags);
		אחרת
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], عدم, عدم);
	}
	אחרת אם (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], عدم, argv[1],
							      عدم, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	אחרת אם (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		אם (ret < 0)
			اذهب out;
		אחרת אם (!ret)
			die(_("no such section: %s"), argv[0]);
		אחרת
			ret = 0;
	}
	אחרת אם (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], عدم);
		אם (ret < 0)
			اذهب out;
		אחרת אם (!ret)
			die(_("no such section: %s"), argv[0]);
		אחרת
			ret = 0;
	}
	אחרת אם (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	אחרת אם (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		אם (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	حرر(comment);
	حرر(value);
	החזר ret;
}

صحيح cmd_config(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *subcommand = عدم;
	بنية option subcommand_opts[] = {
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
	אם (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		החזר subcommand(argc, argv, prefix, repo);
	}

	החזר cmd_config_actions(argc, argv, prefix);
}
