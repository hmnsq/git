/*
 * Builtin help command
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

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

#که_نه_تعریف_شوی DEFAULT_HELP_FORMAT
#تعریف_کړه DEFAULT_HELP_FORMAT "man"
#د_که_پای

ساكن بنية man_viewer_list {
	بنية man_viewer_list *بل;
	حرف name[FLEX_ARRAY];
} *man_viewer_list;

ساكن بنية man_viewer_info_list {
	بنية man_viewer_info_list *بل;
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
} cmd_ډېرپېښ;

ساكن حرف *html_لار;
ساكن صحيح verbose = 1;
ساكن تعداد help_format help_format = HELP_FORMAT_NONE;
ساكن صحيح exclude_guides;
ساكن صحيح show_external_commands = -1;
ساكن صحيح show_aliases = -1;
ساكن بنية option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_ډېرپېښ, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_سېټ_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_سېټ_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_سېټ_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_ډېرپېښ, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_ډېرپېښ,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_ډېرپېښ,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_ډېرپېښ, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_ډېرپېښ, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_ډېرپېښ, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_ډېرپېښ, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_پای_ټکی(),
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

بنية خونه_expansion {
	ثابت حرف *مختاړی;
	ثابت حرف *placeholder;
	فراغ (*fn)(بنية string_list *list, ثابت حرف *مختاړی);
	صحيح وموندل_2;
};

ساكن فراغ سېټ_config_vars(بنية string_list *کلیګانې_uniq, بنية string_list_توکی *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *wildcard = strchr(str, '*');
	ثابت حرف *نښان = strchr(str, '<');
	ثابت حرف *cut;

	إذا (wildcard && نښان)
		cut = wildcard < نښان ? wildcard : نښان;
	وإلا إذا (wildcard)
		cut = wildcard;
	وإلا إذا (نښان)
		cut = نښان;
	وإلا {
		string_list_append(کلیګانې_uniq, str);
		أرجع;
	}

	strbuf_جمع_کړه(&sb, str, cut - str);
	string_list_append(کلیګانې_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ سېټ_config_sections(بنية string_list *کلیګانې_uniq, بنية string_list_توکی *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *dot = strchr(str, '.');
	ثابت حرف *cut;

	إذا (dot)
		cut = dot;
	وإلا {
		سېټ_config_vars(کلیګانې_uniq, var);
		أرجع;
	}

	strbuf_جمع_کړه(&sb, str, cut - str);
	string_list_append(کلیګانې_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ list_config_help(تعداد show_config_type type)
{
	بنية خونه_expansion خونه_expansions[] = {
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
	بنية خونه_expansion *e;
	بنية string_list کلیګانې = STRING_LIST_INIT_DUP;
	بنية string_list کلیګانې_uniq = STRING_LIST_INIT_DUP;
	بنية string_list_توکی *توکی;
	بنية strbuf sb = STRBUF_INIT;

	لكل (p = config_name_list; *p; p++) {
		ثابت حرف *var = *p;

		لكل (e = خونه_expansions; e->مختاړی; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->مختاړی, e->placeholder);
			إذا (!strcasecmp(var, sb.buf)) {
				e->fn(&کلیګانې, e->مختاړی);
				e->وموندل_2++;
				اكسر;
			}
		}

		إذا (!e->مختاړی)
			string_list_append(&کلیګانې, var);
	}

	strbuf_release(&sb);

	لكل (e = خونه_expansions; e->مختاړی; e++)
		إذا (!e->وموندل_2)
			BUG("slot_expansion %s.%s is not used",
			    e->مختاړی, e->placeholder);

	لكل (اندازه_t i = 0; i < کلیګانې.nr; i++) {
		بدل (type) {
		حالة SHOW_CONFIG_HUMAN:
			string_list_append(&کلیګانې_uniq, کلیګانې.توکي[i].string);
			اكسر;
		حالة SHOW_CONFIG_SECTIONS:
			سېټ_config_sections(&کلیګانې_uniq, &کلیګانې.توکي[i]);
			اكسر;
		حالة SHOW_CONFIG_VARS:
			سېټ_config_vars(&کلیګانې_uniq, &کلیګانې.توکي[i]);
			اكسر;
		افتراضي:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_ترتیبول_u(&کلیګانې_uniq, 0);
	for_each_string_list_توکی(توکی, &کلیګانې_uniq)
		puts(توکی->string);
	string_list_clear(&کلیګانې_uniq, 0);
	string_list_clear(&کلیګانې, 0);
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

	لكل (viewer = man_viewer_info_list; viewer; viewer = viewer->بل)
	{
		إذا (!strcasecmp(name, viewer->name))
			أرجع viewer->info;
	}
	أرجع عدم;
}

ساكن صحيح check_emacsclient_نسخه(فراغ)
{
	بنية strbuf بفر = STRBUF_INIT;
	بنية ماشوم_process ec_process = ماشوم_PROCESS_INIT;
	صحيح نسخه;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.دلیلونه, "emacsclient", "--version", عدم);
	ec_process.err = -1;
	ec_process.stdout_منزل_stderr = 1;
	إذا (پیل_command(&ec_process))
		أرجع error(_("Failed to start emacsclient."));

	strbuf_read(&بفر, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	پای_command(&ec_process);

	إذا (!starts_with(بفر.buf, "emacsclient")) {
		strbuf_release(&بفر);
		أرجع error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&بفر, 0, مزي_اوږدوالی("emacsclient"));
	نسخه = atoi(بفر.buf);

	إذا (نسخه < 22) {
		strbuf_release(&بفر);
		أرجع error(_("emacsclient version '%d' too old (< 22)."),
			نسخه);
	}

	strbuf_release(&بفر);
	أرجع 0;
}

ساكن فراغ exec_woman_emacs(ثابت حرف *لار, ثابت حرف *پاڼه)
{
	إذا (!check_emacsclient_نسخه()) {
		/* This works only with emacsclient version >= 22. */
		بنية strbuf man_پاڼه = STRBUF_INIT;

		إذا (!لار)
			لار = "emacsclient";
		strbuf_addf(&man_پاڼه, "(woman \"%s\")", پاڼه);
		execlp(لار, "emacsclient", "-e", man_پاڼه.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), لار);
		strbuf_release(&man_پاڼه);
	}
}

ساكن فراغ exec_man_konqueror(ثابت حرف *لار, ثابت حرف *پاڼه)
{
	ثابت حرف *display = getenv("DISPLAY");
	إذا (display && *display) {
		بنية strbuf man_پاڼه = STRBUF_INIT;
		ثابت حرف *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		إذا (لار) {
			اندازه_t len;
			إذا (strip_suffix(لار, "/konqueror", &len))
				لار = xstrfmt("%.*s/kfmclient", (صحيح)len, لار);
			filename = basename((حرف *)لار);
		} وإلا
			لار = "kfmclient";
		strbuf_addf(&man_پاڼه, "man:%s(1)", پاڼه);
		execlp(لار, filename, "newTab", man_پاڼه.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), لار);
		strbuf_release(&man_پاڼه);
	}
}

ساكن فراغ exec_man_man(ثابت حرف *لار, ثابت حرف *پاڼه)
{
	إذا (!لار)
		لار = "man";
	execlp(لار, "man", پاڼه, (حرف *)عدم);
	warning_errno(_("failed to exec '%s'"), لار);
}

ساكن فراغ exec_man_cmd(ثابت حرف *cmd, ثابت حرف *پاڼه)
{
	بنية strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, پاڼه);
	execl(SHELL_لار, SHELL_لار, "-c", shell_cmd.buf, (حرف *)عدم);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ساكن فراغ جمع_کړه_man_viewer(ثابت حرف *name)
{
	بنية man_viewer_list **p = &man_viewer_list;

	طالما (*p)
		p = &((*p)->بل);
	FLEX_ALLOC_STR(*p, name, name);
}

ساكن صحيح supported_man_viewer(ثابت حرف *name, اندازه_t len)
{
	أرجع (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ساكن فراغ do_جمع_کړه_man_viewer_info(ثابت حرف *name,
				   اندازه_t len,
				   ثابت حرف *ارزښت)
{
	بنية man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(ارزښت);
	new_man_viewer->بل = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ساكن صحيح جمع_کړه_man_viewer_لار(ثابت حرف *name,
			       اندازه_t len,
			       ثابت حرف *ارزښت)
{
	إذا (supported_man_viewer(name, len))
		do_جمع_کړه_man_viewer_info(name, len, ارزښت);
	وإلا
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	أرجع 0;
}

ساكن صحيح جمع_کړه_man_viewer_cmd(ثابت حرف *name,
			      اندازه_t len,
			      ثابت حرف *ارزښت)
{
	إذا (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	وإلا
		do_جمع_کړه_man_viewer_info(name, len, ارزښت);

	أرجع 0;
}

ساكن صحيح جمع_کړه_man_viewer_info(ثابت حرف *var, ثابت حرف *ارزښت)
{
	ثابت حرف *name, *subkey;
	اندازه_t namelen;

	إذا (parse_config_کلی(var, "man", &name, &namelen, &subkey) < 0 || !name)
		أرجع 0;

	إذا (!strcmp(subkey, "path")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		أرجع جمع_کړه_man_viewer_لار(name, namelen, ارزښت);
	}
	إذا (!strcmp(subkey, "cmd")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		أرجع جمع_کړه_man_viewer_cmd(name, namelen, ارزښت);
	}

	أرجع 0;
}

ساكن صحيح git_help_config(ثابت حرف *var, ثابت حرف *ارزښت,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "help.format")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		help_format = parse_help_format(ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "help.htmlpath")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		حرر(html_لار);
		html_لار = xstrdup(ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "man.viewer")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		جمع_کړه_man_viewer(ارزښت);
		أرجع 0;
	}
	إذا (starts_with(var, "man."))
		أرجع جمع_کړه_man_viewer_info(var, ارزښت);

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

ساكن بنية cmdnames main_cmds, other_cmds;

ساكن صحيح is_git_command(ثابت حرف *s)
{
	إذا (is_builtin(s))
		أرجع 1;

	بارول_command_list("git-", &main_cmds, &other_cmds);
	أرجع is_دننه_cmdlist(&main_cmds, s) ||
		is_دننه_cmdlist(&other_cmds, s);
}

ساكن ثابت حرف *cmd_منزل_پاڼه(ثابت حرف *git_cmd)
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

ساكن فراغ تیاری_man_لار(فراغ)
{
	بنية strbuf new_لار = STRBUF_INIT;
	ثابت حرف *old_لار = getenv("MANPATH");
	حرف *git_man_لار = system_لار(GIT_MAN_لار);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_لار, git_man_لار);
	strbuf_addch(&new_لار, ':');
	إذا (old_لار)
		strbuf_addstr(&new_لار, old_لار);

	حرر(git_man_لار);
	setenv("MANPATH", new_لار.buf, 1);

	strbuf_release(&new_لار);
}

ساكن فراغ exec_viewer(ثابت حرف *name, ثابت حرف *پاڼه)
{
	ثابت حرف *info = get_man_viewer_info(name);

	إذا (!strcasecmp(name, "man"))
		exec_man_man(info, پاڼه);
	وإلا إذا (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, پاڼه);
	وإلا إذا (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, پاڼه);
	وإلا إذا (info)
		exec_man_cmd(info, پاڼه);
	وإلا
		warning(_("'%s': unknown man viewer."), name);
}

ساكن فراغ show_man_پاڼه(ثابت حرف *پاڼه)
{
	بنية man_viewer_list *viewer;
	ثابت حرف *fallback = getenv("GIT_MAN_VIEWER");

	تیاری_man_لار();
	لكل (viewer = man_viewer_list; viewer; viewer = viewer->بل)
	{
		exec_viewer(viewer->name, پاڼه); /* will return when unable */
	}
	إذا (fallback)
		exec_viewer(fallback, پاڼه);
	exec_viewer("man", پاڼه);
	die(_("no man viewer handled the request"));
}

ساكن فراغ show_info_پاڼه(ثابت حرف *پاڼه)
{
	setenv("INFOPATH", system_لار(GIT_INFO_لار), 1);
	execlp("info", "info", "gitman", پاڼه, (حرف *)عدم);
	die(_("no info viewer handled the request"));
}

ساكن فراغ get_html_پاڼه_لار(بنية strbuf *پاڼه_لار, ثابت حرف *پاڼه)
{
	بنية stat st;
	ثابت حرف *لار = html_لار;
	حرف *منزل_free = عدم;

	إذا (!لار)
		لار = منزل_free = system_لار(GIT_HTML_لار);

	/*
	 * Check that the page we're looking for exists.
	 */
	إذا (!مزي_پلټنه(لار, "://")) {
		إذا (stat(mkpath("%s/%s.html", لار, پاڼه), &st)
		    || !S_ISREG(st.st_ډېرپېښ))
			die("'%s/%s.html': documentation file not found.",
				لار, پاڼه);
	}

	strbuf_init(پاڼه_لار, 0);
	strbuf_addf(پاڼه_لار, "%s/%s.html", لار, پاڼه);
	حرر(منزل_free);
}

ساكن فراغ open_html(ثابت حرف *لار)
{
	execl_git_cmd("web--browse", "-c", "help.browser", لار, (حرف *)عدم);
}

ساكن فراغ show_html_پاڼه(ثابت حرف *پاڼه)
{
	بنية strbuf پاڼه_لار; /* it leaks but we exec below */

	get_html_پاڼه_لار(&پاڼه_لار, پاڼه);

	open_html(پاڼه_لار.buf);
}

ساكن حرف *check_git_cmd(ثابت حرف *cmd)
{
	حرف *alias;

	إذا (is_git_command(cmd))
		أرجع xstrdup(cmd);

	alias = alias_lookup(cmd);
	إذا (alias) {
		ثابت حرف **argv;
		صحيح شمېر;

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
			وتل(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		شمېر = وېشل_cmdline(alias, &argv);
		إذا (شمېر < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    وېشل_cmdline_strerror(شمېر));
		حرر(argv);
		أرجع alias;
	}

	إذا (exclude_guides)
		أرجع help_unknown_cmd(cmd);

	أرجع xstrdup(cmd);
}

ساكن فراغ no_help_format(ثابت حرف *opt_ډېرپېښ, تعداد help_format fmt)
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
		       builtin_help_usage, builtin_help_options, opt_ډېرپېښ,
		       opt_fmt);
}

ساكن فراغ opt_ډېرپېښ_usage(صحيح argc, ثابت حرف *opt_ډېرپېښ,
			   تعداد help_format fmt)
{
	إذا (argc)
		usage_پیغام_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_ډېرپېښ);

	no_help_format(opt_ډېرپېښ, fmt);
}

صحيح cmd_help(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *مختاړی,
	     بنية repository *repo ناکارول_شوی)
{
	صحيح nongit;
	تعداد help_format parsed_help_format;
	حرف *command = عدم;
	ثابت حرف *پاڼه;

	argc = parse_options(argc, argv, مختاړی, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	إذا (cmd_ډېرپېښ != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_پیغام_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	بدل (cmd_ډېرپېښ) {
	حالة HELP_ACTION_ALL:
		opt_ډېرپېښ_usage(argc, "--all", help_format);
		إذا (verbose) {
			تیاری_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			أرجع 0;
		}
		چاپ_کړه(_("usage: %s%s"), _(git_usage_string), "\n\n");
		بارول_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		چاپ_کړه("%s\n", _(git_نور_info_string));
		اكسر;
	حالة HELP_ACTION_GUIDES:
		opt_ډېرپېښ_usage(argc, "--guides", help_format);
		list_guides_help();
		چاپ_کړه("%s\n", _(git_نور_info_string));
		أرجع 0;
	حالة HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_ډېرپېښ_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		أرجع 0;
	حالة HELP_ACTION_USER_INTERFACES:
		opt_ډېرپېښ_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_DEVELOPER_INTERFACES:
		opt_ډېرپېښ_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_ډېرپېښ_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		أرجع 0;
	حالة HELP_ACTION_ALIASES_FOR_COMPLETION: {
		بنية string_list alias_list = STRING_LIST_INIT_DUP;
		opt_ډېرپېښ_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		لكل (اندازه_t i = 0; i < alias_list.nr; i++)
			چاپ_کړه("%s%c%s%c", alias_list.توکي[i].string, '\n',
			       (حرف *)alias_list.توکي[i].util, '\0');
		string_list_clear(&alias_list, 1);
		أرجع 0;
	}
	حالة HELP_ACTION_CONFIG:
		opt_ډېرپېښ_usage(argc, "--config", help_format);
		تیاری_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		چاپ_کړه("\n%s\n", _("'git help config' for more information"));
		أرجع 0;
	}

	إذا (!argv[0]) {
		چاپ_کړه(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		چاپ_کړه("\n%s\n", _(git_نور_info_string));
		أرجع 0;
	}

	تیاری_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, عدم);

	إذا (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	إذا (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	پاڼه = cmd_منزل_پاڼه(command);
	بدل (help_format) {
	حالة HELP_FORMAT_NONE:
	حالة HELP_FORMAT_MAN:
		show_man_پاڼه(پاڼه);
		اكسر;
	حالة HELP_FORMAT_INFO:
		show_info_پاڼه(پاڼه);
		اكسر;
	حالة HELP_FORMAT_WEB:
		show_html_پاڼه(پاڼه);
		اكسر;
	}

	حرر(command);
	أرجع 0;
}
