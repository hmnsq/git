#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "color.h"
#समावेश "date.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "ident.h"
#समावेश "parse-options.h"
#समावेश "urlmatch.h"
#समावेश "path.h"
#समावेश "quote.h"
#समावेश "setup.h"
#समावेश "strbuf.h"
#समावेश "worktree.h"

अटल स्थिर अक्षर *स्थिर builtin_config_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp] [--value=<pattern>] [--fixed-value] [--default=<default>] [--url=<url>] <name>"),
	N_("git config set [<file-option>] [--type=<type>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	N_("git config remove-section [<file-option>] <name>"),
	N_("git config edit [<file-option>]"),
	N_("git config [<file-option>] --get-colorbool <name> [<stdout-is-tty>]"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर builtin_config_list_usage[] = {
	N_("git config list [<file-option>] [<display-option>] [--includes]"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर builtin_config_get_usage[] = {
	N_("git config get [<file-option>] [<display-option>] [--includes] [--all] [--regexp=<regexp>] [--value=<pattern>] [--fixed-value] [--default=<default>] <name>"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर builtin_config_set_usage[] = {
	N_("git config set [<file-option>] [--type=<type>] [--comment=<message>] [--all] [--value=<pattern>] [--fixed-value] <name> <value>"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर builtin_config_unset_usage[] = {
	N_("git config unset [<file-option>] [--all] [--value=<pattern>] [--fixed-value] <name>"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर builtin_config_rename_section_usage[] = {
	N_("git config rename-section [<file-option>] <old-name> <new-name>"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर builtin_config_remove_section_usage[] = {
	N_("git config remove-section [<file-option>] <name>"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर builtin_config_edit_usage[] = {
	N_("git config edit [<file-option>]"),
	शून्य
};

#घोषणा CONFIG_LOCATION_OPTIONS(opts) \
	OPT_GROUP(N_("Config file location")), \
	OPT_BOOL(0, "global", &opts.use_global_config, N_("use global config file")), \
	OPT_BOOL(0, "system", &opts.use_system_config, N_("use system config file")), \
	OPT_BOOL(0, "local", &opts.use_local_config, N_("use repository config file")), \
	OPT_BOOL(0, "worktree", &opts.use_worktree_config, N_("use per-worktree config file")), \
	OPT_STRING('f', "file", &opts.source.file, N_("file"), N_("use given config file")), \
	OPT_STRING(0, "blob", &opts.source.blob, N_("blob-id"), N_("read config from given blob object"))

काष्ठा config_location_options {
	काष्ठा git_config_source source;
	काष्ठा config_options options;
	अक्षर *file_to_free;
	पूर्णांक use_global_config;
	पूर्णांक use_system_config;
	पूर्णांक use_local_config;
	पूर्णांक use_worktree_config;
	पूर्णांक respect_includes_opt;
};
#घोषणा CONFIG_LOCATION_OPTIONS_INIT { \
	.respect_includes_opt = -1, \
}

क्रमागत config_type {
	TYPE_NONE = 0,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_BOOL_OR_INT,
	TYPE_PATH,
	TYPE_EXPIRY_DATE,
	TYPE_COLOR,
	TYPE_BOOL_OR_STR,
};

#घोषणा CONFIG_TYPE_OPTIONS(type) \
	OPT_GROUP(N_("Type")), \
	OPT_CALLBACK('t', "type", &type, N_("type"), N_("value is given this type"), option_parse_type), \
	OPT_CALLBACK_मूल्य(0, "bool", &type, N_("value is \"true\" or \"false\""), TYPE_BOOL), \
	OPT_CALLBACK_मूल्य(0, "int", &type, N_("value is decimal number"), TYPE_INT), \
	OPT_CALLBACK_मूल्य(0, "bool-or-int", &type, N_("value is --bool or --int"), TYPE_BOOL_OR_INT), \
	OPT_CALLBACK_मूल्य(0, "bool-or-str", &type, N_("value is --bool or string"), TYPE_BOOL_OR_STR), \
	OPT_CALLBACK_मूल्य(0, "path", &type, N_("value is a path (file or directory name)"), TYPE_PATH), \
	OPT_CALLBACK_मूल्य(0, "expiry-date", &type, N_("value is an expiry date"), TYPE_EXPIRY_DATE)

#घोषणा CONFIG_DISPLAY_OPTIONS(opts) \
	OPT_GROUP(N_("Display options")), \
	OPT_BOOL('z', "null", &opts.end_nul, N_("terminate values with NUL byte")), \
	OPT_BOOL(0, "name-only", &opts.omit_मूल्यावली, N_("show variable names only")), \
	OPT_BOOL(0, "show-origin", &opts.show_origin, N_("show origin of config (file, standard input, blob, command line)")), \
	OPT_BOOL(0, "show-scope", &opts.show_scope, N_("show scope of config (worktree, local, global, system, command)")), \
	OPT_BOOL(0, "show-names", &opts.show_कुंजियाँ, N_("show config keys in addition to their values")), \
	CONFIG_TYPE_OPTIONS(opts.type)

काष्ठा config_display_options {
	पूर्णांक end_nul;
	पूर्णांक omit_मूल्यावली;
	पूर्णांक show_origin;
	पूर्णांक show_scope;
	पूर्णांक show_कुंजियाँ;
	क्रमागत config_type type;
	अक्षर *default_मूल्य;
	/* Populated via `display_options_init()`. */
	पूर्णांक term;
	पूर्णांक delim;
	पूर्णांक कुंजी_delim;
};
#घोषणा CONFIG_DISPLAY_OPTIONS_INIT { \
	.term = '\n', \
	.delim = '=', \
	.कुंजी_delim = ' ', \
	.type = TYPE_NONE, \
}

#घोषणा OPT_CALLBACK_मूल्य(s, l, v, h, i) { \
	.type = OPTION_CALLBACK, \
	.short_name = (s), \
	.long_name = (l), \
	.मूल्य = (v), \
	.help = (h), \
	.flags = PARSE_OPT_NOARG | PARSE_OPT_NONEG, \
	.callback = option_parse_type, \
	.defval = (i), \
}

अटल पूर्णांक option_parse_type(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			     पूर्णांक unset)
{
	पूर्णांक new_type, *to_type;

	अगर (unset) {
		*((पूर्णांक *) opt->मूल्य) = 0;
		वापस 0;
	}

	/*
	 * To support '--<type>' style flags, begin with new_type equal to
	 * opt->defval.
	 */
	new_type = opt->defval;
	अगर (!new_type) {
		अगर (!म_भेद(arg, "bool"))
			new_type = TYPE_BOOL;
		अन्यथा अगर (!म_भेद(arg, "int"))
			new_type = TYPE_INT;
		अन्यथा अगर (!म_भेद(arg, "bool-or-int"))
			new_type = TYPE_BOOL_OR_INT;
		अन्यथा अगर (!म_भेद(arg, "bool-or-str"))
			new_type = TYPE_BOOL_OR_STR;
		अन्यथा अगर (!म_भेद(arg, "path"))
			new_type = TYPE_PATH;
		अन्यथा अगर (!म_भेद(arg, "expiry-date"))
			new_type = TYPE_EXPIRY_DATE;
		अन्यथा अगर (!म_भेद(arg, "color"))
			new_type = TYPE_COLOR;
		अन्यथा
			die(_("unrecognized --type argument, %s"), arg);
	}

	to_type = opt->मूल्य;
	अगर (*to_type && *to_type != new_type) {
		/*
		 * Complain when there is a new type not equal to the old type.
		 * This allows for combinations like '--int --type=int' and
		 * '--type=int --type=int', but disallows ones like '--type=bool
		 * --int' and '--type=bool
		 * --type=int'.
		 */
		error(_("only one type at a time"));
		निकास(129);
	}
	*to_type = new_type;

	वापस 0;
}

अटल व्योम check_argc(पूर्णांक argc, पूर्णांक min, पूर्णांक max)
{
	अगर (argc >= min && argc <= max)
		वापस;
	अगर (min == max)
		error(_("wrong number of arguments, should be %d"), min);
	अन्यथा
		error(_("wrong number of arguments, should be from %d to %d"),
		      min, max);
	निकास(129);
}

अटल NORETURN व्योम die_missing_set_मूल्य(स्थिर अक्षर *arg)
{
	स्थिर अक्षर *last_dot = म_खोजप(arg, '.');
	स्थिर अक्षर *eq = last_dot ? म_अक्षर(last_dot + 1, '=') : शून्य;
	अक्षर *prefix = eq ? xstrndup(arg, eq - arg) : शून्य;

	अगर (prefix && git_config_कुंजी_is_valid(prefix)) {
		error(_("missing value to set to the variable '%s'"), arg);
		advise(_("did you mean \"git config set %s %s\"?"),
		       prefix, eq + 1);
	} अन्यथा अगर (git_config_कुंजी_is_valid(arg)) {
		error(_("missing value to set to the variable '%s'"), arg);
	} अन्यथा {
		error(_("missing value to set to a variable with an invalid name '%s'"),
		      arg);
	}
	मुक्त(prefix);
	निकास(129);
}

अटल व्योम show_config_origin(स्थिर काष्ठा config_display_options *opts,
			       स्थिर काष्ठा कुंजी_मूल्य_info *kvi,
			       काष्ठा strbuf *buf)
{
	स्थिर अक्षर term = opts->end_nul ? '\0' : '\t';

	strbuf_addstr(buf, config_origin_type_name(kvi->origin_type));
	strbuf_addch(buf, ':');
	अगर (opts->end_nul)
		strbuf_addstr(buf, kvi->filename ? kvi->filename : "");
	अन्यथा
		quote_c_style(kvi->filename ? kvi->filename : "", buf, शून्य, 0);
	strbuf_addch(buf, term);
}

अटल व्योम show_config_scope(स्थिर काष्ठा config_display_options *opts,
			      स्थिर काष्ठा कुंजी_मूल्य_info *kvi,
			      काष्ठा strbuf *buf)
{
	स्थिर अक्षर term = opts->end_nul ? '\0' : '\t';
	स्थिर अक्षर *scope = config_scope_name(kvi->scope);

	strbuf_addstr(buf, N_(scope));
	strbuf_addch(buf, term);
}

काष्ठा strbuf_list {
	काष्ठा strbuf *वस्तुएँ;
	पूर्णांक nr;
	पूर्णांक alloc;
};

अटल पूर्णांक format_config_int64(काष्ठा strbuf *buf,
			       स्थिर अक्षर *कुंजी_2,
			       स्थिर अक्षर *मूल्य_2,
			       स्थिर काष्ठा कुंजी_मूल्य_info *kvi,
			       पूर्णांक gently)
{
	int64_t v = 0;
	अगर (gently) {
		अगर (!git_parse_int64(मूल्य_2, &v))
			वापस -1;
	} अन्यथा {
		/* may die() */
		v = git_config_int64(कुंजी_2, मूल्य_2 ? मूल्य_2 : "", kvi);
	}

	strbuf_addf(buf, "%"PRId64, v);
	वापस 0;
}

अटल पूर्णांक format_config_bool(काष्ठा strbuf *buf,
			      स्थिर अक्षर *कुंजी_2,
			      स्थिर अक्षर *मूल्य_2,
			      पूर्णांक gently)
{
	पूर्णांक v = 0;
	अगर (gently) {
		अगर ((v = git_parse_maybe_bool(मूल्य_2)) < 0)
			वापस -1;
	} अन्यथा {
		/* may die() */
		v = git_config_bool(कुंजी_2, मूल्य_2);
	}

	strbuf_addstr(buf, v ? "true" : "false");
	वापस 0;
}

अटल पूर्णांक format_config_bool_or_int(काष्ठा strbuf *buf,
				     स्थिर अक्षर *कुंजी_2,
				     स्थिर अक्षर *मूल्य_2,
				     स्थिर काष्ठा कुंजी_मूल्य_info *kvi,
				     पूर्णांक gently)
{
	पूर्णांक v, is_bool = 0;

	अगर (gently) {
		v = git_parse_maybe_bool_text(मूल्य_2);

		अगर (v >= 0)
			is_bool = 1;
		अन्यथा अगर (!git_parse_int(मूल्य_2, &v))
			वापस -1;
	} अन्यथा {
		v = git_config_bool_or_int(कुंजी_2, मूल्य_2, kvi,
					   &is_bool);
	}

	अगर (is_bool)
		strbuf_addstr(buf, v ? "true" : "false");
	अन्यथा
		strbuf_addf(buf, "%d", v);

	वापस 0;
}

/* This mode is always gentle. */
अटल पूर्णांक format_config_bool_or_str(काष्ठा strbuf *buf,
				     स्थिर अक्षर *मूल्य_2)
{
	पूर्णांक v = git_parse_maybe_bool(मूल्य_2);
	अगर (v < 0)
		strbuf_addstr(buf, मूल्य_2);
	अन्यथा
		strbuf_addstr(buf, v ? "true" : "false");
	वापस 0;
}

अटल पूर्णांक format_config_path(काष्ठा strbuf *buf,
			      स्थिर अक्षर *कुंजी_2,
			      स्थिर अक्षर *मूल्य_2,
			      पूर्णांक gently)
{
	अक्षर *v;

	अगर (git_config_pathname(&v, कुंजी_2, मूल्य_2) < 0)
		वापस -1;

	अगर (v)
		strbuf_addstr(buf, v);
	अन्यथा
		वापस gently ? -1 : 1; /* :(optional)no-such-file */

	मुक्त(v);
	वापस 0;
}

अटल पूर्णांक format_config_expiry_date(काष्ठा strbuf *buf,
				     स्थिर अक्षर *कुंजी_2,
				     स्थिर अक्षर *मूल्य_2,
				     पूर्णांक quietly)
{
	timestamp_t t;
	अगर (quietly) {
		अगर (parse_expiry_date(मूल्य_2, &t))
			वापस -1;
	} अन्यथा अगर (git_config_expiry_date(&t, कुंजी_2, मूल्य_2) < 0) {
		वापस -1;
	}

	strbuf_addf(buf, "%"PRItime, t);
	वापस 0;
}

अटल पूर्णांक format_config_color(काष्ठा strbuf *buf,
			       स्थिर अक्षर *कुंजी_2,
			       स्थिर अक्षर *मूल्य_2,
			       पूर्णांक gently)
{
	अक्षर v[COLOR_MAXLEN];

	अगर (gently) {
		अगर (color_parse_quietly(मूल्य_2, v) < 0)
			वापस -1;
	} अन्यथा अगर (git_config_color(v, कुंजी_2, मूल्य_2) < 0) {
		वापस -1;
	}

	strbuf_addstr(buf, v);
	वापस 0;
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
अटल पूर्णांक format_config(स्थिर काष्ठा config_display_options *opts,
			 काष्ठा strbuf *buf, स्थिर अक्षर *कुंजी_2,
			 स्थिर अक्षर *मूल्य_2, स्थिर काष्ठा कुंजी_मूल्य_info *kvi,
			 पूर्णांक gently)
{
	पूर्णांक res = 0;
	अगर (opts->show_scope)
		show_config_scope(opts, kvi, buf);
	अगर (opts->show_origin)
		show_config_origin(opts, kvi, buf);
	अगर (opts->show_कुंजियाँ)
		strbuf_addstr(buf, कुंजी_2);

	अगर (opts->omit_मूल्यावली)
		जाओ terminator;

	अगर (opts->show_कुंजियाँ)
		strbuf_addch(buf, opts->कुंजी_delim);

	चयन (opts->type) {
	हाल TYPE_INT:
		res = format_config_int64(buf, कुंजी_2, मूल्य_2, kvi, gently);
		अवरोध;

	हाल TYPE_BOOL:
		res = format_config_bool(buf, कुंजी_2, मूल्य_2, gently);
		अवरोध;

	हाल TYPE_BOOL_OR_INT:
		res = format_config_bool_or_int(buf, कुंजी_2, मूल्य_2, kvi, gently);
		अवरोध;

	हाल TYPE_BOOL_OR_STR:
		res = format_config_bool_or_str(buf, मूल्य_2);
		अवरोध;

	हाल TYPE_PATH:
		res = format_config_path(buf, कुंजी_2, मूल्य_2, gently);
		अवरोध;

	हाल TYPE_EXPIRY_DATE:
		res = format_config_expiry_date(buf, कुंजी_2, मूल्य_2, gently);
		अवरोध;

	हाल TYPE_COLOR:
		res = format_config_color(buf, कुंजी_2, मूल्य_2, gently);
		अवरोध;

	हाल TYPE_NONE:
		अगर (मूल्य_2) {
			strbuf_addstr(buf, मूल्य_2);
		} अन्यथा {
			/* Just show the key name; back out delimiter */
			अगर (opts->show_कुंजियाँ)
				strbuf_setlen(buf, buf->len - 1);
		}
		अवरोध;

	शेष:
		BUG("undefined type %d", opts->type);
	}

terminator:
	strbuf_addch(buf, opts->term);
	वापस res;
}

अटल पूर्णांक show_all_config(स्थिर अक्षर *कुंजी_2, स्थिर अक्षर *मूल्य_2,
			   स्थिर काष्ठा config_context *ctx,
			   व्योम *cb)
{
	स्थिर काष्ठा config_display_options *opts = cb;
	स्थिर काष्ठा कुंजी_मूल्य_info *kvi = ctx->kvi;
	काष्ठा strbuf formatted = STRBUF_INIT;

	अगर (format_config(opts, &formatted, कुंजी_2, मूल्य_2, kvi, 1) >= 0)
		ख_डालो(formatted.buf, 1, formatted.len, मानक_निकास);

	strbuf_release(&formatted);
	वापस 0;
}

#घोषणा GET_मूल्य_ALL        (1 << 0)
#घोषणा GET_मूल्य_कुंजी_REGEXP (1 << 1)

काष्ठा collect_config_तथ्य {
	स्थिर काष्ठा config_display_options *display_opts;
	काष्ठा strbuf_list *मूल्यावली;
	स्थिर अक्षर *मूल्य_pattern;
	स्थिर अक्षर *कुंजी;
	regex_t *regexp;
	regex_t *कुंजी_regexp;
	पूर्णांक do_not_match;
	अचिन्हित get_मूल्य_flags;
	अचिन्हित flags;
};

अटल पूर्णांक collect_config(स्थिर अक्षर *कुंजी_2, स्थिर अक्षर *मूल्य_2,
			  स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा collect_config_तथ्य *तथ्य = cb;
	काष्ठा strbuf_list *मूल्यावली = तथ्य->मूल्यावली;
	स्थिर काष्ठा कुंजी_मूल्य_info *kvi = ctx->kvi;
	पूर्णांक status;

	अगर (!(तथ्य->get_मूल्य_flags & GET_मूल्य_कुंजी_REGEXP) &&
	    म_भेद(कुंजी_2, तथ्य->कुंजी))
		वापस 0;
	अगर ((तथ्य->get_मूल्य_flags & GET_मूल्य_कुंजी_REGEXP) &&
	    regexec(तथ्य->कुंजी_regexp, कुंजी_2, 0, शून्य, 0))
		वापस 0;
	अगर ((तथ्य->flags & CONFIG_FLAGS_FIXED_मूल्य) &&
	    म_भेद(तथ्य->मूल्य_pattern, (मूल्य_2?मूल्य_2:"")))
		वापस 0;
	अगर (तथ्य->regexp &&
	    (तथ्य->do_not_match ^ !!regexec(तथ्य->regexp, (मूल्य_2?मूल्य_2:""), 0, शून्य, 0)))
		वापस 0;

	ALLOC_GROW(मूल्यावली->वस्तुएँ, मूल्यावली->nr + 1, मूल्यावली->alloc);
	strbuf_init(&मूल्यावली->वस्तुएँ[मूल्यावली->nr], 0);

	status = format_config(तथ्य->display_opts, &मूल्यावली->वस्तुएँ[मूल्यावली->nr++],
			       कुंजी_2, मूल्य_2, kvi, 0);
	अगर (status < 0)
		वापस status;
	अगर (status) {
		strbuf_release(&मूल्यावली->वस्तुएँ[--मूल्यावली->nr]);
		status = 0;
	}
	वापस status;
}

अटल पूर्णांक get_मूल्य(स्थिर काष्ठा config_location_options *opts,
		     स्थिर काष्ठा config_display_options *display_opts,
		     स्थिर अक्षर *कुंजी_2, स्थिर अक्षर *regex_,
		     अचिन्हित get_मूल्य_flags, अचिन्हित flags)
{
	पूर्णांक ret = CONFIG_GENERIC_ERROR;
	काष्ठा strbuf_list मूल्यावली = {शून्य};
	काष्ठा collect_config_तथ्य तथ्य = {
		.display_opts = display_opts,
		.मूल्यावली = &मूल्यावली,
		.get_मूल्य_flags = get_मूल्य_flags,
		.flags = flags,
	};
	अक्षर *कुंजी = शून्य;
	पूर्णांक i;

	अगर (get_मूल्य_flags & GET_मूल्य_कुंजी_REGEXP) {
		अक्षर *tl;

		/*
		 * NEEDSWORK: this naive pattern lowercasing obviously does not
		 * work for more complex patterns like "^[^.]*Foo.*bar".
		 * Perhaps we should deprecate this altogether someday.
		 */

		कुंजी = xstrdup(कुंजी_2);
		क्रम (tl = कुंजी + म_माप(कुंजी) - 1;
		     tl >= कुंजी && *tl != '.';
		     tl--)
			*tl = छोटे(*tl);
		क्रम (tl = कुंजी; *tl && *tl != '.'; tl++)
			*tl = छोटे(*tl);

		तथ्य.कुंजी_regexp = (regex_t*)xmalloc(माप(regex_t));
		अगर (regcomp(तथ्य.कुंजी_regexp, कुंजी, REG_EXTENDED)) {
			error(_("invalid key pattern: %s"), कुंजी_2);
			FREE_AND_NULL(तथ्य.कुंजी_regexp);
			ret = CONFIG_INVALID_PATTERN;
			जाओ free_strings;
		}
	} अन्यथा {
		अगर (git_config_parse_कुंजी(कुंजी_2, &कुंजी, शून्य)) {
			ret = CONFIG_INVALID_कुंजी;
			जाओ free_strings;
		}

		तथ्य.कुंजी = कुंजी;
	}

	अगर (regex_ && (flags & CONFIG_FLAGS_FIXED_मूल्य))
		तथ्य.मूल्य_pattern = regex_;
	अन्यथा अगर (regex_) {
		अगर (regex_[0] == '!') {
			तथ्य.do_not_match = 1;
			regex_++;
		}

		तथ्य.regexp = (regex_t*)xmalloc(माप(regex_t));
		अगर (regcomp(तथ्य.regexp, regex_, REG_EXTENDED)) {
			error(_("invalid pattern: %s"), regex_);
			FREE_AND_NULL(तथ्य.regexp);
			ret = CONFIG_INVALID_PATTERN;
			जाओ free_strings;
		}
	}

	config_with_options(collect_config, &तथ्य,
			    &opts->source, the_repository,
			    &opts->options);

	अगर (!मूल्यावली.nr && display_opts->default_मूल्य) {
		काष्ठा कुंजी_मूल्य_info kvi = KVI_INIT;
		काष्ठा strbuf *item;
		पूर्णांक status;

		kvi_from_param(&kvi);
		ALLOC_GROW(मूल्यावली.वस्तुएँ, मूल्यावली.nr + 1, मूल्यावली.alloc);
		item = &मूल्यावली.वस्तुएँ[मूल्यावली.nr++];
		strbuf_init(item, 0);

		status = format_config(display_opts, item, कुंजी_2,
				       display_opts->default_मूल्य, &kvi, 0);
		अगर (status < 0)
			die(_("failed to format default config value: %s"),
			    display_opts->default_मूल्य);
		अगर (status) {
			/* default was a missing optional value */
			मूल्यावली.nr--;
			strbuf_release(item);
		}
	}

	ret = !मूल्यावली.nr;

	क्रम (i = 0; i < मूल्यावली.nr; i++) {
		काष्ठा strbuf *buf = मूल्यावली.वस्तुएँ + i;
		अगर ((get_मूल्य_flags & GET_मूल्य_ALL) || i == मूल्यावली.nr - 1)
			ख_डालो(buf->buf, 1, buf->len, मानक_निकास);
		strbuf_release(buf);
	}
	मुक्त(मूल्यावली.वस्तुएँ);

free_strings:
	मुक्त(कुंजी);
	अगर (तथ्य.कुंजी_regexp) {
		regfree(तथ्य.कुंजी_regexp);
		मुक्त(तथ्य.कुंजी_regexp);
	}
	अगर (तथ्य.regexp) {
		regfree(तथ्य.regexp);
		मुक्त(तथ्य.regexp);
	}

	वापस ret;
}

अटल अक्षर *normalize_मूल्य(स्थिर अक्षर *कुंजी, स्थिर अक्षर *मूल्य,
			     पूर्णांक type, काष्ठा कुंजी_मूल्य_info *kvi)
{
	अगर (!मूल्य)
		वापस शून्य;

	अगर (type == 0 || type == TYPE_PATH || type == TYPE_EXPIRY_DATE)
		/*
		 * We don't do normalization for TYPE_PATH here: If
		 * the path is like ~/foobar/, we prefer to store
		 * "~/foobar/" in the config file, and to expand the ~
		 * when retrieving the value.
		 * Also don't do normalization for expiry dates.
		 */
		वापस xstrdup(मूल्य);
	अगर (type == TYPE_INT)
		वापस xstrfmt("%"PRId64, git_config_int64(कुंजी, मूल्य, kvi));
	अगर (type == TYPE_BOOL)
		वापस xstrdup(git_config_bool(कुंजी, मूल्य) ?  "true" : "false");
	अगर (type == TYPE_BOOL_OR_INT) {
		पूर्णांक is_bool, v;
		v = git_config_bool_or_int(कुंजी, मूल्य, kvi, &is_bool);
		अगर (!is_bool)
			वापस xstrfmt("%d", v);
		अन्यथा
			वापस xstrdup(v ? "true" : "false");
	}
	अगर (type == TYPE_BOOL_OR_STR) {
		पूर्णांक v = git_parse_maybe_bool(मूल्य);
		अगर (v < 0)
			वापस xstrdup(मूल्य);
		अन्यथा
			वापस xstrdup(v ? "true" : "false");
	}
	अगर (type == TYPE_COLOR) {
		अक्षर v[COLOR_MAXLEN];
		अगर (git_config_color(v, कुंजी, मूल्य))
			die(_("cannot parse color '%s'"), मूल्य);

		/*
		 * The contents of `v` now contain an ANSI escape
		 * sequence, not suitable for including within a
		 * configuration file. Treat the above as a
		 * "sanity-check", and return the given value, which we
		 * know is representable as valid color code.
		 */
		वापस xstrdup(मूल्य);
	}

	BUG("cannot normalize type %d", type);
}

काष्ठा get_color_config_तथ्य {
	पूर्णांक get_color_found;
	स्थिर अक्षर *get_color_slot;
	अक्षर parsed_color[COLOR_MAXLEN];
};

अटल पूर्णांक git_get_color_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
				स्थिर काष्ठा config_context *ctx UNUSED,
				व्योम *cb)
{
	काष्ठा get_color_config_तथ्य *तथ्य = cb;

	अगर (!म_भेद(var, तथ्य->get_color_slot)) {
		अगर (!मूल्य)
			config_error_nonbool(var);
		अगर (color_parse(मूल्य, तथ्य->parsed_color) < 0)
			वापस -1;
		तथ्य->get_color_found = 1;
	}
	वापस 0;
}

अटल पूर्णांक get_color(स्थिर काष्ठा config_location_options *opts,
		      स्थिर अक्षर *var, स्थिर अक्षर *def_color)
{
	काष्ठा get_color_config_तथ्य तथ्य = {
		.get_color_slot = var,
		.parsed_color[0] = '\0',
	};
	पूर्णांक ret;

	config_with_options(git_get_color_config, &तथ्य,
			    &opts->source, the_repository,
			    &opts->options);

	अगर (!तथ्य.get_color_found && def_color) {
		अगर (color_parse(def_color, तथ्य.parsed_color) < 0) {
			ret = error(_("unable to parse default color value"));
			जाओ out;
		}
	}

	ret = 0;

out:
	ख_माला_दो(तथ्य.parsed_color, मानक_निकास);
	वापस ret;
}

काष्ठा get_colorbool_config_तथ्य {
	क्रमागत git_colorbool get_colorbool_found;
	क्रमागत git_colorbool get_diff_color_found;
	क्रमागत git_colorbool get_color_ui_found;
	स्थिर अक्षर *get_colorbool_slot;
};

अटल पूर्णांक git_get_colorbool_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
				    स्थिर काष्ठा config_context *ctx UNUSED,
				    व्योम *cb)
{
	काष्ठा get_colorbool_config_तथ्य *तथ्य = cb;

	अगर (!म_भेद(var, तथ्य->get_colorbool_slot))
		तथ्य->get_colorbool_found = git_config_colorbool(var, मूल्य);
	अन्यथा अगर (!म_भेद(var, "diff.color"))
		तथ्य->get_diff_color_found = git_config_colorbool(var, मूल्य);
	अन्यथा अगर (!म_भेद(var, "color.ui"))
		तथ्य->get_color_ui_found = git_config_colorbool(var, मूल्य);
	वापस 0;
}

अटल पूर्णांक get_colorbool(स्थिर काष्ठा config_location_options *opts,
			 स्थिर अक्षर *var, पूर्णांक print)
{
	काष्ठा get_colorbool_config_तथ्य तथ्य = {
		.get_colorbool_slot = var,
		.get_colorbool_found = GIT_COLOR_UNKNOWN,
		.get_diff_color_found = GIT_COLOR_UNKNOWN,
		.get_color_ui_found = GIT_COLOR_UNKNOWN,
	};
	bool परिणाम;

	config_with_options(git_get_colorbool_config, &तथ्य,
			    &opts->source, the_repository,
			    &opts->options);

	अगर (तथ्य.get_colorbool_found == GIT_COLOR_UNKNOWN) {
		अगर (!म_भेद(तथ्य.get_colorbool_slot, "color.diff"))
			तथ्य.get_colorbool_found = तथ्य.get_diff_color_found;
		अगर (तथ्य.get_colorbool_found == GIT_COLOR_UNKNOWN)
			तथ्य.get_colorbool_found = तथ्य.get_color_ui_found;
	}

	अगर (तथ्य.get_colorbool_found == GIT_COLOR_UNKNOWN)
		/* default value if none found in config */
		तथ्य.get_colorbool_found = GIT_COLOR_AUTO;

	परिणाम = want_color(तथ्य.get_colorbool_found);

	अगर (print) {
		म_लिखो("%s\n", परिणाम ? "true" : "false");
		वापस 0;
	} अन्यथा
		वापस परिणाम ? 0 : 1;
}

अटल व्योम check_write(स्थिर काष्ठा git_config_source *source)
{
	अगर (!source->file && !startup_info->have_repository)
		die(_("not in a git directory"));

	अगर (source->use_stdin)
		die(_("writing to stdin is not supported"));

	अगर (source->blob)
		die(_("writing config blobs is not supported"));
}

काष्ठा urlmatch_current_candidate_मूल्य {
	अक्षर मूल्य_is_null;
	काष्ठा strbuf मूल्य;
	काष्ठा कुंजी_मूल्य_info kvi;
};

अटल पूर्णांक urlmatch_collect_fn(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			       स्थिर काष्ठा config_context *ctx,
			       व्योम *cb)
{
	काष्ठा string_list *मूल्यावली = cb;
	काष्ठा string_list_item *item = string_list_जोड़ो(मूल्यावली, var);
	काष्ठा urlmatch_current_candidate_मूल्य *matched = item->util;
	स्थिर काष्ठा कुंजी_मूल्य_info *kvi = ctx->kvi;

	अगर (!matched) {
		matched = xmalloc(माप(*matched));
		strbuf_init(&matched->मूल्य, 0);
		item->util = matched;
	} अन्यथा {
		strbuf_reset(&matched->मूल्य);
	}
	matched->kvi = *kvi;

	अगर (मूल्य) {
		strbuf_addstr(&matched->मूल्य, मूल्य);
		matched->मूल्य_is_null = 0;
	} अन्यथा {
		matched->मूल्य_is_null = 1;
	}
	वापस 0;
}

अटल पूर्णांक get_urlmatch(स्थिर काष्ठा config_location_options *opts,
			स्थिर काष्ठा config_display_options *_display_opts,
			स्थिर अक्षर *var, स्थिर अक्षर *url)
{
	पूर्णांक ret;
	अक्षर *section;
	अक्षर *section_पुच्छ;
	काष्ठा config_display_options display_opts = *_display_opts;
	काष्ठा string_list_item *item;
	काष्ठा urlmatch_config config = URLMATCH_CONFIG_INIT;
	काष्ठा string_list मूल्यावली = STRING_LIST_INIT_DUP;

	config.collect_fn = urlmatch_collect_fn;
	config.cascade_fn = शून्य;
	config.cb = &मूल्यावली;

	अगर (!url_normalize(url, &config.url))
		die("%s", config.url.err);

	config.section = section = xstrdup_tolower(var);
	section_पुच्छ = म_अक्षर(section, '.');
	अगर (section_पुच्छ) {
		*section_पुच्छ = '\0';
		config.कुंजी = section_पुच्छ + 1;
		display_opts.show_कुंजियाँ = 0;
	} अन्यथा {
		config.कुंजी = शून्य;
		display_opts.show_कुंजियाँ = 1;
	}

	config_with_options(urlmatch_config_entry, &config,
			    &opts->source, the_repository,
			    &opts->options);

	ret = !मूल्यावली.nr;

	for_each_string_list_item(item, &मूल्यावली) {
		काष्ठा urlmatch_current_candidate_मूल्य *matched = item->util;
		काष्ठा strbuf buf = STRBUF_INIT;
		पूर्णांक status;

		status = format_config(&display_opts, &buf, item->string,
				       matched->मूल्य_is_null ? शून्य : matched->मूल्य.buf,
				       &matched->kvi, 0);
		अगर (!status)
			ख_डालो(buf.buf, 1, buf.len, मानक_निकास);
		strbuf_release(&buf);

		strbuf_release(&matched->मूल्य);
	}
	urlmatch_config_release(&config);
	string_list_clear(&मूल्यावली, 1);
	मुक्त(config.url.url);

	मुक्त(section);
	वापस ret;
}

अटल अक्षर *default_user_config(व्योम)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf,
		    _("# This is Git's per-user configuration file.\n"
		      "[user]\n"
		      "# Please adapt and uncomment the following lines:\n"
		      "#	name = %s\n"
		      "#	email = %s\n"),
		    ident_default_name(),
		    ident_default_email());
	वापस strbuf_detach(&buf, शून्य);
}

अटल व्योम location_options_init(काष्ठा config_location_options *opts,
				  स्थिर अक्षर *prefix)
{
	अगर (!opts->source.file)
		opts->source.file = opts->file_to_free =
			xstrdup_or_null(दो_पर्या(CONFIG_ENVIRONMENT));

	अगर (opts->use_global_config + opts->use_system_config +
	    opts->use_local_config + opts->use_worktree_config +
	    !!opts->source.file + !!opts->source.blob > 1) {
		error(_("only one config file at a time"));
		निकास(129);
	}

	अगर (!startup_info->have_repository) {
		अगर (opts->use_local_config)
			die(_("--local can only be used inside a git repository"));
		अगर (opts->source.blob)
			die(_("--blob can only be used inside a git repository"));
		अगर (opts->use_worktree_config)
			die(_("--worktree can only be used inside a git repository"));
	}

	अगर (opts->source.file &&
			!म_भेद(opts->source.file, "-")) {
		opts->source.file = शून्य;
		opts->source.use_stdin = 1;
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	अगर (opts->use_global_config) {
		opts->source.file = opts->file_to_free = git_global_config();
		अगर (!opts->source.file)
			/*
			 * It is unknown if HOME/.gitconfig exists, so
			 * we do not know if we should write to XDG
			 * location; error out even if XDG_CONFIG_HOME
			 * is set and points at a sane location.
			 */
			die(_("$HOME not set"));
		opts->source.scope = CONFIG_SCOPE_GLOBAL;
	} अन्यथा अगर (opts->use_system_config) {
		opts->source.file = opts->file_to_free = git_system_config();
		opts->source.scope = CONFIG_SCOPE_SYSTEM;
	} अन्यथा अगर (opts->use_local_config) {
		opts->source.file = opts->file_to_free = repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
	} अन्यथा अगर (opts->use_worktree_config) {
		काष्ठा worktree **worktrees = get_worktrees(the_repository);
		अगर (the_repository->repository_format_worktree_config)
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config.worktree");
		अन्यथा अगर (worktrees[0] && worktrees[1])
			die(_("--worktree cannot be used with multiple "
			      "working trees unless the config\n"
			      "extension worktreeConfig is enabled. "
			      "Please read \"CONFIGURATION FILE\"\n"
			      "section in \"git help worktree\" for details"));
		अन्यथा
			opts->source.file = opts->file_to_free =
				repo_git_path(the_repository, "config");
		opts->source.scope = CONFIG_SCOPE_LOCAL;
		free_worktrees(worktrees);
	} अन्यथा अगर (opts->source.file) {
		अगर (!is_absolute_path(opts->source.file) && prefix)
			opts->source.file = opts->file_to_free =
				prefix_filename(prefix, opts->source.file);
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	} अन्यथा अगर (opts->source.blob) {
		opts->source.scope = CONFIG_SCOPE_COMMAND;
	}

	अगर (opts->respect_includes_opt == -1)
		opts->options.respect_includes = !opts->source.file;
	अन्यथा
		opts->options.respect_includes = opts->respect_includes_opt;
	अगर (startup_info->have_repository) {
		opts->options.commondir = repo_get_common_dir(the_repository);
		opts->options.git_dir = repo_get_git_dir(the_repository);
	}
}

अटल व्योम location_options_release(काष्ठा config_location_options *opts)
{
	मुक्त(opts->file_to_free);
}

अटल व्योम display_options_init(काष्ठा config_display_options *opts)
{
	अगर (opts->end_nul) {
		opts->term = '\0';
		opts->delim = '\n';
		opts->कुंजी_delim = '\n';
	}
}

अटल व्योम display_options_init_list(काष्ठा config_display_options *opts)
{
	opts->show_कुंजियाँ = 1;

	अगर (opts->end_nul) {
		display_options_init(opts);
	} अन्यथा {
		opts->term = '\n';
		opts->delim = ' ';
		opts->कुंजी_delim = '=';
	}
}

अटल पूर्णांक cmd_config_list(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	काष्ठा config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	काष्ठा option opts[] = {
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

	अगर (config_with_options(show_all_config, &display_opts,
				&location_opts.source, the_repository,
				&location_opts.options) < 0) {
		अगर (location_opts.source.file)
			die_errno(_("unable to read config file '%s'"),
				  location_opts.source.file);
		अन्यथा
			die(_("error processing config file(s)"));
	}

	location_options_release(&location_opts);
	वापस 0;
}

अटल पूर्णांक cmd_config_get(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			  काष्ठा repository *repo UNUSED)
{
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	काष्ठा config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	स्थिर अक्षर *मूल्य_pattern = शून्य, *url = शून्य;
	पूर्णांक flags = 0;
	अचिन्हित get_मूल्य_flags = 0;
	काष्ठा option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter options")),
		OPT_BIT(0, "all", &get_मूल्य_flags, N_("return all values for multi-valued config options"), GET_मूल्य_ALL),
		OPT_BIT(0, "regexp", &get_मूल्य_flags, N_("interpret the name as a regular expression"), GET_मूल्य_कुंजी_REGEXP),
		OPT_STRING(0, "value", &मूल्य_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मूल्य),
		OPT_STRING(0, "url", &url, N_("URL"), N_("show config matching the given URL")),
		CONFIG_DISPLAY_OPTIONS(display_opts),
		OPT_GROUP(N_("Other")),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_STRING(0, "default", &display_opts.default_मूल्य,
			   N_("value"), N_("use default value when missing entry")),
		OPT_END(),
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_get_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	अगर ((flags & CONFIG_FLAGS_FIXED_मूल्य) && !मूल्य_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));
	अगर (display_opts.default_मूल्य &&
	    ((get_मूल्य_flags & GET_मूल्य_ALL) || url))
		die(_("--default= cannot be used with --all or --url="));
	अगर (url && ((get_मूल्य_flags & GET_मूल्य_ALL) ||
		    (get_मूल्य_flags & GET_मूल्य_कुंजी_REGEXP) ||
		    मूल्य_pattern))
		die(_("--url= cannot be used with --all, --regexp or --value"));

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	अगर (display_opts.type != TYPE_COLOR)
		setup_auto_pager("config", 1);

	अगर (url)
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], url);
	अन्यथा अगर (display_opts.type == TYPE_COLOR && !म_माप(argv[0]) && display_opts.default_मूल्य)
		ret = get_color(&location_opts, "", display_opts.default_मूल्य);
	अन्यथा
		ret = get_मूल्य(&location_opts, &display_opts, argv[0], मूल्य_pattern,
				get_मूल्य_flags, flags);

	location_options_release(&location_opts);
	वापस ret;
}

अटल पूर्णांक cmd_config_set(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			  काष्ठा repository *repo UNUSED)
{
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	स्थिर अक्षर *मूल्य_pattern = शून्य, *comment_arg = शून्य;
	अक्षर *comment = शून्य;
	पूर्णांक flags = 0, append = 0, type = 0;
	काष्ठा option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		CONFIG_TYPE_OPTIONS(type),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("replace multi-valued config option with new value"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मूल्य_pattern, N_("pattern"), N_("show config with values matching the pattern")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मूल्य),
		OPT_GROUP(N_("Other")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BOOL(0, "append", &append, N_("add a new line without altering any existing values")),
		OPT_END(),
	};
	काष्ठा कुंजी_मूल्य_info default_kvi = KVI_INIT;
	अक्षर *मूल्य;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_set_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	अगर (argc == 1)
		die_missing_set_मूल्य(argv[0]);
	check_argc(argc, 2, 2);

	अगर ((flags & CONFIG_FLAGS_FIXED_मूल्य) && !मूल्य_pattern)
		die(_("--fixed-value only applies with --value=<pattern>"));
	अगर (append && मूल्य_pattern)
		die(_("--append cannot be used with --value=<pattern>"));
	अगर (append)
		मूल्य_pattern = CONFIG_REGEX_NONE;

	comment = git_config_prepare_comment_string(comment_arg);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	मूल्य = normalize_मूल्य(argv[0], argv[1], type, &default_kvi);

	अगर ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मूल्य_pattern) {
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मूल्य, मूल्य_pattern,
							      comment, flags);
	} अन्यथा {
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
						     argv[0], comment, मूल्य);
		अगर (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use --value=<pattern>, --append or --all to change %s."), argv[0]);
	}

	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मूल्य);
	वापस ret;
}

अटल पूर्णांक cmd_config_unset(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			    काष्ठा repository *repo UNUSED)
{
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	स्थिर अक्षर *मूल्य_pattern = शून्य;
	पूर्णांक flags = 0;
	काष्ठा option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_GROUP(N_("Filter")),
		OPT_BIT(0, "all", &flags, N_("unset all multi-valued config options"), CONFIG_FLAGS_MULTI_REPLACE),
		OPT_STRING(0, "value", &मूल्य_pattern, N_("pattern"), N_("unset multi-valued config options with matching values")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मूल्य),
		OPT_END(),
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_unset_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	अगर ((flags & CONFIG_FLAGS_FIXED_मूल्य) && !मूल्य_pattern)
		die(_("--fixed-value only applies with 'value-pattern'"));

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	अगर ((flags & CONFIG_FLAGS_MULTI_REPLACE) || मूल्य_pattern)
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], शून्य, मूल्य_pattern,
							      शून्य, flags);
	अन्यथा
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0],
						     शून्य, शून्य);

	location_options_release(&location_opts);
	वापस ret;
}

अटल पूर्णांक cmd_config_rename_section(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				     काष्ठा repository *repo UNUSED)
{
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	काष्ठा option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_rename_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 2, 2);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], argv[1]);
	अगर (ret < 0)
		जाओ out;
	अन्यथा अगर (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	वापस ret;
}

अटल पूर्णांक cmd_config_remove_section(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
				     काष्ठा repository *repo UNUSED)
{
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	काष्ठा option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_remove_section_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	check_argc(argc, 1, 1);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
						 argv[0], शून्य);
	अगर (ret < 0)
		जाओ out;
	अन्यथा अगर (!ret)
		die(_("no such section: %s"), argv[0]);
	ret = 0;

out:
	location_options_release(&location_opts);
	वापस ret;
}

अटल पूर्णांक show_editor(काष्ठा config_location_options *opts)
{
	अक्षर *config_file;

	अगर (!opts->source.file && !startup_info->have_repository)
		die(_("not in a git directory"));
	अगर (opts->source.use_stdin)
		die(_("editing stdin is not supported"));
	अगर (opts->source.blob)
		die(_("editing blobs is not supported"));
	repo_config(the_repository, git_default_config, शून्य);
	config_file = opts->source.file ?
			xstrdup(opts->source.file) :
			repo_git_path(the_repository, "config");
	अगर (opts->use_global_config) {
		पूर्णांक fd = खोलो(config_file, O_CREAT | O_EXCL | O_WRONLY, 0666);
		अगर (fd >= 0) {
			अक्षर *content = default_user_config();
			write_str_in_full(fd, content);
			मुक्त(content);
			बंद(fd);
		}
		अन्यथा अगर (त्रुटि_सं != EEXIST)
			die_errno(_("cannot create configuration file %s"), config_file);
	}
	अगर (launch_editor(config_file, शून्य, शून्य)) {
		मुक्त(config_file);
		वापस -1;
	}
	मुक्त(config_file);

	वापस 0;
}

अटल पूर्णांक cmd_config_edit(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	काष्ठा option opts[] = {
		CONFIG_LOCATION_OPTIONS(location_opts),
		OPT_END(),
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, opts, builtin_config_edit_usage, 0);
	check_argc(argc, 0, 0);

	location_options_init(&location_opts, prefix);
	check_write(&location_opts.source);

	ret = show_editor(&location_opts);
	location_options_release(&location_opts);
	वापस ret;
}

अटल पूर्णांक cmd_config_actions(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix)
{
	क्रमागत {
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
	काष्ठा config_location_options location_opts = CONFIG_LOCATION_OPTIONS_INIT;
	काष्ठा config_display_options display_opts = CONFIG_DISPLAY_OPTIONS_INIT;
	स्थिर अक्षर *comment_arg = शून्य;
	पूर्णांक actions = 0;
	अचिन्हित flags = 0;
	काष्ठा option opts[] = {
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
		OPT_STRING(0, "default", &display_opts.default_मूल्य,
			   N_("value"), N_("with --get, use default value when missing entry")),
		OPT_STRING(0, "comment", &comment_arg, N_("value"), N_("human-readable comment string (# will be prepended as needed)")),
		OPT_BIT(0, "fixed-value", &flags, N_("use string equality when comparing values to value pattern"), CONFIG_FLAGS_FIXED_मूल्य),
		OPT_BOOL(0, "includes", &location_opts.respect_includes_opt,
			 N_("respect include directives on lookup")),
		OPT_END(),
	};
	अक्षर *मूल्य = शून्य, *comment = शून्य;
	पूर्णांक ret = 0;
	पूर्णांक actions_implicit;
	काष्ठा कुंजी_मूल्य_info default_kvi = KVI_INIT;

	argc = parse_options(argc, argv, prefix, opts,
			     builtin_config_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	location_options_init(&location_opts, prefix);
	display_options_init(&display_opts);

	अगर ((actions & (ACTION_GET_COLOR|ACTION_GET_COLORBOOL)) && display_opts.type) {
		error(_("--get-color and variable type are incoherent"));
		निकास(129);
	}

	actions_implicit = (actions == 0);
	अगर (actions_implicit)
		चयन (argc) {
		हाल 1: actions = ACTION_GET; अवरोध;
		हाल 2: actions = ACTION_SET; अवरोध;
		हाल 3: actions = ACTION_SET_ALL; अवरोध;
		शेष:
			error(_("no action specified"));
			निकास(129);
		}
	अगर (actions_implicit && argc == 1) {
		स्थिर अक्षर *last_dot = म_खोजप(argv[0], '.');
		अगर (last_dot && म_अक्षर(last_dot + 1, '='))
			die_missing_set_मूल्य(argv[0]);
	}
	अगर (display_opts.omit_मूल्यावली &&
	    !(actions == ACTION_LIST || actions == ACTION_GET_REGEXP)) {
		error(_("--name-only is only applicable to --list or --get-regexp"));
		निकास(129);
	}

	अगर (display_opts.show_origin && !(actions &
		(ACTION_GET|ACTION_GET_ALL|ACTION_GET_REGEXP|ACTION_LIST))) {
		error(_("--show-origin is only applicable to --get, --get-all, "
			"--get-regexp, and --list"));
		निकास(129);
	}

	अगर (display_opts.default_मूल्य && !(actions & ACTION_GET)) {
		error(_("--default is only applicable to --get"));
		निकास(129);
	}

	अगर (comment_arg &&
	    !(actions & (ACTION_ADD|ACTION_SET|ACTION_SET_ALL|ACTION_REPLACE_ALL))) {
		error(_("--comment is only applicable to add/set/replace operations"));
		निकास(129);
	}

	/* check usage of --fixed-value */
	अगर (flags & CONFIG_FLAGS_FIXED_मूल्य) {
		पूर्णांक allowed_usage = 0;

		चयन (actions) {
		/* git config --get <name> <value-pattern> */
		हाल ACTION_GET:
		/* git config --get-all <name> <value-pattern> */
		हाल ACTION_GET_ALL:
		/* git config --get-regexp <name-pattern> <value-pattern> */
		हाल ACTION_GET_REGEXP:
		/* git config --unset <name> <value-pattern> */
		हाल ACTION_UNSET:
		/* git config --unset-all <name> <value-pattern> */
		हाल ACTION_UNSET_ALL:
			allowed_usage = argc > 1 && !!argv[1];
			अवरोध;

		/* git config <name> <value> <value-pattern> */
		हाल ACTION_SET_ALL:
		/* git config --replace-all <name> <value> <value-pattern> */
		हाल ACTION_REPLACE_ALL:
			allowed_usage = argc > 2 && !!argv[2];
			अवरोध;

		/* other options don't allow --fixed-value */
		}

		अगर (!allowed_usage) {
			error(_("--fixed-value only applies with 'value-pattern'"));
			निकास(129);
		}
	}

	comment = git_config_prepare_comment_string(comment_arg);

	/*
	 * The following actions may produce more than one line of output and
	 * should therefore be paged.
	 */
	अगर (actions & (ACTION_LIST | ACTION_GET_ALL | ACTION_GET_REGEXP | ACTION_GET_URLMATCH))
		setup_auto_pager("config", 1);

	अगर (actions == ACTION_LIST) {
		check_argc(argc, 0, 0);
		display_options_init_list(&display_opts);
		अगर (config_with_options(show_all_config, &display_opts,
					&location_opts.source, the_repository,
					&location_opts.options) < 0) {
			अगर (location_opts.source.file)
				die_errno(_("unable to read config file '%s'"),
					  location_opts.source.file);
			अन्यथा
				die(_("error processing config file(s)"));
		}
	}
	अन्यथा अगर (actions == ACTION_EDIT) {
		ret = show_editor(&location_opts);
	}
	अन्यथा अगर (actions == ACTION_SET) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मूल्य = normalize_मूल्य(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file, argv[0], comment, मूल्य);
		अगर (ret == CONFIG_NOTHING_SET)
			error(_("cannot overwrite multiple values with a single value\n"
			"       Use a regexp, --add or --replace-all to change %s."), argv[0]);
	}
	अन्यथा अगर (actions == ACTION_SET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मूल्य = normalize_मूल्य(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मूल्य, argv[2],
							      comment, flags);
	}
	अन्यथा अगर (actions == ACTION_ADD) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		मूल्य = normalize_मूल्य(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मूल्य,
							      CONFIG_REGEX_NONE,
							      comment, flags);
	}
	अन्यथा अगर (actions == ACTION_REPLACE_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 3);
		मूल्य = normalize_मूल्य(argv[0], argv[1], display_opts.type, &default_kvi);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], मूल्य, argv[2],
							      comment, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अन्यथा अगर (actions == ACTION_GET) {
		check_argc(argc, 1, 2);
		ret = get_मूल्य(&location_opts, &display_opts, argv[0], argv[1],
				0, flags);
	}
	अन्यथा अगर (actions == ACTION_GET_ALL) {
		check_argc(argc, 1, 2);
		ret = get_मूल्य(&location_opts, &display_opts, argv[0], argv[1],
				GET_मूल्य_ALL, flags);
	}
	अन्यथा अगर (actions == ACTION_GET_REGEXP) {
		display_opts.show_कुंजियाँ = 1;
		check_argc(argc, 1, 2);
		ret = get_मूल्य(&location_opts, &display_opts, argv[0], argv[1],
				GET_मूल्य_ALL|GET_मूल्य_कुंजी_REGEXP, flags);
	}
	अन्यथा अगर (actions == ACTION_GET_URLMATCH) {
		check_argc(argc, 2, 2);
		ret = get_urlmatch(&location_opts, &display_opts, argv[0], argv[1]);
	}
	अन्यथा अगर (actions == ACTION_UNSET) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		अगर (argc == 2)
			ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
								      argv[0], शून्य, argv[1],
								      शून्य, flags);
		अन्यथा
			ret = repo_config_set_in_file_gently(the_repository, location_opts.source.file,
							     argv[0], शून्य, शून्य);
	}
	अन्यथा अगर (actions == ACTION_UNSET_ALL) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 2);
		ret = repo_config_set_multivar_in_file_gently(the_repository, location_opts.source.file,
							      argv[0], शून्य, argv[1],
							      शून्य, flags | CONFIG_FLAGS_MULTI_REPLACE);
	}
	अन्यथा अगर (actions == ACTION_RENAME_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 2, 2);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], argv[1]);
		अगर (ret < 0)
			जाओ out;
		अन्यथा अगर (!ret)
			die(_("no such section: %s"), argv[0]);
		अन्यथा
			ret = 0;
	}
	अन्यथा अगर (actions == ACTION_REMOVE_SECTION) {
		check_write(&location_opts.source);
		check_argc(argc, 1, 1);
		ret = repo_config_rename_section_in_file(the_repository, location_opts.source.file,
							 argv[0], शून्य);
		अगर (ret < 0)
			जाओ out;
		अन्यथा अगर (!ret)
			die(_("no such section: %s"), argv[0]);
		अन्यथा
			ret = 0;
	}
	अन्यथा अगर (actions == ACTION_GET_COLOR) {
		check_argc(argc, 1, 2);
		ret = get_color(&location_opts, argv[0], argv[1]);
	}
	अन्यथा अगर (actions == ACTION_GET_COLORBOOL) {
		check_argc(argc, 1, 2);
		अगर (argc == 2)
			color_stdout_is_tty = git_config_bool("command line", argv[1]);
		ret = get_colorbool(&location_opts, argv[0], argc == 2);
	}

out:
	location_options_release(&location_opts);
	मुक्त(comment);
	मुक्त(मूल्य);
	वापस ret;
}

पूर्णांक cmd_config(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *subcommand = शून्य;
	काष्ठा option subcommand_opts[] = {
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
	अगर (subcommand) {
		argc = parse_options(argc, argv, prefix, subcommand_opts, builtin_config_usage,
		       PARSE_OPT_SUBCOMMAND_OPTIONAL|PARSE_OPT_KEEP_UNKNOWN_OPT);
		वापस subcommand(argc, argv, prefix, repo);
	}

	वापस cmd_config_actions(argc, argv, prefix);
}
