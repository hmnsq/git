/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "exec-cmd.h"
#ضمن "gettext.h"
#ضمن "pager.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "run-command.h"
#ضمن "config-list.h"
#ضمن "help.h"
#ضمن "alias.h"
#ضمن "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

ساكن بنية man_viewer_list {
	بنية man_viewer_list *next;
	حرف name[FLEX_ARRAY];
} *man_viewer_list;

ساكن بنية man_viewer_info_list {
	بنية man_viewer_info_list *next;
	ثابت حرف *info;
	حرف name[FLEX_ARRAY];
} *man_viewer_info_list;

تعداد help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

تعداد show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ساكن تعداد help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

ساكن حرف *html_path;
ساكن صحيح verbose = 1;
ساكن تعداد help_format help_format = HELP_FORMAT_NONE;
ساكن صحيح exclude_guides;
ساكن صحيح show_external_commands = -1;
ساكن صحيح show_aliases = -1;
ساكن بنية option builtin_help_options[] = {
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

ساكن ثابت حرف * ثابت builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	عدم
};

بنية slot_expansion {
	ثابت حرف *prefix;
	ثابت حرف *placeholder;
	فراغ (*fn)(بنية string_list *list, ثابت حرف *prefix);
	صحيح found;
};

ساكن فراغ set_config_vars(بنية string_list *کلیلەکان_uniq, بنية string_list_item *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *wildcard = strchr(str, '*');
	ثابت حرف *tag = strchr(str, '<');
	ثابت حرف *cut;

	إذا (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	وإلا إذا (wildcard)
		cut = wildcard;
	وإلا إذا (tag)
		cut = tag;
	وإلا {
		string_list_append(کلیلەکان_uniq, str);
		أرجع;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(کلیلەکان_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ set_config_sections(بنية string_list *کلیلەکان_uniq, بنية string_list_item *var)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *str = var->string;
	ثابت حرف *dot = strchr(str, '.');
	ثابت حرف *cut;

	إذا (dot)
		cut = dot;
	وإلا {
		set_config_vars(کلیلەکان_uniq, var);
		أرجع;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(کلیلەکان_uniq, sb.buf);
	strbuf_release(&sb);
}

ساكن فراغ list_config_help(تعداد show_config_type type)
{
	بنية slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_پەیام_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_پەیام_ids },
		{ عدم, عدم, عدم }
	};
	ثابت حرف **p;
	بنية slot_expansion *e;
	بنية string_list کلیلەکان = STRING_LIST_INIT_DUP;
	بنية string_list کلیلەکان_uniq = STRING_LIST_INIT_DUP;
	بنية string_list_item *item;
	بنية strbuf sb = STRBUF_INIT;

	لكل (p = config_name_list; *p; p++) {
		ثابت حرف *var = *p;

		لكل (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			إذا (!strcasecmp(var, sb.buf)) {
				e->fn(&کلیلەکان, e->prefix);
				e->found++;
				اكسر;
			}
		}

		إذا (!e->prefix)
			string_list_append(&کلیلەکان, var);
	}

	strbuf_release(&sb);

	لكل (e = slot_expansions; e->prefix; e++)
		إذا (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	لكل (size_t i = 0; i < کلیلەکان.nr; i++) {
		بدل (type) {
		حالة SHOW_CONFIG_HUMAN:
			string_list_append(&کلیلەکان_uniq, کلیلەکان.کاڵاکان[i].string);
			اكسر;
		حالة SHOW_CONFIG_SECTIONS:
			set_config_sections(&کلیلەکان_uniq, &کلیلەکان.کاڵاکان[i]);
			اكسر;
		حالة SHOW_CONFIG_VARS:
			set_config_vars(&کلیلەکان_uniq, &کلیلەکان.کاڵاکان[i]);
			اكسر;
		افتراضي:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&کلیلەکان_uniq, 0);
	for_each_string_list_item(item, &کلیلەکان_uniq)
		puts(item->string);
	string_list_clear(&کلیلەکان_uniq, 0);
	string_list_clear(&کلیلەکان, 0);
}

ساكن تعداد help_format parse_help_format(ثابت حرف *format)
{
	إذا (!strcmp(format, "man"))
		أرجع HELP_FORMAT_MAN;
	إذا (!strcmp(format, "info"))
		أرجع HELP_FORMAT_INFO;
	إذا (!strcmp(format, "web") || !strcmp(format, "html"))
		أرجع HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ساكن ثابت حرف *get_man_viewer_info(ثابت حرف *name)
{
	بنية man_viewer_info_list *viewer;

	لكل (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		إذا (!strcasecmp(name, viewer->name))
			أرجع viewer->info;
	}
	أرجع عدم;
}

ساكن صحيح check_emacsclient_version(فراغ)
{
	بنية strbuf buffer = STRBUF_INIT;
	بنية child_process ec_process = CHILD_PROCESS_INIT;
	صحيح version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", عدم);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	إذا (start_command(&ec_process))
		أرجع error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	إذا (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		أرجع error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	إذا (version < 22) {
		strbuf_release(&buffer);
		أرجع error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	أرجع 0;
}

ساكن فراغ exec_woman_emacs(ثابت حرف *path, ثابت حرف *page)
{
	إذا (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		بنية strbuf man_page = STRBUF_INIT;

		إذا (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ساكن فراغ exec_man_konqueror(ثابت حرف *path, ثابت حرف *page)
{
	ثابت حرف *display = getenv("DISPLAY");
	إذا (display && *display) {
		بنية strbuf man_page = STRBUF_INIT;
		ثابت حرف *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		إذا (path) {
			size_t len;
			إذا (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (صحيح)len, path);
			filename = basename((حرف *)path);
		} وإلا
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (حرف *)عدم);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ساكن فراغ exec_man_man(ثابت حرف *path, ثابت حرف *page)
{
	إذا (!path)
		path = "man";
	execlp(path, "man", page, (حرف *)عدم);
	warning_errno(_("failed to exec '%s'"), path);
}

ساكن فراغ exec_man_cmd(ثابت حرف *cmd, ثابت حرف *page)
{
	بنية strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (حرف *)عدم);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ساكن فراغ add_man_viewer(ثابت حرف *name)
{
	بنية man_viewer_list **p = &man_viewer_list;

	طالما (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

ساكن صحيح supported_man_viewer(ثابت حرف *name, size_t len)
{
	أرجع (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ساكن فراغ do_add_man_viewer_info(ثابت حرف *name,
				   size_t len,
				   ثابت حرف *بەها)
{
	بنية man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(بەها);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ساكن صحيح add_man_viewer_path(ثابت حرف *name,
			       size_t len,
			       ثابت حرف *بەها)
{
	إذا (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, بەها);
	وإلا
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	أرجع 0;
}

ساكن صحيح add_man_viewer_cmd(ثابت حرف *name,
			      size_t len,
			      ثابت حرف *بەها)
{
	إذا (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	وإلا
		do_add_man_viewer_info(name, len, بەها);

	أرجع 0;
}

ساكن صحيح add_man_viewer_info(ثابت حرف *var, ثابت حرف *بەها)
{
	ثابت حرف *name, *subkey;
	size_t namelen;

	إذا (parse_config_کلیل(var, "man", &name, &namelen, &subkey) < 0 || !name)
		أرجع 0;

	إذا (!strcmp(subkey, "path")) {
		إذا (!بەها)
			أرجع config_error_nonbool(var);
		أرجع add_man_viewer_path(name, namelen, بەها);
	}
	إذا (!strcmp(subkey, "cmd")) {
		إذا (!بەها)
			أرجع config_error_nonbool(var);
		أرجع add_man_viewer_cmd(name, namelen, بەها);
	}

	أرجع 0;
}

ساكن صحيح git_help_config(ثابت حرف *var, ثابت حرف *بەها,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "help.format")) {
		إذا (!بەها)
			أرجع config_error_nonbool(var);
		help_format = parse_help_format(بەها);
		أرجع 0;
	}
	إذا (!strcmp(var, "help.htmlpath")) {
		إذا (!بەها)
			أرجع config_error_nonbool(var);
		حرر(html_path);
		html_path = xstrdup(بەها);
		أرجع 0;
	}
	إذا (!strcmp(var, "man.viewer")) {
		إذا (!بەها)
			أرجع config_error_nonbool(var);
		add_man_viewer(بەها);
		أرجع 0;
	}
	إذا (starts_with(var, "man."))
		أرجع add_man_viewer_info(var, بەها);

	أرجع git_default_config(var, بەها, ctx, cb);
}

ساكن بنية cmdnames main_cmds, other_cmds;

ساكن صحيح is_git_command(ثابت حرف *s)
{
	إذا (is_builtin(s))
		أرجع 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	أرجع is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

ساكن ثابت حرف *cmd_to_page(ثابت حرف *git_cmd)
{
	إذا (!git_cmd)
		أرجع "git";
	وإلا إذا (starts_with(git_cmd, "git"))
		أرجع git_cmd;
	وإلا إذا (is_git_command(git_cmd))
		أرجع xstrfmt("git-%s", git_cmd);
	وإلا إذا (!strcmp("scalar", git_cmd))
		أرجع xstrdup(git_cmd);
	وإلا
		أرجع xstrfmt("git%s", git_cmd);
}

ساكن فراغ setup_man_path(فراغ)
{
	بنية strbuf new_path = STRBUF_INIT;
	ثابت حرف *old_path = getenv("MANPATH");
	حرف *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	إذا (old_path)
		strbuf_addstr(&new_path, old_path);

	حرر(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

ساكن فراغ exec_viewer(ثابت حرف *name, ثابت حرف *page)
{
	ثابت حرف *info = get_man_viewer_info(name);

	إذا (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	وإلا إذا (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	وإلا إذا (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	وإلا إذا (info)
		exec_man_cmd(info, page);
	وإلا
		warning(_("'%s': unknown man viewer."), name);
}

ساكن فراغ show_man_page(ثابت حرف *page)
{
	بنية man_viewer_list *viewer;
	ثابت حرف *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	لكل (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	إذا (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

ساكن فراغ show_info_page(ثابت حرف *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (حرف *)عدم);
	die(_("no info viewer handled the request"));
}

ساكن فراغ get_html_page_path(بنية strbuf *page_path, ثابت حرف *page)
{
	بنية stat st;
	ثابت حرف *path = html_path;
	حرف *to_free = عدم;

	إذا (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	إذا (!strstr(path, "://")) {
		إذا (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	حرر(to_free);
}

ساكن فراغ open_html(ثابت حرف *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (حرف *)عدم);
}

ساكن فراغ show_html_page(ثابت حرف *page)
{
	بنية strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

ساكن حرف *check_git_cmd(ثابت حرف *cmd)
{
	حرف *alias;

	إذا (is_git_command(cmd))
		أرجع xstrdup(cmd);

	alias = alias_lookup(cmd);
	إذا (alias) {
		ثابت حرف **argv;
		صحيح ژماردن;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		إذا (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			حرر(alias);
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
		ژماردن = split_cmdline(alias, &argv);
		إذا (ژماردن < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(ژماردن));
		حرر(argv);
		أرجع alias;
	}

	إذا (exclude_guides)
		أرجع help_unknown_cmd(cmd);

	أرجع xstrdup(cmd);
}

ساكن فراغ no_help_format(ثابت حرف *opt_mode, تعداد help_format fmt)
{
	ثابت حرف *opt_fmt;

	بدل (fmt) {
	حالة HELP_FORMAT_NONE:
		أرجع;
	حالة HELP_FORMAT_MAN:
		opt_fmt = "--man";
		اكسر;
	حالة HELP_FORMAT_INFO:
		opt_fmt = "--info";
		اكسر;
	حالة HELP_FORMAT_WEB:
		opt_fmt = "--web";
		اكسر;
	افتراضي:
		BUG("unreachable");
	}

	usage_پەیام_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

ساكن فراغ opt_mode_usage(صحيح argc, ثابت حرف *opt_mode,
			   تعداد help_format fmt)
{
	إذا (argc)
		usage_پەیام_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

صحيح cmd_help(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *prefix,
	     بنية repository *repo UNUSED)
{
	صحيح nongit;
	تعداد help_format parsed_help_format;
	حرف *command = عدم;
	ثابت حرف *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	إذا (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_پەیام_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	بدل (cmd_mode) {
	حالة HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		إذا (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			أرجع 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		اكسر;
	حالة HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		أرجع 0;
	حالة HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		أرجع 0;
	حالة HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		أرجع 0;
	حالة HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		أرجع 0;
	حالة HELP_ACTION_ALIASES_FOR_COMPLETION: {
		بنية string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		لكل (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.کاڵاکان[i].string, '\n',
			       (حرف *)alias_list.کاڵاکان[i].util, '\0');
		string_list_clear(&alias_list, 1);
		أرجع 0;
	}
	حالة HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		أرجع 0;
	}

	إذا (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		أرجع 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, عدم);

	إذا (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	إذا (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	بدل (help_format) {
	حالة HELP_FORMAT_NONE:
	حالة HELP_FORMAT_MAN:
		show_man_page(page);
		اكسر;
	حالة HELP_FORMAT_INFO:
		show_info_page(page);
		اكسر;
	حالة HELP_FORMAT_WEB:
		show_html_page(page);
		اكسر;
	}

	حرر(command);
	أرجع 0;
}
