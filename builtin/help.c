/*
 * Builtin help command
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DEFAULT_HELP_FORMAT
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_HELP_FORMAT "man"
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ man_viewer_list {
	ರಚನೆ man_viewer_list *ಮುಂದಿನ;
	ಅಕ್ಷರ name[FLEX_ARRAY];
} *man_viewer_list;

ಸ್ಥಿರ ರಚನೆ man_viewer_info_list {
	ರಚನೆ man_viewer_info_list *ಮುಂದಿನ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *info;
	ಅಕ್ಷರ name[FLEX_ARRAY];
} *man_viewer_info_list;

ಎಣಿಕೆ help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

ಎಣಿಕೆ show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ಸ್ಥಿರ ಎಣಿಕೆ help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_ಬಹುಲಕ;

ಸ್ಥಿರ ಅಕ್ಷರ *html_ಮಾರ್ಗ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbose = 1;
ಸ್ಥಿರ ಎಣಿಕೆ help_format help_format = HELP_FORMAT_NONE;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exclude_guides;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_external_commands = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_aliases = -1;
ಸ್ಥಿರ ರಚನೆ option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_ಬಹುಲಕ, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_ಗಣ_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_ಗಣ_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_ಗಣ_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_ಬಹುಲಕ, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_ಬಹುಲಕ,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_ಬಹುಲಕ,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_ಬಹುಲಕ, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_ಬಹುಲಕ, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_ಬಹುಲಕ, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_ಬಹುಲಕ, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_ಕೊನೆ(),
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ಖಾಲಿ
};

ರಚನೆ ಸ್ಥಾನ_expansion {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *placeholder;
	ಶೂನ್ಯ (*fn)(ರಚನೆ string_list *list, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2;
};

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_config_vars(ರಚನೆ string_list *ಕೀಲಿಗಳು_uniq, ರಚನೆ string_list_ಅಂಶ *var)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = var->string;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wildcard = strchr(str, '*');
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್ = strchr(str, '<');
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cut;

	ಆದರೆ (wildcard && ಟ್ಯಾಗ್)
		cut = wildcard < ಟ್ಯಾಗ್ ? wildcard : ಟ್ಯಾಗ್;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (wildcard)
		cut = wildcard;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಟ್ಯಾಗ್)
		cut = ಟ್ಯಾಗ್;
	ಇಲ್ಲದಿದ್ದರೆ {
		string_list_append(ಕೀಲಿಗಳು_uniq, str);
		ಹಿಂತಿರುಗಿ;
	}

	strbuf_ಕೂಡಿಸು(&sb, str, cut - str);
	string_list_append(ಕೀಲಿಗಳು_uniq, sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_config_sections(ರಚನೆ string_list *ಕೀಲಿಗಳು_uniq, ರಚನೆ string_list_ಅಂಶ *var)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = var->string;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dot = strchr(str, '.');
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cut;

	ಆದರೆ (dot)
		cut = dot;
	ಇಲ್ಲದಿದ್ದರೆ {
		ಗಣ_config_vars(ಕೀಲಿಗಳು_uniq, var);
		ಹಿಂತಿರುಗಿ;
	}

	strbuf_ಕೂಡಿಸು(&sb, str, cut - str);
	string_list_append(ಕೀಲಿಗಳು_uniq, sb.buf);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ list_config_help(ಎಣಿಕೆ show_config_type type)
{
	ರಚನೆ ಸ್ಥಾನ_expansion ಸ್ಥಾನ_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_ಸ್ಥಿತಿ_slots },
		{ "fsck", "<msg-id>", list_config_fsck_ಸಂದೇಶ_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_ಸಂದೇಶ_ids },
		{ ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ }
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p;
	ರಚನೆ ಸ್ಥಾನ_expansion *e;
	ರಚನೆ string_list ಕೀಲಿಗಳು = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list ಕೀಲಿಗಳು_uniq = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಗಾಗಿ (p = config_name_list; *p; p++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var = *p;

		ಗಾಗಿ (e = ಸ್ಥಾನ_expansions; e->ಪೂರ್ವಪ್ರತ್ಯಯ; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->ಪೂರ್ವಪ್ರತ್ಯಯ, e->placeholder);
			ಆದರೆ (!strcasecmp(var, sb.buf)) {
				e->fn(&ಕೀಲಿಗಳು, e->ಪೂರ್ವಪ್ರತ್ಯಯ);
				e->ಸಿಕ್ಕಿತು_2++;
				ನಿಲ್ಲಿಸು;
			}
		}

		ಆದರೆ (!e->ಪೂರ್ವಪ್ರತ್ಯಯ)
			string_list_append(&ಕೀಲಿಗಳು, var);
	}

	strbuf_release(&sb);

	ಗಾಗಿ (e = ಸ್ಥಾನ_expansions; e->ಪೂರ್ವಪ್ರತ್ಯಯ; e++)
		ಆದರೆ (!e->ಸಿಕ್ಕಿತು_2)
			BUG("slot_expansion %s.%s is not used",
			    e->ಪೂರ್ವಪ್ರತ್ಯಯ, e->placeholder);

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಕೀಲಿಗಳು.nr; i++) {
		ಆಯ್ಕೆ (type) {
		ಸ್ಥಿತಿ SHOW_CONFIG_HUMAN:
			string_list_append(&ಕೀಲಿಗಳು_uniq, ಕೀಲಿಗಳು.ವಸ್ತುಗಳು[i].string);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SHOW_CONFIG_SECTIONS:
			ಗಣ_config_sections(&ಕೀಲಿಗಳು_uniq, &ಕೀಲಿಗಳು.ವಸ್ತುಗಳು[i]);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SHOW_CONFIG_VARS:
			ಗಣ_config_vars(&ಕೀಲಿಗಳು_uniq, &ಕೀಲಿಗಳು.ವಸ್ತುಗಳು[i]);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_ವಿಂಗಡಿಸು_u(&ಕೀಲಿಗಳು_uniq, 0);
	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಕೀಲಿಗಳು_uniq)
		puts(ಅಂಶ->string);
	string_list_clear(&ಕೀಲಿಗಳು_uniq, 0);
	string_list_clear(&ಕೀಲಿಗಳು, 0);
}

ಸ್ಥಿರ ಎಣಿಕೆ help_format parse_help_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format)
{
	ಆದರೆ (!strcmp(format, "man"))
		ಹಿಂತಿರುಗಿ HELP_FORMAT_MAN;
	ಆದರೆ (!strcmp(format, "info"))
		ಹಿಂತಿರುಗಿ HELP_FORMAT_INFO;
	ಆದರೆ (!strcmp(format, "web") || !strcmp(format, "html"))
		ಹಿಂತಿರುಗಿ HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_man_viewer_info(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ man_viewer_info_list *viewer;

	ಗಾಗಿ (viewer = man_viewer_info_list; viewer; viewer = viewer->ಮುಂದಿನ)
	{
		ಆದರೆ (!strcasecmp(name, viewer->name))
			ಹಿಂತಿರುಗಿ viewer->info;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_emacsclient_ಆವೃತ್ತಿ(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf ಬಫರ್ = STRBUF_INIT;
	ರಚನೆ ಮಗು_process ec_process = ಮಗು_PROCESS_INIT;
	ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.ವಾದಗಳು, "emacsclient", "--version", ಖಾಲಿ);
	ec_process.err = -1;
	ec_process.stdout_ಗಮ್ಯ_stderr = 1;
	ಆದರೆ (ಆರಂಭ_command(&ec_process))
		ಹಿಂತಿರುಗಿ error(_("Failed to start emacsclient."));

	strbuf_read(&ಬಫರ್, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	ಮುಕ್ತಾಯ_command(&ec_process);

	ಆದರೆ (!starts_with(ಬಫರ್.buf, "emacsclient")) {
		strbuf_release(&ಬಫರ್);
		ಹಿಂತಿರುಗಿ error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&ಬಫರ್, 0, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("emacsclient"));
	ಆವೃತ್ತಿ = atoi(ಬಫರ್.buf);

	ಆದರೆ (ಆವೃತ್ತಿ < 22) {
		strbuf_release(&ಬಫರ್);
		ಹಿಂತಿರುಗಿ error(_("emacsclient version '%d' too old (< 22)."),
			ಆವೃತ್ತಿ);
	}

	strbuf_release(&ಬಫರ್);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_woman_emacs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ಆದರೆ (!check_emacsclient_ಆವೃತ್ತಿ()) {
		/* This works only with emacsclient version >= 22. */
		ರಚನೆ strbuf man_ಪುಟ = STRBUF_INIT;

		ಆದರೆ (!ಮಾರ್ಗ)
			ಮಾರ್ಗ = "emacsclient";
		strbuf_addf(&man_ಪುಟ, "(woman \"%s\")", ಪುಟ);
		execlp(ಮಾರ್ಗ, "emacsclient", "-e", man_ಪುಟ.buf, (ಅಕ್ಷರ *)ಖಾಲಿ);
		warning_errno(_("failed to exec '%s'"), ಮಾರ್ಗ);
		strbuf_release(&man_ಪುಟ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_man_konqueror(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *display = getenv("DISPLAY");
	ಆದರೆ (display && *display) {
		ರಚನೆ strbuf man_ಪುಟ = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		ಆದರೆ (ಮಾರ್ಗ) {
			ಗಾತ್ರ_t len;
			ಆದರೆ (strip_suffix(ಮಾರ್ಗ, "/konqueror", &len))
				ಮಾರ್ಗ = xstrfmt("%.*s/kfmclient", (ಪೂರ್ಣಾಂಕ)len, ಮಾರ್ಗ);
			filename = basename((ಅಕ್ಷರ *)ಮಾರ್ಗ);
		} ಇಲ್ಲದಿದ್ದರೆ
			ಮಾರ್ಗ = "kfmclient";
		strbuf_addf(&man_ಪುಟ, "man:%s(1)", ಪುಟ);
		execlp(ಮಾರ್ಗ, filename, "newTab", man_ಪುಟ.buf, (ಅಕ್ಷರ *)ಖಾಲಿ);
		warning_errno(_("failed to exec '%s'"), ಮಾರ್ಗ);
		strbuf_release(&man_ಪುಟ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_man_man(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ಆದರೆ (!ಮಾರ್ಗ)
		ಮಾರ್ಗ = "man";
	execlp(ಮಾರ್ಗ, "man", ಪುಟ, (ಅಕ್ಷರ *)ಖಾಲಿ);
	warning_errno(_("failed to exec '%s'"), ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_man_cmd(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ರಚನೆ strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, ಪುಟ);
	execl(SHELL_ಮಾರ್ಗ, SHELL_ಮಾರ್ಗ, "-c", shell_cmd.buf, (ಅಕ್ಷರ *)ಖಾಲಿ);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_man_viewer(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ man_viewer_list **p = &man_viewer_list;

	ಆಗ (*p)
		p = &((*p)->ಮುಂದಿನ);
	FLEX_ALLOC_STR(*p, name, name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ supported_man_viewer(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಗಾತ್ರ_t len)
{
	ಹಿಂತಿರುಗಿ (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ಸ್ಥಿರ ಶೂನ್ಯ do_ಕೂಡಿಸು_man_viewer_info(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
				   ಗಾತ್ರ_t len,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ರಚನೆ man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(ಮೌಲ್ಯ);
	new_man_viewer->ಮುಂದಿನ = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_man_viewer_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			       ಗಾತ್ರ_t len,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (supported_man_viewer(name, len))
		do_ಕೂಡಿಸು_man_viewer_info(name, len, ಮೌಲ್ಯ);
	ಇಲ್ಲದಿದ್ದರೆ
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_man_viewer_cmd(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			      ಗಾತ್ರ_t len,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆದರೆ (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	ಇಲ್ಲದಿದ್ದರೆ
		do_ಕೂಡಿಸು_man_viewer_info(name, len, ಮೌಲ್ಯ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_man_viewer_info(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, *subkey;
	ಗಾತ್ರ_t namelen;

	ಆದರೆ (parse_config_ಕೀಲಿ(var, "man", &name, &namelen, &subkey) < 0 || !name)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!strcmp(subkey, "path")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_man_viewer_ಮಾರ್ಗ(name, namelen, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(subkey, "cmd")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_man_viewer_cmd(name, namelen, ಮೌಲ್ಯ);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_help_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "help.format")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		help_format = parse_help_format(ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "help.htmlpath")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಬಿಡುಗಡೆ(html_ಮಾರ್ಗ);
		html_ಮಾರ್ಗ = xstrdup(ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "man.viewer")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಕೂಡಿಸು_man_viewer(ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (starts_with(var, "man."))
		ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_man_viewer_info(var, ಮೌಲ್ಯ);

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ರಚನೆ cmdnames main_cmds, other_cmds;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_git_command(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಆದರೆ (is_builtin(s))
		ಹಿಂತಿರುಗಿ 1;

	ತುಂಬು_command_list("git-", &main_cmds, &other_cmds);
	ಹಿಂತಿರುಗಿ is_ಒಳ_cmdlist(&main_cmds, s) ||
		is_ಒಳ_cmdlist(&other_cmds, s);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd_ಗಮ್ಯ_ಪುಟ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_cmd)
{
	ಆದರೆ (!git_cmd)
		ಹಿಂತಿರುಗಿ "git";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(git_cmd, "git"))
		ಹಿಂತಿರುಗಿ git_cmd;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_git_command(git_cmd))
		ಹಿಂತಿರುಗಿ xstrfmt("git-%s", git_cmd);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp("scalar", git_cmd))
		ಹಿಂತಿರುಗಿ xstrdup(git_cmd);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ xstrfmt("git%s", git_cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_man_ಮಾರ್ಗ(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf new_ಮಾರ್ಗ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_ಮಾರ್ಗ = getenv("MANPATH");
	ಅಕ್ಷರ *git_man_ಮಾರ್ಗ = system_ಮಾರ್ಗ(GIT_MAN_ಮಾರ್ಗ);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_ಮಾರ್ಗ, git_man_ಮಾರ್ಗ);
	strbuf_addch(&new_ಮಾರ್ಗ, ':');
	ಆದರೆ (old_ಮಾರ್ಗ)
		strbuf_addstr(&new_ಮಾರ್ಗ, old_ಮಾರ್ಗ);

	ಬಿಡುಗಡೆ(git_man_ಮಾರ್ಗ);
	setenv("MANPATH", new_ಮಾರ್ಗ.buf, 1);

	strbuf_release(&new_ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ exec_viewer(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *info = get_man_viewer_info(name);

	ಆದರೆ (!strcasecmp(name, "man"))
		exec_man_man(info, ಪುಟ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, ಪುಟ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, ಪುಟ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info)
		exec_man_cmd(info, ಪುಟ);
	ಇಲ್ಲದಿದ್ದರೆ
		warning(_("'%s': unknown man viewer."), name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_man_ಪುಟ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ರಚನೆ man_viewer_list *viewer;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fallback = getenv("GIT_MAN_VIEWER");

	ಸಿದ್ಧತೆ_man_ಮಾರ್ಗ();
	ಗಾಗಿ (viewer = man_viewer_list; viewer; viewer = viewer->ಮುಂದಿನ)
	{
		exec_viewer(viewer->name, ಪುಟ); /* will return when unable */
	}
	ಆದರೆ (fallback)
		exec_viewer(fallback, ಪುಟ);
	exec_viewer("man", ಪುಟ);
	die(_("no man viewer handled the request"));
}

ಸ್ಥಿರ ಶೂನ್ಯ show_info_ಪುಟ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	setenv("INFOPATH", system_ಮಾರ್ಗ(GIT_INFO_ಮಾರ್ಗ), 1);
	execlp("info", "info", "gitman", ಪುಟ, (ಅಕ್ಷರ *)ಖಾಲಿ);
	die(_("no info viewer handled the request"));
}

ಸ್ಥಿರ ಶೂನ್ಯ get_html_ಪುಟ_ಮಾರ್ಗ(ರಚನೆ strbuf *ಪುಟ_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ರಚನೆ stat st;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = html_ಮಾರ್ಗ;
	ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;

	ಆದರೆ (!ಮಾರ್ಗ)
		ಮಾರ್ಗ = ಗಮ್ಯ_free = system_ಮಾರ್ಗ(GIT_HTML_ಮಾರ್ಗ);

	/*
	 * Check that the page we're looking for exists.
	 */
	ಆದರೆ (!ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ಮಾರ್ಗ, "://")) {
		ಆದರೆ (stat(mkpath("%s/%s.html", ಮಾರ್ಗ, ಪುಟ), &st)
		    || !S_ISREG(st.st_ಬಹುಲಕ))
			die("'%s/%s.html': documentation file not found.",
				ಮಾರ್ಗ, ಪುಟ);
	}

	strbuf_init(ಪುಟ_ಮಾರ್ಗ, 0);
	strbuf_addf(ಪುಟ_ಮಾರ್ಗ, "%s/%s.html", ಮಾರ್ಗ, ಪುಟ);
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
}

ಸ್ಥಿರ ಶೂನ್ಯ open_html(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	execl_git_cmd("web--browse", "-c", "help.browser", ಮಾರ್ಗ, (ಅಕ್ಷರ *)ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_html_ಪುಟ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ)
{
	ರಚನೆ strbuf ಪುಟ_ಮಾರ್ಗ; /* it leaks but we exec below */

	get_html_ಪುಟ_ಮಾರ್ಗ(&ಪುಟ_ಮಾರ್ಗ, ಪುಟ);

	open_html(ಪುಟ_ಮಾರ್ಗ.buf);
}

ಸ್ಥಿರ ಅಕ್ಷರ *check_git_cmd(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd)
{
	ಅಕ್ಷರ *alias;

	ಆದರೆ (is_git_command(cmd))
		ಹಿಂತಿರುಗಿ xstrdup(cmd);

	alias = alias_lookup(cmd);
	ಆದರೆ (alias) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
		ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		ಆದರೆ (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			ಬಿಡುಗಡೆ(alias);
			ನಿರ್ಗಮಿಸು(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		ಎಣಿಕೆ_2 = ವಿಭಜಿಸು_cmdline(alias, &argv);
		ಆದರೆ (ಎಣಿಕೆ_2 < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    ವಿಭಜಿಸು_cmdline_strerror(ಎಣಿಕೆ_2));
		ಬಿಡುಗಡೆ(argv);
		ಹಿಂತಿರುಗಿ alias;
	}

	ಆದರೆ (exclude_guides)
		ಹಿಂತಿರುಗಿ help_unknown_cmd(cmd);

	ಹಿಂತಿರುಗಿ xstrdup(cmd);
}

ಸ್ಥಿರ ಶೂನ್ಯ no_help_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಬಹುಲಕ, ಎಣಿಕೆ help_format fmt)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_fmt;

	ಆಯ್ಕೆ (fmt) {
	ಸ್ಥಿತಿ HELP_FORMAT_NONE:
		ಹಿಂತಿರುಗಿ;
	ಸ್ಥಿತಿ HELP_FORMAT_MAN:
		opt_fmt = "--man";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ HELP_FORMAT_INFO:
		opt_fmt = "--info";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ HELP_FORMAT_WEB:
		opt_fmt = "--web";
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unreachable");
	}

	usage_ಸಂದೇಶ_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_ಬಹುಲಕ,
		       opt_fmt);
}

ಸ್ಥಿರ ಶೂನ್ಯ opt_ಬಹುಲಕ_usage(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt_ಬಹುಲಕ,
			   ಎಣಿಕೆ help_format fmt)
{
	ಆದರೆ (argc)
		usage_ಸಂದೇಶ_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_ಬಹುಲಕ);

	no_help_format(opt_ಬಹುಲಕ, fmt);
}

ಪೂರ್ಣಾಂಕ cmd_help(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ nongit;
	ಎಣಿಕೆ help_format parsed_help_format;
	ಅಕ್ಷರ *command = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪುಟ;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	ಆದರೆ (cmd_ಬಹುಲಕ != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_ಸಂದೇಶ_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	ಆಯ್ಕೆ (cmd_ಬಹುಲಕ) {
	ಸ್ಥಿತಿ HELP_ACTION_ALL:
		opt_ಬಹುಲಕ_usage(argc, "--all", help_format);
		ಆದರೆ (verbose) {
			ಸಿದ್ಧತೆ_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			ಹಿಂತಿರುಗಿ 0;
		}
		ಬರೆ(_("usage: %s%s"), _(git_usage_string), "\n\n");
		ತುಂಬು_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		ಬರೆ("%s\n", _(git_ಇನ್ನಷ್ಟು_info_string));
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ HELP_ACTION_GUIDES:
		opt_ಬಹುಲಕ_usage(argc, "--guides", help_format);
		list_guides_help();
		ಬರೆ("%s\n", _(git_ಇನ್ನಷ್ಟು_info_string));
		ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_ಬಹುಲಕ_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ HELP_ACTION_USER_INTERFACES:
		opt_ಬಹುಲಕ_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ HELP_ACTION_DEVELOPER_INTERFACES:
		opt_ಬಹುಲಕ_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_ಬಹುಲಕ_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ HELP_ACTION_ALIASES_FOR_COMPLETION: {
		ರಚನೆ string_list alias_list = STRING_LIST_INIT_DUP;
		opt_ಬಹುಲಕ_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < alias_list.nr; i++)
			ಬರೆ("%s%c%s%c", alias_list.ವಸ್ತುಗಳು[i].string, '\n',
			       (ಅಕ್ಷರ *)alias_list.ವಸ್ತುಗಳು[i].util, '\0');
		string_list_clear(&alias_list, 1);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಸ್ಥಿತಿ HELP_ACTION_CONFIG:
		opt_ಬಹುಲಕ_usage(argc, "--config", help_format);
		ಸಿದ್ಧತೆ_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		ಬರೆ("\n%s\n", _("'git help config' for more information"));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!argv[0]) {
		ಬರೆ(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		ಬರೆ("\n%s\n", _(git_ಇನ್ನಷ್ಟು_info_string));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಸಿದ್ಧತೆ_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ಖಾಲಿ);

	ಆದರೆ (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	ಆದರೆ (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	ಪುಟ = cmd_ಗಮ್ಯ_ಪುಟ(command);
	ಆಯ್ಕೆ (help_format) {
	ಸ್ಥಿತಿ HELP_FORMAT_NONE:
	ಸ್ಥಿತಿ HELP_FORMAT_MAN:
		show_man_ಪುಟ(ಪುಟ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ HELP_FORMAT_INFO:
		show_info_ಪುಟ(ಪುಟ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ HELP_FORMAT_WEB:
		show_html_ಪುಟ(ಪುಟ);
		ನಿಲ್ಲಿಸು;
	}

	ಬಿಡುಗಡೆ(command);
	ಹಿಂತಿರುಗಿ 0;
}
