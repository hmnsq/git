#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "abspath.h"
#include "advice.h"
#include "config.h"
#include "color.h"
#include "date.h"
#include "editor.h"
#include "environment.h"
#include "gettext.h"
#include "ident.h"
#include "parse-options.h"
#include "urlmatch.h"
#include "path.h"
#include "quote.h"
#include "setup.h"
#include "strbuf.h"
#include "worktree.h"

static const char *const builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	NULL
};

static const char *const builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	NULL
};

static const char *const builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	NULL
};

static const char *const builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	NULL
};

static const char *const builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	NULL
};

static const char *const builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	NULL
};

static const char *const builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	NULL
};

static const char *const builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	NULL
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

struct config_location_options {
	struct git_config_source source;
	struct config_options options;
	char *file_to_free;
	целое use_global_config;
	целое use_system_config;
	целое use_local_config;
	целое use_worktree_config;
	целое respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

enum config_type {
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

struct config_display_options {
	целое end_nul;
	целое omit_values;
	целое show_origin;
	целое show_scope;
	целое show_keys;
	enum config_type type;
	char *default_value;
	/* Populated via `display_options_init()`. */
	целое term;
	целое delim;
	целое key_delim;
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

static целое option_parse_type(const struct option *opt, const char *arg,
			     целое unset)
{
	целое new_type, *to_type;

	если (unset) {
		*((целое *) opt->value) = 0;
		вернуть 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	если (!new_type) {
		если (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		иначе если (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		иначе если (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		иначе если (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		иначе если (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		иначе если (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		иначе если (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		иначе
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->value;
	если (*to_type && *to_type != new_type) {
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

	вернуть 0;
}

static void check_argc(целое argc, целое min, целое max)
{
	если (argc >= min && argc <= max)
		вернуть;
	если (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	иначе
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

static NORETURN void die_missing_set_value(const char *arg)
{
	const char *last_dot = strrchr(arg, '.');
	const char *eq = last_dot ? strchr(last_dot + 1, '=') : NULL;
	char *prefix = eq ? xstrndup(arg, eq - arg) : NULL;

	если (prefix && git_config_key_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} иначе если (git_config_key_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} иначе {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	free(prefix);
	exit(129);
}

static void show_config_origin(const struct config_display_options *opts,
			       const struct key_value_info *kvi,
			       struct strbuf *buf)
{
	const char term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	если (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	иначе
		quote_c_style(kvi->filename ? kvi->filename : "", buf, NULL, 0);
	strbuf_addch(buf, term);
}

static void show_config_scope(const struct config_display_options *opts,
			      const struct key_value_info *kvi,
			      struct strbuf *buf)
{
	const char term = opts->end_nul ? '\0' : '\t';
	const char *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

struct strbuf_list {
	struct strbuf *items;
	целое nr;
	целое alloc;
};

static целое format_config_int64(struct strbuf *buf,
			       const char *key_,
			       const char *value_,
			       const struct key_value_info *kvi,
			       целое gently)
{
	int64_t v = 0;
	если (gently) {
		если (!git_parse_int64(value_, &v))
			вернуть -1;
	} иначе {
		/* may die() */
		v = git_config_int64(key_, value_ ? value_ : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	вернуть 0;
}

static целое format_config_bool(struct strbuf *buf,
			      const char *key_,
			      const char *value_,
			      целое gently)
{
	целое v = 0;
	если (gently) {
		если ((v = git_parse_maybe_bool(value_)) < 0)
			вернуть -1;
	} иначе {
		/* may die() */
		v = git_config_bool(key_, value_);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	вернуть 0;
}

static целое format_config_bool_or_int(struct strbuf *buf,
				     const char *key_,
				     const char *value_,
				     const struct key_value_info *kvi,
				     целое gently)
{
	целое v, is_bool = 0;

	если (gently) {
		v = git_parse_maybe_bool_text(value_);

		если (v >= 0)
			is_bool = 1;
		иначе если (!git_parse_int(value_, &v))
			вернуть -1;
	} иначе {
		v = git_config_bool_or_int(key_, value_, kvi,
					   &is_bool);
	}

	если (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	иначе
		strbuf_addf(buf, "%d", v);

	вернуть 0;
}

/* This mode is always gentle. */
static целое format_config_bool_or_str(struct strbuf *buf,
				     const char *value_)
{
	целое v = git_parse_maybe_bool(value_);
	если (v < 0)
		strbuf_addstr(buf, value_);
	иначе
		strbuf_addstr(buf, v ? "true" : "false");
	вернуть 0;
}

static целое format_config_path(struct strbuf *buf,
			      const char *key_,
			      const char *value_,
			      целое gently)
{
	char *v;

	если (git_config_pathname(&v, key_, value_) < 0)
		вернуть -1;

	если (v)
		strbuf_addstr(buf, v);
	иначе
		вернуть gently ? -1 : 1; /* :(optional)no-such-file */

	free(v);
	вернуть 0;
}

static целое format_config_expiry_date(struct strbuf *buf,
				     const char *key_,
				     const char *value_,
				     целое quietly)
{
	timestamp_t t;
	если (quietly) {
		если (parse_expiry_date(value_, &t))
			вернуть -1;
	} иначе если (git_config_expiry_date(&t, key_, value_) < 0) {
		вернуть -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	вернуть 0;
}

static целое format_config_color(struct strbuf *buf,
			       const char *key_,
			       const char *value_,
			       целое gently)
{
	char v[COLOR_MAXLEN];

	если (gently) {
		если (color_parse_quietly(value_, v) < 0)
			вернуть -1;
	} иначе если (git_config_color(v, key_, value_) < 0) {
		вернуть -1;
	}

	strbuf_addstr(buf, v);
	вернуть 0;
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
static целое format_config(const struct config_display_options *opts,
			 struct strbuf *buf, const char *key_,
			 const char *value_, const struct key_value_info *kvi,
			 целое gently)
{
	целое res = 0;
	если (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	если (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	если (opts->show_keys)
		strbuf_addstr(buf, key_);

	если (opts->omit_values)
		goto terminator;

	если (opts->show_keys)
		strbuf_addch(buf, opts->key_delim);

	switch (opts->type) {
	case TYPE_INT:
		res = format_config_int64(buf, key_, value_, kvi, gently);
		break;

	case TYPE_BOOL:
		res = format_config_bool(buf, key_, value_, gently);
		break;

	case TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, key_, value_, kvi, gently);
		break;

	case TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, value_);
		break;

	case TYPE_PATH:
		res = format_config_path(buf, key_, value_, gently);
		break;

	case TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, key_, value_, gently);
		break;

	case TYPE_COLOR:
		res = format_config_color(buf, key_, value_, gently);
		break;

	case TYPE_NONE:
		если (value_) {
			strbuf_addstr(buf, value_);
		} иначе {
			/* Just show the key name; back out delimiter */
			если (opts->show_keys)
				strbuf_setlen(buf, buf->len - 1);
		}
		break;

	default:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	вернуть res;
}

static целое show_all_config(const char *key_, const char *value_,
			   const struct config_context *ctx,
			   void *cb)
{
	const struct config_display_options *opts = cb;
	const struct key_value_info *kvi = ctx->kvi;
	struct strbuf formatted = STRBUF_INIT;

	если (format_config(opts, &formatted, key_, value_, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	вернуть 0;
}

#define GET_VALUE_ALL        (1 << 0)
#define GET_VALUE_KEY_REGEXP (1 << 1)

struct collect_config_data {
	const struct config_display_options *display_opts;
	struct strbuf_list *values;
	const char *value_pattern;
	const char *key;
	regex_t *regexp;
	regex_t *key_regexp;
	целое do_not_match;
	unsigned get_value_flags;
	unsigned flags;
};

static целое collect_config(const char *key_, const char *value_,
			  const struct config_context *ctx, void *cb)
{
	struct collect_config_data *data = cb;
	struct strbuf_list *values = data->values;
	const struct key_value_info *kvi = ctx->kvi;
	целое status;

	если (!(data->get_value_flags & GET_VALUE_KEY_REGEXP) &&
	    strcmp(key_, data->key))
		вернуть 0;
	если ((data->get_value_flags & GET_VALUE_KEY_REGEXP) &&
	    regexec(data->key_regexp, key_, 0, NULL, 0))
		вернуть 0;
	если ((data->flags & CONFIG_FLAGS_FIXED_VALUE) &&
	    strcmp(data->value_pattern, (value_?value_:"")))
		вернуть 0;
	если (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (value_?value_:""), 0, NULL, 0)))
		вернуть 0;

	ALLOC_GROW(values->items, values->nr + 1, values->alloc);
	strbuf_init(&values->items[values->nr], 0);

	status = format_config(data->display_opts, &values->items[values->nr++],
			       key_, value_, kvi, 0);
	если (status < 0)
		вернуть status;
	если (status) {
		strbuf_release(&values->items[--values->nr]);
		status = 0;
	}
	вернуть status;
}

static целое get_value(const struct config_location_options *opts,
		     const struct config_display_options *display_opts,
		     const char *key_, const char *regex_,
		     unsigned get_value_flags, unsigned flags)
{
	целое ret = CONFIG_GENERIC_ERROR;
	struct strbuf_list values = {NULL};
	struct collect_config_data data = {
		.display_opts = display_opts,
		.values = &values,
		.get_value_flags = get_value_flags,
		.flags = flags,
	};
	char *key = NULL;
	целое i;

	если (get_value_flags & GET_VALUE_KEY_REGEXP) {
		char *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		key = xstrdup(key_);
		для (tl = key + strlen(key) - 1;
		     tl >= key && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		для (tl = key; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.key_regexp = (regex_t*)xmalloc(sizeof(regex_t));
		если (regcomp(data.key_regexp, key, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), key_);
			FREE_AND_NULL(data.key_regexp);
			ret = CONFIG_INVALID_PATTERN;
			goto free_strings;
		}
	} иначе {
		если (git_config_parse_key(key_, &key, NULL)) {
			ret = CONFIG_INVALID_KEY;
			goto free_strings;
		}

		data.key = key;
	}

	если (regex_ && (flags & CONFIG_FLAGS_FIXED_VALUE))
		data.value_pattern = regex_;
	иначе если (regex_) {
		если (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(sizeof(regex_t));
		если (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			goto free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	если (!values.nr && display_opts->default_value) {
		struct key_value_info kvi = KVI_INIT;
		struct strbuf *item;
		целое status;

		kvi_from_param(&kvi);
		ALLOC_GROW(values.items, values.nr + 1, values.alloc);
		item = &values.items[values.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, key_,
				       display_opts->default_value, &kvi, 0);
		если (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_value);
		если (status) {
			/* default was a missing optional value */
			values.nr--;
			strbuf_release(item);
		}
	}

	ret = !values.nr;

	для (i = 0; i < values.nr; i++) {
		struct strbuf *buf = values.items + i;
		если ((get_value_flags & GET_VALUE_ALL) || i == values.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	free(values.items);

free_strings:
	free(key);
	если (data.key_regexp) {
		regfree(data.key_regexp);
		free(data.key_regexp);
	}
	если (data.regexp) {
		regfree(data.regexp);
		free(data.regexp);
	}

	вернуть ret;
}

static char *normalize_value(const char *key, const char *value,
			     целое type, struct key_value_info *kvi)
{
	если (!value)
		вернуть NULL;

	если (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		вернуть xstrdup(value);
	если (type == TYPE_INT)
		вернуть xstrfmt("%"PRId64, git_config_int64(key, value, kvi));
	если (type == TYPE_BOOL)
		вернуть xstrdup(git_config_bool(key, value) ?  "true" : "false");
	если (type == TYPE_BOOL_OR_INT) {
		целое is_bool, v;
		v = git_config_bool_or_int(key, value, kvi, &is_bool);
		если (!is_bool)
			вернуть xstrfmt("%d", v);
		иначе
			вернуть xstrdup(v ? "true" : "false");
	}
	если (type == TYPE_BOOL_OR_STR) {
		целое v = git_parse_maybe_bool(value);
		если (v < 0)
			вернуть xstrdup(value);
		иначе
			вернуть xstrdup(v ? "true" : "false");
	}
	если (type == TYPE_COLOR) {
		char v[COLOR_MAXLEN];
		если (git_config_color(v, key, value))
			die(_("cannot parse color '%s'"), value);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		вернуть xstrdup(value);
	}

	BUG("cannot normalize type %d", type);
}

struct get_color_config_data {
	целое get_color_found;
	const char *get_color_slot;
	char parsed_color[COLOR_MAXLEN];
};

static целое git_get_color_config(const char *var, const char *value,
				const struct config_context *ctx UNUSED,
				void *cb)
{
	struct get_color_config_data *data = cb;

	если (!strcmp(var, data->get_color_slot)) {
		если (!value)
			config_error_nonbool(var);
		если (color_parse(value, data->parsed_color) < 0)
			вернуть -1;
		data->get_color_found = 1;
	}
	вернуть 0;
}

static целое get_color(const struct config_location_options *opts,
		      const char *var, const char *def_color)
{
	struct get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	целое ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	если (!data.get_color_found && def_color) {
		если (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			goto out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	вернуть ret;
}

struct get_colorbool_config_data {
	enum git_colorbool get_colorbool_found;
	enum git_colorbool get_diff_color_found;
	enum git_colorbool get_color_ui_found;
	const char *get_colorbool_slot;
};

static целое git_get_colorbool_config(const char *var, const char *value,
				    const struct config_context *ctx UNUSED,
				    void *cb)
{
	struct get_colorbool_config_data *data = cb;

	если (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, value);
	иначе если (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, value);
	иначе если (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, value);
	вернуть 0;
}

static целое get_colorbool(const struct config_location_options *opts,
			 const char *var, целое print)
{
	struct get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	если (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		если (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		если (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	если (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	если (print) {
		printf("%s\n", result ? "true" : "false");
		вернуть 0;
	} иначе
		вернуть result ? 0 : 1;
}

static void check_write(const struct git_config_source *source)
{
	если (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	если (source->use_stdin)
		die(_("writing to stdin is not supported"));

	если (source->blob)
		die(_("writing config blobs is not supported"));
}

struct urlmatch_current_candidate_value {
	char value_is_null;
	struct strbuf value;
	struct key_value_info kvi;
};

static целое urlmatch_collect_fn(const char *var, const char *value,
			       const struct config_context *ctx,
			       void *cb)
{
	struct string_list *values = cb;
	struct string_list_item *item = string_list_insert(values, var);
	struct urlmatch_current_candidate_value *matched = item->util;
	const struct key_value_info *kvi = ctx->kvi;

	если (!matched) {
		matched = xmalloc(sizeof(*matched));
		strbuf_init(&matched->value, 0);
		item->util = matched;
	} иначе {
		strbuf_reset(&matched->value);
	}
	matched->kvi = *kvi;

	если (value) {
		strbuf_addstr(&matched->value, value);
		matched->value_is_null = 0;
	} иначе {
		matched->value_is_null = 1;
	}
	вернуть 0;
}

static целое get_urlmatch(const struct config_location_options *opts,
			const struct config_display_options *_display_opts,
			const char *var, const char *url)
{
	целое ret;
	char *section;
	char *section_tail;
	struct config_display_options display_opts = *_display_opts;
	struct string_list_item *item;
	struct urlmatch_config config = URLMATCH_CONFIG_INIT;
	struct string_list values = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = NULL;
	config.cb = &values;

	если (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	если (section_tail) {
		*section_tail = '\0';
		config.key = section_tail + 1;
		display_opts.show_keys = 0;
	} иначе {
		config.key = NULL;
		display_opts.show_keys = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !values.nr;

	for_each_string_list_item(item, &values) {
		struct urlmatch_current_candidate_value *matched = item->util;
		struct strbuf buf = STRBUF_INIT;
		целое status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->value_is_null ? NULL : matched->value.buf,
				       &matched->kvi, 0);
		если (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->value);
	}
	urlmatch_config_release(&config);
	string_list_clear(&values, 1);
	free(config.url.url);

	free(section);
	вернуть ret;
}

static char *default_user_config(void)
{
	struct strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	вернуть strbuf_detach(&buf, NULL);
}

static void location_options_init(struct config_location_options *opts,
				  const char *prefix)
{
	если (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	если (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	если (!startup_info->have_repository) {
		если (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		если (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		если (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	если (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = NULL;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	если (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		если (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} иначе если (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} иначе если (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} иначе если (opts->use_worktree_config) {
		struct worktree **worktrees = get_worktrees(the_repository);
		если (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		иначе если (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		иначе
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} иначе если (opts->source.file) {
		если (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} иначе если (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	если (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	иначе
		opts->options.respect_includes = opts->respect_includes_opt;
	если (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

static void location_options_release(struct config_location_options *opts)
{
	free(opts->file_to_free);
}

static void display_options_init(struct config_display_options *opts)
{
	если (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->key_delim = '\n';
	}
}

static void display_options_init_list(struct config_display_options *opts)
{
	opts->show_keys = 1;

	если (opts->end_nul) {
		display_options_init(opts);
	} иначе {
		opts->term = '\n';
		opts->delim = ' ';
		opts->key_delim = '=';
	}
}

static целое cmd_config_list(целое argc, const char **argv, const char *prefix,
			   struct repository *repo UNUSED)
{
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	struct config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	struct option opts[] = {
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

	если (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		если (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		иначе
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	вернуть 0;
}

static целое cmd_config_get(целое argc, const char **argv, const char *prefix,
			  struct repository *repo UNUSED)
{
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	struct config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	const char *value_pattern = NULL, *url = NULL;
	целое flags = 0;
	unsigned get_value_flags = 0;
	struct option opts[] = {
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
	целое ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	если ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	если (display_opts.default_value &&
	    ((get_value_flags & GET_VALUE_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	если (url && ((get_value_flags & GET_VALUE_ALL) ||
		    (get_value_flags & GET_VALUE_KEY_REGEXP) ||
		    value_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	если (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	если (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	иначе если (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_value)
		ret = get_color(&location_opts, "", display_opts.default_value);
	иначе
		ret = get_value(&location_opts, &display_opts, argv[0], value_pattern,
				get_value_flags, flags);

	location_options_release(&location_opts);
	вернуть ret;
}

static целое cmd_config_set(целое argc, const char **argv, const char *prefix,
			  struct repository *repo UNUSED)
{
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	const char *value_pattern = NULL, *comment_arg = NULL;
	char *comment = NULL;
	целое flags = 0, append = 0, type = 0;
	struct option opts[] = {
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
	struct key_value_info default_kvi = KVI_INIT;
	char *value;
	целое ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	если (argc == 1)
		die_missing_set_value(argv[0]);
	check_argc(argc, 2, 2);

	если ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	если (append && value_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	если (append)
		value_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	value = normalize_value(argv[0], argv[1], type, &default_kvi);

	если ((flags & CONFIG_FLAGS_MULTI_REPLACE) || value_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, value_pattern,
							      comment, flags);
	} иначе {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, value);
		если (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	free(comment);
	free(value);
	вернуть ret;
}

static целое cmd_config_unset(целое argc, const char **argv, const char *prefix,
			    struct repository *repo UNUSED)
{
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	const char *value_pattern = NULL;
	целое flags = 0;
	struct option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &value_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_VALUE),
		OPT_END(),
	};
	целое ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	если ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	если ((flags & CONFIG_FLAGS_MULTI_REPLACE) || value_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], NULL, value_pattern,
							      NULL, flags);
	иначе
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     NULL, NULL);

	location_options_release(&location_opts);
	вернуть ret;
}

static целое cmd_config_rename_section(целое argc, const char **argv, const char *prefix,
				     struct repository *repo UNUSED)
{
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	struct option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	целое ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	если (ret < 0)
		goto out;
	иначе если (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	вернуть ret;
}

static целое cmd_config_remove_section(целое argc, const char **argv, const char *prefix,
				     struct repository *repo UNUSED)
{
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	struct option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	целое ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], NULL);
	если (ret < 0)
		goto out;
	иначе если (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	вернуть ret;
}

static целое show_editor(struct config_location_options *opts)
{
	char *config_file;

	если (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	если (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	если (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, NULL);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	если (opts->use_global_config) {
		целое fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		если (fd >= 0) {
			char *content = default_user_config();
			write_str_in_full(fd, content);
			free(content);
			close(fd);
		}
		иначе если (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	если (launch_editor(config_file, NULL, NULL)) {
		free(config_file);
		вернуть -1;
	}
	free(config_file);

	вернуть 0;
}

static целое cmd_config_edit(целое argc, const char **argv, const char *prefix,
			   struct repository *repo UNUSED)
{
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	struct option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	целое ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	вернуть ret;
}

static целое cmd_config_actions(целое argc, const char **argv, const char *prefix)
{
	enum {
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
	struct config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	struct config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	const char *comment_arg = NULL;
	целое actions = 0;
	unsigned flags = 0;
	struct option opts[] = {
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
	char *value = NULL, *comment = NULL;
	целое ret = 0;
	целое actions_implicit;
	struct key_value_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	если ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	если (actions_implicit)
		switch (argc) {
		case 1: actions = ACTION_GET; break;
		case 2: actions = ACTION_SET; break;
		case 3: actions = ACTION_SET_ALL; break;
		default:
			error(_("no action specified"));
			exit(129);
		}
	если (actions_implicit && argc == 1) {
		const char *last_dot = strrchr(argv[0], '.');
		если (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_value(argv[0]);
	}
	если (display_opts.omit_values &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	если (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	если (display_opts.default_value && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	если (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	если (flags & CONFIG_FLAGS_FIXED_VALUE) {
		целое allowed_usage = 0;

		switch (actions) {
		/* git config --get <name> <value-pattern> */
		case ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		case ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		case ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		case ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		case ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			break;

		/* git config <name> <value> <value-pattern> */
		case ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		case ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			break;

		/* other options don't allow --fixed-value */
		}

		если (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	если (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	если (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		если (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			если (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			иначе
				die(_("error processing config file(s)"));
		}
	}
	иначе если (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	иначе если (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, value);
		если (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	иначе если (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, argv[2],
							      comment, flags);
	}
	иначе если (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	иначе если (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	иначе если (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	иначе если (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				GET_VALUE_ALL, flags);
	}
	иначе если (actions == ACTION_GET_REGEXP) {
		display_opts.show_keys = 1;
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				GET_VALUE_ALL|GET_VALUE_KEY_REGEXP, flags);
	}
	иначе если (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	иначе если (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		если (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], NULL, argv[1],
								      NULL, flags);
		иначе
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], NULL, NULL);
	}
	иначе если (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], NULL, argv[1],
							      NULL, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	иначе если (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		если (ret < 0)
			goto out;
		иначе если (!ret)
			die(_("no such section: %s"), argv[0]);
		иначе
			ret = 0;
	}
	иначе если (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], NULL);
		если (ret < 0)
			goto out;
		иначе если (!ret)
			die(_("no such section: %s"), argv[0]);
		иначе
			ret = 0;
	}
	иначе если (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	иначе если (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		если (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	free(comment);
	free(value);
	вернуть ret;
}

целое cmd_config(целое argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo)
{
	parse_opt_subcommand_fn *subcommand = NULL;
	struct option subcommand_opts[] = {
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
	если (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		вернуть subcommand(argc, argv, prefix, repo);
	}

	вернуть cmd_config_actions(argc, argv, prefix);
}
