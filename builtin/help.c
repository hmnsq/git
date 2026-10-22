/*
 * Builtin help command
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "exec-cmd.h"
#ضمن "gettext.h"
#ضمن "pager.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "run-command.h"
#ضمن "config-list.h"
#ضمن "help.h"
#ضمن "alias.h"
#ضمن "setup.h"

#اگر_تعریف_نشده DEFAULT_HELP_FORMAT
#تعریف_کن DEFAULT_HELP_FORMAT "man"
#پایان_اگر

ساكن بنية man_viewer_list {
	بنية man_viewer_list *بعدی;
	حرف name[FLEX_ARRAY];
} *man_viewer_list;

ساكن بنية man_viewer_info_list {
	بنية man_viewer_info_list *بعدی;
	ثابت حرف *info;
	حرف name[FLEX_ARRAY];
} *man_viewer_info_list;

تعداد help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

تعداد show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ساكن تعداد help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_نما;

ساكن حرف *html_مسیر;
ساكن صحيح verbose = 1;
ساكن تعداد help_format help_format = HELP_FORMAT_NONE;
ساكن صحيح exclude_guides;
ساكن صحيح show_external_commands = -1;
ساكن صحيح show_aliases = -1;
ساكن بنية option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_نما, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_مجموعه_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_مجموعه_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_مجموعه_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_نما, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_نما,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_نما,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_نما, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_نما, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_نما, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_نما, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_انتها(),
};

ساكن ثابت حرف * ثابت builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	عدم
};

بنية جایگاه_expansion {
	ثابت حرف *پیشوند;
	ثابت حرف *placeholder;
	فراغ (*fn)(بنية string_list *list, ثابت حرف *پیشوند);
	صحيح یافت_2;
};

ساكن فراغ مجموعه_config_vars(بنية string_list *کلیدها_uniq, بنية string_list_قلم *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *wildcard = strchr(str, '*');
	ثابت حرف *برچسب = strchr(str, '<');
	ثابت حرف *cut;

	إذا (wildcard && برچسب)
		cut = wildcard < برچسب ? wildcard : برچسب;
	وإلا إذا (wildcard)
		cut = wildcard;
	وإلا إذا (برچسب)
		cut = برچسب;
	وإلا {
		string_list_append(کلیدها_uniq, str);
		أرجع;
	}

	strbuf_جمع_کن(&sb, str, cut - str);
	string_list_append(کلیدها_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ مجموعه_config_sections(بنية string_list *کلیدها_uniq, بنية string_list_قلم *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *dot = strchr(str, '.');
	ثابت حرف *cut;

	إذا (dot)
		cut = dot;
	وإلا {
		مجموعه_config_vars(کلیدها_uniq, var);
		أرجع;
	}

	strbuf_جمع_کن(&sb, str, cut - str);
	string_list_append(کلیدها_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ list_config_help(تعداد show_config_type type)
{
	بنية جایگاه_expansion جایگاه_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_وضعیت_slots },
		{ "fsck", "<msg-id>", list_config_fsck_پیام_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_پیام_ids },
		{ عدم, عدم, عدم }
	};
	ثابت حرف **p;
	بنية جایگاه_expansion *e;
	بنية string_list کلیدها = STRING_LIST_INIT_DUP;
	بنية string_list کلیدها_uniq = STRING_LIST_INIT_DUP;
	بنية string_list_قلم *قلم;
	بنية strbuf sb = STRBUF_INIT;

	لكل (p = config_name_list; *p; p++) {
		ثابت حرف *var = *p;

		لكل (e = جایگاه_expansions; e->پیشوند; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->پیشوند, e->placeholder);
			إذا (!strcasecmp(var, sb.buf)) {
				e->fn(&کلیدها, e->پیشوند);
				e->یافت_2++;
				اكسر;
			}
		}

		إذا (!e->پیشوند)
			string_list_append(&کلیدها, var);
	}

	strbuf_release(&sb);

	لكل (e = جایگاه_expansions; e->پیشوند; e++)
		إذا (!e->یافت_2)
			BUG("slot_expansion %s.%s is not used",
			    e->پیشوند, e->placeholder);

	لكل (اندازه_t i = 0; i < کلیدها.nr; i++) {
		بدل (type) {
		حالة SHOW_CONFIG_HUMAN:
			string_list_append(&کلیدها_uniq, کلیدها.اقلام[i].string);
			اكسر;
		حالة SHOW_CONFIG_SECTIONS:
			مجموعه_config_sections(&کلیدها_uniq, &کلیدها.اقلام[i]);
			اكسر;
		حالة SHOW_CONFIG_VARS:
			مجموعه_config_vars(&کلیدها_uniq, &کلیدها.اقلام[i]);
			اكسر;
		افتراضي:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_مرتب_سازی_u(&کلیدها_uniq, 0);
	for_each_string_list_قلم(قلم, &کلیدها_uniq)
		puts(قلم->string);
	string_list_clear(&کلیدها_uniq, 0);
	string_list_clear(&کلیدها, 0);
}

ساكن تعداد help_format parse_help_format(ثابت حرف *format)
{
	إذا (!strcmp(format, "man"))
		أرجع HELP_FORMAT_MAN;
	إذا (!strcmp(format, "info"))
		أرجع HELP_FORMAT_INFO;
	إذا (!strcmp(format, "web") || !strcmp(format, "html"))
		أرجع HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ساكن ثابت حرف *get_man_viewer_info(ثابت حرف *name)
{
	بنية man_viewer_info_list *viewer;

	لكل (viewer = man_viewer_info_list; viewer; viewer = viewer->بعدی)
	{
		إذا (!strcasecmp(name, viewer->name))
			أرجع viewer->info;
	}
	أرجع عدم;
}

ساكن صحيح check_emacsclient_نسخه(فراغ)
{
	بنية strbuf میانگیر = STRBUF_INIT;
	بنية فرزند_process ec_process = فرزند_PROCESS_INIT;
	صحيح نسخه;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.آرگومانها, "emacsclient", "--version", عدم);
	ec_process.err = -1;
	ec_process.stdout_مقصد_stderr = 1;
	إذا (شروع_command(&ec_process))
		أرجع error(_("Failed to start emacsclient."));

	strbuf_read(&میانگیر, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	پایان_command(&ec_process);

	إذا (!starts_with(میانگیر.buf, "emacsclient")) {
		strbuf_release(&میانگیر);
		أرجع error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&میانگیر, 0, طول_رشته("emacsclient"));
	نسخه = atoi(میانگیر.buf);

	إذا (نسخه < 22) {
		strbuf_release(&میانگیر);
		أرجع error(_("emacsclient version '%d' too old (< 22)."),
			نسخه);
	}

	strbuf_release(&میانگیر);
	أرجع 0;
}

ساكن فراغ exec_woman_emacs(ثابت حرف *مسیر, ثابت حرف *صفحه)
{
	إذا (!check_emacsclient_نسخه()) {
		/* This works only with emacsclient version >= 22. */
		بنية strbuf man_صفحه = STRBUF_INIT;

		إذا (!مسیر)
			مسیر = "emacsclient";
		strbuf_addf(&man_صفحه, "(woman \"%s\")", صفحه);
		execlp(مسیر, "emacsclient", "-e", man_صفحه.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), مسیر);
		strbuf_release(&man_صفحه);
	}
}

ساكن فراغ exec_man_konqueror(ثابت حرف *مسیر, ثابت حرف *صفحه)
{
	ثابت حرف *display = getenv("DISPLAY");
	إذا (display && *display) {
		بنية strbuf man_صفحه = STRBUF_INIT;
		ثابت حرف *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		إذا (مسیر) {
			اندازه_t len;
			إذا (strip_suffix(مسیر, "/konqueror", &len))
				مسیر = xstrfmt("%.*s/kfmclient", (صحيح)len, مسیر);
			filename = basename((حرف *)مسیر);
		} وإلا
			مسیر = "kfmclient";
		strbuf_addf(&man_صفحه, "man:%s(1)", صفحه);
		execlp(مسیر, filename, "newTab", man_صفحه.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), مسیر);
		strbuf_release(&man_صفحه);
	}
}

ساكن فراغ exec_man_man(ثابت حرف *مسیر, ثابت حرف *صفحه)
{
	إذا (!مسیر)
		مسیر = "man";
	execlp(مسیر, "man", صفحه, (حرف *)عدم);
	warning_errno(_("failed to exec '%s'"), مسیر);
}

ساكن فراغ exec_man_cmd(ثابت حرف *cmd, ثابت حرف *صفحه)
{
	بنية strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, صفحه);
	execl(SHELL_مسیر, SHELL_مسیر, "-c", shell_cmd.buf, (حرف *)عدم);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ساكن فراغ جمع_کن_man_viewer(ثابت حرف *name)
{
	بنية man_viewer_list **p = &man_viewer_list;

	طالما (*p)
		p = &((*p)->بعدی);
	FLEX_ALLOC_STR(*p, name, name);
}

ساكن صحيح supported_man_viewer(ثابت حرف *name, اندازه_t len)
{
	أرجع (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ساكن فراغ do_جمع_کن_man_viewer_info(ثابت حرف *name,
				   اندازه_t len,
				   ثابت حرف *مقدار)
{
	بنية man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(مقدار);
	new_man_viewer->بعدی = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ساكن صحيح جمع_کن_man_viewer_مسیر(ثابت حرف *name,
			       اندازه_t len,
			       ثابت حرف *مقدار)
{
	إذا (supported_man_viewer(name, len))
		do_جمع_کن_man_viewer_info(name, len, مقدار);
	وإلا
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	أرجع 0;
}

ساكن صحيح جمع_کن_man_viewer_cmd(ثابت حرف *name,
			      اندازه_t len,
			      ثابت حرف *مقدار)
{
	إذا (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	وإلا
		do_جمع_کن_man_viewer_info(name, len, مقدار);

	أرجع 0;
}

ساكن صحيح جمع_کن_man_viewer_info(ثابت حرف *var, ثابت حرف *مقدار)
{
	ثابت حرف *name, *subkey;
	اندازه_t namelen;

	إذا (parse_config_کلید(var, "man", &name, &namelen, &subkey) < 0 || !name)
		أرجع 0;

	إذا (!strcmp(subkey, "path")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		أرجع جمع_کن_man_viewer_مسیر(name, namelen, مقدار);
	}
	إذا (!strcmp(subkey, "cmd")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		أرجع جمع_کن_man_viewer_cmd(name, namelen, مقدار);
	}

	أرجع 0;
}

ساكن صحيح git_help_config(ثابت حرف *var, ثابت حرف *مقدار,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "help.format")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		help_format = parse_help_format(مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "help.htmlpath")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		حرر(html_مسیر);
		html_مسیر = xstrdup(مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "man.viewer")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		جمع_کن_man_viewer(مقدار);
		أرجع 0;
	}
	إذا (starts_with(var, "man."))
		أرجع جمع_کن_man_viewer_info(var, مقدار);

	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن بنية cmdnames main_cmds, other_cmds;

ساكن صحيح is_git_command(ثابت حرف *s)
{
	إذا (is_builtin(s))
		أرجع 1;

	بارگیری_command_list("git-", &main_cmds, &other_cmds);
	أرجع is_درون_cmdlist(&main_cmds, s) ||
		is_درون_cmdlist(&other_cmds, s);
}

ساكن ثابت حرف *cmd_مقصد_صفحه(ثابت حرف *git_cmd)
{
	إذا (!git_cmd)
		أرجع "git";
	وإلا إذا (starts_with(git_cmd, "git"))
		أرجع git_cmd;
	وإلا إذا (is_git_command(git_cmd))
		أرجع xstrfmt("git-%s", git_cmd);
	وإلا إذا (!strcmp("scalar", git_cmd))
		أرجع xstrdup(git_cmd);
	وإلا
		أرجع xstrfmt("git%s", git_cmd);
}

ساكن فراغ راهاندازی_man_مسیر(فراغ)
{
	بنية strbuf new_مسیر = STRBUF_INIT;
	ثابت حرف *old_مسیر = getenv("MANPATH");
	حرف *git_man_مسیر = system_مسیر(GIT_MAN_مسیر);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_مسیر, git_man_مسیر);
	strbuf_addch(&new_مسیر, ':');
	إذا (old_مسیر)
		strbuf_addstr(&new_مسیر, old_مسیر);

	حرر(git_man_مسیر);
	setenv("MANPATH", new_مسیر.buf, 1);

	strbuf_release(&new_مسیر);
}

ساكن فراغ exec_viewer(ثابت حرف *name, ثابت حرف *صفحه)
{
	ثابت حرف *info = get_man_viewer_info(name);

	إذا (!strcasecmp(name, "man"))
		exec_man_man(info, صفحه);
	وإلا إذا (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, صفحه);
	وإلا إذا (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, صفحه);
	وإلا إذا (info)
		exec_man_cmd(info, صفحه);
	وإلا
		warning(_("'%s': unknown man viewer."), name);
}

ساكن فراغ show_man_صفحه(ثابت حرف *صفحه)
{
	بنية man_viewer_list *viewer;
	ثابت حرف *fallback = getenv("GIT_MAN_VIEWER");

	راهاندازی_man_مسیر();
	لكل (viewer = man_viewer_list; viewer; viewer = viewer->بعدی)
	{
		exec_viewer(viewer->name, صفحه); /* will return when unable */
	}
	إذا (fallback)
		exec_viewer(fallback, صفحه);
	exec_viewer("man", صفحه);
	die(_("no man viewer handled the request"));
}

ساكن فراغ show_info_صفحه(ثابت حرف *صفحه)
{
	setenv("INFOPATH", system_مسیر(GIT_INFO_مسیر), 1);
	execlp("info", "info", "gitman", صفحه, (حرف *)عدم);
	die(_("no info viewer handled the request"));
}

ساكن فراغ get_html_صفحه_مسیر(بنية strbuf *صفحه_مسیر, ثابت حرف *صفحه)
{
	بنية stat st;
	ثابت حرف *مسیر = html_مسیر;
	حرف *مقصد_free = عدم;

	إذا (!مسیر)
		مسیر = مقصد_free = system_مسیر(GIT_HTML_مسیر);

	/*
	 * Check that the page we're looking for exists.
	 */
	إذا (!جستجوی_رشته(مسیر, "://")) {
		إذا (stat(mkpath("%s/%s.html", مسیر, صفحه), &st)
		    || !S_ISREG(st.st_نما))
			die("'%s/%s.html': documentation file not found.",
				مسیر, صفحه);
	}

	strbuf_init(صفحه_مسیر, 0);
	strbuf_addf(صفحه_مسیر, "%s/%s.html", مسیر, صفحه);
	حرر(مقصد_free);
}

ساكن فراغ open_html(ثابت حرف *مسیر)
{
	execl_git_cmd("web--browse", "-c", "help.browser", مسیر, (حرف *)عدم);
}

ساكن فراغ show_html_صفحه(ثابت حرف *صفحه)
{
	بنية strbuf صفحه_مسیر; /* it leaks but we exec below */

	get_html_صفحه_مسیر(&صفحه_مسیر, صفحه);

	open_html(صفحه_مسیر.buf);
}

ساكن حرف *check_git_cmd(ثابت حرف *cmd)
{
	حرف *alias;

	إذا (is_git_command(cmd))
		أرجع xstrdup(cmd);

	alias = alias_lookup(cmd);
	إذا (alias) {
		ثابت حرف **argv;
		صحيح شمار;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		إذا (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			حرر(alias);
			خروج(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		شمار = تقسیم_cmdline(alias, &argv);
		إذا (شمار < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    تقسیم_cmdline_strerror(شمار));
		حرر(argv);
		أرجع alias;
	}

	إذا (exclude_guides)
		أرجع help_unknown_cmd(cmd);

	أرجع xstrdup(cmd);
}

ساكن فراغ no_help_format(ثابت حرف *opt_نما, تعداد help_format fmt)
{
	ثابت حرف *opt_fmt;

	بدل (fmt) {
	حالة HELP_FORMAT_NONE:
		أرجع;
	حالة HELP_FORMAT_MAN:
		opt_fmt = "--man";
		اكسر;
	حالة HELP_FORMAT_INFO:
		opt_fmt = "--info";
		اكسر;
	حالة HELP_FORMAT_WEB:
		opt_fmt = "--web";
		اكسر;
	افتراضي:
		BUG("unreachable");
	}

	usage_پیام_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_نما,
		       opt_fmt);
}

ساكن فراغ opt_نما_usage(صحيح argc, ثابت حرف *opt_نما,
			   تعداد help_format fmt)
{
	إذا (argc)
		usage_پیام_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_نما);

	no_help_format(opt_نما, fmt);
}

صحيح cmd_help(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *پیشوند,
	     بنية repository *repo استفادهنشده)
{
	صحيح nongit;
	تعداد help_format parsed_help_format;
	حرف *command = عدم;
	ثابت حرف *صفحه;

	argc = parse_options(argc, argv, پیشوند, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	إذا (cmd_نما != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_پیام_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	بدل (cmd_نما) {
	حالة HELP_ACTION_ALL:
		opt_نما_usage(argc, "--all", help_format);
		إذا (verbose) {
			راهاندازی_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			أرجع 0;
		}
		چاپ_کن(_("usage: %s%s"), _(git_usage_string), "\n\n");
		بارگیری_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		چاپ_کن("%s\n", _(git_بیشتر_info_string));
		اكسر;
	حالة HELP_ACTION_GUIDES:
		opt_نما_usage(argc, "--guides", help_format);
		list_guides_help();
		چاپ_کن("%s\n", _(git_بیشتر_info_string));
		أرجع 0;
	حالة HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_نما_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		أرجع 0;
	حالة HELP_ACTION_USER_INTERFACES:
		opt_نما_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_DEVELOPER_INTERFACES:
		opt_نما_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_نما_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		أرجع 0;
	حالة HELP_ACTION_ALIASES_FOR_COMPLETION: {
		بنية string_list alias_list = STRING_LIST_INIT_DUP;
		opt_نما_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		لكل (اندازه_t i = 0; i < alias_list.nr; i++)
			چاپ_کن("%s%c%s%c", alias_list.اقلام[i].string, '\n',
			       (حرف *)alias_list.اقلام[i].util, '\0');
		string_list_clear(&alias_list, 1);
		أرجع 0;
	}
	حالة HELP_ACTION_CONFIG:
		opt_نما_usage(argc, "--config", help_format);
		راهاندازی_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		چاپ_کن("\n%s\n", _("'git help config' for more information"));
		أرجع 0;
	}

	إذا (!argv[0]) {
		چاپ_کن(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		چاپ_کن("\n%s\n", _(git_بیشتر_info_string));
		أرجع 0;
	}

	راهاندازی_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, عدم);

	إذا (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	إذا (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	صفحه = cmd_مقصد_صفحه(command);
	بدل (help_format) {
	حالة HELP_FORMAT_NONE:
	حالة HELP_FORMAT_MAN:
		show_man_صفحه(صفحه);
		اكسر;
	حالة HELP_FORMAT_INFO:
		show_info_صفحه(صفحه);
		اكسر;
	حالة HELP_FORMAT_WEB:
		show_html_صفحه(صفحه);
		اكسر;
	}

	حرر(command);
	أرجع 0;
}
