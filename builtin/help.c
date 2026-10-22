/*
 * Builtin help command
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "exec-cmd.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "config-list.h"
#అన్తర్భావ "help.h"
#అన్తర్భావ "alias.h"
#అన్తర్భావ "setup.h"

#నిర్వచితం_కాకపోతే DEFAULT_HELP_FORMAT
#నిర్వచించు DEFAULT_HELP_FORMAT "man"
#అయితే_ముగింపు

స్థిర నిర్మాణం man_viewer_list {
	నిర్మాణం man_viewer_list *తదుపరి;
	అక్షరం name[FLEX_ARRAY];
} *man_viewer_list;

స్థిర నిర్మాణం man_viewer_info_list {
	నిర్మాణం man_viewer_info_list *తదుపరి;
	స్థిరరాశి అక్షరం *info;
	అక్షరం name[FLEX_ARRAY];
} *man_viewer_info_list;

గణన help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

గణన show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

స్థిర గణన help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_బహుళకం;

స్థిర అక్షరం *html_మార్గం;
స్థిర పూర్ణాంకం verbose = 1;
స్థిర గణన help_format help_format = HELP_FORMAT_NONE;
స్థిర పూర్ణాంకం exclude_guides;
స్థిర పూర్ణాంకం show_external_commands = -1;
స్థిర పూర్ణాంకం show_aliases = -1;
స్థిర నిర్మాణం option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_బహుళకం, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_సమితి_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_సమితి_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_సమితి_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_బహుళకం, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_బహుళకం,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_బహుళకం,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_బహుళకం, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_బహుళకం, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_బహుళకం, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_బహుళకం, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_చివర(),
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ఖాళీ
};

నిర్మాణం స్థానం_expansion {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *placeholder;
	శూన్య (*fn)(నిర్మాణం string_list *list, స్థిరరాశి అక్షరం *ఉపసర్గ);
	పూర్ణాంకం దొరికింది_2;
};

స్థిర శూన్య సమితి_config_vars(నిర్మాణం string_list *కీలకాలు_uniq, నిర్మాణం string_list_అంశం *var)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *str = var->string;
	స్థిరరాశి అక్షరం *wildcard = strchr(str, '*');
	స్థిరరాశి అక్షరం *ట్యాగ్ = strchr(str, '<');
	స్థిరరాశి అక్షరం *cut;

	ఒకవేళ (wildcard && ట్యాగ్)
		cut = wildcard < ట్యాగ్ ? wildcard : ట్యాగ్;
	లేకపోతే ఒకవేళ (wildcard)
		cut = wildcard;
	లేకపోతే ఒకవేళ (ట్యాగ్)
		cut = ట్యాగ్;
	లేకపోతే {
		string_list_append(కీలకాలు_uniq, str);
		తిరిగి;
	}

	strbuf_కూడు(&sb, str, cut - str);
	string_list_append(కీలకాలు_uniq, sb.buf);
	strbuf_release(&sb);
}

స్థిర శూన్య సమితి_config_sections(నిర్మాణం string_list *కీలకాలు_uniq, నిర్మాణం string_list_అంశం *var)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *str = var->string;
	స్థిరరాశి అక్షరం *dot = strchr(str, '.');
	స్థిరరాశి అక్షరం *cut;

	ఒకవేళ (dot)
		cut = dot;
	లేకపోతే {
		సమితి_config_vars(కీలకాలు_uniq, var);
		తిరిగి;
	}

	strbuf_కూడు(&sb, str, cut - str);
	string_list_append(కీలకాలు_uniq, sb.buf);
	strbuf_release(&sb);
}

స్థిర శూన్య list_config_help(గణన show_config_type type)
{
	నిర్మాణం స్థానం_expansion స్థానం_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_స్థితి_slots },
		{ "fsck", "<msg-id>", list_config_fsck_సందేశం_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_సందేశం_ids },
		{ ఖాళీ, ఖాళీ, ఖాళీ }
	};
	స్థిరరాశి అక్షరం **p;
	నిర్మాణం స్థానం_expansion *e;
	నిర్మాణం string_list కీలకాలు = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list కీలకాలు_uniq = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	కొరకు (p = config_name_list; *p; p++) {
		స్థిరరాశి అక్షరం *var = *p;

		కొరకు (e = స్థానం_expansions; e->ఉపసర్గ; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->ఉపసర్గ, e->placeholder);
			ఒకవేళ (!strcasecmp(var, sb.buf)) {
				e->fn(&కీలకాలు, e->ఉపసర్గ);
				e->దొరికింది_2++;
				ఆపు;
			}
		}

		ఒకవేళ (!e->ఉపసర్గ)
			string_list_append(&కీలకాలు, var);
	}

	strbuf_release(&sb);

	కొరకు (e = స్థానం_expansions; e->ఉపసర్గ; e++)
		ఒకవేళ (!e->దొరికింది_2)
			BUG("slot_expansion %s.%s is not used",
			    e->ఉపసర్గ, e->placeholder);

	కొరకు (కొలత_t i = 0; i < కీలకాలు.nr; i++) {
		ఎంపిక (type) {
		స్థితి SHOW_CONFIG_HUMAN:
			string_list_append(&కీలకాలు_uniq, కీలకాలు.వస్తువులు[i].string);
			ఆపు;
		స్థితి SHOW_CONFIG_SECTIONS:
			సమితి_config_sections(&కీలకాలు_uniq, &కీలకాలు.వస్తువులు[i]);
			ఆపు;
		స్థితి SHOW_CONFIG_VARS:
			సమితి_config_vars(&కీలకాలు_uniq, &కీలకాలు.వస్తువులు[i]);
			ఆపు;
		అప్రమేయ:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_క్రమీకరించు_u(&కీలకాలు_uniq, 0);
	for_each_string_list_అంశం(అంశం, &కీలకాలు_uniq)
		puts(అంశం->string);
	string_list_clear(&కీలకాలు_uniq, 0);
	string_list_clear(&కీలకాలు, 0);
}

స్థిర గణన help_format parse_help_format(స్థిరరాశి అక్షరం *format)
{
	ఒకవేళ (!strcmp(format, "man"))
		తిరిగి HELP_FORMAT_MAN;
	ఒకవేళ (!strcmp(format, "info"))
		తిరిగి HELP_FORMAT_INFO;
	ఒకవేళ (!strcmp(format, "web") || !strcmp(format, "html"))
		తిరిగి HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

స్థిర స్థిరరాశి అక్షరం *get_man_viewer_info(స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం man_viewer_info_list *viewer;

	కొరకు (viewer = man_viewer_info_list; viewer; viewer = viewer->తదుపరి)
	{
		ఒకవేళ (!strcasecmp(name, viewer->name))
			తిరిగి viewer->info;
	}
	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం check_emacsclient_సంస్కరణ(శూన్య)
{
	నిర్మాణం strbuf బఫరు = STRBUF_INIT;
	నిర్మాణం బిడ్డ_process ec_process = బిడ్డ_PROCESS_INIT;
	పూర్ణాంకం సంస్కరణ;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.వాదనలు, "emacsclient", "--version", ఖాళీ);
	ec_process.err = -1;
	ec_process.stdout_గమ్యం_stderr = 1;
	ఒకవేళ (ప్రారంభం_command(&ec_process))
		తిరిగి error(_("Failed to start emacsclient."));

	strbuf_read(&బఫరు, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	ముగింపు_command(&ec_process);

	ఒకవేళ (!starts_with(బఫరు.buf, "emacsclient")) {
		strbuf_release(&బఫరు);
		తిరిగి error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&బఫరు, 0, అక్షరశ్రేణి_పొడవు("emacsclient"));
	సంస్కరణ = atoi(బఫరు.buf);

	ఒకవేళ (సంస్కరణ < 22) {
		strbuf_release(&బఫరు);
		తిరిగి error(_("emacsclient version '%d' too old (< 22)."),
			సంస్కరణ);
	}

	strbuf_release(&బఫరు);
	తిరిగి 0;
}

స్థిర శూన్య exec_woman_emacs(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *పుట)
{
	ఒకవేళ (!check_emacsclient_సంస్కరణ()) {
		/* This works only with emacsclient version >= 22. */
		నిర్మాణం strbuf man_పుట = STRBUF_INIT;

		ఒకవేళ (!మార్గం)
			మార్గం = "emacsclient";
		strbuf_addf(&man_పుట, "(woman \"%s\")", పుట);
		execlp(మార్గం, "emacsclient", "-e", man_పుట.buf, (అక్షరం *)ఖాళీ);
		warning_errno(_("failed to exec '%s'"), మార్గం);
		strbuf_release(&man_పుట);
	}
}

స్థిర శూన్య exec_man_konqueror(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *పుట)
{
	స్థిరరాశి అక్షరం *display = getenv("DISPLAY");
	ఒకవేళ (display && *display) {
		నిర్మాణం strbuf man_పుట = STRBUF_INIT;
		స్థిరరాశి అక్షరం *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		ఒకవేళ (మార్గం) {
			కొలత_t len;
			ఒకవేళ (strip_suffix(మార్గం, "/konqueror", &len))
				మార్గం = xstrfmt("%.*s/kfmclient", (పూర్ణాంకం)len, మార్గం);
			filename = basename((అక్షరం *)మార్గం);
		} లేకపోతే
			మార్గం = "kfmclient";
		strbuf_addf(&man_పుట, "man:%s(1)", పుట);
		execlp(మార్గం, filename, "newTab", man_పుట.buf, (అక్షరం *)ఖాళీ);
		warning_errno(_("failed to exec '%s'"), మార్గం);
		strbuf_release(&man_పుట);
	}
}

స్థిర శూన్య exec_man_man(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *పుట)
{
	ఒకవేళ (!మార్గం)
		మార్గం = "man";
	execlp(మార్గం, "man", పుట, (అక్షరం *)ఖాళీ);
	warning_errno(_("failed to exec '%s'"), మార్గం);
}

స్థిర శూన్య exec_man_cmd(స్థిరరాశి అక్షరం *cmd, స్థిరరాశి అక్షరం *పుట)
{
	నిర్మాణం strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, పుట);
	execl(SHELL_మార్గం, SHELL_మార్గం, "-c", shell_cmd.buf, (అక్షరం *)ఖాళీ);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

స్థిర శూన్య కూడు_man_viewer(స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం man_viewer_list **p = &man_viewer_list;

	అయితే (*p)
		p = &((*p)->తదుపరి);
	FLEX_ALLOC_STR(*p, name, name);
}

స్థిర పూర్ణాంకం supported_man_viewer(స్థిరరాశి అక్షరం *name, కొలత_t len)
{
	తిరిగి (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

స్థిర శూన్య do_కూడు_man_viewer_info(స్థిరరాశి అక్షరం *name,
				   కొలత_t len,
				   స్థిరరాశి అక్షరం *విలువ)
{
	నిర్మాణం man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(విలువ);
	new_man_viewer->తదుపరి = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

స్థిర పూర్ణాంకం కూడు_man_viewer_మార్గం(స్థిరరాశి అక్షరం *name,
			       కొలత_t len,
			       స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (supported_man_viewer(name, len))
		do_కూడు_man_viewer_info(name, len, విలువ);
	లేకపోతే
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం కూడు_man_viewer_cmd(స్థిరరాశి అక్షరం *name,
			      కొలత_t len,
			      స్థిరరాశి అక్షరం *విలువ)
{
	ఒకవేళ (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	లేకపోతే
		do_కూడు_man_viewer_info(name, len, విలువ);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం కూడు_man_viewer_info(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ)
{
	స్థిరరాశి అక్షరం *name, *subkey;
	కొలత_t namelen;

	ఒకవేళ (parse_config_కీలకం(var, "man", &name, &namelen, &subkey) < 0 || !name)
		తిరిగి 0;

	ఒకవేళ (!strcmp(subkey, "path")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		తిరిగి కూడు_man_viewer_మార్గం(name, namelen, విలువ);
	}
	ఒకవేళ (!strcmp(subkey, "cmd")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		తిరిగి కూడు_man_viewer_cmd(name, namelen, విలువ);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం git_help_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			   స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "help.format")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		help_format = parse_help_format(విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "help.htmlpath")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		విడుదల(html_మార్గం);
		html_మార్గం = xstrdup(విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "man.viewer")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		కూడు_man_viewer(విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (starts_with(var, "man."))
		తిరిగి కూడు_man_viewer_info(var, విలువ);

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర నిర్మాణం cmdnames main_cmds, other_cmds;

స్థిర పూర్ణాంకం is_git_command(స్థిరరాశి అక్షరం *s)
{
	ఒకవేళ (is_builtin(s))
		తిరిగి 1;

	ఎక్కించు_command_list("git-", &main_cmds, &other_cmds);
	తిరిగి is_లోపల_cmdlist(&main_cmds, s) ||
		is_లోపల_cmdlist(&other_cmds, s);
}

స్థిర స్థిరరాశి అక్షరం *cmd_గమ్యం_పుట(స్థిరరాశి అక్షరం *git_cmd)
{
	ఒకవేళ (!git_cmd)
		తిరిగి "git";
	లేకపోతే ఒకవేళ (starts_with(git_cmd, "git"))
		తిరిగి git_cmd;
	లేకపోతే ఒకవేళ (is_git_command(git_cmd))
		తిరిగి xstrfmt("git-%s", git_cmd);
	లేకపోతే ఒకవేళ (!strcmp("scalar", git_cmd))
		తిరిగి xstrdup(git_cmd);
	లేకపోతే
		తిరిగి xstrfmt("git%s", git_cmd);
}

స్థిర శూన్య అమరిక_man_మార్గం(శూన్య)
{
	నిర్మాణం strbuf new_మార్గం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *old_మార్గం = getenv("MANPATH");
	అక్షరం *git_man_మార్గం = system_మార్గం(GIT_MAN_మార్గం);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_మార్గం, git_man_మార్గం);
	strbuf_addch(&new_మార్గం, ':');
	ఒకవేళ (old_మార్గం)
		strbuf_addstr(&new_మార్గం, old_మార్గం);

	విడుదల(git_man_మార్గం);
	setenv("MANPATH", new_మార్గం.buf, 1);

	strbuf_release(&new_మార్గం);
}

స్థిర శూన్య exec_viewer(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *పుట)
{
	స్థిరరాశి అక్షరం *info = get_man_viewer_info(name);

	ఒకవేళ (!strcasecmp(name, "man"))
		exec_man_man(info, పుట);
	లేకపోతే ఒకవేళ (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, పుట);
	లేకపోతే ఒకవేళ (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, పుట);
	లేకపోతే ఒకవేళ (info)
		exec_man_cmd(info, పుట);
	లేకపోతే
		warning(_("'%s': unknown man viewer."), name);
}

స్థిర శూన్య show_man_పుట(స్థిరరాశి అక్షరం *పుట)
{
	నిర్మాణం man_viewer_list *viewer;
	స్థిరరాశి అక్షరం *fallback = getenv("GIT_MAN_VIEWER");

	అమరిక_man_మార్గం();
	కొరకు (viewer = man_viewer_list; viewer; viewer = viewer->తదుపరి)
	{
		exec_viewer(viewer->name, పుట); /* will return when unable */
	}
	ఒకవేళ (fallback)
		exec_viewer(fallback, పుట);
	exec_viewer("man", పుట);
	die(_("no man viewer handled the request"));
}

స్థిర శూన్య show_info_పుట(స్థిరరాశి అక్షరం *పుట)
{
	setenv("INFOPATH", system_మార్గం(GIT_INFO_మార్గం), 1);
	execlp("info", "info", "gitman", పుట, (అక్షరం *)ఖాళీ);
	die(_("no info viewer handled the request"));
}

స్థిర శూన్య get_html_పుట_మార్గం(నిర్మాణం strbuf *పుట_మార్గం, స్థిరరాశి అక్షరం *పుట)
{
	నిర్మాణం stat st;
	స్థిరరాశి అక్షరం *మార్గం = html_మార్గం;
	అక్షరం *గమ్యం_free = ఖాళీ;

	ఒకవేళ (!మార్గం)
		మార్గం = గమ్యం_free = system_మార్గం(GIT_HTML_మార్గం);

	/*
	 * Check that the page we're looking for exists.
	 */
	ఒకవేళ (!అక్షరశ్రేణి_శోధన(మార్గం, "://")) {
		ఒకవేళ (stat(mkpath("%s/%s.html", మార్గం, పుట), &st)
		    || !S_ISREG(st.st_బహుళకం))
			die("'%s/%s.html': documentation file not found.",
				మార్గం, పుట);
	}

	strbuf_init(పుట_మార్గం, 0);
	strbuf_addf(పుట_మార్గం, "%s/%s.html", మార్గం, పుట);
	విడుదల(గమ్యం_free);
}

స్థిర శూన్య open_html(స్థిరరాశి అక్షరం *మార్గం)
{
	execl_git_cmd("web--browse", "-c", "help.browser", మార్గం, (అక్షరం *)ఖాళీ);
}

స్థిర శూన్య show_html_పుట(స్థిరరాశి అక్షరం *పుట)
{
	నిర్మాణం strbuf పుట_మార్గం; /* it leaks but we exec below */

	get_html_పుట_మార్గం(&పుట_మార్గం, పుట);

	open_html(పుట_మార్గం.buf);
}

స్థిర అక్షరం *check_git_cmd(స్థిరరాశి అక్షరం *cmd)
{
	అక్షరం *alias;

	ఒకవేళ (is_git_command(cmd))
		తిరిగి xstrdup(cmd);

	alias = alias_lookup(cmd);
	ఒకవేళ (alias) {
		స్థిరరాశి అక్షరం **argv;
		పూర్ణాంకం లెక్క;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		ఒకవేళ (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			విడుదల(alias);
			నిష్క్రమించు(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		లెక్క = విభజించు_cmdline(alias, &argv);
		ఒకవేళ (లెక్క < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    విభజించు_cmdline_strerror(లెక్క));
		విడుదల(argv);
		తిరిగి alias;
	}

	ఒకవేళ (exclude_guides)
		తిరిగి help_unknown_cmd(cmd);

	తిరిగి xstrdup(cmd);
}

స్థిర శూన్య no_help_format(స్థిరరాశి అక్షరం *opt_బహుళకం, గణన help_format fmt)
{
	స్థిరరాశి అక్షరం *opt_fmt;

	ఎంపిక (fmt) {
	స్థితి HELP_FORMAT_NONE:
		తిరిగి;
	స్థితి HELP_FORMAT_MAN:
		opt_fmt = "--man";
		ఆపు;
	స్థితి HELP_FORMAT_INFO:
		opt_fmt = "--info";
		ఆపు;
	స్థితి HELP_FORMAT_WEB:
		opt_fmt = "--web";
		ఆపు;
	అప్రమేయ:
		BUG("unreachable");
	}

	usage_సందేశం_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_బహుళకం,
		       opt_fmt);
}

స్థిర శూన్య opt_బహుళకం_usage(పూర్ణాంకం argc, స్థిరరాశి అక్షరం *opt_బహుళకం,
			   గణన help_format fmt)
{
	ఒకవేళ (argc)
		usage_సందేశం_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_బహుళకం);

	no_help_format(opt_బహుళకం, fmt);
}

పూర్ణాంకం cmd_help(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం nongit;
	గణన help_format parsed_help_format;
	అక్షరం *command = ఖాళీ;
	స్థిరరాశి అక్షరం *పుట;

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	ఒకవేళ (cmd_బహుళకం != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_సందేశం_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	ఎంపిక (cmd_బహుళకం) {
	స్థితి HELP_ACTION_ALL:
		opt_బహుళకం_usage(argc, "--all", help_format);
		ఒకవేళ (verbose) {
			అమరిక_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			తిరిగి 0;
		}
		రాయి(_("usage: %s%s"), _(git_usage_string), "\n\n");
		ఎక్కించు_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		రాయి("%s\n", _(git_మరిన్ని_info_string));
		ఆపు;
	స్థితి HELP_ACTION_GUIDES:
		opt_బహుళకం_usage(argc, "--guides", help_format);
		list_guides_help();
		రాయి("%s\n", _(git_మరిన్ని_info_string));
		తిరిగి 0;
	స్థితి HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_బహుళకం_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		తిరిగి 0;
	స్థితి HELP_ACTION_USER_INTERFACES:
		opt_బహుళకం_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		తిరిగి 0;
	స్థితి HELP_ACTION_DEVELOPER_INTERFACES:
		opt_బహుళకం_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		తిరిగి 0;
	స్థితి HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_బహుళకం_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		తిరిగి 0;
	స్థితి HELP_ACTION_ALIASES_FOR_COMPLETION: {
		నిర్మాణం string_list alias_list = STRING_LIST_INIT_DUP;
		opt_బహుళకం_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		కొరకు (కొలత_t i = 0; i < alias_list.nr; i++)
			రాయి("%s%c%s%c", alias_list.వస్తువులు[i].string, '\n',
			       (అక్షరం *)alias_list.వస్తువులు[i].util, '\0');
		string_list_clear(&alias_list, 1);
		తిరిగి 0;
	}
	స్థితి HELP_ACTION_CONFIG:
		opt_బహుళకం_usage(argc, "--config", help_format);
		అమరిక_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		రాయి("\n%s\n", _("'git help config' for more information"));
		తిరిగి 0;
	}

	ఒకవేళ (!argv[0]) {
		రాయి(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		రాయి("\n%s\n", _(git_మరిన్ని_info_string));
		తిరిగి 0;
	}

	అమరిక_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ఖాళీ);

	ఒకవేళ (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	ఒకవేళ (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	పుట = cmd_గమ్యం_పుట(command);
	ఎంపిక (help_format) {
	స్థితి HELP_FORMAT_NONE:
	స్థితి HELP_FORMAT_MAN:
		show_man_పుట(పుట);
		ఆపు;
	స్థితి HELP_FORMAT_INFO:
		show_info_పుట(పుట);
		ఆపు;
	స్థితి HELP_FORMAT_WEB:
		show_html_పుట(పుట);
		ఆపు;
	}

	విడుదల(command);
	తిరిగి 0;
}
