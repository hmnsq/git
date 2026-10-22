/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "exec-cmd.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "config-list.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "alias.h"
#ಅನ್ತರ್ಭಾವ "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

ಸ್ಥಿರ ಸಂರಚನಾ man_viewer_list {
	ಸಂರಚನಾ man_viewer_list *next;
	ವರ್ಣ name[FLEX_ARRAY];
} *man_viewer_list;

ಸ್ಥಿರ ಸಂರಚನಾ man_viewer_info_list {
	ಸಂರಚನಾ man_viewer_info_list *next;
	ಧ್ರುವ ವರ್ಣ *info;
	ವರ್ಣ name[FLEX_ARRAY];
} *man_viewer_info_list;

ಗಣನಾ help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

ಗಣನಾ show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ಸ್ಥಿರ ಗಣನಾ help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

ಸ್ಥಿರ ವರ್ಣ *html_path;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbose = 1;
ಸ್ಥಿರ ಗಣನಾ help_format help_format = HELP_FORMAT_NONE;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exclude_guides;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_external_commands = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_aliases = -1;
ಸ್ಥಿರ ಸಂರಚನಾ option builtin_help_options[] = {
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ರಿಕ್ತ
};

ಸಂರಚನಾ slot_expansion {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಧ್ರುವ ವರ್ಣ *placeholder;
	ಶೂನ್ಯ (*fn)(ಸಂರಚನಾ string_list *list, ಧ್ರುವ ವರ್ಣ *prefix);
	ಪೂರ್ಣಾಙ್ಕ found;
};

ಸ್ಥಿರ ಶೂನ್ಯ set_config_vars(ಸಂರಚನಾ string_list *ಕೀಲಿಗಳು_uniq, ಸಂರಚನಾ string_list_item *var)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *str = var->string;
	ಧ್ರುವ ವರ್ಣ *wildcard = strchr(str, '*');
	ಧ್ರುವ ವರ್ಣ *tag = strchr(str, '<');
	ಧ್ರುವ ವರ್ಣ *cut;

	ಯದಿ (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	ಅನ್ಯಥಾ ಯದಿ (wildcard)
		cut = wildcard;
	ಅನ್ಯಥಾ ಯದಿ (tag)
		cut = tag;
	ಅನ್ಯಥಾ {
		string_list_append(ಕೀಲಿಗಳು_uniq, str);
		ಪ್ರತ್ಯಾಗಮ;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(ಕೀಲಿಗಳು_uniq, sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ set_config_sections(ಸಂರಚನಾ string_list *ಕೀಲಿಗಳು_uniq, ಸಂರಚನಾ string_list_item *var)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *str = var->string;
	ಧ್ರುವ ವರ್ಣ *dot = strchr(str, '.');
	ಧ್ರುವ ವರ್ಣ *cut;

	ಯದಿ (dot)
		cut = dot;
	ಅನ್ಯಥಾ {
		set_config_vars(ಕೀಲಿಗಳು_uniq, var);
		ಪ್ರತ್ಯಾಗಮ;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(ಕೀಲಿಗಳು_uniq, sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ list_config_help(ಗಣನಾ show_config_type type)
{
	ಸಂರಚನಾ slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_ಸಂದೇಶ_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_ಸಂದೇಶ_ids },
		{ ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ }
	};
	ಧ್ರುವ ವರ್ಣ **p;
	ಸಂರಚನಾ slot_expansion *e;
	ಸಂರಚನಾ string_list ಕೀಲಿಗಳು = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list ಕೀಲಿಗಳು_uniq = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಪ್ರತಿ (p = config_name_list; *p; p++) {
		ಧ್ರುವ ವರ್ಣ *var = *p;

		ಪ್ರತಿ (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			ಯದಿ (!strcasecmp(var, sb.buf)) {
				e->fn(&ಕೀಲಿಗಳು, e->prefix);
				e->found++;
				ಭಙ್ಗ;
			}
		}

		ಯದಿ (!e->prefix)
			string_list_append(&ಕೀಲಿಗಳು, var);
	}

	strbuf_release(&sb);

	ಪ್ರತಿ (e = slot_expansions; e->prefix; e++)
		ಯದಿ (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	ಪ್ರತಿ (size_t i = 0; i < ಕೀಲಿಗಳು.nr; i++) {
		ವಿಕಲ್ಪ (type) {
		ಪಕ್ಷ SHOW_CONFIG_HUMAN:
			string_list_append(&ಕೀಲಿಗಳು_uniq, ಕೀಲಿಗಳು.ವಸ್ತುಗಳು[i].string);
			ಭಙ್ಗ;
		ಪಕ್ಷ SHOW_CONFIG_SECTIONS:
			set_config_sections(&ಕೀಲಿಗಳು_uniq, &ಕೀಲಿಗಳು.ವಸ್ತುಗಳು[i]);
			ಭಙ್ಗ;
		ಪಕ್ಷ SHOW_CONFIG_VARS:
			set_config_vars(&ಕೀಲಿಗಳು_uniq, &ಕೀಲಿಗಳು.ವಸ್ತುಗಳು[i]);
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&ಕೀಲಿಗಳು_uniq, 0);
	for_each_string_list_item(item, &ಕೀಲಿಗಳು_uniq)
		puts(item->string);
	string_list_clear(&ಕೀಲಿಗಳು_uniq, 0);
	string_list_clear(&ಕೀಲಿಗಳು, 0);
}

ಸ್ಥಿರ ಗಣನಾ help_format parse_help_format(ಧ್ರುವ ವರ್ಣ *format)
{
	ಯದಿ (!strcmp(format, "man"))
		ಪ್ರತ್ಯಾಗಮ HELP_FORMAT_MAN;
	ಯದಿ (!strcmp(format, "info"))
		ಪ್ರತ್ಯಾಗಮ HELP_FORMAT_INFO;
	ಯದಿ (!strcmp(format, "web") || !strcmp(format, "html"))
		ಪ್ರತ್ಯಾಗಮ HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_man_viewer_info(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ man_viewer_info_list *viewer;

	ಪ್ರತಿ (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		ಯದಿ (!strcasecmp(name, viewer->name))
			ಪ್ರತ್ಯಾಗಮ viewer->info;
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_emacsclient_version(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf buffer = STRBUF_INIT;
	ಸಂರಚನಾ child_process ec_process = CHILD_PROCESS_INIT;
	ಪೂರ್ಣಾಙ್ಕ version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", ರಿಕ್ತ);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	ಯದಿ (start_command(&ec_process))
		ಪ್ರತ್ಯಾಗಮ error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	ಯದಿ (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		ಪ್ರತ್ಯಾಗಮ error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	ಯದಿ (version < 22) {
		strbuf_release(&buffer);
		ಪ್ರತ್ಯಾಗಮ error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_woman_emacs(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *page)
{
	ಯದಿ (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		ಸಂರಚನಾ strbuf man_page = STRBUF_INIT;

		ಯದಿ (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (ವರ್ಣ *)ರಿಕ್ತ);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_man_konqueror(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *page)
{
	ಧ್ರುವ ವರ್ಣ *display = getenv("DISPLAY");
	ಯದಿ (display && *display) {
		ಸಂರಚನಾ strbuf man_page = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		ಯದಿ (path) {
			size_t len;
			ಯದಿ (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (ಪೂರ್ಣಾಙ್ಕ)len, path);
			filename = basename((ವರ್ಣ *)path);
		} ಅನ್ಯಥಾ
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (ವರ್ಣ *)ರಿಕ್ತ);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_man_man(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *page)
{
	ಯದಿ (!path)
		path = "man";
	execlp(path, "man", page, (ವರ್ಣ *)ರಿಕ್ತ);
	warning_errno(_("failed to exec '%s'"), path);
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_man_cmd(ಧ್ರುವ ವರ್ಣ *cmd, ಧ್ರುವ ವರ್ಣ *page)
{
	ಸಂರಚನಾ strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (ವರ್ಣ *)ರಿಕ್ತ);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_man_viewer(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ man_viewer_list **p = &man_viewer_list;

	ಯಾವತ್ (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ supported_man_viewer(ಧ್ರುವ ವರ್ಣ *name, size_t len)
{
	ಪ್ರತ್ಯಾಗಮ (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ಸ್ಥಿರ ಶೂನ್ಯ do_add_man_viewer_info(ಧ್ರುವ ವರ್ಣ *name,
				   size_t len,
				   ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಸಂರಚನಾ man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(ಮೌಲ್ಯ);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_man_viewer_path(ಧ್ರುವ ವರ್ಣ *name,
			       size_t len,
			       ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, ಮೌಲ್ಯ);
	ಅನ್ಯಥಾ
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_man_viewer_cmd(ಧ್ರುವ ವರ್ಣ *name,
			      size_t len,
			      ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಯದಿ (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	ಅನ್ಯಥಾ
		do_add_man_viewer_info(name, len, ಮೌಲ್ಯ);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_man_viewer_info(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *name, *subkey;
	size_t namelen;

	ಯದಿ (parse_config_ಕೀಲಿ(var, "man", &name, &namelen, &subkey) < 0 || !name)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!strcmp(subkey, "path")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಪ್ರತ್ಯಾಗಮ add_man_viewer_path(name, namelen, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(subkey, "cmd")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಪ್ರತ್ಯಾಗಮ add_man_viewer_cmd(name, namelen, ಮೌಲ್ಯ);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_help_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			   ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "help.format")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		help_format = parse_help_format(ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "help.htmlpath")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಮುಕ್ತ(html_path);
		html_path = xstrdup(ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "man.viewer")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		add_man_viewer(ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (starts_with(var, "man."))
		ಪ್ರತ್ಯಾಗಮ add_man_viewer_info(var, ಮೌಲ್ಯ);

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಸಂರಚನಾ cmdnames main_cmds, other_cmds;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_git_command(ಧ್ರುವ ವರ್ಣ *s)
{
	ಯದಿ (is_builtin(s))
		ಪ್ರತ್ಯಾಗಮ 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	ಪ್ರತ್ಯಾಗಮ is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *cmd_to_page(ಧ್ರುವ ವರ್ಣ *git_cmd)
{
	ಯದಿ (!git_cmd)
		ಪ್ರತ್ಯಾಗಮ "git";
	ಅನ್ಯಥಾ ಯದಿ (starts_with(git_cmd, "git"))
		ಪ್ರತ್ಯಾಗಮ git_cmd;
	ಅನ್ಯಥಾ ಯದಿ (is_git_command(git_cmd))
		ಪ್ರತ್ಯಾಗಮ xstrfmt("git-%s", git_cmd);
	ಅನ್ಯಥಾ ಯದಿ (!strcmp("scalar", git_cmd))
		ಪ್ರತ್ಯಾಗಮ xstrdup(git_cmd);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ xstrfmt("git%s", git_cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ setup_man_path(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf new_path = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *old_path = getenv("MANPATH");
	ವರ್ಣ *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	ಯದಿ (old_path)
		strbuf_addstr(&new_path, old_path);

	ಮುಕ್ತ(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_viewer(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *page)
{
	ಧ್ರುವ ವರ್ಣ *info = get_man_viewer_info(name);

	ಯದಿ (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	ಅನ್ಯಥಾ ಯದಿ (info)
		exec_man_cmd(info, page);
	ಅನ್ಯಥಾ
		warning(_("'%s': unknown man viewer."), name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_man_page(ಧ್ರುವ ವರ್ಣ *page)
{
	ಸಂರಚನಾ man_viewer_list *viewer;
	ಧ್ರುವ ವರ್ಣ *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	ಪ್ರತಿ (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	ಯದಿ (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

ಸ್ಥಿರ ಶೂನ್ಯ show_info_page(ಧ್ರುವ ವರ್ಣ *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (ವರ್ಣ *)ರಿಕ್ತ);
	die(_("no info viewer handled the request"));
}

ಸ್ಥಿರ ಶೂನ್ಯ get_html_page_path(ಸಂರಚನಾ strbuf *page_path, ಧ್ರುವ ವರ್ಣ *page)
{
	ಸಂರಚನಾ stat st;
	ಧ್ರುವ ವರ್ಣ *path = html_path;
	ವರ್ಣ *to_free = ರಿಕ್ತ;

	ಯದಿ (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	ಯದಿ (!strstr(path, "://")) {
		ಯದಿ (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	ಮುಕ್ತ(to_free);
}

ಸ್ಥಿರ ಶೂನ್ಯ open_html(ಧ್ರುವ ವರ್ಣ *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (ವರ್ಣ *)ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_html_page(ಧ್ರುವ ವರ್ಣ *page)
{
	ಸಂರಚನಾ strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

ಸ್ಥಿರ ವರ್ಣ *check_git_cmd(ಧ್ರುವ ವರ್ಣ *cmd)
{
	ವರ್ಣ *alias;

	ಯದಿ (is_git_command(cmd))
		ಪ್ರತ್ಯಾಗಮ xstrdup(cmd);

	alias = alias_lookup(cmd);
	ಯದಿ (alias) {
		ಧ್ರುವ ವರ್ಣ **argv;
		ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		ಯದಿ (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			ಮುಕ್ತ(alias);
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
		ಎಣಿಕೆ = split_cmdline(alias, &argv);
		ಯದಿ (ಎಣಿಕೆ < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(ಎಣಿಕೆ));
		ಮುಕ್ತ(argv);
		ಪ್ರತ್ಯಾಗಮ alias;
	}

	ಯದಿ (exclude_guides)
		ಪ್ರತ್ಯಾಗಮ help_unknown_cmd(cmd);

	ಪ್ರತ್ಯಾಗಮ xstrdup(cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ no_help_format(ಧ್ರುವ ವರ್ಣ *opt_mode, ಗಣನಾ help_format fmt)
{
	ಧ್ರುವ ವರ್ಣ *opt_fmt;

	ವಿಕಲ್ಪ (fmt) {
	ಪಕ್ಷ HELP_FORMAT_NONE:
		ಪ್ರತ್ಯಾಗಮ;
	ಪಕ್ಷ HELP_FORMAT_MAN:
		opt_fmt = "--man";
		ಭಙ್ಗ;
	ಪಕ್ಷ HELP_FORMAT_INFO:
		opt_fmt = "--info";
		ಭಙ್ಗ;
	ಪಕ್ಷ HELP_FORMAT_WEB:
		opt_fmt = "--web";
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unreachable");
	}

	usage_ಸಂದೇಶ_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

ಸ್ಥಿರ ಶೂನ್ಯ opt_mode_usage(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ *opt_mode,
			   ಗಣನಾ help_format fmt)
{
	ಯದಿ (argc)
		usage_ಸಂದೇಶ_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

ಪೂರ್ಣಾಙ್ಕ cmd_help(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ nongit;
	ಗಣನಾ help_format parsed_help_format;
	ವರ್ಣ *command = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	ಯದಿ (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_ಸಂದೇಶ_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	ವಿಕಲ್ಪ (cmd_mode) {
	ಪಕ್ಷ HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		ಯದಿ (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		ಭಙ್ಗ;
	ಪಕ್ಷ HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪಕ್ಷ HELP_ACTION_ALIASES_FOR_COMPLETION: {
		ಸಂರಚನಾ string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		ಪ್ರತಿ (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.ವಸ್ತುಗಳು[i].string, '\n',
			       (ವರ್ಣ *)alias_list.ವಸ್ತುಗಳು[i].util, '\0');
		string_list_clear(&alias_list, 1);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪಕ್ಷ HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ರಿಕ್ತ);

	ಯದಿ (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	ಯದಿ (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	ವಿಕಲ್ಪ (help_format) {
	ಪಕ್ಷ HELP_FORMAT_NONE:
	ಪಕ್ಷ HELP_FORMAT_MAN:
		show_man_page(page);
		ಭಙ್ಗ;
	ಪಕ್ಷ HELP_FORMAT_INFO:
		show_info_page(page);
		ಭಙ್ಗ;
	ಪಕ್ಷ HELP_FORMAT_WEB:
		show_html_page(page);
		ಭಙ್ಗ;
	}

	ಮುಕ್ತ(command);
	ಪ್ರತ್ಯಾಗಮ 0;
}
