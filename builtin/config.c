#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "date.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "ident.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "urlmatch.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "worktree.h"

સ્થિર અચલ અક્ષર *અચલ builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ખાલી
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

સંરચના config_location_options {
	સંરચના git_config_source source;
	સંરચના config_options options;
	અક્ષર *file_to_free;
	પૂર્ણાંક use_global_config;
	પૂર્ણાંક use_system_config;
	પૂર્ણાંક use_local_config;
	પૂર્ણાંક use_worktree_config;
	પૂર્ણાંક respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

ગણના config_type {
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
	OPT_CALLBACK_કિંમત(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_કિંમત(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_કિંમત(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_કિંમત(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_કિંમત(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_કિંમત(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_કિંમતો, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_ચાવીઓ, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

સંરચના config_display_options {
	પૂર્ણાંક end_nul;
	પૂર્ણાંક omit_કિંમતો;
	પૂર્ણાંક show_origin;
	પૂર્ણાંક show_scope;
	પૂર્ણાંક show_ચાવીઓ;
	ગણના config_type type;
	અક્ષર *default_કિંમત;
	/* Populated via `display_options_init()`. */
	પૂર્ણાંક term;
	પૂર્ણાંક delim;
	પૂર્ણાંક ચાવી_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.ચાવી_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_કિંમત(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.કિંમત = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

સ્થિર પૂર્ણાંક option_parse_type(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			     પૂર્ણાંક unset)
{
	પૂર્ણાંક new_type, *to_type;

	જો (unset) {
		*((પૂર્ણાંક *) opt->કિંમત) = 0;
		પાછા 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	જો (!new_type) {
		જો (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		નહીંતર જો (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		નહીંતર જો (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		નહીંતર જો (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		નહીંતર જો (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		નહીંતર જો (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		નહીંતર જો (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		નહીંતર
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->કિંમત;
	જો (*to_type && *to_type != new_type) {
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

	પાછા 0;
}

સ્થિર શૂન્ય check_argc(પૂર્ણાંક argc, પૂર્ણાંક min, પૂર્ણાંક max)
{
	જો (argc >= min && argc <= max)
		પાછા;
	જો (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	નહીંતર
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

સ્થિર NORETURN શૂન્ય die_missing_set_કિંમત(અચલ અક્ષર *arg)
{
	અચલ અક્ષર *last_dot = strrchr(arg, '.');
	અચલ અક્ષર *eq = last_dot ? strchr(last_dot + 1, '=') : ખાલી;
	અક્ષર *prefix = eq ? xstrndup(arg, eq - arg) : ખાલી;

	જો (prefix && git_config_ચાવી_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} નહીંતર જો (git_config_ચાવી_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} નહીંતર {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	મુક્ત(prefix);
	exit(129);
}

સ્થિર શૂન્ય show_config_origin(અચલ સંરચના config_display_options *opts,
			       અચલ સંરચના ચાવી_કિંમત_info *kvi,
			       સંરચના strbuf *buf)
{
	અચલ અક્ષર term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	જો (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	નહીંતર
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ખાલી, 0);
	strbuf_addch(buf, term);
}

સ્થિર શૂન્ય show_config_scope(અચલ સંરચના config_display_options *opts,
			      અચલ સંરચના ચાવી_કિંમત_info *kvi,
			      સંરચના strbuf *buf)
{
	અચલ અક્ષર term = opts->end_nul ? '\0' : '\t';
	અચલ અક્ષર *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

સંરચના strbuf_list {
	સંરચના strbuf *વસ્તુઓ;
	પૂર્ણાંક nr;
	પૂર્ણાંક alloc;
};

સ્થિર પૂર્ણાંક format_config_int64(સંરચના strbuf *buf,
			       અચલ અક્ષર *ચાવી_2,
			       અચલ અક્ષર *કિંમત_2,
			       અચલ સંરચના ચાવી_કિંમત_info *kvi,
			       પૂર્ણાંક gently)
{
	int64_t v = 0;
	જો (gently) {
		જો (!git_parse_int64(કિંમત_2, &v))
			પાછા -1;
	} નહીંતર {
		/* may die() */
		v = git_config_int64(ચાવી_2, કિંમત_2 ? કિંમત_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	પાછા 0;
}

સ્થિર પૂર્ણાંક format_config_bool(સંરચના strbuf *buf,
			      અચલ અક્ષર *ચાવી_2,
			      અચલ અક્ષર *કિંમત_2,
			      પૂર્ણાંક gently)
{
	પૂર્ણાંક v = 0;
	જો (gently) {
		જો ((v = git_parse_maybe_bool(કિંમત_2)) < 0)
			પાછા -1;
	} નહીંતર {
		/* may die() */
		v = git_config_bool(ચાવી_2, કિંમત_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	પાછા 0;
}

સ્થિર પૂર્ણાંક format_config_bool_or_int(સંરચના strbuf *buf,
				     અચલ અક્ષર *ચાવી_2,
				     અચલ અક્ષર *કિંમત_2,
				     અચલ સંરચના ચાવી_કિંમત_info *kvi,
				     પૂર્ણાંક gently)
{
	પૂર્ણાંક v, is_bool = 0;

	જો (gently) {
		v = git_parse_maybe_bool_text(કિંમત_2);

		જો (v >= 0)
			is_bool = 1;
		નહીંતર જો (!git_parse_int(કિંમત_2, &v))
			પાછા -1;
	} નહીંતર {
		v = git_config_bool_or_int(ચાવી_2, કિંમત_2, kvi,
					   &is_bool);
	}

	જો (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	નહીંતર
		strbuf_addf(buf, "%d", v);

	પાછા 0;
}

/* This mode is always gentle. */
સ્થિર પૂર્ણાંક format_config_bool_or_str(સંરચના strbuf *buf,
				     અચલ અક્ષર *કિંમત_2)
{
	પૂર્ણાંક v = git_parse_maybe_bool(કિંમત_2);
	જો (v < 0)
		strbuf_addstr(buf, કિંમત_2);
	નહીંતર
		strbuf_addstr(buf, v ? "true" : "false");
	પાછા 0;
}

સ્થિર પૂર્ણાંક format_config_path(સંરચના strbuf *buf,
			      અચલ અક્ષર *ચાવી_2,
			      અચલ અક્ષર *કિંમત_2,
			      પૂર્ણાંક gently)
{
	અક્ષર *v;

	જો (git_config_pathname(&v, ચાવી_2, કિંમત_2) < 0)
		પાછા -1;

	જો (v)
		strbuf_addstr(buf, v);
	નહીંતર
		પાછા gently ? -1 : 1; /* :(optional)no-such-file */

	મુક્ત(v);
	પાછા 0;
}

સ્થિર પૂર્ણાંક format_config_expiry_date(સંરચના strbuf *buf,
				     અચલ અક્ષર *ચાવી_2,
				     અચલ અક્ષર *કિંમત_2,
				     પૂર્ણાંક quietly)
{
	timestamp_t t;
	જો (quietly) {
		જો (parse_expiry_date(કિંમત_2, &t))
			પાછા -1;
	} નહીંતર જો (git_config_expiry_date(&t, ચાવી_2, કિંમત_2) < 0) {
		પાછા -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	પાછા 0;
}

સ્થિર પૂર્ણાંક format_config_color(સંરચના strbuf *buf,
			       અચલ અક્ષર *ચાવી_2,
			       અચલ અક્ષર *કિંમત_2,
			       પૂર્ણાંક gently)
{
	અક્ષર v[COLOR_MAXLEN];

	જો (gently) {
		જો (color_parse_quietly(કિંમત_2, v) < 0)
			પાછા -1;
	} નહીંતર જો (git_config_color(v, ચાવી_2, કિંમત_2) < 0) {
		પાછા -1;
	}

	strbuf_addstr(buf, v);
	પાછા 0;
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
સ્થિર પૂર્ણાંક format_config(અચલ સંરચના config_display_options *opts,
			 સંરચના strbuf *buf, અચલ અક્ષર *ચાવી_2,
			 અચલ અક્ષર *કિંમત_2, અચલ સંરચના ચાવી_કિંમત_info *kvi,
			 પૂર્ણાંક gently)
{
	પૂર્ણાંક res = 0;
	જો (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	જો (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	જો (opts->show_ચાવીઓ)
		strbuf_addstr(buf, ચાવી_2);

	જો (opts->omit_કિંમતો)
		જાઓ terminator;

	જો (opts->show_ચાવીઓ)
		strbuf_addch(buf, opts->ચાવી_delim);

	પસંદગી (opts->type) {
	સ્થિતિ TYPE_INT:
		res = format_config_int64(buf, ચાવી_2, કિંમત_2, kvi, gently);
		તોડો;

	સ્થિતિ TYPE_BOOL:
		res = format_config_bool(buf, ચાવી_2, કિંમત_2, gently);
		તોડો;

	સ્થિતિ TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, ચાવી_2, કિંમત_2, kvi, gently);
		તોડો;

	સ્થિતિ TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, કિંમત_2);
		તોડો;

	સ્થિતિ TYPE_PATH:
		res = format_config_path(buf, ચાવી_2, કિંમત_2, gently);
		તોડો;

	સ્થિતિ TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, ચાવી_2, કિંમત_2, gently);
		તોડો;

	સ્થિતિ TYPE_COLOR:
		res = format_config_color(buf, ચાવી_2, કિંમત_2, gently);
		તોડો;

	સ્થિતિ TYPE_NONE:
		જો (કિંમત_2) {
			strbuf_addstr(buf, કિંમત_2);
		} નહીંતર {
			/* Just show the key name; back out delimiter */
			જો (opts->show_ચાવીઓ)
				strbuf_setlen(buf, buf->len - 1);
		}
		તોડો;

	મૂળભૂત:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	પાછા res;
}

સ્થિર પૂર્ણાંક show_all_config(અચલ અક્ષર *ચાવી_2, અચલ અક્ષર *કિંમત_2,
			   અચલ સંરચના config_context *ctx,
			   શૂન્ય *cb)
{
	અચલ સંરચના config_display_options *opts = cb;
	અચલ સંરચના ચાવી_કિંમત_info *kvi = ctx->kvi;
	સંરચના strbuf formatted = STRBUF_INIT;

	જો (format_config(opts, &formatted, ચાવી_2, કિંમત_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	પાછા 0;
}

#define GET_કિંમત_ALL        (1 << 0)
#define GET_કિંમત_ચાવી_REGEXP (1 << 1)

સંરચના collect_config_data {
	અચલ સંરચના config_display_options *display_opts;
	સંરચના strbuf_list *કિંમતો;
	અચલ અક્ષર *કિંમત_pattern;
	અચલ અક્ષર *ચાવી;
	regex_t *regexp;
	regex_t *ચાવી_regexp;
	પૂર્ણાંક do_not_match;
	unsigned get_કિંમત_flags;
	unsigned flags;
};

સ્થિર પૂર્ણાંક collect_config(અચલ અક્ષર *ચાવી_2, અચલ અક્ષર *કિંમત_2,
			  અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના collect_config_data *data = cb;
	સંરચના strbuf_list *કિંમતો = data->કિંમતો;
	અચલ સંરચના ચાવી_કિંમત_info *kvi = ctx->kvi;
	પૂર્ણાંક status;

	જો (!(data->get_કિંમત_flags & GET_કિંમત_ચાવી_REGEXP) &&
	    strcmp(ચાવી_2, data->ચાવી))
		પાછા 0;
	જો ((data->get_કિંમત_flags & GET_કિંમત_ચાવી_REGEXP) &&
	    regexec(data->ચાવી_regexp, ચાવી_2, 0, ખાલી, 0))
		પાછા 0;
	જો ((data->flags & CONFIG_FLAGS_FIXED_કિંમત) &&
	    strcmp(data->કિંમત_pattern, (કિંમત_2?કિંમત_2:"")))
		પાછા 0;
	જો (data->regexp &&
	    (data->do_not_match ^ !!regexec(data->regexp, (કિંમત_2?કિંમત_2:""), 0, ખાલી, 0)))
		પાછા 0;

	ALLOC_GROW(કિંમતો->વસ્તુઓ, કિંમતો->nr + 1, કિંમતો->alloc);
	strbuf_init(&કિંમતો->વસ્તુઓ[કિંમતો->nr], 0);

	status = format_config(data->display_opts, &કિંમતો->વસ્તુઓ[કિંમતો->nr++],
			       ચાવી_2, કિંમત_2, kvi, 0);
	જો (status < 0)
		પાછા status;
	જો (status) {
		strbuf_release(&કિંમતો->વસ્તુઓ[--કિંમતો->nr]);
		status = 0;
	}
	પાછા status;
}

સ્થિર પૂર્ણાંક get_કિંમત(અચલ સંરચના config_location_options *opts,
		     અચલ સંરચના config_display_options *display_opts,
		     અચલ અક્ષર *ચાવી_2, અચલ અક્ષર *regex_,
		     unsigned get_કિંમત_flags, unsigned flags)
{
	પૂર્ણાંક ret = CONFIG_GENERIC_ERROR;
	સંરચના strbuf_list કિંમતો = {ખાલી};
	સંરચના collect_config_data data = {
		.display_opts = display_opts,
		.કિંમતો = &કિંમતો,
		.get_કિંમત_flags = get_કિંમત_flags,
		.flags = flags,
	};
	અક્ષર *ચાવી = ખાલી;
	પૂર્ણાંક i;

	જો (get_કિંમત_flags & GET_કિંમત_ચાવી_REGEXP) {
		અક્ષર *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		ચાવી = xstrdup(ચાવી_2);
		માટે (tl = ચાવી + strlen(ચાવી) - 1;
		     tl >= ચાવી && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		માટે (tl = ચાવી; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		data.ચાવી_regexp = (regex_t*)xmalloc(કદ(regex_t));
		જો (regcomp(data.ચાવી_regexp, ચાવી, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), ચાવી_2);
			FREE_AND_NULL(data.ચાવી_regexp);
			ret = CONFIG_INVALID_PATTERN;
			જાઓ free_strings;
		}
	} નહીંતર {
		જો (git_config_parse_ચાવી(ચાવી_2, &ચાવી, ખાલી)) {
			ret = CONFIG_INVALID_ચાવી;
			જાઓ free_strings;
		}

		data.ચાવી = ચાવી;
	}

	જો (regex_ && (flags & CONFIG_FLAGS_FIXED_કિંમત))
		data.કિંમત_pattern = regex_;
	નહીંતર જો (regex_) {
		જો (regex_[0] == '!') {
			data.do_not_match = 1;
			regex_++;
		}

		data.regexp = (regex_t*)xmalloc(કદ(regex_t));
		જો (regcomp(data.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(data.regexp);
			ret = CONFIG_INVALID_PATTERN;
			જાઓ free_strings;
		}
	}

	config_with_options(collect_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	જો (!કિંમતો.nr && display_opts->default_કિંમત) {
		સંરચના ચાવી_કિંમત_info kvi = KVI_INIT;
		સંરચના strbuf *item;
		પૂર્ણાંક status;

		kvi_from_param(&kvi);
		ALLOC_GROW(કિંમતો.વસ્તુઓ, કિંમતો.nr + 1, કિંમતો.alloc);
		item = &કિંમતો.વસ્તુઓ[કિંમતો.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, ચાવી_2,
				       display_opts->default_કિંમત, &kvi, 0);
		જો (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_કિંમત);
		જો (status) {
			/* default was a missing optional value */
			કિંમતો.nr--;
			strbuf_release(item);
		}
	}

	ret = !કિંમતો.nr;

	માટે (i = 0; i < કિંમતો.nr; i++) {
		સંરચના strbuf *buf = કિંમતો.વસ્તુઓ + i;
		જો ((get_કિંમત_flags & GET_કિંમત_ALL) || i == કિંમતો.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	મુક્ત(કિંમતો.વસ્તુઓ);

free_strings:
	મુક્ત(ચાવી);
	જો (data.ચાવી_regexp) {
		regfree(data.ચાવી_regexp);
		મુક્ત(data.ચાવી_regexp);
	}
	જો (data.regexp) {
		regfree(data.regexp);
		મુક્ત(data.regexp);
	}

	પાછા ret;
}

સ્થિર અક્ષર *normalize_કિંમત(અચલ અક્ષર *ચાવી, અચલ અક્ષર *કિંમત,
			     પૂર્ણાંક type, સંરચના ચાવી_કિંમત_info *kvi)
{
	જો (!કિંમત)
		પાછા ખાલી;

	જો (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		પાછા xstrdup(કિંમત);
	જો (type == TYPE_INT)
		પાછા xstrfmt("%"PRId64, git_config_int64(ચાવી, કિંમત, kvi));
	જો (type == TYPE_BOOL)
		પાછા xstrdup(git_config_bool(ચાવી, કિંમત) ?  "true" : "false");
	જો (type == TYPE_BOOL_OR_INT) {
		પૂર્ણાંક is_bool, v;
		v = git_config_bool_or_int(ચાવી, કિંમત, kvi, &is_bool);
		જો (!is_bool)
			પાછા xstrfmt("%d", v);
		નહીંતર
			પાછા xstrdup(v ? "true" : "false");
	}
	જો (type == TYPE_BOOL_OR_STR) {
		પૂર્ણાંક v = git_parse_maybe_bool(કિંમત);
		જો (v < 0)
			પાછા xstrdup(કિંમત);
		નહીંતર
			પાછા xstrdup(v ? "true" : "false");
	}
	જો (type == TYPE_COLOR) {
		અક્ષર v[COLOR_MAXLEN];
		જો (git_config_color(v, ચાવી, કિંમત))
			die(_("cannot parse color '%s'"), કિંમત);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		પાછા xstrdup(કિંમત);
	}

	BUG("cannot normalize type %d", type);
}

સંરચના get_color_config_data {
	પૂર્ણાંક get_color_found;
	અચલ અક્ષર *get_color_slot;
	અક્ષર parsed_color[COLOR_MAXLEN];
};

સ્થિર પૂર્ણાંક git_get_color_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
				અચલ સંરચના config_context *ctx UNUSED,
				શૂન્ય *cb)
{
	સંરચના get_color_config_data *data = cb;

	જો (!strcmp(var, data->get_color_slot)) {
		જો (!કિંમત)
			config_error_nonbool(var);
		જો (color_parse(કિંમત, data->parsed_color) < 0)
			પાછા -1;
		data->get_color_found = 1;
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_color(અચલ સંરચના config_location_options *opts,
		      અચલ અક્ષર *var, અચલ અક્ષર *def_color)
{
	સંરચના get_color_config_data data = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	પૂર્ણાંક ret;

	config_with_options(git_get_color_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	જો (!data.get_color_found && def_color) {
		જો (color_parse(def_color, data.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			જાઓ out;
		}
	}

	ret = 0;

out:
	fputs(data.parsed_color, stdout);
	પાછા ret;
}

સંરચના get_colorbool_config_data {
	ગણના git_colorbool get_colorbool_found;
	ગણના git_colorbool get_diff_color_found;
	ગણના git_colorbool get_color_ui_found;
	અચલ અક્ષર *get_colorbool_slot;
};

સ્થિર પૂર્ણાંક git_get_colorbool_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
				    અચલ સંરચના config_context *ctx UNUSED,
				    શૂન્ય *cb)
{
	સંરચના get_colorbool_config_data *data = cb;

	જો (!strcmp(var, data->get_colorbool_slot))
		data->get_colorbool_found = git_config_colorbool(var, કિંમત);
	નહીંતર જો (!strcmp(var, "diff.color"))
		data->get_diff_color_found = git_config_colorbool(var, કિંમત);
	નહીંતર જો (!strcmp(var, "color.ui"))
		data->get_color_ui_found = git_config_colorbool(var, કિંમત);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_colorbool(અચલ સંરચના config_location_options *opts,
			 અચલ અક્ષર *var, પૂર્ણાંક print)
{
	સંરચના get_colorbool_config_data data = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool result;

	config_with_options(git_get_colorbool_config, &data,
			    &opts->source, the_repository,
			    &opts->options);

	જો (data.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		જો (!strcmp(data.get_colorbool_slot, "color.diff"))
			data.get_colorbool_found = data.get_diff_color_found;
		જો (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
			data.get_colorbool_found = data.get_color_ui_found;
	}

	જો (data.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		data.get_colorbool_found = GIT_COLOR_AUTO;

	result = want_color(data.get_colorbool_found);

	જો (print) {
		printf("%s\n", result ? "true" : "false");
		પાછા 0;
	} નહીંતર
		પાછા result ? 0 : 1;
}

સ્થિર શૂન્ય check_write(અચલ સંરચના git_config_source *source)
{
	જો (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	જો (source->use_stdin)
		die(_("writing to stdin is not supported"));

	જો (source->blob)
		die(_("writing config blobs is not supported"));
}

સંરચના urlmatch_current_candidate_કિંમત {
	અક્ષર કિંમત_is_null;
	સંરચના strbuf કિંમત;
	સંરચના ચાવી_કિંમત_info kvi;
};

સ્થિર પૂર્ણાંક urlmatch_collect_fn(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			       અચલ સંરચના config_context *ctx,
			       શૂન્ય *cb)
{
	સંરચના string_list *કિંમતો = cb;
	સંરચના string_list_item *item = string_list_ઉમેરો(કિંમતો, var);
	સંરચના urlmatch_current_candidate_કિંમત *matched = item->util;
	અચલ સંરચના ચાવી_કિંમત_info *kvi = ctx->kvi;

	જો (!matched) {
		matched = xmalloc(કદ(*matched));
		strbuf_init(&matched->કિંમત, 0);
		item->util = matched;
	} નહીંતર {
		strbuf_reset(&matched->કિંમત);
	}
	matched->kvi = *kvi;

	જો (કિંમત) {
		strbuf_addstr(&matched->કિંમત, કિંમત);
		matched->કિંમત_is_null = 0;
	} નહીંતર {
		matched->કિંમત_is_null = 1;
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_urlmatch(અચલ સંરચના config_location_options *opts,
			અચલ સંરચના config_display_options *_display_opts,
			અચલ અક્ષર *var, અચલ અક્ષર *url)
{
	પૂર્ણાંક ret;
	અક્ષર *section;
	અક્ષર *section_tail;
	સંરચના config_display_options display_opts = *_display_opts;
	સંરચના string_list_item *item;
	સંરચના urlmatch_config config = URLMATCH_CONFIG_INIT;
	સંરચના string_list કિંમતો = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ખાલી;
	config.cb = &કિંમતો;

	જો (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_tail = strchr(section, '.');
	જો (section_tail) {
		*section_tail = '\0';
		config.ચાવી = section_tail + 1;
		display_opts.show_ચાવીઓ = 0;
	} નહીંતર {
		config.ચાવી = ખાલી;
		display_opts.show_ચાવીઓ = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !કિંમતો.nr;

	for_each_string_list_item(item, &કિંમતો) {
		સંરચના urlmatch_current_candidate_કિંમત *matched = item->util;
		સંરચના strbuf buf = STRBUF_INIT;
		પૂર્ણાંક status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->કિંમત_is_null ? ખાલી : matched->કિંમત.buf,
				       &matched->kvi, 0);
		જો (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->કિંમત);
	}
	urlmatch_config_release(&config);
	string_list_clear(&કિંમતો, 1);
	મુક્ત(config.url.url);

	મુક્ત(section);
	પાછા ret;
}

સ્થિર અક્ષર *default_user_config(શૂન્ય)
{
	સંરચના strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	પાછા strbuf_detach(&buf, ખાલી);
}

સ્થિર શૂન્ય location_options_init(સંરચના config_location_options *opts,
				  અચલ અક્ષર *prefix)
{
	જો (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	જો (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	જો (!startup_info->have_repository) {
		જો (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		જો (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		જો (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	જો (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = ખાલી;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	જો (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		જો (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} નહીંતર જો (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} નહીંતર જો (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} નહીંતર જો (opts->use_worktree_config) {
		સંરચના worktree **worktrees = get_worktrees(the_repository);
		જો (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		નહીંતર જો (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		નહીંતર
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} નહીંતર જો (opts->source.file) {
		જો (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} નહીંતર જો (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	જો (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	નહીંતર
		opts->options.respect_includes = opts->respect_includes_opt;
	જો (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

સ્થિર શૂન્ય location_options_release(સંરચના config_location_options *opts)
{
	મુક્ત(opts->file_to_free);
}

સ્થિર શૂન્ય display_options_init(સંરચના config_display_options *opts)
{
	જો (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->ચાવી_delim = '\n';
	}
}

સ્થિર શૂન્ય display_options_init_list(સંરચના config_display_options *opts)
{
	opts->show_ચાવીઓ = 1;

	જો (opts->end_nul) {
		display_options_init(opts);
	} નહીંતર {
		opts->term = '\n';
		opts->delim = ' ';
		opts->ચાવી_delim = '=';
	}
}

સ્થિર પૂર્ણાંક cmd_config_list(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	સંરચના config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	સંરચના option opts[] = {
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

	જો (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		જો (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		નહીંતર
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_config_get(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			  સંરચના repository *repo UNUSED)
{
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	સંરચના config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	અચલ અક્ષર *કિંમત_pattern = ખાલી, *url = ખાલી;
	પૂર્ણાંક flags = 0;
	unsigned get_કિંમત_flags = 0;
	સંરચના option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_કિંમત_flags, N_("return all values for multi-valued config options"), GET_કિંમત_ALL),
		OPT_BIT(0, "regexp", &get_કિંમત_flags, N_("interpret the name as a regular expression"), GET_કિંમત_ચાવી_REGEXP),
		OPT_STRING(0, "value", &કિંમત_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_કિંમત),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_કિંમત,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	જો ((flags & CONFIG_FLAGS_FIXED_કિંમત) && !કિંમત_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	જો (display_opts.default_કિંમત &&
	    ((get_કિંમત_flags & GET_કિંમત_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	જો (url && ((get_કિંમત_flags & GET_કિંમત_ALL) ||
		    (get_કિંમત_flags & GET_કિંમત_ચાવી_REGEXP) ||
		    કિંમત_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	જો (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	જો (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	નહીંતર જો (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_કિંમત)
		ret = get_color(&location_opts, "", display_opts.default_કિંમત);
	નહીંતર
		ret = get_કિંમત(&location_opts, &display_opts, argv[0], કિંમત_pattern,
				get_કિંમત_flags, flags);

	location_options_release(&location_opts);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_config_set(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			  સંરચના repository *repo UNUSED)
{
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	અચલ અક્ષર *કિંમત_pattern = ખાલી, *comment_arg = ખાલી;
	અક્ષર *comment = ખાલી;
	પૂર્ણાંક flags = 0, append = 0, type = 0;
	સંરચના option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &કિંમત_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_કિંમત),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	સંરચના ચાવી_કિંમત_info default_kvi = KVI_INIT;
	અક્ષર *કિંમત;
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	જો (argc == 1)
		die_missing_set_કિંમત(argv[0]);
	check_argc(argc, 2, 2);

	જો ((flags & CONFIG_FLAGS_FIXED_કિંમત) && !કિંમત_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	જો (append && કિંમત_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	જો (append)
		કિંમત_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	કિંમત = normalize_કિંમત(argv[0], argv[1], type, &default_kvi);

	જો ((flags & CONFIG_FLAGS_MULTI_REPLACE) || કિંમત_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], કિંમત, કિંમત_pattern,
							      comment, flags);
	} નહીંતર {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, કિંમત);
		જો (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	મુક્ત(comment);
	મુક્ત(કિંમત);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_config_unset(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			    સંરચના repository *repo UNUSED)
{
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	અચલ અક્ષર *કિંમત_pattern = ખાલી;
	પૂર્ણાંક flags = 0;
	સંરચના option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &કિંમત_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_કિંમત),
		OPT_END(),
	};
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	જો ((flags & CONFIG_FLAGS_FIXED_કિંમત) && !કિંમત_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	જો ((flags & CONFIG_FLAGS_MULTI_REPLACE) || કિંમત_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ખાલી, કિંમત_pattern,
							      ખાલી, flags);
	નહીંતર
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     ખાલી, ખાલી);

	location_options_release(&location_opts);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_config_rename_section(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				     સંરચના repository *repo UNUSED)
{
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	સંરચના option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	જો (ret < 0)
		જાઓ out;
	નહીંતર જો (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_config_remove_section(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				     સંરચના repository *repo UNUSED)
{
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	સંરચના option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], ખાલી);
	જો (ret < 0)
		જાઓ out;
	નહીંતર જો (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	પાછા ret;
}

સ્થિર પૂર્ણાંક show_editor(સંરચના config_location_options *opts)
{
	અક્ષર *config_file;

	જો (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	જો (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	જો (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ખાલી);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	જો (opts->use_global_config) {
		પૂર્ણાંક fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		જો (fd >= 0) {
			અક્ષર *content = default_user_config();
			write_str_in_full(fd, content);
			મુક્ત(content);
			close(fd);
		}
		નહીંતર જો (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	જો (launch_editor(config_file, ખાલી, ખાલી)) {
		મુક્ત(config_file);
		પાછા -1;
	}
	મુક્ત(config_file);

	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_config_edit(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	સંરચના option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_config_actions(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix)
{
	ગણના {
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
	સંરચના config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	સંરચના config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	અચલ અક્ષર *comment_arg = ખાલી;
	પૂર્ણાંક actions = 0;
	unsigned flags = 0;
	સંરચના option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_કિંમત,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_કિંમત),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	અક્ષર *કિંમત = ખાલી, *comment = ખાલી;
	પૂર્ણાંક ret = 0;
	પૂર્ણાંક actions_implicit;
	સંરચના ચાવી_કિંમત_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	જો ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	જો (actions_implicit)
		પસંદગી (argc) {
		સ્થિતિ 1: actions = ACTION_GET; તોડો;
		સ્થિતિ 2: actions = ACTION_SET; તોડો;
		સ્થિતિ 3: actions = ACTION_SET_ALL; તોડો;
		મૂળભૂત:
			error(_("no action specified"));
			exit(129);
		}
	જો (actions_implicit && argc == 1) {
		અચલ અક્ષર *last_dot = strrchr(argv[0], '.');
		જો (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_કિંમત(argv[0]);
	}
	જો (display_opts.omit_કિંમતો &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	જો (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	જો (display_opts.default_કિંમત && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	જો (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	જો (flags & CONFIG_FLAGS_FIXED_કિંમત) {
		પૂર્ણાંક allowed_usage = 0;

		પસંદગી (actions) {
		/* git config --get <name> <value-pattern> */
		સ્થિતિ ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		સ્થિતિ ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		સ્થિતિ ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		સ્થિતિ ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		સ્થિતિ ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			તોડો;

		/* git config <name> <value> <value-pattern> */
		સ્થિતિ ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		સ્થિતિ ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			તોડો;

		/* other options don't allow --fixed-value */
		}

		જો (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	જો (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	જો (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		જો (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			જો (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			નહીંતર
				die(_("error processing config file(s)"));
		}
	}
	નહીંતર જો (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	નહીંતર જો (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		કિંમત = normalize_કિંમત(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, કિંમત);
		જો (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	નહીંતર જો (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		કિંમત = normalize_કિંમત(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], કિંમત, argv[2],
							      comment, flags);
	}
	નહીંતર જો (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		કિંમત = normalize_કિંમત(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], કિંમત,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	નહીંતર જો (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		કિંમત = normalize_કિંમત(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], કિંમત, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	નહીંતર જો (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_કિંમત(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	નહીંતર જો (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_કિંમત(&location_opts, &display_opts, argv[0], argv[1],
				GET_કિંમત_ALL, flags);
	}
	નહીંતર જો (actions == ACTION_GET_REGEXP) {
		display_opts.show_ચાવીઓ = 1;
		check_argc(argc, 1, 2);
		ret = get_કિંમત(&location_opts, &display_opts, argv[0], argv[1],
				GET_કિંમત_ALL|GET_કિંમત_ચાવી_REGEXP, flags);
	}
	નહીંતર જો (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	નહીંતર જો (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		જો (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], ખાલી, argv[1],
								      ખાલી, flags);
		નહીંતર
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], ખાલી, ખાલી);
	}
	નહીંતર જો (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ખાલી, argv[1],
							      ખાલી, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	નહીંતર જો (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		જો (ret < 0)
			જાઓ out;
		નહીંતર જો (!ret)
			die(_("no such section: %s"), argv[0]);
		નહીંતર
			ret = 0;
	}
	નહીંતર જો (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], ખાલી);
		જો (ret < 0)
			જાઓ out;
		નહીંતર જો (!ret)
			die(_("no such section: %s"), argv[0]);
		નહીંતર
			ret = 0;
	}
	નહીંતર જો (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	નહીંતર જો (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		જો (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	મુક્ત(comment);
	મુક્ત(કિંમત);
	પાછા ret;
}

પૂર્ણાંક cmd_config(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ખાલી;
	સંરચના option subcommand_opts[] = {
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
	જો (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		પાછા subcommand(argc, argv, prefix, repo);
	}

	પાછા cmd_config_actions(argc, argv, prefix);
}
