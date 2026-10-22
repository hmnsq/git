/*
 * Builtin help command
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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

#جيڪڏهن_وصف_نه_ٿيل DEFAULT_HELP_FORMAT
#وصف_ڪريو DEFAULT_HELP_FORMAT "man"
#جيڪڏهن_پورو

ساكن بنية man_viewer_list {
	بنية man_viewer_list *ايندڙ;
	حرف name[FLEX_ARRAY];
} *man_viewer_list;

ساكن بنية man_viewer_info_list {
	بنية man_viewer_info_list *ايندڙ;
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
} cmd_گهڻائي;

ساكن حرف *html_رستو;
ساكن صحيح verbose = 1;
ساكن تعداد help_format help_format = HELP_FORMAT_NONE;
ساكن صحيح exclude_guides;
ساكن صحيح show_external_commands = -1;
ساكن صحيح show_aliases = -1;
ساكن بنية option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_گهڻائي, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_سيٽ_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_سيٽ_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_سيٽ_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_گهڻائي, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_گهڻائي,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_گهڻائي,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_گهڻائي, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_گهڻائي, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_گهڻائي, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_گهڻائي, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_پڇاڙي(),
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

بنية جڳهه_expansion {
	ثابت حرف *اڳياڙي;
	ثابت حرف *placeholder;
	فراغ (*fn)(بنية string_list *list, ثابت حرف *اڳياڙي);
	صحيح مليو_2;
};

ساكن فراغ سيٽ_config_vars(بنية string_list *ڪنجيون_uniq, بنية string_list_شي *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *wildcard = strchr(str, '*');
	ثابت حرف *ٽيگ = strchr(str, '<');
	ثابت حرف *cut;

	إذا (wildcard && ٽيگ)
		cut = wildcard < ٽيگ ? wildcard : ٽيگ;
	وإلا إذا (wildcard)
		cut = wildcard;
	وإلا إذا (ٽيگ)
		cut = ٽيگ;
	وإلا {
		string_list_append(ڪنجيون_uniq, str);
		أرجع;
	}

	strbuf_جوڙيو(&sb, str, cut - str);
	string_list_append(ڪنجيون_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ سيٽ_config_sections(بنية string_list *ڪنجيون_uniq, بنية string_list_شي *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *dot = strchr(str, '.');
	ثابت حرف *cut;

	إذا (dot)
		cut = dot;
	وإلا {
		سيٽ_config_vars(ڪنجيون_uniq, var);
		أرجع;
	}

	strbuf_جوڙيو(&sb, str, cut - str);
	string_list_append(ڪنجيون_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ list_config_help(تعداد show_config_type type)
{
	بنية جڳهه_expansion جڳهه_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_حالت_slots },
		{ "fsck", "<msg-id>", list_config_fsck_نياپو_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_نياپو_ids },
		{ عدم, عدم, عدم }
	};
	ثابت حرف **p;
	بنية جڳهه_expansion *e;
	بنية string_list ڪنجيون = STRING_LIST_INIT_DUP;
	بنية string_list ڪنجيون_uniq = STRING_LIST_INIT_DUP;
	بنية string_list_شي *شي;
	بنية strbuf sb = STRBUF_INIT;

	لكل (p = config_name_list; *p; p++) {
		ثابت حرف *var = *p;

		لكل (e = جڳهه_expansions; e->اڳياڙي; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->اڳياڙي, e->placeholder);
			إذا (!strcasecmp(var, sb.buf)) {
				e->fn(&ڪنجيون, e->اڳياڙي);
				e->مليو_2++;
				اكسر;
			}
		}

		إذا (!e->اڳياڙي)
			string_list_append(&ڪنجيون, var);
	}

	strbuf_release(&sb);

	لكل (e = جڳهه_expansions; e->اڳياڙي; e++)
		إذا (!e->مليو_2)
			BUG("slot_expansion %s.%s is not used",
			    e->اڳياڙي, e->placeholder);

	لكل (سائيز_t i = 0; i < ڪنجيون.nr; i++) {
		بدل (type) {
		حالة SHOW_CONFIG_HUMAN:
			string_list_append(&ڪنجيون_uniq, ڪنجيون.شيون[i].string);
			اكسر;
		حالة SHOW_CONFIG_SECTIONS:
			سيٽ_config_sections(&ڪنجيون_uniq, &ڪنجيون.شيون[i]);
			اكسر;
		حالة SHOW_CONFIG_VARS:
			سيٽ_config_vars(&ڪنجيون_uniq, &ڪنجيون.شيون[i]);
			اكسر;
		افتراضي:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_ترتيب_ڏيو_u(&ڪنجيون_uniq, 0);
	for_each_string_list_شي(شي, &ڪنجيون_uniq)
		puts(شي->string);
	string_list_clear(&ڪنجيون_uniq, 0);
	string_list_clear(&ڪنجيون, 0);
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

	لكل (viewer = man_viewer_info_list; viewer; viewer = viewer->ايندڙ)
	{
		إذا (!strcasecmp(name, viewer->name))
			أرجع viewer->info;
	}
	أرجع عدم;
}

ساكن صحيح check_emacsclient_نسخو(فراغ)
{
	بنية strbuf بفر = STRBUF_INIT;
	بنية ٻار_process ec_process = ٻار_PROCESS_INIT;
	صحيح نسخو;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.دليلون, "emacsclient", "--version", عدم);
	ec_process.err = -1;
	ec_process.stdout_منزل_stderr = 1;
	إذا (شروعات_command(&ec_process))
		أرجع error(_("Failed to start emacsclient."));

	strbuf_read(&بفر, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	پڄاڻي_command(&ec_process);

	إذا (!starts_with(بفر.buf, "emacsclient")) {
		strbuf_release(&بفر);
		أرجع error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&بفر, 0, لڙي_ڊيگهه("emacsclient"));
	نسخو = atoi(بفر.buf);

	إذا (نسخو < 22) {
		strbuf_release(&بفر);
		أرجع error(_("emacsclient version '%d' too old (< 22)."),
			نسخو);
	}

	strbuf_release(&بفر);
	أرجع 0;
}

ساكن فراغ exec_woman_emacs(ثابت حرف *رستو, ثابت حرف *صفحو)
{
	إذا (!check_emacsclient_نسخو()) {
		/* This works only with emacsclient version >= 22. */
		بنية strbuf man_صفحو = STRBUF_INIT;

		إذا (!رستو)
			رستو = "emacsclient";
		strbuf_addf(&man_صفحو, "(woman \"%s\")", صفحو);
		execlp(رستو, "emacsclient", "-e", man_صفحو.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), رستو);
		strbuf_release(&man_صفحو);
	}
}

ساكن فراغ exec_man_konqueror(ثابت حرف *رستو, ثابت حرف *صفحو)
{
	ثابت حرف *display = getenv("DISPLAY");
	إذا (display && *display) {
		بنية strbuf man_صفحو = STRBUF_INIT;
		ثابت حرف *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		إذا (رستو) {
			سائيز_t len;
			إذا (strip_suffix(رستو, "/konqueror", &len))
				رستو = xstrfmt("%.*s/kfmclient", (صحيح)len, رستو);
			filename = basename((حرف *)رستو);
		} وإلا
			رستو = "kfmclient";
		strbuf_addf(&man_صفحو, "man:%s(1)", صفحو);
		execlp(رستو, filename, "newTab", man_صفحو.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), رستو);
		strbuf_release(&man_صفحو);
	}
}

ساكن فراغ exec_man_man(ثابت حرف *رستو, ثابت حرف *صفحو)
{
	إذا (!رستو)
		رستو = "man";
	execlp(رستو, "man", صفحو, (حرف *)عدم);
	warning_errno(_("failed to exec '%s'"), رستو);
}

ساكن فراغ exec_man_cmd(ثابت حرف *cmd, ثابت حرف *صفحو)
{
	بنية strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, صفحو);
	execl(SHELL_رستو, SHELL_رستو, "-c", shell_cmd.buf, (حرف *)عدم);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ساكن فراغ جوڙيو_man_viewer(ثابت حرف *name)
{
	بنية man_viewer_list **p = &man_viewer_list;

	طالما (*p)
		p = &((*p)->ايندڙ);
	FLEX_ALLOC_STR(*p, name, name);
}

ساكن صحيح supported_man_viewer(ثابت حرف *name, سائيز_t len)
{
	أرجع (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ساكن فراغ do_جوڙيو_man_viewer_info(ثابت حرف *name,
				   سائيز_t len,
				   ثابت حرف *قدر)
{
	بنية man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(قدر);
	new_man_viewer->ايندڙ = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ساكن صحيح جوڙيو_man_viewer_رستو(ثابت حرف *name,
			       سائيز_t len,
			       ثابت حرف *قدر)
{
	إذا (supported_man_viewer(name, len))
		do_جوڙيو_man_viewer_info(name, len, قدر);
	وإلا
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	أرجع 0;
}

ساكن صحيح جوڙيو_man_viewer_cmd(ثابت حرف *name,
			      سائيز_t len,
			      ثابت حرف *قدر)
{
	إذا (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	وإلا
		do_جوڙيو_man_viewer_info(name, len, قدر);

	أرجع 0;
}

ساكن صحيح جوڙيو_man_viewer_info(ثابت حرف *var, ثابت حرف *قدر)
{
	ثابت حرف *name, *subkey;
	سائيز_t namelen;

	إذا (parse_config_ڪنجي(var, "man", &name, &namelen, &subkey) < 0 || !name)
		أرجع 0;

	إذا (!strcmp(subkey, "path")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		أرجع جوڙيو_man_viewer_رستو(name, namelen, قدر);
	}
	إذا (!strcmp(subkey, "cmd")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		أرجع جوڙيو_man_viewer_cmd(name, namelen, قدر);
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
		حرر(html_رستو);
		html_رستو = xstrdup(قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "man.viewer")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		جوڙيو_man_viewer(قدر);
		أرجع 0;
	}
	إذا (starts_with(var, "man."))
		أرجع جوڙيو_man_viewer_info(var, قدر);

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن بنية cmdnames main_cmds, other_cmds;

ساكن صحيح is_git_command(ثابت حرف *s)
{
	إذا (is_builtin(s))
		أرجع 1;

	ڀريو_command_list("git-", &main_cmds, &other_cmds);
	أرجع is_اندريون_cmdlist(&main_cmds, s) ||
		is_اندريون_cmdlist(&other_cmds, s);
}

ساكن ثابت حرف *cmd_منزل_صفحو(ثابت حرف *git_cmd)
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

ساكن فراغ تياري_man_رستو(فراغ)
{
	بنية strbuf new_رستو = STRBUF_INIT;
	ثابت حرف *old_رستو = getenv("MANPATH");
	حرف *git_man_رستو = system_رستو(GIT_MAN_رستو);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_رستو, git_man_رستو);
	strbuf_addch(&new_رستو, ':');
	إذا (old_رستو)
		strbuf_addstr(&new_رستو, old_رستو);

	حرر(git_man_رستو);
	setenv("MANPATH", new_رستو.buf, 1);

	strbuf_release(&new_رستو);
}

ساكن فراغ exec_viewer(ثابت حرف *name, ثابت حرف *صفحو)
{
	ثابت حرف *info = get_man_viewer_info(name);

	إذا (!strcasecmp(name, "man"))
		exec_man_man(info, صفحو);
	وإلا إذا (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, صفحو);
	وإلا إذا (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, صفحو);
	وإلا إذا (info)
		exec_man_cmd(info, صفحو);
	وإلا
		warning(_("'%s': unknown man viewer."), name);
}

ساكن فراغ show_man_صفحو(ثابت حرف *صفحو)
{
	بنية man_viewer_list *viewer;
	ثابت حرف *fallback = getenv("GIT_MAN_VIEWER");

	تياري_man_رستو();
	لكل (viewer = man_viewer_list; viewer; viewer = viewer->ايندڙ)
	{
		exec_viewer(viewer->name, صفحو); /* will return when unable */
	}
	إذا (fallback)
		exec_viewer(fallback, صفحو);
	exec_viewer("man", صفحو);
	die(_("no man viewer handled the request"));
}

ساكن فراغ show_info_صفحو(ثابت حرف *صفحو)
{
	setenv("INFOPATH", system_رستو(GIT_INFO_رستو), 1);
	execlp("info", "info", "gitman", صفحو, (حرف *)عدم);
	die(_("no info viewer handled the request"));
}

ساكن فراغ get_html_صفحو_رستو(بنية strbuf *صفحو_رستو, ثابت حرف *صفحو)
{
	بنية stat st;
	ثابت حرف *رستو = html_رستو;
	حرف *منزل_free = عدم;

	إذا (!رستو)
		رستو = منزل_free = system_رستو(GIT_HTML_رستو);

	/*
	 * Check that the page we're looking for exists.
	 */
	إذا (!لڙي_ڳولا(رستو, "://")) {
		إذا (stat(mkpath("%s/%s.html", رستو, صفحو), &st)
		    || !S_ISREG(st.st_گهڻائي))
			die("'%s/%s.html': documentation file not found.",
				رستو, صفحو);
	}

	strbuf_init(صفحو_رستو, 0);
	strbuf_addf(صفحو_رستو, "%s/%s.html", رستو, صفحو);
	حرر(منزل_free);
}

ساكن فراغ open_html(ثابت حرف *رستو)
{
	execl_git_cmd("web--browse", "-c", "help.browser", رستو, (حرف *)عدم);
}

ساكن فراغ show_html_صفحو(ثابت حرف *صفحو)
{
	بنية strbuf صفحو_رستو; /* it leaks but we exec below */

	get_html_صفحو_رستو(&صفحو_رستو, صفحو);

	open_html(صفحو_رستو.buf);
}

ساكن حرف *check_git_cmd(ثابت حرف *cmd)
{
	حرف *alias;

	إذا (is_git_command(cmd))
		أرجع xstrdup(cmd);

	alias = alias_lookup(cmd);
	إذا (alias) {
		ثابت حرف **argv;
		صحيح ڳڻپ;

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
			ٻاهر_نڪرو(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		ڳڻپ = ورهايو_cmdline(alias, &argv);
		إذا (ڳڻپ < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    ورهايو_cmdline_strerror(ڳڻپ));
		حرر(argv);
		أرجع alias;
	}

	إذا (exclude_guides)
		أرجع help_unknown_cmd(cmd);

	أرجع xstrdup(cmd);
}

ساكن فراغ no_help_format(ثابت حرف *opt_گهڻائي, تعداد help_format fmt)
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

	usage_نياپو_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_گهڻائي,
		       opt_fmt);
}

ساكن فراغ opt_گهڻائي_usage(صحيح argc, ثابت حرف *opt_گهڻائي,
			   تعداد help_format fmt)
{
	إذا (argc)
		usage_نياپو_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_گهڻائي);

	no_help_format(opt_گهڻائي, fmt);
}

صحيح cmd_help(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *اڳياڙي,
	     بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح nongit;
	تعداد help_format parsed_help_format;
	حرف *command = عدم;
	ثابت حرف *صفحو;

	argc = parse_options(argc, argv, اڳياڙي, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	إذا (cmd_گهڻائي != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_نياپو_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	بدل (cmd_گهڻائي) {
	حالة HELP_ACTION_ALL:
		opt_گهڻائي_usage(argc, "--all", help_format);
		إذا (verbose) {
			تياري_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			أرجع 0;
		}
		لکو(_("usage: %s%s"), _(git_usage_string), "\n\n");
		ڀريو_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		لکو("%s\n", _(git_وڌيڪ_info_string));
		اكسر;
	حالة HELP_ACTION_GUIDES:
		opt_گهڻائي_usage(argc, "--guides", help_format);
		list_guides_help();
		لکو("%s\n", _(git_وڌيڪ_info_string));
		أرجع 0;
	حالة HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_گهڻائي_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		أرجع 0;
	حالة HELP_ACTION_USER_INTERFACES:
		opt_گهڻائي_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_DEVELOPER_INTERFACES:
		opt_گهڻائي_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_گهڻائي_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		أرجع 0;
	حالة HELP_ACTION_ALIASES_FOR_COMPLETION: {
		بنية string_list alias_list = STRING_LIST_INIT_DUP;
		opt_گهڻائي_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		لكل (سائيز_t i = 0; i < alias_list.nr; i++)
			لکو("%s%c%s%c", alias_list.شيون[i].string, '\n',
			       (حرف *)alias_list.شيون[i].util, '\0');
		string_list_clear(&alias_list, 1);
		أرجع 0;
	}
	حالة HELP_ACTION_CONFIG:
		opt_گهڻائي_usage(argc, "--config", help_format);
		تياري_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		لکو("\n%s\n", _("'git help config' for more information"));
		أرجع 0;
	}

	إذا (!argv[0]) {
		لکو(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		لکو("\n%s\n", _(git_وڌيڪ_info_string));
		أرجع 0;
	}

	تياري_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, عدم);

	إذا (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	إذا (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	صفحو = cmd_منزل_صفحو(command);
	بدل (help_format) {
	حالة HELP_FORMAT_NONE:
	حالة HELP_FORMAT_MAN:
		show_man_صفحو(صفحو);
		اكسر;
	حالة HELP_FORMAT_INFO:
		show_info_صفحو(صفحو);
		اكسر;
	حالة HELP_FORMAT_WEB:
		show_html_صفحو(صفحو);
		اكسر;
	}

	حرر(command);
	أرجع 0;
}
