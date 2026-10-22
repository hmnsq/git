/*
 * Builtin help command
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "exec-cmd.h"
#شامل "gettext.h"
#شامل "pager.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "run-command.h"
#شامل "config-list.h"
#شامل "help.h"
#شامل "alias.h"
#شامل "setup.h"

#اگر_غیر_تعریف_شدہ DEFAULT_HELP_FORMAT
#تعریف_کرو DEFAULT_HELP_FORMAT "man"
#اگر_ختم

ساکن ساخت man_viewer_list {
	ساخت man_viewer_list *اگلا;
	حرف name[FLEX_ARRAY];
} *man_viewer_list;

ساکن ساخت man_viewer_info_list {
	ساخت man_viewer_info_list *اگلا;
	ثابت حرف *info;
	حرف name[FLEX_ARRAY];
} *man_viewer_info_list;

شمار help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

شمار show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ساکن شمار help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_منوال;

ساکن حرف *html_راستہ;
ساکن صحیح verbose = 1;
ساکن شمار help_format help_format = HELP_FORMAT_NONE;
ساکن صحیح exclude_guides;
ساکن صحیح show_external_commands = -1;
ساکن صحیح show_aliases = -1;
ساکن ساخت option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_منوال, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_سیٹ_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_سیٹ_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_سیٹ_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_منوال, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_منوال,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_منوال,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_منوال, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_منوال, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_منوال, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_منوال, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_انتہا(),
};

ساکن ثابت حرف * ثابت builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	تہی
};

ساخت خانہ_expansion {
	ثابت حرف *سابقہ;
	ثابت حرف *placeholder;
	خالی (*fn)(ساخت string_list *list, ثابت حرف *سابقہ);
	صحیح ملا_2;
};

ساکن خالی سیٹ_config_vars(ساخت string_list *کلیدیں_uniq, ساخت string_list_شے *var)
{
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *wildcard = strchr(str, '*');
	ثابت حرف *لیبل = strchr(str, '<');
	ثابت حرف *cut;

	اگر (wildcard && لیبل)
		cut = wildcard < لیبل ? wildcard : لیبل;
	ورنہ اگر (wildcard)
		cut = wildcard;
	ورنہ اگر (لیبل)
		cut = لیبل;
	ورنہ {
		string_list_append(کلیدیں_uniq, str);
		واپس;
	}

	strbuf_جوڑو(&sb, str, cut - str);
	string_list_append(کلیدیں_uniq, sb.buf);
	strbuf_release(&sb);
}

ساکن خالی سیٹ_config_sections(ساخت string_list *کلیدیں_uniq, ساخت string_list_شے *var)
{
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *dot = strchr(str, '.');
	ثابت حرف *cut;

	اگر (dot)
		cut = dot;
	ورنہ {
		سیٹ_config_vars(کلیدیں_uniq, var);
		واپس;
	}

	strbuf_جوڑو(&sb, str, cut - str);
	string_list_append(کلیدیں_uniq, sb.buf);
	strbuf_release(&sb);
}

ساکن خالی list_config_help(شمار show_config_type type)
{
	ساخت خانہ_expansion خانہ_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_حالت_slots },
		{ "fsck", "<msg-id>", list_config_fsck_پیغام_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_پیغام_ids },
		{ تہی, تہی, تہی }
	};
	ثابت حرف **p;
	ساخت خانہ_expansion *e;
	ساخت string_list کلیدیں = STRING_LIST_INIT_DUP;
	ساخت string_list کلیدیں_uniq = STRING_LIST_INIT_DUP;
	ساخت string_list_شے *شے;
	ساخت strbuf sb = STRBUF_INIT;

	برائے (p = config_name_list; *p; p++) {
		ثابت حرف *var = *p;

		برائے (e = خانہ_expansions; e->سابقہ; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->سابقہ, e->placeholder);
			اگر (!strcasecmp(var, sb.buf)) {
				e->fn(&کلیدیں, e->سابقہ);
				e->ملا_2++;
				توڑو;
			}
		}

		اگر (!e->سابقہ)
			string_list_append(&کلیدیں, var);
	}

	strbuf_release(&sb);

	برائے (e = خانہ_expansions; e->سابقہ; e++)
		اگر (!e->ملا_2)
			BUG("slot_expansion %s.%s is not used",
			    e->سابقہ, e->placeholder);

	برائے (سائز_t i = 0; i < کلیدیں.nr; i++) {
		انتخاب (type) {
		حالت SHOW_CONFIG_HUMAN:
			string_list_append(&کلیدیں_uniq, کلیدیں.اشیاء[i].string);
			توڑو;
		حالت SHOW_CONFIG_SECTIONS:
			سیٹ_config_sections(&کلیدیں_uniq, &کلیدیں.اشیاء[i]);
			توڑو;
		حالت SHOW_CONFIG_VARS:
			سیٹ_config_vars(&کلیدیں_uniq, &کلیدیں.اشیاء[i]);
			توڑو;
		طےشدہ:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_ترتیب_دو_u(&کلیدیں_uniq, 0);
	for_each_string_list_شے(شے, &کلیدیں_uniq)
		puts(شے->string);
	string_list_clear(&کلیدیں_uniq, 0);
	string_list_clear(&کلیدیں, 0);
}

ساکن شمار help_format parse_help_format(ثابت حرف *format)
{
	اگر (!strcmp(format, "man"))
		واپس HELP_FORMAT_MAN;
	اگر (!strcmp(format, "info"))
		واپس HELP_FORMAT_INFO;
	اگر (!strcmp(format, "web") || !strcmp(format, "html"))
		واپس HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ساکن ثابت حرف *get_man_viewer_info(ثابت حرف *name)
{
	ساخت man_viewer_info_list *viewer;

	برائے (viewer = man_viewer_info_list; viewer; viewer = viewer->اگلا)
	{
		اگر (!strcasecmp(name, viewer->name))
			واپس viewer->info;
	}
	واپس تہی;
}

ساکن صحیح check_emacsclient_نسخہ(خالی)
{
	ساخت strbuf بفر = STRBUF_INIT;
	ساخت بچہ_process ec_process = بچہ_PROCESS_INIT;
	صحیح نسخہ;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.دلائل, "emacsclient", "--version", تہی);
	ec_process.err = -1;
	ec_process.stdout_مقصد_stderr = 1;
	اگر (آغاز_command(&ec_process))
		واپس error(_("Failed to start emacsclient."));

	strbuf_read(&بفر, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	اختتام_command(&ec_process);

	اگر (!starts_with(بفر.buf, "emacsclient")) {
		strbuf_release(&بفر);
		واپس error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&بفر, 0, لڑی_لمبائی("emacsclient"));
	نسخہ = atoi(بفر.buf);

	اگر (نسخہ < 22) {
		strbuf_release(&بفر);
		واپس error(_("emacsclient version '%d' too old (< 22)."),
			نسخہ);
	}

	strbuf_release(&بفر);
	واپس 0;
}

ساکن خالی exec_woman_emacs(ثابت حرف *راستہ, ثابت حرف *صفحہ)
{
	اگر (!check_emacsclient_نسخہ()) {
		/* This works only with emacsclient version >= 22. */
		ساخت strbuf man_صفحہ = STRBUF_INIT;

		اگر (!راستہ)
			راستہ = "emacsclient";
		strbuf_addf(&man_صفحہ, "(woman \"%s\")", صفحہ);
		execlp(راستہ, "emacsclient", "-e", man_صفحہ.buf, (حرف *)تہی);
		warning_errno(_("failed to exec '%s'"), راستہ);
		strbuf_release(&man_صفحہ);
	}
}

ساکن خالی exec_man_konqueror(ثابت حرف *راستہ, ثابت حرف *صفحہ)
{
	ثابت حرف *display = getenv("DISPLAY");
	اگر (display && *display) {
		ساخت strbuf man_صفحہ = STRBUF_INIT;
		ثابت حرف *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		اگر (راستہ) {
			سائز_t len;
			اگر (strip_suffix(راستہ, "/konqueror", &len))
				راستہ = xstrfmt("%.*s/kfmclient", (صحیح)len, راستہ);
			filename = basename((حرف *)راستہ);
		} ورنہ
			راستہ = "kfmclient";
		strbuf_addf(&man_صفحہ, "man:%s(1)", صفحہ);
		execlp(راستہ, filename, "newTab", man_صفحہ.buf, (حرف *)تہی);
		warning_errno(_("failed to exec '%s'"), راستہ);
		strbuf_release(&man_صفحہ);
	}
}

ساکن خالی exec_man_man(ثابت حرف *راستہ, ثابت حرف *صفحہ)
{
	اگر (!راستہ)
		راستہ = "man";
	execlp(راستہ, "man", صفحہ, (حرف *)تہی);
	warning_errno(_("failed to exec '%s'"), راستہ);
}

ساکن خالی exec_man_cmd(ثابت حرف *cmd, ثابت حرف *صفحہ)
{
	ساخت strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, صفحہ);
	execl(SHELL_راستہ, SHELL_راستہ, "-c", shell_cmd.buf, (حرف *)تہی);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ساکن خالی جوڑو_man_viewer(ثابت حرف *name)
{
	ساخت man_viewer_list **p = &man_viewer_list;

	جب (*p)
		p = &((*p)->اگلا);
	FLEX_ALLOC_STR(*p, name, name);
}

ساکن صحیح supported_man_viewer(ثابت حرف *name, سائز_t len)
{
	واپس (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ساکن خالی do_جوڑو_man_viewer_info(ثابت حرف *name,
				   سائز_t len,
				   ثابت حرف *قدر)
{
	ساخت man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(قدر);
	new_man_viewer->اگلا = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ساکن صحیح جوڑو_man_viewer_راستہ(ثابت حرف *name,
			       سائز_t len,
			       ثابت حرف *قدر)
{
	اگر (supported_man_viewer(name, len))
		do_جوڑو_man_viewer_info(name, len, قدر);
	ورنہ
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	واپس 0;
}

ساکن صحیح جوڑو_man_viewer_cmd(ثابت حرف *name,
			      سائز_t len,
			      ثابت حرف *قدر)
{
	اگر (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	ورنہ
		do_جوڑو_man_viewer_info(name, len, قدر);

	واپس 0;
}

ساکن صحیح جوڑو_man_viewer_info(ثابت حرف *var, ثابت حرف *قدر)
{
	ثابت حرف *name, *subkey;
	سائز_t namelen;

	اگر (parse_config_کلید(var, "man", &name, &namelen, &subkey) < 0 || !name)
		واپس 0;

	اگر (!strcmp(subkey, "path")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		واپس جوڑو_man_viewer_راستہ(name, namelen, قدر);
	}
	اگر (!strcmp(subkey, "cmd")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		واپس جوڑو_man_viewer_cmd(name, namelen, قدر);
	}

	واپس 0;
}

ساکن صحیح git_help_config(ثابت حرف *var, ثابت حرف *قدر,
			   ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "help.format")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		help_format = parse_help_format(قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "help.htmlpath")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		آزاد(html_راستہ);
		html_راستہ = xstrdup(قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "man.viewer")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		جوڑو_man_viewer(قدر);
		واپس 0;
	}
	اگر (starts_with(var, "man."))
		واپس جوڑو_man_viewer_info(var, قدر);

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن ساخت cmdnames main_cmds, other_cmds;

ساکن صحیح is_git_command(ثابت حرف *s)
{
	اگر (is_builtin(s))
		واپس 1;

	بھرو_command_list("git-", &main_cmds, &other_cmds);
	واپس is_اندر_cmdlist(&main_cmds, s) ||
		is_اندر_cmdlist(&other_cmds, s);
}

ساکن ثابت حرف *cmd_مقصد_صفحہ(ثابت حرف *git_cmd)
{
	اگر (!git_cmd)
		واپس "git";
	ورنہ اگر (starts_with(git_cmd, "git"))
		واپس git_cmd;
	ورنہ اگر (is_git_command(git_cmd))
		واپس xstrfmt("git-%s", git_cmd);
	ورنہ اگر (!strcmp("scalar", git_cmd))
		واپس xstrdup(git_cmd);
	ورنہ
		واپس xstrfmt("git%s", git_cmd);
}

ساکن خالی تیاری_man_راستہ(خالی)
{
	ساخت strbuf new_راستہ = STRBUF_INIT;
	ثابت حرف *old_راستہ = getenv("MANPATH");
	حرف *git_man_راستہ = system_راستہ(GIT_MAN_راستہ);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_راستہ, git_man_راستہ);
	strbuf_addch(&new_راستہ, ':');
	اگر (old_راستہ)
		strbuf_addstr(&new_راستہ, old_راستہ);

	آزاد(git_man_راستہ);
	setenv("MANPATH", new_راستہ.buf, 1);

	strbuf_release(&new_راستہ);
}

ساکن خالی exec_viewer(ثابت حرف *name, ثابت حرف *صفحہ)
{
	ثابت حرف *info = get_man_viewer_info(name);

	اگر (!strcasecmp(name, "man"))
		exec_man_man(info, صفحہ);
	ورنہ اگر (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, صفحہ);
	ورنہ اگر (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, صفحہ);
	ورنہ اگر (info)
		exec_man_cmd(info, صفحہ);
	ورنہ
		warning(_("'%s': unknown man viewer."), name);
}

ساکن خالی show_man_صفحہ(ثابت حرف *صفحہ)
{
	ساخت man_viewer_list *viewer;
	ثابت حرف *fallback = getenv("GIT_MAN_VIEWER");

	تیاری_man_راستہ();
	برائے (viewer = man_viewer_list; viewer; viewer = viewer->اگلا)
	{
		exec_viewer(viewer->name, صفحہ); /* will return when unable */
	}
	اگر (fallback)
		exec_viewer(fallback, صفحہ);
	exec_viewer("man", صفحہ);
	die(_("no man viewer handled the request"));
}

ساکن خالی show_info_صفحہ(ثابت حرف *صفحہ)
{
	setenv("INFOPATH", system_راستہ(GIT_INFO_راستہ), 1);
	execlp("info", "info", "gitman", صفحہ, (حرف *)تہی);
	die(_("no info viewer handled the request"));
}

ساکن خالی get_html_صفحہ_راستہ(ساخت strbuf *صفحہ_راستہ, ثابت حرف *صفحہ)
{
	ساخت stat st;
	ثابت حرف *راستہ = html_راستہ;
	حرف *مقصد_free = تہی;

	اگر (!راستہ)
		راستہ = مقصد_free = system_راستہ(GIT_HTML_راستہ);

	/*
	 * Check that the page we're looking for exists.
	 */
	اگر (!لڑی_تلاش(راستہ, "://")) {
		اگر (stat(mkpath("%s/%s.html", راستہ, صفحہ), &st)
		    || !S_ISREG(st.st_منوال))
			die("'%s/%s.html': documentation file not found.",
				راستہ, صفحہ);
	}

	strbuf_init(صفحہ_راستہ, 0);
	strbuf_addf(صفحہ_راستہ, "%s/%s.html", راستہ, صفحہ);
	آزاد(مقصد_free);
}

ساکن خالی open_html(ثابت حرف *راستہ)
{
	execl_git_cmd("web--browse", "-c", "help.browser", راستہ, (حرف *)تہی);
}

ساکن خالی show_html_صفحہ(ثابت حرف *صفحہ)
{
	ساخت strbuf صفحہ_راستہ; /* it leaks but we exec below */

	get_html_صفحہ_راستہ(&صفحہ_راستہ, صفحہ);

	open_html(صفحہ_راستہ.buf);
}

ساکن حرف *check_git_cmd(ثابت حرف *cmd)
{
	حرف *alias;

	اگر (is_git_command(cmd))
		واپس xstrdup(cmd);

	alias = alias_lookup(cmd);
	اگر (alias) {
		ثابت حرف **argv;
		صحیح گنتی;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		اگر (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			آزاد(alias);
			باہر_نکلو(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		گنتی = بانٹو_cmdline(alias, &argv);
		اگر (گنتی < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    بانٹو_cmdline_strerror(گنتی));
		آزاد(argv);
		واپس alias;
	}

	اگر (exclude_guides)
		واپس help_unknown_cmd(cmd);

	واپس xstrdup(cmd);
}

ساکن خالی no_help_format(ثابت حرف *opt_منوال, شمار help_format fmt)
{
	ثابت حرف *opt_fmt;

	انتخاب (fmt) {
	حالت HELP_FORMAT_NONE:
		واپس;
	حالت HELP_FORMAT_MAN:
		opt_fmt = "--man";
		توڑو;
	حالت HELP_FORMAT_INFO:
		opt_fmt = "--info";
		توڑو;
	حالت HELP_FORMAT_WEB:
		opt_fmt = "--web";
		توڑو;
	طےشدہ:
		BUG("unreachable");
	}

	usage_پیغام_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_منوال,
		       opt_fmt);
}

ساکن خالی opt_منوال_usage(صحیح argc, ثابت حرف *opt_منوال,
			   شمار help_format fmt)
{
	اگر (argc)
		usage_پیغام_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_منوال);

	no_help_format(opt_منوال, fmt);
}

صحیح cmd_help(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repo غیر_مستعمل)
{
	صحیح nongit;
	شمار help_format parsed_help_format;
	حرف *command = تہی;
	ثابت حرف *صفحہ;

	argc = parse_options(argc, argv, سابقہ, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	اگر (cmd_منوال != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_پیغام_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	انتخاب (cmd_منوال) {
	حالت HELP_ACTION_ALL:
		opt_منوال_usage(argc, "--all", help_format);
		اگر (verbose) {
			تیاری_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			واپس 0;
		}
		لکھو(_("usage: %s%s"), _(git_usage_string), "\n\n");
		بھرو_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		لکھو("%s\n", _(git_مزید_info_string));
		توڑو;
	حالت HELP_ACTION_GUIDES:
		opt_منوال_usage(argc, "--guides", help_format);
		list_guides_help();
		لکھو("%s\n", _(git_مزید_info_string));
		واپس 0;
	حالت HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_منوال_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		واپس 0;
	حالت HELP_ACTION_USER_INTERFACES:
		opt_منوال_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		واپس 0;
	حالت HELP_ACTION_DEVELOPER_INTERFACES:
		opt_منوال_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		واپس 0;
	حالت HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_منوال_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		واپس 0;
	حالت HELP_ACTION_ALIASES_FOR_COMPLETION: {
		ساخت string_list alias_list = STRING_LIST_INIT_DUP;
		opt_منوال_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		برائے (سائز_t i = 0; i < alias_list.nr; i++)
			لکھو("%s%c%s%c", alias_list.اشیاء[i].string, '\n',
			       (حرف *)alias_list.اشیاء[i].util, '\0');
		string_list_clear(&alias_list, 1);
		واپس 0;
	}
	حالت HELP_ACTION_CONFIG:
		opt_منوال_usage(argc, "--config", help_format);
		تیاری_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		لکھو("\n%s\n", _("'git help config' for more information"));
		واپس 0;
	}

	اگر (!argv[0]) {
		لکھو(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		لکھو("\n%s\n", _(git_مزید_info_string));
		واپس 0;
	}

	تیاری_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, تہی);

	اگر (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	اگر (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	صفحہ = cmd_مقصد_صفحہ(command);
	انتخاب (help_format) {
	حالت HELP_FORMAT_NONE:
	حالت HELP_FORMAT_MAN:
		show_man_صفحہ(صفحہ);
		توڑو;
	حالت HELP_FORMAT_INFO:
		show_info_صفحہ(صفحہ);
		توڑو;
	حالت HELP_FORMAT_WEB:
		show_html_صفحہ(صفحہ);
		توڑو;
	}

	آزاد(command);
	واپس 0;
}
