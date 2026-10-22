/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "exec-cmd.h"
#شامل "gettext.h"
#شامل "pager.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "run-command.h"
#شامل "config-list.h"
#شامل "help.h"
#شامل "alias.h"
#شامل "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

ایستا ساختار man_viewer_list {
	ساختار man_viewer_list *next;
	نویسه name[FLEX_ARRAY];
} *man_viewer_list;

ایستا ساختار man_viewer_info_list {
	ساختار man_viewer_info_list *next;
	ثابت نویسه *info;
	نویسه name[FLEX_ARRAY];
} *man_viewer_info_list;

شمارش help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

شمارش show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ایستا شمارش help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

ایستا نویسه *html_path;
ایستا صحیح verbose = 1;
ایستا شمارش help_format help_format = HELP_FORMAT_NONE;
ایستا صحیح exclude_guides;
ایستا صحیح show_external_commands = -1;
ایستا صحیح show_aliases = -1;
ایستا ساختار option builtin_help_options[] = {
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

ایستا ثابت نویسه * ثابت builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	پوچ
};

ساختار slot_expansion {
	ثابت نویسه *prefix;
	ثابت نویسه *placeholder;
	تهی (*fn)(ساختار string_list *list, ثابت نویسه *prefix);
	صحیح found;
};

ایستا تهی set_config_vars(ساختار string_list *کلیدها_uniq, ساختار string_list_item *var)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *str = var->string;
	ثابت نویسه *wildcard = strchr(str, '*');
	ثابت نویسه *tag = strchr(str, '<');
	ثابت نویسه *cut;

	اگر (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	وگرنه اگر (wildcard)
		cut = wildcard;
	وگرنه اگر (tag)
		cut = tag;
	وگرنه {
		string_list_append(کلیدها_uniq, str);
		بازگشت;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(کلیدها_uniq, sb.buf);
	strbuf_release(&sb);
}

ایستا تهی set_config_sections(ساختار string_list *کلیدها_uniq, ساختار string_list_item *var)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *str = var->string;
	ثابت نویسه *dot = strchr(str, '.');
	ثابت نویسه *cut;

	اگر (dot)
		cut = dot;
	وگرنه {
		set_config_vars(کلیدها_uniq, var);
		بازگشت;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(کلیدها_uniq, sb.buf);
	strbuf_release(&sb);
}

ایستا تهی list_config_help(شمارش show_config_type type)
{
	ساختار slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_پیام_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_پیام_ids },
		{ پوچ, پوچ, پوچ }
	};
	ثابت نویسه **p;
	ساختار slot_expansion *e;
	ساختار string_list کلیدها = STRING_LIST_INIT_DUP;
	ساختار string_list کلیدها_uniq = STRING_LIST_INIT_DUP;
	ساختار string_list_item *item;
	ساختار strbuf sb = STRBUF_INIT;

	برای (p = config_name_list; *p; p++) {
		ثابت نویسه *var = *p;

		برای (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			اگر (!strcasecmp(var, sb.buf)) {
				e->fn(&کلیدها, e->prefix);
				e->found++;
				بشکن;
			}
		}

		اگر (!e->prefix)
			string_list_append(&کلیدها, var);
	}

	strbuf_release(&sb);

	برای (e = slot_expansions; e->prefix; e++)
		اگر (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	برای (size_t i = 0; i < کلیدها.nr; i++) {
		انتخاب (type) {
		حالت SHOW_CONFIG_HUMAN:
			string_list_append(&کلیدها_uniq, کلیدها.اقلام[i].string);
			بشکن;
		حالت SHOW_CONFIG_SECTIONS:
			set_config_sections(&کلیدها_uniq, &کلیدها.اقلام[i]);
			بشکن;
		حالت SHOW_CONFIG_VARS:
			set_config_vars(&کلیدها_uniq, &کلیدها.اقلام[i]);
			بشکن;
		پیشفرض:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&کلیدها_uniq, 0);
	for_each_string_list_item(item, &کلیدها_uniq)
		puts(item->string);
	string_list_clear(&کلیدها_uniq, 0);
	string_list_clear(&کلیدها, 0);
}

ایستا شمارش help_format parse_help_format(ثابت نویسه *format)
{
	اگر (!strcmp(format, "man"))
		بازگشت HELP_FORMAT_MAN;
	اگر (!strcmp(format, "info"))
		بازگشت HELP_FORMAT_INFO;
	اگر (!strcmp(format, "web") || !strcmp(format, "html"))
		بازگشت HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ایستا ثابت نویسه *get_man_viewer_info(ثابت نویسه *name)
{
	ساختار man_viewer_info_list *viewer;

	برای (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		اگر (!strcasecmp(name, viewer->name))
			بازگشت viewer->info;
	}
	بازگشت پوچ;
}

ایستا صحیح check_emacsclient_version(تهی)
{
	ساختار strbuf buffer = STRBUF_INIT;
	ساختار child_process ec_process = CHILD_PROCESS_INIT;
	صحیح version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", پوچ);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	اگر (start_command(&ec_process))
		بازگشت error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	اگر (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		بازگشت error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	اگر (version < 22) {
		strbuf_release(&buffer);
		بازگشت error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	بازگشت 0;
}

ایستا تهی exec_woman_emacs(ثابت نویسه *path, ثابت نویسه *page)
{
	اگر (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		ساختار strbuf man_page = STRBUF_INIT;

		اگر (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (نویسه *)پوچ);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ایستا تهی exec_man_konqueror(ثابت نویسه *path, ثابت نویسه *page)
{
	ثابت نویسه *display = getenv("DISPLAY");
	اگر (display && *display) {
		ساختار strbuf man_page = STRBUF_INIT;
		ثابت نویسه *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		اگر (path) {
			size_t len;
			اگر (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (صحیح)len, path);
			filename = basename((نویسه *)path);
		} وگرنه
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (نویسه *)پوچ);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ایستا تهی exec_man_man(ثابت نویسه *path, ثابت نویسه *page)
{
	اگر (!path)
		path = "man";
	execlp(path, "man", page, (نویسه *)پوچ);
	warning_errno(_("failed to exec '%s'"), path);
}

ایستا تهی exec_man_cmd(ثابت نویسه *cmd, ثابت نویسه *page)
{
	ساختار strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (نویسه *)پوچ);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ایستا تهی add_man_viewer(ثابت نویسه *name)
{
	ساختار man_viewer_list **p = &man_viewer_list;

	وقتی (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

ایستا صحیح supported_man_viewer(ثابت نویسه *name, size_t len)
{
	بازگشت (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ایستا تهی do_add_man_viewer_info(ثابت نویسه *name,
				   size_t len,
				   ثابت نویسه *مقدار)
{
	ساختار man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(مقدار);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ایستا صحیح add_man_viewer_path(ثابت نویسه *name,
			       size_t len,
			       ثابت نویسه *مقدار)
{
	اگر (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, مقدار);
	وگرنه
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	بازگشت 0;
}

ایستا صحیح add_man_viewer_cmd(ثابت نویسه *name,
			      size_t len,
			      ثابت نویسه *مقدار)
{
	اگر (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	وگرنه
		do_add_man_viewer_info(name, len, مقدار);

	بازگشت 0;
}

ایستا صحیح add_man_viewer_info(ثابت نویسه *var, ثابت نویسه *مقدار)
{
	ثابت نویسه *name, *subkey;
	size_t namelen;

	اگر (parse_config_کلید(var, "man", &name, &namelen, &subkey) < 0 || !name)
		بازگشت 0;

	اگر (!strcmp(subkey, "path")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		بازگشت add_man_viewer_path(name, namelen, مقدار);
	}
	اگر (!strcmp(subkey, "cmd")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		بازگشت add_man_viewer_cmd(name, namelen, مقدار);
	}

	بازگشت 0;
}

ایستا صحیح git_help_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			   ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "help.format")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		help_format = parse_help_format(مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "help.htmlpath")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		آزاد(html_path);
		html_path = xstrdup(مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "man.viewer")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		add_man_viewer(مقدار);
		بازگشت 0;
	}
	اگر (starts_with(var, "man."))
		بازگشت add_man_viewer_info(var, مقدار);

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا ساختار cmdnames main_cmds, other_cmds;

ایستا صحیح is_git_command(ثابت نویسه *s)
{
	اگر (is_builtin(s))
		بازگشت 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	بازگشت is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

ایستا ثابت نویسه *cmd_to_page(ثابت نویسه *git_cmd)
{
	اگر (!git_cmd)
		بازگشت "git";
	وگرنه اگر (starts_with(git_cmd, "git"))
		بازگشت git_cmd;
	وگرنه اگر (is_git_command(git_cmd))
		بازگشت xstrfmt("git-%s", git_cmd);
	وگرنه اگر (!strcmp("scalar", git_cmd))
		بازگشت xstrdup(git_cmd);
	وگرنه
		بازگشت xstrfmt("git%s", git_cmd);
}

ایستا تهی setup_man_path(تهی)
{
	ساختار strbuf new_path = STRBUF_INIT;
	ثابت نویسه *old_path = getenv("MANPATH");
	نویسه *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	اگر (old_path)
		strbuf_addstr(&new_path, old_path);

	آزاد(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

ایستا تهی exec_viewer(ثابت نویسه *name, ثابت نویسه *page)
{
	ثابت نویسه *info = get_man_viewer_info(name);

	اگر (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	وگرنه اگر (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	وگرنه اگر (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	وگرنه اگر (info)
		exec_man_cmd(info, page);
	وگرنه
		warning(_("'%s': unknown man viewer."), name);
}

ایستا تهی show_man_page(ثابت نویسه *page)
{
	ساختار man_viewer_list *viewer;
	ثابت نویسه *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	برای (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	اگر (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

ایستا تهی show_info_page(ثابت نویسه *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (نویسه *)پوچ);
	die(_("no info viewer handled the request"));
}

ایستا تهی get_html_page_path(ساختار strbuf *page_path, ثابت نویسه *page)
{
	ساختار stat st;
	ثابت نویسه *path = html_path;
	نویسه *to_free = پوچ;

	اگر (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	اگر (!strstr(path, "://")) {
		اگر (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	آزاد(to_free);
}

ایستا تهی open_html(ثابت نویسه *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (نویسه *)پوچ);
}

ایستا تهی show_html_page(ثابت نویسه *page)
{
	ساختار strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

ایستا نویسه *check_git_cmd(ثابت نویسه *cmd)
{
	نویسه *alias;

	اگر (is_git_command(cmd))
		بازگشت xstrdup(cmd);

	alias = alias_lookup(cmd);
	اگر (alias) {
		ثابت نویسه **argv;
		صحیح شمار;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		اگر (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			آزاد(alias);
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
		شمار = split_cmdline(alias, &argv);
		اگر (شمار < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(شمار));
		آزاد(argv);
		بازگشت alias;
	}

	اگر (exclude_guides)
		بازگشت help_unknown_cmd(cmd);

	بازگشت xstrdup(cmd);
}

ایستا تهی no_help_format(ثابت نویسه *opt_mode, شمارش help_format fmt)
{
	ثابت نویسه *opt_fmt;

	انتخاب (fmt) {
	حالت HELP_FORMAT_NONE:
		بازگشت;
	حالت HELP_FORMAT_MAN:
		opt_fmt = "--man";
		بشکن;
	حالت HELP_FORMAT_INFO:
		opt_fmt = "--info";
		بشکن;
	حالت HELP_FORMAT_WEB:
		opt_fmt = "--web";
		بشکن;
	پیشفرض:
		BUG("unreachable");
	}

	usage_پیام_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

ایستا تهی opt_mode_usage(صحیح argc, ثابت نویسه *opt_mode,
			   شمارش help_format fmt)
{
	اگر (argc)
		usage_پیام_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

صحیح cmd_help(صحیح argc,
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repo UNUSED)
{
	صحیح nongit;
	شمارش help_format parsed_help_format;
	نویسه *command = پوچ;
	ثابت نویسه *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	اگر (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_پیام_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	انتخاب (cmd_mode) {
	حالت HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		اگر (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			بازگشت 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		بشکن;
	حالت HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		بازگشت 0;
	حالت HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		بازگشت 0;
	حالت HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		بازگشت 0;
	حالت HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		بازگشت 0;
	حالت HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		بازگشت 0;
	حالت HELP_ACTION_ALIASES_FOR_COMPLETION: {
		ساختار string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		برای (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.اقلام[i].string, '\n',
			       (نویسه *)alias_list.اقلام[i].util, '\0');
		string_list_clear(&alias_list, 1);
		بازگشت 0;
	}
	حالت HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		بازگشت 0;
	}

	اگر (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		بازگشت 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, پوچ);

	اگر (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	اگر (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	انتخاب (help_format) {
	حالت HELP_FORMAT_NONE:
	حالت HELP_FORMAT_MAN:
		show_man_page(page);
		بشکن;
	حالت HELP_FORMAT_INFO:
		show_info_page(page);
		بشکن;
	حالت HELP_FORMAT_WEB:
		show_html_page(page);
		بشکن;
	}

	آزاد(command);
	بازگشت 0;
}
