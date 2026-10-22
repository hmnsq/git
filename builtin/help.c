/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "exec-cmd.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "config-list.h"
#அந்தர்பாவ "help.h"
#அந்தர்பாவ "alias.h"
#அந்தர்பாவ "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

நிலையான கட்டமைப்பு man_viewer_list {
	கட்டமைப்பு man_viewer_list *next;
	எழுத்து name[FLEX_ARRAY];
} *man_viewer_list;

நிலையான கட்டமைப்பு man_viewer_info_list {
	கட்டமைப்பு man_viewer_info_list *next;
	மாறிலி எழுத்து *info;
	எழுத்து name[FLEX_ARRAY];
} *man_viewer_info_list;

எண்ணிக்கை help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

எண்ணிக்கை show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

நிலையான எண்ணிக்கை help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

நிலையான எழுத்து *html_path;
நிலையான முழு verbose = 1;
நிலையான எண்ணிக்கை help_format help_format = HELP_FORMAT_NONE;
நிலையான முழு exclude_guides;
நிலையான முழு show_external_commands = -1;
நிலையான முழு show_aliases = -1;
நிலையான கட்டமைப்பு option builtin_help_options[] = {
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

நிலையான மாறிலி எழுத்து * மாறிலி builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	வெற்று
};

கட்டமைப்பு slot_expansion {
	மாறிலி எழுத்து *prefix;
	மாறிலி எழுத்து *placeholder;
	வெறுமை (*fn)(கட்டமைப்பு string_list *list, மாறிலி எழுத்து *prefix);
	முழு found;
};

நிலையான வெறுமை set_config_vars(கட்டமைப்பு string_list *சாவிகள்_uniq, கட்டமைப்பு string_list_item *var)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *str = var->string;
	மாறிலி எழுத்து *wildcard = strchr(str, '*');
	மாறிலி எழுத்து *tag = strchr(str, '<');
	மாறிலி எழுத்து *cut;

	எனில் (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	இல்லையெனில் எனில் (wildcard)
		cut = wildcard;
	இல்லையெனில் எனில் (tag)
		cut = tag;
	இல்லையெனில் {
		string_list_append(சாவிகள்_uniq, str);
		திரும்பு;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(சாவிகள்_uniq, sb.buf);
	strbuf_release(&sb);
}

நிலையான வெறுமை set_config_sections(கட்டமைப்பு string_list *சாவிகள்_uniq, கட்டமைப்பு string_list_item *var)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *str = var->string;
	மாறிலி எழுத்து *dot = strchr(str, '.');
	மாறிலி எழுத்து *cut;

	எனில் (dot)
		cut = dot;
	இல்லையெனில் {
		set_config_vars(சாவிகள்_uniq, var);
		திரும்பு;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(சாவிகள்_uniq, sb.buf);
	strbuf_release(&sb);
}

நிலையான வெறுமை list_config_help(எண்ணிக்கை show_config_type type)
{
	கட்டமைப்பு slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_செய்தி_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_செய்தி_ids },
		{ வெற்று, வெற்று, வெற்று }
	};
	மாறிலி எழுத்து **p;
	கட்டமைப்பு slot_expansion *e;
	கட்டமைப்பு string_list சாவிகள் = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list சாவிகள்_uniq = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	க்கு (p = config_name_list; *p; p++) {
		மாறிலி எழுத்து *var = *p;

		க்கு (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			எனில் (!strcasecmp(var, sb.buf)) {
				e->fn(&சாவிகள், e->prefix);
				e->found++;
				நிறுத்து;
			}
		}

		எனில் (!e->prefix)
			string_list_append(&சாவிகள், var);
	}

	strbuf_release(&sb);

	க்கு (e = slot_expansions; e->prefix; e++)
		எனில் (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	க்கு (size_t i = 0; i < சாவிகள்.nr; i++) {
		தேர்வு (type) {
		நிலை SHOW_CONFIG_HUMAN:
			string_list_append(&சாவிகள்_uniq, சாவிகள்.பொருட்கள்[i].string);
			நிறுத்து;
		நிலை SHOW_CONFIG_SECTIONS:
			set_config_sections(&சாவிகள்_uniq, &சாவிகள்.பொருட்கள்[i]);
			நிறுத்து;
		நிலை SHOW_CONFIG_VARS:
			set_config_vars(&சாவிகள்_uniq, &சாவிகள்.பொருட்கள்[i]);
			நிறுத்து;
		இயல்பு:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&சாவிகள்_uniq, 0);
	for_each_string_list_item(item, &சாவிகள்_uniq)
		puts(item->string);
	string_list_clear(&சாவிகள்_uniq, 0);
	string_list_clear(&சாவிகள், 0);
}

நிலையான எண்ணிக்கை help_format parse_help_format(மாறிலி எழுத்து *format)
{
	எனில் (!strcmp(format, "man"))
		திரும்பு HELP_FORMAT_MAN;
	எனில் (!strcmp(format, "info"))
		திரும்பு HELP_FORMAT_INFO;
	எனில் (!strcmp(format, "web") || !strcmp(format, "html"))
		திரும்பு HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

நிலையான மாறிலி எழுத்து *get_man_viewer_info(மாறிலி எழுத்து *name)
{
	கட்டமைப்பு man_viewer_info_list *viewer;

	க்கு (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		எனில் (!strcasecmp(name, viewer->name))
			திரும்பு viewer->info;
	}
	திரும்பு வெற்று;
}

நிலையான முழு check_emacsclient_version(வெறுமை)
{
	கட்டமைப்பு strbuf buffer = STRBUF_INIT;
	கட்டமைப்பு child_process ec_process = CHILD_PROCESS_INIT;
	முழு version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", வெற்று);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	எனில் (start_command(&ec_process))
		திரும்பு error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	எனில் (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		திரும்பு error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	எனில் (version < 22) {
		strbuf_release(&buffer);
		திரும்பு error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	திரும்பு 0;
}

நிலையான வெறுமை exec_woman_emacs(மாறிலி எழுத்து *path, மாறிலி எழுத்து *page)
{
	எனில் (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		கட்டமைப்பு strbuf man_page = STRBUF_INIT;

		எனில் (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (எழுத்து *)வெற்று);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

நிலையான வெறுமை exec_man_konqueror(மாறிலி எழுத்து *path, மாறிலி எழுத்து *page)
{
	மாறிலி எழுத்து *display = getenv("DISPLAY");
	எனில் (display && *display) {
		கட்டமைப்பு strbuf man_page = STRBUF_INIT;
		மாறிலி எழுத்து *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		எனில் (path) {
			size_t len;
			எனில் (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (முழு)len, path);
			filename = basename((எழுத்து *)path);
		} இல்லையெனில்
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (எழுத்து *)வெற்று);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

நிலையான வெறுமை exec_man_man(மாறிலி எழுத்து *path, மாறிலி எழுத்து *page)
{
	எனில் (!path)
		path = "man";
	execlp(path, "man", page, (எழுத்து *)வெற்று);
	warning_errno(_("failed to exec '%s'"), path);
}

நிலையான வெறுமை exec_man_cmd(மாறிலி எழுத்து *cmd, மாறிலி எழுத்து *page)
{
	கட்டமைப்பு strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (எழுத்து *)வெற்று);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

நிலையான வெறுமை add_man_viewer(மாறிலி எழுத்து *name)
{
	கட்டமைப்பு man_viewer_list **p = &man_viewer_list;

	போது (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

நிலையான முழு supported_man_viewer(மாறிலி எழுத்து *name, size_t len)
{
	திரும்பு (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

நிலையான வெறுமை do_add_man_viewer_info(மாறிலி எழுத்து *name,
				   size_t len,
				   மாறிலி எழுத்து *மதிப்பு)
{
	கட்டமைப்பு man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(மதிப்பு);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

நிலையான முழு add_man_viewer_path(மாறிலி எழுத்து *name,
			       size_t len,
			       மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, மதிப்பு);
	இல்லையெனில்
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	திரும்பு 0;
}

நிலையான முழு add_man_viewer_cmd(மாறிலி எழுத்து *name,
			      size_t len,
			      மாறிலி எழுத்து *மதிப்பு)
{
	எனில் (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	இல்லையெனில்
		do_add_man_viewer_info(name, len, மதிப்பு);

	திரும்பு 0;
}

நிலையான முழு add_man_viewer_info(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு)
{
	மாறிலி எழுத்து *name, *subkey;
	size_t namelen;

	எனில் (parse_config_சாவி(var, "man", &name, &namelen, &subkey) < 0 || !name)
		திரும்பு 0;

	எனில் (!strcmp(subkey, "path")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		திரும்பு add_man_viewer_path(name, namelen, மதிப்பு);
	}
	எனில் (!strcmp(subkey, "cmd")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		திரும்பு add_man_viewer_cmd(name, namelen, மதிப்பு);
	}

	திரும்பு 0;
}

நிலையான முழு git_help_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			   மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "help.format")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		help_format = parse_help_format(மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "help.htmlpath")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		விடுவி(html_path);
		html_path = xstrdup(மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "man.viewer")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		add_man_viewer(மதிப்பு);
		திரும்பு 0;
	}
	எனில் (starts_with(var, "man."))
		திரும்பு add_man_viewer_info(var, மதிப்பு);

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான கட்டமைப்பு cmdnames main_cmds, other_cmds;

நிலையான முழு is_git_command(மாறிலி எழுத்து *s)
{
	எனில் (is_builtin(s))
		திரும்பு 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	திரும்பு is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

நிலையான மாறிலி எழுத்து *cmd_to_page(மாறிலி எழுத்து *git_cmd)
{
	எனில் (!git_cmd)
		திரும்பு "git";
	இல்லையெனில் எனில் (starts_with(git_cmd, "git"))
		திரும்பு git_cmd;
	இல்லையெனில் எனில் (is_git_command(git_cmd))
		திரும்பு xstrfmt("git-%s", git_cmd);
	இல்லையெனில் எனில் (!strcmp("scalar", git_cmd))
		திரும்பு xstrdup(git_cmd);
	இல்லையெனில்
		திரும்பு xstrfmt("git%s", git_cmd);
}

நிலையான வெறுமை setup_man_path(வெறுமை)
{
	கட்டமைப்பு strbuf new_path = STRBUF_INIT;
	மாறிலி எழுத்து *old_path = getenv("MANPATH");
	எழுத்து *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	எனில் (old_path)
		strbuf_addstr(&new_path, old_path);

	விடுவி(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

நிலையான வெறுமை exec_viewer(மாறிலி எழுத்து *name, மாறிலி எழுத்து *page)
{
	மாறிலி எழுத்து *info = get_man_viewer_info(name);

	எனில் (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	இல்லையெனில் எனில் (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	இல்லையெனில் எனில் (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	இல்லையெனில் எனில் (info)
		exec_man_cmd(info, page);
	இல்லையெனில்
		warning(_("'%s': unknown man viewer."), name);
}

நிலையான வெறுமை show_man_page(மாறிலி எழுத்து *page)
{
	கட்டமைப்பு man_viewer_list *viewer;
	மாறிலி எழுத்து *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	க்கு (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	எனில் (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

நிலையான வெறுமை show_info_page(மாறிலி எழுத்து *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (எழுத்து *)வெற்று);
	die(_("no info viewer handled the request"));
}

நிலையான வெறுமை get_html_page_path(கட்டமைப்பு strbuf *page_path, மாறிலி எழுத்து *page)
{
	கட்டமைப்பு stat st;
	மாறிலி எழுத்து *path = html_path;
	எழுத்து *to_free = வெற்று;

	எனில் (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	எனில் (!strstr(path, "://")) {
		எனில் (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	விடுவி(to_free);
}

நிலையான வெறுமை open_html(மாறிலி எழுத்து *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (எழுத்து *)வெற்று);
}

நிலையான வெறுமை show_html_page(மாறிலி எழுத்து *page)
{
	கட்டமைப்பு strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

நிலையான எழுத்து *check_git_cmd(மாறிலி எழுத்து *cmd)
{
	எழுத்து *alias;

	எனில் (is_git_command(cmd))
		திரும்பு xstrdup(cmd);

	alias = alias_lookup(cmd);
	எனில் (alias) {
		மாறிலி எழுத்து **argv;
		முழு மொத்தம்;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		எனில் (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			விடுவி(alias);
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
		மொத்தம் = split_cmdline(alias, &argv);
		எனில் (மொத்தம் < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(மொத்தம்));
		விடுவி(argv);
		திரும்பு alias;
	}

	எனில் (exclude_guides)
		திரும்பு help_unknown_cmd(cmd);

	திரும்பு xstrdup(cmd);
}

நிலையான வெறுமை no_help_format(மாறிலி எழுத்து *opt_mode, எண்ணிக்கை help_format fmt)
{
	மாறிலி எழுத்து *opt_fmt;

	தேர்வு (fmt) {
	நிலை HELP_FORMAT_NONE:
		திரும்பு;
	நிலை HELP_FORMAT_MAN:
		opt_fmt = "--man";
		நிறுத்து;
	நிலை HELP_FORMAT_INFO:
		opt_fmt = "--info";
		நிறுத்து;
	நிலை HELP_FORMAT_WEB:
		opt_fmt = "--web";
		நிறுத்து;
	இயல்பு:
		BUG("unreachable");
	}

	usage_செய்தி_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

நிலையான வெறுமை opt_mode_usage(முழு argc, மாறிலி எழுத்து *opt_mode,
			   எண்ணிக்கை help_format fmt)
{
	எனில் (argc)
		usage_செய்தி_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

முழு cmd_help(முழு argc,
	     மாறிலி எழுத்து **argv,
	     மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repo UNUSED)
{
	முழு nongit;
	எண்ணிக்கை help_format parsed_help_format;
	எழுத்து *command = வெற்று;
	மாறிலி எழுத்து *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	எனில் (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_செய்தி_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	தேர்வு (cmd_mode) {
	நிலை HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		எனில் (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			திரும்பு 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		நிறுத்து;
	நிலை HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		திரும்பு 0;
	நிலை HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		திரும்பு 0;
	நிலை HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		திரும்பு 0;
	நிலை HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		திரும்பு 0;
	நிலை HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		திரும்பு 0;
	நிலை HELP_ACTION_ALIASES_FOR_COMPLETION: {
		கட்டமைப்பு string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		க்கு (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.பொருட்கள்[i].string, '\n',
			       (எழுத்து *)alias_list.பொருட்கள்[i].util, '\0');
		string_list_clear(&alias_list, 1);
		திரும்பு 0;
	}
	நிலை HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		திரும்பு 0;
	}

	எனில் (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		திரும்பு 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, வெற்று);

	எனில் (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	எனில் (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	தேர்வு (help_format) {
	நிலை HELP_FORMAT_NONE:
	நிலை HELP_FORMAT_MAN:
		show_man_page(page);
		நிறுத்து;
	நிலை HELP_FORMAT_INFO:
		show_info_page(page);
		நிறுத்து;
	நிலை HELP_FORMAT_WEB:
		show_html_page(page);
		நிறுத்து;
	}

	விடுவி(command);
	திரும்பு 0;
}
