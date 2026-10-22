/*
 * Builtin help command
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "exec-cmd.h"
#समावेश "gettext.h"
#समावेश "pager.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "run-command.h"
#समावेश "config-list.h"
#समावेश "help.h"
#समावेश "alias.h"
#समावेश "setup.h"

#अगर_अघोषित DEFAULT_HELP_FORMAT
#घोषणा DEFAULT_HELP_FORMAT "man"
#पूर्ण_अगर

अटल काष्ठा man_viewer_list {
	काष्ठा man_viewer_list *अग्र;
	अक्षर name[FLEX_ARRAY];
} *man_viewer_list;

अटल काष्ठा man_viewer_info_list {
	काष्ठा man_viewer_info_list *अग्र;
	स्थिर अक्षर *info;
	अक्षर name[FLEX_ARRAY];
} *man_viewer_info_list;

क्रमागत help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

क्रमागत show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

अटल क्रमागत help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

अटल अक्षर *html_path;
अटल पूर्णांक verbose = 1;
अटल क्रमागत help_format help_format = HELP_FORMAT_NONE;
अटल पूर्णांक exclude_guides;
अटल पूर्णांक show_external_commands = -1;
अटल पूर्णांक show_aliases = -1;
अटल काष्ठा option builtin_help_options[] = {
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

अटल स्थिर अक्षर * स्थिर builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	शून्य
};

काष्ठा slot_expansion {
	स्थिर अक्षर *prefix;
	स्थिर अक्षर *placeholder;
	व्योम (*fn)(काष्ठा string_list *list, स्थिर अक्षर *prefix);
	पूर्णांक found;
};

अटल व्योम set_config_vars(काष्ठा string_list *कुंजियाँ_uniq, काष्ठा string_list_item *var)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *str = var->string;
	स्थिर अक्षर *wildcard = म_अक्षर(str, '*');
	स्थिर अक्षर *tag = म_अक्षर(str, '<');
	स्थिर अक्षर *cut;

	अगर (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	अन्यथा अगर (wildcard)
		cut = wildcard;
	अन्यथा अगर (tag)
		cut = tag;
	अन्यथा {
		string_list_append(कुंजियाँ_uniq, str);
		वापस;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(कुंजियाँ_uniq, sb.buf);
	strbuf_release(&sb);
}

अटल व्योम set_config_sections(काष्ठा string_list *कुंजियाँ_uniq, काष्ठा string_list_item *var)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *str = var->string;
	स्थिर अक्षर *dot = म_अक्षर(str, '.');
	स्थिर अक्षर *cut;

	अगर (dot)
		cut = dot;
	अन्यथा {
		set_config_vars(कुंजियाँ_uniq, var);
		वापस;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(कुंजियाँ_uniq, sb.buf);
	strbuf_release(&sb);
}

अटल व्योम list_config_help(क्रमागत show_config_type type)
{
	काष्ठा slot_expansion slot_expansions[] = {
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
		{ शून्य, शून्य, शून्य }
	};
	स्थिर अक्षर **p;
	काष्ठा slot_expansion *e;
	काष्ठा string_list कुंजियाँ = STRING_LIST_INIT_DUP;
	काष्ठा string_list कुंजियाँ_uniq = STRING_LIST_INIT_DUP;
	काष्ठा string_list_item *item;
	काष्ठा strbuf sb = STRBUF_INIT;

	क्रम (p = config_name_list; *p; p++) {
		स्थिर अक्षर *var = *p;

		क्रम (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			अगर (!strcasecmp(var, sb.buf)) {
				e->fn(&कुंजियाँ, e->prefix);
				e->found++;
				अवरोध;
			}
		}

		अगर (!e->prefix)
			string_list_append(&कुंजियाँ, var);
	}

	strbuf_release(&sb);

	क्रम (e = slot_expansions; e->prefix; e++)
		अगर (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	क्रम (माप_प्रकार i = 0; i < कुंजियाँ.nr; i++) {
		चयन (type) {
		हाल SHOW_CONFIG_HUMAN:
			string_list_append(&कुंजियाँ_uniq, कुंजियाँ.वस्तुएँ[i].string);
			अवरोध;
		हाल SHOW_CONFIG_SECTIONS:
			set_config_sections(&कुंजियाँ_uniq, &कुंजियाँ.वस्तुएँ[i]);
			अवरोध;
		हाल SHOW_CONFIG_VARS:
			set_config_vars(&कुंजियाँ_uniq, &कुंजियाँ.वस्तुएँ[i]);
			अवरोध;
		शेष:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&कुंजियाँ_uniq, 0);
	for_each_string_list_item(item, &कुंजियाँ_uniq)
		माला_दो(item->string);
	string_list_clear(&कुंजियाँ_uniq, 0);
	string_list_clear(&कुंजियाँ, 0);
}

अटल क्रमागत help_format parse_help_format(स्थिर अक्षर *format)
{
	अगर (!म_भेद(format, "man"))
		वापस HELP_FORMAT_MAN;
	अगर (!म_भेद(format, "info"))
		वापस HELP_FORMAT_INFO;
	अगर (!म_भेद(format, "web") || !म_भेद(format, "html"))
		वापस HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

अटल स्थिर अक्षर *get_man_viewer_info(स्थिर अक्षर *name)
{
	काष्ठा man_viewer_info_list *viewer;

	क्रम (viewer = man_viewer_info_list; viewer; viewer = viewer->अग्र)
	{
		अगर (!strcasecmp(name, viewer->name))
			वापस viewer->info;
	}
	वापस शून्य;
}

अटल पूर्णांक check_emacsclient_version(व्योम)
{
	काष्ठा strbuf बफर = STRBUF_INIT;
	काष्ठा child_process ec_process = CHILD_PROCESS_INIT;
	पूर्णांक version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", शून्य);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	अगर (start_command(&ec_process))
		वापस error(_("Failed to start emacsclient."));

	strbuf_read(&बफर, ec_process.err, 20);
	बंद(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	अगर (!starts_with(बफर.buf, "emacsclient")) {
		strbuf_release(&बफर);
		वापस error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&बफर, 0, म_माप("emacsclient"));
	version = म_से_प(बफर.buf);

	अगर (version < 22) {
		strbuf_release(&बफर);
		वापस error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&बफर);
	वापस 0;
}

अटल व्योम exec_woman_emacs(स्थिर अक्षर *path, स्थिर अक्षर *page)
{
	अगर (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		काष्ठा strbuf man_page = STRBUF_INIT;

		अगर (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (अक्षर *)शून्य);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

अटल व्योम exec_man_konqueror(स्थिर अक्षर *path, स्थिर अक्षर *page)
{
	स्थिर अक्षर *display = दो_पर्या("DISPLAY");
	अगर (display && *display) {
		काष्ठा strbuf man_page = STRBUF_INIT;
		स्थिर अक्षर *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		अगर (path) {
			माप_प्रकार len;
			अगर (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (पूर्णांक)len, path);
			filename = basename((अक्षर *)path);
		} अन्यथा
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (अक्षर *)शून्य);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

अटल व्योम exec_man_man(स्थिर अक्षर *path, स्थिर अक्षर *page)
{
	अगर (!path)
		path = "man";
	execlp(path, "man", page, (अक्षर *)शून्य);
	warning_errno(_("failed to exec '%s'"), path);
}

अटल व्योम exec_man_cmd(स्थिर अक्षर *cmd, स्थिर अक्षर *page)
{
	काष्ठा strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (अक्षर *)शून्य);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

अटल व्योम add_man_viewer(स्थिर अक्षर *name)
{
	काष्ठा man_viewer_list **p = &man_viewer_list;

	जबतक (*p)
		p = &((*p)->अग्र);
	FLEX_ALLOC_STR(*p, name, name);
}

अटल पूर्णांक supported_man_viewer(स्थिर अक्षर *name, माप_प्रकार len)
{
	वापस (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

अटल व्योम do_add_man_viewer_info(स्थिर अक्षर *name,
				   माप_प्रकार len,
				   स्थिर अक्षर *मूल्य)
{
	काष्ठा man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(मूल्य);
	new_man_viewer->अग्र = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

अटल पूर्णांक add_man_viewer_path(स्थिर अक्षर *name,
			       माप_प्रकार len,
			       स्थिर अक्षर *मूल्य)
{
	अगर (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, मूल्य);
	अन्यथा
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	वापस 0;
}

अटल पूर्णांक add_man_viewer_cmd(स्थिर अक्षर *name,
			      माप_प्रकार len,
			      स्थिर अक्षर *मूल्य)
{
	अगर (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	अन्यथा
		do_add_man_viewer_info(name, len, मूल्य);

	वापस 0;
}

अटल पूर्णांक add_man_viewer_info(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य)
{
	स्थिर अक्षर *name, *subkey;
	माप_प्रकार namelen;

	अगर (parse_config_कुंजी(var, "man", &name, &namelen, &subkey) < 0 || !name)
		वापस 0;

	अगर (!म_भेद(subkey, "path")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		वापस add_man_viewer_path(name, namelen, मूल्य);
	}
	अगर (!म_भेद(subkey, "cmd")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		वापस add_man_viewer_cmd(name, namelen, मूल्य);
	}

	वापस 0;
}

अटल पूर्णांक git_help_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			   स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "help.format")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		help_format = parse_help_format(मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "help.htmlpath")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		मुक्त(html_path);
		html_path = xstrdup(मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "man.viewer")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		add_man_viewer(मूल्य);
		वापस 0;
	}
	अगर (starts_with(var, "man."))
		वापस add_man_viewer_info(var, मूल्य);

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल काष्ठा cmdnames main_cmds, other_cmds;

अटल पूर्णांक is_git_command(स्थिर अक्षर *s)
{
	अगर (is_builtin(s))
		वापस 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	वापस is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

अटल स्थिर अक्षर *cmd_to_page(स्थिर अक्षर *git_cmd)
{
	अगर (!git_cmd)
		वापस "git";
	अन्यथा अगर (starts_with(git_cmd, "git"))
		वापस git_cmd;
	अन्यथा अगर (is_git_command(git_cmd))
		वापस xstrfmt("git-%s", git_cmd);
	अन्यथा अगर (!म_भेद("scalar", git_cmd))
		वापस xstrdup(git_cmd);
	अन्यथा
		वापस xstrfmt("git%s", git_cmd);
}

अटल व्योम setup_man_path(व्योम)
{
	काष्ठा strbuf new_path = STRBUF_INIT;
	स्थिर अक्षर *old_path = दो_पर्या("MANPATH");
	अक्षर *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	अगर (old_path)
		strbuf_addstr(&new_path, old_path);

	मुक्त(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

अटल व्योम exec_viewer(स्थिर अक्षर *name, स्थिर अक्षर *page)
{
	स्थिर अक्षर *info = get_man_viewer_info(name);

	अगर (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	अन्यथा अगर (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	अन्यथा अगर (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	अन्यथा अगर (info)
		exec_man_cmd(info, page);
	अन्यथा
		warning(_("'%s': unknown man viewer."), name);
}

अटल व्योम show_man_page(स्थिर अक्षर *page)
{
	काष्ठा man_viewer_list *viewer;
	स्थिर अक्षर *fallback = दो_पर्या("GIT_MAN_VIEWER");

	setup_man_path();
	क्रम (viewer = man_viewer_list; viewer; viewer = viewer->अग्र)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	अगर (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

अटल व्योम show_info_page(स्थिर अक्षर *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (अक्षर *)शून्य);
	die(_("no info viewer handled the request"));
}

अटल व्योम get_html_page_path(काष्ठा strbuf *page_path, स्थिर अक्षर *page)
{
	काष्ठा stat st;
	स्थिर अक्षर *path = html_path;
	अक्षर *to_free = शून्य;

	अगर (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	अगर (!म_माला(path, "://")) {
		अगर (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	मुक्त(to_free);
}

अटल व्योम open_html(स्थिर अक्षर *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (अक्षर *)शून्य);
}

अटल व्योम show_html_page(स्थिर अक्षर *page)
{
	काष्ठा strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

अटल अक्षर *check_git_cmd(स्थिर अक्षर *cmd)
{
	अक्षर *alias;

	अगर (is_git_command(cmd))
		वापस xstrdup(cmd);

	alias = alias_lookup(cmd);
	अगर (alias) {
		स्थिर अक्षर **argv;
		पूर्णांक गिनती;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		अगर (!exclude_guides || alias[0] == '!') {
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
		गिनती = split_cmdline(alias, &argv);
		अगर (गिनती < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(गिनती));
		मुक्त(argv);
		वापस alias;
	}

	अगर (exclude_guides)
		वापस help_unknown_cmd(cmd);

	वापस xstrdup(cmd);
}

अटल व्योम no_help_format(स्थिर अक्षर *opt_mode, क्रमागत help_format fmt)
{
	स्थिर अक्षर *opt_fmt;

	चयन (fmt) {
	हाल HELP_FORMAT_NONE:
		वापस;
	हाल HELP_FORMAT_MAN:
		opt_fmt = "--man";
		अवरोध;
	हाल HELP_FORMAT_INFO:
		opt_fmt = "--info";
		अवरोध;
	हाल HELP_FORMAT_WEB:
		opt_fmt = "--web";
		अवरोध;
	शेष:
		BUG("unreachable");
	}

	usage_संदेश_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

अटल व्योम opt_mode_usage(पूर्णांक argc, स्थिर अक्षर *opt_mode,
			   क्रमागत help_format fmt)
{
	अगर (argc)
		usage_संदेश_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

पूर्णांक cmd_help(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repo UNUSED)
{
	पूर्णांक nongit;
	क्रमागत help_format parsed_help_format;
	अक्षर *command = शून्य;
	स्थिर अक्षर *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	अगर (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_संदेश_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	चयन (cmd_mode) {
	हाल HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		अगर (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			वापस 0;
		}
		म_लिखो(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		म_लिखो("%s\n", _(git_more_info_string));
		अवरोध;
	हाल HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		म_लिखो("%s\n", _(git_more_info_string));
		वापस 0;
	हाल HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		वापस 0;
	हाल HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		वापस 0;
	हाल HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		वापस 0;
	हाल HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		वापस 0;
	हाल HELP_ACTION_ALIASES_FOR_COMPLETION: {
		काष्ठा string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		क्रम (माप_प्रकार i = 0; i < alias_list.nr; i++)
			म_लिखो("%s%c%s%c", alias_list.वस्तुएँ[i].string, '\n',
			       (अक्षर *)alias_list.वस्तुएँ[i].util, '\0');
		string_list_clear(&alias_list, 1);
		वापस 0;
	}
	हाल HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		म_लिखो("\n%s\n", _("'git help config' for more information"));
		वापस 0;
	}

	अगर (!argv[0]) {
		म_लिखो(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		म_लिखो("\n%s\n", _(git_more_info_string));
		वापस 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, शून्य);

	अगर (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	अगर (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	चयन (help_format) {
	हाल HELP_FORMAT_NONE:
	हाल HELP_FORMAT_MAN:
		show_man_page(page);
		अवरोध;
	हाल HELP_FORMAT_INFO:
		show_info_page(page);
		अवरोध;
	हाल HELP_FORMAT_WEB:
		show_html_page(page);
		अवरोध;
	}

	मुक्त(command);
	वापस 0;
}
