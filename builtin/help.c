/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "exec-cmd.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "pager.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "run-command.h"
#অন্তর্ভাৱ "config-list.h"
#অন্তর্ভাৱ "help.h"
#অন্তর্ভাৱ "alias.h"
#অন্তর্ভাৱ "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

স্থির সংরচনা man_viewer_list {
	সংরচনা man_viewer_list *next;
	ৱর্ণ name[FLEX_ARRAY];
} *man_viewer_list;

স্থির সংরচনা man_viewer_info_list {
	সংরচনা man_viewer_info_list *next;
	ধ্রুৱ ৱর্ণ *info;
	ৱর্ণ name[FLEX_ARRAY];
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

স্থির ৱর্ণ *html_path;
স্থির পূর্ণাঙ্ক verbose = 1;
স্থির গণনা help_format help_format = HELP_FORMAT_NONE;
স্থির পূর্ণাঙ্ক exclude_guides;
স্থির পূর্ণাঙ্ক show_external_commands = -1;
স্থির পূর্ণাঙ্ক show_aliases = -1;
স্থির সংরচনা option builtin_help_options[] = {
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

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	রিক্ত
};

সংরচনা slot_expansion {
	ধ্রুৱ ৱর্ণ *prefix;
	ধ্রুৱ ৱর্ণ *placeholder;
	শূন্য (*fn)(সংরচনা string_list *list, ধ্রুৱ ৱর্ণ *prefix);
	পূর্ণাঙ্ক found;
};

স্থির শূন্য set_config_vars(সংরচনা string_list *চাবিগুলি_uniq, সংরচনা string_list_item *var)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *str = var->string;
	ধ্রুৱ ৱর্ণ *wildcard = strchr(str, '*');
	ধ্রুৱ ৱর্ণ *tag = strchr(str, '<');
	ধ্রুৱ ৱর্ণ *cut;

	যদি (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	অন্যথা যদি (wildcard)
		cut = wildcard;
	অন্যথা যদি (tag)
		cut = tag;
	অন্যথা {
		string_list_append(চাবিগুলি_uniq, str);
		ঘূৰাই;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(চাবিগুলি_uniq, sb.buf);
	strbuf_release(&sb);
}

স্থির শূন্য set_config_sections(সংরচনা string_list *চাবিগুলি_uniq, সংরচনা string_list_item *var)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *str = var->string;
	ধ্রুৱ ৱর্ণ *dot = strchr(str, '.');
	ধ্রুৱ ৱর্ণ *cut;

	যদি (dot)
		cut = dot;
	অন্যথা {
		set_config_vars(চাবিগুলি_uniq, var);
		ঘূৰাই;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(চাবিগুলি_uniq, sb.buf);
	strbuf_release(&sb);
}

স্থির শূন্য list_config_help(গণনা show_config_type type)
{
	সংরচনা slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_বাৰ্তা_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_বাৰ্তা_ids },
		{ রিক্ত, রিক্ত, রিক্ত }
	};
	ধ্রুৱ ৱর্ণ **p;
	সংরচনা slot_expansion *e;
	সংরচনা string_list চাবিগুলি = STRING_LIST_INIT_DUP;
	সংরচনা string_list চাবিগুলি_uniq = STRING_LIST_INIT_DUP;
	সংরচনা string_list_item *item;
	সংরচনা strbuf sb = STRBUF_INIT;

	বাবে (p = config_name_list; *p; p++) {
		ধ্রুৱ ৱর্ণ *var = *p;

		বাবে (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			যদি (!strcasecmp(var, sb.buf)) {
				e->fn(&চাবিগুলি, e->prefix);
				e->found++;
				ভঙ্গ;
			}
		}

		যদি (!e->prefix)
			string_list_append(&চাবিগুলি, var);
	}

	strbuf_release(&sb);

	বাবে (e = slot_expansions; e->prefix; e++)
		যদি (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	বাবে (size_t i = 0; i < চাবিগুলি.nr; i++) {
		ৱিকল্প (type) {
		পক্ষ SHOW_CONFIG_HUMAN:
			string_list_append(&চাবিগুলি_uniq, চাবিগুলি.বস্তুসমূহ[i].string);
			ভঙ্গ;
		পক্ষ SHOW_CONFIG_SECTIONS:
			set_config_sections(&চাবিগুলি_uniq, &চাবিগুলি.বস্তুসমূহ[i]);
			ভঙ্গ;
		পক্ষ SHOW_CONFIG_VARS:
			set_config_vars(&চাবিগুলি_uniq, &চাবিগুলি.বস্তুসমূহ[i]);
			ভঙ্গ;
		স্ৱভাৱ:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&চাবিগুলি_uniq, 0);
	for_each_string_list_item(item, &চাবিগুলি_uniq)
		puts(item->string);
	string_list_clear(&চাবিগুলি_uniq, 0);
	string_list_clear(&চাবিগুলি, 0);
}

স্থির গণনা help_format parse_help_format(ধ্রুৱ ৱর্ণ *format)
{
	যদি (!strcmp(format, "man"))
		ঘূৰাই HELP_FORMAT_MAN;
	যদি (!strcmp(format, "info"))
		ঘূৰাই HELP_FORMAT_INFO;
	যদি (!strcmp(format, "web") || !strcmp(format, "html"))
		ঘূৰাই HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

স্থির ধ্রুৱ ৱর্ণ *get_man_viewer_info(ধ্রুৱ ৱর্ণ *name)
{
	সংরচনা man_viewer_info_list *viewer;

	বাবে (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		যদি (!strcasecmp(name, viewer->name))
			ঘূৰাই viewer->info;
	}
	ঘূৰাই রিক্ত;
}

স্থির পূর্ণাঙ্ক check_emacsclient_version(শূন্য)
{
	সংরচনা strbuf buffer = STRBUF_INIT;
	সংরচনা child_process ec_process = CHILD_PROCESS_INIT;
	পূর্ণাঙ্ক version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", রিক্ত);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	যদি (start_command(&ec_process))
		ঘূৰাই error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	যদি (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		ঘূৰাই error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	যদি (version < 22) {
		strbuf_release(&buffer);
		ঘূৰাই error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	ঘূৰাই 0;
}

স্থির শূন্য exec_woman_emacs(ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *page)
{
	যদি (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		সংরচনা strbuf man_page = STRBUF_INIT;

		যদি (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (ৱর্ণ *)রিক্ত);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

স্থির শূন্য exec_man_konqueror(ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *page)
{
	ধ্রুৱ ৱর্ণ *display = getenv("DISPLAY");
	যদি (display && *display) {
		সংরচনা strbuf man_page = STRBUF_INIT;
		ধ্রুৱ ৱর্ণ *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		যদি (path) {
			size_t len;
			যদি (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (পূর্ণাঙ্ক)len, path);
			filename = basename((ৱর্ণ *)path);
		} অন্যথা
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (ৱর্ণ *)রিক্ত);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

স্থির শূন্য exec_man_man(ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *page)
{
	যদি (!path)
		path = "man";
	execlp(path, "man", page, (ৱর্ণ *)রিক্ত);
	warning_errno(_("failed to exec '%s'"), path);
}

স্থির শূন্য exec_man_cmd(ধ্রুৱ ৱর্ণ *cmd, ধ্রুৱ ৱর্ণ *page)
{
	সংরচনা strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (ৱর্ণ *)রিক্ত);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

স্থির শূন্য add_man_viewer(ধ্রুৱ ৱর্ণ *name)
{
	সংরচনা man_viewer_list **p = &man_viewer_list;

	যেতিয়ালৈ (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

স্থির পূর্ণাঙ্ক supported_man_viewer(ধ্রুৱ ৱর্ণ *name, size_t len)
{
	ঘূৰাই (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

স্থির শূন্য do_add_man_viewer_info(ধ্রুৱ ৱর্ণ *name,
				   size_t len,
				   ধ্রুৱ ৱর্ণ *মান)
{
	সংরচনা man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(মান);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

স্থির পূর্ণাঙ্ক add_man_viewer_path(ধ্রুৱ ৱর্ণ *name,
			       size_t len,
			       ধ্রুৱ ৱর্ণ *মান)
{
	যদি (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, মান);
	অন্যথা
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক add_man_viewer_cmd(ধ্রুৱ ৱর্ণ *name,
			      size_t len,
			      ধ্রুৱ ৱর্ণ *মান)
{
	যদি (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	অন্যথা
		do_add_man_viewer_info(name, len, মান);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক add_man_viewer_info(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান)
{
	ধ্রুৱ ৱর্ণ *name, *subkey;
	size_t namelen;

	যদি (parse_config_চাবি(var, "man", &name, &namelen, &subkey) < 0 || !name)
		ঘূৰাই 0;

	যদি (!strcmp(subkey, "path")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		ঘূৰাই add_man_viewer_path(name, namelen, মান);
	}
	যদি (!strcmp(subkey, "cmd")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		ঘূৰাই add_man_viewer_cmd(name, namelen, মান);
	}

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক git_help_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			   ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "help.format")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		help_format = parse_help_format(মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "help.htmlpath")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		মুক্ত(html_path);
		html_path = xstrdup(মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "man.viewer")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		add_man_viewer(মান);
		ঘূৰাই 0;
	}
	যদি (starts_with(var, "man."))
		ঘূৰাই add_man_viewer_info(var, মান);

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

স্থির সংরচনা cmdnames main_cmds, other_cmds;

স্থির পূর্ণাঙ্ক is_git_command(ধ্রুৱ ৱর্ণ *s)
{
	যদি (is_builtin(s))
		ঘূৰাই 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	ঘূৰাই is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

স্থির ধ্রুৱ ৱর্ণ *cmd_to_page(ধ্রুৱ ৱর্ণ *git_cmd)
{
	যদি (!git_cmd)
		ঘূৰাই "git";
	অন্যথা যদি (starts_with(git_cmd, "git"))
		ঘূৰাই git_cmd;
	অন্যথা যদি (is_git_command(git_cmd))
		ঘূৰাই xstrfmt("git-%s", git_cmd);
	অন্যথা যদি (!strcmp("scalar", git_cmd))
		ঘূৰাই xstrdup(git_cmd);
	অন্যথা
		ঘূৰাই xstrfmt("git%s", git_cmd);
}

স্থির শূন্য setup_man_path(শূন্য)
{
	সংরচনা strbuf new_path = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *old_path = getenv("MANPATH");
	ৱর্ণ *git_man_path = system_path(GIT_MAN_PATH);

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

স্থির শূন্য exec_viewer(ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *page)
{
	ধ্রুৱ ৱর্ণ *info = get_man_viewer_info(name);

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

স্থির শূন্য show_man_page(ধ্রুৱ ৱর্ণ *page)
{
	সংরচনা man_viewer_list *viewer;
	ধ্রুৱ ৱর্ণ *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	বাবে (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	যদি (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

স্থির শূন্য show_info_page(ধ্রুৱ ৱর্ণ *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (ৱর্ণ *)রিক্ত);
	die(_("no info viewer handled the request"));
}

স্থির শূন্য get_html_page_path(সংরচনা strbuf *page_path, ধ্রুৱ ৱর্ণ *page)
{
	সংরচনা stat st;
	ধ্রুৱ ৱর্ণ *path = html_path;
	ৱর্ণ *to_free = রিক্ত;

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

স্থির শূন্য open_html(ধ্রুৱ ৱর্ণ *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (ৱর্ণ *)রিক্ত);
}

স্থির শূন্য show_html_page(ধ্রুৱ ৱর্ণ *page)
{
	সংরচনা strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

স্থির ৱর্ণ *check_git_cmd(ধ্রুৱ ৱর্ণ *cmd)
{
	ৱর্ণ *alias;

	যদি (is_git_command(cmd))
		ঘূৰাই xstrdup(cmd);

	alias = alias_lookup(cmd);
	যদি (alias) {
		ধ্রুৱ ৱর্ণ **argv;
		পূর্ণাঙ্ক গুনতি;

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
		ঘূৰাই alias;
	}

	যদি (exclude_guides)
		ঘূৰাই help_unknown_cmd(cmd);

	ঘূৰাই xstrdup(cmd);
}

স্থির শূন্য no_help_format(ধ্রুৱ ৱর্ণ *opt_mode, গণনা help_format fmt)
{
	ধ্রুৱ ৱর্ণ *opt_fmt;

	ৱিকল্প (fmt) {
	পক্ষ HELP_FORMAT_NONE:
		ঘূৰাই;
	পক্ষ HELP_FORMAT_MAN:
		opt_fmt = "--man";
		ভঙ্গ;
	পক্ষ HELP_FORMAT_INFO:
		opt_fmt = "--info";
		ভঙ্গ;
	পক্ষ HELP_FORMAT_WEB:
		opt_fmt = "--web";
		ভঙ্গ;
	স্ৱভাৱ:
		BUG("unreachable");
	}

	usage_বাৰ্তা_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

স্থির শূন্য opt_mode_usage(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ *opt_mode,
			   গণনা help_format fmt)
{
	যদি (argc)
		usage_বাৰ্তা_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

পূর্ণাঙ্ক cmd_help(পূর্ণাঙ্ক argc,
	     ধ্রুৱ ৱর্ণ **argv,
	     ধ্রুৱ ৱর্ণ *prefix,
	     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক nongit;
	গণনা help_format parsed_help_format;
	ৱর্ণ *command = রিক্ত;
	ধ্রুৱ ৱর্ণ *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	যদি (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_বাৰ্তা_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	ৱিকল্প (cmd_mode) {
	পক্ষ HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		যদি (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			ঘূৰাই 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		ভঙ্গ;
	পক্ষ HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		ঘূৰাই 0;
	পক্ষ HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		ঘূৰাই 0;
	পক্ষ HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		ঘূৰাই 0;
	পক্ষ HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		ঘূৰাই 0;
	পক্ষ HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		ঘূৰাই 0;
	পক্ষ HELP_ACTION_ALIASES_FOR_COMPLETION: {
		সংরচনা string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		বাবে (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.বস্তুসমূহ[i].string, '\n',
			       (ৱর্ণ *)alias_list.বস্তুসমূহ[i].util, '\0');
		string_list_clear(&alias_list, 1);
		ঘূৰাই 0;
	}
	পক্ষ HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		ঘূৰাই 0;
	}

	যদি (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		ঘূৰাই 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, রিক্ত);

	যদি (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	যদি (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	ৱিকল্প (help_format) {
	পক্ষ HELP_FORMAT_NONE:
	পক্ষ HELP_FORMAT_MAN:
		show_man_page(page);
		ভঙ্গ;
	পক্ষ HELP_FORMAT_INFO:
		show_info_page(page);
		ভঙ্গ;
	পক্ষ HELP_FORMAT_WEB:
		show_html_page(page);
		ভঙ্গ;
	}

	মুক্ত(command);
	ঘূৰাই 0;
}
