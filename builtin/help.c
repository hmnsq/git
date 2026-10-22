/*
 * Builtin help command
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "exec-cmd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "help.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "alias.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DEFAULT_HELP_FORMAT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_HELP_FORMAT "man"
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ man_viewer_list {
	ਬਣਤਰ man_viewer_list *ਅਗਲਾ;
	ਅੱਖਰ name[FLEX_ARRAY];
} *man_viewer_list;

ਸਥਿਰ ਬਣਤਰ man_viewer_info_list {
	ਬਣਤਰ man_viewer_info_list *ਅਗਲਾ;
	ਅਚਲ ਅੱਖਰ *info;
	ਅੱਖਰ name[FLEX_ARRAY];
} *man_viewer_info_list;

ਗਿਣਤੀ help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

ਗਿਣਤੀ show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ਸਥਿਰ ਗਿਣਤੀ help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_ਬਹੁਲਕ;

ਸਥਿਰ ਅੱਖਰ *html_ਰਸਤਾ;
ਸਥਿਰ ਪੂਰਨ verbose = 1;
ਸਥਿਰ ਗਿਣਤੀ help_format help_format = HELP_FORMAT_NONE;
ਸਥਿਰ ਪੂਰਨ exclude_guides;
ਸਥਿਰ ਪੂਰਨ show_external_commands = -1;
ਸਥਿਰ ਪੂਰਨ show_aliases = -1;
ਸਥਿਰ ਬਣਤਰ option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_ਬਹੁਲਕ, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_ਸੈੱਟ_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_ਸੈੱਟ_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_ਸੈੱਟ_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_ਬਹੁਲਕ, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_ਬਹੁਲਕ,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_ਬਹੁਲਕ,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_ਬਹੁਲਕ, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_ਬਹੁਲਕ, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_ਬਹੁਲਕ, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_ਬਹੁਲਕ, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_ਸਿਰਾ(),
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ਨਿਰਾ
};

ਬਣਤਰ ਥਾਂ_expansion {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਅਚਲ ਅੱਖਰ *placeholder;
	ਖਾਲੀ (*fn)(ਬਣਤਰ string_list *list, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ);
	ਪੂਰਨ ਮਿਲਿਆ_2;
};

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_config_vars(ਬਣਤਰ string_list *ਕੁੰਜੀਆਂ_uniq, ਬਣਤਰ string_list_ਵਸਤੂ *var)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *str = var->string;
	ਅਚਲ ਅੱਖਰ *wildcard = strchr(str, '*');
	ਅਚਲ ਅੱਖਰ *ਟੈਗ = strchr(str, '<');
	ਅਚਲ ਅੱਖਰ *cut;

	ਜੇ (wildcard && ਟੈਗ)
		cut = wildcard < ਟੈਗ ? wildcard : ਟੈਗ;
	ਨਹੀਂਤਾਂ ਜੇ (wildcard)
		cut = wildcard;
	ਨਹੀਂਤਾਂ ਜੇ (ਟੈਗ)
		cut = ਟੈਗ;
	ਨਹੀਂਤਾਂ {
		string_list_append(ਕੁੰਜੀਆਂ_uniq, str);
		ਵਾਪਸ;
	}

	strbuf_ਜੋੜੋ(&sb, str, cut - str);
	string_list_append(ਕੁੰਜੀਆਂ_uniq, sb.buf);
	strbuf_release(&sb);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_config_sections(ਬਣਤਰ string_list *ਕੁੰਜੀਆਂ_uniq, ਬਣਤਰ string_list_ਵਸਤੂ *var)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *str = var->string;
	ਅਚਲ ਅੱਖਰ *dot = strchr(str, '.');
	ਅਚਲ ਅੱਖਰ *cut;

	ਜੇ (dot)
		cut = dot;
	ਨਹੀਂਤਾਂ {
		ਸੈੱਟ_config_vars(ਕੁੰਜੀਆਂ_uniq, var);
		ਵਾਪਸ;
	}

	strbuf_ਜੋੜੋ(&sb, str, cut - str);
	string_list_append(ਕੁੰਜੀਆਂ_uniq, sb.buf);
	strbuf_release(&sb);
}

ਸਥਿਰ ਖਾਲੀ list_config_help(ਗਿਣਤੀ show_config_type type)
{
	ਬਣਤਰ ਥਾਂ_expansion ਥਾਂ_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_ਸਥਿਤੀ_slots },
		{ "fsck", "<msg-id>", list_config_fsck_ਸੁਨੇਹਾ_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_ਸੁਨੇਹਾ_ids },
		{ ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ }
	};
	ਅਚਲ ਅੱਖਰ **p;
	ਬਣਤਰ ਥਾਂ_expansion *e;
	ਬਣਤਰ string_list ਕੁੰਜੀਆਂ = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list ਕੁੰਜੀਆਂ_uniq = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਲਈ (p = config_name_list; *p; p++) {
		ਅਚਲ ਅੱਖਰ *var = *p;

		ਲਈ (e = ਥਾਂ_expansions; e->ਅਗੇਤਰ; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->ਅਗੇਤਰ, e->placeholder);
			ਜੇ (!strcasecmp(var, sb.buf)) {
				e->fn(&ਕੁੰਜੀਆਂ, e->ਅਗੇਤਰ);
				e->ਮਿਲਿਆ_2++;
				ਰੋਕੋ;
			}
		}

		ਜੇ (!e->ਅਗੇਤਰ)
			string_list_append(&ਕੁੰਜੀਆਂ, var);
	}

	strbuf_release(&sb);

	ਲਈ (e = ਥਾਂ_expansions; e->ਅਗੇਤਰ; e++)
		ਜੇ (!e->ਮਿਲਿਆ_2)
			BUG("slot_expansion %s.%s is not used",
			    e->ਅਗੇਤਰ, e->placeholder);

	ਲਈ (ਮਾਪ_t i = 0; i < ਕੁੰਜੀਆਂ.nr; i++) {
		ਚੋਣ (type) {
		ਹਾਲਤ SHOW_CONFIG_HUMAN:
			string_list_append(&ਕੁੰਜੀਆਂ_uniq, ਕੁੰਜੀਆਂ.ਚੀਜ਼ਾਂ[i].string);
			ਰੋਕੋ;
		ਹਾਲਤ SHOW_CONFIG_SECTIONS:
			ਸੈੱਟ_config_sections(&ਕੁੰਜੀਆਂ_uniq, &ਕੁੰਜੀਆਂ.ਚੀਜ਼ਾਂ[i]);
			ਰੋਕੋ;
		ਹਾਲਤ SHOW_CONFIG_VARS:
			ਸੈੱਟ_config_vars(&ਕੁੰਜੀਆਂ_uniq, &ਕੁੰਜੀਆਂ.ਚੀਜ਼ਾਂ[i]);
			ਰੋਕੋ;
		ਮੂਲ:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_ਛਾਂਟੋ_u(&ਕੁੰਜੀਆਂ_uniq, 0);
	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਕੁੰਜੀਆਂ_uniq)
		puts(ਵਸਤੂ->string);
	string_list_clear(&ਕੁੰਜੀਆਂ_uniq, 0);
	string_list_clear(&ਕੁੰਜੀਆਂ, 0);
}

ਸਥਿਰ ਗਿਣਤੀ help_format parse_help_format(ਅਚਲ ਅੱਖਰ *format)
{
	ਜੇ (!strcmp(format, "man"))
		ਵਾਪਸ HELP_FORMAT_MAN;
	ਜੇ (!strcmp(format, "info"))
		ਵਾਪਸ HELP_FORMAT_INFO;
	ਜੇ (!strcmp(format, "web") || !strcmp(format, "html"))
		ਵਾਪਸ HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_man_viewer_info(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ man_viewer_info_list *viewer;

	ਲਈ (viewer = man_viewer_info_list; viewer; viewer = viewer->ਅਗਲਾ)
	{
		ਜੇ (!strcasecmp(name, viewer->name))
			ਵਾਪਸ viewer->info;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ check_emacsclient_ਸੰਸਕਰਣ(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf ਬਫਰ = STRBUF_INIT;
	ਬਣਤਰ ਬੱਚਾ_process ec_process = ਬੱਚਾ_PROCESS_INIT;
	ਪੂਰਨ ਸੰਸਕਰਣ;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.ਦਲੀਲਾਂ, "emacsclient", "--version", ਨਿਰਾ);
	ec_process.err = -1;
	ec_process.stdout_ਮੰਜ਼ਿਲ_stderr = 1;
	ਜੇ (ਸ਼ੁਰੂ_command(&ec_process))
		ਵਾਪਸ error(_("Failed to start emacsclient."));

	strbuf_read(&ਬਫਰ, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	ਅੰਤ_command(&ec_process);

	ਜੇ (!starts_with(ਬਫਰ.buf, "emacsclient")) {
		strbuf_release(&ਬਫਰ);
		ਵਾਪਸ error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&ਬਫਰ, 0, ਲੜੀ_ਲੰਬਾਈ("emacsclient"));
	ਸੰਸਕਰਣ = atoi(ਬਫਰ.buf);

	ਜੇ (ਸੰਸਕਰਣ < 22) {
		strbuf_release(&ਬਫਰ);
		ਵਾਪਸ error(_("emacsclient version '%d' too old (< 22)."),
			ਸੰਸਕਰਣ);
	}

	strbuf_release(&ਬਫਰ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ exec_woman_emacs(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਜੇ (!check_emacsclient_ਸੰਸਕਰਣ()) {
		/* This works only with emacsclient version >= 22. */
		ਬਣਤਰ strbuf man_ਪੰਨਾ = STRBUF_INIT;

		ਜੇ (!ਰਸਤਾ)
			ਰਸਤਾ = "emacsclient";
		strbuf_addf(&man_ਪੰਨਾ, "(woman \"%s\")", ਪੰਨਾ);
		execlp(ਰਸਤਾ, "emacsclient", "-e", man_ਪੰਨਾ.buf, (ਅੱਖਰ *)ਨਿਰਾ);
		warning_errno(_("failed to exec '%s'"), ਰਸਤਾ);
		strbuf_release(&man_ਪੰਨਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ exec_man_konqueror(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਅਚਲ ਅੱਖਰ *display = getenv("DISPLAY");
	ਜੇ (display && *display) {
		ਬਣਤਰ strbuf man_ਪੰਨਾ = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		ਜੇ (ਰਸਤਾ) {
			ਮਾਪ_t len;
			ਜੇ (strip_suffix(ਰਸਤਾ, "/konqueror", &len))
				ਰਸਤਾ = xstrfmt("%.*s/kfmclient", (ਪੂਰਨ)len, ਰਸਤਾ);
			filename = basename((ਅੱਖਰ *)ਰਸਤਾ);
		} ਨਹੀਂਤਾਂ
			ਰਸਤਾ = "kfmclient";
		strbuf_addf(&man_ਪੰਨਾ, "man:%s(1)", ਪੰਨਾ);
		execlp(ਰਸਤਾ, filename, "newTab", man_ਪੰਨਾ.buf, (ਅੱਖਰ *)ਨਿਰਾ);
		warning_errno(_("failed to exec '%s'"), ਰਸਤਾ);
		strbuf_release(&man_ਪੰਨਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ exec_man_man(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਜੇ (!ਰਸਤਾ)
		ਰਸਤਾ = "man";
	execlp(ਰਸਤਾ, "man", ਪੰਨਾ, (ਅੱਖਰ *)ਨਿਰਾ);
	warning_errno(_("failed to exec '%s'"), ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ exec_man_cmd(ਅਚਲ ਅੱਖਰ *cmd, ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਬਣਤਰ strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, ਪੰਨਾ);
	execl(SHELL_ਰਸਤਾ, SHELL_ਰਸਤਾ, "-c", shell_cmd.buf, (ਅੱਖਰ *)ਨਿਰਾ);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_man_viewer(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ man_viewer_list **p = &man_viewer_list;

	ਜਦੋਂ (*p)
		p = &((*p)->ਅਗਲਾ);
	FLEX_ALLOC_STR(*p, name, name);
}

ਸਥਿਰ ਪੂਰਨ supported_man_viewer(ਅਚਲ ਅੱਖਰ *name, ਮਾਪ_t len)
{
	ਵਾਪਸ (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ਸਥਿਰ ਖਾਲੀ do_ਜੋੜੋ_man_viewer_info(ਅਚਲ ਅੱਖਰ *name,
				   ਮਾਪ_t len,
				   ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਬਣਤਰ man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(ਮੁੱਲ);
	new_man_viewer->ਅਗਲਾ = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_man_viewer_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *name,
			       ਮਾਪ_t len,
			       ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (supported_man_viewer(name, len))
		do_ਜੋੜੋ_man_viewer_info(name, len, ਮੁੱਲ);
	ਨਹੀਂਤਾਂ
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_man_viewer_cmd(ਅਚਲ ਅੱਖਰ *name,
			      ਮਾਪ_t len,
			      ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਜੇ (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	ਨਹੀਂਤਾਂ
		do_ਜੋੜੋ_man_viewer_info(name, len, ਮੁੱਲ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_man_viewer_info(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਅਚਲ ਅੱਖਰ *name, *subkey;
	ਮਾਪ_t namelen;

	ਜੇ (parse_config_ਕੁੰਜੀ(var, "man", &name, &namelen, &subkey) < 0 || !name)
		ਵਾਪਸ 0;

	ਜੇ (!strcmp(subkey, "path")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਵਾਪਸ ਜੋੜੋ_man_viewer_ਰਸਤਾ(name, namelen, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(subkey, "cmd")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਵਾਪਸ ਜੋੜੋ_man_viewer_cmd(name, namelen, ਮੁੱਲ);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ git_help_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			   ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "help.format")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		help_format = parse_help_format(ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "help.htmlpath")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਮੁਕਤ(html_ਰਸਤਾ);
		html_ਰਸਤਾ = xstrdup(ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "man.viewer")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਜੋੜੋ_man_viewer(ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (starts_with(var, "man."))
		ਵਾਪਸ ਜੋੜੋ_man_viewer_info(var, ਮੁੱਲ);

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਬਣਤਰ cmdnames main_cmds, other_cmds;

ਸਥਿਰ ਪੂਰਨ is_git_command(ਅਚਲ ਅੱਖਰ *s)
{
	ਜੇ (is_builtin(s))
		ਵਾਪਸ 1;

	ਲੱਦੋ_command_list("git-", &main_cmds, &other_cmds);
	ਵਾਪਸ is_ਵਿੱਚ_cmdlist(&main_cmds, s) ||
		is_ਵਿੱਚ_cmdlist(&other_cmds, s);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *cmd_ਮੰਜ਼ਿਲ_ਪੰਨਾ(ਅਚਲ ਅੱਖਰ *git_cmd)
{
	ਜੇ (!git_cmd)
		ਵਾਪਸ "git";
	ਨਹੀਂਤਾਂ ਜੇ (starts_with(git_cmd, "git"))
		ਵਾਪਸ git_cmd;
	ਨਹੀਂਤਾਂ ਜੇ (is_git_command(git_cmd))
		ਵਾਪਸ xstrfmt("git-%s", git_cmd);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp("scalar", git_cmd))
		ਵਾਪਸ xstrdup(git_cmd);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ xstrfmt("git%s", git_cmd);
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_man_ਰਸਤਾ(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf new_ਰਸਤਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *old_ਰਸਤਾ = getenv("MANPATH");
	ਅੱਖਰ *git_man_ਰਸਤਾ = system_ਰਸਤਾ(GIT_MAN_ਰਸਤਾ);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_ਰਸਤਾ, git_man_ਰਸਤਾ);
	strbuf_addch(&new_ਰਸਤਾ, ':');
	ਜੇ (old_ਰਸਤਾ)
		strbuf_addstr(&new_ਰਸਤਾ, old_ਰਸਤਾ);

	ਮੁਕਤ(git_man_ਰਸਤਾ);
	setenv("MANPATH", new_ਰਸਤਾ.buf, 1);

	strbuf_release(&new_ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ exec_viewer(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਅਚਲ ਅੱਖਰ *info = get_man_viewer_info(name);

	ਜੇ (!strcasecmp(name, "man"))
		exec_man_man(info, ਪੰਨਾ);
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, ਪੰਨਾ);
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, ਪੰਨਾ);
	ਨਹੀਂਤਾਂ ਜੇ (info)
		exec_man_cmd(info, ਪੰਨਾ);
	ਨਹੀਂਤਾਂ
		warning(_("'%s': unknown man viewer."), name);
}

ਸਥਿਰ ਖਾਲੀ show_man_ਪੰਨਾ(ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਬਣਤਰ man_viewer_list *viewer;
	ਅਚਲ ਅੱਖਰ *fallback = getenv("GIT_MAN_VIEWER");

	ਤਿਆਰੀ_man_ਰਸਤਾ();
	ਲਈ (viewer = man_viewer_list; viewer; viewer = viewer->ਅਗਲਾ)
	{
		exec_viewer(viewer->name, ਪੰਨਾ); /* will return when unable */
	}
	ਜੇ (fallback)
		exec_viewer(fallback, ਪੰਨਾ);
	exec_viewer("man", ਪੰਨਾ);
	die(_("no man viewer handled the request"));
}

ਸਥਿਰ ਖਾਲੀ show_info_ਪੰਨਾ(ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	setenv("INFOPATH", system_ਰਸਤਾ(GIT_INFO_ਰਸਤਾ), 1);
	execlp("info", "info", "gitman", ਪੰਨਾ, (ਅੱਖਰ *)ਨਿਰਾ);
	die(_("no info viewer handled the request"));
}

ਸਥਿਰ ਖਾਲੀ get_html_ਪੰਨਾ_ਰਸਤਾ(ਬਣਤਰ strbuf *ਪੰਨਾ_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਬਣਤਰ stat st;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = html_ਰਸਤਾ;
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;

	ਜੇ (!ਰਸਤਾ)
		ਰਸਤਾ = ਮੰਜ਼ਿਲ_free = system_ਰਸਤਾ(GIT_HTML_ਰਸਤਾ);

	/*
	 * Check that the page we're looking for exists.
	 */
	ਜੇ (!ਲੜੀ_ਖੋਜ(ਰਸਤਾ, "://")) {
		ਜੇ (stat(mkpath("%s/%s.html", ਰਸਤਾ, ਪੰਨਾ), &st)
		    || !S_ISREG(st.st_ਬਹੁਲਕ))
			die("'%s/%s.html': documentation file not found.",
				ਰਸਤਾ, ਪੰਨਾ);
	}

	strbuf_init(ਪੰਨਾ_ਰਸਤਾ, 0);
	strbuf_addf(ਪੰਨਾ_ਰਸਤਾ, "%s/%s.html", ਰਸਤਾ, ਪੰਨਾ);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
}

ਸਥਿਰ ਖਾਲੀ open_html(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	execl_git_cmd("web--browse", "-c", "help.browser", ਰਸਤਾ, (ਅੱਖਰ *)ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ show_html_ਪੰਨਾ(ਅਚਲ ਅੱਖਰ *ਪੰਨਾ)
{
	ਬਣਤਰ strbuf ਪੰਨਾ_ਰਸਤਾ; /* it leaks but we exec below */

	get_html_ਪੰਨਾ_ਰਸਤਾ(&ਪੰਨਾ_ਰਸਤਾ, ਪੰਨਾ);

	open_html(ਪੰਨਾ_ਰਸਤਾ.buf);
}

ਸਥਿਰ ਅੱਖਰ *check_git_cmd(ਅਚਲ ਅੱਖਰ *cmd)
{
	ਅੱਖਰ *alias;

	ਜੇ (is_git_command(cmd))
		ਵਾਪਸ xstrdup(cmd);

	alias = alias_lookup(cmd);
	ਜੇ (alias) {
		ਅਚਲ ਅੱਖਰ **argv;
		ਪੂਰਨ ਗਿਣਤੀ_2;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		ਜੇ (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			ਮੁਕਤ(alias);
			ਬਾਹਰ_ਨਿਕਲੋ(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		ਗਿਣਤੀ_2 = ਵੰਡੋ_cmdline(alias, &argv);
		ਜੇ (ਗਿਣਤੀ_2 < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    ਵੰਡੋ_cmdline_strerror(ਗਿਣਤੀ_2));
		ਮੁਕਤ(argv);
		ਵਾਪਸ alias;
	}

	ਜੇ (exclude_guides)
		ਵਾਪਸ help_unknown_cmd(cmd);

	ਵਾਪਸ xstrdup(cmd);
}

ਸਥਿਰ ਖਾਲੀ no_help_format(ਅਚਲ ਅੱਖਰ *opt_ਬਹੁਲਕ, ਗਿਣਤੀ help_format fmt)
{
	ਅਚਲ ਅੱਖਰ *opt_fmt;

	ਚੋਣ (fmt) {
	ਹਾਲਤ HELP_FORMAT_NONE:
		ਵਾਪਸ;
	ਹਾਲਤ HELP_FORMAT_MAN:
		opt_fmt = "--man";
		ਰੋਕੋ;
	ਹਾਲਤ HELP_FORMAT_INFO:
		opt_fmt = "--info";
		ਰੋਕੋ;
	ਹਾਲਤ HELP_FORMAT_WEB:
		opt_fmt = "--web";
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unreachable");
	}

	usage_ਸੁਨੇਹਾ_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_ਬਹੁਲਕ,
		       opt_fmt);
}

ਸਥਿਰ ਖਾਲੀ opt_ਬਹੁਲਕ_usage(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ *opt_ਬਹੁਲਕ,
			   ਗਿਣਤੀ help_format fmt)
{
	ਜੇ (argc)
		usage_ਸੁਨੇਹਾ_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_ਬਹੁਲਕ);

	no_help_format(opt_ਬਹੁਲਕ, fmt);
}

ਪੂਰਨ cmd_help(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ nongit;
	ਗਿਣਤੀ help_format parsed_help_format;
	ਅੱਖਰ *command = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਪੰਨਾ;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	ਜੇ (cmd_ਬਹੁਲਕ != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_ਸੁਨੇਹਾ_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	ਚੋਣ (cmd_ਬਹੁਲਕ) {
	ਹਾਲਤ HELP_ACTION_ALL:
		opt_ਬਹੁਲਕ_usage(argc, "--all", help_format);
		ਜੇ (verbose) {
			ਤਿਆਰੀ_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			ਵਾਪਸ 0;
		}
		ਲਿਖੋ(_("usage: %s%s"), _(git_usage_string), "\n\n");
		ਲੱਦੋ_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		ਲਿਖੋ("%s\n", _(git_ਹੋਰ_info_string));
		ਰੋਕੋ;
	ਹਾਲਤ HELP_ACTION_GUIDES:
		opt_ਬਹੁਲਕ_usage(argc, "--guides", help_format);
		list_guides_help();
		ਲਿਖੋ("%s\n", _(git_ਹੋਰ_info_string));
		ਵਾਪਸ 0;
	ਹਾਲਤ HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_ਬਹੁਲਕ_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		ਵਾਪਸ 0;
	ਹਾਲਤ HELP_ACTION_USER_INTERFACES:
		opt_ਬਹੁਲਕ_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		ਵਾਪਸ 0;
	ਹਾਲਤ HELP_ACTION_DEVELOPER_INTERFACES:
		opt_ਬਹੁਲਕ_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		ਵਾਪਸ 0;
	ਹਾਲਤ HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_ਬਹੁਲਕ_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		ਵਾਪਸ 0;
	ਹਾਲਤ HELP_ACTION_ALIASES_FOR_COMPLETION: {
		ਬਣਤਰ string_list alias_list = STRING_LIST_INIT_DUP;
		opt_ਬਹੁਲਕ_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		ਲਈ (ਮਾਪ_t i = 0; i < alias_list.nr; i++)
			ਲਿਖੋ("%s%c%s%c", alias_list.ਚੀਜ਼ਾਂ[i].string, '\n',
			       (ਅੱਖਰ *)alias_list.ਚੀਜ਼ਾਂ[i].util, '\0');
		string_list_clear(&alias_list, 1);
		ਵਾਪਸ 0;
	}
	ਹਾਲਤ HELP_ACTION_CONFIG:
		opt_ਬਹੁਲਕ_usage(argc, "--config", help_format);
		ਤਿਆਰੀ_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		ਲਿਖੋ("\n%s\n", _("'git help config' for more information"));
		ਵਾਪਸ 0;
	}

	ਜੇ (!argv[0]) {
		ਲਿਖੋ(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		ਲਿਖੋ("\n%s\n", _(git_ਹੋਰ_info_string));
		ਵਾਪਸ 0;
	}

	ਤਿਆਰੀ_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ਨਿਰਾ);

	ਜੇ (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	ਜੇ (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	ਪੰਨਾ = cmd_ਮੰਜ਼ਿਲ_ਪੰਨਾ(command);
	ਚੋਣ (help_format) {
	ਹਾਲਤ HELP_FORMAT_NONE:
	ਹਾਲਤ HELP_FORMAT_MAN:
		show_man_ਪੰਨਾ(ਪੰਨਾ);
		ਰੋਕੋ;
	ਹਾਲਤ HELP_FORMAT_INFO:
		show_info_ਪੰਨਾ(ਪੰਨਾ);
		ਰੋਕੋ;
	ਹਾਲਤ HELP_FORMAT_WEB:
		show_html_ਪੰਨਾ(ਪੰਨਾ);
		ਰੋਕੋ;
	}

	ਮੁਕਤ(command);
	ਵਾਪਸ 0;
}
