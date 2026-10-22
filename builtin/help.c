/*
 * Builtin help command
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

#جے_بنا_تعریف DEFAULT_HELP_FORMAT
#تعریف_کرو DEFAULT_HELP_FORMAT "man"
#جے_انت

ساكن بنية man_viewer_list {
	بنية man_viewer_list *اگلا;
	حرف name[FLEX_ARRAY];
} *man_viewer_list;

ساكن بنية man_viewer_info_list {
	بنية man_viewer_info_list *اگلا;
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
} cmd_منوال;

ساكن حرف *html_رستہ;
ساكن صحيح verbose = 1;
ساكن تعداد help_format help_format = HELP_FORMAT_NONE;
ساكن صحيح exclude_guides;
ساكن صحيح show_external_commands = -1;
ساكن صحيح show_aliases = -1;
ساكن بنية option builtin_help_options[] = {
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

	OPT_سرا(),
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

بنية تھاں_expansion {
	ثابت حرف *اگیتر;
	ثابت حرف *placeholder;
	فراغ (*fn)(بنية string_list *list, ثابت حرف *اگیتر);
	صحيح لبھیا_2;
};

ساكن فراغ سیٹ_config_vars(بنية string_list *کلیدیں_uniq, بنية string_list_شے *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *wildcard = strchr(str, '*');
	ثابت حرف *ٹیگ = strchr(str, '<');
	ثابت حرف *cut;

	إذا (wildcard && ٹیگ)
		cut = wildcard < ٹیگ ? wildcard : ٹیگ;
	وإلا إذا (wildcard)
		cut = wildcard;
	وإلا إذا (ٹیگ)
		cut = ٹیگ;
	وإلا {
		string_list_append(کلیدیں_uniq, str);
		أرجع;
	}

	strbuf_جوڑو(&sb, str, cut - str);
	string_list_append(کلیدیں_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ سیٹ_config_sections(بنية string_list *کلیدیں_uniq, بنية string_list_شے *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *dot = strchr(str, '.');
	ثابت حرف *cut;

	إذا (dot)
		cut = dot;
	وإلا {
		سیٹ_config_vars(کلیدیں_uniq, var);
		أرجع;
	}

	strbuf_جوڑو(&sb, str, cut - str);
	string_list_append(کلیدیں_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ list_config_help(تعداد show_config_type type)
{
	بنية تھاں_expansion تھاں_expansions[] = {
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
		{ عدم, عدم, عدم }
	};
	ثابت حرف **p;
	بنية تھاں_expansion *e;
	بنية string_list کلیدیں = STRING_LIST_INIT_DUP;
	بنية string_list کلیدیں_uniq = STRING_LIST_INIT_DUP;
	بنية string_list_شے *شے;
	بنية strbuf sb = STRBUF_INIT;

	لكل (p = config_name_list; *p; p++) {
		ثابت حرف *var = *p;

		لكل (e = تھاں_expansions; e->اگیتر; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->اگیتر, e->placeholder);
			إذا (!strcasecmp(var, sb.buf)) {
				e->fn(&کلیدیں, e->اگیتر);
				e->لبھیا_2++;
				اكسر;
			}
		}

		إذا (!e->اگیتر)
			string_list_append(&کلیدیں, var);
	}

	strbuf_release(&sb);

	لكل (e = تھاں_expansions; e->اگیتر; e++)
		إذا (!e->لبھیا_2)
			BUG("slot_expansion %s.%s is not used",
			    e->اگیتر, e->placeholder);

	لكل (ناپ_t i = 0; i < کلیدیں.nr; i++) {
		بدل (type) {
		حالة SHOW_CONFIG_HUMAN:
			string_list_append(&کلیدیں_uniq, کلیدیں.اشیاء[i].string);
			اكسر;
		حالة SHOW_CONFIG_SECTIONS:
			سیٹ_config_sections(&کلیدیں_uniq, &کلیدیں.اشیاء[i]);
			اكسر;
		حالة SHOW_CONFIG_VARS:
			سیٹ_config_vars(&کلیدیں_uniq, &کلیدیں.اشیاء[i]);
			اكسر;
		افتراضي:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_ترتیب_دیو_u(&کلیدیں_uniq, 0);
	for_each_string_list_شے(شے, &کلیدیں_uniq)
		puts(شے->string);
	string_list_clear(&کلیدیں_uniq, 0);
	string_list_clear(&کلیدیں, 0);
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

	لكل (viewer = man_viewer_info_list; viewer; viewer = viewer->اگلا)
	{
		إذا (!strcasecmp(name, viewer->name))
			أرجع viewer->info;
	}
	أرجع عدم;
}

ساكن صحيح check_emacsclient_نسخہ(فراغ)
{
	بنية strbuf بفر = STRBUF_INIT;
	بنية بچہ_process ec_process = بچہ_PROCESS_INIT;
	صحيح نسخہ;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.دلیلاں, "emacsclient", "--version", عدم);
	ec_process.err = -1;
	ec_process.stdout_منزل_stderr = 1;
	إذا (شروع_command(&ec_process))
		أرجع error(_("Failed to start emacsclient."));

	strbuf_read(&بفر, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	انت_command(&ec_process);

	إذا (!starts_with(بفر.buf, "emacsclient")) {
		strbuf_release(&بفر);
		أرجع error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&بفر, 0, لڑی_لمبائی("emacsclient"));
	نسخہ = atoi(بفر.buf);

	إذا (نسخہ < 22) {
		strbuf_release(&بفر);
		أرجع error(_("emacsclient version '%d' too old (< 22)."),
			نسخہ);
	}

	strbuf_release(&بفر);
	أرجع 0;
}

ساكن فراغ exec_woman_emacs(ثابت حرف *رستہ, ثابت حرف *ورقہ)
{
	إذا (!check_emacsclient_نسخہ()) {
		/* This works only with emacsclient version >= 22. */
		بنية strbuf man_ورقہ = STRBUF_INIT;

		إذا (!رستہ)
			رستہ = "emacsclient";
		strbuf_addf(&man_ورقہ, "(woman \"%s\")", ورقہ);
		execlp(رستہ, "emacsclient", "-e", man_ورقہ.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), رستہ);
		strbuf_release(&man_ورقہ);
	}
}

ساكن فراغ exec_man_konqueror(ثابت حرف *رستہ, ثابت حرف *ورقہ)
{
	ثابت حرف *display = getenv("DISPLAY");
	إذا (display && *display) {
		بنية strbuf man_ورقہ = STRBUF_INIT;
		ثابت حرف *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		إذا (رستہ) {
			ناپ_t len;
			إذا (strip_suffix(رستہ, "/konqueror", &len))
				رستہ = xstrfmt("%.*s/kfmclient", (صحيح)len, رستہ);
			filename = basename((حرف *)رستہ);
		} وإلا
			رستہ = "kfmclient";
		strbuf_addf(&man_ورقہ, "man:%s(1)", ورقہ);
		execlp(رستہ, filename, "newTab", man_ورقہ.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), رستہ);
		strbuf_release(&man_ورقہ);
	}
}

ساكن فراغ exec_man_man(ثابت حرف *رستہ, ثابت حرف *ورقہ)
{
	إذا (!رستہ)
		رستہ = "man";
	execlp(رستہ, "man", ورقہ, (حرف *)عدم);
	warning_errno(_("failed to exec '%s'"), رستہ);
}

ساكن فراغ exec_man_cmd(ثابت حرف *cmd, ثابت حرف *ورقہ)
{
	بنية strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, ورقہ);
	execl(SHELL_رستہ, SHELL_رستہ, "-c", shell_cmd.buf, (حرف *)عدم);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ساكن فراغ جوڑو_man_viewer(ثابت حرف *name)
{
	بنية man_viewer_list **p = &man_viewer_list;

	طالما (*p)
		p = &((*p)->اگلا);
	FLEX_ALLOC_STR(*p, name, name);
}

ساكن صحيح supported_man_viewer(ثابت حرف *name, ناپ_t len)
{
	أرجع (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ساكن فراغ do_جوڑو_man_viewer_info(ثابت حرف *name,
				   ناپ_t len,
				   ثابت حرف *قدر)
{
	بنية man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(قدر);
	new_man_viewer->اگلا = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ساكن صحيح جوڑو_man_viewer_رستہ(ثابت حرف *name,
			       ناپ_t len,
			       ثابت حرف *قدر)
{
	إذا (supported_man_viewer(name, len))
		do_جوڑو_man_viewer_info(name, len, قدر);
	وإلا
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	أرجع 0;
}

ساكن صحيح جوڑو_man_viewer_cmd(ثابت حرف *name,
			      ناپ_t len,
			      ثابت حرف *قدر)
{
	إذا (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	وإلا
		do_جوڑو_man_viewer_info(name, len, قدر);

	أرجع 0;
}

ساكن صحيح جوڑو_man_viewer_info(ثابت حرف *var, ثابت حرف *قدر)
{
	ثابت حرف *name, *subkey;
	ناپ_t namelen;

	إذا (parse_config_کلید(var, "man", &name, &namelen, &subkey) < 0 || !name)
		أرجع 0;

	إذا (!strcmp(subkey, "path")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		أرجع جوڑو_man_viewer_رستہ(name, namelen, قدر);
	}
	إذا (!strcmp(subkey, "cmd")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		أرجع جوڑو_man_viewer_cmd(name, namelen, قدر);
	}

	أرجع 0;
}

ساكن صحيح git_help_config(ثابت حرف *var, ثابت حرف *قدر,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "help.format")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		help_format = parse_help_format(قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "help.htmlpath")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		حرر(html_رستہ);
		html_رستہ = xstrdup(قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "man.viewer")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		جوڑو_man_viewer(قدر);
		أرجع 0;
	}
	إذا (starts_with(var, "man."))
		أرجع جوڑو_man_viewer_info(var, قدر);

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن بنية cmdnames main_cmds, other_cmds;

ساكن صحيح is_git_command(ثابت حرف *s)
{
	إذا (is_builtin(s))
		أرجع 1;

	لدو_command_list("git-", &main_cmds, &other_cmds);
	أرجع is_وچ_cmdlist(&main_cmds, s) ||
		is_وچ_cmdlist(&other_cmds, s);
}

ساكن ثابت حرف *cmd_منزل_ورقہ(ثابت حرف *git_cmd)
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

ساكن فراغ تیاری_man_رستہ(فراغ)
{
	بنية strbuf new_رستہ = STRBUF_INIT;
	ثابت حرف *old_رستہ = getenv("MANPATH");
	حرف *git_man_رستہ = system_رستہ(GIT_MAN_رستہ);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_رستہ, git_man_رستہ);
	strbuf_addch(&new_رستہ, ':');
	إذا (old_رستہ)
		strbuf_addstr(&new_رستہ, old_رستہ);

	حرر(git_man_رستہ);
	setenv("MANPATH", new_رستہ.buf, 1);

	strbuf_release(&new_رستہ);
}

ساكن فراغ exec_viewer(ثابت حرف *name, ثابت حرف *ورقہ)
{
	ثابت حرف *info = get_man_viewer_info(name);

	إذا (!strcasecmp(name, "man"))
		exec_man_man(info, ورقہ);
	وإلا إذا (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, ورقہ);
	وإلا إذا (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, ورقہ);
	وإلا إذا (info)
		exec_man_cmd(info, ورقہ);
	وإلا
		warning(_("'%s': unknown man viewer."), name);
}

ساكن فراغ show_man_ورقہ(ثابت حرف *ورقہ)
{
	بنية man_viewer_list *viewer;
	ثابت حرف *fallback = getenv("GIT_MAN_VIEWER");

	تیاری_man_رستہ();
	لكل (viewer = man_viewer_list; viewer; viewer = viewer->اگلا)
	{
		exec_viewer(viewer->name, ورقہ); /* will return when unable */
	}
	إذا (fallback)
		exec_viewer(fallback, ورقہ);
	exec_viewer("man", ورقہ);
	die(_("no man viewer handled the request"));
}

ساكن فراغ show_info_ورقہ(ثابت حرف *ورقہ)
{
	setenv("INFOPATH", system_رستہ(GIT_INFO_رستہ), 1);
	execlp("info", "info", "gitman", ورقہ, (حرف *)عدم);
	die(_("no info viewer handled the request"));
}

ساكن فراغ get_html_ورقہ_رستہ(بنية strbuf *ورقہ_رستہ, ثابت حرف *ورقہ)
{
	بنية stat st;
	ثابت حرف *رستہ = html_رستہ;
	حرف *منزل_free = عدم;

	إذا (!رستہ)
		رستہ = منزل_free = system_رستہ(GIT_HTML_رستہ);

	/*
	 * Check that the page we're looking for exists.
	 */
	إذا (!لڑی_کھوج(رستہ, "://")) {
		إذا (stat(mkpath("%s/%s.html", رستہ, ورقہ), &st)
		    || !S_ISREG(st.st_منوال))
			die("'%s/%s.html': documentation file not found.",
				رستہ, ورقہ);
	}

	strbuf_init(ورقہ_رستہ, 0);
	strbuf_addf(ورقہ_رستہ, "%s/%s.html", رستہ, ورقہ);
	حرر(منزل_free);
}

ساكن فراغ open_html(ثابت حرف *رستہ)
{
	execl_git_cmd("web--browse", "-c", "help.browser", رستہ, (حرف *)عدم);
}

ساكن فراغ show_html_ورقہ(ثابت حرف *ورقہ)
{
	بنية strbuf ورقہ_رستہ; /* it leaks but we exec below */

	get_html_ورقہ_رستہ(&ورقہ_رستہ, ورقہ);

	open_html(ورقہ_رستہ.buf);
}

ساكن حرف *check_git_cmd(ثابت حرف *cmd)
{
	حرف *alias;

	إذا (is_git_command(cmd))
		أرجع xstrdup(cmd);

	alias = alias_lookup(cmd);
	إذا (alias) {
		ثابت حرف **argv;
		صحيح گنتی;

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
		گنتی = ونڈو_cmdline(alias, &argv);
		إذا (گنتی < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    ونڈو_cmdline_strerror(گنتی));
		حرر(argv);
		أرجع alias;
	}

	إذا (exclude_guides)
		أرجع help_unknown_cmd(cmd);

	أرجع xstrdup(cmd);
}

ساكن فراغ no_help_format(ثابت حرف *opt_منوال, تعداد help_format fmt)
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

	usage_پیغام_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_منوال,
		       opt_fmt);
}

ساكن فراغ opt_منوال_usage(صحيح argc, ثابت حرف *opt_منوال,
			   تعداد help_format fmt)
{
	إذا (argc)
		usage_پیغام_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_منوال);

	no_help_format(opt_منوال, fmt);
}

صحيح cmd_help(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *اگیتر,
	     بنية repository *repo بن_ورتیا)
{
	صحيح nongit;
	تعداد help_format parsed_help_format;
	حرف *command = عدم;
	ثابت حرف *ورقہ;

	argc = parse_options(argc, argv, اگیتر, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	إذا (cmd_منوال != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_پیغام_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	بدل (cmd_منوال) {
	حالة HELP_ACTION_ALL:
		opt_منوال_usage(argc, "--all", help_format);
		إذا (verbose) {
			تیاری_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			أرجع 0;
		}
		لکھو(_("usage: %s%s"), _(git_usage_string), "\n\n");
		لدو_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		لکھو("%s\n", _(git_ہور_info_string));
		اكسر;
	حالة HELP_ACTION_GUIDES:
		opt_منوال_usage(argc, "--guides", help_format);
		list_guides_help();
		لکھو("%s\n", _(git_ہور_info_string));
		أرجع 0;
	حالة HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_منوال_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		أرجع 0;
	حالة HELP_ACTION_USER_INTERFACES:
		opt_منوال_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_DEVELOPER_INTERFACES:
		opt_منوال_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_منوال_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		أرجع 0;
	حالة HELP_ACTION_ALIASES_FOR_COMPLETION: {
		بنية string_list alias_list = STRING_LIST_INIT_DUP;
		opt_منوال_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		لكل (ناپ_t i = 0; i < alias_list.nr; i++)
			لکھو("%s%c%s%c", alias_list.اشیاء[i].string, '\n',
			       (حرف *)alias_list.اشیاء[i].util, '\0');
		string_list_clear(&alias_list, 1);
		أرجع 0;
	}
	حالة HELP_ACTION_CONFIG:
		opt_منوال_usage(argc, "--config", help_format);
		تیاری_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		لکھو("\n%s\n", _("'git help config' for more information"));
		أرجع 0;
	}

	إذا (!argv[0]) {
		لکھو(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		لکھو("\n%s\n", _(git_ہور_info_string));
		أرجع 0;
	}

	تیاری_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, عدم);

	إذا (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	إذا (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	ورقہ = cmd_منزل_ورقہ(command);
	بدل (help_format) {
	حالة HELP_FORMAT_NONE:
	حالة HELP_FORMAT_MAN:
		show_man_ورقہ(ورقہ);
		اكسر;
	حالة HELP_FORMAT_INFO:
		show_info_ورقہ(ورقہ);
		اكسر;
	حالة HELP_FORMAT_WEB:
		show_html_ورقہ(ورقہ);
		اكسر;
	}

	حرر(command);
	أرجع 0;
}
