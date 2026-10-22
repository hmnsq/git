/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "exec-cmd.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "config-list.h"
#അന്തര്ഭാവ "help.h"
#അന്തര്ഭാവ "alias.h"
#അന്തര്ഭാവ "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

സ്ഥിതം ഘടന man_viewer_list {
	ഘടന man_viewer_list *next;
	അക്ഷരം name[FLEX_ARRAY];
} *man_viewer_list;

സ്ഥിതം ഘടന man_viewer_info_list {
	ഘടന man_viewer_info_list *next;
	സ്ഥിരം അക്ഷരം *info;
	അക്ഷരം name[FLEX_ARRAY];
} *man_viewer_info_list;

എണ്ണൽ help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

എണ്ണൽ show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

സ്ഥിതം എണ്ണൽ help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

സ്ഥിതം അക്ഷരം *html_path;
സ്ഥിതം പൂർണ്ണസംഖ്യ verbose = 1;
സ്ഥിതം എണ്ണൽ help_format help_format = HELP_FORMAT_NONE;
സ്ഥിതം പൂർണ്ണസംഖ്യ exclude_guides;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_external_commands = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_aliases = -1;
സ്ഥിതം ഘടന option builtin_help_options[] = {
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

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ശൂന്യ
};

ഘടന slot_expansion {
	സ്ഥിരം അക്ഷരം *prefix;
	സ്ഥിരം അക്ഷരം *placeholder;
	ശൂന്യം (*fn)(ഘടന string_list *list, സ്ഥിരം അക്ഷരം *prefix);
	പൂർണ്ണസംഖ്യ found;
};

സ്ഥിതം ശൂന്യം set_config_vars(ഘടന string_list *താക്കോലുകൾ_uniq, ഘടന string_list_item *var)
{
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *str = var->string;
	സ്ഥിരം അക്ഷരം *wildcard = strchr(str, '*');
	സ്ഥിരം അക്ഷരം *tag = strchr(str, '<');
	സ്ഥിരം അക്ഷരം *cut;

	എങ്കിൽ (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	അല്ലെങ്കിൽ എങ്കിൽ (wildcard)
		cut = wildcard;
	അല്ലെങ്കിൽ എങ്കിൽ (tag)
		cut = tag;
	അല്ലെങ്കിൽ {
		string_list_append(താക്കോലുകൾ_uniq, str);
		തിരികെ;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(താക്കോലുകൾ_uniq, sb.buf);
	strbuf_release(&sb);
}

സ്ഥിതം ശൂന്യം set_config_sections(ഘടന string_list *താക്കോലുകൾ_uniq, ഘടന string_list_item *var)
{
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *str = var->string;
	സ്ഥിരം അക്ഷരം *dot = strchr(str, '.');
	സ്ഥിരം അക്ഷരം *cut;

	എങ്കിൽ (dot)
		cut = dot;
	അല്ലെങ്കിൽ {
		set_config_vars(താക്കോലുകൾ_uniq, var);
		തിരികെ;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(താക്കോലുകൾ_uniq, sb.buf);
	strbuf_release(&sb);
}

സ്ഥിതം ശൂന്യം list_config_help(എണ്ണൽ show_config_type type)
{
	ഘടന slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_സന്ദേശം_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_സന്ദേശം_ids },
		{ ശൂന്യ, ശൂന്യ, ശൂന്യ }
	};
	സ്ഥിരം അക്ഷരം **p;
	ഘടന slot_expansion *e;
	ഘടന string_list താക്കോലുകൾ = STRING_LIST_INIT_DUP;
	ഘടന string_list താക്കോലുകൾ_uniq = STRING_LIST_INIT_DUP;
	ഘടന string_list_item *item;
	ഘടന strbuf sb = STRBUF_INIT;

	വേണ്ടി (p = config_name_list; *p; p++) {
		സ്ഥിരം അക്ഷരം *var = *p;

		വേണ്ടി (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			എങ്കിൽ (!strcasecmp(var, sb.buf)) {
				e->fn(&താക്കോലുകൾ, e->prefix);
				e->found++;
				നിർത്തുക;
			}
		}

		എങ്കിൽ (!e->prefix)
			string_list_append(&താക്കോലുകൾ, var);
	}

	strbuf_release(&sb);

	വേണ്ടി (e = slot_expansions; e->prefix; e++)
		എങ്കിൽ (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	വേണ്ടി (size_t i = 0; i < താക്കോലുകൾ.nr; i++) {
		തിരഞ്ഞെടുപ്പ് (type) {
		അവസ്ഥ SHOW_CONFIG_HUMAN:
			string_list_append(&താക്കോലുകൾ_uniq, താക്കോലുകൾ.ഇനങ്ങൾ[i].string);
			നിർത്തുക;
		അവസ്ഥ SHOW_CONFIG_SECTIONS:
			set_config_sections(&താക്കോലുകൾ_uniq, &താക്കോലുകൾ.ഇനങ്ങൾ[i]);
			നിർത്തുക;
		അവസ്ഥ SHOW_CONFIG_VARS:
			set_config_vars(&താക്കോലുകൾ_uniq, &താക്കോലുകൾ.ഇനങ്ങൾ[i]);
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&താക്കോലുകൾ_uniq, 0);
	for_each_string_list_item(item, &താക്കോലുകൾ_uniq)
		puts(item->string);
	string_list_clear(&താക്കോലുകൾ_uniq, 0);
	string_list_clear(&താക്കോലുകൾ, 0);
}

സ്ഥിതം എണ്ണൽ help_format parse_help_format(സ്ഥിരം അക്ഷരം *format)
{
	എങ്കിൽ (!strcmp(format, "man"))
		തിരികെ HELP_FORMAT_MAN;
	എങ്കിൽ (!strcmp(format, "info"))
		തിരികെ HELP_FORMAT_INFO;
	എങ്കിൽ (!strcmp(format, "web") || !strcmp(format, "html"))
		തിരികെ HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_man_viewer_info(സ്ഥിരം അക്ഷരം *name)
{
	ഘടന man_viewer_info_list *viewer;

	വേണ്ടി (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		എങ്കിൽ (!strcasecmp(name, viewer->name))
			തിരികെ viewer->info;
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_emacsclient_version(ശൂന്യം)
{
	ഘടന strbuf buffer = STRBUF_INIT;
	ഘടന child_process ec_process = CHILD_PROCESS_INIT;
	പൂർണ്ണസംഖ്യ version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", ശൂന്യ);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	എങ്കിൽ (start_command(&ec_process))
		തിരികെ error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	എങ്കിൽ (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		തിരികെ error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	എങ്കിൽ (version < 22) {
		strbuf_release(&buffer);
		തിരികെ error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം exec_woman_emacs(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *page)
{
	എങ്കിൽ (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		ഘടന strbuf man_page = STRBUF_INIT;

		എങ്കിൽ (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (അക്ഷരം *)ശൂന്യ);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

സ്ഥിതം ശൂന്യം exec_man_konqueror(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *page)
{
	സ്ഥിരം അക്ഷരം *display = getenv("DISPLAY");
	എങ്കിൽ (display && *display) {
		ഘടന strbuf man_page = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		എങ്കിൽ (path) {
			size_t len;
			എങ്കിൽ (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (പൂർണ്ണസംഖ്യ)len, path);
			filename = basename((അക്ഷരം *)path);
		} അല്ലെങ്കിൽ
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (അക്ഷരം *)ശൂന്യ);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

സ്ഥിതം ശൂന്യം exec_man_man(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *page)
{
	എങ്കിൽ (!path)
		path = "man";
	execlp(path, "man", page, (അക്ഷരം *)ശൂന്യ);
	warning_errno(_("failed to exec '%s'"), path);
}

സ്ഥിതം ശൂന്യം exec_man_cmd(സ്ഥിരം അക്ഷരം *cmd, സ്ഥിരം അക്ഷരം *page)
{
	ഘടന strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (അക്ഷരം *)ശൂന്യ);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

സ്ഥിതം ശൂന്യം add_man_viewer(സ്ഥിരം അക്ഷരം *name)
{
	ഘടന man_viewer_list **p = &man_viewer_list;

	ആകുമ്പോൾ (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ supported_man_viewer(സ്ഥിരം അക്ഷരം *name, size_t len)
{
	തിരികെ (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

സ്ഥിതം ശൂന്യം do_add_man_viewer_info(സ്ഥിരം അക്ഷരം *name,
				   size_t len,
				   സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	ഘടന man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(മൂല്യം);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_man_viewer_path(സ്ഥിരം അക്ഷരം *name,
			       size_t len,
			       സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, മൂല്യം);
	അല്ലെങ്കിൽ
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_man_viewer_cmd(സ്ഥിരം അക്ഷരം *name,
			      size_t len,
			      സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	എങ്കിൽ (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	അല്ലെങ്കിൽ
		do_add_man_viewer_info(name, len, മൂല്യം);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_man_viewer_info(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	സ്ഥിരം അക്ഷരം *name, *subkey;
	size_t namelen;

	എങ്കിൽ (parse_config_താക്കോൽ(var, "man", &name, &namelen, &subkey) < 0 || !name)
		തിരികെ 0;

	എങ്കിൽ (!strcmp(subkey, "path")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		തിരികെ add_man_viewer_path(name, namelen, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(subkey, "cmd")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		തിരികെ add_man_viewer_cmd(name, namelen, മൂല്യം);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_help_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			   സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "help.format")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		help_format = parse_help_format(മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "help.htmlpath")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		സ്വതന്ത്രം(html_path);
		html_path = xstrdup(മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "man.viewer")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		add_man_viewer(മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (starts_with(var, "man."))
		തിരികെ add_man_viewer_info(var, മൂല്യം);

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം ഘടന cmdnames main_cmds, other_cmds;

സ്ഥിതം പൂർണ്ണസംഖ്യ is_git_command(സ്ഥിരം അക്ഷരം *s)
{
	എങ്കിൽ (is_builtin(s))
		തിരികെ 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	തിരികെ is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *cmd_to_page(സ്ഥിരം അക്ഷരം *git_cmd)
{
	എങ്കിൽ (!git_cmd)
		തിരികെ "git";
	അല്ലെങ്കിൽ എങ്കിൽ (starts_with(git_cmd, "git"))
		തിരികെ git_cmd;
	അല്ലെങ്കിൽ എങ്കിൽ (is_git_command(git_cmd))
		തിരികെ xstrfmt("git-%s", git_cmd);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp("scalar", git_cmd))
		തിരികെ xstrdup(git_cmd);
	അല്ലെങ്കിൽ
		തിരികെ xstrfmt("git%s", git_cmd);
}

സ്ഥിതം ശൂന്യം setup_man_path(ശൂന്യം)
{
	ഘടന strbuf new_path = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *old_path = getenv("MANPATH");
	അക്ഷരം *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	എങ്കിൽ (old_path)
		strbuf_addstr(&new_path, old_path);

	സ്വതന്ത്രം(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

സ്ഥിതം ശൂന്യം exec_viewer(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *page)
{
	സ്ഥിരം അക്ഷരം *info = get_man_viewer_info(name);

	എങ്കിൽ (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	അല്ലെങ്കിൽ എങ്കിൽ (info)
		exec_man_cmd(info, page);
	അല്ലെങ്കിൽ
		warning(_("'%s': unknown man viewer."), name);
}

സ്ഥിതം ശൂന്യം show_man_page(സ്ഥിരം അക്ഷരം *page)
{
	ഘടന man_viewer_list *viewer;
	സ്ഥിരം അക്ഷരം *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	വേണ്ടി (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	എങ്കിൽ (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

സ്ഥിതം ശൂന്യം show_info_page(സ്ഥിരം അക്ഷരം *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (അക്ഷരം *)ശൂന്യ);
	die(_("no info viewer handled the request"));
}

സ്ഥിതം ശൂന്യം get_html_page_path(ഘടന strbuf *page_path, സ്ഥിരം അക്ഷരം *page)
{
	ഘടന stat st;
	സ്ഥിരം അക്ഷരം *path = html_path;
	അക്ഷരം *to_free = ശൂന്യ;

	എങ്കിൽ (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	എങ്കിൽ (!strstr(path, "://")) {
		എങ്കിൽ (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	സ്വതന്ത്രം(to_free);
}

സ്ഥിതം ശൂന്യം open_html(സ്ഥിരം അക്ഷരം *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (അക്ഷരം *)ശൂന്യ);
}

സ്ഥിതം ശൂന്യം show_html_page(സ്ഥിരം അക്ഷരം *page)
{
	ഘടന strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

സ്ഥിതം അക്ഷരം *check_git_cmd(സ്ഥിരം അക്ഷരം *cmd)
{
	അക്ഷരം *alias;

	എങ്കിൽ (is_git_command(cmd))
		തിരികെ xstrdup(cmd);

	alias = alias_lookup(cmd);
	എങ്കിൽ (alias) {
		സ്ഥിരം അക്ഷരം **argv;
		പൂർണ്ണസംഖ്യ എണ്ണം;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		എങ്കിൽ (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			സ്വതന്ത്രം(alias);
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
		എണ്ണം = split_cmdline(alias, &argv);
		എങ്കിൽ (എണ്ണം < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(എണ്ണം));
		സ്വതന്ത്രം(argv);
		തിരികെ alias;
	}

	എങ്കിൽ (exclude_guides)
		തിരികെ help_unknown_cmd(cmd);

	തിരികെ xstrdup(cmd);
}

സ്ഥിതം ശൂന്യം no_help_format(സ്ഥിരം അക്ഷരം *opt_mode, എണ്ണൽ help_format fmt)
{
	സ്ഥിരം അക്ഷരം *opt_fmt;

	തിരഞ്ഞെടുപ്പ് (fmt) {
	അവസ്ഥ HELP_FORMAT_NONE:
		തിരികെ;
	അവസ്ഥ HELP_FORMAT_MAN:
		opt_fmt = "--man";
		നിർത്തുക;
	അവസ്ഥ HELP_FORMAT_INFO:
		opt_fmt = "--info";
		നിർത്തുക;
	അവസ്ഥ HELP_FORMAT_WEB:
		opt_fmt = "--web";
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unreachable");
	}

	usage_സന്ദേശം_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

സ്ഥിതം ശൂന്യം opt_mode_usage(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം *opt_mode,
			   എണ്ണൽ help_format fmt)
{
	എങ്കിൽ (argc)
		usage_സന്ദേശം_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

പൂർണ്ണസംഖ്യ cmd_help(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ nongit;
	എണ്ണൽ help_format parsed_help_format;
	അക്ഷരം *command = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	എങ്കിൽ (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_സന്ദേശം_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	തിരഞ്ഞെടുപ്പ് (cmd_mode) {
	അവസ്ഥ HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		എങ്കിൽ (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			തിരികെ 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		നിർത്തുക;
	അവസ്ഥ HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		തിരികെ 0;
	അവസ്ഥ HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		തിരികെ 0;
	അവസ്ഥ HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		തിരികെ 0;
	അവസ്ഥ HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		തിരികെ 0;
	അവസ്ഥ HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		തിരികെ 0;
	അവസ്ഥ HELP_ACTION_ALIASES_FOR_COMPLETION: {
		ഘടന string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		വേണ്ടി (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.ഇനങ്ങൾ[i].string, '\n',
			       (അക്ഷരം *)alias_list.ഇനങ്ങൾ[i].util, '\0');
		string_list_clear(&alias_list, 1);
		തിരികെ 0;
	}
	അവസ്ഥ HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		തിരികെ 0;
	}

	എങ്കിൽ (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		തിരികെ 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ശൂന്യ);

	എങ്കിൽ (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	എങ്കിൽ (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	തിരഞ്ഞെടുപ്പ് (help_format) {
	അവസ്ഥ HELP_FORMAT_NONE:
	അവസ്ഥ HELP_FORMAT_MAN:
		show_man_page(page);
		നിർത്തുക;
	അവസ്ഥ HELP_FORMAT_INFO:
		show_info_page(page);
		നിർത്തുക;
	അവസ്ഥ HELP_FORMAT_WEB:
		show_html_page(page);
		നിർത്തുക;
	}

	സ്വതന്ത്രം(command);
	തിരികെ 0;
}
