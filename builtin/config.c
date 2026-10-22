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

static const var_na *const builtin_config_usage[] = {
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

static const var_na *const builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	NULL
};

static const var_na *const builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	NULL
};

static const var_na *const builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	NULL
};

static const var_na *const builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	NULL
};

static const var_na *const builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	NULL
};

static const var_na *const builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	NULL
};

static const var_na *const builtin_config_edit_usage[] = {
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

sa_nracanaa config_location_options {
	sa_nracanaa git_config_source source;
	sa_nracanaa config_options options;
	var_na *file_to_free;
	ຈຳນວນເຕັມ use_global_config;
	ຈຳນວນເຕັມ use_system_config;
	ຈຳນວນເຕັມ use_local_config;
	ຈຳນວນເຕັມ use_worktree_config;
	ຈຳນວນເຕັມ respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

ga_nanaa config_type {
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

sa_nracanaa config_display_options {
	ຈຳນວນເຕັມ end_nul;
	ຈຳນວນເຕັມ omit_values;
	ຈຳນວນເຕັມ show_origin;
	ຈຳນວນເຕັມ show_scope;
	ຈຳນວນເຕັມ show_keys;
	ga_nanaa config_type type;
	var_na *default_value;
	/* Populated via `display_options_init()`. */
	ຈຳນວນເຕັມ term;
	ຈຳນວນເຕັມ delim;
	ຈຳນວນເຕັມ key_delim;
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

static ຈຳນວນເຕັມ option_parse_type(const sa_nracanaa option *opt, const var_na *arg,
			     ຈຳນວນເຕັມ unset)
{
	ຈຳນວນເຕັມ new_type, *to_type;

	ຖ້າ (unset) {
		*((ຈຳນວນເຕັມ *) opt->value) = 0;
		ສົ່ງຄືນ 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	ຖ້າ (!new_type) {
		ຖ້າ (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		ບໍ່ດັ່ງນັ້ນ
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->value;
	ຖ້າ (*to_type && *to_type != new_type) {
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

	ສົ່ງຄືນ 0;
}

static void check_argc(ຈຳນວນເຕັມ argc, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max)
{
	ຖ້າ (argc >= min && argc <= max)
		ສົ່ງຄືນ;
	ຖ້າ (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	ບໍ່ດັ່ງນັ້ນ
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

static NORETURN void die_missing_set_value(const var_na *arg)
{
	const var_na *last_dot = strrchr(arg, '.');
	const var_na *eq = last_dot ? strchr(last_dot + 1, '=') : NULL;
	var_na *prefix = eq ? xstrndup(arg, eq - arg) : NULL;

	ຖ້າ (prefix && git_config_key_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (git_config_key_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} ບໍ່ດັ່ງນັ້ນ {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	free(prefix);
	exit(129);
}

static void show_config_origin(const sa_nracanaa config_display_options *opts,
			       const sa_nracanaa key_value_info *kvi,
			       sa_nracanaa strbuf *buf)
{
	const var_na term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	ຖ້າ (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	ບໍ່ດັ່ງນັ້ນ
		quote_c_style(kvi->filename ? kvi->filename : "", buf, NULL, 0);
	strbuf_addch(buf, term);
}

static void show_config_scope(const sa_nracanaa config_display_options *opts,
			      const sa_nracanaa key_value_info *kvi,
			      sa_nracanaa strbuf *buf)
{
	const var_na term = opts->end_nul ? '\0' : '\t';
	const var_na *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

sa_nracanaa strbuf_list {
	sa_nracanaa strbuf *items;
	ຈຳນວນເຕັມ nr;
	ຈຳນວນເຕັມ alloc;
};

static ຈຳນວນເຕັມ format_config_int64(sa_nracanaa strbuf *buf,
			       const var_na *key_,
			       const var_na *value_,
			       const sa_nracanaa key_value_info *kvi,
			       ຈຳນວນເຕັມ gently)
{
	int64_t v = 0;
	ຖ້າ (gently) {
		ຖ້າ (!git_parse_int64(value_, &v))
			ສົ່ງຄືນ -1;
	} ບໍ່ດັ່ງນັ້ນ {
		/* may die() */
		v = git_config_int64(key_, value_ ? value_ : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ format_config_bool(sa_nracanaa strbuf *buf,
			      const var_na *key_,
			      const var_na *value_,
			      ຈຳນວນເຕັມ gently)
{
	ຈຳນວນເຕັມ v = 0;
	ຖ້າ (gently) {
		ຖ້າ ((v = git_parse_maybe_bool(value_)) < 0)
			ສົ່ງຄືນ -1;
	} ບໍ່ດັ່ງນັ້ນ {
		/* may die() */
		v = git_config_bool(key_, value_);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ format_config_bool_or_int(sa_nracanaa strbuf *buf,
				     const var_na *key_,
				     const var_na *value_,
				     const sa_nracanaa key_value_info *kvi,
				     ຈຳນວນເຕັມ gently)
{
	ຈຳນວນເຕັມ v, is_bool = 0;

	ຖ້າ (gently) {
		v = git_parse_maybe_bool_text(value_);

		ຖ້າ (v >= 0)
			is_bool = 1;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!git_parse_int(value_, &v))
			ສົ່ງຄືນ -1;
	} ບໍ່ດັ່ງນັ້ນ {
		v = git_config_bool_or_int(key_, value_, kvi,
					   &is_bool);
	}

	ຖ້າ (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	ບໍ່ດັ່ງນັ້ນ
		strbuf_addf(buf, "%d", v);

	ສົ່ງຄືນ 0;
}

/* This mode is always gentle. */
static ຈຳນວນເຕັມ format_config_bool_or_str(sa_nracanaa strbuf *buf,
				     const var_na *value_)
{
	ຈຳນວນເຕັມ v = git_parse_maybe_bool(value_);
	ຖ້າ (v < 0)
		strbuf_addstr(buf, value_);
	ບໍ່ດັ່ງນັ້ນ
		strbuf_addstr(buf, v ? "true" : "false");
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ format_config_path(sa_nracanaa strbuf *buf,
			      const var_na *key_,
			      const var_na *value_,
			      ຈຳນວນເຕັມ gently)
{
	var_na *v;

	ຖ້າ (git_config_pathname(&v, key_, value_) < 0)
		ສົ່ງຄືນ -1;

	ຖ້າ (v)
		strbuf_addstr(buf, v);
	ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ gently ? -1 : 1; /* :(optional)no-such-file */

	free(v);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ format_config_expiry_date(sa_nracanaa strbuf *buf,
				     const var_na *key_,
				     const var_na *value_,
				     ຈຳນວນເຕັມ quietly)
{
	timestamp_t t;
	ຖ້າ (quietly) {
		ຖ້າ (parse_expiry_date(value_, &t))
			ສົ່ງຄືນ -1;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (git_config_expiry_date(&t, key_, value_) < 0) {
		ສົ່ງຄືນ -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ format_config_color(sa_nracanaa strbuf *buf,
			       const var_na *key_,
			       const var_na *value_,
			       ຈຳນວນເຕັມ gently)
{
	var_na v[COLOR_MAXLEN];

	ຖ້າ (gently) {
		ຖ້າ (color_parse_quietly(value_, v) < 0)
			ສົ່ງຄືນ -1;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (git_config_color(v, key_, value_) < 0) {
		ສົ່ງຄືນ -1;
	}

	strbuf_addstr(buf, v);
	ສົ່ງຄືນ 0;
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
static ຈຳນວນເຕັມ format_config(const sa_nracanaa config_display_options *opts,
			 sa_nracanaa strbuf *buf, const var_na *key_,
			 const var_na *value_, const sa_nracanaa key_value_info *kvi,
			 ຈຳນວນເຕັມ gently)
{
	ຈຳນວນເຕັມ res = 0;
	ຖ້າ (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	ຖ້າ (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	ຖ້າ (opts->show_keys)
		strbuf_addstr(buf, key_);

	ຖ້າ (opts->omit_values)
		goto terminator;

	ຖ້າ (opts->show_keys)
		strbuf_addch(buf, opts->key_delim);

	switch (opts->type) {
	pak_sa TYPE_INT:
		res = format_config_int64(buf, key_, value_, kvi, gently);
		bha_nga;

	pak_sa TYPE_BOOL:
		res = format_config_bool(buf, key_, value_, gently);
		bha_nga;

	pak_sa TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, key_, value_, kvi, gently);
		bha_nga;

	pak_sa TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, value_);
		bha_nga;

	pak_sa TYPE_PATH:
		res = format_config_path(buf, key_, value_, gently);
		bha_nga;

	pak_sa TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, key_, value_, gently);
		bha_nga;

	pak_sa TYPE_COLOR:
		res = format_config_color(buf, key_, value_, gently);
		bha_nga;

	pak_sa TYPE_NONE:
		ຖ້າ (value_) {
			strbuf_addstr(buf, value_);
		} ບໍ່ດັ່ງນັ້ນ {
			/* Just show the key name; back out delimiter */
			ຖ້າ (opts->show_keys)
				strbuf_setlen(buf, buf->len - 1);
		}
		bha_nga;

	default:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ show_all_config(const var_na *key_, const var_na *value_,
			   const sa_nracanaa config_context *ctx,
			   void *cb)
{
	const sa_nracanaa config_display_options *opts = cb;
	const sa_nracanaa key_value_info *kvi = ctx->kvi;
	sa_nracanaa strbuf formatted = STRBUF_INIT;

	ຖ້າ (format_config(opts, &formatted, key_, value_, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	ສົ່ງຄືນ 0;
}

#define GET_VALUE_ALL        (1 << 0)
#define GET_VALUE_KEY_REGEXP (1 << 1)

sa_nracanaa collect_config_data {
	const sa_nracanaa config_display_options *display_opts;
	sa_nracanaa strbuf_list *values;
	const var_na *value_pattern;
	const var_na *key;
	regex_t *regexp;
	regex_t *key_regexp;
	ຈຳນວນເຕັມ do_not_match;
	unsigned get_value_flags;
	unsigned flags;
};

static ຈຳນວນເຕັມ collect_config(const var_na *key_, const var_na *value_,
			  const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa collect_config_data *data = cb;
	sa_nracanaa strbuf_list *values = data->values;
	const sa_nracanaa key_value_info *kvi = ctx->kvi;
	ຈຳນວນເຕັມ status;

	ຖ້າ (!(data->get_value_flags & GET_VALUE_KEY_REGEXP) &&
	    strcmp(key_, data->key))
		ສົ່ງຄືນ 0;
	ຖ້າ ((data->get_value_flags & GET_VALUE_KEY_REGEXP) &&
	    regexec(data->key_regexp, key_, 0, NULL, 0))
		ສົ່ງຄືນ 0;
	ຖ້າ ((data->flags & CONFIG_FLAGS_FIXED_VALUE) &&
	    strcmp(data->value_pattern, (value_?value_:"")))
		ສົ່ງຄືນ 0;
	ຖ້າ (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (value_?value_:""), 0, NULL, 0)))
		ສົ່ງຄືນ 0;

	ALLOC_GROW(values->items, values->nr + 1, values->alloc);
	strbuf_init(&values->items[values->nr], 0);

	status = format_config(data->display_opts, &values->items[values->nr++],
			       key_, value_, kvi, 0);
	ຖ້າ (status < 0)
		ສົ່ງຄືນ status;
	ຖ້າ (status) {
		strbuf_release(&values->items[--values->nr]);
		status = 0;
	}
	ສົ່ງຄືນ status;
}

static ຈຳນວນເຕັມ get_value(const sa_nracanaa config_location_options *opts,
		     const sa_nracanaa config_display_options *display_opts,
		     const var_na *key_, const var_na *regex_,
		     unsigned get_value_flags, unsigned flags)
{
	ຈຳນວນເຕັມ ret = CONFIG_GENERIC_ERROR;
	sa_nracanaa strbuf_list values = {NULL};
	sa_nracanaa collect_config_data data = {
		.display_opts = display_opts,
		.values = &values,
		.get_value_flags = get_value_flags,
		.flags = flags,
	};
	var_na *key = NULL;
	ຈຳນວນເຕັມ i;

	ຖ້າ (get_value_flags & GET_VALUE_KEY_REGEXP) {
		var_na *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		key = xstrdup(key_);
		ສຳລັບ (tl = key + strlen(key) - 1;
		     tl >= key && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		ສຳລັບ (tl = key; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.key_regexp = (regex_t*)xmalloc(parimaa_na(regex_t));
		ຖ້າ (regcomp(data.key_regexp, key, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), key_);
			FREE_AND_NULL(data.key_regexp);
			ret = CONFIG_INVALID_PATTERN;
			goto free_strings;
		}
	} ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (git_config_parse_key(key_, &key, NULL)) {
			ret = CONFIG_INVALID_KEY;
			goto free_strings;
		}

		data.key = key;
	}

	ຖ້າ (regex_ && (flags & CONFIG_FLAGS_FIXED_VALUE))
		data.value_pattern = regex_;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (regex_) {
		ຖ້າ (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(parimaa_na(regex_t));
		ຖ້າ (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			goto free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ຖ້າ (!values.nr && display_opts->default_value) {
		sa_nracanaa key_value_info kvi = KVI_INIT;
		sa_nracanaa strbuf *item;
		ຈຳນວນເຕັມ status;

		kvi_from_param(&kvi);
		ALLOC_GROW(values.items, values.nr + 1, values.alloc);
		item = &values.items[values.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, key_,
				       display_opts->default_value, &kvi, 0);
		ຖ້າ (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_value);
		ຖ້າ (status) {
			/* default was a missing optional value */
			values.nr--;
			strbuf_release(item);
		}
	}

	ret = !values.nr;

	ສຳລັບ (i = 0; i < values.nr; i++) {
		sa_nracanaa strbuf *buf = values.items + i;
		ຖ້າ ((get_value_flags & GET_VALUE_ALL) || i == values.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	free(values.items);

free_strings:
	free(key);
	ຖ້າ (data.key_regexp) {
		regfree(data.key_regexp);
		free(data.key_regexp);
	}
	ຖ້າ (data.regexp) {
		regfree(data.regexp);
		free(data.regexp);
	}

	ສົ່ງຄືນ ret;
}

static var_na *normalize_value(const var_na *key, const var_na *value,
			     ຈຳນວນເຕັມ type, sa_nracanaa key_value_info *kvi)
{
	ຖ້າ (!value)
		ສົ່ງຄືນ NULL;

	ຖ້າ (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		ສົ່ງຄືນ xstrdup(value);
	ຖ້າ (type == TYPE_INT)
		ສົ່ງຄືນ xstrfmt("%"PRId64, git_config_int64(key, value, kvi));
	ຖ້າ (type == TYPE_BOOL)
		ສົ່ງຄືນ xstrdup(git_config_bool(key, value) ?  "true" : "false");
	ຖ້າ (type == TYPE_BOOL_OR_INT) {
		ຈຳນວນເຕັມ is_bool, v;
		v = git_config_bool_or_int(key, value, kvi, &is_bool);
		ຖ້າ (!is_bool)
			ສົ່ງຄືນ xstrfmt("%d", v);
		ບໍ່ດັ່ງນັ້ນ
			ສົ່ງຄືນ xstrdup(v ? "true" : "false");
	}
	ຖ້າ (type == TYPE_BOOL_OR_STR) {
		ຈຳນວນເຕັມ v = git_parse_maybe_bool(value);
		ຖ້າ (v < 0)
			ສົ່ງຄືນ xstrdup(value);
		ບໍ່ດັ່ງນັ້ນ
			ສົ່ງຄືນ xstrdup(v ? "true" : "false");
	}
	ຖ້າ (type == TYPE_COLOR) {
		var_na v[COLOR_MAXLEN];
		ຖ້າ (git_config_color(v, key, value))
			die(_("cannot parse color '%s'"), value);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		ສົ່ງຄືນ xstrdup(value);
	}

	BUG("cannot normalize type %d", type);
}

sa_nracanaa get_color_config_data {
	ຈຳນວນເຕັມ get_color_found;
	const var_na *get_color_slot;
	var_na parsed_color[COLOR_MAXLEN];
};

static ຈຳນວນເຕັມ git_get_color_config(const var_na *var, const var_na *value,
				const sa_nracanaa config_context *ctx UNUSED,
				void *cb)
{
	sa_nracanaa get_color_config_data *data = cb;

	ຖ້າ (!strcmp(var, data->get_color_slot)) {
		ຖ້າ (!value)
			config_error_nonbool(var);
		ຖ້າ (color_parse(value, data->parsed_color) < 0)
			ສົ່ງຄືນ -1;
		data->get_color_found = 1;
	}
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ get_color(const sa_nracanaa config_location_options *opts,
		      const var_na *var, const var_na *def_color)
{
	sa_nracanaa get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	ຈຳນວນເຕັມ ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ຖ້າ (!data.get_color_found && def_color) {
		ຖ້າ (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			goto out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	ສົ່ງຄືນ ret;
}

sa_nracanaa get_colorbool_config_data {
	ga_nanaa git_colorbool get_colorbool_found;
	ga_nanaa git_colorbool get_diff_color_found;
	ga_nanaa git_colorbool get_color_ui_found;
	const var_na *get_colorbool_slot;
};

static ຈຳນວນເຕັມ git_get_colorbool_config(const var_na *var, const var_na *value,
				    const sa_nracanaa config_context *ctx UNUSED,
				    void *cb)
{
	sa_nracanaa get_colorbool_config_data *data = cb;

	ຖ້າ (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, value);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, value);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, value);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ get_colorbool(const sa_nracanaa config_location_options *opts,
			 const var_na *var, ຈຳນວນເຕັມ print)
{
	sa_nracanaa get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	ຖ້າ (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		ຖ້າ (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		ຖ້າ (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	ຖ້າ (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	ຖ້າ (print) {
		printf("%s\n", result ? "true" : "false");
		ສົ່ງຄືນ 0;
	} ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ result ? 0 : 1;
}

static void check_write(const sa_nracanaa git_config_source *source)
{
	ຖ້າ (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	ຖ້າ (source->use_stdin)
		die(_("writing to stdin is not supported"));

	ຖ້າ (source->blob)
		die(_("writing config blobs is not supported"));
}

sa_nracanaa urlmatch_current_candidate_value {
	var_na value_is_null;
	sa_nracanaa strbuf value;
	sa_nracanaa key_value_info kvi;
};

static ຈຳນວນເຕັມ urlmatch_collect_fn(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx,
			       void *cb)
{
	sa_nracanaa string_list *values = cb;
	sa_nracanaa string_list_item *item = string_list_insert(values, var);
	sa_nracanaa urlmatch_current_candidate_value *matched = item->util;
	const sa_nracanaa key_value_info *kvi = ctx->kvi;

	ຖ້າ (!matched) {
		matched = xmalloc(parimaa_na(*matched));
		strbuf_init(&matched->value, 0);
		item->util = matched;
	} ບໍ່ດັ່ງນັ້ນ {
		strbuf_reset(&matched->value);
	}
	matched->kvi = *kvi;

	ຖ້າ (value) {
		strbuf_addstr(&matched->value, value);
		matched->value_is_null = 0;
	} ບໍ່ດັ່ງນັ້ນ {
		matched->value_is_null = 1;
	}
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ get_urlmatch(const sa_nracanaa config_location_options *opts,
			const sa_nracanaa config_display_options *_display_opts,
			const var_na *var, const var_na *url)
{
	ຈຳນວນເຕັມ ret;
	var_na *section;
	var_na *section_tail;
	sa_nracanaa config_display_options display_opts = *_display_opts;
	sa_nracanaa string_list_item *item;
	sa_nracanaa urlmatch_config config = URLMATCH_CONFIG_INIT;
	sa_nracanaa string_list values = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = NULL;
	config.cb = &values;

	ຖ້າ (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	ຖ້າ (section_tail) {
		*section_tail = '\0';
		config.key = section_tail + 1;
		display_opts.show_keys = 0;
	} ບໍ່ດັ່ງນັ້ນ {
		config.key = NULL;
		display_opts.show_keys = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !values.nr;

	for_each_string_list_item(item, &values) {
		sa_nracanaa urlmatch_current_candidate_value *matched = item->util;
		sa_nracanaa strbuf buf = STRBUF_INIT;
		ຈຳນວນເຕັມ status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->value_is_null ? NULL : matched->value.buf,
				       &matched->kvi, 0);
		ຖ້າ (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->value);
	}
	urlmatch_config_release(&config);
	string_list_clear(&values, 1);
	free(config.url.url);

	free(section);
	ສົ່ງຄືນ ret;
}

static var_na *default_user_config(void)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	ສົ່ງຄືນ strbuf_detach(&buf, NULL);
}

static void location_options_init(sa_nracanaa config_location_options *opts,
				  const var_na *prefix)
{
	ຖ້າ (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	ຖ້າ (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	ຖ້າ (!startup_info->have_repository) {
		ຖ້າ (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		ຖ້າ (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		ຖ້າ (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	ຖ້າ (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = NULL;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	ຖ້າ (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		ຖ້າ (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (opts->use_worktree_config) {
		sa_nracanaa worktree **worktrees = get_worktrees(the_repository);
		ຖ້າ (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		ບໍ່ດັ່ງນັ້ນ
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (opts->source.file) {
		ຖ້າ (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	ຖ້າ (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	ບໍ່ດັ່ງນັ້ນ
		opts->options.respect_includes = opts->respect_includes_opt;
	ຖ້າ (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

static void location_options_release(sa_nracanaa config_location_options *opts)
{
	free(opts->file_to_free);
}

static void display_options_init(sa_nracanaa config_display_options *opts)
{
	ຖ້າ (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->key_delim = '\n';
	}
}

static void display_options_init_list(sa_nracanaa config_display_options *opts)
{
	opts->show_keys = 1;

	ຖ້າ (opts->end_nul) {
		display_options_init(opts);
	} ບໍ່ດັ່ງນັ້ນ {
		opts->term = '\n';
		opts->delim = ' ';
		opts->key_delim = '=';
	}
}

static ຈຳນວນເຕັມ cmd_config_list(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	sa_nracanaa config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	sa_nracanaa option opts[] = {
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

	ຖ້າ (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		ຖ້າ (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		ບໍ່ດັ່ງນັ້ນ
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ cmd_config_get(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
			  sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	sa_nracanaa config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	const var_na *value_pattern = NULL, *url = NULL;
	ຈຳນວນເຕັມ flags = 0;
	unsigned get_value_flags = 0;
	sa_nracanaa option opts[] = {
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
	ຈຳນວນເຕັມ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ຖ້າ ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	ຖ້າ (display_opts.default_value &&
	    ((get_value_flags & GET_VALUE_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	ຖ້າ (url && ((get_value_flags & GET_VALUE_ALL) ||
		    (get_value_flags & GET_VALUE_KEY_REGEXP) ||
		    value_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	ຖ້າ (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	ຖ້າ (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_value)
		ret = get_color(&location_opts, "", display_opts.default_value);
	ບໍ່ດັ່ງນັ້ນ
		ret = get_value(&location_opts, &display_opts, argv[0], value_pattern,
				get_value_flags, flags);

	location_options_release(&location_opts);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ cmd_config_set(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
			  sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	const var_na *value_pattern = NULL, *comment_arg = NULL;
	var_na *comment = NULL;
	ຈຳນວນເຕັມ flags = 0, append = 0, type = 0;
	sa_nracanaa option opts[] = {
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
	sa_nracanaa key_value_info default_kvi = KVI_INIT;
	var_na *value;
	ຈຳນວນເຕັມ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ຖ້າ (argc == 1)
		die_missing_set_value(argv[0]);
	check_argc(argc, 2, 2);

	ຖ້າ ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	ຖ້າ (append && value_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	ຖ້າ (append)
		value_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	value = normalize_value(argv[0], argv[1], type, &default_kvi);

	ຖ້າ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || value_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, value_pattern,
							      comment, flags);
	} ບໍ່ດັ່ງນັ້ນ {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, value);
		ຖ້າ (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	free(comment);
	free(value);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ cmd_config_unset(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
			    sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	const var_na *value_pattern = NULL;
	ຈຳນວນເຕັມ flags = 0;
	sa_nracanaa option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &value_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_VALUE),
		OPT_END(),
	};
	ຈຳນວນເຕັມ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ຖ້າ ((flags & CONFIG_FLAGS_FIXED_VALUE) && !value_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ຖ້າ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || value_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], NULL, value_pattern,
							      NULL, flags);
	ບໍ່ດັ່ງນັ້ນ
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     NULL, NULL);

	location_options_release(&location_opts);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ cmd_config_rename_section(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
				     sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	sa_nracanaa option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ຈຳນວນເຕັມ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	ຖ້າ (ret < 0)
		goto out;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ cmd_config_remove_section(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
				     sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	sa_nracanaa option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ຈຳນວນເຕັມ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], NULL);
	ຖ້າ (ret < 0)
		goto out;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ show_editor(sa_nracanaa config_location_options *opts)
{
	var_na *config_file;

	ຖ້າ (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	ຖ້າ (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	ຖ້າ (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, NULL);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	ຖ້າ (opts->use_global_config) {
		ຈຳນວນເຕັມ fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		ຖ້າ (fd >= 0) {
			var_na *content = default_user_config();
			write_str_in_full(fd, content);
			free(content);
			close(fd);
		}
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	ຖ້າ (launch_editor(config_file, NULL, NULL)) {
		free(config_file);
		ສົ່ງຄືນ -1;
	}
	free(config_file);

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ cmd_config_edit(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	sa_nracanaa option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	ຈຳນວນເຕັມ ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ cmd_config_actions(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix)
{
	ga_nanaa {
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
	sa_nracanaa config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	sa_nracanaa config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	const var_na *comment_arg = NULL;
	ຈຳນວນເຕັມ actions = 0;
	unsigned flags = 0;
	sa_nracanaa option opts[] = {
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
	var_na *value = NULL, *comment = NULL;
	ຈຳນວນເຕັມ ret = 0;
	ຈຳນວນເຕັມ actions_implicit;
	sa_nracanaa key_value_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	ຖ້າ ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	ຖ້າ (actions_implicit)
		switch (argc) {
		pak_sa 1: actions = ACTION_GET; bha_nga;
		pak_sa 2: actions = ACTION_SET; bha_nga;
		pak_sa 3: actions = ACTION_SET_ALL; bha_nga;
		default:
			error(_("no action specified"));
			exit(129);
		}
	ຖ້າ (actions_implicit && argc == 1) {
		const var_na *last_dot = strrchr(argv[0], '.');
		ຖ້າ (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_value(argv[0]);
	}
	ຖ້າ (display_opts.omit_values &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	ຖ້າ (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	ຖ້າ (display_opts.default_value && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	ຖ້າ (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	ຖ້າ (flags & CONFIG_FLAGS_FIXED_VALUE) {
		ຈຳນວນເຕັມ allowed_usage = 0;

		switch (actions) {
		/* git config --get <name> <value-pattern> */
		pak_sa ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		pak_sa ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		pak_sa ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		pak_sa ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		pak_sa ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			bha_nga;

		/* git config <name> <value> <value-pattern> */
		pak_sa ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		pak_sa ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			bha_nga;

		/* other options don't allow --fixed-value */
		}

		ຖ້າ (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	ຖ້າ (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	ຖ້າ (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		ຖ້າ (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			ຖ້າ (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			ບໍ່ດັ່ງນັ້ນ
				die(_("error processing config file(s)"));
		}
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, value);
		ຖ້າ (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, argv[2],
							      comment, flags);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		value = normalize_value(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], value, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				GET_VALUE_ALL, flags);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_GET_REGEXP) {
		display_opts.show_keys = 1;
		check_argc(argc, 1, 2);
		ret = get_value(&location_opts, &display_opts, argv[0], argv[1],
				GET_VALUE_ALL|GET_VALUE_KEY_REGEXP, flags);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ຖ້າ (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], NULL, argv[1],
								      NULL, flags);
		ບໍ່ດັ່ງນັ້ນ
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], NULL, NULL);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], NULL, argv[1],
							      NULL, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		ຖ້າ (ret < 0)
			goto out;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!ret)
			die(_("no such section: %s"), argv[0]);
		ບໍ່ດັ່ງນັ້ນ
			ret = 0;
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], NULL);
		ຖ້າ (ret < 0)
			goto out;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!ret)
			die(_("no such section: %s"), argv[0]);
		ບໍ່ດັ່ງນັ້ນ
			ret = 0;
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		ຖ້າ (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	free(comment);
	free(value);
	ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ cmd_config(ຈຳນວນເຕັມ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *subcommand = NULL;
	sa_nracanaa option subcommand_opts[] = {
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
	ຖ້າ (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		ສົ່ງຄືນ subcommand(argc, argv, prefix, repo);
	}

	ສົ່ງຄືນ cmd_config_actions(argc, argv, prefix);
}
