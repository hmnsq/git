/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "exec-cmd.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "pager.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "config-list.h"
#অন্তর্ভাব "help.h"
#অন্তর্ভাব "alias.h"
#অন্তর্ভাব "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

স্থির কাঠামো man_viewer_list {
	কাঠামো man_viewer_list *পরবর্তী;
	অক্ষর name[FLEX_ARRAY];
} *man_viewer_list;

স্থির কাঠামো man_viewer_info_list {
	কাঠামো man_viewer_info_list *পরবর্তী;
	ধ্রুবক অক্ষর *info;
	অক্ষর name[FLEX_ARRAY];
} *man_viewer_info_list;

গণনা help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

গণনা show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

স্থির গণনা help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

স্থির অক্ষর *html_path;
স্থির পূর্ণসংখ্যা verbose = 1;
স্থির গণনা help_format help_format = HELP_FORMAT_NONE;
স্থির পূর্ণসংখ্যা exclude_guides;
স্থির পূর্ণসংখ্যা show_external_commands = -1;
স্থির পূর্ণসংখ্যা show_aliases = -1;
স্থির কাঠামো option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_mode, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_SET_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_SET_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_SET_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_mode, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_mode,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_mode,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_mode, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_mode, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_mode, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_mode, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_END(),
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ফাঁকা
};

কাঠামো slot_expansion {
	ধ্রুবক অক্ষর *prefix;
	ধ্রুবক অক্ষর *placeholder;
	শূন্য (*fn)(কাঠামো string_list *list, ধ্রুবক অক্ষর *prefix);
	পূর্ণসংখ্যা found;
};

স্থির শূন্য set_config_vars(কাঠামো string_list *চাবিগুলি_uniq, কাঠামো string_list_item *var)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *str = var->string;
	ধ্রুবক অক্ষর *wildcard = strchr(str, '*');
	ধ্রুবক অক্ষর *tag = strchr(str, '<');
	ধ্রুবক অক্ষর *cut;

	যদি (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	অন্যথা যদি (wildcard)
		cut = wildcard;
	অন্যথা যদি (tag)
		cut = tag;
	অন্যথা {
		string_list_append(চাবিগুলি_uniq, str);
		ফেরত;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(চাবিগুলি_uniq, sb.buf);
	strbuf_release(&sb);
}

স্থির শূন্য set_config_sections(কাঠামো string_list *চাবিগুলি_uniq, কাঠামো string_list_item *var)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *str = var->string;
	ধ্রুবক অক্ষর *dot = strchr(str, '.');
	ধ্রুবক অক্ষর *cut;

	যদি (dot)
		cut = dot;
	অন্যথা {
		set_config_vars(চাবিগুলি_uniq, var);
		ফেরত;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(চাবিগুলি_uniq, sb.buf);
	strbuf_release(&sb);
}

স্থির শূন্য list_config_help(গণনা show_config_type type)
{
	কাঠামো slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_বার্তা_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_বার্তা_ids },
		{ ফাঁকা, ফাঁকা, ফাঁকা }
	};
	ধ্রুবক অক্ষর **p;
	কাঠামো slot_expansion *e;
	কাঠামো string_list চাবিগুলি = STRING_LIST_INIT_DUP;
	কাঠামো string_list চাবিগুলি_uniq = STRING_LIST_INIT_DUP;
	কাঠামো string_list_item *item;
	কাঠামো strbuf sb = STRBUF_INIT;

	জন্য (p = config_name_list; *p; p++) {
		ধ্রুবক অক্ষর *var = *p;

		জন্য (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			যদি (!strcasecmp(var, sb.buf)) {
				e->fn(&চাবিগুলি, e->prefix);
				e->found++;
				বিরতি;
			}
		}

		যদি (!e->prefix)
			string_list_append(&চাবিগুলি, var);
	}

	strbuf_release(&sb);

	জন্য (e = slot_expansions; e->prefix; e++)
		যদি (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	জন্য (আকার_t i = 0; i < চাবিগুলি.nr; i++) {
		নির্বাচন (type) {
		ক্ষেত্র SHOW_CONFIG_HUMAN:
			string_list_append(&চাবিগুলি_uniq, চাবিগুলি.বস্তুসমূহ[i].string);
			বিরতি;
		ক্ষেত্র SHOW_CONFIG_SECTIONS:
			set_config_sections(&চাবিগুলি_uniq, &চাবিগুলি.বস্তুসমূহ[i]);
			বিরতি;
		ক্ষেত্র SHOW_CONFIG_VARS:
			set_config_vars(&চাবিগুলি_uniq, &চাবিগুলি.বস্তুসমূহ[i]);
			বিরতি;
		পূর্বনির্ধারিত:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&চাবিগুলি_uniq, 0);
	for_each_string_list_item(item, &চাবিগুলি_uniq)
		puts(item->string);
	string_list_clear(&চাবিগুলি_uniq, 0);
	string_list_clear(&চাবিগুলি, 0);
}

স্থির গণনা help_format parse_help_format(ধ্রুবক অক্ষর *format)
{
	যদি (!strcmp(format, "man"))
		ফেরত HELP_FORMAT_MAN;
	যদি (!strcmp(format, "info"))
		ফেরত HELP_FORMAT_INFO;
	যদি (!strcmp(format, "web") || !strcmp(format, "html"))
		ফেরত HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

স্থির ধ্রুবক অক্ষর *get_man_viewer_info(ধ্রুবক অক্ষর *name)
{
	কাঠামো man_viewer_info_list *viewer;

	জন্য (viewer = man_viewer_info_list; viewer; viewer = viewer->পরবর্তী)
	{
		যদি (!strcasecmp(name, viewer->name))
			ফেরত viewer->info;
	}
	ফেরত ফাঁকা;
}

স্থির পূর্ণসংখ্যা check_emacsclient_version(শূন্য)
{
	কাঠামো strbuf বাফার = STRBUF_INIT;
	কাঠামো child_process ec_process = CHILD_PROCESS_INIT;
	পূর্ণসংখ্যা version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", ফাঁকা);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	যদি (start_command(&ec_process))
		ফেরত error(_("Failed to start emacsclient."));

	strbuf_read(&বাফার, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	যদি (!starts_with(বাফার.buf, "emacsclient")) {
		strbuf_release(&বাফার);
		ফেরত error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&বাফার, 0, strlen("emacsclient"));
	version = atoi(বাফার.buf);

	যদি (version < 22) {
		strbuf_release(&বাফার);
		ফেরত error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&বাফার);
	ফেরত 0;
}

স্থির শূন্য exec_woman_emacs(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *page)
{
	যদি (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		কাঠামো strbuf man_page = STRBUF_INIT;

		যদি (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (অক্ষর *)ফাঁকা);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

স্থির শূন্য exec_man_konqueror(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *page)
{
	ধ্রুবক অক্ষর *display = getenv("DISPLAY");
	যদি (display && *display) {
		কাঠামো strbuf man_page = STRBUF_INIT;
		ধ্রুবক অক্ষর *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		যদি (path) {
			আকার_t len;
			যদি (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (পূর্ণসংখ্যা)len, path);
			filename = basename((অক্ষর *)path);
		} অন্যথা
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (অক্ষর *)ফাঁকা);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

স্থির শূন্য exec_man_man(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *page)
{
	যদি (!path)
		path = "man";
	execlp(path, "man", page, (অক্ষর *)ফাঁকা);
	warning_errno(_("failed to exec '%s'"), path);
}

স্থির শূন্য exec_man_cmd(ধ্রুবক অক্ষর *cmd, ধ্রুবক অক্ষর *page)
{
	কাঠামো strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (অক্ষর *)ফাঁকা);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

স্থির শূন্য add_man_viewer(ধ্রুবক অক্ষর *name)
{
	কাঠামো man_viewer_list **p = &man_viewer_list;

	যখন (*p)
		p = &((*p)->পরবর্তী);
	FLEX_ALLOC_STR(*p, name, name);
}

স্থির পূর্ণসংখ্যা supported_man_viewer(ধ্রুবক অক্ষর *name, আকার_t len)
{
	ফেরত (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

স্থির শূন্য do_add_man_viewer_info(ধ্রুবক অক্ষর *name,
				   আকার_t len,
				   ধ্রুবক অক্ষর *মান)
{
	কাঠামো man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(মান);
	new_man_viewer->পরবর্তী = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

স্থির পূর্ণসংখ্যা add_man_viewer_path(ধ্রুবক অক্ষর *name,
			       আকার_t len,
			       ধ্রুবক অক্ষর *মান)
{
	যদি (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, মান);
	অন্যথা
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা add_man_viewer_cmd(ধ্রুবক অক্ষর *name,
			      আকার_t len,
			      ধ্রুবক অক্ষর *মান)
{
	যদি (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	অন্যথা
		do_add_man_viewer_info(name, len, মান);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা add_man_viewer_info(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান)
{
	ধ্রুবক অক্ষর *name, *subkey;
	আকার_t namelen;

	যদি (parse_config_চাবি(var, "man", &name, &namelen, &subkey) < 0 || !name)
		ফেরত 0;

	যদি (!strcmp(subkey, "path")) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		ফেরত add_man_viewer_path(name, namelen, মান);
	}
	যদি (!strcmp(subkey, "cmd")) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		ফেরত add_man_viewer_cmd(name, namelen, মান);
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা git_help_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			   ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "help.format")) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		help_format = parse_help_format(মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "help.htmlpath")) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		মুক্ত(html_path);
		html_path = xstrdup(মান);
		ফেরত 0;
	}
	যদি (!strcmp(var, "man.viewer")) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		add_man_viewer(মান);
		ফেরত 0;
	}
	যদি (starts_with(var, "man."))
		ফেরত add_man_viewer_info(var, মান);

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির কাঠামো cmdnames main_cmds, other_cmds;

স্থির পূর্ণসংখ্যা is_git_command(ধ্রুবক অক্ষর *s)
{
	যদি (is_builtin(s))
		ফেরত 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	ফেরত is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

স্থির ধ্রুবক অক্ষর *cmd_to_page(ধ্রুবক অক্ষর *git_cmd)
{
	যদি (!git_cmd)
		ফেরত "git";
	অন্যথা যদি (starts_with(git_cmd, "git"))
		ফেরত git_cmd;
	অন্যথা যদি (is_git_command(git_cmd))
		ফেরত xstrfmt("git-%s", git_cmd);
	অন্যথা যদি (!strcmp("scalar", git_cmd))
		ফেরত xstrdup(git_cmd);
	অন্যথা
		ফেরত xstrfmt("git%s", git_cmd);
}

স্থির শূন্য setup_man_path(শূন্য)
{
	কাঠামো strbuf new_path = STRBUF_INIT;
	ধ্রুবক অক্ষর *old_path = getenv("MANPATH");
	অক্ষর *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	যদি (old_path)
		strbuf_addstr(&new_path, old_path);

	মুক্ত(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

স্থির শূন্য exec_viewer(ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *page)
{
	ধ্রুবক অক্ষর *info = get_man_viewer_info(name);

	যদি (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	অন্যথা যদি (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	অন্যথা যদি (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	অন্যথা যদি (info)
		exec_man_cmd(info, page);
	অন্যথা
		warning(_("'%s': unknown man viewer."), name);
}

স্থির শূন্য show_man_page(ধ্রুবক অক্ষর *page)
{
	কাঠামো man_viewer_list *viewer;
	ধ্রুবক অক্ষর *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	জন্য (viewer = man_viewer_list; viewer; viewer = viewer->পরবর্তী)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	যদি (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

স্থির শূন্য show_info_page(ধ্রুবক অক্ষর *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (অক্ষর *)ফাঁকা);
	die(_("no info viewer handled the request"));
}

স্থির শূন্য get_html_page_path(কাঠামো strbuf *page_path, ধ্রুবক অক্ষর *page)
{
	কাঠামো stat st;
	ধ্রুবক অক্ষর *path = html_path;
	অক্ষর *to_free = ফাঁকা;

	যদি (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	যদি (!strstr(path, "://")) {
		যদি (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	মুক্ত(to_free);
}

স্থির শূন্য open_html(ধ্রুবক অক্ষর *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (অক্ষর *)ফাঁকা);
}

স্থির শূন্য show_html_page(ধ্রুবক অক্ষর *page)
{
	কাঠামো strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

স্থির অক্ষর *check_git_cmd(ধ্রুবক অক্ষর *cmd)
{
	অক্ষর *alias;

	যদি (is_git_command(cmd))
		ফেরত xstrdup(cmd);

	alias = alias_lookup(cmd);
	যদি (alias) {
		ধ্রুবক অক্ষর **argv;
		পূর্ণসংখ্যা গুনতি;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		যদি (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			মুক্ত(alias);
			exit(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		গুনতি = split_cmdline(alias, &argv);
		যদি (গুনতি < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(গুনতি));
		মুক্ত(argv);
		ফেরত alias;
	}

	যদি (exclude_guides)
		ফেরত help_unknown_cmd(cmd);

	ফেরত xstrdup(cmd);
}

স্থির শূন্য no_help_format(ধ্রুবক অক্ষর *opt_mode, গণনা help_format fmt)
{
	ধ্রুবক অক্ষর *opt_fmt;

	নির্বাচন (fmt) {
	ক্ষেত্র HELP_FORMAT_NONE:
		ফেরত;
	ক্ষেত্র HELP_FORMAT_MAN:
		opt_fmt = "--man";
		বিরতি;
	ক্ষেত্র HELP_FORMAT_INFO:
		opt_fmt = "--info";
		বিরতি;
	ক্ষেত্র HELP_FORMAT_WEB:
		opt_fmt = "--web";
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("unreachable");
	}

	usage_বার্তা_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

স্থির শূন্য opt_mode_usage(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর *opt_mode,
			   গণনা help_format fmt)
{
	যদি (argc)
		usage_বার্তা_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

পূর্ণসংখ্যা cmd_help(পূর্ণসংখ্যা argc,
	     ধ্রুবক অক্ষর **argv,
	     ধ্রুবক অক্ষর *prefix,
	     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা nongit;
	গণনা help_format parsed_help_format;
	অক্ষর *command = ফাঁকা;
	ধ্রুবক অক্ষর *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	যদি (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_বার্তা_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	নির্বাচন (cmd_mode) {
	ক্ষেত্র HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		যদি (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			ফেরত 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		বিরতি;
	ক্ষেত্র HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		ফেরত 0;
	ক্ষেত্র HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		ফেরত 0;
	ক্ষেত্র HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		ফেরত 0;
	ক্ষেত্র HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		ফেরত 0;
	ক্ষেত্র HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		ফেরত 0;
	ক্ষেত্র HELP_ACTION_ALIASES_FOR_COMPLETION: {
		কাঠামো string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		জন্য (আকার_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.বস্তুসমূহ[i].string, '\n',
			       (অক্ষর *)alias_list.বস্তুসমূহ[i].util, '\0');
		string_list_clear(&alias_list, 1);
		ফেরত 0;
	}
	ক্ষেত্র HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		ফেরত 0;
	}

	যদি (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		ফেরত 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ফাঁকা);

	যদি (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	যদি (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	নির্বাচন (help_format) {
	ক্ষেত্র HELP_FORMAT_NONE:
	ক্ষেত্র HELP_FORMAT_MAN:
		show_man_page(page);
		বিরতি;
	ক্ষেত্র HELP_FORMAT_INFO:
		show_info_page(page);
		বিরতি;
	ক্ষেত্র HELP_FORMAT_WEB:
		show_html_page(page);
		বিরতি;
	}

	মুক্ত(command);
	ফেরত 0;
}
