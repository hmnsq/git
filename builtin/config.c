#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "date.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "ident.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "urlmatch.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "worktree.h"

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ফাঁকা
};

#define CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

কাঠামো config_location_options {
	কাঠামো git_config_source source;
	কাঠামো config_options options;
	অক্ষর *file_to_free;
	পূর্ণসংখ্যা use_global_config;
	পূর্ণসংখ্যা use_system_config;
	পূর্ণসংখ্যা use_local_config;
	পূর্ণসংখ্যা use_worktree_config;
	পূর্ণসংখ্যা respect_includes_opt;
};
#define CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

গণনা config_type {
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
	OPT_CALLBACK_মান(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_মান(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_মান(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_মান(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_মান(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_মান(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#define CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_মানসমূহ, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_চাবিগুলি, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

কাঠামো config_display_options {
	পূর্ণসংখ্যা end_nul;
	পূর্ণসংখ্যা omit_মানসমূহ;
	পূর্ণসংখ্যা show_origin;
	পূর্ণসংখ্যা show_scope;
	পূর্ণসংখ্যা show_চাবিগুলি;
	গণনা config_type type;
	অক্ষর *default_মান;
	/* Populated via `display_options_init()`. */
	পূর্ণসংখ্যা term;
	পূর্ণসংখ্যা delim;
	পূর্ণসংখ্যা চাবি_delim;
};
#define CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.চাবি_delim = ' ', \
	.type = TYPE_NONE, \
}

#define OPT_CALLBACK_মান(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.মান = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

স্থির পূর্ণসংখ্যা option_parse_type(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg,
			     পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা new_type, *to_type;

	যদি (unset) {
		*((পূর্ণসংখ্যা *) opt->মান) = 0;
		ফেরত 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	যদি (!new_type) {
		যদি (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		অন্যথা যদি (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		অন্যথা যদি (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		অন্যথা যদি (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		অন্যথা যদি (!strcmp(arg, "path"))
			new_type = TYPE_PATH;
		অন্যথা যদি (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		অন্যথা যদি (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		অন্যথা
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->মান;
	যদি (*to_type && *to_type != new_type) {
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

	ফেরত 0;
}

স্থির শূন্য check_argc(পূর্ণসংখ্যা argc, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max)
{
	যদি (argc >= min && argc <= max)
		ফেরত;
	যদি (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	অন্যথা
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	exit(129);
}

স্থির NORETURN শূন্য die_missing_set_মান(ধ্রুবক অক্ষর *arg)
{
	ধ্রুবক অক্ষর *last_dot = strrchr(arg, '.');
	ধ্রুবক অক্ষর *eq = last_dot ? strchr(last_dot + 1, '=') : ফাঁকা;
	অক্ষর *prefix = eq ? xstrndup(arg, eq - arg) : ফাঁকা;

	যদি (prefix && git_config_চাবি_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} অন্যথা যদি (git_config_চাবি_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} অন্যথা {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	মুক্ত(prefix);
	exit(129);
}

স্থির শূন্য show_config_origin(ধ্রুবক কাঠামো config_display_options *opts,
			       ধ্রুবক কাঠামো চাবি_মান_info *kvi,
			       কাঠামো strbuf *buf)
{
	ধ্রুবক অক্ষর term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	যদি (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	অন্যথা
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ফাঁকা, 0);
	strbuf_addch(buf, term);
}

স্থির শূন্য show_config_scope(ধ্রুবক কাঠামো config_display_options *opts,
			      ধ্রুবক কাঠামো চাবি_মান_info *kvi,
			      কাঠামো strbuf *buf)
{
	ধ্রুবক অক্ষর term = opts->end_nul ? '\0' : '\t';
	ধ্রুবক অক্ষর *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

কাঠামো strbuf_list {
	কাঠামো strbuf *বস্তুসমূহ;
	পূর্ণসংখ্যা nr;
	পূর্ণসংখ্যা alloc;
};

স্থির পূর্ণসংখ্যা format_config_int64(কাঠামো strbuf *buf,
			       ধ্রুবক অক্ষর *চাবি_2,
			       ধ্রুবক অক্ষর *মান_2,
			       ধ্রুবক কাঠামো চাবি_মান_info *kvi,
			       পূর্ণসংখ্যা gently)
{
	int64_t v = 0;
	যদি (gently) {
		যদি (!git_parse_int64(মান_2, &v))
			ফেরত -1;
	} অন্যথা {
		/* may die() */
		v = git_config_int64(চাবি_2, মান_2 ? মান_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা format_config_bool(কাঠামো strbuf *buf,
			      ধ্রুবক অক্ষর *চাবি_2,
			      ধ্রুবক অক্ষর *মান_2,
			      পূর্ণসংখ্যা gently)
{
	পূর্ণসংখ্যা v = 0;
	যদি (gently) {
		যদি ((v = git_parse_maybe_bool(মান_2)) < 0)
			ফেরত -1;
	} অন্যথা {
		/* may die() */
		v = git_config_bool(চাবি_2, মান_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা format_config_bool_or_int(কাঠামো strbuf *buf,
				     ধ্রুবক অক্ষর *চাবি_2,
				     ধ্রুবক অক্ষর *মান_2,
				     ধ্রুবক কাঠামো চাবি_মান_info *kvi,
				     পূর্ণসংখ্যা gently)
{
	পূর্ণসংখ্যা v, is_bool = 0;

	যদি (gently) {
		v = git_parse_maybe_bool_text(মান_2);

		যদি (v >= 0)
			is_bool = 1;
		অন্যথা যদি (!git_parse_int(মান_2, &v))
			ফেরত -1;
	} অন্যথা {
		v = git_config_bool_or_int(চাবি_2, মান_2, kvi,
					   &is_bool);
	}

	যদি (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	অন্যথা
		strbuf_addf(buf, "%d", v);

	ফেরত 0;
}

/* This mode is always gentle. */
স্থির পূর্ণসংখ্যা format_config_bool_or_str(কাঠামো strbuf *buf,
				     ধ্রুবক অক্ষর *মান_2)
{
	পূর্ণসংখ্যা v = git_parse_maybe_bool(মান_2);
	যদি (v < 0)
		strbuf_addstr(buf, মান_2);
	অন্যথা
		strbuf_addstr(buf, v ? "true" : "false");
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা format_config_path(কাঠামো strbuf *buf,
			      ধ্রুবক অক্ষর *চাবি_2,
			      ধ্রুবক অক্ষর *মান_2,
			      পূর্ণসংখ্যা gently)
{
	অক্ষর *v;

	যদি (git_config_pathname(&v, চাবি_2, মান_2) < 0)
		ফেরত -1;

	যদি (v)
		strbuf_addstr(buf, v);
	অন্যথা
		ফেরত gently ? -1 : 1; /* :(optional)no-such-file */

	মুক্ত(v);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা format_config_expiry_date(কাঠামো strbuf *buf,
				     ধ্রুবক অক্ষর *চাবি_2,
				     ধ্রুবক অক্ষর *মান_2,
				     পূর্ণসংখ্যা quietly)
{
	timestamp_t t;
	যদি (quietly) {
		যদি (parse_expiry_date(মান_2, &t))
			ফেরত -1;
	} অন্যথা যদি (git_config_expiry_date(&t, চাবি_2, মান_2) < 0) {
		ফেরত -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা format_config_color(কাঠামো strbuf *buf,
			       ধ্রুবক অক্ষর *চাবি_2,
			       ধ্রুবক অক্ষর *মান_2,
			       পূর্ণসংখ্যা gently)
{
	অক্ষর v[COLOR_MAXLEN];

	যদি (gently) {
		যদি (color_parse_quietly(মান_2, v) < 0)
			ফেরত -1;
	} অন্যথা যদি (git_config_color(v, চাবি_2, মান_2) < 0) {
		ফেরত -1;
	}

	strbuf_addstr(buf, v);
	ফেরত 0;
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
স্থির পূর্ণসংখ্যা format_config(ধ্রুবক কাঠামো config_display_options *opts,
			 কাঠামো strbuf *buf, ধ্রুবক অক্ষর *চাবি_2,
			 ধ্রুবক অক্ষর *মান_2, ধ্রুবক কাঠামো চাবি_মান_info *kvi,
			 পূর্ণসংখ্যা gently)
{
	পূর্ণসংখ্যা res = 0;
	যদি (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	যদি (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	যদি (opts->show_চাবিগুলি)
		strbuf_addstr(buf, চাবি_2);

	যদি (opts->omit_মানসমূহ)
		যাও terminator;

	যদি (opts->show_চাবিগুলি)
		strbuf_addch(buf, opts->চাবি_delim);

	নির্বাচন (opts->type) {
	ক্ষেত্র TYPE_INT:
		res = format_config_int64(buf, চাবি_2, মান_2, kvi, gently);
		বিরতি;

	ক্ষেত্র TYPE_BOOL:
		res = format_config_bool(buf, চাবি_2, মান_2, gently);
		বিরতি;

	ক্ষেত্র TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, চাবি_2, মান_2, kvi, gently);
		বিরতি;

	ক্ষেত্র TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, মান_2);
		বিরতি;

	ক্ষেত্র TYPE_PATH:
		res = format_config_path(buf, চাবি_2, মান_2, gently);
		বিরতি;

	ক্ষেত্র TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, চাবি_2, মান_2, gently);
		বিরতি;

	ক্ষেত্র TYPE_COLOR:
		res = format_config_color(buf, চাবি_2, মান_2, gently);
		বিরতি;

	ক্ষেত্র TYPE_NONE:
		যদি (মান_2) {
			strbuf_addstr(buf, মান_2);
		} অন্যথা {
			/* Just show the key name; back out delimiter */
			যদি (opts->show_চাবিগুলি)
				strbuf_setlen(buf, buf->len - 1);
		}
		বিরতি;

	পূর্বনির্ধারিত:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা show_all_config(ধ্রুবক অক্ষর *চাবি_2, ধ্রুবক অক্ষর *মান_2,
			   ধ্রুবক কাঠামো config_context *ctx,
			   শূন্য *cb)
{
	ধ্রুবক কাঠামো config_display_options *opts = cb;
	ধ্রুবক কাঠামো চাবি_মান_info *kvi = ctx->kvi;
	কাঠামো strbuf formatted = STRBUF_INIT;

	যদি (format_config(opts, &formatted, চাবি_2, মান_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	ফেরত 0;
}

#define GET_মান_ALL        (1 << 0)
#define GET_মান_চাবি_REGEXP (1 << 1)

কাঠামো collect_config_তথ্য {
	ধ্রুবক কাঠামো config_display_options *display_opts;
	কাঠামো strbuf_list *মানসমূহ;
	ধ্রুবক অক্ষর *মান_pattern;
	ধ্রুবক অক্ষর *চাবি;
	regex_t *regexp;
	regex_t *চাবি_regexp;
	পূর্ণসংখ্যা do_not_match;
	unsigned get_মান_flags;
	unsigned flags;
};

স্থির পূর্ণসংখ্যা collect_config(ধ্রুবক অক্ষর *চাবি_2, ধ্রুবক অক্ষর *মান_2,
			  ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	কাঠামো collect_config_তথ্য *তথ্য = cb;
	কাঠামো strbuf_list *মানসমূহ = তথ্য->মানসমূহ;
	ধ্রুবক কাঠামো চাবি_মান_info *kvi = ctx->kvi;
	পূর্ণসংখ্যা status;

	যদি (!(তথ্য->get_মান_flags & GET_মান_চাবি_REGEXP) &&
	    strcmp(চাবি_2, তথ্য->চাবি))
		ফেরত 0;
	যদি ((তথ্য->get_মান_flags & GET_মান_চাবি_REGEXP) &&
	    regexec(তথ্য->চাবি_regexp, চাবি_2, 0, ফাঁকা, 0))
		ফেরত 0;
	যদি ((তথ্য->flags & CONFIG_FLAGS_FIXED_মান) &&
	    strcmp(তথ্য->মান_pattern, (মান_2?মান_2:"")))
		ফেরত 0;
	যদি (তথ্য->regexp &&
	    (তথ্য->do_not_match ^ !!regexec(তথ্য->regexp, (মান_2?মান_2:""), 0, ফাঁকা, 0)))
		ফেরত 0;

	ALLOC_GROW(মানসমূহ->বস্তুসমূহ, মানসমূহ->nr + 1, মানসমূহ->alloc);
	strbuf_init(&মানসমূহ->বস্তুসমূহ[মানসমূহ->nr], 0);

	status = format_config(তথ্য->display_opts, &মানসমূহ->বস্তুসমূহ[মানসমূহ->nr++],
			       চাবি_2, মান_2, kvi, 0);
	যদি (status < 0)
		ফেরত status;
	যদি (status) {
		strbuf_release(&মানসমূহ->বস্তুসমূহ[--মানসমূহ->nr]);
		status = 0;
	}
	ফেরত status;
}

স্থির পূর্ণসংখ্যা get_মান(ধ্রুবক কাঠামো config_location_options *opts,
		     ধ্রুবক কাঠামো config_display_options *display_opts,
		     ধ্রুবক অক্ষর *চাবি_2, ধ্রুবক অক্ষর *regex_,
		     unsigned get_মান_flags, unsigned flags)
{
	পূর্ণসংখ্যা ret = CONFIG_GENERIC_ERROR;
	কাঠামো strbuf_list মানসমূহ = {ফাঁকা};
	কাঠামো collect_config_তথ্য তথ্য = {
		.display_opts = display_opts,
		.মানসমূহ = &মানসমূহ,
		.get_মান_flags = get_মান_flags,
		.flags = flags,
	};
	অক্ষর *চাবি = ফাঁকা;
	পূর্ণসংখ্যা i;

	যদি (get_মান_flags & GET_মান_চাবি_REGEXP) {
		অক্ষর *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		চাবি = xstrdup(চাবি_2);
		জন্য (tl = চাবি + strlen(চাবি) - 1;
		     tl >= চাবি && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		জন্য (tl = চাবি; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		তথ্য.চাবি_regexp = (regex_t*)xmalloc(আকার(regex_t));
		যদি (regcomp(তথ্য.চাবি_regexp, চাবি, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), চাবি_2);
			FREE_AND_NULL(তথ্য.চাবি_regexp);
			ret = CONFIG_INVALID_PATTERN;
			যাও free_strings;
		}
	} অন্যথা {
		যদি (git_config_parse_চাবি(চাবি_2, &চাবি, ফাঁকা)) {
			ret = CONFIG_INVALID_চাবি;
			যাও free_strings;
		}

		তথ্য.চাবি = চাবি;
	}

	যদি (regex_ && (flags & CONFIG_FLAGS_FIXED_মান))
		তথ্য.মান_pattern = regex_;
	অন্যথা যদি (regex_) {
		যদি (regex_[0] == '!') {
			তথ্য.do_not_match = 1;
			regex_++;
		}

		তথ্য.regexp = (regex_t*)xmalloc(আকার(regex_t));
		যদি (regcomp(তথ্য.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(তথ্য.regexp);
			ret = CONFIG_INVALID_PATTERN;
			যাও free_strings;
		}
	}

	config_with_options(collect_config, &তথ্য,
			    &opts->source, the_repository,
			    &opts->options);

	যদি (!মানসমূহ.nr && display_opts->default_মান) {
		কাঠামো চাবি_মান_info kvi = KVI_INIT;
		কাঠামো strbuf *item;
		পূর্ণসংখ্যা status;

		kvi_from_param(&kvi);
		ALLOC_GROW(মানসমূহ.বস্তুসমূহ, মানসমূহ.nr + 1, মানসমূহ.alloc);
		item = &মানসমূহ.বস্তুসমূহ[মানসমূহ.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, চাবি_2,
				       display_opts->default_মান, &kvi, 0);
		যদি (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_মান);
		যদি (status) {
			/* default was a missing optional value */
			মানসমূহ.nr--;
			strbuf_release(item);
		}
	}

	ret = !মানসমূহ.nr;

	জন্য (i = 0; i < মানসমূহ.nr; i++) {
		কাঠামো strbuf *buf = মানসমূহ.বস্তুসমূহ + i;
		যদি ((get_মান_flags & GET_মান_ALL) || i == মানসমূহ.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	মুক্ত(মানসমূহ.বস্তুসমূহ);

free_strings:
	মুক্ত(চাবি);
	যদি (তথ্য.চাবি_regexp) {
		regfree(তথ্য.চাবি_regexp);
		মুক্ত(তথ্য.চাবি_regexp);
	}
	যদি (তথ্য.regexp) {
		regfree(তথ্য.regexp);
		মুক্ত(তথ্য.regexp);
	}

	ফেরত ret;
}

স্থির অক্ষর *normalize_মান(ধ্রুবক অক্ষর *চাবি, ধ্রুবক অক্ষর *মান,
			     পূর্ণসংখ্যা type, কাঠামো চাবি_মান_info *kvi)
{
	যদি (!মান)
		ফেরত ফাঁকা;

	যদি (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		ফেরত xstrdup(মান);
	যদি (type == TYPE_INT)
		ফেরত xstrfmt("%"PRId64, git_config_int64(চাবি, মান, kvi));
	যদি (type == TYPE_BOOL)
		ফেরত xstrdup(git_config_bool(চাবি, মান) ?  "true" : "false");
	যদি (type == TYPE_BOOL_OR_INT) {
		পূর্ণসংখ্যা is_bool, v;
		v = git_config_bool_or_int(চাবি, মান, kvi, &is_bool);
		যদি (!is_bool)
			ফেরত xstrfmt("%d", v);
		অন্যথা
			ফেরত xstrdup(v ? "true" : "false");
	}
	যদি (type == TYPE_BOOL_OR_STR) {
		পূর্ণসংখ্যা v = git_parse_maybe_bool(মান);
		যদি (v < 0)
			ফেরত xstrdup(মান);
		অন্যথা
			ফেরত xstrdup(v ? "true" : "false");
	}
	যদি (type == TYPE_COLOR) {
		অক্ষর v[COLOR_MAXLEN];
		যদি (git_config_color(v, চাবি, মান))
			die(_("cannot parse color '%s'"), মান);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		ফেরত xstrdup(মান);
	}

	BUG("cannot normalize type %d", type);
}

কাঠামো get_color_config_তথ্য {
	পূর্ণসংখ্যা get_color_found;
	ধ্রুবক অক্ষর *get_color_slot;
	অক্ষর parsed_color[COLOR_MAXLEN];
};

স্থির পূর্ণসংখ্যা git_get_color_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
				ধ্রুবক কাঠামো config_context *ctx UNUSED,
				শূন্য *cb)
{
	কাঠামো get_color_config_তথ্য *তথ্য = cb;

	যদি (!strcmp(var, তথ্য->get_color_slot)) {
		যদি (!মান)
			config_error_nonbool(var);
		যদি (color_parse(মান, তথ্য->parsed_color) < 0)
			ফেরত -1;
		তথ্য->get_color_found = 1;
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা get_color(ধ্রুবক কাঠামো config_location_options *opts,
		      ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *def_color)
{
	কাঠামো get_color_config_তথ্য তথ্য = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	পূর্ণসংখ্যা ret;

	config_with_options(git_get_color_config, &তথ্য,
			    &opts->source, the_repository,
			    &opts->options);

	যদি (!তথ্য.get_color_found && def_color) {
		যদি (color_parse(def_color, তথ্য.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			যাও out;
		}
	}

	ret = 0;

out:
	fputs(তথ্য.parsed_color, stdout);
	ফেরত ret;
}

কাঠামো get_colorbool_config_তথ্য {
	গণনা git_colorbool get_colorbool_found;
	গণনা git_colorbool get_diff_color_found;
	গণনা git_colorbool get_color_ui_found;
	ধ্রুবক অক্ষর *get_colorbool_slot;
};

স্থির পূর্ণসংখ্যা git_get_colorbool_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
				    ধ্রুবক কাঠামো config_context *ctx UNUSED,
				    শূন্য *cb)
{
	কাঠামো get_colorbool_config_তথ্য *তথ্য = cb;

	যদি (!strcmp(var, তথ্য->get_colorbool_slot))
		তথ্য->get_colorbool_found = git_config_colorbool(var, মান);
	অন্যথা যদি (!strcmp(var, "diff.color"))
		তথ্য->get_diff_color_found = git_config_colorbool(var, মান);
	অন্যথা যদি (!strcmp(var, "color.ui"))
		তথ্য->get_color_ui_found = git_config_colorbool(var, মান);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা get_colorbool(ধ্রুবক কাঠামো config_location_options *opts,
			 ধ্রুবক অক্ষর *var, পূর্ণসংখ্যা print)
{
	কাঠামো get_colorbool_config_তথ্য তথ্য = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool ফলাফল;

	config_with_options(git_get_colorbool_config, &তথ্য,
			    &opts->source, the_repository,
			    &opts->options);

	যদি (তথ্য.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		যদি (!strcmp(তথ্য.get_colorbool_slot, "color.diff"))
			তথ্য.get_colorbool_found = তথ্য.get_diff_color_found;
		যদি (তথ্য.get_colorbool_found == GIT_COLOR_UNKNOWN)
			তথ্য.get_colorbool_found = তথ্য.get_color_ui_found;
	}

	যদি (তথ্য.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		তথ্য.get_colorbool_found = GIT_COLOR_AUTO;

	ফলাফল = want_color(তথ্য.get_colorbool_found);

	যদি (print) {
		printf("%s\n", ফলাফল ? "true" : "false");
		ফেরত 0;
	} অন্যথা
		ফেরত ফলাফল ? 0 : 1;
}

স্থির শূন্য check_write(ধ্রুবক কাঠামো git_config_source *source)
{
	যদি (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	যদি (source->use_stdin)
		die(_("writing to stdin is not supported"));

	যদি (source->blob)
		die(_("writing config blobs is not supported"));
}

কাঠামো urlmatch_current_candidate_মান {
	অক্ষর মান_is_null;
	কাঠামো strbuf মান;
	কাঠামো চাবি_মান_info kvi;
};

স্থির পূর্ণসংখ্যা urlmatch_collect_fn(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			       ধ্রুবক কাঠামো config_context *ctx,
			       শূন্য *cb)
{
	কাঠামো string_list *মানসমূহ = cb;
	কাঠামো string_list_item *item = string_list_ঢোকাও(মানসমূহ, var);
	কাঠামো urlmatch_current_candidate_মান *matched = item->util;
	ধ্রুবক কাঠামো চাবি_মান_info *kvi = ctx->kvi;

	যদি (!matched) {
		matched = xmalloc(আকার(*matched));
		strbuf_init(&matched->মান, 0);
		item->util = matched;
	} অন্যথা {
		strbuf_reset(&matched->মান);
	}
	matched->kvi = *kvi;

	যদি (মান) {
		strbuf_addstr(&matched->মান, মান);
		matched->মান_is_null = 0;
	} অন্যথা {
		matched->মান_is_null = 1;
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা get_urlmatch(ধ্রুবক কাঠামো config_location_options *opts,
			ধ্রুবক কাঠামো config_display_options *_display_opts,
			ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *url)
{
	পূর্ণসংখ্যা ret;
	অক্ষর *section;
	অক্ষর *section_লেজ;
	কাঠামো config_display_options display_opts = *_display_opts;
	কাঠামো string_list_item *item;
	কাঠামো urlmatch_config config = URLMATCH_CONFIG_INIT;
	কাঠামো string_list মানসমূহ = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ফাঁকা;
	config.cb = &মানসমূহ;

	যদি (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_লেজ = strchr(section, '.');
	যদি (section_লেজ) {
		*section_লেজ = '\0';
		config.চাবি = section_লেজ + 1;
		display_opts.show_চাবিগুলি = 0;
	} অন্যথা {
		config.চাবি = ফাঁকা;
		display_opts.show_চাবিগুলি = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !মানসমূহ.nr;

	for_each_string_list_item(item, &মানসমূহ) {
		কাঠামো urlmatch_current_candidate_মান *matched = item->util;
		কাঠামো strbuf buf = STRBUF_INIT;
		পূর্ণসংখ্যা status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->মান_is_null ? ফাঁকা : matched->মান.buf,
				       &matched->kvi, 0);
		যদি (!status)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->মান);
	}
	urlmatch_config_release(&config);
	string_list_clear(&মানসমূহ, 1);
	মুক্ত(config.url.url);

	মুক্ত(section);
	ফেরত ret;
}

স্থির অক্ষর *default_user_config(শূন্য)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	ফেরত strbuf_detach(&buf, ফাঁকা);
}

স্থির শূন্য location_options_init(কাঠামো config_location_options *opts,
				  ধ্রুবক অক্ষর *prefix)
{
	যদি (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	যদি (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		exit(129);
	}

	যদি (!startup_info->have_repository) {
		যদি (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		যদি (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		যদি (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	যদি (opts->source.file &&
			!strcmp(opts->source.file, "-")) {
		opts->source.file = ফাঁকা;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	যদি (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		যদি (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} অন্যথা যদি (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} অন্যথা যদি (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} অন্যথা যদি (opts->use_worktree_config) {
		কাঠামো worktree **worktrees = get_worktrees(the_repository);
		যদি (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		অন্যথা যদি (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		অন্যথা
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} অন্যথা যদি (opts->source.file) {
		যদি (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} অন্যথা যদি (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	যদি (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	অন্যথা
		opts->options.respect_includes = opts->respect_includes_opt;
	যদি (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

স্থির শূন্য location_options_release(কাঠামো config_location_options *opts)
{
	মুক্ত(opts->file_to_free);
}

স্থির শূন্য display_options_init(কাঠামো config_display_options *opts)
{
	যদি (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->চাবি_delim = '\n';
	}
}

স্থির শূন্য display_options_init_list(কাঠামো config_display_options *opts)
{
	opts->show_চাবিগুলি = 1;

	যদি (opts->end_nul) {
		display_options_init(opts);
	} অন্যথা {
		opts->term = '\n';
		opts->delim = ' ';
		opts->চাবি_delim = '=';
	}
}

স্থির পূর্ণসংখ্যা cmd_config_list(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo UNUSED)
{
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	কাঠামো config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	কাঠামো option opts[] = {
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

	যদি (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		যদি (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		অন্যথা
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_config_get(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo UNUSED)
{
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	কাঠামো config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ধ্রুবক অক্ষর *মান_pattern = ফাঁকা, *url = ফাঁকা;
	পূর্ণসংখ্যা flags = 0;
	unsigned get_মান_flags = 0;
	কাঠামো option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_মান_flags, N_("return all values for multi-valued config options"), GET_মান_ALL),
		OPT_BIT(0, "regexp", &get_মান_flags, N_("interpret the name as a regular expression"), GET_মান_চাবি_REGEXP),
		OPT_STRING(0, "value", &মান_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_মান),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_মান,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	যদি ((flags & CONFIG_FLAGS_FIXED_মান) && !মান_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	যদি (display_opts.default_মান &&
	    ((get_মান_flags & GET_মান_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	যদি (url && ((get_মান_flags & GET_মান_ALL) ||
		    (get_মান_flags & GET_মান_চাবি_REGEXP) ||
		    মান_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	যদি (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	যদি (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	অন্যথা যদি (display_opts.type == TYPE_COLOR && !strlen(argv[0]) && display_opts.default_মান)
		ret = get_color(&location_opts, "", display_opts.default_মান);
	অন্যথা
		ret = get_মান(&location_opts, &display_opts, argv[0], মান_pattern,
				get_মান_flags, flags);

	location_options_release(&location_opts);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_config_set(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			  কাঠামো repository *repo UNUSED)
{
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ধ্রুবক অক্ষর *মান_pattern = ফাঁকা, *comment_arg = ফাঁকা;
	অক্ষর *comment = ফাঁকা;
	পূর্ণসংখ্যা flags = 0, append = 0, type = 0;
	কাঠামো option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &মান_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_মান),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	কাঠামো চাবি_মান_info default_kvi = KVI_INIT;
	অক্ষর *মান;
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	যদি (argc == 1)
		die_missing_set_মান(argv[0]);
	check_argc(argc, 2, 2);

	যদি ((flags & CONFIG_FLAGS_FIXED_মান) && !মান_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	যদি (append && মান_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	যদি (append)
		মান_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	মান = normalize_মান(argv[0], argv[1], type, &default_kvi);

	যদি ((flags & CONFIG_FLAGS_MULTI_REPLACE) || মান_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], মান, মান_pattern,
							      comment, flags);
	} অন্যথা {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, মান);
		যদি (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	মুক্ত(comment);
	মুক্ত(মান);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_config_unset(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			    কাঠামো repository *repo UNUSED)
{
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	ধ্রুবক অক্ষর *মান_pattern = ফাঁকা;
	পূর্ণসংখ্যা flags = 0;
	কাঠামো option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &মান_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_মান),
		OPT_END(),
	};
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	যদি ((flags & CONFIG_FLAGS_FIXED_মান) && !মান_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	যদি ((flags & CONFIG_FLAGS_MULTI_REPLACE) || মান_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ফাঁকা, মান_pattern,
							      ফাঁকা, flags);
	অন্যথা
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     ফাঁকা, ফাঁকা);

	location_options_release(&location_opts);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_config_rename_section(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				     কাঠামো repository *repo UNUSED)
{
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	কাঠামো option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	যদি (ret < 0)
		যাও out;
	অন্যথা যদি (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_config_remove_section(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				     কাঠামো repository *repo UNUSED)
{
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	কাঠামো option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], ফাঁকা);
	যদি (ret < 0)
		যাও out;
	অন্যথা যদি (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা show_editor(কাঠামো config_location_options *opts)
{
	অক্ষর *config_file;

	যদি (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	যদি (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	যদি (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ফাঁকা);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	যদি (opts->use_global_config) {
		পূর্ণসংখ্যা fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		যদি (fd >= 0) {
			অক্ষর *content = default_user_config();
			write_str_in_full(fd, content);
			মুক্ত(content);
			close(fd);
		}
		অন্যথা যদি (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	যদি (launch_editor(config_file, ফাঁকা, ফাঁকা)) {
		মুক্ত(config_file);
		ফেরত -1;
	}
	মুক্ত(config_file);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_config_edit(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			   কাঠামো repository *repo UNUSED)
{
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	কাঠামো option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_config_actions(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix)
{
	গণনা {
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
	কাঠামো config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	কাঠামো config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	ধ্রুবক অক্ষর *comment_arg = ফাঁকা;
	পূর্ণসংখ্যা actions = 0;
	unsigned flags = 0;
	কাঠামো option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_মান,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_মান),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	অক্ষর *মান = ফাঁকা, *comment = ফাঁকা;
	পূর্ণসংখ্যা ret = 0;
	পূর্ণসংখ্যা actions_implicit;
	কাঠামো চাবি_মান_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	যদি ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		exit(129);
	}

	actions_implicit = (actions == 0);
	যদি (actions_implicit)
		নির্বাচন (argc) {
		ক্ষেত্র 1: actions = ACTION_GET; বিরতি;
		ক্ষেত্র 2: actions = ACTION_SET; বিরতি;
		ক্ষেত্র 3: actions = ACTION_SET_ALL; বিরতি;
		পূর্বনির্ধারিত:
			error(_("no action specified"));
			exit(129);
		}
	যদি (actions_implicit && argc == 1) {
		ধ্রুবক অক্ষর *last_dot = strrchr(argv[0], '.');
		যদি (last_dot && strchr(last_dot + 1, '='))
			die_missing_set_মান(argv[0]);
	}
	যদি (display_opts.omit_মানসমূহ &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		exit(129);
	}

	যদি (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		exit(129);
	}

	যদি (display_opts.default_মান && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		exit(129);
	}

	যদি (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		exit(129);
	}

	/* check usage of --fixed-value */
	যদি (flags & CONFIG_FLAGS_FIXED_মান) {
		পূর্ণসংখ্যা allowed_usage = 0;

		নির্বাচন (actions) {
		/* git config --get <name> <value-pattern> */
		ক্ষেত্র ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		ক্ষেত্র ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		ক্ষেত্র ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		ক্ষেত্র ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		ক্ষেত্র ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			বিরতি;

		/* git config <name> <value> <value-pattern> */
		ক্ষেত্র ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		ক্ষেত্র ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			বিরতি;

		/* other options don't allow --fixed-value */
		}

		যদি (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			exit(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	যদি (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	যদি (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		যদি (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			যদি (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			অন্যথা
				die(_("error processing config file(s)"));
		}
	}
	অন্যথা যদি (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	অন্যথা যদি (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		মান = normalize_মান(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, মান);
		যদি (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	অন্যথা যদি (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		মান = normalize_মান(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], মান, argv[2],
							      comment, flags);
	}
	অন্যথা যদি (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		মান = normalize_মান(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], মান,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	অন্যথা যদি (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		মান = normalize_মান(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], মান, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	অন্যথা যদি (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_মান(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	অন্যথা যদি (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_মান(&location_opts, &display_opts, argv[0], argv[1],
				GET_মান_ALL, flags);
	}
	অন্যথা যদি (actions == ACTION_GET_REGEXP) {
		display_opts.show_চাবিগুলি = 1;
		check_argc(argc, 1, 2);
		ret = get_মান(&location_opts, &display_opts, argv[0], argv[1],
				GET_মান_ALL|GET_মান_চাবি_REGEXP, flags);
	}
	অন্যথা যদি (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	অন্যথা যদি (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		যদি (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], ফাঁকা, argv[1],
								      ফাঁকা, flags);
		অন্যথা
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], ফাঁকা, ফাঁকা);
	}
	অন্যথা যদি (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], ফাঁকা, argv[1],
							      ফাঁকা, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	অন্যথা যদি (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		যদি (ret < 0)
			যাও out;
		অন্যথা যদি (!ret)
			die(_("no such section: %s"), argv[0]);
		অন্যথা
			ret = 0;
	}
	অন্যথা যদি (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], ফাঁকা);
		যদি (ret < 0)
			যাও out;
		অন্যথা যদি (!ret)
			die(_("no such section: %s"), argv[0]);
		অন্যথা
			ret = 0;
	}
	অন্যথা যদি (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	অন্যথা যদি (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		যদি (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	মুক্ত(comment);
	মুক্ত(মান);
	ফেরত ret;
}

পূর্ণসংখ্যা cmd_config(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ফাঁকা;
	কাঠামো option subcommand_opts[] = {
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
	যদি (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		ফেরত subcommand(argc, argv, prefix, repo);
	}

	ফেরত cmd_config_actions(argc, argv, prefix);
}
