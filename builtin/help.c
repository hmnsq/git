/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "exec-cmd.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "config-list.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "alias.h"
#अन्तर्भाव "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

स्थिर संरचना man_viewer_list {
	संरचना man_viewer_list *next;
	अक्षर name[FLEX_ARRAY];
} *man_viewer_list;

स्थिर संरचना man_viewer_info_list {
	संरचना man_viewer_info_list *next;
	स्थिरांक अक्षर *info;
	अक्षर name[FLEX_ARRAY];
} *man_viewer_info_list;

गणना help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

गणना show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

स्थिर गणना help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

स्थिर अक्षर *html_path;
स्थिर पूर्णांक verbose = 1;
स्थिर गणना help_format help_format = HELP_FORMAT_NONE;
स्थिर पूर्णांक exclude_guides;
स्थिर पूर्णांक show_external_commands = -1;
स्थिर पूर्णांक show_aliases = -1;
स्थिर संरचना option builtin_help_options[] = {
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

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	रिक्त
};

संरचना slot_expansion {
	स्थिरांक अक्षर *prefix;
	स्थिरांक अक्षर *placeholder;
	शून्य (*fn)(संरचना string_list *list, स्थिरांक अक्षर *prefix);
	पूर्णांक found;
};

स्थिर शून्य set_config_vars(संरचना string_list *किल्ल्या_uniq, संरचना string_list_item *var)
{
	संरचना strbuf sb = STRBUF_INIT;
	स्थिरांक अक्षर *str = var->string;
	स्थिरांक अक्षर *wildcard = म_अक्षर(str, '*');
	स्थिरांक अक्षर *tag = म_अक्षर(str, '<');
	स्थिरांक अक्षर *cut;

	जर (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	अन्यथा जर (wildcard)
		cut = wildcard;
	अन्यथा जर (tag)
		cut = tag;
	अन्यथा {
		string_list_append(किल्ल्या_uniq, str);
		परत;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(किल्ल्या_uniq, sb.buf);
	strbuf_release(&sb);
}

स्थिर शून्य set_config_sections(संरचना string_list *किल्ल्या_uniq, संरचना string_list_item *var)
{
	संरचना strbuf sb = STRBUF_INIT;
	स्थिरांक अक्षर *str = var->string;
	स्थिरांक अक्षर *dot = म_अक्षर(str, '.');
	स्थिरांक अक्षर *cut;

	जर (dot)
		cut = dot;
	अन्यथा {
		set_config_vars(किल्ल्या_uniq, var);
		परत;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(किल्ल्या_uniq, sb.buf);
	strbuf_release(&sb);
}

स्थिर शून्य list_config_help(गणना show_config_type type)
{
	संरचना slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_संदेश_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_संदेश_ids },
		{ रिक्त, रिक्त, रिक्त }
	};
	स्थिरांक अक्षर **p;
	संरचना slot_expansion *e;
	संरचना string_list किल्ल्या = STRING_LIST_INIT_DUP;
	संरचना string_list किल्ल्या_uniq = STRING_LIST_INIT_DUP;
	संरचना string_list_item *item;
	संरचना strbuf sb = STRBUF_INIT;

	साठी (p = config_name_list; *p; p++) {
		स्थिरांक अक्षर *var = *p;

		साठी (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			जर (!strcasecmp(var, sb.buf)) {
				e->fn(&किल्ल्या, e->prefix);
				e->found++;
				थांबा;
			}
		}

		जर (!e->prefix)
			string_list_append(&किल्ल्या, var);
	}

	strbuf_release(&sb);

	साठी (e = slot_expansions; e->prefix; e++)
		जर (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	साठी (माप_प्रकार i = 0; i < किल्ल्या.nr; i++) {
		निवड (type) {
		स्थिती SHOW_CONFIG_HUMAN:
			string_list_append(&किल्ल्या_uniq, किल्ल्या.वस्तुसंच[i].string);
			थांबा;
		स्थिती SHOW_CONFIG_SECTIONS:
			set_config_sections(&किल्ल्या_uniq, &किल्ल्या.वस्तुसंच[i]);
			थांबा;
		स्थिती SHOW_CONFIG_VARS:
			set_config_vars(&किल्ल्या_uniq, &किल्ल्या.वस्तुसंच[i]);
			थांबा;
		पूर्वनिर्धारित:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&किल्ल्या_uniq, 0);
	for_each_string_list_item(item, &किल्ल्या_uniq)
		माला_दो(item->string);
	string_list_clear(&किल्ल्या_uniq, 0);
	string_list_clear(&किल्ल्या, 0);
}

स्थिर गणना help_format parse_help_format(स्थिरांक अक्षर *format)
{
	जर (!म_भेद(format, "man"))
		परत HELP_FORMAT_MAN;
	जर (!म_भेद(format, "info"))
		परत HELP_FORMAT_INFO;
	जर (!म_भेद(format, "web") || !म_भेद(format, "html"))
		परत HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

स्थिर स्थिरांक अक्षर *get_man_viewer_info(स्थिरांक अक्षर *name)
{
	संरचना man_viewer_info_list *viewer;

	साठी (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		जर (!strcasecmp(name, viewer->name))
			परत viewer->info;
	}
	परत रिक्त;
}

स्थिर पूर्णांक check_emacsclient_version(शून्य)
{
	संरचना strbuf buffer = STRBUF_INIT;
	संरचना child_process ec_process = CHILD_PROCESS_INIT;
	पूर्णांक version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", रिक्त);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	जर (start_command(&ec_process))
		परत error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	बंद(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	जर (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		परत error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, म_माप("emacsclient"));
	version = म_से_प(buffer.buf);

	जर (version < 22) {
		strbuf_release(&buffer);
		परत error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	परत 0;
}

स्थिर शून्य exec_woman_emacs(स्थिरांक अक्षर *path, स्थिरांक अक्षर *page)
{
	जर (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		संरचना strbuf man_page = STRBUF_INIT;

		जर (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (अक्षर *)रिक्त);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

स्थिर शून्य exec_man_konqueror(स्थिरांक अक्षर *path, स्थिरांक अक्षर *page)
{
	स्थिरांक अक्षर *display = दो_पर्या("DISPLAY");
	जर (display && *display) {
		संरचना strbuf man_page = STRBUF_INIT;
		स्थिरांक अक्षर *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		जर (path) {
			माप_प्रकार len;
			जर (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (पूर्णांक)len, path);
			filename = basename((अक्षर *)path);
		} अन्यथा
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (अक्षर *)रिक्त);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

स्थिर शून्य exec_man_man(स्थिरांक अक्षर *path, स्थिरांक अक्षर *page)
{
	जर (!path)
		path = "man";
	execlp(path, "man", page, (अक्षर *)रिक्त);
	warning_errno(_("failed to exec '%s'"), path);
}

स्थिर शून्य exec_man_cmd(स्थिरांक अक्षर *cmd, स्थिरांक अक्षर *page)
{
	संरचना strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (अक्षर *)रिक्त);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

स्थिर शून्य add_man_viewer(स्थिरांक अक्षर *name)
{
	संरचना man_viewer_list **p = &man_viewer_list;

	पर्यंत (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

स्थिर पूर्णांक supported_man_viewer(स्थिरांक अक्षर *name, माप_प्रकार len)
{
	परत (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

स्थिर शून्य do_add_man_viewer_info(स्थिरांक अक्षर *name,
				   माप_प्रकार len,
				   स्थिरांक अक्षर *मूल्य)
{
	संरचना man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(मूल्य);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

स्थिर पूर्णांक add_man_viewer_path(स्थिरांक अक्षर *name,
			       माप_प्रकार len,
			       स्थिरांक अक्षर *मूल्य)
{
	जर (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, मूल्य);
	अन्यथा
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	परत 0;
}

स्थिर पूर्णांक add_man_viewer_cmd(स्थिरांक अक्षर *name,
			      माप_प्रकार len,
			      स्थिरांक अक्षर *मूल्य)
{
	जर (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	अन्यथा
		do_add_man_viewer_info(name, len, मूल्य);

	परत 0;
}

स्थिर पूर्णांक add_man_viewer_info(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य)
{
	स्थिरांक अक्षर *name, *subkey;
	माप_प्रकार namelen;

	जर (parse_config_किल्ली(var, "man", &name, &namelen, &subkey) < 0 || !name)
		परत 0;

	जर (!म_भेद(subkey, "path")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		परत add_man_viewer_path(name, namelen, मूल्य);
	}
	जर (!म_भेद(subkey, "cmd")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		परत add_man_viewer_cmd(name, namelen, मूल्य);
	}

	परत 0;
}

स्थिर पूर्णांक git_help_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			   स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	जर (!म_भेद(var, "help.format")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		help_format = parse_help_format(मूल्य);
		परत 0;
	}
	जर (!म_भेद(var, "help.htmlpath")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		मुक्त(html_path);
		html_path = xstrdup(मूल्य);
		परत 0;
	}
	जर (!म_भेद(var, "man.viewer")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		add_man_viewer(मूल्य);
		परत 0;
	}
	जर (starts_with(var, "man."))
		परत add_man_viewer_info(var, मूल्य);

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर संरचना cmdnames main_cmds, other_cmds;

स्थिर पूर्णांक is_git_command(स्थिरांक अक्षर *s)
{
	जर (is_builtin(s))
		परत 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	परत is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

स्थिर स्थिरांक अक्षर *cmd_to_page(स्थिरांक अक्षर *git_cmd)
{
	जर (!git_cmd)
		परत "git";
	अन्यथा जर (starts_with(git_cmd, "git"))
		परत git_cmd;
	अन्यथा जर (is_git_command(git_cmd))
		परत xstrfmt("git-%s", git_cmd);
	अन्यथा जर (!म_भेद("scalar", git_cmd))
		परत xstrdup(git_cmd);
	अन्यथा
		परत xstrfmt("git%s", git_cmd);
}

स्थिर शून्य setup_man_path(शून्य)
{
	संरचना strbuf new_path = STRBUF_INIT;
	स्थिरांक अक्षर *old_path = दो_पर्या("MANPATH");
	अक्षर *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	जर (old_path)
		strbuf_addstr(&new_path, old_path);

	मुक्त(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

स्थिर शून्य exec_viewer(स्थिरांक अक्षर *name, स्थिरांक अक्षर *page)
{
	स्थिरांक अक्षर *info = get_man_viewer_info(name);

	जर (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	अन्यथा जर (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	अन्यथा जर (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	अन्यथा जर (info)
		exec_man_cmd(info, page);
	अन्यथा
		warning(_("'%s': unknown man viewer."), name);
}

स्थिर शून्य show_man_page(स्थिरांक अक्षर *page)
{
	संरचना man_viewer_list *viewer;
	स्थिरांक अक्षर *fallback = दो_पर्या("GIT_MAN_VIEWER");

	setup_man_path();
	साठी (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	जर (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

स्थिर शून्य show_info_page(स्थिरांक अक्षर *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (अक्षर *)रिक्त);
	die(_("no info viewer handled the request"));
}

स्थिर शून्य get_html_page_path(संरचना strbuf *page_path, स्थिरांक अक्षर *page)
{
	संरचना stat st;
	स्थिरांक अक्षर *path = html_path;
	अक्षर *to_free = रिक्त;

	जर (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	जर (!म_माला(path, "://")) {
		जर (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	मुक्त(to_free);
}

स्थिर शून्य open_html(स्थिरांक अक्षर *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (अक्षर *)रिक्त);
}

स्थिर शून्य show_html_page(स्थिरांक अक्षर *page)
{
	संरचना strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

स्थिर अक्षर *check_git_cmd(स्थिरांक अक्षर *cmd)
{
	अक्षर *alias;

	जर (is_git_command(cmd))
		परत xstrdup(cmd);

	alias = alias_lookup(cmd);
	जर (alias) {
		स्थिरांक अक्षर **argv;
		पूर्णांक मोजणी;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		जर (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			मुक्त(alias);
			निकास(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(मानक_त्रुटि, _("'%s' is aliased to '%s'"), cmd, alias);
		मोजणी = split_cmdline(alias, &argv);
		जर (मोजणी < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(मोजणी));
		मुक्त(argv);
		परत alias;
	}

	जर (exclude_guides)
		परत help_unknown_cmd(cmd);

	परत xstrdup(cmd);
}

स्थिर शून्य no_help_format(स्थिरांक अक्षर *opt_mode, गणना help_format fmt)
{
	स्थिरांक अक्षर *opt_fmt;

	निवड (fmt) {
	स्थिती HELP_FORMAT_NONE:
		परत;
	स्थिती HELP_FORMAT_MAN:
		opt_fmt = "--man";
		थांबा;
	स्थिती HELP_FORMAT_INFO:
		opt_fmt = "--info";
		थांबा;
	स्थिती HELP_FORMAT_WEB:
		opt_fmt = "--web";
		थांबा;
	पूर्वनिर्धारित:
		BUG("unreachable");
	}

	usage_संदेश_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

स्थिर शून्य opt_mode_usage(पूर्णांक argc, स्थिरांक अक्षर *opt_mode,
			   गणना help_format fmt)
{
	जर (argc)
		usage_संदेश_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

पूर्णांक cmd_help(पूर्णांक argc,
	     स्थिरांक अक्षर **argv,
	     स्थिरांक अक्षर *prefix,
	     संरचना repository *repo UNUSED)
{
	पूर्णांक nongit;
	गणना help_format parsed_help_format;
	अक्षर *command = रिक्त;
	स्थिरांक अक्षर *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	जर (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_संदेश_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	निवड (cmd_mode) {
	स्थिती HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		जर (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			परत 0;
		}
		म_लिखो(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		म_लिखो("%s\n", _(git_more_info_string));
		थांबा;
	स्थिती HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		म_लिखो("%s\n", _(git_more_info_string));
		परत 0;
	स्थिती HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		परत 0;
	स्थिती HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		परत 0;
	स्थिती HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		परत 0;
	स्थिती HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		परत 0;
	स्थिती HELP_ACTION_ALIASES_FOR_COMPLETION: {
		संरचना string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		साठी (माप_प्रकार i = 0; i < alias_list.nr; i++)
			म_लिखो("%s%c%s%c", alias_list.वस्तुसंच[i].string, '\n',
			       (अक्षर *)alias_list.वस्तुसंच[i].util, '\0');
		string_list_clear(&alias_list, 1);
		परत 0;
	}
	स्थिती HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		म_लिखो("\n%s\n", _("'git help config' for more information"));
		परत 0;
	}

	जर (!argv[0]) {
		म_लिखो(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		म_लिखो("\n%s\n", _(git_more_info_string));
		परत 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, रिक्त);

	जर (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	जर (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	निवड (help_format) {
	स्थिती HELP_FORMAT_NONE:
	स्थिती HELP_FORMAT_MAN:
		show_man_page(page);
		थांबा;
	स्थिती HELP_FORMAT_INFO:
		show_info_page(page);
		थांबा;
	स्थिती HELP_FORMAT_WEB:
		show_html_page(page);
		थांबा;
	}

	मुक्त(command);
	परत 0;
}
