#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "date.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "ident.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "urlmatch.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "worktree.h"

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_సమితి_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	ఖాళీ
};

#నిర్వచించు CONFIG_LOCATION_OPTIONS(opts) \
	OPT_సమూహం(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.ఆధారం.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.ఆధారం.blob, N_("blob-id"), N_("read config from given blob object"))

నిర్మాణం config_location_options {
	నిర్మాణం git_config_ఆధారం ఆధారం;
	నిర్మాణం config_options options;
	అక్షరం *file_గమ్యం_free;
	పూర్ణాంకం use_global_config;
	పూర్ణాంకం use_system_config;
	పూర్ణాంకం use_local_config;
	పూర్ణాంకం use_worktree_config;
	పూర్ణాంకం respect_includes_opt;
};
#నిర్వచించు CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

గణన config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_మార్గం,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#నిర్వచించు CONFIG_TYPE_OPTIONS(type) \
	OPT_సమూహం(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_విలువ(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_విలువ(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_విలువ(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_విలువ(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_విలువ(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_మార్గం), \
	OPT_CALLBACK_విలువ(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#నిర్వచించు CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_సమూహం(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.చివర_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_విలువలు, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_కీలకాలు, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

నిర్మాణం config_display_options {
	పూర్ణాంకం చివర_nul;
	పూర్ణాంకం omit_విలువలు;
	పూర్ణాంకం show_origin;
	పూర్ణాంకం show_scope;
	పూర్ణాంకం show_కీలకాలు;
	గణన config_type type;
	అక్షరం *default_విలువ;
	/* Populated via `display_options_init()`. */
	పూర్ణాంకం పదం;
	పూర్ణాంకం delim;
	పూర్ణాంకం కీలకం_delim;
};
#నిర్వచించు CONFIG_DISPLAY_OPTIONS_INIT { \
	.పదం = '\n', \
	.delim = '=', \
	.కీలకం_delim = ' ', \
	.type = TYPE_NONE, \
}

#నిర్వచించు OPT_CALLBACK_విలువ(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.విలువ = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

స్థిర పూర్ణాంకం option_parse_type(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			     పూర్ణాంకం unset)
{
	పూర్ణాంకం new_type, *గమ్యం_type;

	ఒకవేళ (unset) {
		*((పూర్ణాంకం *) opt->విలువ) = 0;
		తిరిగి 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	ఒకవేళ (!new_type) {
		ఒకవేళ (!strcmp(arg, "bool"))
			new_type = TYPE_BOOL;
		లేకపోతే ఒకవేళ (!strcmp(arg, "int"))
			new_type = TYPE_INT;
		లేకపోతే ఒకవేళ (!strcmp(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		లేకపోతే ఒకవేళ (!strcmp(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		లేకపోతే ఒకవేళ (!strcmp(arg, "path"))
			new_type = TYPE_మార్గం;
		లేకపోతే ఒకవేళ (!strcmp(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		లేకపోతే ఒకవేళ (!strcmp(arg, "color"))
			new_type = TYPE_COLOR;
		లేకపోతే
			die(_("unrecognized --type argument, %s"), arg);
	}

	గమ్యం_type = opt->విలువ;
	ఒకవేళ (*గమ్యం_type && *గమ్యం_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		నిష్క్రమించు(129);
	}
	*గమ్యం_type = new_type;

	తిరిగి 0;
}

స్థిర శూన్య check_argc(పూర్ణాంకం argc, పూర్ణాంకం min, పూర్ణాంకం max)
{
	ఒకవేళ (argc >= min && argc <= max)
		తిరిగి;
	ఒకవేళ (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	లేకపోతే
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	నిష్క్రమించు(129);
}

స్థిర NORETURN శూన్య die_missing_సమితి_విలువ(స్థిరరాశి అక్షరం *arg)
{
	స్థిరరాశి అక్షరం *చివరి_dot = strrchr(arg, '.');
	స్థిరరాశి అక్షరం *eq = చివరి_dot ? strchr(చివరి_dot + 1, '=') : ఖాళీ;
	అక్షరం *ఉపసర్గ = eq ? xstrndup(arg, eq - arg) : ఖాళీ;

	ఒకవేళ (ఉపసర్గ && git_config_కీలకం_is_valid(ఉపసర్గ)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       ఉపసర్గ, eq + 1);
	} లేకపోతే ఒకవేళ (git_config_కీలకం_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} లేకపోతే {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	విడుదల(ఉపసర్గ);
	నిష్క్రమించు(129);
}

స్థిర శూన్య show_config_origin(స్థిరరాశి నిర్మాణం config_display_options *opts,
			       స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi,
			       నిర్మాణం strbuf *buf)
{
	స్థిరరాశి అక్షరం పదం = opts->చివర_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	ఒకవేళ (opts->చివర_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	లేకపోతే
		quote_c_style(kvi->filename ? kvi->filename : "", buf, ఖాళీ, 0);
	strbuf_addch(buf, పదం);
}

స్థిర శూన్య show_config_scope(స్థిరరాశి నిర్మాణం config_display_options *opts,
			      స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi,
			      నిర్మాణం strbuf *buf)
{
	స్థిరరాశి అక్షరం పదం = opts->చివర_nul ? '\0' : '\t';
	స్థిరరాశి అక్షరం *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, పదం);
}

నిర్మాణం strbuf_list {
	నిర్మాణం strbuf *వస్తువులు;
	పూర్ణాంకం nr;
	పూర్ణాంకం alloc;
};

స్థిర పూర్ణాంకం format_config_int64(నిర్మాణం strbuf *buf,
			       స్థిరరాశి అక్షరం *కీలకం_2,
			       స్థిరరాశి అక్షరం *విలువ_2,
			       స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi,
			       పూర్ణాంకం gently)
{
	int64_t v = 0;
	ఒకవేళ (gently) {
		ఒకవేళ (!git_parse_int64(విలువ_2, &v))
			తిరిగి -1;
	} లేకపోతే {
		/* may die() */
		v = git_config_int64(కీలకం_2, విలువ_2 ? విలువ_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం format_config_bool(నిర్మాణం strbuf *buf,
			      స్థిరరాశి అక్షరం *కీలకం_2,
			      స్థిరరాశి అక్షరం *విలువ_2,
			      పూర్ణాంకం gently)
{
	పూర్ణాంకం v = 0;
	ఒకవేళ (gently) {
		ఒకవేళ ((v = git_parse_maybe_bool(విలువ_2)) < 0)
			తిరిగి -1;
	} లేకపోతే {
		/* may die() */
		v = git_config_bool(కీలకం_2, విలువ_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం format_config_bool_or_int(నిర్మాణం strbuf *buf,
				     స్థిరరాశి అక్షరం *కీలకం_2,
				     స్థిరరాశి అక్షరం *విలువ_2,
				     స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi,
				     పూర్ణాంకం gently)
{
	పూర్ణాంకం v, is_bool = 0;

	ఒకవేళ (gently) {
		v = git_parse_maybe_bool_పాఠ్యం(విలువ_2);

		ఒకవేళ (v >= 0)
			is_bool = 1;
		లేకపోతే ఒకవేళ (!git_parse_int(విలువ_2, &v))
			తిరిగి -1;
	} లేకపోతే {
		v = git_config_bool_or_int(కీలకం_2, విలువ_2, kvi,
					   &is_bool);
	}

	ఒకవేళ (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	లేకపోతే
		strbuf_addf(buf, "%d", v);

	తిరిగి 0;
}

/* This mode is always gentle. */
స్థిర పూర్ణాంకం format_config_bool_or_str(నిర్మాణం strbuf *buf,
				     స్థిరరాశి అక్షరం *విలువ_2)
{
	పూర్ణాంకం v = git_parse_maybe_bool(విలువ_2);
	ఒకవేళ (v < 0)
		strbuf_addstr(buf, విలువ_2);
	లేకపోతే
		strbuf_addstr(buf, v ? "true" : "false");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం format_config_మార్గం(నిర్మాణం strbuf *buf,
			      స్థిరరాశి అక్షరం *కీలకం_2,
			      స్థిరరాశి అక్షరం *విలువ_2,
			      పూర్ణాంకం gently)
{
	అక్షరం *v;

	ఒకవేళ (git_config_pathname(&v, కీలకం_2, విలువ_2) < 0)
		తిరిగి -1;

	ఒకవేళ (v)
		strbuf_addstr(buf, v);
	లేకపోతే
		తిరిగి gently ? -1 : 1; /* :(optional)no-such-file */

	విడుదల(v);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం format_config_expiry_date(నిర్మాణం strbuf *buf,
				     స్థిరరాశి అక్షరం *కీలకం_2,
				     స్థిరరాశి అక్షరం *విలువ_2,
				     పూర్ణాంకం quietly)
{
	timestamp_t t;
	ఒకవేళ (quietly) {
		ఒకవేళ (parse_expiry_date(విలువ_2, &t))
			తిరిగి -1;
	} లేకపోతే ఒకవేళ (git_config_expiry_date(&t, కీలకం_2, విలువ_2) < 0) {
		తిరిగి -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం format_config_color(నిర్మాణం strbuf *buf,
			       స్థిరరాశి అక్షరం *కీలకం_2,
			       స్థిరరాశి అక్షరం *విలువ_2,
			       పూర్ణాంకం gently)
{
	అక్షరం v[COLOR_MAXLEN];

	ఒకవేళ (gently) {
		ఒకవేళ (color_parse_quietly(విలువ_2, v) < 0)
			తిరిగి -1;
	} లేకపోతే ఒకవేళ (git_config_color(v, కీలకం_2, విలువ_2) < 0) {
		తిరిగి -1;
	}

	strbuf_addstr(buf, v);
	తిరిగి 0;
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
స్థిర పూర్ణాంకం format_config(స్థిరరాశి నిర్మాణం config_display_options *opts,
			 నిర్మాణం strbuf *buf, స్థిరరాశి అక్షరం *కీలకం_2,
			 స్థిరరాశి అక్షరం *విలువ_2, స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi,
			 పూర్ణాంకం gently)
{
	పూర్ణాంకం res = 0;
	ఒకవేళ (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	ఒకవేళ (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	ఒకవేళ (opts->show_కీలకాలు)
		strbuf_addstr(buf, కీలకం_2);

	ఒకవేళ (opts->omit_విలువలు)
		వెళ్ళు terminator;

	ఒకవేళ (opts->show_కీలకాలు)
		strbuf_addch(buf, opts->కీలకం_delim);

	ఎంపిక (opts->type) {
	స్థితి TYPE_INT:
		res = format_config_int64(buf, కీలకం_2, విలువ_2, kvi, gently);
		ఆపు;

	స్థితి TYPE_BOOL:
		res = format_config_bool(buf, కీలకం_2, విలువ_2, gently);
		ఆపు;

	స్థితి TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, కీలకం_2, విలువ_2, kvi, gently);
		ఆపు;

	స్థితి TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, విలువ_2);
		ఆపు;

	స్థితి TYPE_మార్గం:
		res = format_config_మార్గం(buf, కీలకం_2, విలువ_2, gently);
		ఆపు;

	స్థితి TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, కీలకం_2, విలువ_2, gently);
		ఆపు;

	స్థితి TYPE_COLOR:
		res = format_config_color(buf, కీలకం_2, విలువ_2, gently);
		ఆపు;

	స్థితి TYPE_NONE:
		ఒకవేళ (విలువ_2) {
			strbuf_addstr(buf, విలువ_2);
		} లేకపోతే {
			/* Just show the key name; back out delimiter */
			ఒకవేళ (opts->show_కీలకాలు)
				strbuf_setlen(buf, buf->len - 1);
		}
		ఆపు;

	అప్రమేయ:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->పదం);
	తిరిగి res;
}

స్థిర పూర్ణాంకం show_all_config(స్థిరరాశి అక్షరం *కీలకం_2, స్థిరరాశి అక్షరం *విలువ_2,
			   స్థిరరాశి నిర్మాణం config_context *ctx,
			   శూన్య *cb)
{
	స్థిరరాశి నిర్మాణం config_display_options *opts = cb;
	స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi = ctx->kvi;
	నిర్మాణం strbuf formatted = STRBUF_INIT;

	ఒకవేళ (format_config(opts, &formatted, కీలకం_2, విలువ_2, kvi, 1) >= 0)
		fwrite(formatted.buf, 1, formatted.len, stdout);

	strbuf_release(&formatted);
	తిరిగి 0;
}

#నిర్వచించు GET_విలువ_ALL        (1 << 0)
#నిర్వచించు GET_విలువ_కీలకం_REGEXP (1 << 1)

నిర్మాణం collect_config_దత్తాంశం {
	స్థిరరాశి నిర్మాణం config_display_options *display_opts;
	నిర్మాణం strbuf_list *విలువలు;
	స్థిరరాశి అక్షరం *విలువ_నమూనా;
	స్థిరరాశి అక్షరం *కీలకం;
	regex_t *regexp;
	regex_t *కీలకం_regexp;
	పూర్ణాంకం do_not_సరిపోలిక;
	గుర్తులేని get_విలువ_flags;
	గుర్తులేని flags;
};

స్థిర పూర్ణాంకం collect_config(స్థిరరాశి అక్షరం *కీలకం_2, స్థిరరాశి అక్షరం *విలువ_2,
			  స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం collect_config_దత్తాంశం *దత్తాంశం = cb;
	నిర్మాణం strbuf_list *విలువలు = దత్తాంశం->విలువలు;
	స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi = ctx->kvi;
	పూర్ణాంకం స్థితి_2;

	ఒకవేళ (!(దత్తాంశం->get_విలువ_flags & GET_విలువ_కీలకం_REGEXP) &&
	    strcmp(కీలకం_2, దత్తాంశం->కీలకం))
		తిరిగి 0;
	ఒకవేళ ((దత్తాంశం->get_విలువ_flags & GET_విలువ_కీలకం_REGEXP) &&
	    regexec(దత్తాంశం->కీలకం_regexp, కీలకం_2, 0, ఖాళీ, 0))
		తిరిగి 0;
	ఒకవేళ ((దత్తాంశం->flags & CONFIG_FLAGS_FIXED_విలువ) &&
	    strcmp(దత్తాంశం->విలువ_నమూనా, (విలువ_2?విలువ_2:"")))
		తిరిగి 0;
	ఒకవేళ (దత్తాంశం->regexp &&
	    (దత్తాంశం->do_not_సరిపోలిక ^ !!regexec(దత్తాంశం->regexp, (విలువ_2?విలువ_2:""), 0, ఖాళీ, 0)))
		తిరిగి 0;

	ALLOC_GROW(విలువలు->వస్తువులు, విలువలు->nr + 1, విలువలు->alloc);
	strbuf_init(&విలువలు->వస్తువులు[విలువలు->nr], 0);

	స్థితి_2 = format_config(దత్తాంశం->display_opts, &విలువలు->వస్తువులు[విలువలు->nr++],
			       కీలకం_2, విలువ_2, kvi, 0);
	ఒకవేళ (స్థితి_2 < 0)
		తిరిగి స్థితి_2;
	ఒకవేళ (స్థితి_2) {
		strbuf_release(&విలువలు->వస్తువులు[--విలువలు->nr]);
		స్థితి_2 = 0;
	}
	తిరిగి స్థితి_2;
}

స్థిర పూర్ణాంకం get_విలువ(స్థిరరాశి నిర్మాణం config_location_options *opts,
		     స్థిరరాశి నిర్మాణం config_display_options *display_opts,
		     స్థిరరాశి అక్షరం *కీలకం_2, స్థిరరాశి అక్షరం *regex_,
		     గుర్తులేని get_విలువ_flags, గుర్తులేని flags)
{
	పూర్ణాంకం ret = CONFIG_GENERIC_ERROR;
	నిర్మాణం strbuf_list విలువలు = {ఖాళీ};
	నిర్మాణం collect_config_దత్తాంశం దత్తాంశం = {
		.display_opts = display_opts,
		.విలువలు = &విలువలు,
		.get_విలువ_flags = get_విలువ_flags,
		.flags = flags,
	};
	అక్షరం *కీలకం = ఖాళీ;
	పూర్ణాంకం i;

	ఒకవేళ (get_విలువ_flags & GET_విలువ_కీలకం_REGEXP) {
		అక్షరం *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		కీలకం = xstrdup(కీలకం_2);
		కొరకు (tl = కీలకం + అక్షరశ్రేణి_పొడవు(కీలకం) - 1;
		     tl >= కీలకం && *tl != '.';
		     tl--)
			*tl = tolower(*tl);
		కొరకు (tl = కీలకం; *tl && *tl != '.'; tl++)
			*tl = tolower(*tl);

		దత్తాంశం.కీలకం_regexp = (regex_t*)xmalloc(పరిమాణం(regex_t));
		ఒకవేళ (regcomp(దత్తాంశం.కీలకం_regexp, కీలకం, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), కీలకం_2);
			FREE_AND_NULL(దత్తాంశం.కీలకం_regexp);
			ret = CONFIG_INVALID_నమూనా;
			వెళ్ళు free_అక్షరశ్రేణులు;
		}
	} లేకపోతే {
		ఒకవేళ (git_config_parse_కీలకం(కీలకం_2, &కీలకం, ఖాళీ)) {
			ret = CONFIG_INVALID_కీలకం;
			వెళ్ళు free_అక్షరశ్రేణులు;
		}

		దత్తాంశం.కీలకం = కీలకం;
	}

	ఒకవేళ (regex_ && (flags & CONFIG_FLAGS_FIXED_విలువ))
		దత్తాంశం.విలువ_నమూనా = regex_;
	లేకపోతే ఒకవేళ (regex_) {
		ఒకవేళ (regex_[0] == '!') {
			దత్తాంశం.do_not_సరిపోలిక = 1;
			regex_++;
		}

		దత్తాంశం.regexp = (regex_t*)xmalloc(పరిమాణం(regex_t));
		ఒకవేళ (regcomp(దత్తాంశం.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(దత్తాంశం.regexp);
			ret = CONFIG_INVALID_నమూనా;
			వెళ్ళు free_అక్షరశ్రేణులు;
		}
	}

	config_with_options(collect_config, &దత్తాంశం,
			    &opts->ఆధారం, the_repository,
			    &opts->options);

	ఒకవేళ (!విలువలు.nr && display_opts->default_విలువ) {
		నిర్మాణం కీలకం_విలువ_info kvi = KVI_INIT;
		నిర్మాణం strbuf *అంశం;
		పూర్ణాంకం స్థితి_2;

		kvi_from_param(&kvi);
		ALLOC_GROW(విలువలు.వస్తువులు, విలువలు.nr + 1, విలువలు.alloc);
		అంశం = &విలువలు.వస్తువులు[విలువలు.nr++];
		strbuf_init(అంశం, 0);

		స్థితి_2 = format_config(display_opts, అంశం, కీలకం_2,
				       display_opts->default_విలువ, &kvi, 0);
		ఒకవేళ (స్థితి_2 < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_విలువ);
		ఒకవేళ (స్థితి_2) {
			/* default was a missing optional value */
			విలువలు.nr--;
			strbuf_release(అంశం);
		}
	}

	ret = !విలువలు.nr;

	కొరకు (i = 0; i < విలువలు.nr; i++) {
		నిర్మాణం strbuf *buf = విలువలు.వస్తువులు + i;
		ఒకవేళ ((get_విలువ_flags & GET_విలువ_ALL) || i == విలువలు.nr - 1)
			fwrite(buf->buf, 1, buf->len, stdout);
		strbuf_release(buf);
	}
	విడుదల(విలువలు.వస్తువులు);

free_అక్షరశ్రేణులు:
	విడుదల(కీలకం);
	ఒకవేళ (దత్తాంశం.కీలకం_regexp) {
		regfree(దత్తాంశం.కీలకం_regexp);
		విడుదల(దత్తాంశం.కీలకం_regexp);
	}
	ఒకవేళ (దత్తాంశం.regexp) {
		regfree(దత్తాంశం.regexp);
		విడుదల(దత్తాంశం.regexp);
	}

	తిరిగి ret;
}

స్థిర అక్షరం *normalize_విలువ(స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *విలువ,
			     పూర్ణాంకం type, నిర్మాణం కీలకం_విలువ_info *kvi)
{
	ఒకవేళ (!విలువ)
		తిరిగి ఖాళీ;

	ఒకవేళ (type == 0 || type == TYPE_మార్గం || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		తిరిగి xstrdup(విలువ);
	ఒకవేళ (type == TYPE_INT)
		తిరిగి xstrfmt("%"PRId64, git_config_int64(కీలకం, విలువ, kvi));
	ఒకవేళ (type == TYPE_BOOL)
		తిరిగి xstrdup(git_config_bool(కీలకం, విలువ) ?  "true" : "false");
	ఒకవేళ (type == TYPE_BOOL_OR_INT) {
		పూర్ణాంకం is_bool, v;
		v = git_config_bool_or_int(కీలకం, విలువ, kvi, &is_bool);
		ఒకవేళ (!is_bool)
			తిరిగి xstrfmt("%d", v);
		లేకపోతే
			తిరిగి xstrdup(v ? "true" : "false");
	}
	ఒకవేళ (type == TYPE_BOOL_OR_STR) {
		పూర్ణాంకం v = git_parse_maybe_bool(విలువ);
		ఒకవేళ (v < 0)
			తిరిగి xstrdup(విలువ);
		లేకపోతే
			తిరిగి xstrdup(v ? "true" : "false");
	}
	ఒకవేళ (type == TYPE_COLOR) {
		అక్షరం v[COLOR_MAXLEN];
		ఒకవేళ (git_config_color(v, కీలకం, విలువ))
			die(_("cannot parse color '%s'"), విలువ);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		తిరిగి xstrdup(విలువ);
	}

	BUG("cannot normalize type %d", type);
}

నిర్మాణం get_color_config_దత్తాంశం {
	పూర్ణాంకం get_color_దొరికింది;
	స్థిరరాశి అక్షరం *get_color_స్థానం;
	అక్షరం parsed_color[COLOR_MAXLEN];
};

స్థిర పూర్ణాంకం git_get_color_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
				స్థిరరాశి నిర్మాణం config_context *ctx వాడని,
				శూన్య *cb)
{
	నిర్మాణం get_color_config_దత్తాంశం *దత్తాంశం = cb;

	ఒకవేళ (!strcmp(var, దత్తాంశం->get_color_స్థానం)) {
		ఒకవేళ (!విలువ)
			config_error_nonbool(var);
		ఒకవేళ (color_parse(విలువ, దత్తాంశం->parsed_color) < 0)
			తిరిగి -1;
		దత్తాంశం->get_color_దొరికింది = 1;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_color(స్థిరరాశి నిర్మాణం config_location_options *opts,
		      స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *def_color)
{
	నిర్మాణం get_color_config_దత్తాంశం దత్తాంశం = {
		.get_color_స్థానం = var,
		.parsed_color[0] = '\0',
	};
	పూర్ణాంకం ret;

	config_with_options(git_get_color_config, &దత్తాంశం,
			    &opts->ఆధారం, the_repository,
			    &opts->options);

	ఒకవేళ (!దత్తాంశం.get_color_దొరికింది && def_color) {
		ఒకవేళ (color_parse(def_color, దత్తాంశం.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			వెళ్ళు బహిర్గతం;
		}
	}

	ret = 0;

బహిర్గతం:
	fputs(దత్తాంశం.parsed_color, stdout);
	తిరిగి ret;
}

నిర్మాణం get_colorbool_config_దత్తాంశం {
	గణన git_colorbool get_colorbool_దొరికింది;
	గణన git_colorbool get_diff_color_దొరికింది;
	గణన git_colorbool get_color_ui_దొరికింది;
	స్థిరరాశి అక్షరం *get_colorbool_స్థానం;
};

స్థిర పూర్ణాంకం git_get_colorbool_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
				    స్థిరరాశి నిర్మాణం config_context *ctx వాడని,
				    శూన్య *cb)
{
	నిర్మాణం get_colorbool_config_దత్తాంశం *దత్తాంశం = cb;

	ఒకవేళ (!strcmp(var, దత్తాంశం->get_colorbool_స్థానం))
		దత్తాంశం->get_colorbool_దొరికింది = git_config_colorbool(var, విలువ);
	లేకపోతే ఒకవేళ (!strcmp(var, "diff.color"))
		దత్తాంశం->get_diff_color_దొరికింది = git_config_colorbool(var, విలువ);
	లేకపోతే ఒకవేళ (!strcmp(var, "color.ui"))
		దత్తాంశం->get_color_ui_దొరికింది = git_config_colorbool(var, విలువ);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_colorbool(స్థిరరాశి నిర్మాణం config_location_options *opts,
			 స్థిరరాశి అక్షరం *var, పూర్ణాంకం print)
{
	నిర్మాణం get_colorbool_config_దత్తాంశం దత్తాంశం = {
		.get_colorbool_స్థానం = var,
		.get_colorbool_దొరికింది = GIT_COLOR_UNKNOWN,
		.get_diff_color_దొరికింది = GIT_COLOR_UNKNOWN,
		.get_color_ui_దొరికింది = GIT_COLOR_UNKNOWN,
	};
	bool ఫలితం;

	config_with_options(git_get_colorbool_config, &దత్తాంశం,
			    &opts->ఆధారం, the_repository,
			    &opts->options);

	ఒకవేళ (దత్తాంశం.get_colorbool_దొరికింది == GIT_COLOR_UNKNOWN) {
		ఒకవేళ (!strcmp(దత్తాంశం.get_colorbool_స్థానం, "color.diff"))
			దత్తాంశం.get_colorbool_దొరికింది = దత్తాంశం.get_diff_color_దొరికింది;
		ఒకవేళ (దత్తాంశం.get_colorbool_దొరికింది == GIT_COLOR_UNKNOWN)
			దత్తాంశం.get_colorbool_దొరికింది = దత్తాంశం.get_color_ui_దొరికింది;
	}

	ఒకవేళ (దత్తాంశం.get_colorbool_దొరికింది == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		దత్తాంశం.get_colorbool_దొరికింది = GIT_COLOR_AUTO;

	ఫలితం = want_color(దత్తాంశం.get_colorbool_దొరికింది);

	ఒకవేళ (print) {
		రాయి("%s\n", ఫలితం ? "true" : "false");
		తిరిగి 0;
	} లేకపోతే
		తిరిగి ఫలితం ? 0 : 1;
}

స్థిర శూన్య check_write(స్థిరరాశి నిర్మాణం git_config_ఆధారం *ఆధారం)
{
	ఒకవేళ (!ఆధారం->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	ఒకవేళ (ఆధారం->use_stdin)
		die(_("writing to stdin is not supported"));

	ఒకవేళ (ఆధారం->blob)
		die(_("writing config blobs is not supported"));
}

నిర్మాణం urlmatch_current_candidate_విలువ {
	అక్షరం విలువ_is_null;
	నిర్మాణం strbuf విలువ;
	నిర్మాణం కీలకం_విలువ_info kvi;
};

స్థిర పూర్ణాంకం urlmatch_collect_fn(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			       స్థిరరాశి నిర్మాణం config_context *ctx,
			       శూన్య *cb)
{
	నిర్మాణం string_list *విలువలు = cb;
	నిర్మాణం string_list_అంశం *అంశం = string_list_చొప్పించు(విలువలు, var);
	నిర్మాణం urlmatch_current_candidate_విలువ *matched = అంశం->util;
	స్థిరరాశి నిర్మాణం కీలకం_విలువ_info *kvi = ctx->kvi;

	ఒకవేళ (!matched) {
		matched = xmalloc(పరిమాణం(*matched));
		strbuf_init(&matched->విలువ, 0);
		అంశం->util = matched;
	} లేకపోతే {
		strbuf_reset(&matched->విలువ);
	}
	matched->kvi = *kvi;

	ఒకవేళ (విలువ) {
		strbuf_addstr(&matched->విలువ, విలువ);
		matched->విలువ_is_null = 0;
	} లేకపోతే {
		matched->విలువ_is_null = 1;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_urlmatch(స్థిరరాశి నిర్మాణం config_location_options *opts,
			స్థిరరాశి నిర్మాణం config_display_options *_display_opts,
			స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *url)
{
	పూర్ణాంకం ret;
	అక్షరం *section;
	అక్షరం *section_తోక;
	నిర్మాణం config_display_options display_opts = *_display_opts;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం urlmatch_config config = URLMATCH_CONFIG_INIT;
	నిర్మాణం string_list విలువలు = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = ఖాళీ;
	config.cb = &విలువలు;

	ఒకవేళ (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_తోక = strchr(section, '.');
	ఒకవేళ (section_తోక) {
		*section_తోక = '\0';
		config.కీలకం = section_తోక + 1;
		display_opts.show_కీలకాలు = 0;
	} లేకపోతే {
		config.కీలకం = ఖాళీ;
		display_opts.show_కీలకాలు = 1;
	}

	config_with_options(urlmatch_config_నమోదు, &config,
			    &opts->ఆధారం, the_repository,
			    &opts->options);

	ret = !విలువలు.nr;

	for_each_string_list_అంశం(అంశం, &విలువలు) {
		నిర్మాణం urlmatch_current_candidate_విలువ *matched = అంశం->util;
		నిర్మాణం strbuf buf = STRBUF_INIT;
		పూర్ణాంకం స్థితి_2;

		స్థితి_2 = format_config(&display_opts, &buf, అంశం->string,
				       matched->విలువ_is_null ? ఖాళీ : matched->విలువ.buf,
				       &matched->kvi, 0);
		ఒకవేళ (!స్థితి_2)
			fwrite(buf.buf, 1, buf.len, stdout);
		strbuf_release(&buf);

		strbuf_release(&matched->విలువ);
	}
	urlmatch_config_release(&config);
	string_list_clear(&విలువలు, 1);
	విడుదల(config.url.url);

	విడుదల(section);
	తిరిగి ret;
}

స్థిర అక్షరం *default_user_config(శూన్య)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	తిరిగి strbuf_detach(&buf, ఖాళీ);
}

స్థిర శూన్య location_options_init(నిర్మాణం config_location_options *opts,
				  స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	ఒకవేళ (!opts->ఆధారం.file)
		opts->ఆధారం.file = opts->file_గమ్యం_free =
			xstrdup_or_null(getenv(CONFIG_ENVIRONMENT));

	ఒకవేళ (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->ఆధారం.file + !!opts->ఆధారం.blob > 1) {
		error(_("only one config file at a time"));
		నిష్క్రమించు(129);
	}

	ఒకవేళ (!startup_info->have_repository) {
		ఒకవేళ (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		ఒకవేళ (opts->ఆధారం.blob)
			die(_("--blob can only be used inside a git repository"));
		ఒకవేళ (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	ఒకవేళ (opts->ఆధారం.file &&
			!strcmp(opts->ఆధారం.file, "-")) {
		opts->ఆధారం.file = ఖాళీ;
		opts->ఆధారం.use_stdin = 1;
		opts->ఆధారం.scope = CONFIG_SCOPE_COMMAND;
	}

	ఒకవేళ (opts->use_global_config) {
		opts->ఆధారం.file = opts->file_గమ్యం_free = git_global_config();
		ఒకవేళ (!opts->ఆధారం.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->ఆధారం.scope = CONFIG_SCOPE_GLOBAL;
	} లేకపోతే ఒకవేళ (opts->use_system_config) {
		opts->ఆధారం.file = opts->file_గమ్యం_free = git_system_config();
		opts->ఆధారం.scope = CONFIG_SCOPE_SYSTEM;
	} లేకపోతే ఒకవేళ (opts->use_local_config) {
		opts->ఆధారం.file = opts->file_గమ్యం_free = repo_git_మార్గం(the_repository, "config");
		opts->ఆధారం.scope = CONFIG_SCOPE_LOCAL;
	} లేకపోతే ఒకవేళ (opts->use_worktree_config) {
		నిర్మాణం worktree **worktrees = get_worktrees(the_repository);
		ఒకవేళ (the_repository->repository_format_worktree_config)
			opts->ఆధారం.file = opts->file_గమ్యం_free =
				repo_git_మార్గం(the_repository, "config.worktree");
		లేకపోతే ఒకవేళ (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		లేకపోతే
			opts->ఆధారం.file = opts->file_గమ్యం_free =
				repo_git_మార్గం(the_repository, "config");
		opts->ఆధారం.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} లేకపోతే ఒకవేళ (opts->ఆధారం.file) {
		ఒకవేళ (!is_absolute_మార్గం(opts->ఆధారం.file) && ఉపసర్గ)
			opts->ఆధారం.file = opts->file_గమ్యం_free =
				ఉపసర్గ_filename(ఉపసర్గ, opts->ఆధారం.file);
		opts->ఆధారం.scope = CONFIG_SCOPE_COMMAND;
	} లేకపోతే ఒకవేళ (opts->ఆధారం.blob) {
		opts->ఆధారం.scope = CONFIG_SCOPE_COMMAND;
	}

	ఒకవేళ (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->ఆధారం.file;
	లేకపోతే
		opts->options.respect_includes = opts->respect_includes_opt;
	ఒకవేళ (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

స్థిర శూన్య location_options_release(నిర్మాణం config_location_options *opts)
{
	విడుదల(opts->file_గమ్యం_free);
}

స్థిర శూన్య display_options_init(నిర్మాణం config_display_options *opts)
{
	ఒకవేళ (opts->చివర_nul) {
		opts->పదం = '\0';
		opts->delim = '\n';
		opts->కీలకం_delim = '\n';
	}
}

స్థిర శూన్య display_options_init_list(నిర్మాణం config_display_options *opts)
{
	opts->show_కీలకాలు = 1;

	ఒకవేళ (opts->చివర_nul) {
		display_options_init(opts);
	} లేకపోతే {
		opts->పదం = '\n';
		opts->delim = ' ';
		opts->కీలకం_delim = '=';
	}
}

స్థిర పూర్ణాంకం cmd_config_list(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	నిర్మాణం config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_సమూహం(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, opts, builtin_config_list_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, ఉపసర్గ);
	display_options_init_list(&display_opts);

	అమరిక_auto_pager("config", 1);

	ఒకవేళ (config_with_options(show_all_config, &display_opts,
				&location_opts.ఆధారం, the_repository,
				&location_opts.options) < 0) {
		ఒకవేళ (location_opts.ఆధారం.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.ఆధారం.file);
		లేకపోతే
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_config_get(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	నిర్మాణం config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	స్థిరరాశి అక్షరం *విలువ_నమూనా = ఖాళీ, *url = ఖాళీ;
	పూర్ణాంకం flags = 0;
	గుర్తులేని get_విలువ_flags = 0;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_సమూహం(N_("Filter options")),
		OPT_బిట్టు(0, "all", &get_విలువ_flags, N_("return all values for multi-valued config options"), GET_విలువ_ALL),
		OPT_బిట్టు(0, "regexp", &get_విలువ_flags, N_("interpret the name as a regular expression"), GET_విలువ_కీలకం_REGEXP),
		OPT_STRING(0, "value", &విలువ_నమూనా, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_బిట్టు(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_విలువ),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_సమూహం(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_విలువ,
			   N_("value"), N_("use default value when missing entry")),
		OPT_చివర(),
	};
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ఒకవేళ ((flags & CONFIG_FLAGS_FIXED_విలువ) && !విలువ_నమూనా)
		die(_("--fixed-value only applies with 'value-pattern'"));
	ఒకవేళ (display_opts.default_విలువ &&
	    ((get_విలువ_flags & GET_విలువ_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	ఒకవేళ (url && ((get_విలువ_flags & GET_విలువ_ALL) ||
		    (get_విలువ_flags & GET_విలువ_కీలకం_REGEXP) ||
		    విలువ_నమూనా))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, ఉపసర్గ);
	display_options_init(&display_opts);

	ఒకవేళ (display_opts.type != TYPE_COLOR)
		అమరిక_auto_pager("config", 1);

	ఒకవేళ (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	లేకపోతే ఒకవేళ (display_opts.type == TYPE_COLOR && !అక్షరశ్రేణి_పొడవు(argv[0]) && display_opts.default_విలువ)
		ret = get_color(&location_opts, "", display_opts.default_విలువ);
	లేకపోతే
		ret = get_విలువ(&location_opts, &display_opts, argv[0], విలువ_నమూనా,
				get_విలువ_flags, flags);

	location_options_release(&location_opts);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_config_సమితి(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			  నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	స్థిరరాశి అక్షరం *విలువ_నమూనా = ఖాళీ, *comment_arg = ఖాళీ;
	అక్షరం *comment = ఖాళీ;
	పూర్ణాంకం flags = 0, append = 0, type = 0;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_సమూహం(N_("Filter")),
		OPT_బిట్టు(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &విలువ_నమూనా, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_బిట్టు(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_విలువ),
		OPT_సమూహం(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_చివర(),
	};
	నిర్మాణం కీలకం_విలువ_info default_kvi = KVI_INIT;
	అక్షరం *విలువ;
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, builtin_config_సమితి_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ఒకవేళ (argc == 1)
		die_missing_సమితి_విలువ(argv[0]);
	check_argc(argc, 2, 2);

	ఒకవేళ ((flags & CONFIG_FLAGS_FIXED_విలువ) && !విలువ_నమూనా)
		die(_("--fixed-value only applies with --value=<pattern>"));
	ఒకవేళ (append && విలువ_నమూనా)
		die(_("--append cannot be used with --value=<pattern>"));
	ఒకవేళ (append)
		విలువ_నమూనా = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, ఉపసర్గ);
	check_write(&location_opts.ఆధారం);

	విలువ = normalize_విలువ(argv[0], argv[1], type, &default_kvi);

	ఒకవేళ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || విలువ_నమూనా) {
		ret = repo_config_సమితి_multivar_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
							      argv[0], విలువ, విలువ_నమూనా,
							      comment, flags);
	} లేకపోతే {
		ret = repo_config_సమితి_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
						     argv[0], comment, విలువ);
		ఒకవేళ (ret == CONFIG_NOTHING_సమితి)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	విడుదల(comment);
	విడుదల(విలువ);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_config_unset(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			    నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	స్థిరరాశి అక్షరం *విలువ_నమూనా = ఖాళీ;
	పూర్ణాంకం flags = 0;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_సమూహం(N_("Filter")),
		OPT_బిట్టు(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &విలువ_నమూనా, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_బిట్టు(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_విలువ),
		OPT_చివర(),
	};
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	ఒకవేళ ((flags & CONFIG_FLAGS_FIXED_విలువ) && !విలువ_నమూనా)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, ఉపసర్గ);
	check_write(&location_opts.ఆధారం);

	ఒకవేళ ((flags & CONFIG_FLAGS_MULTI_REPLACE) || విలువ_నమూనా)
		ret = repo_config_సమితి_multivar_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
							      argv[0], ఖాళీ, విలువ_నమూనా,
							      ఖాళీ, flags);
	లేకపోతే
		ret = repo_config_సమితి_లోపల_file_gently(the_repository, location_opts.ఆధారం.file, argv[0],
						     ఖాళీ, ఖాళీ);

	location_options_release(&location_opts);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_config_rename_section(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				     నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_చివర(),
	};
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, ఉపసర్గ);
	check_write(&location_opts.ఆధారం);

	ret = repo_config_rename_section_లోపల_file(the_repository, location_opts.ఆధారం.file,
						 argv[0], argv[1]);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;
	లేకపోతే ఒకవేళ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

బహిర్గతం:
	location_options_release(&location_opts);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_config_remove_section(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				     నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_చివర(),
	};
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, ఉపసర్గ);
	check_write(&location_opts.ఆధారం);

	ret = repo_config_rename_section_లోపల_file(the_repository, location_opts.ఆధారం.file,
						 argv[0], ఖాళీ);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;
	లేకపోతే ఒకవేళ (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

బహిర్గతం:
	location_options_release(&location_opts);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం show_editor(నిర్మాణం config_location_options *opts)
{
	అక్షరం *config_file;

	ఒకవేళ (!opts->ఆధారం.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	ఒకవేళ (opts->ఆధారం.use_stdin)
		die(_("editing stdin is not supported"));
	ఒకవేళ (opts->ఆధారం.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, ఖాళీ);
	config_file = opts->ఆధారం.file ?
			xstrdup(opts->ఆధారం.file) :
			repo_git_మార్గం(the_repository, "config");
	ఒకవేళ (opts->use_global_config) {
		పూర్ణాంకం fd = open(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		ఒకవేళ (fd >= 0) {
			అక్షరం *content = default_user_config();
			write_str_లోపల_నిండిన(fd, content);
			విడుదల(content);
			close(fd);
		}
		లేకపోతే ఒకవేళ (errno != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	ఒకవేళ (launch_editor(config_file, ఖాళీ, ఖాళీ)) {
		విడుదల(config_file);
		తిరిగి -1;
	}
	విడుదల(config_file);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_config_edit(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_చివర(),
	};
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, ఉపసర్గ);
	check_write(&location_opts.ఆధారం);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_config_actions(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	గణన {
		ACTION_GET = (1<<0),
		ACTION_GET_ALL = (1<<1),
		ACTION_GET_REGEXP = (1<<2),
		ACTION_REPLACE_ALL = (1<<3),
		ACTION_కూడు = (1<<4),
		ACTION_UNSET = (1<<5),
		ACTION_UNSET_ALL = (1<<6),
		ACTION_RENAME_SECTION = (1<<7),
		ACTION_REMOVE_SECTION = (1<<8),
		ACTION_LIST = (1<<9),
		ACTION_EDIT = (1<<10),
		ACTION_సమితి = (1<<11),
		ACTION_సమితి_ALL = (1<<12),
		ACTION_GET_COLOR = (1<<13),
		ACTION_GET_COLORBOOL = (1<<14),
		ACTION_GET_URLMATCH = (1<<15),
	};
	నిర్మాణం config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	నిర్మాణం config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	స్థిరరాశి అక్షరం *comment_arg = ఖాళీ;
	పూర్ణాంకం actions = 0;
	గుర్తులేని flags = 0;
	నిర్మాణం option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_సమూహం(N_("Action")),
		OPT_CMDMODE(0, "get", &actions, N_("get value: name [<value-pattern>]"), ACTION_GET),
		OPT_CMDMODE(0, "get-all", &actions, N_("get all values: key [<value-pattern>]"), ACTION_GET_ALL),
		OPT_CMDMODE(0, "get-regexp", &actions, N_("get values for regexp: name-regex [<value-pattern>]"), ACTION_GET_REGEXP),
		OPT_CMDMODE(0, "get-urlmatch", &actions, N_("get value specific for the URL: section[.var] URL"), ACTION_GET_URLMATCH),
		OPT_CMDMODE(0, "replace-all", &actions, N_("replace all matching variables: name value [<value-pattern>]"), ACTION_REPLACE_ALL),
		OPT_CMDMODE(0, "add", &actions, N_("add a new variable: name value"), ACTION_కూడు),
		OPT_CMDMODE(0, "unset", &actions, N_("remove a variable: name [<value-pattern>]"), ACTION_UNSET),
		OPT_CMDMODE(0, "unset-all", &actions, N_("remove all matches: name [<value-pattern>]"), ACTION_UNSET_ALL),
		OPT_CMDMODE(0, "rename-section", &actions, N_("rename section: old-name new-name"), ACTION_RENAME_SECTION),
		OPT_CMDMODE(0, "remove-section", &actions, N_("remove a section: name"), ACTION_REMOVE_SECTION),
		OPT_CMDMODE('l', "list", &actions, N_("list all"), ACTION_LIST),
		OPT_CMDMODE('e', "edit", &actions, N_("open an editor"), ACTION_EDIT),
		OPT_CMDMODE(0, "get-color", &actions, N_("find the color configured: slot [<default>]"), ACTION_GET_COLOR),
		OPT_CMDMODE(0, "get-colorbool", &actions, N_("find the color setting: slot [<stdout-is-tty>]"), ACTION_GET_COLORBOOL),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_సమూహం(N_("Other")),
		OPT_STRING(0, "default", &display_opts.default_విలువ,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_బిట్టు(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_విలువ),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_చివర(),
	};
	అక్షరం *విలువ = ఖాళీ, *comment = ఖాళీ;
	పూర్ణాంకం ret = 0;
	పూర్ణాంకం actions_implicit;
	నిర్మాణం కీలకం_విలువ_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, ఉపసర్గ, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, ఉపసర్గ);
	display_options_init(&display_opts);

	ఒకవేళ ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		నిష్క్రమించు(129);
	}

	actions_implicit = (actions == 0);
	ఒకవేళ (actions_implicit)
		ఎంపిక (argc) {
		స్థితి 1: actions = ACTION_GET; ఆపు;
		స్థితి 2: actions = ACTION_సమితి; ఆపు;
		స్థితి 3: actions = ACTION_సమితి_ALL; ఆపు;
		అప్రమేయ:
			error(_("no action specified"));
			నిష్క్రమించు(129);
		}
	ఒకవేళ (actions_implicit && argc == 1) {
		స్థిరరాశి అక్షరం *చివరి_dot = strrchr(argv[0], '.');
		ఒకవేళ (చివరి_dot && strchr(చివరి_dot + 1, '='))
			die_missing_సమితి_విలువ(argv[0]);
	}
	ఒకవేళ (display_opts.omit_విలువలు &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		నిష్క్రమించు(129);
	}

	ఒకవేళ (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		నిష్క్రమించు(129);
	}

	ఒకవేళ (display_opts.default_విలువ && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		నిష్క్రమించు(129);
	}

	ఒకవేళ (comment_arg &&
	    !(actions & (ACTION_కూడు|ACTION_సమితి|ACTION_సమితి_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		నిష్క్రమించు(129);
	}

	/* check usage of --fixed-value */
	ఒకవేళ (flags & CONFIG_FLAGS_FIXED_విలువ) {
		పూర్ణాంకం allowed_usage = 0;

		ఎంపిక (actions) {
		/* git config --get <name> <value-pattern> */
		స్థితి ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		స్థితి ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		స్థితి ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		స్థితి ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		స్థితి ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			ఆపు;

		/* git config <name> <value> <value-pattern> */
		స్థితి ACTION_సమితి_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		స్థితి ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			ఆపు;

		/* other options don't allow --fixed-value */
		}

		ఒకవేళ (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			నిష్క్రమించు(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	ఒకవేళ (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		అమరిక_auto_pager("config", 1);

	ఒకవేళ (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		ఒకవేళ (config_with_options(show_all_config, &display_opts,
					&location_opts.ఆధారం, the_repository,
					&location_opts.options) < 0) {
			ఒకవేళ (location_opts.ఆధారం.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.ఆధారం.file);
			లేకపోతే
				die(_("error processing config file(s)"));
		}
	}
	లేకపోతే ఒకవేళ (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_సమితి) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 2, 2);
		విలువ = normalize_విలువ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_సమితి_లోపల_file_gently(the_repository, location_opts.ఆధారం.file, argv[0], comment, విలువ);
		ఒకవేళ (ret == CONFIG_NOTHING_సమితి)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_సమితి_ALL) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 2, 3);
		విలువ = normalize_విలువ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_సమితి_multivar_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
							      argv[0], విలువ, argv[2],
							      comment, flags);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_కూడు) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 2, 2);
		విలువ = normalize_విలువ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_సమితి_multivar_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
							      argv[0], విలువ,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 2, 3);
		విలువ = normalize_విలువ(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_సమితి_multivar_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
							      argv[0], విలువ, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_విలువ(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_విలువ(&location_opts, &display_opts, argv[0], argv[1],
				GET_విలువ_ALL, flags);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_GET_REGEXP) {
		display_opts.show_కీలకాలు = 1;
		check_argc(argc, 1, 2);
		ret = get_విలువ(&location_opts, &display_opts, argv[0], argv[1],
				GET_విలువ_ALL|GET_విలువ_కీలకం_REGEXP, flags);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_UNSET) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 1, 2);
		ఒకవేళ (argc == 2)
			ret = repo_config_సమితి_multivar_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
								      argv[0], ఖాళీ, argv[1],
								      ఖాళీ, flags);
		లేకపోతే
			ret = repo_config_సమితి_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
							     argv[0], ఖాళీ, ఖాళీ);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 1, 2);
		ret = repo_config_సమితి_multivar_లోపల_file_gently(the_repository, location_opts.ఆధారం.file,
							      argv[0], ఖాళీ, argv[1],
							      ఖాళీ, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_లోపల_file(the_repository, location_opts.ఆధారం.file,
							 argv[0], argv[1]);
		ఒకవేళ (ret < 0)
			వెళ్ళు బహిర్గతం;
		లేకపోతే ఒకవేళ (!ret)
			die(_("no such section: %s"), argv[0]);
		లేకపోతే
			ret = 0;
	}
	లేకపోతే ఒకవేళ (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.ఆధారం);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_లోపల_file(the_repository, location_opts.ఆధారం.file,
							 argv[0], ఖాళీ);
		ఒకవేళ (ret < 0)
			వెళ్ళు బహిర్గతం;
		లేకపోతే ఒకవేళ (!ret)
			die(_("no such section: %s"), argv[0]);
		లేకపోతే
			ret = 0;
	}
	లేకపోతే ఒకవేళ (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	లేకపోతే ఒకవేళ (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		ఒకవేళ (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

బహిర్గతం:
	location_options_release(&location_opts);
	విడుదల(comment);
	విడుదల(విలువ);
	తిరిగి ret;
}

పూర్ణాంకం cmd_config(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *subcommand = ఖాళీ;
	నిర్మాణం option subcommand_opts[] = {
		OPT_SUBCOMMAND("list", &subcommand, cmd_config_list),
		OPT_SUBCOMMAND("get", &subcommand, cmd_config_get),
		OPT_SUBCOMMAND("set", &subcommand, cmd_config_సమితి),
		OPT_SUBCOMMAND("unset", &subcommand, cmd_config_unset),
		OPT_SUBCOMMAND("rename-section", &subcommand, cmd_config_rename_section),
		OPT_SUBCOMMAND("remove-section", &subcommand, cmd_config_remove_section),
		OPT_SUBCOMMAND("edit", &subcommand, cmd_config_edit),
		OPT_చివర(),
	};

	/*
	 * This is somewhat hacky: we first parse the command line while
	 * keeping all args intact in order to determine whether a subcommand
	 * has been specified. If so, we re-parse it a second time, but this
	 * time we drop KEEP_ARGV0. This is so that we don't munge the command
	 * line in case no subcommand was given, which would otherwise confuse
	 * us when parsing the legacy-style modes that don't use subcommands.
	 */
	argc = parse_options(argc, argv, ఉపసర్గ, subcommand_opts, builtin_config_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_ARGV0|PARSE_OPT_KEEP_UNKNOWN_OPT);
	ఒకవేళ (subcommand) {
		argc = parse_options(argc, argv, ఉపసర్గ, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		తిరిగి subcommand(argc, argv, ఉపసర్గ, repo);
	}

	తిరిగి cmd_config_actions(argc, argv, ఉపసర్గ);
}
